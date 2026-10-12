#include <iostream>

#include "application.h"
#include "console.h"

int main() {
  SetUpConsole();
  Application application(std::cin, std::cout, std::cerr);
  return application.Run();
}
