#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Simulation/SimulationOptions.hpp>
#include <Types/Decisions.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>

namespace pokesim {
struct Accuracy;
struct ActionQueueItem;
struct ActionQueue;
struct AddedTargets;
struct BaseEffectChance;
struct BasePower;
struct AtkBoost;
struct DefBoost;
struct SpaBoost;
struct SpdBoost;
struct SpeBoost;
struct ChoiceLock;
struct CloneTo;
struct CurrentActionMoveSlot;
struct Damage;
struct DamageRollModifiers;
struct DamageRolls;
struct DisabledMoveSlots;
struct Evs;
struct Ivs;
struct Battle;
struct ParentBattle;
struct RootBattle;
struct ParentEntity;
struct CurrentAction;
struct CurrentActionTargets;
struct CurrentActionSource;
struct CurrentActionTarget;
struct FailedCurrentActionSource;
struct FailedCurrentActionTarget;
struct CurrentActionMovesAsSource;
struct CurrentActionMovesAsTarget;
struct CurrentEffectSource;
struct CurrentEffectTarget;
struct CurrentEffectsAsSource;
struct CurrentEffectsAsTarget;
struct FaintQueue;
struct FoeSide;
struct RecycledAction;
struct RecycledActionMove;
struct AddedRecycledActionMove1;
struct AddedRecycledActionMove2;
struct Side;
struct Sides;
struct Team;
struct EventModifier;
struct FoesRemaining;
struct HitCount;
struct Id;
struct LastUsedMove;
struct Level;
struct MoveSlot;
struct MoveSlots;
struct AbilityName;
struct GenderName;
struct ItemName;
struct MoveName;
struct NatureName;
struct PseudoWeatherName;
struct SideConditionName;
struct SourceSlotName;
struct SpeciesName;
struct StatName;
struct StatusName;
struct TargetSlotName;
struct TerrainName;
struct TypeName;
struct VolatileName;
struct WeatherName;
struct PlayerSide;
struct PrimaryAbility;
struct SecondaryAbility;
struct HiddenAbility;
struct BaseStats;
struct Pp;
struct Position;
struct MovePriority;
struct Probability;
struct RngSeed;
struct SideDecision;
struct SpeedTieIndexes;
struct SpeciesTypes;
struct Turn;
struct Winner;
namespace analyze_effect {
struct EffectTarget;
struct EffectMove;
struct GroupedInputs;
struct Inputs;
struct OriginalInputEntities;
struct MovePair;
struct SkippedInputCount;
struct RemovedEffect;
struct EffectMultiplier;
}  // namespace analyze_effect
namespace calc_damage {
struct CritChanceDivisor;
struct CritBoost;
struct RealEffectiveStat;
struct DamageFormulaVariables;
struct UsesUntilKo;
struct AttackerHpRecovered;
struct AttackerHpLost;
}  // namespace calc_damage
namespace internal {
template <types::eventPossibilities>
struct RandomEventChances;
struct RandomBinaryProbability;
struct RandomEventCount;
template <types::eventPossibilities>
struct RandomEventChancesStack;
struct RandomBinaryProbabilityStack;
struct RandomEventCountStack;
struct RandomEqualChanceStack;
struct RandomEventIndex;
}  // namespace internal
namespace action {
struct Team;
}  // namespace action
namespace simulate_turn {
struct TurnOutcomeBattles;
}  // namespace simulate_turn
namespace stat {
struct Hp;
struct Atk;
struct Def;
struct Spa;
struct Spd;
struct Spe;
struct CurrentHp;
struct EffectiveAtk;
struct EffectiveDef;
struct EffectiveSpa;
struct EffectiveSpd;
struct EffectiveSpe;
}  // namespace stat
}  // namespace pokesim

