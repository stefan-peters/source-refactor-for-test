#include "test.h"

TEST(HelperTests, FormatTest) {
	ASSERT_EQ(
		f("string and  ws"),
		f("   string  and ws  ")
	);
}