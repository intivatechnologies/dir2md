#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

#include "misc/get_root_name.hpp"

using namespace std;

static const string TAB_SPACE = "  ";
static string loadedTabSequence = "";
static vector<string> dontIncludeFolders = { ".git", "build" };

void traverseRoot(string& buildTree, string root) {
	for(const auto& disincludedFolder : dontIncludeFolders)
		if(getRootName(root) == disincludedFolder)
			return;

	loadedTabSequence += TAB_SPACE;
	for (const auto& entry : filesystem::directory_iterator(root)) {
		buildTree += loadedTabSequence + getRootName(entry.path().string()) + '\n';
		if (entry.is_directory())
			traverseRoot(buildTree, entry.path().string());
	}
	loadedTabSequence = loadedTabSequence
		.substr(0, loadedTabSequence.size() - TAB_SPACE.size());
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		//then we have a root path to work with
		string buildTree = getRootName(argv[1]) + '\n';
		traverseRoot(buildTree, argv[1]);

		cout << buildTree << endl;
	}

	return 0;
}