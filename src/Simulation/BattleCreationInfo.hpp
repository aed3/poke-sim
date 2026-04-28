#pragma once

#include <Components/Decisions.hpp>
#include <Components/EVsIVs.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Types/Effect.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Gender.hpp>
#include <Types/Enums/Nature.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Stat.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Move.hpp>
#include <vector>

namespace pokesim {
struct MoveCreationInfo {
  dex::Move name = dex::Move::NO_MOVE;
  types::pp pp = MechanicConstants::MovePp::MIN;
  types::pp maxPp = MechanicConstants::MoveMaxPp::MIN;
};

struct PokemonCreationInfo {
  std::optional<types::stateId> id = std::nullopt;
  dex::Species species = dex::Species::NO_SPECIES;
  dex::Item item = dex::Item::NO_ITEM;
  dex::Ability ability = dex::Ability::NO_ABILITY;
  dex::Gender gender = dex::Gender::NO_GENDER;
  types::level level = MechanicConstants::PokemonLevel::MIN;

  dex::Nature nature = dex::Nature::NO_NATURE;
  Evs evs;
  Ivs ivs;
  struct {
    types::stat hp = MechanicConstants::PokemonHpStat::MIN;
    types::stat atk = MechanicConstants::PokemonStat::MIN;
    types::stat def = MechanicConstants::PokemonStat::MIN;
    types::stat spa = MechanicConstants::PokemonStat::MIN;
    types::stat spd = MechanicConstants::PokemonStat::MIN;
    types::stat spe = MechanicConstants::PokemonStat::MIN;
  } stats;

  std::vector<MoveCreationInfo> moves{};

  std::optional<types::stat> currentHp = std::nullopt;
  std::optional<SpeciesTypes> currentTypes = std::nullopt;
  dex::Status status = dex::Status::NO_STATUS;

  struct {
    std::optional<types::boost> atk = std::nullopt;
    std::optional<types::boost> def = std::nullopt;
    std::optional<types::boost> spa = std::nullopt;
    std::optional<types::boost> spd = std::nullopt;
    std::optional<types::boost> spe = std::nullopt;
  } currentBoosts;
};

struct SideCreationInfo {
  std::vector<PokemonCreationInfo> team;
};

struct TurnDecisionInfo {
  SideDecision p1;
  SideDecision p2;
};

struct CalcDamageInputInfo {
  Slot attackerSlot = Slot::NONE;
  Slot defenderSlot = Slot::NONE;
  std::vector<dex::Move> moves;
};

struct AnalyzeEffectInputInfo {
 private:
  struct BoostInfo {
    dex::Stat stat = dex::Stat::ATK;
    types::boost boost = MechanicConstants::PokemonStatBoost::BASE;
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
  types::battleTurn turn = MechanicConstants::TurnCount::MIN;
  std::optional<types::rngState> rngSeed = std::nullopt;
  types::probability probability = MechanicConstants::Probability::MAX;

  SideCreationInfo p1;
  SideCreationInfo p2;

  std::vector<TurnDecisionInfo> decisionsToSimulate;
  std::vector<CalcDamageInputInfo> damageCalculations;
  std::vector<AnalyzeEffectInputInfo> effectsToAnalyze;
};
}  // namespace pokesim
