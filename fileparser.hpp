#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "common.hpp"
#include "fileinfo.hpp"
#include <fstream>
#include <memory>
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <map>

using std::unique_ptr;
using std::shared_ptr; using std::make_shared;

class IFileParser {
public:
    virtual FileInfo parseFile(const file::path& path) = 0;
};

class SingleLineCommentParser: public IFileParser {
public:
    SingleLineCommentParser(const string& commentToken): commentToken(commentToken) { }

    FileInfo parseFile(const file::path& path);

private:
    string commentToken;
};

class MultiLineCommentParser: public IFileParser {
public:
    MultiLineCommentParser(string start, string end, string singleLine = ""):
        multiStartToken(start),
        multiEndToken(end),
        singleLineToken(singleLine) { }

    FileInfo parseFile(const file::path &path);

private:
    string multiStartToken;
    string multiEndToken;
    string singleLineToken;
};

class FileParserFactory {
public:
    static shared_ptr<IFileParser> makeFileParser(const file::path& path);

    static vector<string> getExtensions();

private:
    static vector<string> cExts;
    static vector<string> lispExts;
    static std::map<string, shared_ptr<IFileParser> > parsers;

    static void init();
    static bool isInitialized;
};

#endif // FILEPARSER_H
