#include "gtest_lite.h"

#include "errors.h"
#include "parsing_utils.h"

TEST(Trim, RemovesSpacesOnBothSides) { EXPECT_EQ(Trim("  abc \t\r"), "abc"); }

TEST(Trim, KeepsInnerSpaces) { EXPECT_EQ(Trim(" a  b "), "a  b"); }

TEST(Trim, EmptyAndBlankTextGiveEmptyString) {
  EXPECT_EQ(Trim(""), "");
  EXPECT_EQ(Trim(" \t\r "), "");
}

TEST(Trim, TextWithoutSpacesIsUnchanged) { EXPECT_EQ(Trim("abc"), "abc"); }

TEST(CheckRange, AcceptsBounds) {
  EXPECT_NO_THROW(CheckRange(1, 1, 12, "x"));
  EXPECT_NO_THROW(CheckRange(12, 1, 12, "x"));
  EXPECT_NO_THROW(CheckRange(5, 1, 12, "x"));
}

TEST(CheckRange, RejectsBelowAndAbove) {
  EXPECT_THROW(CheckRange(0, 1, 12, "x"), RangeError);
  EXPECT_THROW(CheckRange(13, 1, 12, "x"), RangeError);
}

TEST(CheckRange, MessageContainsRangeAndValue) {
  try {
    CheckRange(13, 1, 12, "Месяц");
    EXPECT_TRUE(false);
  } catch (const RangeError& error) {
    std::string message = error.what();
    EXPECT_TRUE(message.find("Месяц") != std::string::npos);
    EXPECT_TRUE(message.find("1..12") != std::string::npos);
    EXPECT_TRUE(message.find("13") != std::string::npos);
  }
}

TEST(Errors, HierarchyAllowsCatchingByBaseClass) {
  EXPECT_THROW(throw SyntaxError("x"), LessonError);
  EXPECT_THROW(throw RangeError("x"), LessonError);
  EXPECT_THROW(throw RangeError("x"), std::runtime_error);
}

