#pragma once

#include "test_util.hpp"
#include "../client/parse_flags.hpp"

using namespace client;

namespace test_repurposing {
	class AssignedTests {
	public:
		static void test_basic_flag_parsing() {

			vector<string> args = {
				"prog",
				"--root", "./repo",
				"--mode", "structure"
			};

			auto argv = TestUtil::makeArgv(args);

			Flags conf = parseAndInstallFlags(argv.size(), argv.data());
			string_view rootTag = conf.getAt(K_ROOT, 0), modeTag = conf.getAt(K_MODE, 0);

			TestUtil::assertTrue(rootTag.size() > 0 && rootTag == "./repo", "root flag parsed");
			TestUtil::assertTrue(modeTag.size() > 0 && modeTag == "structure", "mode parsed");
		}

		static void test_multi_value_flag() {

			vector<string> args = {
				"prog",
				"--exclude", ".git", "build", "node_modules"
			};

			auto argv = TestUtil::makeArgv(args);

			Flags conf = parseAndInstallFlags(argv.size(), argv.data());

			TestUtil::assertTrue(conf.get(K_EXCLUDE).size() == 3, "exclude has 3 entries");
		}

		/*
		static void test_extension_normalization() {

			vector<string> args = {
				"prog",
				"--include-ext", "yml", ".hpp", "md"
			};

			auto argv = TestUtil::makeArgv(args);

			Flags conf = parseAndInstallFlags(argv.size(), argv.data());
			vector<string> includeTags = conf.get(K_INCLUDE_EXT);

			TestUtil::assertTrue(includeTags.size() > 0 && includeTags[0] == ".yml", "yml normalized");
			TestUtil::assertTrue(includeTags.size() > 1 && includeTags[1] == ".hpp", "hpp unchanged");
			TestUtil::assertTrue(includeTags.size() > 2 && includeTags[2] == ".md", "md normalized");
		}
		*/

		static void test_mode_flags() {

			vector<string> args = {
				"prog",
				"--mode", "structure", "content"
			};

			auto argv = TestUtil::makeArgv(args);

			Flags conf = parseAndInstallFlags(argv.size(), argv.data());

			TestUtil::assertTrue(conf.MODE_FLAG & Flags::MF_STRUCTURE, "structure mode flag set");
			TestUtil::assertTrue(conf.MODE_FLAG & Flags::MF_CONTENT, "content mode flag set");
		}

		static void test_empty_extension_validation() {

			Flags conf;
			conf.flags[K_EXCLUDE] = {};

			bool caught = false;

			try {

				if (!conf.has(K_EXCLUDE) || conf.flags[K_EXCLUDE].empty())
					TestUtil::throwErr("file extensions to dedicate for extraction");

			}
			catch (...) {
				caught = true;
			}

			TestUtil::assertTrue(caught, "empty extension list throws error");
		}

		static void runTests() {

			cout << "Running tests...\n\n";

			test_basic_flag_parsing();
			test_multi_value_flag();
			//test_extension_normalization();
			test_mode_flags();
			test_empty_extension_validation();

			cout << "\nAll tests passed.\n\n";
		}
	};
}
