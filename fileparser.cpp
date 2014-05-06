#include "fileparser.hpp"

vector<string> FileParserFactory::cExts { ".cpp", ".hpp", ".cs", ".h", ".c",
                                                   ".java", ".scala", ".php" };
vector<string> FileParserFactory::lispExts { ".lisp", ".clj", ".scm", ".ss", ".rkt" };

bool FileParserFactory::isInitialized = false;

std::map<string, FileParser> FileParserFactory::parsers = std::map<string, FileParser>();

void FileParserFactory::init() {
    FileParser cParser("/*", "*/", "//");
    for (auto& ext: cExts) {
        parsers[ext] = cParser;
    }

    FileParser lispParser(";");
    for (auto& ext: lispExts) {
        parsers[ext] = lispParser;
    }
    FileParser htmlParser("<!--", "-->");

    parsers[".html"] = htmlParser;
    parsers[".htm"] = htmlParser;

    FileParser rubyParser("=begin", "=end", "#");
    parsers[".rb"] = rubyParser;

    FileParser pythonParser("#");
    parsers[".py"] = pythonParser;
    parsers[".coffee"] = pythonParser;

    FileParser cssParser("/*", "*/");
    parsers[".css"] = cssParser;

}

FileParser& FileParserFactory::makeFileParser(const file::path& path) {
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
