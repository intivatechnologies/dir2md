#include "folder_rep.hpp"
#include "../misc/get_root_name.hpp"

#include <filesystem>

using namespace file_records;

const vector<string> FolderRep::disincludeFolders = { ".git", "build" };

//takes a root path and lists all nested root paths
//on each nested path that is a folder, the function is called recursively
//on each nested path that is a file, a custom operation is performed
FolderRep* FolderRep::installFolderAtRoot(string root) {
	vector<FolderRecord*> folderChildren;
	const string name = getRootName(root);

	bool disincludeFolder = false;
	for (string disincluded : disincludeFolders)
		if(name == disincluded) {
			disincludeFolder = true;
			break;
		}

	if(!disincludeFolder)
		for (const auto& entry : filesystem::directory_iterator(root)) {
			filesystem::path path = entry.path();
			if (entry.is_regular_file())
				folderChildren.push_back(new FileRep(path.filename().string(), path.string(),
					path.extension().string()));
			else
				folderChildren.push_back(installFolderAtRoot(path.string()));
		}

	return new FolderRep(name, root, folderChildren);
}

void FolderRep::loadAllRootsByExtension(vector<string>& roots, FolderRep* parentFolder,
const string ext) {
	for (const auto& child : parentFolder->getChildren()) {
		if(child->recordClassifier == FolderRecord::RecordClassifier::FILE) {
			FileRep* sameChild = static_cast<FileRep*>(child);
			if (sameChild->getExtension() == ext)
				roots.push_back(sameChild->getRoot());
		} else 
			loadAllRootsByExtension(roots, static_cast<FolderRep*>(child), ext);
	}
}