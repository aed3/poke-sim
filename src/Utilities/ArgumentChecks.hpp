#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Simulation/SimulationOptions.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>

namespace pokesim {
struct Accuracy;
struct AddedTargets;
struct BaseEffectChance;
struct BasePower;
struct AtkBoost;
struct DefBoost;
struct SpaBoost;
struct SpdBoost;
struct SpeBoost;
struct CloneTo;
struct Damage;
struct DamageRollModifiers;
struct DamageRolls;
struct SlotDecision;
struct SideDecision;
struct Evs;
struct Ivs;
struct ActionQueue;
struct Battle;
struct ParentBattle;
struct RootBattle;
struct ParentEntity;
struct ChoiceLock;
struct CurrentAction;
struct NextAction;
struct CurrentActionTargets;
struct CurrentActionTarget;
struct CurrentActionSource;
struct CurrentActionMovesAsTarget;
struct CurrentActionMovesAsSource;
struct CurrentActionMoveSlot;
struct CurrentEffectTarget;
struct CurrentEffectSource;
struct CurrentEffectsAsTarget;
struct CurrentEffectsAsSource;
struct FoeSide;
struct LastUsedMove;
struct MoveSlots;
struct Pokemon;
struct Side;
struct Sides;
struct Team;
struct EventModifier;
struct HitCount;
struct Id;
struct Level;
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
struct Pp;
struct MaxPp;
struct PlayerSide;
struct PrimaryAbility;
struct SecondaryAbility;
struct HiddenAbility;
struct BaseStats;
struct Position;
struct MovePriority;
struct Probability;
struct RngSeed;
template <types::eventPossibilities>
struct RandomEventChances;
struct RandomBinaryChance;
struct RandomEventCount;
template <types::eventPossibilities>
struct RandomEventChancesStack;
struct RandomBinaryChanceStack;
struct RandomEventCountStack;
struct RandomEqualChanceStack;
struct RandomEventIndex;
struct SpeedTieIndexes;
struct SpeciesTypes;
struct SpeedSort;
struct Turn;
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
struct AttackingLevel;
struct AttackingStat;
struct DefendingStat;
struct RealEffectiveStat;
struct Power;
struct UsesUntilKo;
struct AttackerHpRecovered;
struct AttackerHpLost;
}  // namespace calc_damage
namespace action {
struct Move;
struct Item;
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
void checkMoveSlot(types::entity, const types::registry&);
void checkActionMove(types::entity, const types::registry&);

template <>
void check(const Accuracy&);

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
void check(const calc_damage::AttackingLevel&);

template <>
void check(const calc_damage::AttackingStat&);

template <>
void check(const calc_damage::DefendingStat&);

template <>
void check(const calc_damage::RealEffectiveStat&);

template <>
void check(const calc_damage::Power&);

// template <> void check(const CloneTo&);

template <>
void check(const Damage&);

template <>
void check(const DamageRollModifiers&);

template <>
void check(const DamageRolls&);

template <>
void check(const SlotDecision&);

template <>
void check(const SideDecision&);

template <>
void check(const Evs&);

template <>
void check(const Ivs&);

template <>
void check(const ActionQueue&, const types::registry&);

template <>
void check(const Battle&, const types::registry&);

template <>
void check(const ParentBattle&, const types::registry&);

template <>
void check(const RootBattle&, const types::registry&);

template <>
void check(const ParentEntity&, const types::registry&);

template <>
void check(const ChoiceLock&, const types::registry&);

template <>
void check(const CurrentAction&, const types::registry&);

template <>
void check(const NextAction&, const types::registry&);

template <>
void check(const CurrentActionTargets&, const types::registry&);

template <>
void check(const CurrentActionTarget&, const types::registry&);

template <>
void check(const CurrentActionSource&, const types::registry&);

template <>
void check(const CurrentActionMovesAsTarget&, const types::registry&);

template <>
void check(const CurrentActionMovesAsSource&, const types::registry&);

template <>
void check(const CurrentActionMoveSlot&, const types::registry&);

template <>
void check(const CurrentEffectTarget&, const types::registry&);

template <>
void check(const CurrentEffectSource&, const types::registry&);

template <>
void check(const CurrentEffectsAsTarget&, const types::registry&);

template <>
void check(const CurrentEffectsAsSource&, const types::registry&);

template <>
void check(const FoeSide&, const types::registry&);

template <>
void check(const LastUsedMove&, const types::registry&);

template <>
void check(const MoveSlots&, const types::registry&);

template <>
void check(const Pokemon&, const types::registry&);

template <>
void check(const Side&, const types::registry&);

template <>
void check(const Sides&, const types::registry&);

template <>
void check(const Team&, const types::registry&);

// template <> void check(const EventModifier&);

template <>
void check(const HitCount&);

// template <> void check(const Id&);

template <>
void check(const Level&);

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
void check(const Pp&);

template <>
void check(const MaxPp&);

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
void check(const Position&);

template <>
void check(const MovePriority&);

template <>
void check(const Probability&);

template <>
void check(const RngSeed&);

template <>
void check(const RandomEventChances<2U>&);

template <>
void check(const RandomEventChances<3U>&);

template <>
void check(const RandomEventChances<4U>&);

template <>
void check(const RandomEventChances<5U>&);

template <>
void check(const RandomBinaryChance&);

// template <> void check(const RandomEventCount&);

template <>
void check(const RandomEventChancesStack<2U>&, const types::registry&);

template <>
void check(const RandomEventChancesStack<3U>&, const types::registry&);

template <>
void check(const RandomEventChancesStack<4U>&, const types::registry&);

template <>
void check(const RandomEventChancesStack<5U>&, const types::registry&);

template <>
void check(const RandomBinaryChanceStack&, const types::registry&);

template <>
void check(const RandomEventCountStack&, const types::registry&);

template <>
void check(const RandomEqualChanceStack&, const types::registry&);

// template <> void check(const RandomEventIndex&);

template <>
void check(const action::Move&);

template <>
void check(const action::Item&);

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
void check(const SpeedSort&);

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
void check(const DamageRollKind&);

template <>
void check(const DamageRollOptions&);
}  // namespace pokesim::debug

#endif