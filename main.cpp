inline constexpr const char* K_IGNORE = "ignore";
inline constexpr const char* K_IGNORE_FROM = "ignore-from";
inline constexpr const char* K_MODE = "mode";
inline constexpr const char* K_ROOT = "root";
inline constexpr const char* K_USE_REPO_CONFIG = "use-repo-config";

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <string>
#include <filesystem>
#include <regex>

#include <fstream>

#include "client/parse_flags.hpp"
//#include "test_repurposing/tests.hpp"

#include <sstream>
#include <iomanip>

using namespace std;
using namespace filesystem;
//using namespace test_repurposing;

bool isIgnored(const path& p, const vector<string>& ignorePatterns) {
	string pathStr = p.string();
	for (const string& pattern : ignorePatterns) {
		// Simple conversion: escape backslashes and turn '*' into '.*'
		string regexPattern = pattern;
		size_t pos = 0;
		while ((pos = regexPattern.find("\\", pos)) != string::npos) {
			regexPattern.replace(pos, 1, "\\\\");
			pos += 2;
		}

		// Replace globs with regex equivalent
		pos = 0;
		while ((pos = regexPattern.find("*", pos)) != string::npos) {
			regexPattern.replace(pos, 1, ".*");
			pos += 2;
		}

		try {
			if (regex_match(pathStr, regex(regexPattern, regex_constants::icase))) {
				return true;
			}
		}
		catch (...) { continue; }
	}
	return false;
}

vector<string> getContentFromFile(const path& fileRoot) {
	vector<string> lines;

	// 1. Open the file using the provided path
	ifstream file(fileRoot);

	// 2. Check if the file actually opened (exists and has permissions)
	if (!file.is_open()) {
		// Return an empty vector or handle error as needed
		return lines;
	}

	string currentLine;
	// 3. Read the file line by line until the end (EOF)
	while (getline(file, currentLine)) {
		// 4. Add each line to our vector
		lines.push_back(currentLine);
	}

	file.close();

	return lines;
}

struct DirectoryTree {
	string name;
	path fullPath;
	bool isDirectory, isActivated = false;

	vector<DirectoryTree> children;
	vector<string> metadata;

	DirectoryTree(const path& rootPath) {
		this->fullPath = rootPath;
		this->name = rootPath.filename().string();
		this->isDirectory = is_directory(rootPath);
	}

	void activate(const vector<string>& ignorePatterns) {
		if (isActivated || !isDirectory) return;

		try {
			for (const auto& entry : directory_iterator(fullPath)) {
				if (isIgnored(entry.path(), ignorePatterns)) continue;

				// Create child
				children.emplace_back(entry.path());
			}
			isActivated = true;
		}
		catch (...) { /* Handle permission errors */ }
	}

	void print(const vector<string>& ignorePatterns, stringstream& ss, int depth = 1) {
		// 1. Calculate the base indentation
		// If depth is 1 (root), we use 0 spaces. If deeper, we use (depth-1)*2
		string indent = (depth > 1) ? string((depth - 1) * 2, ' ') : "";

		// 2. Print the tree branch and name
		ss << indent << "|_" << name << "\n";

		// 3. Print the associated list of text (the "metadata")
		// We add an extra 3 spaces to the indent so the text sits 
		// underneath the name, past the "|_" prefix.
		string textIndent = indent + "   ";
		for (const string& info : metadata) {
			ss << textIndent << info << "\n";
		}

		// 4. Recurse into folders
		if (isDirectory) {
			activate(ignorePatterns);
			for (auto& child : children) {
				child.print(ignorePatterns, ss, depth + 1);
			}
		}
	}

	DirectoryTree* findNode(const path& targetPath, const vector<string>& ignorePatterns) {
		try {
			// 1. Exact Match Check
			// Use equivalent to handle different slash types ( / vs \ )
			if (exists(this->fullPath) && exists(targetPath)) {
				if (equivalent(this->fullPath, targetPath)) {
					return this;
				}
			}

			if (this->isDirectory) {
				// 2. Optimization: Only enter if targetPath is "under" this folder
				// We check if targetPath starts with this->fullPath
				string targetStr = targetPath.lexically_normal().string();
				string currentStr = this->fullPath.lexically_normal().string();

				// If target isn't inside this folder, don't bother searching children
				if (targetStr.find(currentStr) == string::npos) {
					return nullptr;
				}

				// 3. Lazy Activation
				this->activate(ignorePatterns);

				// 4. Recurse
				for (auto& child : children) {
					DirectoryTree* found = child.findNode(targetPath, ignorePatterns);
					if (found) return found;
				}
			}
		}
		catch (const filesystem_error& e) {
			// This catches "Access Denied" or "Path Not Found" errors
			return nullptr;
		}

		return nullptr;
	}

