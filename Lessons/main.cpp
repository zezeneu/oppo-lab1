#include <chrono>
#include <clocale>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

struct Lesson {
  std::chrono::year_month_day date;
  int hours;
  int minutes;
  std::string teacher_name;
};

// Включает кодировку Windows-1251 в консоли Windows, чтобы кириллица
// вводилась и выводилась корректно. На других системах ничего не делает.
void SetUpConsole() {
#ifdef _WIN32
  std::setlocale(LC_ALL, "Russian");
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);
#endif
}

// Строка целиком: гггг.мм.дд чч:мм "Имя преподавателя".
// Склеено из трёх частей:
//   дата  [0-9]{4}\.[0-9]{1,2}\.[0-9]{1,2}  (месяц и день – 1 или 2 цифры)
//   время [0-9]{1,2}:[0-9]{1,2}              (часы и минуты – 1 или 2 цифры)
//   имя   "[^"]+"                             (любые символы, кроме кавычки)
// Скобки запоминают куски: год, месяц, день, часы, минуты, имя.
const std::regex& LinePattern() {
  static const std::regex kPattern(
      R"re(\s*([0-9]{4})\.([0-9]{1,2})\.([0-9]{1,2})\s+([0-9]{1,2}):([0-9]{1,2})\s+"([^"]+)"\s*)re");
  return kPattern;
}

// Собирает дату; несуществующую дату (например, 30 февраля) отвергает
// проверка std::chrono::year_month_day::ok().
std::chrono::year_month_day MakeDate(const std::smatch& parts) {
  const std::chrono::year_month_day date{
      std::chrono::year{std::stoi(parts[1])},
      std::chrono::month{static_cast<unsigned>(std::stoi(parts[2]))},
      std::chrono::day{static_cast<unsigned>(std::stoi(parts[3]))}};
  if (!date.ok() || static_cast<int>(date.year()) < 1) {
    throw std::runtime_error("Такой даты нет в календаре: " + parts[0].str());
  }
  return date;
}

// Преобразует строку описания в объект Lesson.
Lesson ParseLesson(const std::string& line) {
  std::smatch parts;
  if (!std::regex_match(line, parts, LinePattern())) {
    throw std::runtime_error(
        "Ожидалась строка вида гггг.мм.дд чч:мм \"Имя преподавателя\"");
  }
  Lesson lesson{MakeDate(parts), std::stoi(parts[4]), std::stoi(parts[5]),
                parts[6]};
  if (lesson.hours > 23 || lesson.minutes > 59) {
    throw std::runtime_error("Недопустимое время (нужно 00:00 – 23:59)");
  }
  return lesson;
}

void PrintLesson(const Lesson& lesson) {
  std::cout << std::setfill('0');
  std::cout << "Дата: " << std::setw(4) << static_cast<int>(lesson.date.year())
            << "." << std::setw(2) << unsigned{lesson.date.month()} << "."
            << std::setw(2) << unsigned{lesson.date.day()} << std::endl;
  std::cout << "Время: " << std::setw(2) << lesson.hours << ":"
            << std::setw(2) << lesson.minutes << std::endl;
  std::cout << "Преподаватель: " << lesson.teacher_name << std::endl;
}

// Читает из файла первую непустую строку (метку BOM в начале файла
// пропускает).
std::string ReadFirstLine(const std::string& path) {
  std::ifstream file(path);
  if (!file) {
    throw std::runtime_error("Не удалось открыть файл: " + path);
  }
  std::string line;
  bool is_first = true;
  while (std::getline(file, line)) {
    if (is_first && line.compare(0, 3, "\xEF\xBB\xBF") == 0) {
      line.erase(0, 3);
    }
    is_first = false;
    if (line.find_first_not_of(" \t\r") != std::string::npos) {
      return line;
    }
  }
  throw std::runtime_error("В файле нет ни одной строки: " + path);
}

// Путь к файлу: первый аргумент программы или lessons.txt рядом с программой.
std::string InputPath(int argc, char** argv) {
  return argc > 1 ? argv[1] : "lessons.txt";
}

int main(int argc, char** argv) {
  SetUpConsole();
  try {
    Lesson lesson = ParseLesson(ReadFirstLine(InputPath(argc, argv)));
    std::cout << "Разобранный объект:" << std::endl;
    PrintLesson(lesson);
  } catch (const std::exception& error) {
    std::cerr << "Ошибка: " << error.what() << std::endl;
    return 1;
  }
  return 0;
}
