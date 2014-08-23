#ifndef TEST_H
#define TEST_H

#include <string>
#include <algorithm>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

// unify the string
inline std::string f(const std::string& str) {
	std::string s = str;
	s.erase( std::remove_if( s.begin(), s.end(), ::isspace ), s.end() );
	return s;
}

#endif