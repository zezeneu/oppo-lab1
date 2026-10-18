#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "console.h"
#include "lesson.h"
#include "lesson_filtering.h"
#include "lesson_io.h"
#include "lesson_sorting.h"

// Файл с занятиями по умолчанию (лежит рядом с программой).
const char* const kDefaultInputFile = "lessons.txt";

// Просит ввести имя преподавателя и печатает его занятия.
void FilterAndPrint(const std::vector<Lesson>& lessons) {
  std::cout << std::endl << "Введите имя преподавателя для фильтра: ";
  std::string teacher_name;
  std::getline(std::cin, teacher_name);
  PrintSection("Занятия преподавателя \"" + teacher_name + "\":",
               FilterByTeacher(lessons, teacher_name));
}

// Путь к файлу: первый аргумент программы или файл по умолчанию.
std::string InputPath(int argc, char** argv) {
  return argc > 1 ? argv[1] : kDefaultInputFile;
}

int main(int argc, char** argv) {
  SetUpConsole();
  const std::string path = InputPath(argc, argv);
  std::ifstream file(path);
  if (!file) {
    std::cerr << "Не удалось открыть файл: " << path << std::endl;
    return 1;
  }
  std::cout << "Чтение занятий из файла: " << path << std::endl;
  std::vector<Lesson> lessons = ReadAllLessons(file);
  PrintSection("Все занятия (в порядке ввода):", lessons);
  SortByTime(lessons);
  PrintSection("Занятия по возрастанию времени:", lessons);
  FilterAndPrint(lessons);
  return 0;
}
