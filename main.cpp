constexpr const char* K_IGNORE = "ignore";
constexpr const char* K_MODE = "mode";
constexpr const char* K_ROOT = "root";

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
#include "client/parse_flags.hpp"
#include "test_repurposing/tests.hpp"

using namespace std;
using namespace file_records;
using namespace client;
using namespace test_repurposing;

void presentStructureTreeIfPrompted(Flags& conf, FilesystemNode& rootNode) {
	if (conf.MODE_FLAG & conf.MF_STRUCTURE) {
		//list the tree
		string tree = rootNode.name + '\n';
		traverseStringTree(tree, rootNode);
		cout << "> PROJECT STRUCTURE:" << endl << tree << endl;
	}
}

bool presentContentsIfPrompted(Flags& conf, FilesystemNode& rootNode) {
	if (conf.MODE_FLAG & conf.MF_CONTENT) {
		/*
		//list all roots with extensions that are dedicated to content extraction
		vector<string> contentRoots;
		traverseContentRootsByExtension(contentRoots, rootNode, conf.get(K_INCLUDE_EXT));

		cout << endl << "> CONTENT ROOTS:" << endl;
		for (string contentRoot : contentRoots)
			cout << ">> " << contentRoot << endl;
			*/

		/*
		vector<string> contents = getContentFromFiles(contentRoots);

		cout << endl << "> CONTENTS:" << endl;
		for (int i = 0; i < contentRoots.size(); i++) {
			cout << ">> " << contentRoots[i] << endl;
			cout << contents[i] << endl << endl;
		}
		*/

		return true;
	}
	else
		return false;
}

/*
bool checkThatContentExtensionsAreAvailable(Flags& conf) {
	if (conf.get(K_INCLUDE_EXT).size() > 0) {
		cout << "> CONTENT EXTENSIONS:" << endl;
		for (auto& contentExtension : conf.get(K_INCLUDE_EXT))
			cout << '-' << contentExtension << endl;

		return true;
	}
	else {
		TestUtil::throwErr("file extensions to dedicate for extraction");
		return false;
	}
}
*/

int main(int argc, char* argv[]) {
	//AssignedTests::runTests();

	if (argc > 1) {
		Flags conf = parseAndInstallFlags(argc, argv);

		if (conf.has(K_ROOT)) {
			//then we have a root path to work with
			filesystem::path rootPath(conf.flags[K_ROOT][0]);
			filesystem::directory_entry rootEntry(rootPath);

			//build the tree
			FilesystemNode rootNode(rootEntry);
			rootNode.buildOut(conf.get(K_IGNORE));
			presentStructureTreeIfPrompted(conf, rootNode);
			
			//present the contents
			//if (checkThatContentExtensionsAreAvailable(conf) && presentContentsIfPrompted(conf, rootNode));
		} else
			throw "You must specify a root path with the --root flag.";
	}

	return 0;
}