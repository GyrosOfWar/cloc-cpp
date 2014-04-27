#include "fileparser.hpp"

FileInfo FileParser::parseFile(const file::path &p) {
    int sourceLines = 0;
    int blankLines = 0;
    int commentLines = 0;

    auto ext = p.extension().generic_string();
    auto lineParser = LineParserFactory::makeLineParser(ext);

    std::ifstream file(p.generic_string());
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

    return FileInfo(p, blankLines, sourceLines, commentLines);
}

vector<string> FileParser::supportedExtensions =
    { ".cpp", ".hpp", ".c", ".h", ".py", ".scala", ".java", ".scala", ".clj", ".lisp", ".cs" };
