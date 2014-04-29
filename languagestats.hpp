#ifndef LANGUAGESTATS_HPP
#define LANGUAGESTATS_HPP
#include "common.hpp"

class LanguageStats {
public:
    LanguageStats(string name = ""):
        numFiles(0),
        blankLines(0),
        commentLines(0),
        sourceLines(0),
        extension(name) { }

    int getNumFiles() const { return numFiles; }
    int getBlankLines() const { return blankLines; }
    int getSourceLines() const { return sourceLines; }
    int getCommentLines() const { return commentLines; }
    string getExtension() const { return extension; }

    void addFileInfo(const FileInfo& info, const string& extension) {
        if (extension == "") {
            this->extension = extension;
        }
        numFiles++;
        blankLines += info.getBlankLines();
        commentLines += info.getCommentLines();
        sourceLines += info.getSourceLines();
    }

private:
    int numFiles;
    int blankLines;
    int commentLines;
    int sourceLines;
    string extension;
};

#endif // LANGUAGESTATS_HPP
