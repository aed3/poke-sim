#pragma once

#include <Components/SideDecisions.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Types/Constants.hpp>
#include <Types/Decisions.hpp>
#include <Types/Effect.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Gender.hpp>
#include <Types/Enums/Nature.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Stat.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <Types/State.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
struct MoveCreationInfo {
  dex::Move name = dex::Move::NO_MOVE;
  std::optional<types::pp> pp = std::nullopt;
  std::optional<types::pp> maxPp = std::nullopt;
};

struct PokemonCreationInfo {
 private:
  struct Evs {
    std::optional<types::ev> hp = std::nullopt;
    std::optional<types::ev> atk = std::nullopt;
    std::optional<types::ev> def = std::nullopt;
    std::optional<types::ev> spa = std::nullopt;
    std::optional<types::ev> spd = std::nullopt;
    std::optional<types::ev> spe = std::nullopt;
  };

  struct Ivs {
    std::optional<types::iv> hp = std::nullopt;
    std::optional<types::iv> atk = std::nullopt;
    std::optional<types::iv> def = std::nullopt;
    std::optional<types::iv> spa = std::nullopt;
    std::optional<types::iv> spd = std::nullopt;
    std::optional<types::iv> spe = std::nullopt;
  };

  struct Stats {
    std::optional<types::stat> hp = std::nullopt;
    std::optional<types::stat> atk = std::nullopt;
    std::optional<types::stat> def = std::nullopt;
    std::optional<types::stat> spa = std::nullopt;
    std::optional<types::stat> spd = std::nullopt;
    std::optional<types::stat> spe = std::nullopt;
  };

  struct Boosts {
    std::optional<types::boost> atk = std::nullopt;
    std::optional<types::boost> def = std::nullopt;
    std::optional<types::boost> spa = std::nullopt;
    std::optional<types::boost> spd = std::nullopt;
    std::optional<types::boost> spe = std::nullopt;
  };

 public:
  dex::Species species = dex::Species::NO_SPECIES;
  std::optional<types::level> level = std::nullopt;
  std::optional<dex::Item> item = std::nullopt;
  std::optional<dex::Ability> ability = std::nullopt;
  std::optional<dex::Gender> gender = std::nullopt;
  std::optional<dex::Nature> nature = std::nullopt;
  std::optional<types::stateId> id = std::nullopt;

  Evs evs{};
  Ivs ivs{};
  Stats stats{};

  std::vector<MoveCreationInfo> moves{};

  std::optional<types::stat> currentHp = std::nullopt;
  std::optional<SpeciesTypes> currentTypes = std::nullopt;
  std::optional<dex::Status> status = std::nullopt;

  Boosts currentBoosts{};
};

struct SideCreationInfo {
  std::vector<PokemonCreationInfo> team;
};

using TurnDecisionInfo = types::sides<SideDecision>;

struct CalcDamageInputInfo {
  Slot attackerSlot = Slot::NONE;
  Slot defenderSlot = Slot::NONE;
  std::vector<dex::Move> moves;
};

struct AnalyzeEffectInputInfo {
 private:
  struct BoostInfo {
    dex::Stat stat = dex::Stat::NONE;
    types::boost boost = Constants::PokemonStatBoost::DEFAULT;
  };

 public:
  Slot attackerSlot = Slot::NONE;
  Slot defenderSlot = Slot::NONE;
  Slot effectTarget = Slot::NONE;
  std::vector<dex::Move> moves;
  std::optional<types::effectEnum> effect = std::nullopt;
  std::optional<BoostInfo> boostEffect = std::nullopt;
};

struct BattleCreationInfo {
  bool runWithSimulateTurn = false;
  bool runWithCalculateDamage = false;
  bool runWithAnalyzeEffect = false;
  std::optional<types::battleTurn> turn = std::nullopt;
  std::optional<types::rngState> rngSeed = std::nullopt;
  std::optional<types::probability> probability = std::nullopt;

  types::sides<SideCreationInfo> sides;

  std::vector<TurnDecisionInfo> decisionsToSimulate;
  std::vector<CalcDamageInputInfo> damageCalculations;
  std::vector<AnalyzeEffectInputInfo> effectsToAnalyze;
};
}  // namespace pokesim
