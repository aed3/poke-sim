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
    auto& options = simulation.simulateTurnOptions;
    options.applyChangesToInputBattle = true;
    options.damageRollsConsidered.p1 = DamageRollKind::ALL_DAMAGE_ROLLS;
    options.damageRollsConsidered.p2 = DamageRollKind::ALL_DAMAGE_ROLLS;
    options.branchProbabilityLowerLimit = std::nullopt;
    options.randomChanceLowerLimit = std::nullopt;
    options.randomChanceUpperLimit = std::nullopt;

    options.makeBranchesOnRandomEvents = false;
  }
};

struct ChooseRandomBranchingOptions : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {"Branching"};
  static constexpr types::entityIndex MAX_INPUTS = 1U << 16U;
  static void run(types::rngState&, Simulation& simulation) {
    auto& options = simulation.simulateTurnOptions;
    options.applyChangesToInputBattle = true;
    options.damageRollsConsidered.p1 = DamageRollKind::AVERAGE_DAMAGE;
    options.damageRollsConsidered.p2 = DamageRollKind::AVERAGE_DAMAGE;
    options.branchProbabilityLowerLimit = std::nullopt;
    options.randomChanceLowerLimit = std::nullopt;
    options.randomChanceUpperLimit = std::nullopt;

    options.makeBranchesOnRandomEvents = true;
  }
};

struct ChooseCalcDamageOptions : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {};
  static void run(types::rngState&, Simulation& simulation) {
    simulation.calculateDamageOptions.damageRollOptions.p1 = DamageRollKind::ALL_DAMAGE_ROLLS;
    simulation.calculateDamageOptions.damageRollOptions.p2 = DamageRollKind::ALL_DAMAGE_ROLLS;
    simulation.calculateDamageOptions.calculateUpToFoeHp = true;
    simulation.calculateDamageOptions.noKoChanceCalculation = false;
  }
};

struct ChooseAnalyzeEffectOptions : BenchmarkInputHolder {
  inline static const std::vector<std::string> TAGS = {};
  static void run(types::rngState&, Simulation& simulation) {
    simulation.analyzeEffectOptions.damageRollOptions.p1 = DamageRollKind::ALL_DAMAGE_ROLLS;
    simulation.analyzeEffectOptions.damageRollOptions.p2 = DamageRollKind::ALL_DAMAGE_ROLLS;
    simulation.analyzeEffectOptions.calculateUpToFoeHp = true;
    simulation.analyzeEffectOptions.noKoChanceCalculation = false;
    simulation.analyzeEffectOptions.reconsiderActiveEffects = true;
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
