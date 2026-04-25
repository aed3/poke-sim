#pragma once

#ifdef POKESIM_USE_COMBINED_SOURCE_AND_HEADER_FILES
#include "../extras/PokeSim.hpp"
#else
#include <PokeSim.hpp>
#endif

#include <Catch2/catch_amalgamated.hpp>

#define BENCHMARK_CASE(name, tags, setup)                           \
  TEST_CASE(name, tags) {                                           \
    BENCHMARK_ADVANCED(name)(Catch::Benchmark::Chronometer meter) { \
      pokesim::Simulation simulation = setup;                       \
      meter.measure([&simulation] { return simulation.run(); });    \
    };                                                              \
  }

class BenchmarkReporter : public Catch::ConsoleReporter {
  bool hasABenchmarkRan = false;
  static inline const std::vector<std::string> TABLE_HEADER = {
    "Name", "Samples", "Iterations/Sample", "Estimated Completion Time", "Mean", "Mean Lower", "Mean Upper", "SD"};
  static inline const std::size_t COLUMN_COUNT = BenchmarkReporter::TABLE_HEADER.size();

  std::size_t currentCell = 0;

  void addCells(const std::vector<std::string>& cells) {
    for (const std::string& cell : cells) {
      m_config->stream() << "| " << cell << " ";
      currentCell++;
      if (currentCell == COLUMN_COUNT) {
        m_config->stream() << "|\n";
        currentCell = 0;
      }
    }
  }

  void flush() { m_config->stream().flush(); }

  static std::string nanosecondsToString(double nanoseconds) {
    if (10e3 > nanoseconds) {
      return std::to_string(nanoseconds) + "ns";
    }
    if (10e6 > nanoseconds) {
      return std::to_string(nanoseconds / 10e3) + "us";
    }
    return std::to_string(nanoseconds / 10e6) + "ms";
  }

 public:
  using Catch::ConsoleReporter::ConsoleReporter;

  static std::string getDescription() { return "Reporter for showing benchmark results in a markdown format."; }

  void benchmarkPreparing(std::string const& name) override {
    if (!hasABenchmarkRan) {
      addCells(BenchmarkReporter::TABLE_HEADER);
      addCells(std::vector<std::string>{BenchmarkReporter::COLUMN_COUNT, "---"});
      hasABenchmarkRan = true;
    }
    addCells({name});
    flush();
  }

  void benchmarkStarting(Catch::BenchmarkInfo const& info) override {
    addCells({
      std::to_string(info.samples),
      std::to_string(info.iterations),
      nanosecondsToString(info.estimatedDuration),
    });
    flush();
  }

  void benchmarkEnded(Catch::BenchmarkStats<> const& stats) override {
    const auto& mean = stats.mean;
    addCells({
      nanosecondsToString(mean.point.count()),
      nanosecondsToString(mean.lower_bound.count()),
      nanosecondsToString(mean.upper_bound.count()),
      nanosecondsToString(stats.standardDeviation.point.count()),
    });
    flush();
  }

  void assertionStarting(Catch::AssertionInfo const&) override {}
  bool assertionEnded(Catch::AssertionStats const& stats) override {
    if (!stats.assertionResult.isOk()) {
      return Catch::ConsoleReporter::assertionEnded(stats);
    }
    return false;
  }
  void sectionStarting(Catch::SectionInfo const&) override {}
  void sectionEnded(Catch::SectionStats const&) override {}
};
