#include "console.h"

#include <clocale>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

void SetUpConsole() {
#ifdef _WIN32
  std::setlocale(LC_ALL, "Russian");
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);
#endif
}
