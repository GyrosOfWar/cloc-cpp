#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "common.hpp"
#include "fileinfo.hpp"
#include <fstream>
#include <memory>
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <map>

class FileParser {
public:
    FileParser():
        singleToken(""),
        multiStartToken(""),
        multiEndToken(""),
        isSingleLine(false),
        isMultiLine(false)
    { }

    FileParser(const string& singleToken):
        singleToken(singleToken),
        multiStartToken(""),
        multiEndToken("") {
        isSingleLine = true;
        isMultiLine = false;
    }

    FileParser(const string& multiStartToken, const string& multiEndToken):
        multiStartToken(multiStartToken),
        multiEndToken(multiEndToken),
        singleToken("") {
        isSingleLine = false;
        isMultiLine = true;
    }

    FileParser(const string& multiStartToken, const string& multiEndToken, const string& singleToken):
        singleToken(singleToken),
        multiStartToken(multiStartToken),
        multiEndToken(multiEndToken) {
        isSingleLine = true;
        isMultiLine = true;
    }

    FileInfo parse(const file::path& path) {
        std::ifstream file(path.generic_string());
        string line;
        FileInfo info;
        bool multiLine = false;


        if(file) {
            while(std::getline(file, line)) {
                boost::trim(line);

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

private:
    string singleToken;
    string multiStartToken;
    string multiEndToken;
    bool isSingleLine;
    bool isMultiLine;

    enum LineType {
        SINGLE_LINE_COMMENT,
        SOURCE_LINE,
        BLANK_LINE,
        MULTI_LINE_COMMENT_START,
        MULTI_LINE_COMMENT,
        MULTI_LINE_COMMENT_END
    };

    LineType parseSingleLine(const string& line) {
        if (boost::starts_with(line, singleToken)) {
            return SINGLE_LINE_COMMENT;
        }

        if (line == "") {
            return BLANK_LINE;
        }

        return SOURCE_LINE;
    }

    LineType parseMultiLine(const string& line, bool& currentlyMultiLine) {
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
