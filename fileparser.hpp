#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "common.hpp"
#include "fileinfo.hpp"
#include <fstream>
#include <memory>
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <map>

enum LineType {
    SINGLE_LINE_COMMENT,
    SOURCE_LINE,
    BLANK_LINE,
    MULTI_LINE_COMMENT_START,
    MULTI_LINE_COMMENT,
    MULTI_LINE_COMMENT_END
};


class FileParser {
public:
    FileParser():
        singleToken(""),
        multiStartToken(""),
        multiEndToken(""),
        isSingleLine(false),
        isMultiLine(false) { }

    FileParser(const string& singleToken):
        singleToken(singleToken),
        multiStartToken(""),
        multiEndToken(""),
        isSingleLine(true),
        isMultiLine(false) { }

    FileParser(const string& multiStartToken, const string& multiEndToken):
        multiStartToken(multiStartToken),
        multiEndToken(multiEndToken),
        singleToken(""),
        isSingleLine(false),
        isMultiLine(true) { }

    FileParser(const string& multiStartToken, const string& multiEndToken, const string& singleToken):
        singleToken(singleToken),
        multiStartToken(multiStartToken),
        multiEndToken(multiEndToken),
        isSingleLine(true),
        isMultiLine(true) { }

    FileInfo parse(const file::path& path);

private:
    string singleToken;
    string multiStartToken;
    string multiEndToken;
    bool isSingleLine;
    bool isMultiLine;
    LineType parseSingleLine(const string& line);

    LineType parseMultiLine(const string& line, bool& currentlyMultiLine);
};

class FileParserFactory {
public:
    static FileParser& makeFileParser(const file::path& path);

    static vector<string> getExtensions();

private:
    static vector<string> cExts;
    static vector<string> lispExts;
    static std::map<string, FileParser> parsers;

    static void init();
    static bool isInitialized;
};

#endif // FILEPARSER_H
