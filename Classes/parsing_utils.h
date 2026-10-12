/// @file
/// @brief Небольшие функции для разбора и проверки текста.
#ifndef LESSONS_PARSING_UTILS_H_
#define LESSONS_PARSING_UTILS_H_

#include <string>

/// Убирает пробельные символы в начале и в конце текста.
std::string Trim(const std::string& text);

/// Бросает RangeError, если value не лежит в диапазоне [min, max].
void CheckRange(int value, int min, int max, const std::string& what);

#endif  // LESSONS_PARSING_UTILS_H_
