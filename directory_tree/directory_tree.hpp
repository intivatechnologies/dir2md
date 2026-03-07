#pragma once

#include <string>

#include "../file_records/folder_rep.hpp"

using namespace std;
using namespace file_records;

namespace directory_tree {
	class DirectoryTree {
		//privatize constructor to enforce singleton pattern
		DirectoryTree() = default;

		static const char indentationUnits[];
		string indentationVolume = "";

		void modifyIndentationStep(bool positiveStep);
		void getFamilyOfChildren(string& textualRep, FolderRep* parent);
		void getFamily(string& textualRep, FolderRep* parent);

	public:
		static DirectoryTree& getInstance() {
			static DirectoryTree instance; // Thread-safe in C++11+
			return instance;
		}

		// Delete copy/assignment to ensure single instance
		DirectoryTree(const DirectoryTree&) = delete;
		DirectoryTree& operator=(const DirectoryTree&) = delete;

		string startAt(FolderRep* rep);
	};
}
