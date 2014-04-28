#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "common.hpp"
#include "fileinfo.hpp"
#include <fstream>
#include <memory>
#include <boost/algorithm/string.hpp>
#include <cassert>


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
    MultiLineCommentParser(string start, string end, string singleLine):
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
    static std::unique_ptr<IFileParser> makeFileParser(const file::path& path);

    static vector<string> getExtensions();

private:
    static vector<string> cExts;
    static vector<string> lispExts;
    static vector<string> rubyExts;
};

#endif // FILEPARSER_H
