#include "text_tree.hpp"
#include "placed_line.hpp"
#include "../misc/get_root_name.hpp"

using namespace text_tree;

string TextTree::startAt(const char* root) {
	vector<PlacedLine> lines;
	PlacedLine::writeDirectory(lines, root, getRootName(root));

	string text_tree = "", indentation = "";
	int tabsSoFar = 0;

	//loads indentation into the text (based on the PlacedLine objects)
	for (PlacedLine pl : lines) {
		switch (pl.getPlaceOperation()) {
		case PlaceOperation::TABULATE:
			indentation += "\t";
			++tabsSoFar;
			break;
		case PlaceOperation::DETABULATE:
			--tabsSoFar;
			indentation = "";
			for (int i = 0; i < tabsSoFar; i++)
				indentation += "\t";
			break;
		}

		text_tree += indentation + pl.to_string() + "\n";
	}

	return text_tree;
}