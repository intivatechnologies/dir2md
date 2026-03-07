#pragma once
#ifndef FOLDER_REP_HPP
#define FOLDER_REP_HPP

#include <string>
#include <vector>

using namespace std;

namespace file_records {
	class FileRecord {
		const string name, root;

	public:
		FileRecord(const string name, const string root) : name(name), root(root) {}
		virtual ~FileRecord() = default; // Make destructor virtual for polymorphism

		const string getName() const {
			return name;
		}

		const string getRoot() const {
			return root;
		}
	};

	class FileRep : public FileRecord {
	protected:
		const string extension;

	public:
		FileRep(const string name, const string root, const string extension) 
			: FileRecord(name, root), extension(extension) {}

		const string getExtension() const {
			return extension;
		}
	};

	class FolderRep : public FileRecord {
		static const vector<string> disincludeFolders;

		const vector<FileRecord*> children;

	public:
		static FolderRep* installFolderAtRoot(string root);
		static void loadAllRootsByExtension(vector<string>& roots, FolderRep* parentFolder,
			const string ext);

		FolderRep(const string name, const string root, const vector<FileRecord*> children)
			: FileRecord(name, root), children(children) {}

		const vector<FileRecord*>& getChildren() const {
			return children;
		}
	};
}

#endif