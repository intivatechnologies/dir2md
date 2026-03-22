#pragma once

#include <regex>
#include <vector>
#include <string>

using namespace std;

enum GLOB_PATTERN {
	FF_MATCH_WHERE_VALUE_AT_END, //Matches any file or folder that ends with value (e.g. '*.git')
	FF_MATCH_WHERE_VALUE_AT_START, //Matches any file or folder that starts with with value (e.g. '.git*')

	FOLDER_MATCH_WHERE_VALUE_AT_END, //Matches any file or folder that ends with value (e.g. '*.git/')
	FOLDER_MATCH_WHERE_VALUE_AT_START, //Matches any file or folder that starts with with value (e.g. '.git*/')

    //Matches any file or folder that starts within with folder (e.g. '.git/*')
	FOLDER_MATCH_WHERE_VALUE_AT_INSIDE,

	DIRECT_MATCH,
	NO_MATCH
};

/*
class PatternMatcher {
private:
    struct Rule {
        regex pattern;
        bool isNegation;
        bool onlyFolders;
    };

    vector<Rule> rules;

    string globToRegex(string glob) {
        string res = "^";
        for (int i = 0; i < glob.length(); ++i) {
            char c = glob[i];

            switch (c) {
                case '.': res += "\\."; break;
                case '?': res += "."; break; // Matches exactly one character
                case '*':
                    if (i + 1 < glob.length() && glob[i+1] == '*') {
                        res += ".*"; // Double asterisk: Match everything including slashes
                        i++;
                    } else {
                        res += "[^/]*"; // Single asterisk: Match everything EXCEPT slashes
                    }
                    break;
                case '[':
                    // Character sets (e.g., [0-9] or [a-z]) translate directly to Regex
                    while (i < glob.length() && glob[i] != ']') {
                        res += glob[i];
                        i++;
                    }
                    if (i < glob.length()) res += ']';
                    break;
                case '/':
                    res += "/";
                    break;
                default:
                    // Escape any other potential regex special characters
                    if (string("\\+^$|()").find(c) != string::npos) res += "\\";
                    res += c;
                    break;
            }
        }
        return res + "$";
    }

public:
    void loadPatterns(const vector<string>& patterns) {
        for (string p : patterns) {
            if (p.empty()) continue;

            bool isNeg = (p[0] == '!');
            if (isNeg) p = p.substr(1);

            bool foldersOnly = (p.back() == '/');
            if (foldersOnly) p.pop_back();

            rules.push_back({
                regex(globToRegex(p)),
                isNeg,
                foldersOnly
            });
        }
    }

    bool shouldInclude(string path, bool isFolder) {
        bool included = true;
        for (const auto& rule : rules) {
            if (rule.onlyFolders && !isFolder) continue;
            if (regex_match(path, rule.pattern)) {
                included = rule.isNegation;
            }
        }
        return included;
    }

    void filter(const Folder& current, vector<string>& results) {
        if (!shouldInclude(current.fullPath, true)) return;

        results.push_back(current.fullPath);

        for (const auto& file : current.files) {
            if (shouldInclude(file.fullPath, false)) {
                results.push_back(file.fullPath);
            }
        }

        for (const auto& child : current.children) {
            filter(child, results);
        }
    }
};
*/
