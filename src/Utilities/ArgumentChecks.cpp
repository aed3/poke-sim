#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <CalcDamage/Helpers.hpp>
#include <Components/headers.hpp>
#include <Config/Require.hpp>
#include <Types/headers.hpp>
#include <entt/core/type_info.hpp>

#include "ArgumentChecks.hpp"

namespace pokesim::debug {
namespace {
template <typename T>
bool has(types::entity entity, const types::registry& registry) {
  return registry.all_of<T>(entity);
}

template <typename List, typename Value>
bool listContains(const List& list, const Value& value) {
  return std::find(list.begin(), list.end(), value) != list.end();
}

template <typename MaxMinStruct, typename T>
void checkBounds(const T& value) {
  POKESIM_REQUIRE(
    value >= MaxMinStruct::MIN,
    std::to_string(value) + " is not in the bounds of " + std::string(entt::type_name<MaxMinStruct>().value()) + ".");
  POKESIM_REQUIRE(
    value <= MaxMinStruct::MAX,
    std::to_string(value) + " is not in the bounds of " + std::string(entt::type_name<MaxMinStruct>().value()) + ".");
}

void checkBoost(types::boost boost) {
  checkBounds<MechanicConstants::PokemonStatBoost>(boost);
}

void checkStat(types::stat stat, bool hp = false) {
  if (hp) {
    checkBounds<MechanicConstants::PokemonHpStat>(stat);
  }
  else {
    checkBounds<MechanicConstants::PokemonStat>(stat);
  }
}

void checkEffectiveStat(types::stat stat, bool hp = false) {
  if (hp) {
    checkBounds<MechanicConstants::PokemonHpStat>(stat);
  }
  else {
    checkBounds<MechanicConstants::PokemonEffectiveStat>(stat);
  }
}

void checkBaseStat(types::baseStat stat) {
  checkBounds<MechanicConstants::PokemonBaseStat>(stat);
}

void checkSlot(Slot slot) {
  POKESIM_REQUIRE_NM(slot != Slot::NONE);
}

void checkPlayerSideId(PlayerSideId sideId) {
  POKESIM_REQUIRE_NM(sideId == PlayerSideId::P1 || sideId == PlayerSideId::P2);
}

void checkEv(types::ev ev) {
  checkBounds<MechanicConstants::PokemonEv>(ev);
}

void checkIv(types::iv iv) {
  checkBounds<MechanicConstants::PokemonIv>(iv);
}

void checkPercentChance(types::percentChance chance) {
  checkBounds<MechanicConstants::PercentChance>(chance);
}

void checkProbability(types::probability probability) {
  checkBounds<MechanicConstants::Probability>(probability);
}

void checkTeamOrder(const types::teamOrder& teamOrder) {
  for (types::teamPositionIndex position : teamOrder) {
    checkBounds<MechanicConstants::TeamSize>(position);
  }
}

void checkAction(types::entity actionEntity, const types::registry& registry) {
  types::registry::checkEntity(actionEntity, registry);
  if (has<action::Team>(actionEntity, registry)) {
    POKESIM_REQUIRE_NM(!has<action::Item>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<action::Move>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<action::tags::Switch>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<SourceSlotName>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<TargetSlotName>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<SpeedSort>(actionEntity, registry));

    checkTeamOrder(registry.get<action::Team>(actionEntity).val);
  }
  else {
    POKESIM_REQUIRE_NM(has<SpeedSort>(actionEntity, registry));
  }

  if (registry.any_of<action::Item, action::Move, action::tags::Switch>(actionEntity)) {
    POKESIM_REQUIRE_NM(has<SourceSlotName>(actionEntity, registry));
    POKESIM_REQUIRE_NM(has<TargetSlotName>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<action::Team>(actionEntity, registry));

    const auto& [source, target, speedSort] = registry.get<SourceSlotName, TargetSlotName, SpeedSort>(actionEntity);
    check(source);
    check(target);
    check(speedSort);

    if (has<action::Item>(actionEntity, registry)) {
      check((ItemName)registry.get<action::Item>(actionEntity));
    }
    if (has<action::Move>(actionEntity, registry)) {
      check((MoveName)registry.get<action::Move>(actionEntity));
    }
  }
}

template <types::eventPossibilities RANDOM_OPTIONS>
void checkRandomChanceEvents(const RandomEventChances<RANDOM_OPTIONS>& randomEventChances) {
  for (types::percentChance chance : randomEventChances.val) {
    checkPercentChance(chance);
  }
}
}  // namespace

void checkBattle(types::entity battleEntity, const types::registry& registry) {
  types::registry::checkEntity(battleEntity, registry);
  POKESIM_REQUIRE_NM(has<tags::Battle>(battleEntity, registry));
  POKESIM_REQUIRE_NM(has<Sides>(battleEntity, registry));
  POKESIM_REQUIRE_NM(has<Probability>(battleEntity, registry));
  POKESIM_REQUIRE_NM(has<RngSeed>(battleEntity, registry));
  const auto& [sides, probability] = registry.get<Sides, Probability>(battleEntity);

  check(probability);

  POKESIM_REQUIRE_NM(sides.val.size() == MechanicConstants::SIDE_COUNT);
  auto [p1SideEntity, p2SideEntity] = sides.val;

  POKESIM_REQUIRE_NM(registry.get<Battle>(p1SideEntity).val == battleEntity);
  POKESIM_REQUIRE_NM(registry.get<Battle>(p2SideEntity).val == battleEntity);
  POKESIM_REQUIRE_NM(has<tags::Side>(p1SideEntity, registry));
  POKESIM_REQUIRE_NM(has<tags::Side>(p2SideEntity, registry));

  POKESIM_REQUIRE_NM(registry.get<PlayerSide>(p1SideEntity).val == PlayerSideId::P1);
  POKESIM_REQUIRE_NM(registry.get<PlayerSide>(p2SideEntity).val == PlayerSideId::P2);

  POKESIM_REQUIRE_NM(registry.get<FoeSide>(p1SideEntity).val == p2SideEntity);
  POKESIM_REQUIRE_NM(registry.get<FoeSide>(p2SideEntity).val == p1SideEntity);
}

void checkSide(types::entity sideEntity, const types::registry& registry) {
  types::registry::checkEntity(sideEntity, registry);
  POKESIM_REQUIRE_NM(has<tags::Side>(sideEntity, registry));
  POKESIM_REQUIRE_NM(has<Team>(sideEntity, registry));
  POKESIM_REQUIRE_NM(has<FoeSide>(sideEntity, registry));
  POKESIM_REQUIRE_NM(has<Battle>(sideEntity, registry));
  POKESIM_REQUIRE_NM(has<PlayerSide>(sideEntity, registry));

  const auto& [battle, team, foeSide] = registry.get<Battle, Team, FoeSide>(sideEntity);
  checkBounds<MechanicConstants::TeamSize>(team.val.size());

  POKESIM_REQUIRE_NM(registry.get<FoeSide>(foeSide.val).val == sideEntity);

  for (types::entity pokemonEntity : team.val) {
    types::registry::checkEntity(pokemonEntity, registry);
    POKESIM_REQUIRE_NM(has<tags::Pokemon>(pokemonEntity, registry));
    POKESIM_REQUIRE_NM(has<Battle>(pokemonEntity, registry));
    POKESIM_REQUIRE_NM(has<Side>(pokemonEntity, registry));
    POKESIM_REQUIRE_NM(registry.get<Battle>(pokemonEntity).val == battle.val);
    POKESIM_REQUIRE_NM(registry.get<Side>(pokemonEntity).val == sideEntity);
  }
}

void checkPokemon(types::entity pokemonEntity, const types::registry& registry) {
  types::registry::checkEntity(pokemonEntity, registry);
  POKESIM_REQUIRE_NM(has<tags::Pokemon>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<Id>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<Side>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<Battle>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<SpeciesName>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<AbilityName>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<Level>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<MoveSlots>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<Evs>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<Ivs>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<SpeciesTypes>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<stat::Hp>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<stat::Atk>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<stat::Def>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<stat::Spa>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<stat::Spd>(pokemonEntity, registry));
  POKESIM_REQUIRE_NM(has<stat::Spe>(pokemonEntity, registry));

  const auto& [side, battle, speciesName, speciesTypes, abilityName, level, moveSlots, evs, ivs] =
    registry.get<Side, Battle, SpeciesName, SpeciesTypes, AbilityName, Level, MoveSlots, Evs, Ivs>(pokemonEntity);

  const auto& [hp, atk, def, spa, spd, spe] =
    registry.get<stat::Hp, stat::Atk, stat::Def, stat::Spa, stat::Spd, stat::Spe>(pokemonEntity);

  const auto& [currentHp, effectiveAtk, effectiveDef, effectiveSpa, effectiveSpd, effectiveSpe] = registry.try_get<
    stat::CurrentHp,
    stat::EffectiveAtk,
    stat::EffectiveDef,
    stat::EffectiveSpa,
    stat::EffectiveSpd,
    stat::EffectiveSpe>(pokemonEntity);

  POKESIM_REQUIRE_NM(has<tags::Battle>(battle.val, registry));
  POKESIM_REQUIRE_NM(has<tags::Side>(side.val, registry));

  check(speciesName);
  check(speciesTypes);
  check(abilityName);
  check(level);
  check(hp);
  check(atk);
  check(def);
  check(spa);
  check(spd);
  check(spe);
  check(evs);
  check(ivs);

  if (currentHp) check(*currentHp);
  if (effectiveAtk) check(*effectiveAtk);
  if (effectiveDef) check(*effectiveDef);
  if (effectiveSpa) check(*effectiveSpa);
  if (effectiveSpd) check(*effectiveSpd);
  if (effectiveSpe) check(*effectiveSpe);

  checkBounds<MechanicConstants::MoveSlots>(moveSlots.val.size());
}

void checkMoveSlot(types::entity moveSlotEntity, const types::registry& registry) {
  types::registry::checkEntity(moveSlotEntity, registry);
  POKESIM_REQUIRE_NM(has<MoveName>(moveSlotEntity, registry));
  POKESIM_REQUIRE_NM(has<Pp>(moveSlotEntity, registry));
  POKESIM_REQUIRE_NM(has<MaxPp>(moveSlotEntity, registry));

  const auto& [move, pp, maxPp] = registry.get<MoveName, Pp, MaxPp>(moveSlotEntity);

  check(move);
  check(pp);
  check(maxPp);
}

void checkActionMove(types::entity moveEntity, const types::registry& registry) {
  types::registry::checkEntity(moveEntity, registry);
  POKESIM_REQUIRE_NM(has<CurrentActionSource>(moveEntity, registry));
  POKESIM_REQUIRE_NM(has<CurrentActionTargets>(moveEntity, registry));
  POKESIM_REQUIRE_NM(has<Battle>(moveEntity, registry));
  POKESIM_REQUIRE_NM(has<TypeName>(moveEntity, registry));
  POKESIM_REQUIRE_NM(has<MoveName>(moveEntity, registry) != has<tags::SimulateTurn>(moveEntity, registry));

  const auto& [attacker, defender, battle, typeName] =
    registry.get<CurrentActionSource, CurrentActionTargets, Battle, TypeName>(moveEntity);

  POKESIM_REQUIRE_NM(has<tags::CurrentActionMoveSource>(attacker.val, registry));
  POKESIM_REQUIRE_NM(has<tags::CurrentActionMoveTarget>(defender.only(), registry));
  POKESIM_REQUIRE_NM(has<tags::Battle>(battle.val, registry));
  check(typeName);

  if (has<MoveName>(moveEntity, registry)) {
    check(registry.get<MoveName>(moveEntity));
  }

  bool isPhysical = has<move::tags::Physical>(moveEntity, registry);
  bool isSpecial = has<move::tags::Special>(moveEntity, registry);
  bool isStatus = has<move::tags::Status>(moveEntity, registry);
  POKESIM_REQUIRE_NM(isPhysical || isSpecial || isStatus);
  POKESIM_REQUIRE_NM(isPhysical == !(isSpecial || isStatus));
  POKESIM_REQUIRE_NM(isSpecial == !(isPhysical || isStatus));
  POKESIM_REQUIRE_NM(isStatus == !(isPhysical || isSpecial));
}

template <>
void check(const Accuracy& accuracy) {
  checkBounds<MechanicConstants::MoveBaseAccuracy>(accuracy.val);
}

template <>
void check(const AddedTargets& addedTargets) {
  POKESIM_REQUIRE_NM(listContains(VALID_ADDED_TARGET_OPTIONS, addedTargets.val));
}

template <>
void check(const analyze_effect::EffectTarget& effectTarget, const types::registry& registry) {
  checkPokemon(effectTarget.val, registry);
}

template <>
void check(const analyze_effect::EffectMoves& effectMoves) {
  for (dex::Move move : effectMoves.val) {
    check(MoveName{move});
  }
}

template <>
void check(const analyze_effect::Inputs& inputs, const types::registry& registry) {
  for (types::entity input : inputs.val) {
    types::registry::checkEntity(input, registry);
    POKESIM_REQUIRE_NM(has<analyze_effect::tags::Input>(input, registry));
    POKESIM_REQUIRE_NM(has<analyze_effect::Attacker>(input, registry));
    POKESIM_REQUIRE_NM(has<analyze_effect::Defenders>(input, registry));
    POKESIM_REQUIRE_NM(has<analyze_effect::EffectTarget>(input, registry));
    POKESIM_REQUIRE_NM(has<analyze_effect::EffectMoves>(input, registry));
    POKESIM_REQUIRE_NM(has<Battle>(input, registry));
    const auto
      [pseudoWeather,
       sideCondition,
       status,
       terrain,
       volatileCondition,
       weather,
       atkBoost,
       defBoost,
       spaBoost,
       spdBoost,
       speBoost] =
        registry.try_get<
          PseudoWeatherName,
          SideConditionName,
          StatusName,
          TerrainName,
          VolatileName,
          WeatherName,
          AtkBoost,
          DefBoost,
          SpaBoost,
          SpdBoost,
          SpeBoost>(input);
    POKESIM_REQUIRE_NM(
      pseudoWeather || sideCondition || status || terrain || volatileCondition || weather || atkBoost || defBoost ||
      spaBoost || spdBoost || speBoost);

    auto const& [attacker, defenders, target, moves] = registry.get<
      analyze_effect::Attacker,
      analyze_effect::Defenders,
      analyze_effect::EffectTarget,
      analyze_effect::EffectMoves>(input);
    POKESIM_REQUIRE_NM(has<analyze_effect::tags::Attacker>(attacker.val, registry));
    POKESIM_REQUIRE_NM(has<analyze_effect::tags::Defender>(defenders.only(), registry));
    checkPokemon(attacker.val, registry);
    checkPokemon(defenders.only(), registry);
    checkPokemon(target.val, registry);

    for (dex::Move move : moves.val) check(MoveName{move});

    if (pseudoWeather) check(*pseudoWeather);
    if (sideCondition) check(*sideCondition);
    if (status) check(*status);
    if (terrain) check(*terrain);
    if (volatileCondition) check(*volatileCondition);
    if (weather) check(*weather);
    if (atkBoost) check(*atkBoost);
    if (defBoost) check(*defBoost);
    if (spaBoost) check(*spaBoost);
    if (spdBoost) check(*spdBoost);
    if (speBoost) check(*speBoost);
  }
}

template <>
void check(const analyze_effect::MovePairs& movePairs, const types::registry& registry) {
  for (auto [parentBattleMove, childBattleMove] : movePairs.val) {
    checkActionMove(parentBattleMove, registry);
    checkActionMove(childBattleMove, registry);
  }

  for (auto [parentBattleMove, childBattleMove] : movePairs.val) {
    const auto& [parentAttacker, parentDefenders, parentBattle, parentTypeName, parentMoveName] =
      registry.get<analyze_effect::Attacker, analyze_effect::Defenders, Battle, TypeName, MoveName>(parentBattleMove);
    const auto& [childAttacker, childDefenders, childBattle, childTypeName, childMoveName] =
      registry.get<analyze_effect::Attacker, analyze_effect::Defenders, Battle, TypeName, MoveName>(childBattleMove);
    if (has<analyze_effect::tags::BattleCloneForCalculation>(childBattle.val, registry)) {
      POKESIM_REQUIRE_NM(parentAttacker.val != childAttacker.val);
      POKESIM_REQUIRE_NM(parentDefenders.only() != childDefenders.only());
      POKESIM_REQUIRE_NM(parentBattle.val != childBattle.val);
    }
    else {
      POKESIM_REQUIRE_NM(parentAttacker.val == childAttacker.val);
      POKESIM_REQUIRE_NM(parentDefenders.only() == childDefenders.only());
      POKESIM_REQUIRE_NM(parentBattle.val == childBattle.val);
    }

    POKESIM_REQUIRE_NM(parentTypeName.name == childTypeName.name);
    POKESIM_REQUIRE_NM(parentMoveName.name == childMoveName.name);

    POKESIM_REQUIRE_NM(
      has<move::tags::Physical>(parentBattleMove, registry) == has<move::tags::Physical>(childBattleMove, registry));
    POKESIM_REQUIRE_NM(
      has<move::tags::Special>(parentBattleMove, registry) == has<move::tags::Special>(childBattleMove, registry));
    POKESIM_REQUIRE_NM(
      has<move::tags::Status>(parentBattleMove, registry) == has<move::tags::Status>(childBattleMove, registry));
  }
}

template <>
void check(const BaseEffectChance& chance) {
  checkBounds<MechanicConstants::MoveBaseEffectChance>(chance.val);
}

template <>
void check(const BasePower& basePower) {
  checkBounds<MechanicConstants::MoveBasePower>(basePower.val);
}

template <>
void check(const AtkBoost& atkBoost) {
  checkBoost(atkBoost.val);
}

template <>
void check(const DefBoost& defBoost) {
  checkBoost(defBoost.val);
}

template <>
void check(const SpaBoost& spaBoost) {
  checkBoost(spaBoost.val);
}

template <>
void check(const SpdBoost& spdBoost) {
  checkBoost(spdBoost.val);
}

template <>
void check(const SpeBoost& speBoost) {
  checkBoost(speBoost.val);
}

template <>
void check(const calc_damage::CritChanceDivisor& critChanceDivisor) {
  POKESIM_REQUIRE_NM(listContains(MechanicConstants::CRIT_CHANCE_DIVISORS, critChanceDivisor.val));
}

template <>
void check(const calc_damage::CritBoost& critBoost) {
  checkBounds<MechanicConstants::CritBoost>(critBoost.val);
}

template <>
void check(const calc_damage::AttackingLevel& attackingLevel) {
  checkBounds<MechanicConstants::PokemonLevel>(attackingLevel.val);
}

template <>
void check(const calc_damage::AttackingStat& attackingStat) {
  checkEffectiveStat(attackingStat.val);
}

template <>
void check(const calc_damage::DefendingStat& defendingStat) {
  checkEffectiveStat(defendingStat.val);
}

template <>
void check(const Damage& damage) {
  POKESIM_REQUIRE_NM(damage.val <= MechanicConstants::Damage::MAX);
}

template <>
void check(const DamageRollModifiers& modifiers) {
  POKESIM_REQUIRE_NM(listContains(VALID_STAB_BOOST_KINDS, modifiers.stab));
  checkBounds<MechanicConstants::TypeEffectivenessShift>(modifiers.typeEffectiveness);
}

template <>
void check(const DamageRolls& damageRolls) {
  POKESIM_REQUIRE_NM(damageRolls.val.size() <= MechanicConstants::DamageRollCount::MAX);
  for (const Damage& damage : damageRolls.val) {
    check(damage);
  }
}

template <>
void check(const SlotDecision& slotDecision) {
  checkSlot(slotDecision.sourceSlot);
  checkSlot(slotDecision.targetSlot);
  POKESIM_REQUIRE_NM(!(slotDecision.moveChoice.has_value() && slotDecision.itemChoice.has_value()));
  POKESIM_REQUIRE_NM(!(slotDecision.megaEvolve && slotDecision.primalRevert));
}

template <>
void check(const SideDecision& sideDecision) {
  checkPlayerSideId(sideDecision.sideId);
  if (sideDecision.decisions.holds<types::slotDecisions>()) {
    const types::slotDecisions& decisions = sideDecision.decisions.get<types::slotDecisions>();
    for (const SlotDecision& decision : decisions) {
      check(decision);
    }
  }
  else {
    checkTeamOrder(sideDecision.decisions.get<types::teamOrder>());
  }
}

template <>
void check(const Evs& evs) {
  checkEv(evs.hp);
  checkEv(evs.atk);
  checkEv(evs.def);
  checkEv(evs.spa);
  checkEv(evs.spd);
  checkEv(evs.spe);
}

template <>
void check(const Ivs& ivs) {
  checkIv(ivs.hp);
  checkIv(ivs.atk);
  checkIv(ivs.def);
  checkIv(ivs.spa);
  checkIv(ivs.spd);
  checkIv(ivs.spe);
}

template <>
void check(const ActionQueue& actionQueue, const types::registry& registry) {
  checkBounds<MechanicConstants::ActionQueueLength>(actionQueue.val.size());
  for (types::entity entity : actionQueue.val) {
    checkAction(entity, registry);
  }
}

template <>
void check(const Battle& battle, const types::registry& registry) {
  checkBattle(battle.val, registry);
}

template <>
void check(const ParentBattle& battle, const types::registry& registry) {
  checkBattle(battle.val, registry);
}

template <>
void check(const RootBattle& battle, const types::registry& registry) {
  checkBattle(battle.val, registry);
}

template <>
void check(const ParentEntity& parentEntity, const types::registry& registry) {
  types::registry::checkEntity(parentEntity.val, registry);
}

template <>
void check(const ChoiceLock& choiceLock, const types::registry& registry) {
  checkPokemon(choiceLock.val, registry);
}

template <>
void check(const CurrentAction& currentAction, const types::registry& registry) {
  checkAction(currentAction.val, registry);
}

template <>
void check(const NextAction& nextAction, const types::registry& registry) {
  checkAction(nextAction.val, registry);
}

template <>
void check(const CurrentActionTargets& targets, const types::registry& registry) {
  for (types::entity target : targets.val) {
    checkPokemon(target, registry);
  }
}

template <>
void check(const CurrentActionSource& source, const types::registry& registry) {
  checkPokemon(source.val, registry);
}

template <>
void check(const CurrentActionMoves& moves, const types::registry& registry) {
  for (types::entity moveEntity : moves.val) {
    checkActionMove(moveEntity, registry);
  }
}

template <>
void check(const CurrentActionMoveSlot& move, const types::registry& registry) {
  checkMoveSlot(move.val, registry);
}

template <>
void check(const FoeSide& foeSide, const types::registry& registry) {
  checkSide(foeSide.val, registry);
}

template <>
void check(const LastUsedMove& lastUsedMove, const types::registry& registry) {
  checkMoveSlot(lastUsedMove.val, registry);
}

template <>
void check(const MoveSlots& moveSlots, const types::registry& registry) {
  checkBounds<MechanicConstants::MoveSlots>(moveSlots.val.size());
  for (types::entity moveEntity : moveSlots.val) {
    checkMoveSlot(moveEntity, registry);
  }
}

template <>
void check(const Pokemon& pokemon, const types::registry& registry) {
  checkPokemon(pokemon.val, registry);
}

template <>
void check(const Side& side, const types::registry& registry) {
  checkSide(side.val, registry);
}

template <>
void check(const Sides& sides, const types::registry& registry) {
  POKESIM_REQUIRE_NM(sides.val.size() == MechanicConstants::SIDE_COUNT);
  checkSide(sides.p1(), registry);
  checkSide(sides.p2(), registry);
}

template <>
void check(const Team& team, const types::registry& registry) {
  for (types::entity pokemonEntity : team.val) {
    checkPokemon(pokemonEntity, registry);
  }
}

template <>
void check(const HitCount& hitCount) {
  checkBounds<MechanicConstants::MoveHits>(hitCount.val);
}

template <>
void check(const Level& level) {
  checkBounds<MechanicConstants::PokemonLevel>(level.val);
}

template <>
void check(const AbilityName& abilityName) {
  POKESIM_REQUIRE_NM(abilityName.name != dex::Ability::NO_ABILITY);
  POKESIM_REQUIRE_NM((std::underlying_type_t<dex::Ability>)abilityName.name <= dex::TOTAL_ABILITY_COUNT);
}

template <>
void check(const GenderName& genderName) {
  POKESIM_REQUIRE_NM(genderName.name != dex::Gender::NO_GENDER);
  POKESIM_REQUIRE_NM((std::underlying_type_t<dex::Gender>)genderName.name <= dex::TOTAL_GENDER_COUNT);
}

template <>
void check(const ItemName& itemName) {
  POKESIM_REQUIRE_NM(itemName.name != dex::Item::NO_ITEM);
  POKESIM_REQUIRE_NM((std::underlying_type_t<dex::Item>)itemName.name <= dex::TOTAL_ITEM_COUNT);
}

template <>
void check(const MoveName& moveName) {
  POKESIM_REQUIRE_NM(moveName.name != dex::Move::NO_MOVE);
  POKESIM_REQUIRE_NM((std::underlying_type_t<dex::Move>)moveName.name <= dex::TOTAL_MOVE_COUNT);
}

template <>
void check(const NatureName& natureName) {
  POKESIM_REQUIRE_NM(natureName.name != dex::Nature::NO_NATURE);
  POKESIM_REQUIRE_NM((std::underlying_type_t<dex::Nature>)natureName.name <= dex::TOTAL_NATURE_COUNT);
}

template <>
void check(const PseudoWeatherName& pseudoWeatherName) {
  POKESIM_REQUIRE_NM(pseudoWeatherName.name != dex::PseudoWeather::NO_PSEUDO_WEATHER);
  POKESIM_REQUIRE_NM(
    (std::underlying_type_t<dex::PseudoWeather>)pseudoWeatherName.name <= dex::TOTAL_PSEUDO_WEATHER_COUNT);
}

template <>
void check(const SideConditionName& sideConditionName) {
  POKESIM_REQUIRE_NM(sideConditionName.name != dex::SideCondition::NO_SIDE_CONDITION);
  POKESIM_REQUIRE_NM(
    (std::underlying_type_t<dex::SideCondition>)sideConditionName.name <= dex::TOTAL_SIDE_CONDITION_COUNT);
}

template <>
void check(const SourceSlotName& sourceSlotName) {
  checkSlot(sourceSlotName.name);
}

template <>
void check(const SpeciesName& speciesName) {
  POKESIM_REQUIRE_NM(speciesName.name != dex::Species::NO_SPECIES);
  POKESIM_REQUIRE_NM((std::underlying_type_t<dex::Species>)speciesName.name <= dex::TOTAL_SPECIES_COUNT);
}

template <>
void check(const StatName& statName) {
  switch (statName.name) {
    case dex::Stat::HP:
    case dex::Stat::ATK:
    case dex::Stat::DEF:
    case dex::Stat::SPA:
    case dex::Stat::SPD:
    case dex::Stat::SPE: return;
  }

  POKESIM_REQUIRE_FAIL(std::to_string((std::underlying_type_t<dex::Stat>)statName.name) + " is not a valid Stat enum.");
}

template <>
void check(const StatusName& statusName) {
  POKESIM_REQUIRE_NM(statusName.name != dex::Status::NO_STATUS);
  POKESIM_REQUIRE_NM((std::underlying_type_t<dex::Status>)statusName.name <= dex::TOTAL_STATUS_COUNT);
}

template <>
void check(const TargetSlotName& targetSlotName) {
  checkSlot(targetSlotName.name);
}

template <>
void check(const TerrainName& terrainName) {
  POKESIM_REQUIRE_NM(terrainName.name != dex::Terrain::NO_TERRAIN);
  POKESIM_REQUIRE_NM((std::underlying_type_t<dex::Terrain>)terrainName.name <= dex::TOTAL_TERRAIN_COUNT);
}

template <>
void check(const TypeName& typeName) {
  POKESIM_REQUIRE_NM(typeName.name != dex::Type::NO_TYPE);
  POKESIM_REQUIRE_NM((std::underlying_type_t<dex::Type>)typeName.name <= dex::TOTAL_TYPE_COUNT);
}

template <>
void check(const VolatileName& volatileName) {
  POKESIM_REQUIRE_NM(volatileName.name != dex::Volatile::NO_VOLATILE);
  POKESIM_REQUIRE_NM((std::underlying_type_t<dex::Volatile>)volatileName.name <= dex::TOTAL_VOLATILE_COUNT);
}

template <>
void check(const WeatherName& weatherName) {
  POKESIM_REQUIRE_NM(weatherName.name != dex::Weather::NO_WEATHER);
  POKESIM_REQUIRE_NM((std::underlying_type_t<dex::Weather>)weatherName.name <= dex::TOTAL_WEATHER_COUNT);
}

template <>
void check(const Pp& pp) {
  checkBounds<MechanicConstants::MovePp>(pp.val);
}

template <>
void check(const MaxPp& maxPp) {
  checkBounds<MechanicConstants::MoveMaxPp>(maxPp.val);
}

template <>
void check(const PlayerSide& playerSide) {
  checkPlayerSideId(playerSide.val);
}

template <>
void check(const PrimaryAbility& ability) {
  check(AbilityName{ability.name});
}

template <>
void check(const SecondaryAbility& ability) {
  check(AbilityName{ability.name});
}

template <>
void check(const HiddenAbility& ability) {
  check(AbilityName{ability.name});
}

template <>
void check(const BaseStats& baseStats) {
  checkBaseStat(baseStats.hp);
  checkBaseStat(baseStats.atk);
  checkBaseStat(baseStats.def);
  checkBaseStat(baseStats.spa);
  checkBaseStat(baseStats.spd);
  checkBaseStat(baseStats.spe);
}

template <>
void check(const Position& position) {
  checkBounds<MechanicConstants::TeamSize>(position.val);
}

template <>
void check(const MovePriority& movePriority) {
  checkBounds<MechanicConstants::MovePriority>(movePriority.val);
}

template <>
void check(const Probability& probability) {
  checkProbability(probability.val);
}

template <>
void check(const RngSeed& seed) {
  POKESIM_REQUIRE_NM(seed.val != 0U);
}

template <>
void check(const RandomEventChances<2U>& randomEventChances) {
  checkRandomChanceEvents(randomEventChances);
}

template <>
void check(const RandomEventChances<3U>& randomEventChances) {
  checkRandomChanceEvents(randomEventChances);
}

template <>
void check(const RandomEventChances<4U>& randomEventChances) {
  checkRandomChanceEvents(randomEventChances);
}

template <>
void check(const RandomEventChances<5U>& randomEventChances) {
  checkRandomChanceEvents(randomEventChances);
}

template <>
void check(const RandomBinaryChance& randomBinaryChance) {
  checkPercentChance(randomBinaryChance.val);
}

template <>
void check(const RandomEventChancesStack<2U>& randomEventChancesStack, const types::registry& registry) {
  for (const auto& [chances, target] : randomEventChancesStack.val) {
    checkPokemon(target, registry);
    check(RandomEventChances<2U>{chances});
  }
}

template <>
void check(const RandomEventChancesStack<3U>& randomEventChancesStack, const types::registry& registry) {
  for (const auto& [chances, target] : randomEventChancesStack.val) {
    checkPokemon(target, registry);
    check(RandomEventChances<3U>{chances});
  }
}

template <>
void check(const RandomEventChancesStack<4U>& randomEventChancesStack, const types::registry& registry) {
  for (const auto& [chances, target] : randomEventChancesStack.val) {
    checkPokemon(target, registry);
    check(RandomEventChances<4U>{chances});
  }
}

template <>
void check(const RandomEventChancesStack<5U>& randomEventChancesStack, const types::registry& registry) {
  for (const auto& [chances, target] : randomEventChancesStack.val) {
    checkPokemon(target, registry);
    check(RandomEventChances<5U>{chances});
  }
}

template <>
void check(const RandomBinaryChanceStack& randomBinaryChanceStack, const types::registry& registry) {
  for (const auto& [chance, target] : randomBinaryChanceStack.val) {
    checkPokemon(target, registry);
    check(RandomBinaryChance{chance});
  }
}

template <>
void check(const RandomEventCountStack& randomEventCountStack, const types::registry& registry) {
  for (const auto& [chance, target] : randomEventCountStack.val) {
    checkPokemon(target, registry);
  }
}

template <>
void check(const RandomEqualChanceStack& randomEqualChanceStack, const types::registry& registry) {
  for (const auto& target : randomEqualChanceStack.val) {
    checkPokemon(target, registry);
  }
}

template <>
void check(const action::Move& move) {
  check(MoveName{move});
}

template <>
void check(const action::Item& item) {
  check(ItemName{item});
}

template <>
void check(const SpeedTieIndexes& speedTieIndexes) {
  types::activePokemonIndex total = 0U;
  for (const auto& span : speedTieIndexes.val) {
    checkBounds<MechanicConstants::ActivePokemon>(span.start);
    checkBounds<MechanicConstants::ActivePokemon>(span.length);
    checkBounds<MechanicConstants::ActivePokemon>(span.start + span.length);
    total += span.length;
  }
  checkBounds<MechanicConstants::ActivePokemon>(total);
}

template <>
void check(const action::Team& team) {
  checkTeamOrder(team.val);
}

template <>
void check(const simulate_turn::TurnOutcomeBattles& teamOutcomBattles, const types::registry& registry) {
  for (types::entity entity : teamOutcomBattles.val) {
    checkBattle(entity, registry);
  }
}

template <>
void check(const calc_damage::UsesUntilKo& usesUntilKo) {
  checkBounds<MechanicConstants::DamageRollCount>(usesUntilKo.val.size());

  types::moveHits lastUses = 0U;
  types::probability totalProbability = 0.0F;
  for (const auto& useUntilKo : usesUntilKo.val) {
    checkBounds<MechanicConstants::PokemonHpStat>(useUntilKo.uses);  // TODO(aed3): What if a move does no damage?
    checkProbability(useUntilKo.probability);
    POKESIM_REQUIRE(lastUses < useUntilKo.uses, "The list should be in order from least to most hits.");
    totalProbability += useUntilKo.probability;
    lastUses = useUntilKo.uses;
  }

  POKESIM_REQUIRE_NM(usesUntilKo.minHits() == usesUntilKo.val.front());
  POKESIM_REQUIRE_NM(usesUntilKo.maxHits() == usesUntilKo.val.back());

  checkProbability(totalProbability);
}

template <>
void check(const calc_damage::AttackerHpRecovered& attackerHpRecovered) {
  check((DamageRolls)attackerHpRecovered);
}

template <>
void check(const calc_damage::AttackerHpLost& attackerHpLost) {
  check((DamageRolls)attackerHpLost);
}

template <>
void check(const analyze_effect::EffectMultiplier& effectiveMultiplier) {
  checkBounds<MechanicConstants::AnalyzeEffectMultiplier>(effectiveMultiplier.val);
}

template <>
void check(const SpeciesTypes& speciesTypes) {
  check(TypeName{speciesTypes.type1()});
  POKESIM_REQUIRE_NM(speciesTypes.type1() != speciesTypes.type2());
  if (speciesTypes.type2() != dex::Type::NO_TYPE) {
    check(TypeName{speciesTypes.type2()});
  }
}

template <>
void check(const SpeedSort& speedSort) {
  POKESIM_REQUIRE_NM(listContains(VALID_ACTION_ORDERS, speedSort.order));
  checkBounds<MechanicConstants::MovePriority>(speedSort.priority);
  checkStat(speedSort.speed);
}

template <>
void check(const stat::Hp& hp) {
  checkStat(hp.val, true);
}

template <>
void check(const stat::Atk& atk) {
  checkStat(atk.val);
}

template <>
void check(const stat::Def& def) {
  checkStat(def.val);
}

template <>
void check(const stat::Spa& spa) {
  checkStat(spa.val);
}

template <>
void check(const stat::Spd& spd) {
  checkStat(spd.val);
}

template <>
void check(const stat::Spe& spe) {
  checkStat(spe.val);
}

template <>
void check(const stat::CurrentHp& hp) {
  checkEffectiveStat(hp.val, true);
}

template <>
void check(const stat::EffectiveAtk& atk) {
  checkEffectiveStat(atk.val);
}

template <>
void check(const stat::EffectiveDef& def) {
  checkEffectiveStat(def.val);
}

template <>
void check(const stat::EffectiveSpa& spa) {
  checkEffectiveStat(spa.val);
}

template <>
void check(const stat::EffectiveSpd& spd) {
  checkEffectiveStat(spd.val);
}

template <>
void check(const stat::EffectiveSpe& spe) {
  checkEffectiveStat(spe.val);
}

template <>
void check(const Turn& turn) {
  checkBounds<MechanicConstants::TurnCount>(turn.val);
}

template <>
void check(const DamageRollKind& damageRollKind) {
  if (listContains(VALID_DAMAGE_ROLL_KINDS, damageRollKind)) {
    return;
  }

  bool hasAverage = calc_damage::damageKindsMatch(damageRollKind, DamageRollKind::AVERAGE_DAMAGE);
  bool hasMax = calc_damage::damageKindsMatch(damageRollKind, DamageRollKind::MAX_DAMAGE);
  bool hasMin = calc_damage::damageKindsMatch(damageRollKind, DamageRollKind::MIN_DAMAGE);
  bool hasAll = calc_damage::damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS);

  POKESIM_REQUIRE_NM(hasAll != (hasAverage || hasMax || hasMin));
  POKESIM_REQUIRE_NM(hasAverage || hasMax || hasMin || hasAll);
  using DamageRollKindBase = std::underlying_type_t<DamageRollKind>;
  DamageRollKindBase binaryValue = 0U;
  for (DamageRollKind kind : VALID_DAMAGE_ROLL_KINDS) {
    binaryValue |= (DamageRollKindBase)kind;
  }

  POKESIM_REQUIRE_NM((binaryValue | (DamageRollKindBase)damageRollKind) == binaryValue);
}

template <>
void check(const DamageRollOptions& damageRollOptions) {
  check(damageRollOptions.p1);
  check(damageRollOptions.p2);
}
}  // namespace pokesim::debug

#endif