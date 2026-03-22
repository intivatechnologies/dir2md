#pragma once

#include <map>
#include <string>
#include <vector>
#include <string_view>
#include <stdexcept>

using namespace std;

namespace client {
	struct Flags {
		map<string, vector<string>> flags;

		enum ModeFlag {
			MF_NONE = 0,
			MF_STRUCTURE = 1 << 0,
			MF_CONTENT = 1 << 1
		} MODE_FLAG = ModeFlag::MF_NONE;

		bool has(const string& key) const {
			return flags.find(key) != flags.end();
		}

		//return vector is immutable; you cannot add to it
        const vector<string>& get(const string& key) {
			auto it = flags.find(key);
			if (it != flags.end()) 
				return it->second;

			//this wouldn't be safe if the vector were mutable because the variable's
			//allocation is independent of the key
			static const vector<string> empty;
			return empty;
        }

		const string& getAt(const string& key, int index) {
			return get(key)[index];
		}

		void add(const string& key, const string value) {
			flags[key].push_back(value);
		}

		void parse(int argc, char* argv[]) {
			string currentFlag;
			const string escapes = "\\\"'\n\t\r";

			for (int i = 0; i < argc; ++i) {
				string_view token = argv[i];
				if (token.empty()) continue;

				// 1. Simplified Escape Character Check
				if (escapes.find(token.front()) != string::npos ||
					escapes.find(token.back()) != string::npos) {
					throw runtime_error("Tokens cannot start or end with escape characters.");
				}

				// 2. Process for --flags
				if (token.rfind("--", 0) == 0) {
					currentFlag = token.substr(2);
					flags[currentFlag]; // Ensure key exists
				}

				// 3. Process for tags (values)
				else if (!currentFlag.empty())
					flags[currentFlag].emplace_back(token);
			}
		}
	};

	void printAllFlagsAndTags(const Flags& conf) {
		for (const auto& pair : conf.flags) {
			cout << "--" << pair.first;
			for (const auto& tag : conf.flags.at(pair.first))
				cout << ' ' << tag;
			cout << endl;
		}
		cout << endl;
	}

	Flags parseAndInstallFlags(int argc, char* argv[]) {
		Flags conf;
		conf.parse(argc, argv);

		/*
		//ensure file extensions have a dot char at the beginning
		for (int i = 0; i < conf.flags[K_INCLUDE_EXT].size(); i++) {
			if (conf.flags[K_INCLUDE_EXT][i][0] != '.')
				conf.flags[K_INCLUDE_EXT][i] = '.' + conf.flags[K_INCLUDE_EXT][i];
		}
		*/

		//extract one or modes into K_MODE
		vector<string> modes = conf.flags[K_MODE];
		for (auto& m : modes) {
			if (m == "structure")
				conf.MODE_FLAG = (Flags::ModeFlag)(conf.MODE_FLAG | conf.MF_STRUCTURE);

			if (m == "content")
				conf.MODE_FLAG = (Flags::ModeFlag)(conf.MODE_FLAG | conf.MF_CONTENT);
		}

		//print all flags and tags
		printAllFlagsAndTags(conf);

		return conf;
	}
}