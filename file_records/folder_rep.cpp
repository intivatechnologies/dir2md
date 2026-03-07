#include "folder_rep.hpp"
#include "../misc/get_root_name.hpp"

#include <filesystem>

using namespace file_records;

const vector<string> FolderRep::disincludeFolders = { ".git", "build" };

FolderRep* FolderRep::installFolderAtRoot(string root) {
	vector<FileRecord*> rootChildren;
	const string name = getRootName(root);

	bool disincludeFolder = false;
	for (string disincluded : disincludeFolders) {
		if(name == disincluded) {
			disincludeFolder = true;
			break;
		}
	}

	if(!disincludeFolder)
		for (const auto& entry : filesystem::directory_iterator(root)) {
			filesystem::path path = entry.path();
			if (entry.is_regular_file())
				rootChildren.push_back(new FileRep(path.filename().string(), path.string(),
					path.extension().string()));
			else
				rootChildren.push_back(installFolderAtRoot(path.string()));
		}

	return new FolderRep(name, root, rootChildren);
}

void FolderRep::loadAllRootsByExtension(vector<string>& roots, FolderRep* parentFolder,
const string ext) {
	for (const auto& child : parentFolder->getChildren()) {
		if (dynamic_cast<FileRep*>(child) != nullptr) {
			FileRep* sameChild = static_cast<FileRep*>(child);
			if (sameChild->getExtension() == ext)
				roots.push_back(sameChild->getRoot());
		}
		else if (dynamic_cast<FolderRep*>(child) != nullptr)
			loadAllRootsByExtension(roots, static_cast<FolderRep*>(child), ext);
	}
}