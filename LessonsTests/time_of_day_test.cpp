#include <gtest/gtest.h>

#include "errors.h"
#include "time_of_day.h"

TEST(TimeOfDay, ConstructorStoresFields) {
  TimeOfDay time(14, 30);
  EXPECT_EQ(time.hours(), 14);
  EXPECT_EQ(time.minutes(), 30);
}

TEST(TimeOfDay, ConstructorAcceptsBounds) {
  EXPECT_NO_THROW(TimeOfDay(0, 0));
  EXPECT_NO_THROW(TimeOfDay(23, 59));
}

TEST(TimeOfDay, ConstructorRejectsBadHours) {
  EXPECT_THROW(TimeOfDay(24, 0), RangeError);
  EXPECT_THROW(TimeOfDay(-1, 0), RangeError);
}

TEST(TimeOfDay, ConstructorRejectsBadMinutes) {
  EXPECT_THROW(TimeOfDay(10, 60), RangeError);
  EXPECT_THROW(TimeOfDay(10, -1), RangeError);
}

TEST(TimeOfDay, ToStringPadsWithZeros) {
  EXPECT_EQ(TimeOfDay(9, 5).ToString(), "09:05");
  EXPECT_EQ(TimeOfDay(0, 0).ToString(), "00:00");
  EXPECT_EQ(TimeOfDay(23, 59).ToString(), "23:59");
}

TEST(TimeOfDay, EqualityAndInequality) {
  EXPECT_TRUE(TimeOfDay(9, 5) == TimeOfDay(9, 5));
  EXPECT_FALSE(TimeOfDay(9, 5) == TimeOfDay(9, 6));
  EXPECT_TRUE(TimeOfDay(9, 5) != TimeOfDay(10, 5));
  EXPECT_FALSE(TimeOfDay(9, 5) != TimeOfDay(9, 5));
}

TEST(TimeOfDay, OrderingComparesHoursThenMinutes) {
  EXPECT_TRUE(TimeOfDay(9, 59) < TimeOfDay(10, 0));
  EXPECT_TRUE(TimeOfDay(10, 0) < TimeOfDay(10, 1));
  EXPECT_FALSE(TimeOfDay(10, 1) < TimeOfDay(10, 1));
  EXPECT_FALSE(TimeOfDay(11, 0) < TimeOfDay(10, 59));
}
