// Сценарий программы: ввод занятий, сортировка по времени, фильтр по
// преподавателю. Потоки передаются снаружи, поэтому класс можно проверять
// модульными тестами без консоли.
#ifndef LESSONS_APPLICATION_H_
#define LESSONS_APPLICATION_H_

#include <istream>
#include <ostream>

#include "lesson_list.h"
#include "lesson_printer.h"
#include "lesson_reader.h"

class Application {
 public:
  Application(std::istream& lessons_in, std::istream& console_in,
              std::ostream& out, std::ostream& err);

  // Выполняет сценарий. Возвращает 0; ошибочные строки ввода не считаются
  // аварией: они выводятся в err, а программа продолжает работу.
  int Run();

 private:
  LessonList ReadLessons();
  void ShowFiltered(const LessonList& lessons);

  std::istream& lessons_in_;  // откуда читаем занятия (файл)
  std::istream& console_in_;  // откуда читаем имя для фильтра (консоль)
  std::ostream& out_;
  LessonReader reader_;
  LessonPrinter printer_;
  LessonPrinter error_printer_;
};

#endif  // LESSONS_APPLICATION_H_
