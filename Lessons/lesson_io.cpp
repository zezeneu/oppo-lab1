#include "lesson_io.h"

#include <iostream>
#include <stdexcept>

namespace {

// Разбирает строку; при ошибке сообщает о ней и ничего не добавляет.
void TryAddLesson(const std::string& line, std::vector<Lesson>& lessons) {
  try {
    lessons.push_back(ParseLesson(line));
  } catch (const std::runtime_error& error) {
    std::cerr << "Пропущена строка: " << error.what() << std::endl;
  }
}

}  // namespace

std::vector<Lesson> ReadAllLessons(std::istream& in) {
  std::vector<Lesson> lessons;
  std::string line;
  while (std::getline(in, line) && !line.empty()) {
    TryAddLesson(line, lessons);
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
