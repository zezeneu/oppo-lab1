#include <iostream>

#include "application.h"
#include "console.h"

int main() {  // fix-a and fix-b
  SetUpConsole();
  // Отключает синхронизацию с C-потоками: ввод и вывод заметно быстрее.
  std::ios::sync_with_stdio(false);
  Application application(std::cin, std::cout, std::cerr);
  return application.Run();
}
