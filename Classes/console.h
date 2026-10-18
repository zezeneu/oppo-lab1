/// @file
/// @brief Настройка консоли.
#ifndef LESSONS_CONSOLE_H_
#define LESSONS_CONSOLE_H_

/// Включает кодировку Windows-1251 в консоли Windows, чтобы кириллица
/// вводилась и выводилась корректно. На других системах ничего не делает.
void SetUpConsole();

#endif  // LESSONS_CONSOLE_H_
