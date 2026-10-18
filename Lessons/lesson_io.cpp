#include "lesson_io.h"

#include <iostream>
#include <stdexcept>

namespace {

// Разбирает строку; при ошибке сообщает о ней (с номером строки в файле)
// и ничего не добавляет.
void TryAddLesson(const std::string& line, int number,
                  std::vector<Lesson>& lessons) {
  try {
    lessons.push_back(ParseLesson(line));
  } catch (const std::runtime_error& error) {
    std::cerr << "Строка " << number << " пропущена: " << error.what()
              << std::endl;
  }
}

// Пустая строка или строка из одних пробелов.
bool IsBlank(const std::string& line) {
  return line.find_first_not_of(" \t\r") == std::string::npos;
}

// Убирает метку UTF-8 BOM (EF BB BF), которую некоторые редакторы
// добавляют в начало файла.
void StripBom(std::string& line) {
  if (line.compare(0, 3, "\xEF\xBB\xBF") == 0) {
    line.erase(0, 3);
  }
}

}  // namespace

std::vector<Lesson> ReadAllLessons(std::istream& in) {
  std::vector<Lesson> lessons;
  std::string line;
  int number = 0;
  while (std::getline(in, line)) {
    if (++number == 1) {
      StripBom(line);
    }
    if (!IsBlank(line)) {
      TryAddLesson(line, number, lessons);
    }
  }
  return lessons;
}

void PrintLesson(const Lesson& lesson) {
  std::cout << FormatDate(lesson.date) << "  " << FormatTime(lesson.time)
            << "  " << lesson.teacher_name << std::endl;
}

void PrintSection(const std::string& title,
                  const std::vector<Lesson>& lessons) {
  std::cout << std::endl << title << std::endl;
  if (lessons.empty()) {
    std::cout << "(нет занятий)" << std::endl;
  }
  for (const Lesson& lesson : lessons) {
    PrintLesson(lesson);
  }
}
