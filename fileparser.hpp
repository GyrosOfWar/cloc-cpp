#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "common.hpp"
#include "fileinfo.hpp"
#include <fstream>
#include <memory>
#include <functional>
#include <boost/algorithm/string.hpp>


class IFileParser {
public:
    virtual FileInfo parseFile(const file::path& path) = 0;
};

class CFileParser: public IFileParser {
public:
    FileInfo parseFile(const file::path& path);
};

class LispFileParser: public IFileParser {
public:
    FileInfo parseFile(const file::path& path);
};

class FileParserFactory {
public:
    static std::unique_ptr<IFileParser> makeFileParser(const file::path& path) {
        auto ext = path.extension().generic_string();
        if (std::find(cIshExtensions.begin(), cIshExtensions.end(), ext) != cIshExtensions.end()) {
            return std::unique_ptr<IFileParser>(new CFileParser());
        }

        if (std::find(lispExtensions.begin(), lispExtensions.end(), ext) != lispExtensions.end()) {
            return std::unique_ptr<IFileParser>(new LispFileParser());
        }
    }

    static vector<string> getExtensions() {
        vector<string> ret;
        ret.insert(ret.end(), cIshExtensions.begin(), cIshExtensions.end());
        ret.insert(ret.end(), lispExtensions.begin(), lispExtensions.end());
        return ret;
    }

private:
    static vector<string> cIshExtensions;
    static vector<string> lispExtensions;
};

#endif // FILEPARSER_H
