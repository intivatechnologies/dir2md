#pragma once

#include <string>
#include <vector>
#include <map>

#include "../misc/get_root_name.hpp"

using namespace std;
using namespace std::filesystem;

namespace text_tree {
	enum PlaceOperation {
		TABULATE,
		DETABULATE,
		CONTINUE
	};

	/* A line placed */
	class PlacedLine {
		static const vector<string> ignoreDirectoryNames;
		static vector<PlacedLine> lookInsideDirectory(string root);

		const string line;
		const PlaceOperation placeOperation;

	public:
		static void writeDirectory(vector<PlacedLine>& container, string absPath, string rootName,
			PlaceOperation startingOperation = PlaceOperation::CONTINUE);

		const string& to_string() const { return line; };
		const PlaceOperation& getPlaceOperation() const { return placeOperation; };
		explicit PlacedLine(string line, PlaceOperation placeOperation = PlaceOperation::CONTINUE)
			: line(line), placeOperation(placeOperation) {
		}
	};
}