/*
#include <iostream>

#include <vector>
#include <string>
#include <map>

#include <functional>

#include <fstream>
#include <sstream>

#include "file_records/folder_rep.hpp"
#include "directory_tree/directory_tree.hpp"
#include "misc/get_content_from_files.hpp"

using namespace std;
using namespace file_records;
using namespace directory_tree;

static vector<string> includeForFileContents;
static map<string, string> fileContents;

int main(int argc, char* argv[]) {
	if (argc > 1) {
		if (argc > 2) {
			for (int i = 2; i < argc; i++)
				includeForFileContents.push_back(argv[i]);
		}

		FolderRep* assignedFolder = FolderRep::installFolderAtRoot(argv[1]);

		//for (const auto& child : assignedFolder->getChildren())
		//	cout << child->getName() << endl;

		string textualRep = DirectoryTree::getInstance().startAt(assignedFolder);

		cout << "> PROJECT STRUCTURE:" << endl;
		cout << textualRep << endl << endl;

		if (argc > 2) {
			vector<string> allRootsByExtensions;
			for (int i = 2; i < argc; i++)
				FolderRep::loadAllRootsByExtension(allRootsByExtensions, assignedFolder, argv[i]);
			
			cout << "> FILE PATHS TO DISPLAY CONTENTS" << endl;
			for (string rootByExtension : allRootsByExtensions)
				cout << rootByExtension << endl;
			cout << endl;

			vector<string> contentsOfAppliedFiles = getContentFromFiles(allRootsByExtensions);

			for(int i = 0; i < allRootsByExtensions.size(); i++) {
				cout << "> CONTENTS OF \"" << allRootsByExtensions[i] << "\":" << endl;
				cout << contentsOfAppliedFiles[i] << endl << endl;
			}
		}

		return 0;
	}

	cout << "An input directory could not be found." << endl;
	return -1;
}
*/