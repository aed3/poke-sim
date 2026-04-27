#include "Tests.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  Catch::Session session;
  if (session.applyCommandLine(argc, argv)) return 1;

  auto result = session.run();
  if (result == Catch::ResultWas::Ok) {
    std::cout << "Passed " << pokesim::debug::require::count << " debug assertions" << "\n";
  }
  return result;
}
