#pragma once

#ifdef POKESIM_USE_COMBINED_SOURCE_AND_HEADER_FILES
#include "../extras/PokeSim.hpp"
#else
#include <PokeSim.hpp>
#endif

#include <Catch2/catch_amalgamated.hpp>

namespace pokesim {
inline std::string createBenchmarkName(const std::initializer_list<std::vector<std::string>>& tagLists) {
  std::string name;
  for (const auto& tags : tagLists) {
    for (const auto& tag : tags) {
      name += tag + "-";
    }
  }

  return name.substr(0, name.size() - 1U);
}

inline std::string createTestCaseTags(const std::initializer_list<std::vector<std::string>>& tagLists) {
  std::string name;
  for (const auto& tags : tagLists) {
    for (const auto& tag : tags) {
      name += "[" + tag + "]";
    }
  }

  return name;
}

struct BenchmarkInputHolder {
  static constexpr types::entityIndex MAX_INPUTS = 1U << 17U;
};

using CreatePokedexFn = Pokedex(types::rngState&);
using CreateSimulationFn = Simulation(types::rngState&, Pokedex&);
using ChooseOptionsFn = void(types::rngState&, Simulation&);
using AssignInputsFn = void(types::rngState&, types::entityIndex, Simulation&, Pokedex&);

inline void runBenchmark(
  const std::string& name, types::entityIndex maxInputs, CreatePokedexFn createPokedex,
  CreateSimulationFn createSimulation, ChooseOptionsFn chooseOptions, AssignInputsFn assignInputs) {
  for (types::entityIndex inputCount = 1; inputCount < maxInputs; inputCount *= 2U) {
    types::rngState rngState = 1U;
    DYNAMIC_SECTION(inputCount) {
      BENCHMARK_ADVANCED(name + " [" + std::to_string(inputCount) + "]")(Catch::Benchmark::Chronometer meter) {
        Pokedex pokedex = createPokedex(rngState);
        std::vector<Simulation> simulations;
        simulations.reserve(meter.runs());

        types::rngState rngStateAtSimulationCreation = rngState;
        for (int iteration = 0; iteration < meter.runs(); iteration++) {
          rngState = rngStateAtSimulationCreation;
          Simulation& simulation = simulations.emplace_back(createSimulation(rngState, pokedex));
          chooseOptions(rngState, simulation);
          assignInputs(rngState, inputCount, simulation, pokedex);
        }

        meter.measure([&simulations](int iteration) { simulations[iteration].run(); });

        for (Simulation& simulation : simulations) {
          simulation.registry.clear();
        }
      };
    }
  }
}

struct CreatePokedex : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {"SV"};
  static Pokedex run(types::rngState&) { return Pokedex{GameMechanics::SCARLET_VIOLET}; }
};

struct CreateSingleBattleSimulation : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {"SingleBattle"};
  static Simulation run(types::rngState&, Pokedex& pokedex) {
    return Simulation{pokedex, BattleFormat::SINGLES_BATTLE_FORMAT};
  }
};

struct CreateDoubleBattleSimulation : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {"DoubleBattle"};
  static Simulation run(types::rngState&, Pokedex& pokedex) {
    return Simulation{pokedex, BattleFormat::DOUBLES_BATTLE_FORMAT};
  }
};

struct ChooseMonteCarloOptions : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {"MonteCarlo"};
  static void run(types::rngState&, Simulation& simulation) {
    simulation.simulateTurnOptions.makeBranchesOnRandomEvents = false;
    simulation.simulateTurnOptions.damageRollsConsidered.p1 = DamageRollKind::ALL_DAMAGE_ROLLS;
    simulation.simulateTurnOptions.damageRollsConsidered.p2 = DamageRollKind::ALL_DAMAGE_ROLLS;
  }
};

struct ChooseRandomBranchingOptions : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {"Branching"};
  static constexpr types::entityIndex MAX_INPUTS = 1U << 16U;
  static void run(types::rngState&, Simulation& simulation) {
    simulation.simulateTurnOptions.makeBranchesOnRandomEvents = true;
    simulation.simulateTurnOptions.damageRollsConsidered.p1 = DamageRollKind::AVERAGE_DAMAGE;
    simulation.simulateTurnOptions.damageRollsConsidered.p2 = DamageRollKind::AVERAGE_DAMAGE;
  }
};

#define BENCHMARK_CASE(CreatePokedex, CreateSimulation, ChooseOptions, AssignInputs)                                   \
  TEST_CASE(                                                                                                           \
    createBenchmarkName({CreatePokedex::TAGS, CreateSimulation::TAGS, ChooseOptions::TAGS, AssignInputs::TAGS}),       \
    createTestCaseTags({CreatePokedex::TAGS, CreateSimulation::TAGS, ChooseOptions::TAGS, AssignInputs::TAGS})) {      \
    std::string benchmarkName =                                                                                        \
      createBenchmarkName({CreatePokedex::TAGS, CreateSimulation::TAGS, ChooseOptions::TAGS, AssignInputs::TAGS});     \
    types::entityIndex maxInputs = std::min(                                                                           \
      {CreatePokedex::MAX_INPUTS, CreateSimulation::MAX_INPUTS, ChooseOptions::MAX_INPUTS, AssignInputs::MAX_INPUTS}); \
    runBenchmark(                                                                                                      \
      benchmarkName,                                                                                                   \
      maxInputs,                                                                                                       \
      CreatePokedex::run,                                                                                              \
      CreateSimulation::run,                                                                                           \
      ChooseOptions::run,                                                                                              \
      AssignInputs::run);                                                                                              \
  }
}  // namespace pokesim

