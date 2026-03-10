#pragma once
#define K_INCLUDE_EXT "include-ext"
#define K_EXCLUDE_DIR "exclude-dir"
#define K_MODE "mode"
#define K_ROOT "root"

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

			Flags conf = parseFlags(argv.size(), argv.data());

			TestUtil::assertTrue(conf.flags["root"][0] == "./repo", "root flag parsed");
			TestUtil::assertTrue(conf.flags["mode"][0] == "structure", "mode parsed");
		}

		static void test_multi_value_flag() {

			vector<string> args = {
				"prog",
				"--exclude-dir", ".git", "build", "node_modules"
			};

			auto argv = TestUtil::makeArgv(args);

			Flags conf = parseFlags(argv.size(), argv.data());

			TestUtil::assertTrue(conf.flags["exclude-dir"].size() == 3, "exclude-dir has 3 entries");
		}

		static void test_extension_normalization() {

			vector<string> args = {
				"prog",
				"--include-ext", "yml", ".hpp", "md"
			};

			auto argv = TestUtil::makeArgv(args);

			Flags conf = parseFlags(argv.size(), argv.data());

			auto it = conf.flags.find(K_INCLUDE_EXT);
			installFlags(conf, it);

			TestUtil::assertTrue(conf.flags[K_INCLUDE_EXT][0] == ".yml", "yml normalized");
			TestUtil::assertTrue(conf.flags[K_INCLUDE_EXT][1] == ".hpp", "hpp unchanged");
			TestUtil::assertTrue(conf.flags[K_INCLUDE_EXT][2] == ".md", "md normalized");
		}

		static void test_mode_flags() {

			vector<string> args = {
				"prog",
				"--mode", "structure", "content"
			};

			auto argv = TestUtil::makeArgv(args);

			Flags conf = parseFlags(argv.size(), argv.data());

			auto it = conf.flags.find(K_INCLUDE_EXT);
			installFlags(conf, it);

			TestUtil::assertTrue(conf.MODE_FLAG & Flags::MF_STRUCTURE, "structure mode flag set");
			TestUtil::assertTrue(conf.MODE_FLAG & Flags::MF_CONTENT, "content mode flag set");
		}

		static void test_empty_extension_validation() {

			Flags conf;
			conf.flags[K_INCLUDE_EXT] = {};

			bool caught = false;

			try {

				auto it = conf.flags.find(K_INCLUDE_EXT);

				if (it == conf.flags.end() || it->second.empty())
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
			test_extension_normalization();
			test_mode_flags();
			test_empty_extension_validation();

			cout << "\nAll tests passed.\n\n";
		}
	};
}
