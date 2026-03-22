#pragma once

#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace test_repurposing {
	class TestUtil {
	public:
		static void throwErr(string reasonInp) {
			throw "Your input failed to specify " + reasonInp + ".";
		}

		static void assertTrue(bool cond, const string& msg) {
			if (!cond) {
				cout << "[FAIL] " << msg << endl;
				exit(1);
			}
			else
				cout << "[PASS] " << msg << endl;
		};

		static vector<char*> makeArgv(vector<string>& args) {
			vector<char*> out;
			for (auto& s : args)
				out.push_back(const_cast<char*>(s.c_str()));
			return out;
		};
	};
}
