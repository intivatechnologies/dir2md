#include "directory_tree.hpp"
#include "../misc/get_root_name.hpp"

#include <filesystem>

using namespace directory_tree;
using namespace std::filesystem;

FolderRep* DirectoryTree::getFolderAtRoot(string root) {
	vector<FileRep*> rootChildren;

	for (const auto& entry : directory_iterator(root)) {
		if (entry.is_regular_file())
			rootChildren.push_back(new FileRep(entry.path().filename().string()));
		else
			rootChildren.push_back(getFolderAtRoot(entry.path().string()));
	}

	return new FolderRep(getRootName(root), rootChildren);
}

string DirectoryTree::startAt(string root) {
	FolderRep* rootFolder = getFolderAtRoot(root);
	string textualRep = "";
	int indentationAmount = 0;

	const auto getIndentationVolume = [&indentationAmount]() {
		string indentationVolume = "";
		for (int i = 0; i < indentationAmount; i++)
			indentationVolume += '\t';
		return indentationVolume;
	};

	for (const auto& entry : rootFolder->getChildren()) {
		if (entry != nullptr) {
			string indentationVolume = getIndentationVolume();
			if (dynamic_cast<FolderRep*>(entry) != nullptr) {
				vector<FileRep*> entryChildren = static_cast<FolderRep*>(entry)->getChildren();
				textualRep += indentationVolume + entry->getName() + '\n';

				++indentationAmount;
				indentationVolume = getIndentationVolume();

				for (const auto& entryChild : entryChildren) {
					if(entryChild != nullptr)
						textualRep += indentationVolume + entryChild->getName() + '\n';
				}

				--indentationAmount;
			}
			else
				textualRep += indentationVolume + entry->getName() + '\n';
		}
	}

	return textualRep;
}