#include "test.h"
#include <s2tr/refactor.h>


TEST(ElseTests, CodeThatShouldNotBeChanged) {
  std::string code_that_should_not_be_changed = "void test(){if (true) {int i = 0;} else {int j=2;}}";

  ASSERT_EQ(
    f(code_that_should_not_be_changed), 
    f(s2tr::refactor(code_that_should_not_be_changed))
  );
}

TEST(ElseTests, AddMissingBraces) {

  const char* expected = "void test(){"
"  	if (true) {"
"  		int i = 0;"
"   } else {"
"  		int j=2;"
"  	}"
"}";

  const char* original = "void test(){"
"  	if (true)"
"  		int i = 0;"
"  	else"
"  		int j=2;"
"}";

  ASSERT_EQ(
    f(expected), 
    f(s2tr::refactor(original))
  );
}


TEST(ElseTests, AddMissingBracesComplex) {

  const char* expected = "void test(){"
"  	if (true) {"
"       if(false || false) {"
"  		  int i = 0;"
"       } else {"
"  		   int j=2;"
"  	    }"
"   else {"
"       int x = 3;"
"   }"
"}";

  const char* original = "void test(){"
"  	if (true)"
"       if(false || false)"
"  		   int i = 0;"
"    	else"
"  	    	int j=2;"
"   else"
"       int x = 3;"
"}";

  ASSERT_EQ(
    f(expected), 
    f(s2tr::refactor(original))
  );
}


TEST(ElseTests, AddMissingElse) {

  const char* expected = "void test(){"
"  	if (true) {"
"  		int i = 0;"
"   } else {"
"  	}"
"}";

  const char* original = "void test(){"
"  	if (true) {"
"  		int i = 0;"
"   }"
"}";

  ASSERT_EQ(
    f(expected), 
    f(s2tr::refactor(original))
  );
}


TEST(ElseTests, AddMissingElseComplex) {

  const char* expected = "void test(){"
"  	if (true) {"
"       if(false) {"
"  		    int i = 0;"
"       } else {"
"       }"
"   } else {"
"  	}"
"}";

  const char* original = "void test(){"
"  	if (true) {"
"       if(false) {"
"  		   int i = 0;"
"       }"
"   }"
"}";

  ASSERT_EQ(
    f(expected), 
    f(s2tr::refactor(original))
  );
}


TEST(ElseTests, AddMissingElseWithoutBraces) {

  const char* expected = "void test(){"
"  	if (true) {"
"  		int i = 0;"
"   } else {"
"  	}"
"}";

  const char* original = "void test(){"
"  	if (true)"
"  		int i = 0;"
"}";

  ASSERT_EQ(
    f(expected), 
    f(s2tr::refactor(original))
  );
}
