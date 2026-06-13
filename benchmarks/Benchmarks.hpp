#pragma once

#ifdef POKESIM_USE_COMBINED_SOURCE_AND_HEADER_FILES
#include "../extras/PokeSim.hpp"
#else
#include <PokeSim.hpp>
#endif

#include <Catch2/catch_amalgamated.hpp>

#ifdef _MSC_VER
// In theory, disabling a warning about conversions is a bad idea. That warning, in theory, should also be triggered
// regardless of whether a value is being assigned to an integer or an optional holding that integer. That second theory
// isn't true, which means making the parameters to BattleCreationInfo optional reveals issues I hadn't previously
// seen. I'm disabling this warning here for now until I decide on a better way to deal with that warning besides
// writing "(std::uint8_t)" everywhere. I wouldn't mind that solution, but BattleCreationInfo is a public API and I
// don't want library users to have to do that a bunch. Maybe a custom class like I did for variant, maybe a literal to
// define an uint8 directly, or maybe something else.
// TODO(aed3): Fix this properly.
#pragma warning(disable : 4244)
#endif

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
  static Simulation run(types::rngState&, Pokedex& pokedex) { return Simulation{pokedex, BattleFormat::SINGLES}; }
};

struct CreateDoubleBattleSimulation : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {"DoubleBattle"};
  static Simulation run(types::rngState&, Pokedex& pokedex) { return Simulation{pokedex, BattleFormat::DOUBLES}; }
};

struct ChooseMonteCarloOptions : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {"MonteCarlo"};
  static void run(types::rngState&, Simulation& simulation) {
    auto& options = simulation.simulateTurnOptions;

    options.setApplyChangesToInputBattle(true)
      .setDamageRollsConsidered({DamageRollKind::ALL_DAMAGE_ROLLS})
      .setMakeBranchesOnRandomEvents(false);
  }
};

struct ChooseRandomBranchingOptions : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {"Branching"};
  static constexpr types::entityIndex MAX_INPUTS = 1U << 14U;
  static void run(types::rngState&, Simulation& simulation) {
    simulation.simulateTurnOptions.setApplyChangesToInputBattle(true)
      .setDamageRollsConsidered({DamageRollKind::AVERAGE_DAMAGE})
      .setMakeBranchesOnRandomEvents(true);
  }
};

struct ChooseCalcDamageOptions : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {};
  static void run(types::rngState&, Simulation& simulation) {
    simulation.calculateDamageOptions.setCalculateUpToFoeHp(true).setNoKoChanceCalculation(false).setDamageRollOptions(
      {DamageRollKind::ALL_DAMAGE_ROLLS});
  }
};

