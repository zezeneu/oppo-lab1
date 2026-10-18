#include <fstream>
#include <iostream>
#include <string>

#include "application.h"
#include "console.h"

// Путь к файлу с занятиями: первый аргумент программы или lessons.txt рядом с
// программой.
std::string InputPath(int argc, char** argv) {
  return argc > 1 ? argv[1] : "lessons.txt";
}

int main(int argc, char** argv) {
  SetUpConsole();
  const std::string path = InputPath(argc, argv);
  std::ifstream file(path);
  if (!file) {
    std::cerr << "Не удалось открыть файл: " << path << '\n';
    return 1;
  }
  Application application(file, std::cin, std::cout, std::cerr);
  return application.Run();
}
