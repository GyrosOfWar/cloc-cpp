#include <iostream>
#include <chrono>

// Single line comment

/** Doxygen-style doc-comment
	@brief A test class
*/ 
class Test {
public:
	Test();
	~Test();
};

class OtherTest {
public:
	OtherTest();
	Test doSomething();
};

// Single line comment: This file should
// have:
// 12 Code lines
// 10 Comment lines 
// 4 Blank lines
// -> 26 overall lines