// Мини-фреймворк модульных тестов с тем же синтаксисом, что у Google Test:
// TEST, EXPECT_EQ, EXPECT_TRUE, EXPECT_THROW и т. д. Работает без установки
// библиотек. Чтобы перейти на настоящий Google Test, достаточно заменить
// #include "gtest_lite.h" на #include <gtest/gtest.h> и подключить пакет.
#ifndef LESSONS_TESTS_GTEST_LITE_H_
#define LESSONS_TESTS_GTEST_LITE_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace testing {

struct TestCase {
  std::string suite;
  std::string name;
  void (*body)();
};

inline std::vector<TestCase>& Registry() {
  static std::vector<TestCase> registry;
  return registry;
}

inline int& FailuresInCurrentTest() {
  static int failures = 0;
  return failures;
}

struct Registrar {
  Registrar(const char* suite, const char* name, void (*body)()) {
    Registry().push_back(TestCase{suite, name, body});
  }
};

// Печать значения в сообщении об ошибке: если у типа нет operator<<,
// выводится заглушка.
template <typename T>
auto Print(std::ostream& os, const T& value, int)
    -> decltype(os << value, void()) {
  os << value;
}
template <typename T>
void Print(std::ostream& os, const T&, long) {
  os << "<значение>";
}

inline void ReportFailure(const char* file, int line,
                          const std::string& message) {
  ++FailuresInCurrentTest();
  std::cout << file << ":" << line << ": Failure\n" << message << std::endl;
}

// Сравнения. Каждый аргумент вычисляется ровно один раз (как в Google Test).
template <typename A, typename B>
bool ReportBinary(bool ok, const A& a, const B& b, const char* expr_a,
                  const char* expr_b, const char* op, const char* file,
                  int line) {
  if (ok) {
    return true;
  }
  std::ostringstream text;
  text << "  Expected: (" << expr_a << ") " << op << " (" << expr_b
       << ")\n    Actual: ";
  Print(text, a, 0);
  text << " vs ";
  Print(text, b, 0);
  ReportFailure(file, line, text.str());
  return false;
}

#define LESSONS_DEFINE_CHECK(Name, op)                                        \
  template <typename A, typename B>                                        \
  bool Name(const A& a, const B& b, const char* expr_a, const char* expr_b, \
            const char* file, int line) {                                  \
    return ReportBinary(a op b, a, b, expr_a, expr_b, #op, file, line);    \
  }
LESSONS_DEFINE_CHECK(CheckEq, ==)
LESSONS_DEFINE_CHECK(CheckNe, !=)
LESSONS_DEFINE_CHECK(CheckLt, <)
LESSONS_DEFINE_CHECK(CheckGt, >)
#undef LESSONS_DEFINE_CHECK

inline bool CheckBool(bool ok, const char* text, bool expected,
                      const char* file, int line) {
  if (ok == expected) {
    return true;
  }
  ReportFailure(file, line,
                std::string("  Expected ") + (expected ? "true" : "false") +
                    ": " + text);
  return false;
}

inline int RunAllTests() {
  int failed_tests = 0;
  std::vector<std::string> failed_names;
  std::cout << "[==========] Running " << Registry().size() << " tests\n";
  for (const TestCase& test : Registry()) {
    std::string full_name = test.suite + "." + test.name;
    FailuresInCurrentTest() = 0;
    std::cout << "[ RUN      ] " << full_name << std::endl;
    test.body();
    bool ok = FailuresInCurrentTest() == 0;
    std::cout << (ok ? "[       OK ] " : "[  FAILED  ] ") << full_name << "\n";
    if (!ok) {
      ++failed_tests;
      failed_names.push_back(full_name);
    }
  }
  std::cout << "[==========] " << Registry().size() << " tests ran\n";
  std::cout << "[  PASSED  ] " << Registry().size() - failed_names.size()
            << " tests\n";
  for (const std::string& name : failed_names) {
    std::cout << "[  FAILED  ] " << name << "\n";
  }
  return failed_tests == 0 ? 0 : 1;
}

}  // namespace testing

#define TEST(suite, name)                                                  \
  static void suite##_##name##_Body();                                     \
  static ::testing::Registrar suite##_##name##_Registrar(                  \
      #suite, #name, &suite##_##name##_Body);                              \
  static void suite##_##name##_Body()

#define EXPECT_EQ(a, b) ::testing::CheckEq((a), (b), #a, #b, __FILE__, __LINE__)
#define EXPECT_NE(a, b) ::testing::CheckNe((a), (b), #a, #b, __FILE__, __LINE__)
#define EXPECT_LT(a, b) ::testing::CheckLt((a), (b), #a, #b, __FILE__, __LINE__)
#define EXPECT_GT(a, b) ::testing::CheckGt((a), (b), #a, #b, __FILE__, __LINE__)
#define EXPECT_TRUE(c) ::testing::CheckBool(!!(c), #c, true, __FILE__, __LINE__)
#define EXPECT_FALSE(c) \
  ::testing::CheckBool(!!(c), #c, false, __FILE__, __LINE__)

// ASSERT_* прерывают текущий тест (тело теста должно возвращать void).
#define ASSERT_TRUE(c) \
  if (!EXPECT_TRUE(c)) return
#define ASSERT_EQ(a, b) \
  if (!EXPECT_EQ(a, b)) return

#define EXPECT_THROW(statement, exception_type)                            \
  do {                                                                     \
    bool caught_ = false;                                                  \
    try {                                                                  \
      statement;                                                           \
    } catch (const exception_type&) {                                      \
      caught_ = true;                                                      \
    } catch (...) {                                                        \
    }                                                                      \
    ::testing::CheckBool(caught_, "throws " #exception_type ": " #statement, \
                         true, __FILE__, __LINE__);                        \
  } while (0)

#define EXPECT_NO_THROW(statement)                                         \
  do {                                                                     \
    bool threw_ = false;                                                   \
    try {                                                                  \
      statement;                                                           \
    } catch (...) {                                                        \
      threw_ = true;                                                       \
    }                                                                      \
    ::testing::CheckBool(!threw_, "no throw: " #statement, true, __FILE__, \
                         __LINE__);                                        \
  } while (0)

#define RUN_ALL_TESTS() ::testing::RunAllTests()

#endif  // LESSONS_TESTS_GTEST_LITE_H_
