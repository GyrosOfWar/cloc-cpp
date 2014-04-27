#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "common.hpp"
#include "lineparser.hpp"
#include "fileinfo.hpp"
#include <fstream>
#include <memory>
#include <functional>

class FileParser {
public:
    static FileInfo parseFile(const file::path& p);
    static vector<string> supportedExtensions;
    static FileInfo parseFile(const boost::filesystem::path &p, const std::function<LineType(string)>& f);
};

#endif // FILEPARSER_H
