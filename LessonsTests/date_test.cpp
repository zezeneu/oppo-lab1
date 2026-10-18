#include <gtest/gtest.h>

#include "date.h"
#include "errors.h"

TEST(Date, ConstructorStoresFields) {
  Date date(2024, 3, 11);
  EXPECT_EQ(date.year(), 2024);
  EXPECT_EQ(date.month(), 3);
  EXPECT_EQ(date.day(), 11);
}

TEST(Date, ConstructorAcceptsFirstAndLastDayOfYear) {
  EXPECT_NO_THROW(Date(2023, 1, 1));
  EXPECT_NO_THROW(Date(2023, 12, 31));
}

TEST(Date, ConstructorAcceptsYearBounds) {
  EXPECT_NO_THROW(Date(Date::kMinYear, 1, 1));
  EXPECT_NO_THROW(Date(Date::kMaxYear, 12, 31));
}

TEST(Date, ConstructorRejectsYearOutOfRange) {
  EXPECT_THROW(Date(0, 1, 1), RangeError);
  EXPECT_THROW(Date(10000, 1, 1), RangeError);
  EXPECT_THROW(Date(-5, 1, 1), RangeError);
}

TEST(Date, ConstructorRejectsMonthOutOfRange) {
  EXPECT_THROW(Date(2024, 0, 1), RangeError);
  EXPECT_THROW(Date(2024, 13, 1), RangeError);
}

TEST(Date, ConstructorRejectsDayOutOfRange) {
  EXPECT_THROW(Date(2024, 1, 0), RangeError);
  EXPECT_THROW(Date(2024, 1, 32), RangeError);
  EXPECT_THROW(Date(2024, 4, 31), RangeError);
  EXPECT_THROW(Date(2024, 6, 31), RangeError);
  EXPECT_THROW(Date(2024, 9, 31), RangeError);
  EXPECT_THROW(Date(2024, 11, 31), RangeError);
}

TEST(Date, LastDaysOfLongMonthsAreValid) {
  EXPECT_NO_THROW(Date(2024, 1, 31));
  EXPECT_NO_THROW(Date(2024, 3, 31));
  EXPECT_NO_THROW(Date(2024, 5, 31));
  EXPECT_NO_THROW(Date(2024, 7, 31));
  EXPECT_NO_THROW(Date(2024, 8, 31));
  EXPECT_NO_THROW(Date(2024, 10, 31));
  EXPECT_NO_THROW(Date(2024, 12, 31));
}

TEST(Date, LastDaysOfShortMonthsAreValid) {
  EXPECT_NO_THROW(Date(2024, 4, 30));
  EXPECT_NO_THROW(Date(2024, 6, 30));
  EXPECT_NO_THROW(Date(2024, 9, 30));
  EXPECT_NO_THROW(Date(2024, 11, 30));
}

TEST(Date, FebruaryInLeapYear) {
  EXPECT_NO_THROW(Date(2024, 2, 29));
  EXPECT_THROW(Date(2024, 2, 30), RangeError);
}

TEST(Date, FebruaryInCommonYear) {
  EXPECT_NO_THROW(Date(2023, 2, 28));
  EXPECT_THROW(Date(2023, 2, 29), RangeError);
}

TEST(Date, ToStringPadsWithZeros) {
  EXPECT_EQ(Date(2024, 3, 5).ToString(), "2024.03.05");
  EXPECT_EQ(Date(7, 12, 31).ToString(), "0007.12.31");
}

TEST(Date, EqualityAndInequality) {
  EXPECT_TRUE(Date(2024, 3, 5) == Date(2024, 3, 5));
  EXPECT_FALSE(Date(2024, 3, 5) == Date(2024, 3, 6));
  EXPECT_TRUE(Date(2024, 3, 5) != Date(2024, 4, 5));
  EXPECT_FALSE(Date(2024, 3, 5) != Date(2024, 3, 5));
}

TEST(Date, OrderingComparesYearThenMonthThenDay) {
  EXPECT_TRUE(Date(2023, 12, 31) < Date(2024, 1, 1));
  EXPECT_TRUE(Date(2024, 1, 31) < Date(2024, 2, 1));
  EXPECT_TRUE(Date(2024, 2, 1) < Date(2024, 2, 2));
  EXPECT_FALSE(Date(2024, 2, 2) < Date(2024, 2, 2));
  EXPECT_FALSE(Date(2024, 3, 1) < Date(2024, 2, 28));
}

TEST(Date, NonexistentDatesAreRejectedByChrono) {
  EXPECT_THROW(Date(2024, 2, 30), RangeError);
  EXPECT_THROW(Date(2023, 2, 29), RangeError);
  EXPECT_THROW(Date(1900, 2, 29), RangeError);
  EXPECT_THROW(Date(2023, 4, 31), RangeError);
}

TEST(Date, LeapDaysAreAccepted) {
  EXPECT_NO_THROW(Date(2024, 2, 29));
  EXPECT_NO_THROW(Date(2000, 2, 29));
}
