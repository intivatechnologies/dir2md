#define K_INCLUDE_EXT "include-ext"
#define K_EXCLUDE_DIR "exclude-dir"
#define K_MODE "mode"
#define K_ROOT "root"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>

#include "file_records/filesystem_node.hpp"
#include "traversals/traversals_content_roots.hpp"
#include "traversals/traversals_string_tree.hpp"
#include "misc/get_content_from_files.hpp"
#include "misc/get_root_name.hpp"

using namespace std;
using namespace file_records;

void throwErr(string reasonInp) {
	throw "Your input failed to specify " + reasonInp + ".";
}

struct Config {
	map<string, vector<string>> flags;

	enum ModeFlag {
		MF_NONE = 0,
		MF_STRUCTURE = 1 << 0,
		MF_CONTENT = 1 << 1
	} MODE_FLAG = ModeFlag::MF_NONE;

	void reset() {
		flags.clear();
		MODE_FLAG = MF_NONE;
	}
};

//populate static map flags 'flags' with all flags and tags
Config parseFlags(const int FLAGS_INDEX, int argc, char* argv[]) {
	Config conf;

	string currentFlag = "";
	for (int i = FLAGS_INDEX; i < argc; i++) {
		string token = argv[i];
		if (token == "\\")
			continue;

		//process for --flags
		if (token.rfind("--", 0) == 0)
		{
			currentFlag = token.substr(2);
			conf.flags[currentFlag]; // ensure key exists
		}

		//process for tags
		else
			conf.flags[currentFlag].push_back(token);
	}

	return conf;
}

//assign tags and refine where needed
void installFlags(Config& conf, const map<string, vector<string>>::iterator FLAG_INCLUDE_EXTENSIONS_ITER) {
	//ensure file extensions have a dot char at the beginning
	if (FLAG_INCLUDE_EXTENSIONS_ITER != conf.flags.end())
		for (auto& tag : FLAG_INCLUDE_EXTENSIONS_ITER->second) {
			if (tag[0] != '.')
				tag.insert(tag.begin(), '.');
		}

	//extract one or modes into K_MODE
	auto modeIter = conf.flags.find(K_MODE);
	if (modeIter != conf.flags.end())
		for (auto& m : modeIter->second) {
			if (m == "structure")
				conf.MODE_FLAG = (Config::ModeFlag)(conf.MODE_FLAG | conf.MF_STRUCTURE);

			if (m == "content")
				conf.MODE_FLAG = (Config::ModeFlag)(conf.MODE_FLAG | conf.MF_CONTENT);
		}

	//print all flags and tags
	for (const auto& pair : conf.flags) {
		cout << "--" << pair.first;
		for (const auto& tag : conf.flags.at(pair.first))
			cout << ' ' << tag;
		cout << endl;
	}
	cout << endl;
}

void runListTreeCheck(Config& conf, FilesystemNode& rootNode) {
	if (conf.MODE_FLAG & conf.MF_STRUCTURE) {
		//list the tree
		string tree = rootNode.name + '\n';
		traverseStringTree(tree, rootNode);
		cout << "> PROJECT STRUCTURE:" << endl << tree << endl;
	}
}

bool runContentExtensionsCheckValidation(Config& conf, const map<string, vector<string>>
::iterator FLAG_INCLUDE_EXTENSIONS_ITER) {
	if (FLAG_INCLUDE_EXTENSIONS_ITER != conf.flags.end()
		&& FLAG_INCLUDE_EXTENSIONS_ITER->second.size() > 0) {
		cout << "> CONTENT EXTENSIONS:" << endl;
		for (auto& contentExtension : FLAG_INCLUDE_EXTENSIONS_ITER->second)
			cout << '-' << contentExtension << endl;

		return true;
	}
	else {
		throwErr("file extensions to dedicate for extraction");
		return false;
	}
}

int main(int argc, char* argv[]) {
	const int FLAGS_INDEX = 1;

	if (argc > FLAGS_INDEX) {
		Config conf = parseFlags(FLAGS_INDEX, argc, argv);

		const auto FLAG_INCLUDE_EXTENSIONS_ITER = conf.flags.find(K_INCLUDE_EXT);
		installFlags(conf, FLAG_INCLUDE_EXTENSIONS_ITER);

		auto rootIter = conf.flags.find(K_ROOT);
		if (rootIter != conf.flags.end()) {
			//then we have a root path to work with
			filesystem::path rootPath(rootIter->second.at(0));
			filesystem::directory_entry rootEntry(rootPath);

			//build the tree
			FilesystemNode rootNode(&rootEntry);
			rootNode.buildOut(conf.flags.at(K_EXCLUDE_DIR));
			runListTreeCheck(conf, rootNode);

			if (runContentExtensionsCheckValidation(conf, FLAG_INCLUDE_EXTENSIONS_ITER)
			&& (conf.MODE_FLAG & conf.MF_CONTENT)) {
				//list all roots with extensions that are dedicated to content extraction
				vector<string> contentRoots;
				traverseContentRootsByExtension(contentRoots, rootNode, FLAG_INCLUDE_EXTENSIONS_ITER->second);

				cout << endl << "> CONTENT ROOTS:" << endl;
				for (string contentRoot : contentRoots)
					cout << ">> " << contentRoot << endl;

				/*
				vector<string> contents = getContentFromFiles(contentRoots);

				cout << endl << "> CONTENTS:" << endl;
				for (int i = 0; i < contentRoots.size(); i++) {
					cout << ">> " << contentRoots[i] << endl;
					cout << contents[i] << endl << endl;
				}
				*/
			}
		}
	}

	return 0;
}