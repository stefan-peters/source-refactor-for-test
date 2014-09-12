#include "test.h"
#include <s2tr/refactor.h>

TEST(ElseTests, AddMissingElseForCompoundIf) {

  const std::string expected = code("if (true) {"
                                    "  int i = 0;"
                                    "} else {"
                                    "}");

  const std::string original = code("if (true) {"
                                    "  int i = 0;"
                                    "}");

  ASSERT_EQ(f(expected), f(s2tr::refactor(original)));
}

TEST(ElseTests, AddMissingElseForNonCompundIf) {

  const std::string expected = code("if (true) {"
                                    "  int i = 0;"
                                    "} else {"
                                    "}");

  const std::string original = code("if (true)"
                                    "  int i = 0;");

  ASSERT_EQ(f(expected), f(s2tr::refactor(original)));
}

TEST(ElseTests, AddMissingElseComplex) {

  const std::string expected = code("if (true) {"
                                    "  if(false) {"
                                    "    int i = 0;"
                                    "  } else {}"
                                    "} else {}");

  const std::string original = code("if (true) {"
                                    "  if(false) {"
                                    "    int i = 0;"
                                    "  }"
                                    "}");

  ASSERT_EQ(f(expected), f(s2tr::refactor(original)));
}

TEST(ElseTests, AddMissingElseWithoutBraces) {

  const std::string expected = code("if (true) {"
                                    "  int i = 0;"
                                    "} else {"
                                    "}"
                                    "int j = 1;");

  const std::string original = code("if (true)"
                                    "  int i = 0; int j = 1;");

  ASSERT_EQ(f(expected), f(s2tr::refactor(original)));
}