namespace pokesim::debug {
template <typename T>
void check(const T&, const types::registry&) {}
template <typename T>
void check(const T&) {}

void checkBattle(types::entity, const types::registry&);
void checkSide(types::entity, const types::registry&);
void checkPokemon(types::entity, const types::registry&);
void checkActionMove(types::entity, const types::registry&);
void checkPercentChance(types::percentChance);

template <>
void check(const Accuracy&);

template <>
void check(const ActionQueueItem&);

template <>
void check(const ActionQueue&);

template <>
void check(const AddedTargets&);

template <>
void check(const analyze_effect::EffectTarget&, const types::registry&);

template <>
void check(const analyze_effect::EffectMove&);

template <>
void check(const analyze_effect::GroupedInputs&, const types::registry&);

template <>
void check(const analyze_effect::Inputs&, const types::registry&);

// template <> void check(const analyze_effect::OriginalInputEntities&, const types::registry&);

template <>
void check(const analyze_effect::MovePair&, const types::registry&);

// template <> void check(const analyze_effect::SkippedInputCount&);

// template <> void check(const analyze_effect::RemovedEffect&);

template <>
void check(const BaseEffectChance&);

template <>
void check(const BasePower&);

template <>
void check(const AtkBoost&);

template <>
void check(const DefBoost&);

template <>
void check(const SpaBoost&);

template <>
void check(const SpdBoost&);

template <>
void check(const SpeBoost&);

template <>
void check(const calc_damage::CritChanceDivisor&);

template <>
void check(const calc_damage::CritBoost&);

template <>
void check(const calc_damage::RealEffectiveStat&);

template <>
void check(const calc_damage::DamageFormulaVariables&);

template <>
void check(const ChoiceLock&);

// template <> void check(const CloneTo&);

template <>
void check(const CurrentActionMoveSlot&);

template <>
void check(const Damage&);

template <>
void check(const DamageRollModifiers&);

template <>
void check(const DamageRolls&);

template <>
void check(const DisabledMoveSlots&);

template <>
void check(const Evs&);

template <>
void check(const Ivs&);

template <>
void check(const Battle&, const types::registry&);

template <>
void check(const ParentBattle&, const types::registry&);

template <>
void check(const RootBattle&, const types::registry&);

template <>
void check(const ParentEntity&, const types::registry&);

template <>
void check(const CurrentAction&, const types::registry&);

template <>
void check(const CurrentActionTargets&, const types::registry&);

template <>
void check(const CurrentActionSource&, const types::registry&);

template <>
void check(const CurrentActionTarget&, const types::registry&);

template <>
void check(const FailedCurrentActionSource&, const types::registry&);

template <>
void check(const FailedCurrentActionTarget&, const types::registry&);

template <>
void check(const CurrentActionMovesAsSource&, const types::registry&);

template <>
void check(const CurrentActionMovesAsTarget&, const types::registry&);

template <>
void check(const CurrentEffectSource&, const types::registry&);

template <>
void check(const CurrentEffectTarget&, const types::registry&);

template <>
void check(const CurrentEffectsAsSource&, const types::registry&);

template <>
void check(const CurrentEffectsAsTarget&, const types::registry&);

template <>
void check(const FaintQueue&, const types::registry&);

template <>
void check(const FoeSide&, const types::registry&);

template <>
void check(const RecycledAction&, const types::registry&);

template <>
void check(const RecycledActionMove&, const types::registry&);

template <>
void check(const AddedRecycledActionMove1&, const types::registry&);

template <>
void check(const AddedRecycledActionMove2&, const types::registry&);

template <>
void check(const Side&, const types::registry&);

template <>
void check(const Sides&, const types::registry&);

template <>
void check(const Team&, const types::registry&);

// template <> void check(const EventModifier&);

template <>
void check(const FoesRemaining&);

template <>
void check(const HitCount&);

// template <> void check(const Id&);

template <>
void check(const LastUsedMove&);

template <>
void check(const Level&);

template <>
void check(const MoveSlot&);

template <>
void check(const MoveSlots&);

template <>
void check(const AbilityName&);

template <>
void check(const GenderName&);

template <>
void check(const ItemName&);

template <>
void check(const MoveName&);

template <>
void check(const NatureName&);

template <>
void check(const PseudoWeatherName&);

template <>
void check(const SideConditionName&);

template <>
void check(const SourceSlotName&);

template <>
void check(const SpeciesName&);

template <>
void check(const StatName&);

template <>
void check(const StatusName&);

template <>
void check(const TargetSlotName&);

template <>
void check(const TerrainName&);

template <>
void check(const TypeName&);

template <>
void check(const VolatileName&);

template <>
void check(const WeatherName&);

template <>
void check(const PlayerSide&);

template <>
void check(const PrimaryAbility&);

template <>
void check(const SecondaryAbility&);

template <>
void check(const HiddenAbility&);

template <>
void check(const BaseStats&);

template <>
void check(const Pp&);

template <>
void check(const Position&);

template <>
void check(const MovePriority&);

template <>
void check(const Probability&);

template <>
void check(const RngSeed&);

template <>
void check(const internal::RandomEventChances<2U>&);

template <>
void check(const internal::RandomEventChances<3U>&);

template <>
void check(const internal::RandomEventChances<4U>&);

template <>
void check(const internal::RandomEventChances<5U>&);

template <>
void check(const internal::RandomBinaryProbability&);

template <>
void check(const internal::RandomEventCount&);

template <>
void check(const internal::RandomEventChancesStack<2U>&, const types::registry&);

template <>
void check(const internal::RandomEventChancesStack<3U>&, const types::registry&);

template <>
void check(const internal::RandomEventChancesStack<4U>&, const types::registry&);

template <>
void check(const internal::RandomEventChancesStack<5U>&, const types::registry&);

template <>
void check(const internal::RandomBinaryProbabilityStack&, const types::registry&);

template <>
void check(const internal::RandomEventCountStack&, const types::registry&);

template <>
void check(const internal::RandomEqualChanceStack&, const types::registry&);

// template <> void check(const internal::RandomEventIndex&);

template <>
void check(const SideDecision&);

template <>
void check(const SpeedTieIndexes&);

template <>
void check(const action::Team&);

template <>
void check(const simulate_turn::TurnOutcomeBattles&, const types::registry&);

template <>
void check(const calc_damage::UsesUntilKo&);

template <>
void check(const calc_damage::AttackerHpRecovered&);

template <>
void check(const calc_damage::AttackerHpLost&);

template <>
void check(const analyze_effect::EffectMultiplier&);

template <>
void check(const SpeciesTypes&);

template <>
void check(const stat::Hp&);

template <>
void check(const stat::Atk&);

template <>
void check(const stat::Def&);

template <>
void check(const stat::Spa&);

template <>
void check(const stat::Spd&);

template <>
void check(const stat::Spe&);

template <>
void check(const stat::CurrentHp&);

template <>
void check(const stat::EffectiveAtk&);

template <>
void check(const stat::EffectiveDef&);

template <>
void check(const stat::EffectiveSpa&);

template <>
void check(const stat::EffectiveSpd&);

template <>
void check(const stat::EffectiveSpe&);

template <>
void check(const Turn&);

template <>
void check(const Winner&);

template <>
void check(const types::slotDecision&);

template <>
void check(const DamageRollKind&);

template <>
void check(const DamageRollOptions&);
}  // namespace pokesim::debug

#endif
