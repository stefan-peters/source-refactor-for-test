#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <s2tr/refactor.h>



TEST(ElseTests, CodeThatShouldNotBeChanged) {
  std::string code_that_should_not_be_changed = "if(true){int i = 0;}else{int j=2;}";

  ASSERT_EQ(code_that_should_not_be_changed, s2tr::refactor(code_that_should_not_be_changed));
}
