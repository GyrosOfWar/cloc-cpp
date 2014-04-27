#ifndef LINEPARSER_HPP
#define LINEPARSER_HPP
#include "common.hpp"
#include <boost/algorithm/string/predicate.hpp>
#include <functional>

enum LineType {
    SOURCE_LINE,
    BLANK_LINE,
    COMMENT_LINE
};
class LineParser {
public:
    virtual ~LineParser() {}

    virtual LineType parse(const string& line) = 0;
};
class CLineParser: public LineParser {
public:
    LineType parse(const string &line);
};

class PythonLineParser: public LineParser {
public:
    LineType parse(const string& line);
};

class LispLineParser: public LineParser {
public:
    LineType parse(const string &line);
};

class LineParserFactory {
public:
    static std::unique_ptr<LineParser> makeLineParser(const string& extension) {
        if (extension == ".cpp" || extension == ".hpp" || extension == ".h" || extension == ".c"
                || extension == ".java" || extension == ".php" || extension == ".scala"
                || extension == ".cs") {
            return std::unique_ptr<LineParser>(new CLineParser());
        }

        if (extension == ".py") {
            return std::unique_ptr<LineParser>(new PythonLineParser());
        }

        if (extension == ".clj" || extension == ".lisp") {
            return std::unique_ptr<LineParser>(new LispLineParser());
        }
    }
};



#endif // LINEPARSER_HPP
