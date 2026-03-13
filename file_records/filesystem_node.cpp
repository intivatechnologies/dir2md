#include "filesystem_node.hpp"

using namespace file_records;

bool matchesPattern(string pattern, string epfs, string parentEpfs, bool isDirectory) {
    // Pattern ends with '/*' -> match contents of a folder (applied at parent level, skip here)
    // We handle this by checking if the entry's parent matches the folder pattern
    // e.g. folder_name/* or *folder_name/* or folder_name*/*
    if (pattern.size() >= 2 && pattern.substr(pattern.size() - 2) == "/*") {
        string folderPart = pattern.substr(0, pattern.size() - 2); // strip trailing /*

        bool folderEndsWithSlash = false;
        if (!folderPart.empty() && folderPart.back() == '/') {
            folderPart.pop_back(); // strip optional trailing /
            folderEndsWithSlash = true;
        }
        
        // Match parent folder name against folderPart
        if (folderPart.front() == '*' && folderPart.back() == '*') {
            // *value* -> contains
            string mid = folderPart.substr(1, folderPart.size() - 2);
            return parentEpfs.find(mid) != string::npos;
        }
        else if (folderPart.front() == '*') {
            // *value -> ends with
            string suffix = folderPart.substr(1);
            return parentEpfs.size() >= suffix.size() &&
                parentEpfs.substr(parentEpfs.size() - suffix.size()) == suffix;
        }
        else if (folderPart.back() == '*') {
            // value* -> starts with
            string prefix = folderPart.substr(0, folderPart.size() - 1);
            return parentEpfs.size() >= prefix.size() &&
                parentEpfs.substr(0, prefix.size()) == prefix;
        }
        else {
            // exact folder name
            return parentEpfs == folderPart;
        }
    }

    // Pattern ends with '/' -> only match directories
    bool dirOnly = (!pattern.empty() && pattern.back() == '/');
    if (dirOnly) {
        if (!isDirectory) return false;
        pattern.pop_back(); // strip trailing /
    }

    // Now match `name` against `p`
    if (pattern.front() == '*' && pattern.back() == '*') {
        // *value* -> contains
        string mid = pattern.substr(1, pattern.size() - 2);
        return epfs.find(mid) != string::npos;
    }
    else if (pattern.front() == '*') {
        // *value -> ends with
        string suffix = pattern.substr(1);
        return epfs.size() >= suffix.size() &&
            epfs.substr(epfs.size() - suffix.size()) == suffix;
    }
    else if (pattern.back() == '*') {
        // value* -> starts with
        string prefix = pattern.substr(0, pattern.size() - 1);
        return epfs.size() >= prefix.size() &&
            epfs.substr(0, prefix.size()) == prefix;
    }
    else {
        // exact match
        return epfs == pattern;
    }
}

void FilesystemNode::buildOut(const vector<string>& patterns) {
	for (filesystem::directory_entry entry : filesystem::directory_iterator(path)) {
		bool excluded = false;
		for (const string& pattern : patterns) 
			if (matchesPattern(pattern, entry.path().filename().string(),
            entry.path().parent_path().filename().string(), entry.is_directory())) {
				excluded = true;
				break;
			}
		if (excluded) continue;

		FilesystemNode entryNode(entry);
		if (entry.is_directory())
			entryNode.buildOut(patterns);
		children.push_back(entryNode);
	}
}