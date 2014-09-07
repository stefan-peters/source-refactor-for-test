#ifndef TEST_H
#define TEST_H

#include <string>
#include <iostream>
#include <algorithm>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "clang/Format/Format.h"

// unify the string
inline std::string f(const std::string& str) {
	using namespace clang;
	using namespace clang::format;

	std::string s = str;
//	s.erase( std::remove_if( s.begin(), s.end(), ::isspace ), s.end() );

	std::vector<tooling::Range> ranges(1, tooling::Range(0, str.length()));
    tooling::Replacements replaces = reformat(getGoogleStyle(FormatStyle::LK_Cpp), str, ranges);

    return applyAllReplacements(str, replaces);
//	return s;
}


inline std::string code(const std::string& str) {
	return std::string("void code() { ") + str + std::string(" }");
}

#endif