	void collectFilesWithExtensions(const vector<string>& ignorePatterns) {
		// 1. If this is a directory, wake it up to see its children
		if (this->isDirectory) {
			this->activate(ignorePatterns);

			for (auto& child : children) {
				// Recurse into the child
				child.collectFilesWithExtensions(ignorePatterns);
			}
		}
		// 2. If it's a file, check if it has an extension
		else {
			if (this->fullPath.has_extension()) {
				// 3. Populate this specific node's metadata with an empty static vector
				vector<string> fileMetadata = getContentFromFile(this->fullPath);
				this->metadata = fileMetadata;
			}
		}
	}
};

int main(int argc, char* argv[]) {
	//AssignedTests::runTests();

	if (argc > 1) {
		using namespace client;
		Flags conf = parseAndInstallFlags(argc, argv);

		if (conf.has(K_ROOT)) {
			const path rootPath(conf.flags[K_ROOT][0]);

			// 1. Get the Local AppData path (No Admin required)
			char* localAppData = nullptr;
			size_t localLen = 0;
			_dupenv_s(&localAppData, &localLen, "LOCALAPPDATA");

			// 2b. Get ready for the ignore paths
			vector<string> ignorePaths;

			// 3. Assemble the ignore paths
			if (conf.has(K_IGNORE_FROM)) {
				try {
					// 1. Get the list of fragments (e.g., {"C:\Users\...", "of", "Duty\ignore.txt"})
					vector<string> fragments = conf.get(K_IGNORE_FROM);
					string fullPathStr = "";

					// 2. Re-assemble them with spaces
					for (size_t i = 0; i < fragments.size(); ++i) {
						fullPathStr += fragments[i];
						if (i < fragments.size() - 1) fullPathStr += " ";
					}

					// 3. Clean and convert to path
					fullPathStr.erase(remove(fullPathStr.begin(), fullPathStr.end(), '\"'), fullPathStr.end());
					path includeFromFile(fullPathStr);

					if (exists(includeFromFile)) {
						ignorePaths = getContentFromFile(includeFromFile);
					}
					else {
						cerr << "Warning: Include file not found at " << includeFromFile << endl;
					}
				}
				catch (const exception& e) {
					cerr << "Path error: " << e.what() << endl;
				}
			}
			else if (conf.has(K_IGNORE)) {
				ignorePaths = conf.get(K_IGNORE);
			}
			else if (conf.has(K_USE_REPO_CONFIG)) {
				//create path for .repoignore
				path installDir;
				if (localAppData != nullptr) {
					installDir = path(localAppData) / "dir2md";
					free(localAppData);
				}
				else {
					installDir = current_path() / "config";
				}

				path ignoreFilePath = installDir / ".repoignore";

				// 2. Create the directory if it doesn't exist
				try {
					if (!exists(installDir)) {
						create_directories(installDir);
					}
				}
				catch (const filesystem_error& e) {
					// Final fallback to current directory if AppData is locked
					installDir = current_path();
					ignoreFilePath = installDir / ".repoignore";
				}

				// 3. Generate the file if it doesn't exist
				if (!exists(ignoreFilePath)) {
					ofstream ignoreFile(ignoreFilePath);
					if (ignoreFile.is_open()) {
						// Write the default ignore values line-by-line
						ignorePaths = { ".git/*", "build/*", "node_modules/*" };

						ofstream ignoreFile(ignoreFilePath);
						if (ignoreFile.is_open()) {
							for (const string& ip : ignorePaths) {
								ignoreFile << ip << endl;
							}
							ignoreFile.close();
						}
					}
				}
				else {
					ignorePaths = getContentFromFile(ignoreFilePath);
				}
			}

			//1. extract the ignore paths and assemble each of them with the root path
			vector<string> preassembledIgnorePaths;
			for (const string& ignorePath : ignorePaths) {
				string ipConverted = ignorePath;

				//swap out instances of '/' with '\\'
				size_t slashIndex = ipConverted.find('/');
				while (slashIndex != string::npos) {
					replace(ipConverted.begin() + slashIndex, ipConverted.begin() + slashIndex + 1,
						'/', '\\');
					slashIndex = ipConverted.find('/');
				}

				ipConverted = rootPath.string() + '\\' + ipConverted;
				preassembledIgnorePaths.push_back(ipConverted);
			}

			DirectoryTree rootTree(rootPath);
			stringstream ss;

			//Start the traversal from the root to assign metadata
			//rootTree.collectFilesWithExtensions(preassembledIgnorePaths);

			rootTree.print(preassembledIgnorePaths, ss);

			string finalOutput = ss.str();
			cout << finalOutput << endl;
		} else
			throw "You must specify a root path with the --root flag.";
	}

	return 0;
}