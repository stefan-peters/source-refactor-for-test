#include "test.h"
#include <s2tr/refactor.h>

TEST(BraceTests, CodeThatShouldNotBeChanged) {
  std::string code_that_should_not_be_changed =
      code("if (true) {int i = 0;} else {int j=2;}");

  ASSERT_EQ(f(code_that_should_not_be_changed),
            f(s2tr::refactor(code_that_should_not_be_changed)));
}

TEST(BraceTests, AddMissingBraces) {

  const std::string expected = code("if (true) {"
                                    "  int i = 0;"
                                    "} else {"
                                    "	 int j=2;"
                                    "}");

  const std::string original = code(
                               "if (true)"
                               "  int i = 0;"
                               "else"
                               "  int j=2;");

  ASSERT_EQ(f(expected), f(s2tr::refactor(original)));
}

TEST(BraceTests, AddMissingBracesComplex) {

  const std::string expected = code("if (true) {"
                                    "  if(false || false) {"
                                    "    int i = 0;"
                                    "  } else {"
                                    "    int j=2;"
                                    "  }"
                                    "} else {"
                                    "  int x = 3;"
                                    "}");

  const std::string original = code("if (true)"
                                    "  if(false || false)"
                                    "    int i = 0;"
                                    "  else"
                                    "    int j=2;"
                                    "else"
                                    "  int x = 3;");

  ASSERT_EQ(f(expected), f(s2tr::refactor(original)));
}

// std::string x = f(original);
// std::cout << "\n---------------------------\n";
// std::cout << x;
// std::cout << "\n---------------------------\n";
// std::cout << f(s2tr::refactor(original));
// std::cout << "\n---------------------------\n";
