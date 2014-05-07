#include "fileparser.hpp"

FileInfo FileParser::parse(const file::path& path) {
    std::ifstream file(path.generic_string());
    string line;
    FileInfo info;
    bool multiLine = false;

    if(file) {
        while(std::getline(file, line)) {
            boost::trim_left(line);

            LineType result;
            if (isSingleLine) {
                result = parseSingleLine(line);
            }
            if (isMultiLine) {
                result = parseMultiLine(line, multiLine);
            }

            switch(result) {
            case SINGLE_LINE_COMMENT:
            case MULTI_LINE_COMMENT:
            case MULTI_LINE_COMMENT_END:
            case MULTI_LINE_COMMENT_START:
                info.incSourceLines();
                break;
            case SOURCE_LINE:
                info.incSourceLines();
                break;
            case BLANK_LINE:
                info.incBlankLines();
                break;
            default:
                assert(0);
            }
        }
    }

    return info;
}

LineType FileParser::parseSingleLine(const string& line) {
    if (boost::starts_with(line, singleToken)) {
        return SINGLE_LINE_COMMENT;
    }

    if (line == "") {
        return BLANK_LINE;
    }

    return SOURCE_LINE;
}

LineType FileParser::parseMultiLine(const string& line, bool& currentlyMultiLine) {
    if (currentlyMultiLine) {
        if (boost::find_first(line, multiEndToken)) {
            currentlyMultiLine = false;
            return MULTI_LINE_COMMENT_END;
        }
        else {
            return MULTI_LINE_COMMENT;
        }
    }
    else if (boost::starts_with(line, multiStartToken)) {
        if (!boost::find_first(line, multiEndToken)) {
            currentlyMultiLine = true;
        }
        return MULTI_LINE_COMMENT_START;
    }
    else if (line == "") {
        return BLANK_LINE;
    }

    return SOURCE_LINE;
}

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
