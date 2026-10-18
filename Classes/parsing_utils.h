/// @file
/// @brief Небольшие функции для разбора и проверки текста.
#ifndef LESSONS_PARSING_UTILS_H_
#define LESSONS_PARSING_UTILS_H_

#include <string>

/// Убирает в начале текста метку порядка байтов UTF-8 (EF BB BF), которую
/// добавляют некоторые редакторы (например, Блокнот Windows) в начало файла.
void StripUtf8Bom(std::string* text);

/// Убирает пробельные символы в начале и в конце текста.
std::string Trim(const std::string& text);

/// Записывает неотрицательное число, дополняя слева нулями до width знаков
/// (число длиннее width не обрезается): ZeroPad(7, 2) -> "07".
/// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std::string ZeroPad(int value, size_t width);

/// Бросает RangeError, если value не лежит в диапазоне [min, max].
void CheckRange(int value, int min, int max, const std::string& what);

#endif  // LESSONS_PARSING_UTILS_H_
