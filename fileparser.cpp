#include "fileparser.hpp"

vector<string> FileParserFactory::cExts { ".cpp", ".hpp", ".cs", ".h", ".c",
                                                   ".java", ".scala", ".php" };
vector<string> FileParserFactory::lispExts { ".lisp", ".clj", ".scm", ".ss", ".rkt" };

bool FileParserFactory::isInitialized = false;

std::map<string, shared_ptr<IFileParser> > FileParserFactory::parsers = std::map<string, shared_ptr<IFileParser> >();

void FileParserFactory::init() {
    // TODO maybe use a big std::map literal for all of this? Might be more readable
    auto cParser = std::make_shared<MultiLineCommentParser>("/*", "*/", "//");

    for (auto& ext: cExts) {
        parsers[ext] = cParser;
    }

    auto lispParser = std::make_shared<SingleLineCommentParser>(";");
    for (auto& ext: lispExts) {
        parsers[ext] = lispParser;
    }

    auto htmlParser = make_shared<MultiLineCommentParser>("<!--", "-->");

    parsers[".html"] = htmlParser;
    parsers[".htm"] = htmlParser;

    auto rubyParser = make_shared<MultiLineCommentParser>("=begin", "=end", "#");
    parsers[".rb"] = rubyParser;

    auto pythonParser = make_shared<SingleLineCommentParser>("#");
    parsers[".py"] = pythonParser;
    parsers[".coffee"] = pythonParser;

    auto cssParser = make_shared<MultiLineCommentParser>("/*", "*/");
    parsers[".css"] = cssParser;
}

shared_ptr<IFileParser> FileParserFactory::makeFileParser(const file::path& path) {
    if(!isInitialized) {
        init();
        isInitialized = true;
    }

    auto ext = path.extension().generic_string();

    return parsers[ext];
}

vector<string> FileParserFactory::getExtensions() {
    vector<string> ret;
    ret.insert(ret.end(), cExts.begin(), cExts.end());
    ret.insert(ret.end(), lispExts.begin(), lispExts.end());
    ret.push_back(".rb");
    ret.push_back(".py");
    ret.push_back(".htm");
    ret.push_back(".html");
    ret.push_back(".css");
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

        else if (line == "") {
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

        else if (singleLineToken != "" && boost::starts_with(line, singleLineToken)) {
            info.incCommentLines();
        }

        else if (boost::starts_with(line, multiStartToken)) {
            info.incCommentLines();
            if (!boost::find_first(line, multiEndToken)) {
                multiLine = true;
            }
        }
        else if (line == "") {
            info.incBlankLines();
        }

        else {
            info.incSourceLines();
        }
    }

    return info;
}
