#pragma once
#ifndef FILESYSTEM_NODE_HPP
#define FILESYSTEM_NODE_HPP

#include <string>
#include <vector>
#include <filesystem>

using namespace std;

namespace file_records {
	class FilesystemNode {
	public:
		string name, extension;
		filesystem::path path;
		vector<FilesystemNode> children;

		FilesystemNode(filesystem::directory_entry& directoryEntry) {
			name = directoryEntry.path().filename().string();
			path = directoryEntry.path();
			if (directoryEntry.is_regular_file())
				extension = directoryEntry.path().extension().string();
		}

		void buildOut(const vector<string>& patterns);
	};
}

#endif