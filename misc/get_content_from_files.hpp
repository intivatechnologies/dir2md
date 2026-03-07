#pragma once

#include <vector>
#include <string>

#include <fstream>
#include <sstream>

using namespace std;

vector<string> getContentFromFiles(const vector<string>& fileRoots) {
	vector<string> contents;
	for (const string& fileRoot : fileRoots) {
		ifstream file(fileRoot);
		if (!file)
			throw std::runtime_error("Failed to open file");

		ostringstream buffer;
		buffer << file.rdbuf();
		contents.push_back(buffer.str());
	}

	return contents;
}
