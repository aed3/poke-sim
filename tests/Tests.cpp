#include "Tests.hpp"

int main(int argc, char* argv[]) {
  Catch::Session session;
  if (session.applyCommandLine(argc, argv)) return 1;

  return session.run();
}
