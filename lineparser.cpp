#include "lineparser.hpp"

LineType CLineParser::parse(const string &line) {
    if (boost::starts_with(line, "//") || boost::starts_with(line, "/*") || boost::starts_with(line, "*")) {
        return COMMENT_LINE;
    }
    if (line == "") {
        return BLANK_LINE;
    }

    return SOURCE_LINE;
}

LineType PythonLineParser::parse(const string &line) {
    if (boost::starts_with(line, "#")) {
        return COMMENT_LINE;
    }
    if (line == "") {
        return BLANK_LINE;
    }

    return SOURCE_LINE;
}

LineType LispLineParser::parse(const string &line) {
    if (boost::starts_with(line, ";")) {
        return COMMENT_LINE;
    }
    if (line == "") {
        return BLANK_LINE;
    }
    return SOURCE_LINE;
}
