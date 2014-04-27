#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "common.hpp"
#include "lineparser.hpp"
#include "fileinfo.hpp"
#include <fstream>
#include <memory>

class FileParser {
public:
    static FileInfo parseFile(const file::path& p);
    static vector<string> supportedExtensions;
};

#endif // FILEPARSER_H
