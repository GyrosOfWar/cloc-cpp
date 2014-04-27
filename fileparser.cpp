#include "fileparser.hpp"

FileParser::FileParser(const file::path &p): path(p) {
    auto extension = p.extension();
    if (extension == ".cpp" || extension == ".hpp" || extension == ".h" || extension == ".c"
            || extension == ".java" || extension == ".php" || extension == ".scala"
            || extension == ".cs") {
        lineParser = std::unique_ptr<LineParser>(new CLineParser());
    }

    if (extension == ".py") {
        lineParser = std::unique_ptr<LineParser>(new PythonLineParser());
    }

    if (extension == ".clj" || extension == ".lisp") {
        lineParser = std::unique_ptr<LineParser>(new LispLineParser());
    }
}

FileInfo FileParser::parseFile() {
    int sourceLines = 0;
    int blankLines = 0;
    int commentLines = 0;

    std::ifstream file(this->path.generic_string());
    string line;

    if (file) {
        while (std::getline(file, line)) {
            auto result = lineParser->parse(line);
            switch(result) {
            case BLANK_LINE:
                blankLines++;
                break;
            case COMMENT_LINE:
                commentLines++;
                break;
            case SOURCE_LINE:
                sourceLines++;
                break;
            }
        }
    }

    return FileInfo(path, blankLines, sourceLines, commentLines);
}
