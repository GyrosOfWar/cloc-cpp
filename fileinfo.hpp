#ifndef FILEINFO_HPP
#define FILEINFO_HPP
#include "common.hpp"
#include <sstream>

class FileInfo {
public:
    FileInfo(const boost::filesystem::path &path, int blank, int source, int comment):
        path(path),
        blankLines(blank),
        sourceLines(source),
        commentLines(comment) { }
    FileInfo():
        path(file::path()),
        blankLines(0),
        sourceLines(0),
        commentLines(0) { }

    file::path getPath() const { return path; }
    int getBlankLines() const { return blankLines; }
    int getSourceLines() const { return sourceLines; }
    int getCommentLines() const { return commentLines; }

    string getExtension() const { return path.extension().generic_string(); }

    string toString() const {
        std::ostringstream str;

        str << "LoC stats for file " << path.filename() << ": " << endl
            << "Lines of code: " << sourceLines << endl
            << "Comment lines: " << commentLines << endl
            << "Blank lines: " << blankLines << endl;
        return str.str();
    }

private:
    file::path path;
    int blankLines;
    int sourceLines;
    int commentLines;
};

#endif // FILEINFO_HPP
