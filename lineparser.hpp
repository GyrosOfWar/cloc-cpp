#ifndef LINEPARSER_HPP
#define LINEPARSER_HPP
#include "common.hpp"
#include <boost/algorithm/string/predicate.hpp>
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

#endif // LINEPARSER_HPP
