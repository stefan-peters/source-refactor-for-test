#include "test.h"
#include <s2tr/refactor.h>

TEST(ElseTests, AddMissingElseForCompoundIf) {

  std::string expected = code("  	if (true) {"
                              "  		int i = 0;"
                              "   } else {"
                              "  	}");

  std::string original = code("  	if (true) {"
                              "  		int i = 0;"
                              "   }"); 

  ASSERT_EQ(f(expected), f(s2tr::refactor(original)));
}

TEST(ElseTests, AddMissingElseForNonCompundIf) {

  std::string expected = code("   if (true) {"
                              "     int i = 0;"
                              "   } else {"
                              "   }");

  std::string original = code("   if (true)"
                              "     int i = 0;");  

  ASSERT_EQ(f(expected), f(s2tr::refactor(original)));
}

TEST(ElseTests, AddMissingElseComplex) {

  const char* expected = "void test(){"
"  	if (true) {"
"       if(false) {"
"  		    int i = 0;"
"       } else {}"
"   } else {}"
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
"   int j = 1;"
"}";

  const char* original = "void test(){"
"  	if (true)"
"  		int i = 0; int j = 1;"
"}";

  ASSERT_EQ(
    f(expected),
    f(s2tr::refactor(original))
  );
}
