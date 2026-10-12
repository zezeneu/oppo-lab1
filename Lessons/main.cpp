#include <iostream>
#include <string>
#include <vector>

#include "console.h"
#include "lesson.h"
#include "lesson_filtering.h"
#include "lesson_io.h"
#include "lesson_sorting.h"

// Просит ввести имя преподавателя и печатает его занятия.
void FilterAndPrint(const std::vector<Lesson>& lessons) {
  std::cout << std::endl << "Введите имя преподавателя для фильтра: ";
  std::string teacher_name;
  std::getline(std::cin, teacher_name);
  PrintSection("Занятия преподавателя \"" + teacher_name + "\":",
               FilterByTeacher(lessons, teacher_name));
}

void PrintPrompt() {
  std::cout << "Вводите занятия по одному в строке:" << std::endl
            << "гггг.мм.дд чч:мм \"Имя преподавателя\"" << std::endl
            << "Пустая строка завершает ввод." << std::endl;
}

int main() {
  SetUpConsole();
  PrintPrompt();
  std::vector<Lesson> lessons = ReadAllLessons(std::cin);
  PrintSection("Все занятия (в порядке ввода):", lessons);
  SortByTime(lessons);
  PrintSection("Занятия по возрастанию времени:", lessons);
  FilterAndPrint(lessons);
  return 0;
}