struct ChooseAnalyzeEffectOptions : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {};
  static void run(types::rngState&, Simulation& simulation) {
    simulation.analyzeEffectOptions.setCalculateUpToFoeHp(true)
      .setNoKoChanceCalculation(false)
      .setReconsiderActiveEffects(true)
      .setDamageRollOptions({DamageRollKind::ALL_DAMAGE_ROLLS});
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
    "Iterations / Sample",
    "Estimated Completion Time",
    "Mean of Samples",
    "Standard Deviation",
    "Mean / Input Count",
    "Mean Lower Bound / Input Count",
    "Mean Upper Bound / Input Count",
  };
  static inline const std::size_t COLUMN_COUNT = BenchmarkReporter::TABLE_HEADER.size();

  std::size_t currentCell = 0U;
  std::vector<std::pair<std::string, std::string>> failedBenchmarks;
  std::string currentInputSize;

  void addCells(const std::vector<std::string>& cells) {
    for (std::string cell : cells) {
      int cellSizeDiff = (int)(TABLE_HEADER[currentCell].size() - cell.size());
      if (cellSizeDiff > 0) {
        cell.append(cellSizeDiff, ' ');
      }
      m_stream << "| " << cell << " ";
      currentCell++;
      if (currentCell == COLUMN_COUNT) {
        nextTableLine();
      }
    }
  }

  void nextTableLine() {
    if (currentCell) {
      m_stream << "|\n";
      currentCell = 0;
    }
  }

  void flush() { m_stream.flush(); }

  static std::string nanosecondsToString(double nanoseconds) {
    double value = nanoseconds;
    std::string units = "ns";
    if (nanoseconds >= 1e6) {
      value = nanoseconds / 1e6;
      units = "ms";
    }
    else if (nanoseconds >= 1e3) {
      value = nanoseconds / 1e3;
      units = "us";
    }

    std::stringstream stream;
    stream << std::fixed << value << units;
    return stream.str();
  }

  static std::string inputSizeFromName(const std::string& name) {
    auto numberStart = name.find_first_of('[') + 1U;
    return name.substr(numberStart, name.size() - numberStart - 1U);
  }

 public:
  BenchmarkReporter(Catch::ReporterConfig&& config) : Catch::StreamingReporterBase(std::move(config)) {}

  static std::string getDescription() { return "Reporter for showing benchmark results in a markdown format."; }

  void testCaseStarting(const Catch::TestCaseInfo& info) override {
    Catch::StreamingReporterBase::testCaseStarting(info);
    m_stream << "## " << info.name << "\n";
    addCells(BenchmarkReporter::TABLE_HEADER);
    addCells(std::vector<std::string>{BenchmarkReporter::COLUMN_COUNT, "---"});
    flush();
  }

  void testCaseEnded(const Catch::TestCaseStats& stats) override {
    Catch::StreamingReporterBase::testCaseEnded(stats);
    if (!failedBenchmarks.empty()) {
      m_stream << "\n### Failed Benchmarks:\n";
      for (auto& [inputSize, error] : failedBenchmarks) {
        m_stream << "- Input Size " << inputSize << ": " << error << "\n";
      }
      failedBenchmarks.clear();
    }
    m_stream << "\n\n";
  }

  void benchmarkPreparing(Catch::StringRef name) override {
    Catch::StreamingReporterBase::benchmarkPreparing(name);
    currentInputSize = inputSizeFromName(name.data());
    addCells({currentInputSize});
    flush();
  }

  void benchmarkStarting(const Catch::BenchmarkInfo& info) override {
    Catch::StreamingReporterBase::benchmarkStarting(info);
    addCells({
      std::to_string(info.samples),
      std::to_string(info.iterations),
      nanosecondsToString(info.estimatedDuration),
    });
    flush();
  }

  void benchmarkFailed(Catch::StringRef error) override {
    failedBenchmarks.push_back({currentInputSize, error.data()});
    nextTableLine();
  }

  void benchmarkEnded(const Catch::BenchmarkStats<>& stats) override {
    Catch::StreamingReporterBase::benchmarkEnded(stats);
    int inputSize = std::stoi(inputSizeFromName(stats.info.name));
    const auto& mean = stats.mean;
    addCells({
      nanosecondsToString(mean.point.count()),
      nanosecondsToString(stats.standardDeviation.point.count()),
      nanosecondsToString(mean.point.count() / inputSize),
      nanosecondsToString(mean.lower_bound.count() / inputSize),
      nanosecondsToString(mean.upper_bound.count() / inputSize),
    });
    nextTableLine();
    flush();
  }

  void testRunStarting(const Catch::TestRunInfo& testRunInfo) override {
    Catch::StreamingReporterBase::testRunStarting(testRunInfo);
    if (m_config->testSpec().hasFilters()) {
      m_stream << m_colour->guardColour(Catch::Colour::BrightYellow) << "Filters: " << m_config->testSpec() << '\n';
    }
    flush();
  }

  void assertionEnded(const Catch::AssertionStats& stats) override {
    const Catch::AssertionResult& result = stats.assertionResult;
    Catch::ResultWas::OfType resultType = result.getResultType();

    if (result.isOk() && resultType != Catch::ResultWas::Warning) {
      return;
    }

    nextTableLine();

    m_stream << "\n### Assertion ";
    switch (resultType) {
      case Catch::ResultWas::ThrewException: {
        m_stream << "Threw an Exception";
        break;
      }
      case Catch::ResultWas::FatalErrorCondition: {
        m_stream << "Fatally Failed";
        break;
      }
      case Catch::ResultWas::Warning: {
        m_stream << "Warned";
        break;
      }
      case Catch::ResultWas::ExplicitFailure: {
        m_stream << "Failed Intentionally";
        break;
      }
      default: {
        m_stream << "Failed";
        break;
      }
    }
    m_stream << ":\n" << result.getSourceInfo();
    if (result.hasExpression()) {
      m_stream << "\n" << result.getExpressionInMacro();
    }
    if (result.hasExpandedExpression()) {
      m_stream << "\n  " << result.getExpandedExpression();
    }
    for (const auto& message : stats.infoMessages) {
      if (message.type != Catch::ResultWas::Info) {
        m_stream << "\n- " << message.message;
      }
    }

    m_stream << "\n\n";
  }
};