class BenchmarkReporter : public Catch::StreamingReporterBase {
  static inline const std::vector<std::string> TABLE_HEADER = {
    "Inputs",
    "Samples",
    "Iterations/Sample",
    "Estimated Completion Time",
    "Found Mean",
    "Mean Lower Bound",
    "Mean Upper Bound",
    "Standard Deviation",
    "Mean/InputCount",
  };
  static inline const std::size_t COLUMN_COUNT = BenchmarkReporter::TABLE_HEADER.size();
  std::size_t currentCell = 0U;

  void addCells(const std::vector<std::string>& cells) {
    for (const std::string& cell : cells) {
      m_stream << "| " << cell << " ";
      currentCell++;
      if (currentCell == COLUMN_COUNT) {
        m_stream << "|\n";
        currentCell = 0;
      }
    }
  }

  void flush() { m_stream.flush(); }

  static std::string nanosecondsToString(double nanoseconds) {
    std::string str;
    if (1e3 > nanoseconds) {
      str = Catch::getFormattedDuration(nanoseconds) + "ns";
    }
    else if (1e6 > nanoseconds) {
      str = Catch::getFormattedDuration(nanoseconds / 1e3) + "us";
    }
    else {
      str = Catch::getFormattedDuration(nanoseconds / 1e6) + "ms";
    }
    return str.insert(0, 14U < str.size() ? 0U : 14U - str.size(), ' ');
  }

  static std::string inputSizeFromName(const std::string& name) {
    auto numberStart = name.find_first_of('[') + 1U;
    return name.substr(numberStart, name.size() - numberStart - 1U);
  }

 public:
  BenchmarkReporter(Catch::ReporterConfig&& config) : Catch::StreamingReporterBase(std::move(config)) {}

  static std::string getDescription() { return "Reporter for showing benchmark results in a markdown format."; }

  void testCaseStarting(Catch::TestCaseInfo const& info) override {
    Catch::StreamingReporterBase::testCaseStarting(info);
    m_stream << "## " << info.name << "\n";
    addCells(BenchmarkReporter::TABLE_HEADER);
    addCells(std::vector<std::string>{BenchmarkReporter::COLUMN_COUNT, "---"});
    flush();
  }

  void testCaseEnded(const Catch::TestCaseStats& stats) override {
    Catch::StreamingReporterBase::testCaseEnded(stats);
    m_stream << "\n\n";
  }

  void benchmarkPreparing(Catch::StringRef name) override {
    Catch::StreamingReporterBase::benchmarkPreparing(name);
    std::string inputSize = inputSizeFromName(name.data());
    addCells({inputSize.append(6U - inputSize.size(), ' ')});
    flush();
  }

  void benchmarkStarting(Catch::BenchmarkInfo const& info) override {
    Catch::StreamingReporterBase::benchmarkStarting(info);
    addCells({
      std::to_string(info.samples),
      std::to_string(info.iterations),
      nanosecondsToString(info.estimatedDuration),
    });
    flush();
  }

  void benchmarkEnded(Catch::BenchmarkStats<> const& stats) override {
    Catch::StreamingReporterBase::benchmarkEnded(stats);
    std::string inputSize = inputSizeFromName(stats.info.name);
    const auto& mean = stats.mean;
    addCells({
      nanosecondsToString(mean.point.count()),
      nanosecondsToString(mean.lower_bound.count()),
      nanosecondsToString(mean.upper_bound.count()),
      nanosecondsToString(stats.standardDeviation.point.count()),
      nanosecondsToString(mean.point.count() / std::stoi(inputSize)),
    });
    flush();
  }

  void testRunStarting(Catch::TestRunInfo const& testRunInfo) override {
    Catch::StreamingReporterBase::testRunStarting(testRunInfo);
    if (m_config->testSpec().hasFilters()) {
      m_stream << m_colour->guardColour(Catch::Colour::BrightYellow) << "Filters: " << m_config->testSpec() << '\n';
    }
    flush();
  }

  void testRunEnded(Catch::TestRunStats const& testRunStats) override {
    Catch::printTestRunTotals(m_stream, *m_colour, testRunStats.totals);
    m_stream << "\n\n";
    flush();
    Catch::StreamingReporterBase::testRunEnded(testRunStats);
  }
};

CATCH_REGISTER_REPORTER("benchmark", BenchmarkReporter)
