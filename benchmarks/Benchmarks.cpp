#include "Benchmarks.hpp"

CATCH_REGISTER_REPORTER("benchmark", BenchmarkReporter)

int main(int argc, char* argv[]) {
  Catch::Session session;
  if (session.applyCommandLine(argc, argv)) return 1;

  return session.run();
}
