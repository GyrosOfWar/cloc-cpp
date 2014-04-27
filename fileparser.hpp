#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "common.hpp"
#include "lineparser.hpp"
#include "fileinfo.hpp"
#include <fstream>
#include <memory>

class FileParser {
public:
    FileParser(const file::path& p);
    FileInfo parseFile();
private:
    file::path path;
    std::unique_ptr<LineParser> lineParser;
};

#endif // FILEPARSER_H
