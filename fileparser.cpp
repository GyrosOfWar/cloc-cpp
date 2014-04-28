#include "fileparser.hpp"

vector<string> FileParserFactory::cIshExtensions { ".cpp", ".hpp", ".cs", ".h", ".c",
                                                   ".java", ".scala", ".php" };
vector<string> FileParserFactory::lispExtensions { ".lisp", ".clj", ".scm", ".ss", ".rkt" };

FileInfo CFileParser::parseFile(const file::path& path) {
    FileInfo info(path);
    std::ifstream file(path.generic_string());
    string line;
    bool multiLine = false;

    while (std::getline(file, line)) {
        boost::trim(line);
        // If we're inside a multi-line comment, any line that's
        // not "*/" is a comment
        if (multiLine) {
            if (boost::find_first(line, "*/")) {
                info.incCommentLines();
                multiLine = false;
            }
            else {
                info.incCommentLines();
            }
        }
        else {
            if (boost::starts_with(line, "//")) {
                info.incCommentLines();
            }

            if (boost::starts_with(line, "/*")) {
                multiLine = true;
                info.incCommentLines();
            }
            // If we encounter a "*/" without a previous */ it's probably
            // an error or inside another comment for whatever reason
            if (boost::find_first(line, "*/")) {
                info.incCommentLines();
            }

            if (line == string()) {
                info.incBlankLines();
            }

            else {
                info.incSourceLines();
            }
        }

    }
    return info;
}

FileInfo LispFileParser::parseFile(const file::path &path) {
    FileInfo info(path);
    std::ifstream file(path.generic_string());
    string line;

    while (std::getline(file, line)) {
        if (boost::starts_with(line, ";")) {
            info.incCommentLines();
        }
        if (line == string()) {
            info.incBlankLines();
        }
        else {
            info.incSourceLines();
        }
    }

    return info;
}
