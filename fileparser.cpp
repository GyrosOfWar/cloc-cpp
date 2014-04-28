#include "fileparser.hpp"

vector<string> FileParserFactory::cExts { ".cpp", ".hpp", ".cs", ".h", ".c",
                                                   ".java", ".scala", ".php" };
vector<string> FileParserFactory::lispExts { ".lisp", ".clj", ".scm", ".ss", ".rkt" };

vector<string> FileParserFactory::rubyExts { ".rb", ".rbw" };

std::unique_ptr<IFileParser> FileParserFactory::makeFileParser(const file::path& path) {
    auto ext = path.extension().generic_string();

    if (std::find(cExts.begin(), cExts.end(), ext) != cExts.end()) {
        return std::unique_ptr<IFileParser>(new MultiLineCommentParser("/*", "*/", "//"));
    }

    if (std::find(lispExts.begin(), lispExts.end(), ext) != lispExts.end()) {
        return std::unique_ptr<IFileParser>(new SingleLineCommentParser(";"));
    }
    if (std::find(rubyExts.begin(), rubyExts.end(), ext) != rubyExts.end()) {
        return std::unique_ptr<IFileParser>(new MultiLineCommentParser("=begin", "=end" , "#"));
    }


    assert(false);
}

vector<string> FileParserFactory::getExtensions() {
    vector<string> ret;
    ret.insert(ret.end(), cExts.begin(), cExts.end());
    ret.insert(ret.end(), lispExts.begin(), lispExts.end());
    ret.insert(ret.end(), rubyExts.begin(), rubyExts.end());
    return ret;
}

FileInfo SingleLineCommentParser::parseFile(const file::path &path) {
    std::ifstream file(path.generic_string());
    string line;
    FileInfo info;

    while (std::getline(file, line)) {
        boost::trim(line);

        if (boost::starts_with(line, commentToken)) {
            info.incCommentLines();
        }

        if (line == "") {
            info.incBlankLines();
        }

        else {
            info.incSourceLines();
        }
    }
    return info;
}

FileInfo MultiLineCommentParser::parseFile(const file::path &path) {
    std::ifstream file(path.generic_string());
    string line;
    FileInfo info;
    bool multiLine = false;

    while (std::getline(file, line)) {
        boost::trim(line);

        if (multiLine) {
            if (boost::find_first(line, multiEndToken)) {
                info.incCommentLines();
                multiLine = false;
            }

            else {
                info.incCommentLines();
            }
        }

        if (boost::starts_with(line, singleLineToken)) {
            info.incCommentLines();
        }

        if (boost::starts_with(line, multiStartToken)) {
            info.incCommentLines();
            if (!boost::find_first(line, multiEndToken)) {
                multiLine = true;
            }
        }
        if (line == "") {
            info.incBlankLines();
        }

        else {
            info.incSourceLines();
        }
    }

    return info;
}
