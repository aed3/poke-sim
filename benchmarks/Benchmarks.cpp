#include "Benchmarks.hpp"

CATCH_REGISTER_REPORTER("benchmark", BenchmarkReporter)

int main(int argc, char* argv[]) {
  Catch::Session session;
  if (session.applyCommandLine(argc, argv)) return 1;
  session.configData().runOrder = Catch::TestRunOrder::Declared;

  return session.run();
}
