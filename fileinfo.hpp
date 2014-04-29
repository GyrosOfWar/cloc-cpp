#ifndef FILEINFO_HPP
#define FILEINFO_HPP
#include "common.hpp"
#include <sstream>
/**
 * @brief FileInfo stores the lines of code, comments and number of
 * blank lines for a file.
 *
 */
class FileInfo {
public:
    FileInfo(int blank, int source, int comment):
        blankLines(blank),
        sourceLines(source),
        commentLines(comment) { }
    FileInfo():
        blankLines(0),
        sourceLines(0),
        commentLines(0) { }

    int getBlankLines() const { return blankLines; }
    int getSourceLines() const { return sourceLines; }
    int getCommentLines() const { return commentLines; }

    void incBlankLines() { blankLines++; }
    void incSourceLines() { sourceLines++; }
    void incCommentLines() { commentLines++; }

private:
    int blankLines;
    int sourceLines;
    int commentLines;
};

#endif // FILEINFO_HPP
