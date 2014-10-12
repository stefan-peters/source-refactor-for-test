#include "test.h"
#include <s2tr/refactor.h>


TEST(IfTests, NotChangingIf) {
  std::string code_that_should_not_be_changed = code("if(true){int i;}");

  ASSERT_EQ(
    f(code_that_should_not_be_changed), 
    f(s2tr::refactor(code_that_should_not_be_changed))
  );
}