// AUTO GENERATED FILE, DO NOT EDIT

#include "PokeSim.hpp"

/**
 * FILE ORDER
 * src/Utilities/AssertComponentsEqual.cpp
 * src/Utilities/ArgumentChecks.cpp
 * src/Simulation/SimulationSetup.cpp
 * src/Simulation/SimulationResults.cpp
 * src/Simulation/Simulation.cpp
 * src/Simulation/RunEvent.cpp
 * src/Simulation/MoveHitSteps.cpp
 * src/SimulateTurn/SimulateTurn.cpp
 * src/SimulateTurn/RandomChance.cpp
 * src/SimulateTurn/ManageActionQueue.cpp
 * src/SimulateTurn/CalcDamageSpecifics.cpp
 * src/Pokedex/Setup/SpeciesDexDataSetup.cpp
 * src/Pokedex/Setup/MoveDexDataSetup.cpp
 * src/Pokedex/Setup/ItemDexDataSetup.cpp
 * src/Pokedex/Setup/GetSpeciesBuild.cpp
 * src/Pokedex/Setup/GetMoveBuild.cpp
 * src/Pokedex/Setup/GetItemBuild.cpp
 * src/Pokedex/Setup/GetAbilityBuild.cpp
 * src/Pokedex/Setup/AbilityDexDataSetup.cpp
 * src/Pokedex/Pokedex.cpp
 * src/Pokedex/Moves/MoveEvents.cpp
 * src/Pokedex/Items/ItemEvents.cpp
 * src/Pokedex/Effects/EffectEvents.cpp
 * src/Pokedex/Abilities/AbilityEvents.cpp
 * src/CalcDamage/Setup/CalcDamageInputSetup.cpp
 * src/CalcDamage/CalcDamage.cpp
 * src/Battle/Side/ManageSideState.cpp
 * src/Battle/Setup/SideStateSetup.cpp
 * src/Battle/Setup/PokemonStateSetup.cpp
 * src/Battle/Setup/MoveStateSetup.cpp
 * src/Battle/Setup/EmplaceTagFromEnum.cpp
 * src/Battle/Setup/BattleStateSetup.cpp
 * src/Battle/Pokemon/ManagePokemonState.cpp
 * src/Battle/ManageBattleState.cpp
 * src/Battle/Helpers/Helpers.cpp
 * src/Battle/Clone/Clone.cpp
 * src/AnalyzeEffect/Setup/AnalyzeEffectInputSetup.cpp
 * src/AnalyzeEffect/AnalyzeEffect.cpp
 */


/////////////// START OF src/Utilities/AssertComponentsEqual.cpp ///////////////

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <cstddef>
#include <limits>
#include <vector>

namespace pokesim::debug {
types::entity createEntityCopy(types::entity entity, const types::registry& src, types::registry& dst) {
  types::entity dstEntity = dst.create();
  for (auto [id, storage] : src.storage()) {
    if (storage.contains(entity)) {
      entt::resolve(storage.type())
        .invoke(types::registry::MetaFunctions::COPY_TO_OTHER_REGISTRY, {}, &src, entity, &dst, dstEntity);
    }
  }
  return dstEntity;
}

void hasSameComponents(
  const types::registry& currReg, types::entity currEntity, const types::registry& initReg, types::entity initEntity,
  const TypesToIgnore& typesToIgnore) {
  for (auto [id, currStorage] : currReg.storage()) {
    if (typesToIgnore.contains(id)) continue;

    if (currStorage.contains(currEntity)) {
      std::string typeName{currStorage.type().name()};
      const auto* const initStorage = initReg.storage(id);
      POKESIM_REQUIRE(initStorage != nullptr, "The inital registry never contained this component: " + typeName);
      POKESIM_REQUIRE(
        initStorage->contains(initEntity),
        "The inital registry doesn't contain the current's component: " + typeName);
    }
  }

  for (auto [id, initStorage] : initReg.storage()) {
    if (typesToIgnore.contains(id)) continue;

    if (initStorage.contains(initEntity)) {
      std::string typeName{initStorage.type().name()};
      const auto* const currStorage = currReg.storage(id);
      POKESIM_REQUIRE(currStorage != nullptr, "The current registry never contained this component: " + typeName);
      POKESIM_REQUIRE(
        currStorage->contains(currEntity),
        "The current registry doesn't contain the initial's component: " + typeName);
    }
  }
}

void areEntitiesEqual(
  const types::registry& currReg, types::entity currEntity, const types::registry& initReg, types::entity initEntity,
  const TypesToIgnore& typesToIgnore) {
  hasSameComponents(currReg, currEntity, initReg, initEntity, typesToIgnore);
  for (auto [id, storage] : currReg.storage()) {
    if (typesToIgnore.contains(id)) continue;

    if (storage.contains(currEntity)) {
      entt::resolve(storage.type())
        .invoke(
          types::registry::MetaFunctions::ENTITY_COMPONENTS_EQUAL,
          {},
          &currReg,
          currEntity,
          &initReg,
          initEntity);
    }
  }
}

types::entity findCopyParent(
  const entt::dense_map<types::entity, types::entity>& initialEntities, const types::registry& registry,
  types::entity entity) {
  if (initialEntities.contains(entity)) {
    return entity;
  }

  const ParentEntity* parentEntity = registry.try_get<ParentEntity>(entity);
  for (types::entityIndex i = 0U; parentEntity != nullptr; i++) {
    if (
      i >= registry.storage<types::registry::entity_type>()->size() ||
      i == std::numeric_limits<types::entityIndex>::max()) {
      POKESIM_REQUIRE_FAIL("A loop in the battle tree caused an infinite loop.");
      break;
    }

    for (auto [original, _] : initialEntities) {
      if (original == parentEntity->val) {
        return original;
      }
    }
    parentEntity = registry.try_get<ParentEntity>(parentEntity->val);
  }

  POKESIM_REQUIRE_FAIL("Could not find original entity of a clone.");
  return entt::null;
}

bool checkIfCopyParent(types::entity potentialChild, types::entity potentialParent, const types::registry& registry) {
  const ParentEntity* parentEntity = registry.try_get<ParentEntity>(potentialChild);
  for (types::entityIndex i = 0U; parentEntity != nullptr; i++) {
    if (
      i >= registry.storage<types::registry::entity_type>()->size() ||
      i == std::numeric_limits<types::entityIndex>::max()) {
      POKESIM_REQUIRE_FAIL("A loop in the battle tree caused an infinite loop.");
      return false;
    }

    if (parentEntity->val == potentialParent) {
      return true;
    }
    parentEntity = registry.try_get<ParentEntity>(parentEntity->val);
  }

  return false;
}
}  // namespace pokesim::debug

#endif

//////////////// END OF src/Utilities/AssertComponentsEqual.cpp ////////////////

////////////////// START OF src/Utilities/ArgumentChecks.cpp ///////////////////

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

namespace pokesim::debug {
namespace {
template <typename T>
bool has(types::entity entity, const types::registry& registry) {
  return registry.all_of<T>(entity);
}

template <typename... T>
std::size_t totalComponentsPresent(types::entity entity, const types::registry& registry) {
  return ((has<T>(entity, registry) ? 1U : 0U) + ...);
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
    POKESIM_REQUIRE_NM(!has<action::tags::Item>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<action::tags::Move>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<action::tags::Switch>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<SourceSlotName>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<TargetSlotName>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<SpeedSort>(actionEntity, registry));

    checkTeamOrder(registry.get<action::Team>(actionEntity).val);
  }
  else {
    POKESIM_REQUIRE_NM(has<SpeedSort>(actionEntity, registry));
  }

  if (registry.any_of<action::tags::Item, action::tags::Move, action::tags::Switch>(actionEntity)) {
    POKESIM_REQUIRE_NM(has<SourceSlotName>(actionEntity, registry));
    POKESIM_REQUIRE_NM(has<TargetSlotName>(actionEntity, registry));
    POKESIM_REQUIRE_NM(!has<action::Team>(actionEntity, registry));

    const auto& [source, target, speedSort] = registry.get<SourceSlotName, TargetSlotName, SpeedSort>(actionEntity);
    check(source);
    check(target);
    check(speedSort);

    if (has<action::tags::Item>(actionEntity, registry)) {
      check(registry.get<ItemName>(actionEntity));
    }
    if (has<action::tags::Move>(actionEntity, registry)) {
      check(registry.get<MoveName>(actionEntity));
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
  POKESIM_REQUIRE_NM(has<CurrentActionTarget>(moveEntity, registry));
  POKESIM_REQUIRE_NM(has<Battle>(moveEntity, registry));
  POKESIM_REQUIRE_NM(has<TypeName>(moveEntity, registry));
  POKESIM_REQUIRE_NM(has<MoveName>(moveEntity, registry) != has<tags::SimulateTurn>(moveEntity, registry));

  const auto& [attacker, defender, battle, typeName] =
    registry.get<CurrentActionSource, CurrentActionTarget, Battle, TypeName>(moveEntity);

  POKESIM_REQUIRE_NM(has<tags::CurrentActionMoveSource>(attacker.val, registry));
  POKESIM_REQUIRE_NM(has<tags::CurrentActionMoveTarget>(defender.val, registry));
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

  std::size_t totalOUsesOffenseTags =
    totalComponentsPresent<calc_damage::tags::UsesAtk, calc_damage::tags::UsesSpa, calc_damage::tags::UsesDefAsOffense>(
      moveEntity,
      registry);
  std::size_t totalOUsesDefenseTags =
    totalComponentsPresent<calc_damage::tags::UsesDef, calc_damage::tags::UsesSpa>(moveEntity, registry);

  POKESIM_REQUIRE_NM(totalOUsesOffenseTags <= 1U);
  POKESIM_REQUIRE_NM(totalOUsesDefenseTags <= 1U);

  const auto [attackingLevel, attackingStat, defendingStat, realEffectiveStat, critBoost, critChanceDivisor] =
    registry.try_get<
      calc_damage::AttackingLevel,
      calc_damage::AttackingStat,
      calc_damage::DefendingStat,
      calc_damage::RealEffectiveStat,
      calc_damage::CritBoost,
      calc_damage::CritChanceDivisor>(moveEntity);

  if (attackingLevel) check(*attackingLevel);
  if (attackingStat) check(*attackingStat);
  if (defendingStat) check(*defendingStat);
  if (realEffectiveStat) check(*realEffectiveStat);
  if (critBoost) check(*critBoost);
  if (critChanceDivisor) check(*critChanceDivisor);
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
void check(const analyze_effect::EffectMove& effectMove) {
  check(MoveName{effectMove.val});
}

template <>
void check(const analyze_effect::GroupedInputs& groupedInputs, const types::registry& registry) {
  for (types::entity input : groupedInputs.val) {
    types::registry::checkEntity(input, registry);
  }
}

template <>
void check(const analyze_effect::Inputs& inputs, const types::registry& registry) {
  for (types::entity input : inputs.val) {
    types::registry::checkEntity(input, registry);
    POKESIM_REQUIRE_NM(has<analyze_effect::tags::Input>(input, registry));
    POKESIM_REQUIRE_NM(has<analyze_effect::Attacker>(input, registry));
    POKESIM_REQUIRE_NM(has<analyze_effect::Defender>(input, registry));
    POKESIM_REQUIRE_NM(has<analyze_effect::EffectTarget>(input, registry));
    POKESIM_REQUIRE_NM(has<analyze_effect::EffectMove>(input, registry));
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
       speBoost] = analyze_effect::tryGetAllInputEffects(input, registry);
    POKESIM_REQUIRE_NM(
      pseudoWeather || sideCondition || status || terrain || volatileCondition || weather || atkBoost || defBoost ||
      spaBoost || spdBoost || speBoost);

    auto const& [attacker, defender, target, move] = registry.get<
      analyze_effect::Attacker,
      analyze_effect::Defender,
      analyze_effect::EffectTarget,
      analyze_effect::EffectMove>(input);

    checkPokemon(attacker.val, registry);
    checkPokemon(defender.val, registry);
    checkPokemon(target.val, registry);

    check(move);

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
void check(const analyze_effect::MovePair& movePair, const types::registry& registry) {
  auto [parentBattleMove, childBattleMove] = movePair;
  if (parentBattleMove != entt::null) {
    checkActionMove(parentBattleMove, registry);
  }
  if (childBattleMove != entt::null) {
    checkActionMove(childBattleMove, registry);
  }

  if (parentBattleMove == entt::null || childBattleMove == entt::null) {
    return;
  }

  const auto& [parentAttacker, parentDefenders, parentBattle, parentTypeName, parentMoveName] =
    registry.get<analyze_effect::Attacker, analyze_effect::Defender, Battle, TypeName, MoveName>(parentBattleMove);
  const auto& [childAttacker, childDefenders, childBattle, childTypeName, childMoveName] =
    registry.get<analyze_effect::Attacker, analyze_effect::Defender, Battle, TypeName, MoveName>(childBattleMove);
  if (has<analyze_effect::tags::BattleCloneForCalculation>(childBattle.val, registry)) {
    POKESIM_REQUIRE_NM(parentAttacker.val != childAttacker.val);
    POKESIM_REQUIRE_NM(parentDefenders.val != childDefenders.val);
    POKESIM_REQUIRE_NM(parentBattle.val != childBattle.val);
  }
  else {
    POKESIM_REQUIRE_NM(parentAttacker.val == childAttacker.val);
    POKESIM_REQUIRE_NM(parentDefenders.val == childDefenders.val);
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
void check(const calc_damage::RealEffectiveStat& realEffectiveStat) {
  checkEffectiveStat(realEffectiveStat.val);
}

template <>
void check(const calc_damage::Power& power) {
  checkBounds<MechanicConstants::MovePower>(power.val);
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
  checkMoveSlot(choiceLock.val, registry);
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
void check(const CurrentActionTarget& target, const types::registry& registry) {
  checkPokemon(target.val, registry);
}

template <>
void check(const CurrentActionSource& source, const types::registry& registry) {
  checkPokemon(source.val, registry);
}

template <>
void check(const CurrentActionMovesAsTarget& moves, const types::registry& registry) {
  for (types::entity moveEntity : moves.val) {
    checkActionMove(moveEntity, registry);
  }
}

template <>
void check(const CurrentActionMovesAsSource& moves, const types::registry& registry) {
  for (types::entity moveEntity : moves.val) {
    checkActionMove(moveEntity, registry);
  }
}

template <>
void check(const CurrentActionMoveSlot& move, const types::registry& registry) {
  checkMoveSlot(move.val, registry);
}

template <>
void check(const CurrentEffectTarget& target, const types::registry& registry) {
  checkPokemon(target.val, registry);
}

template <>
void check(const CurrentEffectSource& source, const types::registry& registry) {
  checkPokemon(source.val, registry);
}

template <>
void check(const CurrentEffectsAsTarget& effects, const types::registry& registry) {
  for (types::entity effect : effects.val) {
    types::registry::checkEntity(effect, registry);
  }
}

template <>
void check(const CurrentEffectsAsSource& effects, const types::registry& registry) {
  for (types::entity effect : effects.val) {
    types::registry::checkEntity(effect, registry);
  }
}

template <>
void check(const FaintQueue& faintQueue, const types::registry& registry) {
  for (types::entity pokemon : faintQueue.val) {
    checkPokemon(pokemon, registry);
  }
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
void check(const simulate_turn::TurnOutcomeBattles& teamOutcomeBattles, const types::registry& registry) {
  for (types::entity entity : teamOutcomeBattles.val) {
    checkBattle(entity, registry);
  }
}

template <>
void check(const calc_damage::UsesUntilKo& usesUntilKo) {
  checkBounds<MechanicConstants::DamageRollCount>(usesUntilKo.val.size());

  types::moveHits lastHits = 0U;
  types::damageRollIndex totalDamageRollsIncluded = 0U;
  for (const auto& useUntilKo : usesUntilKo.val) {
    checkBounds<MechanicConstants::PokemonHpStat>(useUntilKo.hits);  // TODO(aed3): What if a move does no damage?
    checkBounds<MechanicConstants::DamageRollCount>(useUntilKo.damageRollsIncluded);
    POKESIM_REQUIRE(lastHits < useUntilKo.hits, "The list should be in order from least to most hits.");
    totalDamageRollsIncluded += useUntilKo.damageRollsIncluded;
    lastHits = useUntilKo.hits;
  }

  POKESIM_REQUIRE_NM(usesUntilKo.minUses() == usesUntilKo.val.front());
  POKESIM_REQUIRE_NM(usesUntilKo.maxUses() == usesUntilKo.val.back());

  POKESIM_REQUIRE_NM(totalDamageRollsIncluded == MechanicConstants::DamageRollCount::MAX);
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
  checkBounds<MechanicConstants::PokemonCurrentHpStat>(hp.val);
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
void check(const Winner& winner) {
  // No winner (aka a tie) is valid.
  POKESIM_REQUIRE_NM(
    winner.val == PlayerSideId::P1 || winner.val == PlayerSideId::P2 || winner.val == PlayerSideId::NONE);
}

template <>
void check(const DamageRollKind& damageRollKind) {
  if (listContains(VALID_DAMAGE_ROLL_KINDS, damageRollKind)) {
    return;
  }

  bool hasAverage = damageRollKind & DamageRollKind::AVERAGE_DAMAGE;
  bool hasMax = damageRollKind & DamageRollKind::MAX_DAMAGE;
  bool hasMin = damageRollKind & DamageRollKind::MIN_DAMAGE;
  bool hasAll = damageRollKind & DamageRollKind::ALL_DAMAGE_ROLLS;

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

/////////////////// END OF src/Utilities/ArgumentChecks.cpp ////////////////////

///////////////// START OF src/Simulation/SimulationSetup.cpp //////////////////

#include <utility>

namespace pokesim {
types::entityVector Simulation::createInitialMoves(const std::vector<MoveCreationInfo>& moveInfoList) {
  types::entityVector moveEntities{};
  moveEntities.reserve((types::entityVector::size_type)moveInfoList.size());

  for (const MoveCreationInfo& moveInfo : moveInfoList) {
    MoveStateSetup moveSetup(registry);
    moveSetup.setName(moveInfo.name);
    moveSetup.setPP(moveInfo.pp);
    moveSetup.setMaxPP(moveInfo.maxPp);
    moveEntities.push_back(moveSetup.entity());
  }

  return moveEntities;
}

PokemonStateSetup Simulation::createInitialPokemon(const PokemonCreationInfo& pokemonInfo) {
  PokemonStateSetup pokemonSetup(registry);
  if (pokemonInfo.id.has_value()) {
    pokemonSetup.setID(pokemonInfo.id.value());
  }
  else {
    pokemonSetup.setAutoID();
  }

  pokemonSetup.setSpecies(pokemonInfo.species);
  pokemonSetup.setLevel(pokemonInfo.level);
  if (pokemonInfo.currentTypes.has_value()) {
    pokemonSetup.setTypes(pokemonInfo.currentTypes.value());
  }
  else {
    pokemonSetup.setTypes(pokedex().getSpeciesData<SpeciesTypes>(pokemonInfo.species));
  }
  if (pokemonInfo.gender != dex::Gender::NO_GENDER) pokemonSetup.setGender(pokemonInfo.gender);
  if (pokemonInfo.ability != dex::Ability::NO_ABILITY) pokemonSetup.setAbility(pokemonInfo.ability);
  if (pokemonInfo.item != dex::Item::NO_ITEM) pokemonSetup.setItem(pokemonInfo.item);
  if (pokemonInfo.nature != dex::Nature::NO_NATURE) pokemonSetup.setNature(pokemonInfo.nature);
  if (pokemonInfo.status != dex::Status::NO_STATUS) pokemonSetup.setStatus(pokemonInfo.status);

  pokemonSetup.setEVs(pokemonInfo.evs);
  pokemonSetup.setIVs(pokemonInfo.ivs);
  pokemonSetup.setStat<stat::Hp>(pokemonInfo.stats.hp);
  pokemonSetup.setStat<stat::Atk>(pokemonInfo.stats.atk);
  pokemonSetup.setStat<stat::Def>(pokemonInfo.stats.def);
  pokemonSetup.setStat<stat::Spa>(pokemonInfo.stats.spa);
  pokemonSetup.setStat<stat::Spd>(pokemonInfo.stats.spd);
  pokemonSetup.setStat<stat::Spe>(pokemonInfo.stats.spe);

  pokemonSetup.setCurrentHp(pokemonInfo.currentHp.value_or(pokemonInfo.stats.hp));

  if (pokemonInfo.currentHp.has_value() && pokemonInfo.currentHp == MechanicConstants::PokemonCurrentHpStat::MIN) {
    pokemonSetup.setProperty<tags::Fainted>();
  }

  const auto& currentBoosts = pokemonInfo.currentBoosts;
  if (currentBoosts.atk.has_value()) pokemonSetup.setBoost<AtkBoost>(currentBoosts.atk.value());
  if (currentBoosts.def.has_value()) pokemonSetup.setBoost<DefBoost>(currentBoosts.def.value());
  if (currentBoosts.spa.has_value()) pokemonSetup.setBoost<SpaBoost>(currentBoosts.spa.value());
  if (currentBoosts.spd.has_value()) pokemonSetup.setBoost<SpdBoost>(currentBoosts.spd.value());
  if (currentBoosts.spe.has_value()) pokemonSetup.setBoost<SpeBoost>(currentBoosts.spe.value());

  pokemonSetup.setProperty<tags::AtkStatUpdateRequired>();
  pokemonSetup.setProperty<tags::DefStatUpdateRequired>();
  pokemonSetup.setProperty<tags::SpaStatUpdateRequired>();
  pokemonSetup.setProperty<tags::SpdStatUpdateRequired>();
  pokemonSetup.setProperty<tags::SpeStatUpdateRequired>();

  return pokemonSetup;
}

void Simulation::createInitialSide(
  SideStateSetup sideSetup, const SideCreationInfo& sideInfo, const BattleCreationInfo& battleInfo) {
  internal::maxSizedVector<PokemonStateSetup, MechanicConstants::TeamSize::MAX> pokemonSetupList;
  pokemonSetupList.reserve(sideInfo.team.size());

  for (std::size_t i = 0U; i < sideInfo.team.size(); i++) {
    const PokemonCreationInfo& pokemonInfo = sideInfo.team[i];
    PokemonStateSetup pokemonSetup = createInitialPokemon(pokemonInfo);
    bool battleStarted = battleInfo.turn > MechanicConstants::TurnCount::MIN;
    bool inActiveSlot = (isBattleFormat(BattleFormat::SINGLES_BATTLE_FORMAT) ? 1U : 2U) > i;
    bool isFainted =
      pokemonInfo.currentHp.has_value() && pokemonInfo.currentHp == MechanicConstants::PokemonCurrentHpStat::MIN;
    if (battleStarted && inActiveSlot && !isFainted) {
      pokemonSetup.setProperty<tags::ActivePokemon>();
    }

    types::entityVector moveEntities = createInitialMoves(pokemonInfo.moves);

    if (battleInfo.runWithSimulateTurn) {
      registry.insert<tags::SimulateTurn>(moveEntities.begin(), moveEntities.end());
    }
    if (battleInfo.runWithCalculateDamage) {
      registry.insert<tags::CalculateDamage>(moveEntities.begin(), moveEntities.end());
    }
    if (battleInfo.runWithAnalyzeEffect) {
      registry.insert<tags::AnalyzeEffect>(moveEntities.begin(), moveEntities.end());
    }

    pokemonSetup.setMoves(moveEntities);
    pokemonSetupList.push_back(pokemonSetup);
  }

  if (battleInfo.runWithSimulateTurn) {
    sideSetup.setProperty<tags::SimulateTurn>();
    registry.insert<tags::SimulateTurn>(pokemonSetupList.begin(), pokemonSetupList.end());
  }
  if (battleInfo.runWithCalculateDamage) {
    sideSetup.setProperty<tags::CalculateDamage>();
    registry.insert<tags::CalculateDamage>(pokemonSetupList.begin(), pokemonSetupList.end());
  }
  if (battleInfo.runWithAnalyzeEffect) {
    sideSetup.setProperty<tags::AnalyzeEffect>();
    registry.insert<tags::AnalyzeEffect>(pokemonSetupList.begin(), pokemonSetupList.end());
  }

  sideSetup.setTeam(pokemonSetupList);
}

std::tuple<SideStateSetup, SideStateSetup> Simulation::createInitialBattle(
  BattleStateSetup battleStateSetup, const BattleCreationInfo& battleInfo) {
  battleStateSetup.setAutoID();
  battleStateSetup.setTurn(battleInfo.turn);
  battleStateSetup.setRNGSeed(battleInfo.rngSeed);
  battleStateSetup.setProbability(battleInfo.probability);

  if (battleInfo.runWithSimulateTurn) {
    battleStateSetup.setProperty<tags::SimulateTurn>();
  }
  if (battleInfo.runWithCalculateDamage || !battleInfo.damageCalculations.empty()) {
    battleStateSetup.setProperty<tags::CalculateDamage>();
  }
  if (battleInfo.runWithAnalyzeEffect || !battleInfo.effectsToAnalyze.empty()) {
    battleStateSetup.setProperty<tags::AnalyzeEffect>();
  }

  SideStateSetup p1SideSetup(registry, PlayerSideId::P1);
  SideStateSetup p2SideSetup(registry, PlayerSideId::P2);

  types::entity battleEntity = battleStateSetup.entity();
  types::entity p1Entity = p1SideSetup.entity();
  types::entity p2Entity = p2SideSetup.entity();

  battleStateSetup.setSide(p1Entity);
  battleStateSetup.setSide(p2Entity);

  p1SideSetup.setOpponent(p2Entity);
  p2SideSetup.setOpponent(p1Entity);

  p1SideSetup.setBattle(battleEntity);
  p2SideSetup.setBattle(battleEntity);

  return {p1SideSetup, p2SideSetup};
}

void Simulation::createInitialTurnDecision(
  BattleStateSetup battleStateSetup, const TurnDecisionInfo& turnDecisionInfo) {
  types::handle battleHandle{registry, battleStateSetup.entity()};
  const Sides& sides = battleHandle.get<Sides>();

  registry.emplace<SideDecision>(sides.p1(), turnDecisionInfo.p1);
  registry.emplace<SideDecision>(sides.p2(), turnDecisionInfo.p2);
}

void Simulation::createCalcDamageInput(
  BattleStateSetup battleStateSetup, const CalcDamageInputInfo& inputInfo, debug::SimulationSetupChecks& debugChecks) {
  POKESIM_REQUIRE(inputInfo.attackerSlot != Slot::NONE, "A damage calculation must have a attacker.");
  POKESIM_REQUIRE(inputInfo.defenderSlot != Slot::NONE, "A damage calculation must have a defender.");
  POKESIM_REQUIRE(!inputInfo.moves.empty(), "A damage calculation must have a move.");

  const Sides& sides = registry.get<Sides>(battleStateSetup.entity());
  types::entity attackerEntity = slotToPokemonEntity(registry, sides, inputInfo.attackerSlot);
  types::entity defenderEntity = slotToPokemonEntity(registry, sides, inputInfo.defenderSlot);

  for (dex::Move move : inputInfo.moves) {
    calc_damage::InputSetup inputSetup(registry);
    POKESIM_REQUIRE(move != dex::Move::NO_MOVE, "A damage calculation must have a move.");

    inputSetup.setup(battleStateSetup.entity(), attackerEntity, defenderEntity, move, pokedex());
    debugChecks.addToCalcDamageChecklist(battleStateSetup, inputSetup, inputInfo);
  }
}

void Simulation::createAnalyzeEffectInput(
  BattleStateSetup battleStateSetup, const AnalyzeEffectInputInfo& inputInfo,
  debug::SimulationSetupChecks& debugChecks) {
  POKESIM_REQUIRE(inputInfo.attackerSlot != Slot::NONE, "An effect analysis must have a attacker.");
  POKESIM_REQUIRE(inputInfo.defenderSlot != Slot::NONE, "An effect analysis must have a defender.");
  POKESIM_REQUIRE(inputInfo.effectTarget != Slot::NONE, "An effect analysis must have a effect target.");
  POKESIM_REQUIRE(!inputInfo.moves.empty(), "An effect analysis must include a move.");

  const auto& effect = inputInfo.effect;
  const auto& boostEffect = inputInfo.boostEffect;
  POKESIM_REQUIRE(
    boostEffect.has_value() || (effect.has_value() && !effect.value().empty()),
    "An effect analysis must have an effect.");

  const Sides& sides = registry.get<Sides>(battleStateSetup.entity());
  types::entity attackerEntity = slotToPokemonEntity(registry, sides, inputInfo.attackerSlot);
  types::entity defenderEntity = slotToPokemonEntity(registry, sides, inputInfo.defenderSlot);
  types::entity effectTargetEntity = slotToPokemonEntity(registry, sides, inputInfo.effectTarget);

  for (dex::Move move : inputInfo.moves) {
    analyze_effect::InputSetup inputSetup(registry);

    inputSetup.setAttacker(attackerEntity);
    inputSetup.setDefender(defenderEntity);
    inputSetup.setEffectTarget(effectTargetEntity);
    inputSetup.setEffectMove(move);
    inputSetup.setBattle(battleStateSetup.entity());

    if (effect.has_value()) {
      inputSetup.setEffect(effect.value());
    }
    if (boostEffect.has_value()) {
      inputSetup.setBoostEffect(boostEffect.value().stat, boostEffect.value().boost);
    }
    debugChecks.addToAnalyzeEffectChecklist(battleStateSetup, inputSetup, inputInfo);
  }
}

void Simulation::createInitialStates(const std::vector<BattleCreationInfo>& battleInfoList) {
  debug::SimulationSetupChecks debugChecks(this, battleInfoList);

  for (const BattleCreationInfo& battleInfo : battleInfoList) {
    BattleStateSetup battleStateSetup(registry);
    auto [p1SideSetup, p2SideSetup] = createInitialBattle(battleStateSetup, battleInfo);

    createInitialSide(p1SideSetup, battleInfo.p1, battleInfo);
    createInitialSide(p2SideSetup, battleInfo.p2, battleInfo);

    debugChecks.addToBattleChecklist(battleStateSetup, battleInfo);

    if (!battleInfo.decisionsToSimulate.empty()) {
      POKESIM_REQUIRE(
        battleInfo.decisionsToSimulate.size() < std::numeric_limits<types::entityIndex>::max(),
        "Cannot make more clones than there are entities available.");

      types::entityIndex cloneCount = (types::entityIndex)(battleInfo.decisionsToSimulate.size() - 1U);
      if (cloneCount) {
        battleStateSetup.setProperty<tags::CloneFrom>();
        const types::ClonedEntityMap entityMap = pokesim::clone(registry, cloneCount);

        const auto& clonedBattles = entityMap.at(battleStateSetup.entity());
        internal::maxSizedVector<BattleStateSetup> clones;
        clones.reserve(clonedBattles.size());

        for (types::entity entity : clonedBattles) {
          clones.emplace_back(registry, entity);
        }

        for (types::entityIndex i = 0U; i < cloneCount; i++) {
          BattleStateSetup& setupClone = clones[i];
          const TurnDecisionInfo& turnDecisionInfo = battleInfo.decisionsToSimulate[i];
          debugChecks.addToBattleChecklist(setupClone, battleInfo);

          createInitialTurnDecision(setupClone, turnDecisionInfo);
          setupClone.setID(i);

          debugChecks.addToTurnDecisionChecklist(setupClone, turnDecisionInfo);
        }
      }

      createInitialTurnDecision(battleStateSetup, battleInfo.decisionsToSimulate.back());
      debugChecks.addToTurnDecisionChecklist(battleStateSetup, battleInfo.decisionsToSimulate.back());
      battleStateSetup.setID(cloneCount);
    }

    for (const CalcDamageInputInfo& calcDamageInputInfo : battleInfo.damageCalculations) {
      createCalcDamageInput(battleStateSetup, calcDamageInputInfo, debugChecks);
    }

    for (const AnalyzeEffectInputInfo& analyzeEffectInputInfo : battleInfo.effectsToAnalyze) {
      createAnalyzeEffectInput(battleStateSetup, analyzeEffectInputInfo, debugChecks);
    }
  }

  debugChecks.checkOutputs();
}
}  // namespace pokesim

////////////////// END OF src/Simulation/SimulationSetup.cpp ///////////////////

//////////////// START OF src/Simulation/SimulationResults.cpp /////////////////

namespace pokesim {
namespace simulate_turn {
Results::Results(const Simulation& simulation_) : simulation(&simulation_) {}

types::view<TurnOutcomeBattles> Results::turnOutcomeBattlesResults() const {
  return simulation->registry.view<TurnOutcomeBattles>();
}
}  // namespace simulate_turn

namespace calc_damage {
Results::Results(const Simulation& simulation_) : simulation(&simulation_) {}

types::view<DamageRolls> Results::damageRollResults() const {
  return simulation->registry.view<DamageRolls>();
}

types::view<UsesUntilKo> Results::usesUntilKoResults() const {
  return simulation->registry.view<UsesUntilKo>();
}

types::view<AttackerHpRecovered> Results::hpRecoveredResults() const {
  return simulation->registry.view<AttackerHpRecovered>();
}

types::view<AttackerHpLost> Results::hpLostResults() const {
  return simulation->registry.view<AttackerHpLost>();
}
}  // namespace calc_damage

namespace analyze_effect {
Results::Results(const Simulation& simulation_) : simulation(&simulation_) {}

types::view<EffectMultiplier> Results::effectMultiplierResults() const {
  return simulation->registry.view<EffectMultiplier>();
}

types::view<MultipliedDamageRolls> Results::multipliedDamageRollsResults() const {
  return simulation->registry.view<MultipliedDamageRolls>();
}

types::view<MultipliedUsesUntilKo> Results::multipliedUsesUntilKoResults() const {
  return simulation->registry.view<MultipliedUsesUntilKo>();
}
}  // namespace analyze_effect
}  // namespace pokesim

///////////////// END OF src/Simulation/SimulationResults.cpp //////////////////

//////////////////// START OF src/Simulation/Simulation.cpp ////////////////////

namespace pokesim {
Simulation::Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_)
    : battleFormat(battleFormat_), pokedexPointer(&pokedex_) {
  Pokedex::attachSimulation(pokedexPointer, this);
}

Simulation::~Simulation() {
  Pokedex::detachSimulation(pokedexPointer, this);
}

Simulation::Simulation(Simulation&& other) noexcept : Simulation(*other.pokedexPointer, other.battleFormat) {
  registry = std::move(other.registry);
  analyzeEffectOptions = other.analyzeEffectOptions;
  calculateDamageOptions = other.calculateDamageOptions;
  simulateTurnOptions = other.simulateTurnOptions;
}

const Pokedex& Simulation::pokedex() const {
  POKESIM_REQUIRE(
    Pokedex::isPokedexAttachedToSimulation(pokedexPointer, this),
    "The Pokedex has changed since initialization.");
  return *pokedexPointer;
}

void Simulation::clearAllResults() {
  clearSimulateTurnResults();
  clearCalculateDamageResults();
  clearAnalyzeEffectResults();
}

void Simulation::clearSimulateTurnResults() {
  registry.clear<simulate_turn::TurnOutcomeBattles>();
}

void Simulation::clearCalculateDamageResults() {
  registry.clear<calc_damage::UsesUntilKo, calc_damage::AttackerHpRecovered, calc_damage::AttackerHpLost>();
}

void Simulation::clearAnalyzeEffectResults() {
  registry.clear<
    analyze_effect::EffectMultiplier,
    analyze_effect::MultipliedDamageRolls,
    analyze_effect::MultipliedUsesUntilKo>();
}

simulate_turn::Results Simulation::simulateTurn(std::optional<simulate_turn::Options> options) {
  if (options.has_value()) {
    simulateTurnOptions = options.value();
  }

  simulate_turn::run(*this);

  return {*this};
}

calc_damage::Results Simulation::calculateDamage(std::optional<calc_damage::Options> options) {
  if (options.has_value()) {
    calculateDamageOptions = options.value();
  }

  updateAllStats(*this);
  calc_damage::run(*this);

  return {*this};
}

analyze_effect::Results Simulation::analyzeEffect(std::optional<analyze_effect::Options> options) {
  if (options.has_value()) {
    analyzeEffectOptions = options.value();
  }

  updateAllStats(*this);
  analyze_effect::run(*this);

  return {*this};
}

simulate_turn::Results Simulation::simulateTurn(
  const std::vector<BattleCreationInfo>& battleInfoList, std::optional<simulate_turn::Options> options) {
  createInitialStates(battleInfoList);
  return simulateTurn(options);
}

calc_damage::Results Simulation::calculateDamage(
  const std::vector<BattleCreationInfo>& battleInfoList, std::optional<calc_damage::Options> options) {
  createInitialStates(battleInfoList);
  return calculateDamage(options);
}

analyze_effect::Results Simulation::analyzeEffect(
  const std::vector<BattleCreationInfo>& battleInfoList, std::optional<analyze_effect::Options> options) {
  createInitialStates(battleInfoList);
  return analyzeEffect(options);
}

void Simulation::run() {
  clearAllResults();
  simulateTurn();
  calculateDamage();
  analyzeEffect();
}

types::entityVector Simulation::selectedBattleEntities() const {
  if (hasActiveSelection<tags::SelectedForViewBattle>()) {
    auto view = registry.view<tags::SelectedForViewBattle, tags::Battle>();
    return {view.begin(), view.end()};
  }

  auto view = registry.view<tags::Battle>();
  return {view.begin(), view.end()};
}

types::entityVector Simulation::selectedMoveEntities() const {
  if (hasActiveSelection<tags::SelectedForViewMove>()) {
    auto view = registry.view<tags::SelectedForViewMove, tags::CurrentActionMove>();
    return {view.begin(), view.end()};
  }

  auto view = registry.view<tags::CurrentActionMove>();
  return {view.begin(), view.end()};
}

types::entityVector Simulation::selectedPokemonEntities() const {
  if (hasActiveSelection<tags::SelectedForViewPokemon>()) {
    auto view = registry.view<tags::SelectedForViewPokemon, tags::Pokemon>();
    return {view.begin(), view.end()};
  }

  auto view = registry.view<tags::Pokemon>();
  return {view.begin(), view.end()};
}
}  // namespace pokesim

///////////////////// END OF src/Simulation/Simulation.cpp /////////////////////

///////////////////// START OF src/Simulation/RunEvent.cpp /////////////////////

#include <cstdint>
#include <type_traits>

// TODO(aed3) Auto generate?

namespace pokesim {
namespace {
template <typename ModifiedComponent>
void applyEventModifier(ModifiedComponent& component, EventModifier eventModifier) {
  component.val = applyChainedModifier(component.val, eventModifier.val);
}

template <typename... PokemonSpecifiers>
internal::RegistryContainer::SelectionFunction getMoveEventPokemonSelector() {
  static const size_t SelectAnyPokemon = sizeof...(PokemonSpecifiers) == 0U;
  return internal::RegistryContainer::SelectionFunction{
    [](const void*, const types::registry& registry) -> types::entityVector {
      entt::dense_set<types::entity> entities;
      auto selectedMoveView = registry.view<tags::SelectedForViewMove>();
      auto begin = selectedMoveView.begin();
      auto end = selectedMoveView.end();
      if (selectedMoveView.empty()) {
        auto anyMoveView = registry.view<tags::CurrentActionMove>();
        begin = anyMoveView.begin();
        end = anyMoveView.end();
      }

      std::for_each(begin, end, [&registry, &entities](types::entity entity) {
        if constexpr (
          SelectAnyPokemon || std::disjunction_v<std::is_same<PokemonSpecifiers, tags::CurrentActionMoveSource>...>) {
          entities.insert(registry.get<CurrentActionSource>(entity).val);
        }

        if constexpr (
          SelectAnyPokemon || std::disjunction_v<std::is_same<PokemonSpecifiers, tags::CurrentActionMoveTarget>...>) {
          entities.insert(registry.get<CurrentActionTarget>(entity).val);
        }
      });

      return {entities.begin(), entities.end()};
    }};
}

void applyBasePowerEventModifier(types::handle moveHandle, BasePower basePower, EventModifier eventModifier) {
  calc_damage::Power& power = moveHandle.emplace<calc_damage::Power>(basePower.val);
  power.val = applyChainedModifier(power.val, eventModifier.val);
}
}  // namespace

void runBeforeMove(Simulation& simulation) {
  dex::events::Paralysis::onBeforeMove(simulation);
  dex::events::ChoiceLock::onBeforeMove(simulation);
}

void runResidual(Simulation& simulation) {
  dex::events::Burn::onResidual(simulation);
}

void runAccuracyEvent(Simulation&) {}

void runModifyAccuracyEvent(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewMove, Accuracy>();

  dex::events::BrightPowder::onModifyAccuracy(simulation);

  simulation.viewForSelectedMoves<applyEventModifier<Accuracy>>();
  simulation.registry.clear<EventModifier>();
}

void runModifyCritBoostEvent(Simulation&) {}

void runBasePowerEvent(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewMove, BasePower>();

  dex::events::KnockOff::onBasePower(simulation);

  simulation.viewForSelectedMoves<applyBasePowerEventModifier>();
  simulation.registry.clear<EventModifier>();
}

void runModifyDamageEvent(Simulation& simulation) {
  dex::events::LifeOrb::onModifyDamage(simulation);
}

void runAfterModifyDamageEvent(Simulation& simulation) {
  dex::events::FocusSash::onAfterModifyDamage(simulation);
}

void runDamageEvent(Simulation& simulation) {
  dex::events::FocusSash::onDamage(simulation);

  simulation.registry.clear<calc_damage::tags::RanAfterModifyDamage>();
}

void runDamagingHitEvent(Simulation& simulation) {
  dex::events::Static::onDamagingHit(simulation);
}

void runAfterHitEvent(Simulation& simulation) {
  dex::events::KnockOff::onAfterHit(simulation);
}

void runAfterMoveUsedEvent(Simulation& simulation) {
  dex::events::LifeOrb::onAfterMoveUsed(simulation);
}

void runModifySecondariesEvent(Simulation&) {}
template <typename StatusType>
void runStatusImmunityEvent(Simulation&) {
  // This is where Corrosion (will add CanSetStatus back), the `onSetStatus` events that only block status (i.e. Misty
  // Terrain), and all the `onImmunity` events that relate to non-volatile status conditions will go
}

template void runStatusImmunityEvent<status::tags::Burn>(Simulation&);
template void runStatusImmunityEvent<status::tags::Freeze>(Simulation&);
template void runStatusImmunityEvent<status::tags::Paralysis>(Simulation&);
template void runStatusImmunityEvent<status::tags::Poison>(Simulation&);
template void runStatusImmunityEvent<status::tags::Sleep>(Simulation&);
template void runStatusImmunityEvent<status::tags::Toxic>(Simulation&);

void runAfterSetStatusEvent(Simulation&) {}

void runChangeBoostEvent(Simulation&) {}
void runTryBoostEvent(Simulation&) {}

template <typename BoostType>
void runAfterEachBoostEvent(Simulation&) {}
template void runAfterEachBoostEvent<AtkBoost>(Simulation&);
template void runAfterEachBoostEvent<DefBoost>(Simulation&);
template void runAfterEachBoostEvent<SpaBoost>(Simulation&);
template void runAfterEachBoostEvent<SpdBoost>(Simulation&);
template void runAfterEachBoostEvent<SpeBoost>(Simulation&);

void runAfterBoostEvent(Simulation&) {}

void runModifyMove(Simulation& simulation) {
  // internal::SelectForPokemonView<> selectedPokemon{
  //   simulation,
  //   getMoveEventPokemonSelector<tags::CurrentActionMoveSource>()};

  dex::events::ChoiceScarf::onSourceModifyMove(simulation);
  dex::events::ChoiceSpecs::onSourceModifyMove(simulation);
}

void runDisableMove(Simulation& simulation) {
  dex::events::ChoiceLock::onDisableMove(simulation);
}

void runModifyAtk(Simulation&) {}

void runModifyDef(Simulation&) {}

void runModifySpa(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewPokemon>();

  // Priority 1
  dex::events::ChoiceSpecs::onModifySpa(simulation);

  // Priority 5
  dex::events::Plus::onModifySpA(simulation);

  simulation.viewForSelectedPokemon<applyEventModifier<stat::EffectiveSpa>>();
  simulation.registry.clear<EventModifier>();
}

void runModifySpd(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewPokemon>();

  dex::events::AssaultVest::onModifySpd(simulation);

  simulation.viewForSelectedPokemon<applyEventModifier<stat::EffectiveSpd>>();
  simulation.registry.clear<EventModifier>();
}

void runModifySpe(Simulation& simulation) {
  simulation.addToEntities<EventModifier, tags::SelectedForViewPokemon>();

  dex::events::ChoiceScarf::onModifySpe(simulation);

  simulation.viewForSelectedPokemon<applyEventModifier<stat::EffectiveSpe>>();
  simulation.registry.clear<EventModifier>();

  dex::events::Paralysis::onModifySpe(simulation);
}

void runStartSleep(Simulation&) {}
void runStartFreeze(Simulation&) {
  // Just Shaymin
}

void runTryTakeItemEvent(Simulation&) {}
void runAfterUseItemEvent(Simulation&) {}

void runEndItemEvent(Simulation& simulation) {
  dex::events::AssaultVest::onEnd(simulation);
  dex::events::ChoiceScarf::onEnd(simulation);
  dex::events::ChoiceSpecs::onEnd(simulation);
}

void runEndAbilityEvent(Simulation&) {}
void runFaintEvent(Simulation&) {}
void runAfterFaintEvent(Simulation&) {}
}  // namespace pokesim

////////////////////// END OF src/Simulation/RunEvent.cpp //////////////////////

/////////////////// START OF src/Simulation/MoveHitSteps.cpp ///////////////////

#include <algorithm>

namespace pokesim {
namespace {
void deductMoveHitCount(types::handle moveHandle, HitCount& hitCount) {
  POKESIM_REQUIRE(hitCount.val > 0U, "A hit count shouldn't be decremented if it's already 0.");
  hitCount.val--;
  if (!hitCount.val) {
    moveHandle.remove<HitCount>();
  }
}

void updateCurrentActionTargets(types::registry& registry, CurrentActionTargets& targets) {
  types::activePokemonIndex deleteCount = 0U;
  for (types::entity& target : targets.val) {
    if (!registry.all_of<tags::CurrentActionMoveTarget>(target)) {
      types::activePokemonIndex swapIndex = targets.val.size() - 1 - deleteCount;
      POKESIM_REQUIRE(swapIndex < targets.val.size(), "Swap index out of bounds.");
      std::swap(target, targets.val[swapIndex]);
      deleteCount++;
    }
  }

  targets.val.pop_count(deleteCount);
}

void removeFailedHitTargets(
  types::handle moveHandle, Battle battle, CurrentActionTarget target, CurrentActionSource source) {
  types::registry& registry = *moveHandle.registry();

  CurrentActionMovesAsTarget& targetedMoves = registry.get<CurrentActionMovesAsTarget>(target.val);
  auto newTargetedMoveEnd = std::remove(targetedMoves.val.begin(), targetedMoves.val.end(), moveHandle.entity());
  targetedMoves.val.erase(newTargetedMoveEnd, targetedMoves.val.end());

  if (targetedMoves.val.empty()) {
    registry.remove<tags::CurrentActionMoveTarget>(target.val);
  }

  CurrentActionMovesAsSource& sourcedMoves = registry.get<CurrentActionMovesAsSource>(source.val);
  auto newSourcedMoveEnd = std::remove(sourcedMoves.val.begin(), sourcedMoves.val.end(), moveHandle.entity());
  sourcedMoves.val.erase(newSourcedMoveEnd, sourcedMoves.val.end());

  if (sourcedMoves.val.empty()) {
    registry.remove<tags::CurrentActionMoveSource>(source.val);
  }

  setFailedActionMove(moveHandle, battle);
}

void postMoveHitCheck(Simulation& simulation) {
  simulation.view<removeFailedHitTargets, Tags<tags::CurrentActionMove>, entt::exclude_t<tags::internal::MoveHits>>();
  simulation.view<updateCurrentActionTargets>();
  simulation.registry.clear<Damage>();
}

template <auto Function>
void runMoveHitCheck(Simulation& simulation) {
  simulation.addToEntities<tags::internal::MoveHits, tags::CurrentActionMove>();

  internal::SelectForCurrentActionMoveView<tags::internal::MoveHits> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  Function(simulation);
  selectedMoves.deselect();

  postMoveHitCheck(simulation);
  simulation.registry.clear<tags::internal::MoveHits>();
}

void trySetStatusFromEffect(Simulation&) {}
void trySetVolatileFromEffect(Simulation&) {}
void trySetSideConditionFromEffect(Simulation&) {}
void trySetSlotConditionFromEffect(Simulation&) {}
void trySetWeatherFromEffect(Simulation&) {}
void trySetTerrainFromEffect(Simulation&) {}
void trySetPseudoWeatherFromEffect(Simulation&) {}

void setEffectSource(types::handle handle, CurrentActionSource source) {
  handle.emplace<CurrentEffectSource>(source.val);
  handle.registry()->get_or_emplace<CurrentEffectsAsSource>(source.val).val.push_back(handle.entity());
}

template <typename TargetEntityHolder>
void setEffectTarget(types::handle handle, TargetEntityHolder target) {
  handle.emplace<CurrentEffectTarget>(target.val);
  handle.registry()->get_or_emplace<CurrentEffectsAsTarget>(target.val).val.push_back(handle.entity());
}

void runMoveEffects(Simulation& simulation) {
  internal::SelectForCurrentActionMoveView<tags::internal::RunEffect> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  simulation.viewForSelectedMoves<setEffectSource>();
  simulation.viewForSelectedMoves<setEffectTarget<CurrentActionSource>, Tags<move::effect::tags::MoveSource>>();
  simulation.viewForSelectedMoves<setEffectTarget<CurrentActionTarget>, Tags<move::effect::tags::MoveTarget>>();

  tryBoost(simulation);
  trySetStatus(simulation);
  trySetStatusFromEffect(simulation);
  trySetVolatileFromEffect(simulation);
  trySetSideConditionFromEffect(simulation);
  trySetSlotConditionFromEffect(simulation);
  trySetWeatherFromEffect(simulation);
  trySetTerrainFromEffect(simulation);
  trySetPseudoWeatherFromEffect(simulation);

  simulation.registry.clear<CurrentEffectSource, CurrentEffectTarget, CurrentEffectsAsSource, CurrentEffectsAsTarget>();
}

template <typename TargetEntityHolder>
void removeFaintedSecondaryEffectTarget(
  types::handle handle, TargetEntityHolder target, BaseEffectChance baseEffectChance, Battle battle,
  const simulate_turn::Options& options) {
  types::registry& registry = *handle.registry();
  internal::PercentChanceLimitResult limitReached =
    internal::checkPercentChanceLimits(baseEffectChance.val, registry.get<Probability>(battle.val).val, options);

  if (limitReached == internal::PercentChanceLimitResult::REACHED_PASS_LIMIT) {
    return;
  }
  types::stat hp = registry.get<stat::CurrentHp>(target.val).val;
  if (hp == MechanicConstants::PokemonCurrentHpStat::MIN) {
    handle.remove<move::effect::tags::Secondary>();
  }
}

// Skipping secondary effects entirely for a fainted target is not something Showdown does. This is done here to prevent
// more random chance splits than needed and should not cause outcome deviations from Showdown. If, for example, a move
// exists that has a random chance to add a side or field effect regardless of the target's HP, then this function will
// need to be reworked.
void removeFaintedSecondaryEffectTargets(Simulation& simulation) {
  internal::SelectForCurrentActionMoveView<move::effect::tags::Secondary> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  simulation.viewForSelectedMoves<
    removeFaintedSecondaryEffectTarget<CurrentActionSource>,
    Tags<move::effect::tags::MoveSource>>(simulation.simulateTurnOptions);
  simulation.viewForSelectedMoves<
    removeFaintedSecondaryEffectTarget<CurrentActionTarget>,
    Tags<move::effect::tags::MoveTarget>>(simulation.simulateTurnOptions);
}

// TODO(aed3): When adding damage source, change this to accept the move's handle and CurrentActionSource to pass to
// applyDamage.
void applyDamageToTarget(types::registry& registry, Damage damage, CurrentActionTarget target) {
  pokesim::applyDamage({registry, target.val}, damage.val);
}

void setMoveHitCount(Simulation& simulation) {
  auto noAssignedHitCount =
    simulation.registry.view<tags::SelectedForViewMove>(entt::exclude<move::tags::VariableHitCount, HitCount>);
  simulation.registry.insert<HitCount>(noAssignedHitCount.begin(), noAssignedHitCount.end(), {(types::moveHits)1U});

  runRandomEventChances<4U, tags::SelectedForViewMove, move::tags::VariableHitCount>(
    simulation,
    MechanicConstants::PROGRESSIVE_MULTI_HIT_CHANCES,
    false,
    [](Simulation& sim) {
      sim.addToEntities<HitCount, tags::RandomEventA>(HitCount{2U});
      sim.addToEntities<HitCount, tags::RandomEventB>(HitCount{3U});
      sim.addToEntities<HitCount, tags::RandomEventC>(HitCount{4U});
      sim.addToEntities<HitCount, tags::RandomEventD>(HitCount{5U});
    });
}

void applyDamage(Simulation& simulation) {
  simulation.viewForSelectedMoves<applyDamageToTarget>();

  simulation.removeFromEntities<tags::internal::MoveHits, tags::SelectedForViewMove>(
    entt::exclude<Damage, move::tags::Status>);
  simulation.removeFromEntities<tags::SelectedForViewMove>(entt::exclude<tags::internal::MoveHits>);
}

void runPrimaryMoveEffects(Simulation& simulation) {
  simulation.addToEntities<tags::internal::RunEffect, move::effect::tags::Primary, tags::SelectedForViewMove>();
  runMoveEffects(simulation);
  simulation.registry.clear<tags::internal::RunEffect>();
}

void runSecondaryMoveEffects(Simulation& simulation) {
  removeFaintedSecondaryEffectTargets(simulation);
  runModifySecondariesEvent(simulation);

  runRandomBinaryChance<BaseEffectChance, move::effect::tags::Secondary, tags::SelectedForViewMove>(
    simulation,
    [](Simulation& sim) {
      sim.addToEntities<tags::internal::RunEffect, tags::SelectedForViewMove, tags::RandomEventCheckPassed>();
    });

  runMoveEffects(simulation);
  simulation.registry.clear<tags::internal::RunEffect>();
}

void accuracyCheck(Simulation& simulation) {
  runModifyAccuracyEvent(simulation);
  runAccuracyEvent(simulation);

  runRandomBinaryChance<Accuracy, tags::SelectedForViewMove>(simulation, [](Simulation& sim) {
    sim.removeFromEntities<tags::internal::MoveHits, tags::SelectedForViewMove, tags::RandomEventCheckFailed>();
  });
}

void moveHitLoop(Simulation& simulation) {
  setMoveHitCount(simulation);

  while (!simulation.registry.view<HitCount>().empty()) {
    internal::SelectForCurrentActionMoveView<HitCount> selectedMoves{simulation};
    POKESIM_REQUIRE(
      !selectedMoves.hasNoneSelected(),
      "HitCount should only be present on active moves, meaning this loop should only be entered if there are moves to "
      "select.");
    calc_damage::run(simulation);  // 1. call to this.battle.getDamage
    runDamageEvent(simulation);

    applyDamage(simulation);            // 2. call to this.battle.spreadDamage
    runPrimaryMoveEffects(simulation);  // 3. primary effects
    // 4. self drops
    runSecondaryMoveEffects(simulation);  // 5. secondary effects
    // 6. force switch

    runDamagingHitEvent(simulation);
    runAfterHitEvent(simulation);

    updateAllStats(simulation);
    postMoveHitCheck(simulation);
    simulation.view<deductMoveHitCount>();
  }
}
}  // namespace

void runMoveHitChecks(Simulation& simulation) {
  // invulnerabilityCheck
  // hitCheck
  // immunityCheck
  runMoveHitCheck<accuracyCheck>(simulation);
  // breakProtectCheck
  // stealBoostCheck
  runMoveHitCheck<moveHitLoop>(simulation);
}
}  // namespace pokesim

//////////////////// END OF src/Simulation/MoveHitSteps.cpp ////////////////////

////////////////// START OF src/SimulateTurn/SimulateTurn.cpp //////////////////

namespace pokesim::simulate_turn {
namespace {
void addTargetAllyToTargets(types::registry& registry, const Battle& battle) {
  const Sides& sides = registry.get<Sides>(battle.val);
  const TargetSlotName& targetSlotName = registry.get<TargetSlotName>(registry.get<CurrentAction>(battle.val).val);

  types::entity allyEntity = slotToAllyPokemonEntity(registry, sides, targetSlotName.name);
  if (allyEntity == entt::null) {
    return;
  }

  CurrentActionTargets& targets = registry.get<CurrentActionTargets>(battle.val);
  targets.val.push_back(allyEntity);
}

void addUserAllyToTargets(types::registry& registry, const Battle& battle) {
  const Sides& sides = registry.get<Sides>(battle.val);
  const SourceSlotName& sourceSlotName = registry.get<SourceSlotName>(registry.get<CurrentAction>(battle.val).val);

  types::entity allyEntity = slotToAllyPokemonEntity(registry, sides, sourceSlotName.name);
  if (allyEntity == entt::null) {
    return;
  }

  CurrentActionTargets& targets = registry.get<CurrentActionTargets>(battle.val);
  targets.val.push_back(allyEntity);
}

void resolveMoveTargets(types::registry& registry, CurrentActionTargets& targets) {
  for (types::entity target : targets.val) {
    registry.emplace_or_replace<pokesim::tags::CurrentActionMoveTarget>(target);
  }

  // More to do...
}

void createActionMoveForTargets(
  types::handle targetHandle, Battle battle, CurrentActionSource source, const Pokedex& pokedex) {
  types::registry& registry = *targetHandle.registry();

  dex::Move move = registry.get<MoveName>(registry.get<CurrentAction>(battle.val).val).name;
  types::entity moveEntity = createActionMoveForTarget(targetHandle, battle.val, source.val, move, pokedex);

  registry.emplace<pokesim::tags::SimulateTurn>(moveEntity);
}

void getMoveTargets(Simulation& simulation) {
  if (simulation.isBattleFormat(BattleFormat::DOUBLES_BATTLE_FORMAT)) {
    simulation
      .view<addTargetAllyToTargets, Tags<pokesim::tags::CurrentActionMove, move::added_targets::tags::TargetAlly>>();
    simulation
      .view<addUserAllyToTargets, Tags<pokesim::tags::CurrentActionMove, move::added_targets::tags::UserAlly>>();
  }
  simulation.view<resolveMoveTargets, Tags<pokesim::tags::CurrentActionMove>, entt::exclude_t<AddedTargets>>();
  simulation.view<
    createActionMoveForTargets,
    Tags<pokesim::tags::CurrentActionMoveTarget>,
    entt::exclude_t<CurrentActionMovesAsTarget>>(simulation.pokedex());
}

void useMove(Simulation& simulation) {
  // ModifyTarget
  // ModifyType
  runModifyMove(simulation);

  getMoveTargets(simulation);
  runMoveHitChecks(simulation);
  runAfterMoveUsedEvent(simulation);
}

template <typename ActionTag>
void removeActionBySource(types::handle sourceHandle, Battle battle) {
  types::registry& registry = *sourceHandle.registry();
  registry.remove<ActionTag>(battle.val);
  registry.remove<CurrentActionSource>(battle.val);
  sourceHandle.remove<pokesim::tags::CurrentActionMoveSource>();
}

void runMoveAction(Simulation& simulation) {
  simulation.viewForSelectedBattles<setCurrentActionSource, Tags<action::tags::Move>>();

  simulation.view<
    removeActionBySource<action::tags::Move>,
    Tags<pokesim::tags::CurrentActionMoveSource, pokesim::tags::Fainted>>();
  simulation.view<
    removeActionBySource<action::tags::Move>,
    Tags<pokesim::tags::CurrentActionMoveSource>,
    entt::exclude_t<pokesim::tags::ActivePokemon>>();

  pokesim::internal::SelectForBattleView<action::tags::Move> selectedBattle{simulation};
  if (selectedBattle.hasNoneSelected()) return;

  simulation.viewForSelectedBattles<setCurrentActionTarget>();
  simulation.viewForSelectedBattles<setCurrentActionMove>(simulation.pokedex());

  runBeforeMove(simulation);

  simulation.view<deductPp, Tags<pokesim::tags::CurrentActionMoveSlot>>();
  simulation.view<setLastMoveUsed>();

  useMove(simulation);
}

void runResidualAction(Simulation& simulation) {
  pokesim::internal::SelectForBattleView<action::tags::Residual> selectedBattle{simulation};
  if (selectedBattle.hasNoneSelected()) return;

  simulation.viewForSelectedBattles<speedSort, Tags<tags::SpeedSortNeeded>>();
  simulation.registry.clear<tags::SpeedSortNeeded>();

  runResidual(simulation);
}

void runBeforeTurnAction(Simulation&) {
  // Barely used, will find different way of handling it
}

void setFainting(types::registry& registry, FaintQueue& faintQueue) {
  types::entity pokemon = faintQueue.val.front();
  faintQueue.val.erase(faintQueue.val.begin());
  registry.emplace<pokesim::tags::Fainting>(pokemon);
}

void clearFaintQueue(types::handle battleHandle, const FaintQueue& faintQueue) {
  if (faintQueue.val.empty()) {
    battleHandle.remove<FaintQueue>();
  }
}

void checkWin(types::handle battleHandle, const Sides& sides) {
  types::registry& registry = *battleHandle.registry();

  for (types::entity sideEntity : sides.val) {
    types::teamPositionIndex pokemonLeft = foeSidePokemonLeft(registry, sideEntity);
    if (!pokemonLeft) {
      battleHandle.emplace<Winner>(registry.get<PlayerSide>(sideEntity).val);
      clearActionQueue(battleHandle, battleHandle.get<ActionQueue>());
      return;
    }
  }
}

void faintPokemon(Simulation& simulation) {
  using LoopLimits = MechanicConstants::ActivePokemon;
  types::activePokemonIndex iterations = LoopLimits::MIN;
  while (!simulation.registry.view<FaintQueue>().empty()) {
    POKESIM_REQUIRE(
      iterations < LoopLimits::MAX,
      "More Pokemon were queued to faint in at least one battle than possible.");

    simulation.viewForSelectedBattles<setFainting>();

    pokesim::internal::SelectForPokemonView<pokesim::tags::Fainting> selectedPokemon{simulation};
    POKESIM_REQUIRE(
      !selectedPokemon.hasNoneSelected(),
      "This loop should only be run if setFainting had Pokemon to set as fainting.");

    runFaintEvent(simulation);
    runEndAbilityEvent(simulation);
    runEndItemEvent(simulation);
    simulation.viewForSelectedPokemon<clearVolatiles>();

    simulation.addToEntities<pokesim::tags::Fainted, pokesim::tags::Fainting>();
    simulation.removeFromEntities<pokesim::tags::ActivePokemon, pokesim::tags::Fainting>();
    simulation.removeFromEntities<pokesim::tags::Fainting>();

    simulation.viewForSelectedBattles<clearFaintQueue>();
    iterations++;
  }

  simulation.viewForSelectedBattles<checkWin, Tags<>, entt::exclude_t<Winner>>();

  runAfterFaintEvent(simulation);
}

void runCurrentAction(Simulation& simulation) {
  runBeforeTurnAction(simulation);
  runMoveAction(simulation);
  runResidualAction(simulation);

  clearCurrentAction(simulation);
  faintPokemon(simulation);
  // Update
  // Switch requests

  updateAllStats(simulation);
  simulation.viewForSelectedBattles<speedSort, Tags<tags::SpeedSortNeeded>>();
  simulation.registry.clear<tags::SpeedSortNeeded>();
}

void incrementTurn(Turn& turn) {
  turn.val++;
}

void updateActivePokemonPostTurn(types::registry& registry, const pokesim::MoveSlots& moveSlots) {
  registry.remove<pokesim::move::tags::Disabled>(moveSlots.val.begin(), moveSlots.val.end());
}

void nextTurn(Simulation& simulation) {
  simulation.viewForSelectedBattles<incrementTurn>();

  pokesim::internal::SelectForPokemonView<pokesim::tags::SimulateTurn, pokesim::tags::ActivePokemon> selectedPokemon{
    simulation};
  if (!selectedPokemon.hasNoneSelected()) {
    simulation.viewForSelectedPokemon<updateActivePokemonPostTurn>();
    runDisableMove(simulation);
  }
}

void simulateTurn(Simulation& simulation) {
  const auto& options = simulation.simulateTurnOptions;
#ifndef POKESIM_ALL_DAMAGE_ALL_BRANCHES
  POKESIM_REQUIRE(
    !options.makeBranchesOnRandomEvents || !(options.damageRollsConsidered.p1 & DamageRollKind::ALL_DAMAGE_ROLLS ||
                                             options.damageRollsConsidered.p2 & DamageRollKind::ALL_DAMAGE_ROLLS),
    "Creating a branch for every damage roll is disabled by default to prevent easily reaching the battle count limit. "
    "Rebuild PokeSim with the flag POKESIM_ALL_DAMAGE_ALL_BRANCHES to enable this option combination.");
#endif

  if (!options.applyChangesToInputBattle) {
    simulation.addToEntities<pokesim::tags::CloneFrom, pokesim::tags::Battle, pokesim::tags::SimulateTurn>();
    const auto entityMap = clone(simulation.registry, 1U);
    for (const auto& inputBattleMapping : entityMap) {
      simulation.registry.emplace<tags::Input>(inputBattleMapping.first);
    }
  }

  pokesim::internal::SelectForBattleView<pokesim::tags::SimulateTurn> selectedBattles{
    simulation,
    entt::exclude<tags::Input>};
  pokesim::internal::SelectForSideView<pokesim::tags::SimulateTurn> selectedSides{
    simulation,
    entt::exclude<tags::Input>};
  if (selectedBattles.hasNoneSelected() || selectedSides.hasNoneSelected()) return;

  simulation.viewForSelectedBattles<assignRootBattle>();

  updateAllStats(simulation);
  simulation.viewForSelectedSides<resolveDecision>();
  simulation.removeFromEntities<SideDecision, pokesim::tags::SelectedForViewSide>();

  // simulation.viewForSelectedBattles<addBeforeTurnAction, Tags<>, entt::exclude_t<pokesim::tags::BattleMidTurn>>();
  simulation.viewForSelectedBattles<speedSort>();
  simulation.viewForSelectedBattles<addResidualAction, Tags<>, entt::exclude_t<pokesim::tags::BattleMidTurn>>();

  simulation.addToEntities<pokesim::tags::BattleMidTurn, Turn, pokesim::tags::SelectedForViewBattle>();

  simulation.viewForSelectedBattles<setCurrentAction>();
  using ActionsLimit = MechanicConstants::ActionQueueLength;
  types::actionQueueIndex actionsTaken = ActionsLimit::MIN;
  while (!simulation.registry.view<action::tags::Current>().empty()) {
    POKESIM_REQUIRE(
      actionsTaken < ActionsLimit::MAX,
      "More actions in a turn were queued to be taken than in at least one battle than are possible.");

    runCurrentAction(simulation);
    simulation.viewForSelectedBattles<setCurrentAction, Tags<>, entt::exclude_t<Winner>>();
    actionsTaken++;
  }

  nextTurn(simulation);

  simulation.removeFromEntities<pokesim::tags::BattleMidTurn, Turn, pokesim::tags::SelectedForViewBattle>();
  simulation.viewForSelectedBattles<collectTurnOutcomeBattles>();
  simulation.registry.clear<tags::Input>();
}
}  // namespace

void run(Simulation& simulation) {
  debug::Checks debugChecks(simulation);
  debugChecks.checkInputs();

  simulateTurn(simulation);

  debugChecks.checkOutputs();
}
}  // namespace pokesim::simulate_turn

/////////////////// END OF src/SimulateTurn/SimulateTurn.cpp ///////////////////

////////////////// START OF src/SimulateTurn/RandomChance.cpp //////////////////

#include <optional>

namespace pokesim::internal {
namespace {
template <typename Type>
void updateProbability(Probability& currentProbability, Type eventValue) {
  if constexpr (std::is_same_v<types::probability, Type>) {
    currentProbability.val *= eventValue;
  }
  else {
    currentProbability.val *= (types::probability)eventValue * MechanicConstants::PercentChanceToProbability;
  }
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT, typename RandomEventTag>
void updateProbabilityFromRandomEventChance(
  types::registry& registry, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, const Battle& battle) {
  static constexpr bool isEventA = std::is_same_v<RandomEventTag, tags::RandomEventA>;
  static constexpr bool isEventB = std::is_same_v<RandomEventTag, tags::RandomEventB>;
  static constexpr bool isEventC = std::is_same_v<RandomEventTag, tags::RandomEventC>;
  static constexpr bool isEventD = std::is_same_v<RandomEventTag, tags::RandomEventD>;
  static constexpr bool isEventE = std::is_same_v<RandomEventTag, tags::RandomEventE>;

  static_assert(
    isEventA || isEventB || isEventC || isEventD || isEventE,
    "Using a type that isn't a valid random event type.");

  Probability& probability = registry.get<Probability>(battle.val);

  if constexpr (isEventA) {
    updateProbability(probability, eventChances.chanceA());
  }
  else if constexpr (isEventB) {
    updateProbability(probability, eventChances.chanceB());
  }
  else if constexpr (isEventC) {
    updateProbability(probability, eventChances.chanceC());
  }
  else if constexpr (isEventD) {
    updateProbability(probability, eventChances.chanceD());
  }
  else if constexpr (isEventE) {
    updateProbability(probability, eventChances.chanceE());
  }
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT, typename RandomEventTag>
void viewUpdateProbabilityFromRandomEventChance(Simulation& simulation) {
  simulation.view<updateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, RandomEventTag>, Tags<RandomEventTag>>();
}

template <bool Reciprocal, typename RandomEventTag>
void updateProbabilityFromRandomBinaryChance(
  types::registry& registry, const RandomBinaryChance& eventChance, const Battle& battle) {
  static constexpr bool hasEventPassed = std::is_same_v<RandomEventTag, tags::RandomEventCheckPassed>;
  static constexpr bool hasEventFailed = std::is_same_v<RandomEventTag, tags::RandomEventCheckFailed>;

  static_assert(hasEventPassed || hasEventFailed, "Using a type that isn't a valid random binary chance outcome type.");

  Probability& probability = registry.get<Probability>(battle.val);

  if constexpr (hasEventPassed) {
    if constexpr (Reciprocal) {
      updateProbability(probability, eventChance.reciprocalPass());
    }
    else {
      updateProbability(probability, eventChance.pass());
    }
  }
  else if constexpr (hasEventFailed) {
    if constexpr (Reciprocal) {
      updateProbability(probability, eventChance.reciprocalFail());
    }
    else {
      updateProbability(probability, eventChance.fail());
    }
  }
}

void updateProbabilityFromRandomEqualChance(
  types::registry& registry, const Battle& battle, const RandomEventIndex&,
  types::eventPossibilities possibleEventCount) {
  Probability& probability = registry.get<Probability>(battle.val);

  updateProbability(probability, MechanicConstants::Probability::MAX / (types::probability)possibleEventCount);
}

void updateProbabilityFromRandomEventCount(
  types::registry& registry, const RandomEventCount& eventChance, const Battle& battle) {
  Probability& probability = registry.get<Probability>(battle.val);

  updateProbability(probability, MechanicConstants::Probability::MAX / (types::probability)eventChance.val);
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT>
void assignRandomEvent(
  types::handle handle, const Battle& battle, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances) {
  RngSeed& rngSeed = handle.registry()->get<RngSeed>(battle.val);
  types::percentChance rng =
    (types::percentChance)nextBoundedRandomValue(rngSeed, MechanicConstants::PercentChance::MAX);

  if (rng <= eventChances.val[0]) {
    handle.emplace<tags::RandomEventA>();
    return;
  }
  if (rng <= eventChances.val[1]) {
    handle.emplace<tags::RandomEventB>();
    return;
  }

  if constexpr (POSSIBLE_EVENT_COUNT >= 3U) {
    if (rng <= eventChances.val[2]) {
      handle.emplace<tags::RandomEventC>();
      return;
    }
  }

  if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
    if (rng <= eventChances.val[3]) {
      handle.emplace<tags::RandomEventD>();
      return;
    }
  }

  if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
    if (rng <= eventChances.val[4]) {
      handle.emplace<tags::RandomEventE>();
    }
  }
}

void assignRandomBinaryEvent(types::handle handle, const Battle& battle, const RandomBinaryChance& eventChance) {
  RngSeed& rngSeed = handle.registry()->get<RngSeed>(battle.val);
  types::percentChance rng =
    (types::percentChance)nextBoundedRandomValue(rngSeed, MechanicConstants::PercentChance::MAX);

  if (rng <= eventChance.pass()) {
    handle.emplace<tags::RandomEventCheckPassed>();
  }
  else {
    handle.emplace<tags::RandomEventCheckFailed>();
  }
}

void assignReciprocalRandomBinaryEvent(
  types::handle handle, const Battle& battle, const RandomBinaryChance& eventChance) {
  RngSeed& rngSeed = handle.registry()->get<RngSeed>(battle.val);
  types::percentChance rng = (types::percentChance)nextBoundedRandomValue(rngSeed, eventChance.val);

  if (rng == 0U) {
    handle.emplace<tags::RandomEventCheckPassed>();
  }
  else {
    handle.emplace<tags::RandomEventCheckFailed>();
  }
}

void assignRandomEqualChance(types::handle handle, const Battle& battle, types::eventPossibilities possibleEventCount) {
  RngSeed& rngSeed = handle.registry()->get<RngSeed>(battle.val);
  types::eventPossibilities rng = (types::eventPossibilities)nextBoundedRandomValue(rngSeed, possibleEventCount);

  handle.emplace<RandomEventIndex>(rng);
}

void assignRandomEventCount(types::handle handle, const Battle& battle, const RandomEventCount& eventCount) {
  RngSeed& rngSeed = handle.registry()->get<RngSeed>(battle.val);
  types::eventPossibilities rng = (types::eventPossibilities)nextBoundedRandomValue(rngSeed, eventCount.val);

  handle.emplace<RandomEventIndex>(rng);
}

void assignIndexToClones(
  types::registry& registry, const types::ClonedEntityMap& clonedEntityMap, const types::entityVector& originals) {
  for (types::entity original : originals) {
    registry.emplace<RandomEventIndex>(original, (types::eventPossibilities)0U);

    const auto clonedPointer = clonedEntityMap.find(original);
    if (clonedPointer == clonedEntityMap.end()) continue;
    const auto& cloned = clonedPointer->second;

    for (types::entityIndex index = 0U; index < cloned.size(); index++) {
      POKESIM_REQUIRE(
        std::numeric_limits<types::eventPossibilities>::max() > index,
        "Number of clones shouldn't be greater than the number of possible events.");
      registry.emplace<RandomEventIndex>(cloned[index], (types::eventPossibilities)(index + 1U));
    }
  }
}

template <typename Stack, typename Random>
void placeChanceFromStack(types::handle battleHandle, Stack& stack) {
  if constexpr (!std::is_empty_v<Random>) {
    auto [eventChances, entity] = stack.val.back();
    battleHandle.registry()->emplace<Random>(entity, eventChances);
  }
  else {
    auto entity = stack.val.back();
    battleHandle.registry()->emplace<Random>(entity);
  }

  stack.val.pop_back();
  if (stack.val.empty()) {
    battleHandle.remove<Stack>();
  }
}

template <
  typename Random, typename RandomStack, auto AssignRandomEvents, typename UpdateProbabilities,
  typename... AssignRandomEventsTags, typename... AssignArgs>
void randomChanceEvent(
  Simulation& simulation, types::entityIndex cloneCount, types::callback applyChoices,
  void (*assignClonesToEvents)(types::registry&, const types::ClonedEntityMap&, const types::entityVector&),
  UpdateProbabilities updateProbabilities, const AssignArgs&... assignArgs) {
  if (simulation.isBattleFormat(BattleFormat::DOUBLES_BATTLE_FORMAT)) {
    simulation.view<placeChanceFromStack<RandomStack, Random>>();
  }

  types::registry& registry = simulation.registry;
  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents) {
    auto chanceEntityView = registry.view<Random>();
    if (chanceEntityView.empty()) {
      applyChoices(simulation);
      clearRandomChanceResult(simulation);
      return;
    }

    if constexpr (std::is_same_v<RandomEventCount, Random>) {
      entt::dense_map<types::eventPossibilities, std::pair<types::entityVector, types::entityVector>>
        entitiesByEventCount{};

      auto collectEntityEventCounts =
        [&entitiesByEventCount](types::entity chanceEntity, const Battle& battle, const RandomEventCount& eventCount) {
          entitiesByEventCount[eventCount.val].first.push_back(chanceEntity);
          entitiesByEventCount[eventCount.val].second.push_back(battle.val);
        };

      registry.view<Battle, RandomEventCount>().each(collectEntityEventCounts);

      for (const auto& [eventCount, entities] : entitiesByEventCount) {
        const auto [chanceEntities, battleEntities] = entities;
        if (eventCount == 1U) {
          assignClonesToEvents(registry, {}, chanceEntities);
          continue;
        }

        registry.insert<tags::CloneFrom>(battleEntities.begin(), battleEntities.end());
        auto clonedEntityMap = clone(registry, eventCount - 1U);

        assignClonesToEvents(registry, clonedEntityMap, chanceEntities);
      }
    }
    else {
      auto assignCloneTags = [&registry](const Battle& battle, auto&&...) {
        registry.emplace<tags::CloneFrom>(battle.val);
      };
      registry.view<Battle, Random>().each(assignCloneTags);

      types::entityVector chanceEntities{chanceEntityView.begin(), chanceEntityView.end()};
      auto clonedEntityMap = clone(registry, cloneCount);

      assignClonesToEvents(registry, clonedEntityMap, chanceEntities);
    }

    applyChoices(simulation);
    updateProbabilities(simulation);
  }
  else {
    simulation.view<AssignRandomEvents, Tags<AssignRandomEventsTags...>>(assignArgs...);
    applyChoices(simulation);
    updateProbabilities(simulation);
  }

  registry.clear<Random>();
  clearRandomChanceResult(simulation);
  if (simulation.isBattleFormat(BattleFormat::DOUBLES_BATTLE_FORMAT) && !registry.view<RandomStack>().empty()) {
    randomChanceEvent<Random, RandomStack, AssignRandomEvents, UpdateProbabilities, AssignRandomEventsTags...>(
      simulation,
      cloneCount,
      applyChoices,
      assignClonesToEvents,
      updateProbabilities,
      assignArgs...);
  }
}

template <bool Reciprocal>
void randomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  auto assignClonesToEvents =
    [](types::registry& registry, const types::ClonedEntityMap& clonedEntityMap, const types::entityVector& originals) {
      for (types::entity original : originals) {
        const auto& cloned = clonedEntityMap.at(original);
        registry.emplace<tags::RandomEventCheckPassed>(original);
        registry.emplace<tags::RandomEventCheckFailed>(cloned[0]);
      }
    };

  auto defaultUpdateProbabilities = [](Simulation& sim) {
    sim.view<
      updateProbabilityFromRandomBinaryChance<Reciprocal, tags::RandomEventCheckPassed>,
      Tags<tags::RandomEventCheckPassed>>();
    sim.view<
      updateProbabilityFromRandomBinaryChance<Reciprocal, tags::RandomEventCheckFailed>,
      Tags<tags::RandomEventCheckFailed>>();
  };

  if constexpr (Reciprocal) {
    return randomChanceEvent<
      RandomBinaryChance,
      RandomBinaryChanceStack,
      assignReciprocalRandomBinaryEvent,
      types::callback>(
      simulation,
      1U,
      applyChoices,
      assignClonesToEvents,
      updateProbabilities.value_or(defaultUpdateProbabilities));
  }
  else {
    return randomChanceEvent<RandomBinaryChance, RandomBinaryChanceStack, assignRandomBinaryEvent, types::callback>(
      simulation,
      1U,
      applyChoices,
      assignClonesToEvents,
      updateProbabilities.value_or(defaultUpdateProbabilities));
  }
}
}  // namespace

template <types::eventPossibilities POSSIBLE_EVENT_COUNT>
void randomEventChances(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  static_assert(
    POSSIBLE_EVENT_COUNT >= 2U,
    "RandomEventChances should only be used for events with more than two options.");
  static_assert(
    POSSIBLE_EVENT_COUNT <= MAX_TYPICAL_RANDOM_OPTIONS,
    "Random events with more options than this should use RandomEqualChance or RandomEventCount");

  auto assignClonesToEvents =
    [](types::registry& registry, const types::ClonedEntityMap& clonedEntityMap, const types::entityVector& originals) {
      for (types::entity original : originals) {
        const auto& cloned = clonedEntityMap.at(original);
        registry.emplace<tags::RandomEventA>(original);
        registry.emplace<tags::RandomEventB>(cloned[0]);
        if constexpr (POSSIBLE_EVENT_COUNT >= 3U) {
          registry.emplace<tags::RandomEventC>(cloned[1]);
        }
        if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
          registry.emplace<tags::RandomEventD>(cloned[2]);
        }
        if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
          registry.emplace<tags::RandomEventE>(cloned[3]);
        }
      }
    };

  auto defaultUpdateProbabilities = [](Simulation& sim) {
    viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventA>(sim);
    viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventB>(sim);

    if constexpr (POSSIBLE_EVENT_COUNT >= 3U) {
      viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventC>(sim);
    }
    if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
      viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventD>(sim);
    }
    if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
      viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventE>(sim);
    }
  };

  return randomChanceEvent<
    RandomEventChances<POSSIBLE_EVENT_COUNT>,
    RandomEventChancesStack<POSSIBLE_EVENT_COUNT>,
    assignRandomEvent<POSSIBLE_EVENT_COUNT>,
    types::callback>(
    simulation,
    POSSIBLE_EVENT_COUNT - 1U,
    applyChoices,
    assignClonesToEvents,
    updateProbabilities.value_or(defaultUpdateProbabilities));
}

void randomEqualChance(
  Simulation& simulation, const types::eventPossibilities possibleEventCount, types::callback applyChoices,
  types::optionalCallback updateProbabilities) {
  auto defaultUpdateProbabilities = [possibleEventCount](Simulation& sim) {
    sim.view<updateProbabilityFromRandomEqualChance>(possibleEventCount);
  };

  types::entityIndex cloneCount =
    possibleEventCount > MechanicConstants::DamageRollCount::MAX ? 0U : possibleEventCount - 1U;

  if (updateProbabilities.has_value()) {
    randomChanceEvent<
      tags::RandomEqualChance,
      RandomEqualChanceStack,
      assignRandomEqualChance,
      types::callback,
      tags::RandomEqualChance>(
      simulation,
      cloneCount,
      applyChoices,
      assignIndexToClones,
      updateProbabilities.value(),
      possibleEventCount);
    return;
  }

  randomChanceEvent<
    tags::RandomEqualChance,
    RandomEqualChanceStack,
    assignRandomEqualChance,
    decltype(defaultUpdateProbabilities),
    tags::RandomEqualChance>(
    simulation,
    cloneCount,
    applyChoices,
    assignIndexToClones,
    defaultUpdateProbabilities,
    possibleEventCount);
}

void randomEventCount(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  auto defaultUpdateProbabilities = [](Simulation& sim) { sim.view<updateProbabilityFromRandomEventCount>(); };

  randomChanceEvent<RandomEventCount, RandomEventCountStack, assignRandomEventCount, types::callback>(
    simulation,
    0U,
    applyChoices,
    assignIndexToClones,
    updateProbabilities.value_or(defaultUpdateProbabilities));
}

void randomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  randomBinaryChance<false>(simulation, applyChoices, updateProbabilities);
}

void reciprocalRandomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  randomBinaryChance<true>(simulation, applyChoices, updateProbabilities);
}

void clearRandomChanceResult(Simulation& simulation) {
  simulation.registry.clear<tags::RandomEventA>();
  simulation.registry.clear<tags::RandomEventB>();
  simulation.registry.clear<tags::RandomEventC>();
  simulation.registry.clear<tags::RandomEventD>();
  simulation.registry.clear<tags::RandomEventE>();
  simulation.registry.clear<RandomEventIndex>();
}

template void randomEventChances<2U>(Simulation&, types::callback, types::optionalCallback);
template void randomEventChances<3U>(Simulation&, types::callback, types::optionalCallback);
template void randomEventChances<4U>(Simulation&, types::callback, types::optionalCallback);
template void randomEventChances<5U>(Simulation&, types::callback, types::optionalCallback);
}  // namespace pokesim::internal

/////////////////// END OF src/SimulateTurn/RandomChance.cpp ///////////////////

/////////////// START OF src/SimulateTurn/ManageActionQueue.cpp ////////////////

namespace pokesim::simulate_turn {
namespace {
void resolveSlotDecisions(
  types::handle sideHandle, const types::slotDecisions& decisions, ActionQueue& battleActionQueue) {
  types::registry& registry = *sideHandle.registry();
  for (const SlotDecision& decision : decisions) {
    POKESIM_REQUIRE(decision.sourceSlot != Slot::NONE, "Source slot must be assigned.");
    POKESIM_REQUIRE(decision.targetSlot != Slot::NONE, "Target slot must be assigned.");
    POKESIM_REQUIRE(
      !(decision.moveChoice.has_value() && decision.itemChoice.has_value()),
      "Decisions can't have a move and an item choice.");

    types::handle actionHandle = {registry, registry.create()};
    actionHandle.emplace<SourceSlotName>(decision.sourceSlot);
    actionHandle.emplace<TargetSlotName>(decision.targetSlot);

    SpeedSort speedSort;
    types::entity sourceEntity = slotToPokemonEntity(registry, sideHandle.entity(), decision.sourceSlot);

    stat::EffectiveSpe* effectiveSpe = registry.try_get<stat::EffectiveSpe>(sourceEntity);
    if (effectiveSpe != nullptr) {
      speedSort.speed = effectiveSpe->val;
    }
    else {
      speedSort.speed = registry.get<stat::Spe>(sourceEntity).val;
    }

    if (decision.moveChoice.has_value()) {
      actionHandle.emplace<action::tags::Move>();
      actionHandle.emplace<MoveName>(decision.moveChoice.value());

      speedSort.order = ActionOrder::MOVE;
      speedSort.priority = MechanicConstants::MovePriority::BASE;  // TODO (aed3): Move priority + modify priority
      speedSort.fractionalPriority = false;                        // TODO (aed3): get fractionalPriority
    }
    else if (decision.itemChoice.has_value()) {
      actionHandle.emplace<action::tags::Item>();
      actionHandle.emplace<ItemName>(decision.itemChoice.value());
      speedSort.order = ActionOrder::ITEM;
    }
    else {
      actionHandle.emplace<action::tags::Switch>();
      speedSort.order = ActionOrder::SWITCH;
    }

    actionHandle.emplace<SpeedSort>(speedSort);

    battleActionQueue.val.push_back(actionHandle.entity());
  }
}

void resolveTeamDecision(types::registry& registry, const types::teamOrder& teamOrder, ActionQueue& battleActionQueue) {
  types::handle actionHandle = {registry, registry.create()};

  actionHandle.emplace<action::Team>(teamOrder);

  battleActionQueue.val.push_back(actionHandle.entity());
}
}  // namespace
void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision) {
  POKESIM_REQUIRE(sideDecision.sideId != PlayerSideId::NONE, "Decisions must be assigned to a player.");
  POKESIM_REQUIRE(!sideDecision.decisions.valueless_by_exception(), "Decisions must be non-empty.");

  ActionQueue& battleActionQueue = sideHandle.registry()->get<ActionQueue>(sideHandle.get<Battle>().val);

  if (sideDecision.decisions.holds<types::slotDecisions>()) {
    const auto& decisions = sideDecision.decisions.get<types::slotDecisions>();

#ifdef POKESIM_DEBUG_CHECK_UTILITIES
    for (const SlotDecision& decision : decisions) {
      if (sideDecision.sideId == PlayerSideId::P1) {
        POKESIM_REQUIRE(
          (decision.sourceSlot == Slot::P1A || decision.sourceSlot == Slot::P1B),
          "Source must be from a player 1 in battle slot.");
      }
      else {
        POKESIM_REQUIRE(
          (decision.sourceSlot == Slot::P2A || decision.sourceSlot == Slot::P2B),
          "Source must be from a player 2 in battle slot.");
      }
    }
#endif

    resolveSlotDecisions(sideHandle, decisions, battleActionQueue);
  }
  else if (sideDecision.decisions.holds<types::teamOrder>()) {
    const auto& teamOrder = sideDecision.decisions.get<types::teamOrder>();

    POKESIM_REQUIRE(
      sideHandle.get<Team>().val.size() == teamOrder.size(),
      "Must pick a placement for each Pokemon on the team.");
    resolveTeamDecision(*sideHandle.registry(), teamOrder, battleActionQueue);
  }
  else {
    POKESIM_REQUIRE_FAIL(
      "Decision kind of index " + std::to_string(sideDecision.decisions.index()) + "not implemented yet.");
  }
}

void speedSort(types::handle handle, ActionQueue& actionQueue) {
  auto& entityList = actionQueue.val;

  if (entityList.size() == 1U) return;
  const types::registry* registry = handle.registry();

  internal::maxSizedVector<std::pair<SpeedSort, types::entity>, MechanicConstants::ActionQueueLength::MAX>
    speedSortList;
  speedSortList.reserve(entityList.size());

  for (types::entity entity : entityList) {
    speedSortList.push_back({registry->get<SpeedSort>(entity), entity});
  }

  // TODO (aed3): Test how different sorting algorithms affect speed
  std::sort(speedSortList.begin(), speedSortList.end(), [](const auto& pairA, const auto& pairB) {
    if (pairA.first.order != pairB.first.order) {
      return pairA.first.order < pairB.first.order;
    }

    if (pairA.first.priority != pairB.first.priority) {
      return pairB.first.priority < pairA.first.priority;
    }

    if (pairA.first.fractionalPriority != pairB.first.fractionalPriority) {
      return pairB.first.fractionalPriority;
    }

    if (pairA.first.speed != pairB.first.speed) {
      return pairB.first.speed < pairA.first.speed;
    }

    return false;
  });

  SpeedTieIndexes speedTies;
  types::activePokemonIndex lastEqual = 0U, tieCount = 1U;

  auto speedSortEqual = [](const SpeedSort& speedSortA, const SpeedSort& speedSortB) {
    return speedSortA.order == speedSortB.order && speedSortA.priority == speedSortB.priority &&
           speedSortA.speed == speedSortB.speed && speedSortA.fractionalPriority == speedSortB.fractionalPriority;
  };

  for (types::activePokemonIndex i = 0U; i < speedSortList.size(); i++) {
    entityList[i] = speedSortList[i].second;

    if (i > 0U && speedSortEqual(speedSortList[i].first, speedSortList[i - 1].first)) {
      tieCount++;
    }
    else {
      if (tieCount > 1U) {
        speedTies.val.push_back({lastEqual, tieCount});
      }
      lastEqual = i;
      tieCount = 1U;
    }
  }

  if (tieCount > 1U) {
    speedTies.val.push_back({lastEqual, tieCount});
  }

  if (!speedTies.val.empty()) {
    handle.emplace<SpeedTieIndexes>(speedTies);
  }
}

void addBeforeTurnAction(types::registry& registry, ActionQueue& actionQueue) {
  types::handle actionHandle{registry, registry.create()};
  SpeedSort speedSort{ActionOrder::BEFORE_TURN};

  actionHandle.emplace<action::tags::BeforeTurn>();
  actionHandle.emplace<SpeedSort>(speedSort);
  actionQueue.val.push_back(actionHandle.entity());
}

void addResidualAction(types::registry& registry, ActionQueue& actionQueue) {
  types::handle actionHandle{registry, registry.create()};
  SpeedSort speedSort{ActionOrder::RESIDUAL};

  actionHandle.emplace<action::tags::Residual>();
  actionHandle.emplace<SpeedSort>(speedSort);
  actionQueue.val.push_back(actionHandle.entity());
}

void setCurrentAction(types::handle battleHandle, ActionQueue& actionQueue) {
  types::registry& registry = *battleHandle.registry();

  if (actionQueue.val.empty()) return;

  types::entity newCurrentAction = actionQueue.val.front();
  registry.emplace<action::tags::Current>(newCurrentAction);

  if (registry.all_of<action::tags::Move>(newCurrentAction)) {
    battleHandle.emplace<action::tags::Move>();
    battleHandle.emplace<MoveName>(registry.get<MoveName>(newCurrentAction));
  }
  else if (registry.all_of<action::tags::Residual>(newCurrentAction)) {
    battleHandle.emplace<action::tags::Residual>();
  }
  else {
    POKESIM_REQUIRE_FAIL("Action kind not implemented yet.");
  }

  actionQueue.val.erase(actionQueue.val.begin());

  battleHandle.remove<NextAction>();
  battleHandle.emplace<CurrentAction>(newCurrentAction);
  if (!actionQueue.val.empty()) {
    battleHandle.emplace<NextAction>(actionQueue.val[0]);
  }
}

void clearActionQueue(types::handle battleHandle, ActionQueue& actionQueue) {
  battleHandle.remove<NextAction>();
  battleHandle.registry()->destroy(actionQueue.val.begin(), actionQueue.val.end());
  actionQueue.val.clear();
}
}  // namespace pokesim::simulate_turn

//////////////// END OF src/SimulateTurn/ManageActionQueue.cpp /////////////////

////////////// START OF src/SimulateTurn/CalcDamageSpecifics.cpp ///////////////

namespace pokesim::simulate_turn {
namespace {
void applyDamageRollIndex(Damage& damage, const DamageRolls& damageRolls, const RandomEventIndex& randomRollIndex) {
  types::eventPossibilities damageRollIndex = 0U;
  for (types::damageRollIndex i = 0U; i < damageRolls.val.size(); i++) {
    if (randomRollIndex.val == damageRollIndex) {
      damage = damageRolls.val[damageRollIndex];
      return;
    }

    if (i == 0U || damageRolls.val[i - 1].val != damageRolls.val[i].val) {
      damageRollIndex++;
    }
  }

  POKESIM_REQUIRE_FAIL("How was a damage roll not found that matched the event index?");
}

void assignPartialProbability(
  types::registry& registry, const Battle& battle, const RandomEventCount& randomEventCount) {
  if (randomEventCount.val != 1U) {
    Probability& probability = registry.get<Probability>(battle.val);
    probability.val *= MechanicConstants::Probability::MAX / (types::probability)randomEventCount.val;
  }
}

void assignAllDamageRollProbability(
  types::registry& registry, const Damage& damage, DamageRolls& damageRolls, const Battle& battle) {
  types::eventPossibilities damageCount = 0U;
  for (const Damage damageRoll : damageRolls.val) {
    if (damageRoll.val == damage.val) {
      damageCount++;
    }
  }

  POKESIM_REQUIRE(damageCount > 0U, "How was a damage roll not found that matched the damage dealt?");

  Probability& probability = registry.get<Probability>(battle.val);
  probability.val *= damageCount / (types::probability)MechanicConstants::DamageRollCount::MAX;
}

types::eventPossibilities countUniqueDamageRolls(types::handle moveHandle) {
  const DamageRolls& damageRolls = moveHandle.get<DamageRolls>();
  types::eventPossibilities eventPossibilities = 1U;
  for (types::damageRollIndex i = 1U; i < damageRolls.val.size(); i++) {
    if (damageRolls.val[i - 1].val != damageRolls.val[i].val) {
      eventPossibilities++;
    }
  }
  return eventPossibilities;
}

void updateAllDamageRollProbabilities(Simulation& simulation) {
  simulation.viewForSelectedMoves<assignAllDamageRollProbability>();
}

void updatePartialProbabilities(Simulation& simulation) {
  simulation.viewForSelectedMoves<assignPartialProbability>();
}
}  // namespace

void cloneFromDamageRolls(Simulation& simulation, DamageRollKind damageRollKind) {
  pokesim::internal::SelectForCurrentActionMoveView<pokesim::tags::SimulateTurn, DamageRolls> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) return;

  bool forAllDamageRolls = damageRollKind & DamageRollKind::ALL_DAMAGE_ROLLS;
  bool forRequiredDamageRolls = simulation.simulateTurnOptions.makeBranchesOnRandomEvents || forAllDamageRolls;
  auto applyChoices = [](Simulation& sim) { sim.viewForSelectedMoves<applyDamageRollIndex>(); };
  auto updateProbabilities = forAllDamageRolls ? updateAllDamageRollProbabilities : updatePartialProbabilities;

  runRandomEventCount<countUniqueDamageRolls, pokesim::tags::SelectedForViewMove>(
    simulation,
    forRequiredDamageRolls,
    applyChoices,
    updateProbabilities);

  simulation.removeFromEntities<DamageRolls, pokesim::tags::SelectedForViewMove>();
}

void setIfMoveCrits(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<pokesim::tags::SimulateTurn> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) return;

  runReciprocalRandomBinaryChance<calc_damage::CritChanceDivisor, pokesim::tags::SelectedForViewMove>(
    simulation,
    [](Simulation& sim) { sim.addToEntities<calc_damage::tags::Crit, pokesim::tags::RandomEventCheckPassed>(); });

  simulation.registry.clear<calc_damage::CritChanceDivisor>();
}
}  // namespace pokesim::simulate_turn

/////////////// END OF src/SimulateTurn/CalcDamageSpecifics.cpp ////////////////

////////////// START OF src/Pokedex/Setup/SpeciesDexDataSetup.cpp //////////////

namespace pokesim::dex::internal {
void SpeciesDexDataSetup::setName(Species species) {
  handle.emplace<SpeciesName>(species);
}

void SpeciesDexDataSetup::setType(Type type1, Type type2) {
  handle.emplace<SpeciesTypes>(type1, type2);
}

void SpeciesDexDataSetup::setBaseStats(
  types::baseStat hp, types::baseStat atk, types::baseStat def, types::baseStat spa, types::baseStat spd,
  types::baseStat spe) {
  handle.emplace<BaseStats>(hp, atk, def, spa, spd, spe);
}

void SpeciesDexDataSetup::setPrimaryAbility(Ability ability) {
  handle.emplace<PrimaryAbility>(ability);
}

void SpeciesDexDataSetup::setSecondaryAbility(Ability ability) {
  handle.emplace<SecondaryAbility>(ability);
}

void SpeciesDexDataSetup::setHiddenAbility(Ability ability) {
  handle.emplace<HiddenAbility>(ability);
}
}  // namespace pokesim::dex::internal

/////////////// END OF src/Pokedex/Setup/SpeciesDexDataSetup.cpp ///////////////

/////////////// START OF src/Pokedex/Setup/MoveDexDataSetup.cpp ////////////////

namespace pokesim::dex::internal {
void MoveDexDataSetup::setName(Move move) {
  handle.emplace<MoveName>(move);
}

void MoveDexDataSetup::setNameTag(Move move) {
  move::tags::emplaceTagFromEnum(move, handle);
}

void MoveDexDataSetup::setType(Type type) {
  handle.emplace<TypeName>(type);
}

void MoveDexDataSetup::addAddedTargets(AddedTargetOptions addedTargets) {
  AddedTargets& existingTargets = handle.get_or_emplace<AddedTargets>();
  existingTargets.val = existingTargets.val | addedTargets;

  switch (addedTargets) {
    case AddedTargetOptions::TARGET_ALLY: {
      setProperty<move::added_targets::tags::TargetAlly>();
      break;
    }
    case AddedTargetOptions::USER_ALLY: {
      setProperty<move::added_targets::tags::UserAlly>();
      break;
    }
    case AddedTargetOptions::TARGET_SIDE: {
      setProperty<move::added_targets::tags::TargetSide>();
      break;
    }
    case AddedTargetOptions::USER_SIDE: {
      setProperty<move::added_targets::tags::UserSide>();
      break;
    }
    case AddedTargetOptions::FIELD: {
      setProperty<move::added_targets::tags::Field>();
      break;
    }
    default: break;
  }
}

void MoveDexDataSetup::setAccuracy(types::baseAccuracy accuracy) {
  handle.emplace<Accuracy>(accuracy);
}

void MoveDexDataSetup::setBasePower(types::basePower basePower) {
  handle.emplace<BasePower>(basePower);
}

void MoveDexDataSetup::setCategoryPhysical() {
  POKESIM_REQUIRE(!(handle.any_of<move::tags::Special, move::tags::Status>()), "A move can only have one category.");
  setProperty<move::tags::Physical>();
}

void MoveDexDataSetup::setCategorySpecial() {
  POKESIM_REQUIRE(!(handle.any_of<move::tags::Physical, move::tags::Status>()), "A move can only have one category.");
  setProperty<move::tags::Special>();
}

void MoveDexDataSetup::setCategoryStatus() {
  POKESIM_REQUIRE(!(handle.any_of<move::tags::Physical, move::tags::Special>()), "A move can only have one category.");
  setProperty<move::tags::Status>();
}

void MoveDexDataSetup::setBasePp(types::pp pp) {
  handle.emplace<Pp>(pp);
}

void MoveDexDataSetup::setPriority(types::priority priority) {
  handle.emplace<MovePriority>(priority);
}

void MoveDexDataSetup::setHitCount(types::moveHits hitCount) {
  handle.emplace<HitCount>(hitCount);
}

void MoveDexDataSetup::setEffectTargetsMoveSource() {
  POKESIM_REQUIRE(
    !handle.all_of<move::effect::tags::MoveTarget>(),
    "Moves effects can only affect the target or source, not both.");
  handle.emplace<move::effect::tags::MoveSource>();
}

void MoveDexDataSetup::setEffectTargetsMoveTarget() {
  POKESIM_REQUIRE(
    !handle.all_of<move::effect::tags::MoveTarget>(),
    "Moves effects can only affect the source or target, not both.");
  handle.emplace<move::effect::tags::MoveTarget>();
}
}  // namespace pokesim::dex::internal

//////////////// END OF src/Pokedex/Setup/MoveDexDataSetup.cpp /////////////////

/////////////// START OF src/Pokedex/Setup/ItemDexDataSetup.cpp ////////////////

namespace pokesim::dex::internal {
void ItemDexDataSetup::setName(Item item) {
  handle.emplace<ItemName>(item);
}
}  // namespace pokesim::dex::internal

//////////////// END OF src/Pokedex/Setup/ItemDexDataSetup.cpp /////////////////

//////////////// START OF src/Pokedex/Setup/GetSpeciesBuild.cpp ////////////////

// TODO(aed3): Make this and the individual species files auto generated

namespace pokesim {
namespace {
template <typename T>
struct BuildSpecies {
 private:
  enum class Optional : std::uint8_t {
    primaryAbility,
    secondaryAbility,
    hiddenAbility,
  };

  template <auto Member>
  using void_t = std::void_t<decltype(Member)>;

  template <Optional, typename = T, typename V = void>
  struct has : std::false_type {};
  template <typename Type>
  struct has<Optional::primaryAbility, Type, void_t<Type::primaryAbility>> : std::true_type {};
  template <typename Type>
  struct has<Optional::secondaryAbility, Type, void_t<Type::secondaryAbility>> : std::true_type {};
  template <typename Type>
  struct has<Optional::hiddenAbility, Type, void_t<Type::hiddenAbility>> : std::true_type {};

 public:
  static types::entity build(types::registry& registry) {
    dex::internal::SpeciesDexDataSetup species(registry);

    species.setName(T::name);
    species.setBaseStats(T::hp, T::atk, T::def, T::spa, T::spd, T::spe);
    species.setType(T::type.type1(), T::type.type2());

    if constexpr (has<Optional::primaryAbility>::value) {
      species.setPrimaryAbility(T::primaryAbility);
    }
    if constexpr (has<Optional::secondaryAbility>::value) {
      species.setSecondaryAbility(T::secondaryAbility);
    }
    if constexpr (has<Optional::hiddenAbility>::value) {
      species.setHiddenAbility(T::hiddenAbility);
    }

    return species.entity();
  }
};

template <template <GameMechanics> class T>
auto buildSpeciesSV(types::registry& registry) {
  return BuildSpecies<T<GameMechanics::SCARLET_VIOLET>>::build(registry);
}
}  // namespace

types::entity Pokedex::buildSpecies(dex::Species species, types::registry& registry) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  if (isMechanics(GameMechanics::SCARLET_VIOLET)) {
    switch (species) {
      case Species::AMPHAROS:  return buildSpeciesSV<Ampharos>(registry);
      case Species::GARDEVOIR: return buildSpeciesSV<Gardevoir>(registry);
      case Species::EMPOLEON:  return buildSpeciesSV<Empoleon>(registry);
      case Species::PANGORO:   return buildSpeciesSV<Pangoro>(registry);
      case Species::RIBOMBEE:  return buildSpeciesSV<Ribombee>(registry);
      case Species::DRAGAPULT: return buildSpeciesSV<Dragapult>(registry);

      default: break;
    }
  }

  POKESIM_REQUIRE_FAIL("Building a species that does not exist.");
  return types::entity{};
}
}  // namespace pokesim

///////////////// END OF src/Pokedex/Setup/GetSpeciesBuild.cpp /////////////////

///////////////// START OF src/Pokedex/Setup/GetMoveBuild.cpp //////////////////

// TODO(aed3): Make this and the individual move files auto generated

namespace pokesim {
namespace {
template <typename T>
struct BuildMove {
 private:
  enum class Optional : std::uint8_t {
    accuracy,
    basePower,
    hitCount,
    targetPrimaryEffect,
    targetSecondaryEffect,
    sourcePrimaryEffect,
    sourceSecondaryEffect,
    chance,
    atkBoost,
    defBoost,
    spaBoost,
    spdBoost,
    speBoost,

    // moveTags and effectTags are not optional because setting them as optional does not work with clang
  };

  enum class MoveEffectKind : std::uint8_t {
    primary,
    secondary,
  };

  template <auto Member>
  using void_t = std::void_t<decltype(Member)>;

  template <Optional, typename, typename V = void>
  struct has : std::false_type {};
  template <typename Type>
  struct has<Optional::accuracy, Type, void_t<Type::accuracy>> : std::true_type {};
  template <typename Type>
  struct has<Optional::basePower, Type, void_t<Type::basePower>> : std::true_type {};
  template <typename Type>
  struct has<Optional::hitCount, Type, void_t<Type::hitCount>> : std::true_type {};
  template <typename Type>
  struct has<Optional::targetPrimaryEffect, Type, std::void_t<typename Type::targetPrimaryEffect>> : std::true_type {};
  template <typename Type>
  struct has<Optional::targetSecondaryEffect, Type, std::void_t<typename Type::targetSecondaryEffect>>
      : std::true_type {};
  template <typename Type>
  struct has<Optional::sourcePrimaryEffect, Type, std::void_t<typename Type::sourcePrimaryEffect>> : std::true_type {};
  template <typename Type>
  struct has<Optional::sourceSecondaryEffect, Type, std::void_t<typename Type::sourceSecondaryEffect>>
      : std::true_type {};
  template <typename Type>
  struct has<Optional::chance, Type, void_t<Type::chance>> : std::true_type {};
  template <typename Type>
  struct has<Optional::atkBoost, Type, void_t<Type::atkBoost>> : std::true_type {};
  template <typename Type>
  struct has<Optional::defBoost, Type, void_t<Type::defBoost>> : std::true_type {};
  template <typename Type>
  struct has<Optional::spaBoost, Type, void_t<Type::spaBoost>> : std::true_type {};
  template <typename Type>
  struct has<Optional::spdBoost, Type, void_t<Type::spdBoost>> : std::true_type {};
  template <typename Type>
  struct has<Optional::speBoost, Type, void_t<Type::speBoost>> : std::true_type {};

  template <typename EffectType, typename EffectData, MoveEffectKind moveEffectKind, typename... EffectValues>
  static void setEffect(dex::internal::MoveDexDataSetup& move, const EffectValues&... effectValues) {
    if constexpr (moveEffectKind == MoveEffectKind::primary) {
      static_assert(
        !has<Optional::chance, EffectData>::value,
        "Primary effects cannot have a chance to happen because they always happen if the move is successful.");
      move.setPrimaryEffect<EffectType>(effectValues...);
    }
    else {
      types::percentChance chance = MechanicConstants::MoveBaseEffectChance::MAX;
      if constexpr (has<Optional::chance, EffectData>::value) {
        chance = EffectData::chance;
      }
      move.setSecondaryEffect<EffectType>(chance, effectValues...);
    }
  }

  template <typename EffectData, MoveEffectKind moveEffectKind, typename... EffectTypes>
  static void setEffectTags(dex::internal::MoveDexDataSetup& move, Tags<EffectTypes...>) {
    if constexpr (moveEffectKind == MoveEffectKind::primary) {
      static_assert(
        !has<Optional::chance, EffectData>::value,
        "Primary effects cannot have a chance to happen because they always happen if the move is successful.");
      (move.setPrimaryEffect<EffectTypes>(), ...);
    }
    else {
      types::percentChance chance = MechanicConstants::MoveBaseEffectChance::MAX;
      if constexpr (has<Optional::chance, EffectData>::value) {
        chance = EffectData::chance;
      }
      (move.setSecondaryEffect<EffectTypes>(chance), ...);
    }
  }

  template <typename EffectData, MoveEffectKind moveEffectKind>
  static void buildEffect(dex::internal::MoveDexDataSetup& move) {
    if constexpr (has<Optional::atkBoost, EffectData>::value) {
      setEffect<AtkBoost, EffectData, moveEffectKind>(move, EffectData::atkBoost);
    }

    if constexpr (has<Optional::defBoost, EffectData>::value) {
      setEffect<DefBoost, EffectData, moveEffectKind>(move, EffectData::defBoost);
    }

    if constexpr (has<Optional::spaBoost, EffectData>::value) {
      setEffect<SpaBoost, EffectData, moveEffectKind>(move, EffectData::spaBoost);
    }

    if constexpr (has<Optional::spdBoost, EffectData>::value) {
      setEffect<SpdBoost, EffectData, moveEffectKind>(move, EffectData::spdBoost);
    }

    if constexpr (has<Optional::speBoost, EffectData>::value) {
      setEffect<SpeBoost, EffectData, moveEffectKind>(move, EffectData::speBoost);
    }

    setEffectTags<EffectData, moveEffectKind>(move, EffectData::effectTags);
  }

 public:
  static types::entity build(types::registry& registry, bool forActiveMove) {
    dex::internal::MoveDexDataSetup move(registry);

    if (forActiveMove) {
      move.setNameTag(T::name);
    }
    else {
      move.setName(T::name);
      move.setBasePp(T::basePp);
    }

    move.setType(T::type);
    switch (T::category) {
      case dex::MoveCategory::PHYSICAL: {
        move.setCategoryPhysical();
        break;
      }
      case dex::MoveCategory::SPECIAL: {
        move.setCategorySpecial();
        break;
      }
      case dex::MoveCategory::STATUS: {
        move.setCategoryStatus();
        break;
      }
    }

    if constexpr (has<Optional::accuracy, T>::value) {
      move.setAccuracy(T::accuracy);
    }
    if constexpr (has<Optional::basePower, T>::value) {
      move.setBasePower(T::basePower);
    }
    if constexpr (has<Optional::hitCount, T>::value) {
      move.setHitCount(T::hitCount);
    }

    if constexpr (has<Optional::sourcePrimaryEffect, T>::value) {
      buildEffect<typename T::sourcePrimaryEffect, MoveEffectKind::primary>(move);
      move.setEffectTargetsMoveSource();
    }

    if constexpr (has<Optional::targetPrimaryEffect, T>::value) {
      buildEffect<typename T::targetPrimaryEffect, MoveEffectKind::primary>(move);
      move.setEffectTargetsMoveTarget();
    }

    if constexpr (has<Optional::sourceSecondaryEffect, T>::value) {
      buildEffect<typename T::sourceSecondaryEffect, MoveEffectKind::secondary>(move);
      move.setEffectTargetsMoveSource();
    }

    if constexpr (has<Optional::targetSecondaryEffect, T>::value) {
      buildEffect<typename T::targetSecondaryEffect, MoveEffectKind::secondary>(move);
      move.setEffectTargetsMoveTarget();
    }

    move.setProperties(T::moveTags);

    switch (T::target) {
      case MoveTarget::ANY_SINGLE_TARGET: {
        move.setProperty<move::tags::AnySingleTarget>();
        break;
      }
      case MoveTarget::ANY_SINGLE_FOE: {
        move.setProperty<move::tags::AnySingleFoe>();
        break;
      }
      case MoveTarget::ANY_SINGLE_ALLY: {
        move.setProperty<move::tags::AnySingleAlly>();
        break;
      }
      case MoveTarget::ALLY_OR_SELF: {
        move.setProperty<move::tags::AllyOrSelf>();
        break;
      }
      case MoveTarget::SELF: {
        move.setProperty<move::tags::Self>();
        break;
      }
      case MoveTarget::ALL_FOES: {
        move.setProperty<move::tags::AllFoes>();
        move.addAddedTargets(AddedTargetOptions::TARGET_ALLY);
        break;
      }
      case MoveTarget::ALLIES_AND_FOES: {
        move.setProperty<move::tags::AlliesAndFoes>();
        move.addAddedTargets(AddedTargetOptions::TARGET_ALLY);
        move.addAddedTargets(AddedTargetOptions::USER_ALLY);
        break;
      }
      case MoveTarget::ALLIES_AND_SELF: {
        move.setProperty<move::tags::AlliesAndSelf>();
        // Deliberately not USER_ALLY as the target of AlliesAndSelf moves is the user
        move.addAddedTargets(AddedTargetOptions::TARGET_ALLY);
        break;
      }
      case MoveTarget::FOE_SIDE: {
        move.setProperty<move::tags::FoeSide>();
        move.addAddedTargets(AddedTargetOptions::TARGET_SIDE);
        break;
      }
      case MoveTarget::ALLY_SIDE: {
        move.setProperty<move::tags::AllySide>();
        move.addAddedTargets(AddedTargetOptions::USER_SIDE);
        break;
      }
      case MoveTarget::FIELD: {
        move.setProperty<move::tags::Field>();
        move.addAddedTargets(AddedTargetOptions::FIELD);
        break;
      }
      case MoveTarget::ALLY_TEAM: {
        move.setProperty<move::tags::AllyTeam>();
        move.addAddedTargets(AddedTargetOptions::USER_SIDE);
        break;
      }
      case MoveTarget::RETALIATION: {
        move.setProperty<move::tags::Retaliation>();
        break;
      }
      case MoveTarget::RANDOM_FOE: {
        move.setProperty<move::tags::RandomFoe>();
        break;
      }
      default: break;
    }

    return move.entity();
  }
};

template <template <GameMechanics> class T>
auto buildMoveSV(types::registry& registry, bool forActiveMove) {
  return BuildMove<T<GameMechanics::SCARLET_VIOLET>>::build(registry, forActiveMove);
}
}  // namespace

types::entity Pokedex::buildMove(dex::Move move, types::registry& registry, bool forActiveMove) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  if (isMechanics(GameMechanics::SCARLET_VIOLET)) {
    switch (move) {
      case Move::FURY_ATTACK:  return buildMoveSV<FuryAttack>(registry, forActiveMove);
      case Move::THUNDERBOLT:  return buildMoveSV<Thunderbolt>(registry, forActiveMove);
      case Move::WILL_O_WISP:  return buildMoveSV<WillOWisp>(registry, forActiveMove);
      case Move::KNOCK_OFF:    return buildMoveSV<KnockOff>(registry, forActiveMove);
      case Move::QUIVER_DANCE: return buildMoveSV<QuiverDance>(registry, forActiveMove);
      case Move::MOONBLAST:    return buildMoveSV<Moonblast>(registry, forActiveMove);
      case Move::SPLASH:       return buildMoveSV<Splash>(registry, forActiveMove);

      default: break;
    }
  }

  POKESIM_REQUIRE_FAIL("Building a move that does not exist.");
  return types::entity{};
}
}  // namespace pokesim

////////////////// END OF src/Pokedex/Setup/GetMoveBuild.cpp ///////////////////

///////////////// START OF src/Pokedex/Setup/GetItemBuild.cpp //////////////////

// TODO(aed3): Make this and the individual item files auto generated

namespace pokesim {
namespace {
template <typename T>
struct BuildItem {
 private:
 public:
  static types::entity build(types::registry& registry) {
    dex::internal::ItemDexDataSetup item(registry);

    item.setName(T::name);

    item.setProperties(T::itemTags);

    return item.entity();
  }
};

template <template <GameMechanics> class T>
auto buildItemSV(types::registry& registry) {
  return BuildItem<T<GameMechanics::SCARLET_VIOLET>>::build(registry);
}
}  // namespace

types::entity Pokedex::buildItem(dex::Item item, types::registry& registry) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  if (isMechanics(GameMechanics::SCARLET_VIOLET)) {
    switch (item) {
      case Item::ASSAULT_VEST:  return buildItemSV<AssaultVest>(registry);
      case Item::BRIGHT_POWDER: return buildItemSV<BrightPowder>(registry);
      case Item::CHOICE_SCARF:  return buildItemSV<ChoiceScarf>(registry);
      case Item::CHOICE_SPECS:  return buildItemSV<ChoiceSpecs>(registry);
      case Item::FOCUS_SASH:    return buildItemSV<FocusSash>(registry);
      case Item::LIFE_ORB:      return buildItemSV<LifeOrb>(registry);

      default: break;
    }
  }

  POKESIM_REQUIRE_FAIL("Building an item that does not exist.");
  return types::entity{};
}
}  // namespace pokesim

////////////////// END OF src/Pokedex/Setup/GetItemBuild.cpp ///////////////////

//////////////// START OF src/Pokedex/Setup/GetAbilityBuild.cpp ////////////////

// TODO(aed3): Make this and the individual ability files auto generated

namespace pokesim {
namespace {
template <typename T>
struct BuildAbility {
 private:
 public:
  static types::entity build(types::registry& registry) {
    dex::internal::AbilityDexDataSetup ability(registry);

    ability.setName(T::name);

    return ability.entity();
  }
};

template <template <GameMechanics> class T>
auto buildAbilitySV(types::registry& registry) {
  return BuildAbility<T<GameMechanics::SCARLET_VIOLET>>::build(registry);
}
}  // namespace

types::entity Pokedex::buildAbility(dex::Ability ability, types::registry& registry) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  if (isMechanics(GameMechanics::SCARLET_VIOLET)) {
    switch (ability) {
      case Ability::PLUS:   return buildAbilitySV<Plus>(registry);
      case Ability::STATIC: return buildAbilitySV<Static>(registry);

      default: break;
    }
  }

  POKESIM_REQUIRE_FAIL("Building an ability that does not exist.");
  return types::entity{};
}
}  // namespace pokesim

///////////////// END OF src/Pokedex/Setup/GetAbilityBuild.cpp /////////////////

////////////// START OF src/Pokedex/Setup/AbilityDexDataSetup.cpp //////////////

namespace pokesim::dex::internal {
void AbilityDexDataSetup::setName(Ability ability) {
  handle.emplace<AbilityName>(ability);
}
}  // namespace pokesim::dex::internal

/////////////// END OF src/Pokedex/Setup/AbilityDexDataSetup.cpp ///////////////

/////////////////////// START OF src/Pokedex/Pokedex.cpp ///////////////////////

namespace pokesim {
template <typename Build, typename T>
void Pokedex::load(entt::dense_map<T, types::entity>& map, const entt::dense_set<T>& list, Build build) {
  map.reserve(map.size() + list.size());
  for (T listItem : list) {
    if (!map.contains(listItem)) {
      map[listItem] = build(listItem);
    }
  }
}

void Pokedex::loadSpecies(const entt::dense_set<dex::Species>& speciesSet) {
  load(speciesMap, speciesSet, [this](dex::Species species) { return buildSpecies(species, dexRegistry); });
}

void Pokedex::loadItems(const entt::dense_set<dex::Item>& itemSet) {
  load(itemsMap, itemSet, [this](dex::Item item) { return buildItem(item, dexRegistry); });
}

void Pokedex::loadMoves(const entt::dense_set<dex::Move>& moveSet) {
  load(movesMap, moveSet, [this](dex::Move move) { return buildMove(move, dexRegistry, false); });
}

void Pokedex::loadAbilities(const entt::dense_set<dex::Ability>& abilitySet) {
  load(abilitiesMap, abilitySet, [this](dex::Ability ability) { return buildAbility(ability, dexRegistry); });
}

types::entity Pokedex::buildActionMove(dex::Move move, types::registry& registry) const {
  return buildMove(move, registry, true);
}
}  // namespace pokesim

//////////////////////// END OF src/Pokedex/Pokedex.cpp ////////////////////////

////////////////// START OF src/Pokedex/Moves/MoveEvents.cpp ///////////////////

namespace pokesim::dex::events {
namespace {
void knockOffOnBasePowerCheckRemovableItem(
  types::registry& registry, CurrentActionSource source, CurrentActionTarget target) {
  if (registry.get<stat::CurrentHp>(source.val).val) {
    registry.emplace_or_replace<tags::CanRemoveItem>(target.val);
  }
}

void knockOffOnAfterHitCheckRemovableItem(types::registry& registry, CurrentActionTarget target) {
  if (registry.get<stat::CurrentHp>(target.val).val) {
    registry.emplace_or_replace<tags::CanRemoveItem>(target.val);
  }
}

void knockOffOnBasePower(types::registry& registry, EventModifier& eventModifier, CurrentActionTarget target) {
  if (!registry.all_of<tags::CanRemoveItem>(target.val)) {
    return;
  }
  static constexpr auto modifier = latest::KnockOff::onBasePowerMultiplier;
  chainComponentToModifier(eventModifier, modifier);
}
}  // namespace

void KnockOff::onBasePower(Simulation& simulation) {
  simulation.viewForSelectedMoves<knockOffOnBasePowerCheckRemovableItem, Tags<move::tags::KnockOff>>();
  checkIfCanRemoveItem(simulation);
  simulation.viewForSelectedMoves<knockOffOnBasePower, Tags<move::tags::KnockOff>>();

  simulation.registry.clear<tags::CanRemoveItem>();
}

void KnockOff::onAfterHit(Simulation& simulation) {
  simulation.viewForSelectedMoves<knockOffOnAfterHitCheckRemovableItem, Tags<move::tags::KnockOff>>();
  tryRemoveItem(simulation);
  simulation.registry.clear<tags::CanRemoveItem>();
}
}  // namespace pokesim::dex::events

/////////////////// END OF src/Pokedex/Moves/MoveEvents.cpp ////////////////////

////////////////// START OF src/Pokedex/Items/ItemEvents.cpp ///////////////////

#include <cmath>

namespace pokesim::dex::events {
namespace {
void setChoiceLock(types::handle pokemonHandle, Battle battle) {
  types::entity moveSlot = pokemonHandle.registry()->get<CurrentActionMoveSlot>(battle.val).val;
  pokemonHandle.emplace<pokesim::ChoiceLock>(moveSlot);
}

template <typename Numerator>
void setMoveTargetModifier(
  types::registry& registry, const CurrentActionMovesAsTarget& moves, Numerator numerator,
  types::eventModifier denominator) {
  for (types::entity move : moves.val) {
    EventModifier* eventModifier = registry.try_get<EventModifier>(move);
    if (eventModifier) {
      chainComponentToModifier(*eventModifier, numerator, denominator);
    }
  }
}

template <typename Numerator>
void sourceModifyDamage(
  types::registry& registry, const CurrentActionMovesAsSource& moves, Numerator numerator,
  types::eventModifier denominator) {
  for (types::entity move : moves.val) {
    DamageRollModifiers& modifier = registry.get<DamageRollModifiers>(move);
    modifier.modifyDamageEvent = chainValueToModifier(modifier.modifyDamageEvent, numerator, denominator);
  }
}

template <typename SimulationTag>
struct FocusSashOnAfterModifyDamage {
  static void run(Simulation& simulation, types::damage hpToKeep) {
    simulation.viewForSelectedPokemon<modifyDamage, Tags<item::tags::FocusSash, SimulationTag, tags::CanUseItem>>(
      hpToKeep);
  }

  static void modifyDamage(
    types::handle pokemonHandle, const CurrentActionMovesAsTarget& moves, stat::CurrentHp currentHp, stat::Hp hp,
    types::damage hpToKeep) {
    if (currentHp.val != hp.val) {
      return;
    }

    types::registry& registry = *pokemonHandle.registry();
    for (types::entity move : moves.val) {
      if constexpr (std::is_same_v<tags::SimulateTurn, SimulationTag>) {
        Damage& damage = registry.get<Damage>(move);
        if (damage.val < hp.val) {
          continue;
        }

        damage.val = hp.val - hpToKeep;
        pokemonHandle.emplace<calc_damage::tags::RanAfterModifyDamage>();
      }
      else {
        DamageRolls& damageRolls = registry.get<DamageRolls>(move);
        for (Damage& roll : damageRolls.val) {
          if (roll.val < hp.val) {
            break;
          }

          roll.val = hp.val - hpToKeep;
        }
      }
    }
  }
};

void lifeOrbOnAfterMove(
  types::handle pokemonHandle, const CurrentActionMovesAsSource& moves, stat::Hp hp, types::stat hpDivisor) {
  bool onlyStatusMoves = true;
  types::registry& registry = *pokemonHandle.registry();
  for (types::entity move : moves.val) {
    onlyStatusMoves &= registry.all_of<move::tags::Status>(move);
  }

  if (!onlyStatusMoves) {
    applyDamage(pokemonHandle, hp.val / hpDivisor);
  }
}
}  // namespace

void AssaultVest::onModifySpd(Simulation& simulation) {
  static constexpr auto modifier = latest::AssaultVest::onModifySpdModifier;
  simulation.viewForSelectedPokemon<chainComponentToModifier<types::effectMultiplier>, Tags<item::tags::AssaultVest>>(
    modifier,
    1U);
}

void AssaultVest::onEnd(Simulation& simulation) {
  simulation.addToEntities<tags::SpdStatUpdateRequired, tags::SelectedForViewPokemon, item::tags::AssaultVest>();
}

void BrightPowder::onModifyAccuracy(Simulation& simulation) {
  static constexpr auto numerator = latest::BrightPowder::onModifyAccuracyNumerator;
  static constexpr auto denominator = latest::BrightPowder::onModifyAccuracyDenominator;
  simulation.viewForSelectedPokemon<setMoveTargetModifier<types::eventModifier>, Tags<item::tags::BrightPowder>>(
    numerator,
    denominator);
}

void ChoiceScarf::onModifySpe(Simulation& simulation) {
  static constexpr auto modifier = latest::ChoiceScarf::onModifySpeModifier;
  simulation.viewForSelectedPokemon<chainComponentToModifier<types::effectMultiplier>, Tags<item::tags::ChoiceScarf>>(
    modifier,
    1U);
}

void ChoiceScarf::onSourceModifyMove(Simulation& simulation) {
  simulation
    .view<setChoiceLock, Tags<item::tags::ChoiceScarf, tags::CurrentActionMoveSource>, entt::exclude_t<ChoiceLock>>();
}

void ChoiceScarf::onEnd(Simulation& simulation) {
  simulation.addToEntities<tags::SpeStatUpdateRequired, tags::SelectedForViewPokemon, item::tags::ChoiceScarf>();
}

void ChoiceSpecs::onModifySpa(Simulation& simulation) {
  static constexpr auto modifier = latest::ChoiceSpecs::onModifySpaModifier;
  simulation.viewForSelectedPokemon<chainComponentToModifier<types::effectMultiplier>, Tags<item::tags::ChoiceSpecs>>(
    modifier,
    1U);
}

void ChoiceSpecs::onSourceModifyMove(Simulation& simulation) {
  simulation
    .view<setChoiceLock, Tags<item::tags::ChoiceSpecs, tags::CurrentActionMoveSource>, entt::exclude_t<ChoiceLock>>();
}

void ChoiceSpecs::onEnd(Simulation& simulation) {
  simulation.addToEntities<tags::SpaStatUpdateRequired, tags::SelectedForViewPokemon, item::tags::ChoiceSpecs>();
}

void FocusSash::onAfterModifyDamage(Simulation& simulation) {
  static constexpr auto hpToKeep = latest::FocusSash::onAfterModifyDamageHpToKeep;
  simulation.addToEntities<tags::CanUseItem, tags::CurrentActionMoveTarget, item::tags::FocusSash>();
  checkIfCanUseItem(simulation);

  Simulation::forEachSimulationTag<FocusSashOnAfterModifyDamage>(simulation, hpToKeep);

  simulation.registry.clear<tags::CanUseItem>();
}

void FocusSash::onDamage(Simulation& simulation) {
  simulation.addToEntities<tags::CanUseItem, calc_damage::tags::RanAfterModifyDamage, item::tags::FocusSash>();
  tryUseItem(simulation);
}

void LifeOrb::onModifyDamage(Simulation& simulation) {
  static constexpr auto numerator = latest::LifeOrb::onModifyDamageNumerator;
  static constexpr auto denominator = latest::LifeOrb::onModifyDamageDenominator;
  simulation.viewForSelectedPokemon<sourceModifyDamage<types::eventModifier>, Tags<item::tags::LifeOrb>>(
    numerator,
    denominator);
}

void LifeOrb::onAfterMoveUsed(Simulation& simulation) {
  static constexpr auto divisor = latest::LifeOrb::onAfterMoveUsedHpDecreaseDivisor;
  simulation.viewForSelectedPokemon<lifeOrbOnAfterMove, Tags<item::tags::LifeOrb>>(divisor);
}
}  // namespace pokesim::dex::events

/////////////////// END OF src/Pokedex/Items/ItemEvents.cpp ////////////////////

//////////////// START OF src/Pokedex/Effects/EffectEvents.cpp /////////////////

namespace pokesim::dex::events {
namespace {

void damageByHpDivisor(types::handle pokemonHandle, stat::Hp hp, types::stat hpDivisor) {
  applyDamage(pokemonHandle, hp.val / hpDivisor);
}

void applyBurnModifier(types::registry& registry, const CurrentActionMovesAsSource& moves) {
  for (types::entity move : moves.val) {
    if (registry.all_of<move::tags::Physical>(move) /*entt::exclude<ignores burn (i.e. Facade) tag>*/) {
      registry.get<DamageRollModifiers>(move).burn = true;
    }
  }
}

void paralysisOnModifySpeed(stat::EffectiveSpe& effectiveSpe, types::stat speedDividend) {
  effectiveSpe.val = effectiveSpe.val * speedDividend / dex::latest::Paralysis::speedDivisor;
}

void paralysisOnBeforeMove(types::handle pokemonHandle, Battle battle, const CurrentActionMovesAsSource& moves) {
  types::registry& registry = *pokemonHandle.registry();
  for (types::entity move : moves.val) {
    setFailedActionMove(types::handle{registry, move}, battle);
  }
}

void choiceLockRemoveWithoutItem(types::handle pokemonHandle, const pokesim::ChoiceLock&) {
  pokemonHandle.remove<pokesim::ChoiceLock>();
}

void choiceLockRemoveWithItem(
  types::handle pokemonHandle, const ItemName& itemName, const pokesim::ChoiceLock&, const Pokedex& pokedex) {
  if (!pokedex.itemHas<item::tags::Choice>(itemName.name)) {
    pokemonHandle.remove<pokesim::ChoiceLock>();
  }
}

void choiceLockOnDisableMove(
  types::registry& registry, const pokesim::ChoiceLock& choiceLocked, const MoveSlots& moveSlots) {
  POKESIM_REQUIRE(
    std::find(moveSlots.val.begin(), moveSlots.val.end(), choiceLocked.val) != moveSlots.val.end(),
    "Should skip if the move is no longer present, but when does that happen?");

  for (types::entity entity : moveSlots.val) {
    if (entity != choiceLocked.val) {
      registry.emplace<move::tags::Disabled>(entity);
    }
  }
}
}  // namespace

void Burn::onSetDamageRollModifiers(Simulation& simulation) {
  simulation
    .viewForSelectedPokemon<applyBurnModifier, Tags<status::tags::Burn> /*, entt::exclude<ability::tags::Guts> */>();
}

void Burn::onResidual(Simulation& simulation) {
  static constexpr auto divisor = latest::Burn::onResidualHpDecreaseDivisor;
  simulation.viewForSelectedPokemon<damageByHpDivisor, Tags<status::tags::Burn>>(divisor);
}

void Paralysis::onModifySpe(Simulation& simulation) {
  constexpr auto speedDividend = dex::latest::Paralysis::speedDividend;
  simulation.viewForSelectedPokemon<
    paralysisOnModifySpeed,
    Tags<status::tags::Paralysis> /*, entt::exclude_t<ability::tags::QuickFeet>*/>(speedDividend);
}

void Paralysis::onBeforeMove(Simulation& simulation) {
  constexpr auto chance = dex::latest::Paralysis::onBeforeMoveChance;
  simulation.addToEntities<BaseEffectChance, tags::CurrentActionMoveSource, status::tags::Paralysis>(
    BaseEffectChance{chance});
  runRandomBinaryChance<BaseEffectChance, tags::CurrentActionMoveSource>(simulation, [](Simulation& sim) {
    sim.viewForSelectedPokemon<paralysisOnBeforeMove, Tags<tags::RandomEventCheckPassed>>();
  });
  simulation.removeFromEntities<BaseEffectChance, tags::CurrentActionMoveSource, status::tags::Paralysis>();
}

void ChoiceLock::onBeforeMove(Simulation& simulation) {
  simulation.viewForSelectedPokemon<choiceLockRemoveWithoutItem, Tags<>, entt::exclude_t<ItemName>>();
  simulation.viewForSelectedPokemon<choiceLockRemoveWithItem>(simulation.pokedex());
}

void ChoiceLock::onDisableMove(Simulation& simulation) {
  simulation.viewForSelectedPokemon<choiceLockRemoveWithoutItem, Tags<>, entt::exclude_t<ItemName>>();
  simulation.viewForSelectedPokemon<choiceLockRemoveWithItem>(simulation.pokedex());
  simulation.viewForSelectedPokemon<choiceLockOnDisableMove>();
}
}  // namespace pokesim::dex::events

///////////////// END OF src/Pokedex/Effects/EffectEvents.cpp //////////////////

/////////////// START OF src/Pokedex/Abilities/AbilityEvents.cpp ///////////////

namespace pokesim::dex::events {
namespace {
void plusOnModifySpa(types::handle, EventModifier&) {}

void staticOnDamagingHit(
  types::handle targetHandle, const CurrentActionMovesAsTarget& moves, types::percentChance chanceOfStatic) {
  types::registry& registry = *targetHandle.registry();
  for (types::entity move : moves.val) {
    if (!registry.all_of<move::tags::Contact>(move)) {
      continue;
    }
    types::entity source = registry.get<CurrentActionSource>(move).val;
    /*
    if (registry.all_of<item::tags::ProtectivePads>(source)) {
      continue;
    }
    */

    types::entity effectSource = targetHandle.entity();
    types::entity effectTarget = source;
    registry.emplace<BaseEffectChance>(move, chanceOfStatic);
    registry.emplace_or_replace<status::tags::Paralysis>(move);
    registry.emplace<CurrentEffectSource>(move, effectSource);
    registry.emplace<CurrentEffectsAsSource>(effectSource, types::entityVector{move});
    registry.emplace<CurrentEffectTarget>(move, effectTarget);
    registry.emplace<CurrentEffectsAsTarget>(effectTarget, types::entityVector{move});
  }
}
}  // namespace

void Plus::onModifySpA(Simulation& simulation) {
  if (simulation.isBattleFormat(BattleFormat::SINGLES_BATTLE_FORMAT)) {
    return;
  }
  simulation.viewForSelectedPokemon<plusOnModifySpa, Tags<ability::tags::Plus>>();
}

void Static::onDamagingHit(Simulation& simulation) {
  static constexpr auto chanceOfStatic = dex::latest::Static::onDamagingHitChance;
  simulation.viewForSelectedPokemon<staticOnDamagingHit, Tags<ability::tags::Static>>(chanceOfStatic);
  checkIfCanSetStatus(simulation);
  runRandomBinaryChance<BaseEffectChance, tags::CanSetStatus>(simulation, [](Simulation& sim) {
    sim.removeFromEntities<tags::CanSetStatus, tags::SelectedForViewMove, tags::RandomEventCheckFailed>();
  });
  setStatus(simulation);
  simulation.registry.clear<CurrentEffectSource, CurrentEffectTarget, CurrentEffectsAsSource, CurrentEffectsAsTarget>();
}
}  // namespace pokesim::dex::events

//////////////// END OF src/Pokedex/Abilities/AbilityEvents.cpp ////////////////

//////////// START OF src/CalcDamage/Setup/CalcDamageInputSetup.cpp ////////////

namespace pokesim::calc_damage {
InputSetup::InputSetup(types::registry& _registry) : registry(&_registry) {}

void InputSetup::setup(
  types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity, dex::Move move,
  const Pokedex& pokedex) {
  moveEntity = createActionMoveForTarget({*registry, targetEntity}, battleEntity, sourceEntity, move, pokedex);
  types::handle handle{*registry, moveEntity};

  handle.emplace<MoveName>(move);
  registry->emplace_or_replace<tags::Attacker>(sourceEntity);
  registry->emplace_or_replace<tags::Defender>(targetEntity);
}

types::entity InputSetup::entity() const {
  POKESIM_REQUIRE(moveEntity != entt::null, "Getting move entity before proper setup.");
  return moveEntity;
}
}  // namespace pokesim::calc_damage

///////////// END OF src/CalcDamage/Setup/CalcDamageInputSetup.cpp /////////////

//////////////////// START OF src/CalcDamage/CalcDamage.cpp ////////////////////

namespace pokesim::calc_damage {
namespace {
void clearRunVariables(Simulation& simulation) {
  simulation.registry.clear<
    tags::Crit,
    AttackingLevel,
    AttackingStat,
    DefendingStat,
    DamageRollModifiers,
    Power,
    tags::UsesAtk,
    tags::UsesDef,
    tags::UsesSpa,
    tags::UsesSpd,
    tags::UsesDefAsOffense,
    tags::IgnoresAttackingBoost,
    tags::IgnoresDefendingBoost>();
  simulation.removeFromEntities<Damage, pokesim::tags::CalculateDamage>();
}

void checkForAndApplyStab(
  types::handle moveHandle, const Attacker& attacker, TypeName type, DamageRollModifiers& modifier) {
  const SpeciesTypes& attackerTypes = moveHandle.registry()->get<SpeciesTypes>(attacker.val);

  if (attackerTypes.has(type.name)) {
    modifier.stab = StabBoostKind::STANDARD;
  }
}

void checkForAndApplyTypeEffectiveness(
  types::handle moveHandle, Defender defender, TypeName type, DamageRollModifiers& modifier, const Pokedex& pokedex) {
  const SpeciesTypes& defenderTypes = moveHandle.registry()->get<SpeciesTypes>(defender.val);

  modifier.typeEffectiveness = getAttackEffectiveness(defenderTypes, type.name, pokedex.typeChart());
}

void applyCritDamageIncrease(Damage& damage) {
  damage.val = (types::damage)(damage.val * MechanicConstants::CRIT_MULTIPLIER);
}

void setDamageToMinimumPossible(Damage& damage) {
  damage.val = std::max(damage.val, MechanicConstants::Damage::MIN);
}

void setDefendingSide(types::handle moveHandle, Defender defender) {
  types::registry& registry = *moveHandle.registry();
  PlayerSideId playerSide = registry.get<PlayerSide>(registry.get<Side>(defender.val).val).val;
  switch (playerSide) {
    case PlayerSideId::NONE: {
      POKESIM_REQUIRE_FAIL("Player side wasn't set properly.");
      break;
    }
    case PlayerSideId::P1: {
      moveHandle.emplace<tags::P1Defending>();
      break;
    }
    case PlayerSideId::P2: {
      moveHandle.emplace<tags::P2Defending>();
      break;
    }
  }
}

void modifyDamage(Damage& damage, const DamageRollModifiers& modifiers) {
  types::effectMultiplier stab = ((std::underlying_type_t<StabBoostKind>)modifiers.stab) / 100.0F;
  damage.val = (types::damage)fixedPointMultiply(damage.val, stab);

  types::eventModifier typeEffectivenessModifier = MechanicConstants::FIXED_POINT_SCALE;
  if (modifiers.typeEffectiveness < 0) {
    typeEffectivenessModifier = typeEffectivenessModifier >> -modifiers.typeEffectiveness;
  }
  else {
    typeEffectivenessModifier = typeEffectivenessModifier << modifiers.typeEffectiveness;
  }

  damage.val = applyChainedModifier(damage.val, typeEffectivenessModifier);
  damage.val = applyChainedModifier(damage.val, modifiers.modifyDamageEvent);

  if (modifiers.burn) {
    damage.val = (types::damage)fixedPointMultiply(damage.val, dex::latest::Burn::physicalDamageMultiplier);
  }

  setDamageToMinimumPossible(damage);
}

void calculateAllDamageRolls(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  damageRolls.val.reserve(MechanicConstants::DamageRollCount::MAX);
  for (types::damageRollIndex i = 0U; i < MechanicConstants::DamageRollCount::MAX; i++) {
    Damage& damageRoll = damageRolls.val.emplace_back(damage);
    applyDamageRoll(damageRoll.val, i);
    modifyDamage(damageRoll, modifier);
  }
}

void applyAverageDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  modifyDamage(damage, modifier);
  applyAverageDamageRoll(damage.val);
  damageRolls.val.emplace_back(damage);
}

void applyMinDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  applyMinDamageRoll(damage.val);
  modifyDamage(damage, modifier);
  damageRolls.val.emplace_back(damage);
}

void applyDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  modifyDamage(damage, modifier);
  damageRolls.val.emplace_back(damage);
}

void reduceDamageRollsToDefenderHp(
  types::handle moveHandle, DamageRolls& damageRolls, Damage& damage, Defender defender) {
  const stat::CurrentHp& defenderHp = moveHandle.registry()->get<stat::CurrentHp>(defender.val);
  for (auto& damageRoll : damageRolls.val) {
    damageRoll.val = std::min(defenderHp.val, damageRoll.val);
  }
  damage.val = std::min(defenderHp.val, damage.val);
}

void assignCritChanceDivisor(types::handle moveHandle, CritBoost critBoost) {
  std::size_t index =
    std::min((std::size_t)critBoost.val, pokesim::MechanicConstants::CRIT_CHANCE_DIVISORS.size() - 1U);
  moveHandle.emplace<CritChanceDivisor>(pokesim::MechanicConstants::CRIT_CHANCE_DIVISORS[index]);
}

void setSourceLevel(types::handle moveHandle, Attacker attacker) {
  moveHandle.emplace<AttackingLevel>(moveHandle.registry()->get<Level>(attacker.val).val);
}

template <typename Category>
void setUsedAttackStat(types::handle moveHandle, Attacker attacker) {
  types::stat attackingStat = MechanicConstants::PokemonEffectiveStat::MIN;
  if constexpr (std::is_same_v<Category, move::tags::Physical>) {
    attackingStat = moveHandle.registry()->get<stat::EffectiveAtk>(attacker.val).val;
    moveHandle.emplace<tags::UsesAtk>();
  }
  else {
    attackingStat = moveHandle.registry()->get<stat::EffectiveSpa>(attacker.val).val;
    moveHandle.emplace<tags::UsesSpa>();
  }
  moveHandle.emplace<AttackingStat>(attackingStat);
}

template <typename Category>
void setUsedDefenseStat(types::handle moveHandle, Defender defender) {
  types::stat defendingStat = MechanicConstants::PokemonEffectiveStat::MIN;
  if constexpr (std::is_same_v<Category, move::tags::Physical>) {
    defendingStat = moveHandle.registry()->get<stat::EffectiveDef>(defender.val).val;
    moveHandle.emplace<tags::UsesDef>();
  }
  else {
    defendingStat = moveHandle.registry()->get<stat::EffectiveSpd>(defender.val).val;
    moveHandle.emplace<tags::UsesSpd>();
  }
  moveHandle.emplace<DefendingStat>(defendingStat);
}

template <typename BoostType>
void setIgnoreAttackingBoostIfNegative(types::handle moveHandle, Attacker attacker) {
  BoostType* boost = moveHandle.registry()->try_get<BoostType>(attacker.val);
  if (boost && boost->val < MechanicConstants::PokemonStatBoost::BASE) {
    moveHandle.emplace<tags::IgnoresAttackingBoost>();
  }
}

template <typename BoostType>
void setIgnoreDefendingBoostIfPositive(types::handle moveHandle, Defender defender) {
  BoostType* boost = moveHandle.registry()->try_get<BoostType>(defender.val);
  if (boost && boost->val > MechanicConstants::PokemonStatBoost::BASE) {
    moveHandle.emplace<tags::IgnoresDefendingBoost>();
  }
}

void calculateBaseDamage(
  types::handle moveHandle, Power power, AttackingLevel level, AttackingStat attack, DefendingStat defense) {
  // NOLINTNEXTLINE(readability-magic-numbers)
  types::damage damage = ((((2U * level.val / 5U + 2U) * power.val * attack.val) / defense.val) / 50U) + 2U;
  moveHandle.emplace_or_replace<Damage>(damage);
}

void applyUsesUntilKo(types::handle moveHandle, const DamageRolls& damageRolls, Defender defender) {
  const stat::CurrentHp& defenderHp = moveHandle.registry()->get<stat::CurrentHp>(defender.val);
  UsesUntilKo usesUntilKo;
  POKESIM_REQUIRE(
    damageRolls.val.size() == MechanicConstants::DamageRollCount::MAX,
    "All the damage rolls are needed to calculate this correctly.");

  for (const Damage& damageRoll : damageRolls.val) {
    types::moveHits hits = (types::moveHits)std::ceil(defenderHp.val / (types::probability)damageRoll.val);
    if (usesUntilKo.val.empty() || usesUntilKo.val.back().hits != hits) {
      usesUntilKo.val.push_back({hits});
    }

    usesUntilKo.val.back().damageRollsIncluded++;
  }
  moveHandle.emplace<UsesUntilKo>(usesUntilKo);
}

template <typename SimulationTag, auto ApplyDamageRollKind>
void applySideDamageRollOptions(Simulation& simulation) {
  static constexpr bool isSimulateTurn = std::is_same_v<pokesim::tags::SimulateTurn, SimulationTag>;
  static constexpr bool isCalculateDamage = std::is_same_v<pokesim::tags::CalculateDamage, SimulationTag>;
  static constexpr bool isAnalyzeEffect = std::is_same_v<pokesim::tags::AnalyzeEffect, SimulationTag>;

  static_assert(
    isSimulateTurn || isCalculateDamage || isAnalyzeEffect,
    "Using a type that isn't a valid simulation tag.");

  pokesim::internal::SelectForCurrentActionMoveView<SimulationTag> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  static constexpr bool onlyPassDamageRoll = pokesim::internal::getArgumentCount(ApplyDamageRollKind) == 2U;

  DamageRollOptions damageRollOptions;
  bool noKoChanceCalculation = false;
  bool calculateUpToFoeHp = false;
  if constexpr (isSimulateTurn) {
    damageRollOptions = simulation.simulateTurnOptions.damageRollsConsidered;
    calculateUpToFoeHp = true;
  }
  else if constexpr (isCalculateDamage) {
    damageRollOptions = simulation.calculateDamageOptions.damageRollOptions;
    calculateUpToFoeHp = simulation.calculateDamageOptions.calculateUpToFoeHp;
    noKoChanceCalculation = simulation.calculateDamageOptions.noKoChanceCalculation;
  }
  else if constexpr (isAnalyzeEffect) {
    damageRollOptions = simulation.analyzeEffectOptions.damageRollOptions;
    calculateUpToFoeHp = simulation.analyzeEffectOptions.calculateUpToFoeHp;
    noKoChanceCalculation = simulation.calculateDamageOptions.noKoChanceCalculation;
  }

  if (damageRollOptions.sidesMatch()) {
    if constexpr (onlyPassDamageRoll) {
      ApplyDamageRollKind(simulation, damageRollOptions.p1);
    }
    else {
      ApplyDamageRollKind(simulation, damageRollOptions.p1, calculateUpToFoeHp, noKoChanceCalculation);
    }
  }
  else {
    simulation.viewForSelectedMoves<setDefendingSide>();
    pokesim::internal::SelectForCurrentActionMoveView<tags::P1Defending> p1DefendingMoves{simulation};
    if (!p1DefendingMoves.hasNoneSelected()) {
      if constexpr (onlyPassDamageRoll) {
        ApplyDamageRollKind(simulation, damageRollOptions.p1);
      }
      else {
        ApplyDamageRollKind(simulation, damageRollOptions.p1, calculateUpToFoeHp, noKoChanceCalculation);
      }
    }
    p1DefendingMoves.deselect();

    pokesim::internal::SelectForCurrentActionMoveView<tags::P2Defending> p2DefendingMoves{simulation};
    if (!p2DefendingMoves.hasNoneSelected()) {
      if constexpr (onlyPassDamageRoll) {
        ApplyDamageRollKind(simulation, damageRollOptions.p2);
      }
      else {
        ApplyDamageRollKind(simulation, damageRollOptions.p2, calculateUpToFoeHp, noKoChanceCalculation);
      }
    }

    simulation.registry.clear<tags::P1Defending, tags::P2Defending>();
  }
}

template <typename SimulationTag>
void setIfMoveCrits(Simulation& simulation, DamageRollKind damageRollKind) {
  if (damageRollKind & DamageRollKind::GUARANTEED_CRIT_CHANCE) {
    simulation.addToEntities<tags::Crit, pokesim::tags::SelectedForViewMove>();
    return;
  }

  if constexpr (std::is_same_v<SimulationTag, pokesim::tags::SimulateTurn>) {
    simulation.addToEntities<calc_damage::CritBoost, pokesim::tags::SelectedForViewMove, pokesim::tags::SimulateTurn>();
    runModifyCritBoostEvent(simulation);
    simulation.viewForSelectedMoves<assignCritChanceDivisor>();
    simulation.registry.clear<CritBoost>();

    simulate_turn::setIfMoveCrits(simulation);
  }
}

template <typename SimulationTag>
void applyDamageRollsAndModifiers(
  Simulation& simulation, DamageRollKind damageRollKind, bool calculateUpToFoeHp, bool noKoChanceCalculation) {
  POKESIM_REQUIRE(
    damageRollKind != DamageRollKind::NONE,
    "Cannot calculate damage without knowing what rolls to consider.");
  POKESIM_REQUIRE(
    damageRollKind != DamageRollKind::GUARANTEED_CRIT_CHANCE,
    "Must pick a damage roll kind to go along with crits.");

  simulation.addToEntities<DamageRolls, DamageRollModifiers, pokesim::tags::SelectedForViewMove>();
  if (damageRollKind & DamageRollKind::ALL_DAMAGE_ROLLS) {
    simulation.viewForSelectedMoves<calculateAllDamageRolls>();
  }
  else {
    if (damageRollKind & DamageRollKind::MAX_DAMAGE) {
      simulation.viewForSelectedMoves<applyDamageRollModifier>();
    }

    if (damageRollKind & DamageRollKind::AVERAGE_DAMAGE) {
      simulation.viewForSelectedMoves<applyAverageDamageRollModifier>();
    }

    if (damageRollKind & DamageRollKind::MIN_DAMAGE) {
      simulation.viewForSelectedMoves<applyMinDamageRollModifier>();
    }
  }

  if constexpr (std::is_same_v<pokesim::tags::SimulateTurn, SimulationTag>) {
    if (calculateUpToFoeHp) {
      simulation.viewForSelectedMoves<reduceDamageRollsToDefenderHp>();
    }
    simulate_turn::cloneFromDamageRolls(simulation, damageRollKind);
  }
  else {
    simulation.viewForSelectedMoves<modifyDamage>();
    if (calculateUpToFoeHp) {
      simulation.viewForSelectedMoves<reduceDamageRollsToDefenderHp>();
    }
    if (!noKoChanceCalculation && damageRollKind & DamageRollKind::ALL_DAMAGE_ROLLS) {
      simulation.viewForSelectedMoves<applyUsesUntilKo, Tags<SimulationTag>>();
    }
  }
}

template <typename EffectiveStat>
void saveRealEffectiveAttackerStat(types::registry& registry, Attacker attacker) {
  EffectiveStat effectiveStat = registry.get<EffectiveStat>(attacker.val);
  registry.emplace<RealEffectiveStat>(attacker.val, effectiveStat.val);
}

template <typename EffectiveStat>
void saveRealEffectiveDefenderStat(types::registry& registry, Defender defender) {
  EffectiveStat effectiveStat = registry.get<EffectiveStat>(defender.val);
  registry.emplace<RealEffectiveStat>(defender.val, effectiveStat.val);
}

template <typename EffectiveStat>
void resetEffectiveAndAttackingStat(types::registry& registry, Attacker attacker, AttackingStat& attackingStat) {
  auto [effectiveStat, realEffectiveStat] = registry.get<EffectiveStat, RealEffectiveStat>(attacker.val);
  attackingStat.val = effectiveStat.val;
  effectiveStat.val = realEffectiveStat.val;
}

template <typename EffectiveStat>
void resetEffectiveAndDefendingStat(types::registry& registry, Defender defender, DefendingStat& defendingStat) {
  auto [effectiveStat, realEffectiveStat] = registry.get<EffectiveStat, RealEffectiveStat>(defender.val);
  defendingStat.val = effectiveStat.val;
  effectiveStat.val = realEffectiveStat.val;
}

template <typename EffectiveStat, typename IgnoresBoostTag, typename UsesStatTag>
void setUnboostedStat(Simulation& simulation) {
  static constexpr bool forAttacker = std::is_same_v<IgnoresBoostTag, tags::IgnoresAttackingBoost>;
  if constexpr (forAttacker) {
    simulation.viewForSelectedMoves<saveRealEffectiveAttackerStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }
  else {
    simulation.viewForSelectedMoves<saveRealEffectiveDefenderStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }

  internal::SelectForPokemonView<RealEffectiveStat> selectedPokemon{simulation};
  if (selectedPokemon.hasNoneSelected()) {
    return;
  }

  if constexpr (std::is_same_v<UsesStatTag, tags::UsesAtk>) {
    simulation.viewForSelectedPokemon<resetEffectiveAtk>();
    runModifyAtk(simulation);
  }
  else if constexpr (
    std::is_same_v<UsesStatTag, tags::UsesDef> || std::is_same_v<UsesStatTag, tags::UsesDefAsOffense>) {
    simulation.viewForSelectedPokemon<resetEffectiveDef>();
    runModifyDef(simulation);
  }
  else if constexpr (std::is_same_v<UsesStatTag, tags::UsesSpa>) {
    simulation.viewForSelectedPokemon<resetEffectiveSpa>();
    runModifySpa(simulation);
  }
  else if constexpr (std::is_same_v<UsesStatTag, tags::UsesSpd>) {
    simulation.viewForSelectedPokemon<resetEffectiveSpd>();
    runModifySpd(simulation);
  }
  else {
    static_assert("No other stat is used as the attacking or defending stat.");
  }

  if constexpr (forAttacker) {
    simulation
      .viewForSelectedMoves<resetEffectiveAndAttackingStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }
  else {
    simulation
      .viewForSelectedMoves<resetEffectiveAndDefendingStat<EffectiveStat>, Tags<IgnoresBoostTag, UsesStatTag>>();
  }

  simulation.registry.clear<RealEffectiveStat>();
}

void setDamageFormulaVariables(Simulation& simulation) {
  simulation.viewForSelectedMoves<setSourceLevel>();

  simulation.viewForSelectedMoves<setUsedAttackStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<setUsedAttackStat<move::tags::Special>, Tags<move::tags::Special>>();
  simulation.viewForSelectedMoves<setUsedDefenseStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<setUsedDefenseStat<move::tags::Special>, Tags<move::tags::Special>>();

  simulation.viewForSelectedMoves<setIgnoreAttackingBoostIfNegative<AtkBoost>, Tags<tags::Crit, tags::UsesAtk>>();
  simulation.viewForSelectedMoves<setIgnoreAttackingBoostIfNegative<SpaBoost>, Tags<tags::Crit, tags::UsesSpa>>();
  simulation.viewForSelectedMoves<setIgnoreDefendingBoostIfPositive<DefBoost>, Tags<tags::Crit, tags::UsesDef>>();
  simulation.viewForSelectedMoves<setIgnoreDefendingBoostIfPositive<SpdBoost>, Tags<tags::Crit, tags::UsesSpd>>();
  // simulation.viewForSelectedMoves<setIgnoreAttackingBoostIfNegative<DefBoost>, Tags<tags::Crit,
  // tags::UsesDefAsOffense>>();

  // simulation.viewForSelectedPokemon<dex::latest::Unaware::onUsesBoost, Tags<ability::tags::Unaware,
  // tags::Attacker>>();

  setUnboostedStat<stat::EffectiveAtk, tags::IgnoresAttackingBoost, tags::UsesAtk>(simulation);
  setUnboostedStat<stat::EffectiveSpa, tags::IgnoresAttackingBoost, tags::UsesSpa>(simulation);
  setUnboostedStat<stat::EffectiveDef, tags::IgnoresDefendingBoost, tags::UsesDef>(simulation);
  setUnboostedStat<stat::EffectiveSpd, tags::IgnoresDefendingBoost, tags::UsesSpd>(simulation);
  // setUnboostedStat<stat::EffectiveDef, tags::IgnoresAttackingBoost, tags::UsesDefAsOffense>(simulation);
}

void calcDamage(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<> selectedMoves{simulation, entt::exclude<move::tags::Status>};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  using SimulateTurn = pokesim::tags::SimulateTurn;
  using CalculateDamage = pokesim::tags::CalculateDamage;
  using AnalyzeEffect = pokesim::tags::AnalyzeEffect;

  applySideDamageRollOptions<SimulateTurn, setIfMoveCrits<SimulateTurn>>(simulation);
  applySideDamageRollOptions<CalculateDamage, setIfMoveCrits<CalculateDamage>>(simulation);
  applySideDamageRollOptions<AnalyzeEffect, setIfMoveCrits<AnalyzeEffect>>(simulation);

  runBasePowerEvent(simulation);
  setDamageFormulaVariables(simulation);

  simulation.viewForSelectedMoves<calculateBaseDamage>();
  simulation.viewForSelectedMoves<applyCritDamageIncrease, Tags<tags::Crit>>();

  simulation.addToEntities<DamageRollModifiers, pokesim::tags::SelectedForViewMove>();
  setDamageRollModifiers(simulation);

  applySideDamageRollOptions<SimulateTurn, applyDamageRollsAndModifiers<SimulateTurn>>(simulation);
  applySideDamageRollOptions<CalculateDamage, applyDamageRollsAndModifiers<CalculateDamage>>(simulation);
  applySideDamageRollOptions<AnalyzeEffect, applyDamageRollsAndModifiers<AnalyzeEffect>>(simulation);

  runAfterModifyDamageEvent(simulation);

  clearRunVariables(simulation);
}
}  // namespace

void applyDamageRoll(types::damage& damage, types::damageRollIndex damageRoll) {
  damage = (types::damage)(damage * ((100U - damageRoll) / 100.0F));
}

void applyAverageDamageRoll(types::damage& damage) {
  damage = (types::damage)(damage * (100U - (MechanicConstants::DamageRollCount::MAX - 1U) / 2.0F) / 100.0F);
}

void applyMinDamageRoll(types::damage& damage) {
  applyDamageRoll(damage, MechanicConstants::DamageRollCount::MAX - 1U);
}

void setDamageRollModifiers(Simulation& simulation) {
  simulation.viewForSelectedMoves<checkForAndApplyStab>();
  simulation.viewForSelectedMoves<checkForAndApplyTypeEffectiveness>(simulation.pokedex());
  dex::events::Burn::onSetDamageRollModifiers(simulation);
  runModifyDamageEvent(simulation);
}

void run(Simulation& simulation) {
  debug::Checks debugChecks(simulation);
  debugChecks.checkInputs();

  calcDamage(simulation);

  debugChecks.checkOutputs();
}
}  // namespace pokesim::calc_damage

///////////////////// END OF src/CalcDamage/CalcDamage.cpp /////////////////////

///////////////// START OF src/Battle/Side/ManageSideState.cpp /////////////////

namespace pokesim {
types::teamPositionIndex sidePokemonLeft(const types::registry& registry, types::entity sideEntity) {
  const Team& team = registry.get<Team>(sideEntity);
  types::teamPositionIndex pokemonLeft = 0U;
  for (types::entity pokemon : team.val) {
    if (!registry.all_of<tags::Fainted>(pokemon)) {
      pokemonLeft++;
    }
  }
  return pokemonLeft;
}

types::teamPositionIndex foeSidePokemonLeft(const types::registry& registry, types::entity sideEntity) {
  return sidePokemonLeft(registry, registry.get<FoeSide>(sideEntity).val);
}
}  // namespace pokesim

////////////////// END OF src/Battle/Side/ManageSideState.cpp //////////////////

///////////////// START OF src/Battle/Setup/SideStateSetup.cpp /////////////////

namespace pokesim {
SideStateSetup::SideStateSetup(types::registry& registry, types::entity entity, PlayerSideId playerSideId)
    : StateSetupBase(registry, entity) {
  handle.emplace<tags::Side>();
  handle.emplace<PlayerSide>(playerSideId);
}

void SideStateSetup::initBlank() {
  handle.emplace<Battle>();
  handle.emplace<Team>();
  handle.emplace<FoeSide>();
}

void SideStateSetup::setTeam(std::vector<PokemonStateSetup>& team) {
  Team& teamEntities = handle.emplace<Team>();
  Battle battle = handle.get<Battle>();
  POKESIM_REQUIRE(team.size() <= teamEntities.val.max_size(), "Cannot add more Pokemon to a team than MAX_TEAM_SIZE.");

  for (PokemonStateSetup& pokemonSetup : team) {
    teamEntities.val.push_back(pokemonSetup.entity());
    pokemonSetup.setPostion(teamEntities.val.size());
    pokemonSetup.setSide(entity());
    pokemonSetup.setBattle(battle.val);
  }
}

void SideStateSetup::setOpponent(types::entity entity) {
  handle.emplace<FoeSide>(entity);
}

void SideStateSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
}

void SideStateSetup::setPlayerSide(PlayerSideId playerSideId) {
  handle.emplace<PlayerSide>(playerSideId);
}
}  // namespace pokesim

////////////////// END OF src/Battle/Setup/SideStateSetup.cpp //////////////////

/////////////// START OF src/Battle/Setup/PokemonStateSetup.cpp ////////////////

namespace pokesim {
PokemonStateSetup::PokemonStateSetup(types::registry& registry, types::entity entity)
    : StateSetupBase(registry, entity) {
  handle.emplace<tags::Pokemon>();
}

void PokemonStateSetup::initBlank() {
  handle.emplace<SpeciesName>();
  handle.emplace<Side>();
  handle.emplace<Battle>();
  setAutoID();
}

void PokemonStateSetup::setAutoID() {
  setID((uint16_t)handle.registry()->view<SpeciesName>().size() + 1U);
}

void PokemonStateSetup::setID(types::stateId id) {
  handle.emplace<Id>(id);
}

void PokemonStateSetup::setSpecies(dex::Species speciesName) {
  handle.emplace<SpeciesName>(speciesName);
}

void PokemonStateSetup::setSide(types::entity entity) {
  handle.emplace<Side>(entity);
}

void PokemonStateSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
}

void PokemonStateSetup::setCurrentHp(types::stat hp) {
  handle.emplace<stat::CurrentHp>(hp);
}

void PokemonStateSetup::setTypes(SpeciesTypes types) {
  handle.emplace<SpeciesTypes>(types);
  type::tags::emplaceTagFromEnum(types.type1(), handle);
  type::tags::emplaceTagFromEnum(types.type2(), handle);
}

void PokemonStateSetup::setLevel(types::level level) {
  handle.emplace<Level>(level);
}

void PokemonStateSetup::setGender(dex::Gender gender) {
  handle.emplace<GenderName>(gender);
}

void PokemonStateSetup::setAbility(dex::Ability ability) {
  handle.emplace<AbilityName>(ability);
  ability::tags::emplaceTagFromEnum(ability, handle);
}

void PokemonStateSetup::setItem(dex::Item item) {
  handle.emplace<ItemName>(item);
  item::tags::emplaceTagFromEnum(item, handle);
}

void PokemonStateSetup::setMoves(const std::vector<types::entity>& moveSlots) {
  MoveSlots& moveEntities = handle.emplace<MoveSlots>();
  POKESIM_REQUIRE(
    moveSlots.size() <= moveEntities.val.max_size(),
    "Cannot add more moves to a Pokemon than MAX_MOVE_SLOTS.");
  for (types::entity moveSlot : moveSlots) {
    moveEntities.val.push_back(moveSlot);
  }
}

void PokemonStateSetup::setPostion(types::teamPositionIndex position) {
  handle.emplace<Position>(position);
}

void PokemonStateSetup::setStatus(dex::Status status) {
  handle.emplace<StatusName>(status);
  status::tags::emplaceTagFromEnum(status, handle);
}

void PokemonStateSetup::setNature(dex::Nature nature) {
  handle.emplace<NatureName>(nature);
  nature::tags::emplaceTagFromEnum(nature, handle);
}

void PokemonStateSetup::setEVs(
  types::ev hp, types::ev atk, types::ev def, types::ev spa, types::ev spd, types::ev spe) {
  handle.emplace<Evs>(hp, atk, def, spa, spd, spe);
}

void PokemonStateSetup::setEVs(const Evs& evs) {
  handle.emplace<Evs>(evs);
}

void PokemonStateSetup::setIVs(
  types::iv hp, types::iv atk, types::iv def, types::iv spa, types::iv spd, types::iv spe) {
  handle.emplace<Ivs>(hp, atk, def, spa, spd, spe);
}

void PokemonStateSetup::setIVs(const Ivs& ivs) {
  handle.emplace<Ivs>(ivs);
}
}  // namespace pokesim

//////////////// END OF src/Battle/Setup/PokemonStateSetup.cpp /////////////////

///////////////// START OF src/Battle/Setup/MoveStateSetup.cpp /////////////////

namespace pokesim {
void MoveStateSetup::initBlank() {
  handle.emplace<MoveName>();
  handle.emplace<Pp>();
  handle.emplace<MaxPp>();
}

void MoveStateSetup::setName(dex::Move moveName) {
  handle.emplace<MoveName>(moveName);
}

void MoveStateSetup::setPP(types::pp pp) {
  handle.emplace<Pp>(pp);
}

void MoveStateSetup::setMaxPP(types::pp maxPp) {
  handle.emplace<MaxPp>(maxPp);
}
}  // namespace pokesim

////////////////// END OF src/Battle/Setup/MoveStateSetup.cpp //////////////////

/////////////// START OF src/Battle/Setup/EmplaceTagFromEnum.cpp ///////////////

namespace pokesim {
namespace {
template <typename Tag>
struct EmplaceTag {
  static void run(types::handle handle) { handle.emplace<Tag>(); }
};
}  // namespace

void ability::tags::emplaceTagFromEnum(dex::Ability ability, types::handle handle) {
  enumToTag<EmplaceTag>(ability, handle);
}

void item::tags::emplaceTagFromEnum(dex::Item item, types::handle handle) {
  enumToTag<EmplaceTag>(item, handle);
}

void nature::tags::emplaceTagFromEnum(dex::Nature nature, types::handle handle) {
  enumToTag<EmplaceTag>(nature, handle);
}

void status::tags::emplaceTagFromEnum(dex::Status status, types::handle handle) {
  enumToTag<EmplaceTag>(status, handle);
}

void type::tags::emplaceTagFromEnum(dex::Type type, types::handle handle) {
  enumToTag<EmplaceTag>(type, handle);
}

void move::tags::emplaceTagFromEnum(dex::Move move, types::handle handle) {
  enumToTag<EmplaceTag>(move, handle);
}
}  // namespace pokesim

//////////////// END OF src/Battle/Setup/EmplaceTagFromEnum.cpp ////////////////

//////////////// START OF src/Battle/Setup/BattleStateSetup.cpp ////////////////

#include <atomic>

namespace pokesim {
BattleStateSetup::BattleStateSetup(types::registry& registry, types::entity entity) : StateSetupBase(registry, entity) {
  if (!handle.any_of<ActionQueue, tags::Battle>()) {
    handle.emplace<ActionQueue>();
    handle.emplace<tags::Battle>();
  }
}

void BattleStateSetup::initBlank() {
  handle.emplace<Sides>();
  handle.emplace<ActionQueue>();
  setAutoID();
  setTurn(MechanicConstants::TurnCount::MIN);
  setProbability(MechanicConstants::Probability::MAX);
}

void BattleStateSetup::setAutoID() {
  setID((types::stateId)handle.registry()->view<Sides>().size());
}

void BattleStateSetup::setID(types::stateId id) {
  handle.emplace_or_replace<Id>(id);
}

void BattleStateSetup::setSide(types::entity sideEntity) {
  auto& sides = handle.get_or_emplace<Sides>();
  PlayerSideId sideId = handle.registry()->get<PlayerSide>(sideEntity).val;
  switch (sideId) {
    case PlayerSideId::P1: sides.p1() = sideEntity; break;
    case PlayerSideId::P2: sides.p2() = sideEntity; break;

    default: POKESIM_REQUIRE_FAIL("sideID must be assigned P1 or P2."); break;
  }
}

void BattleStateSetup::setRNGSeed(std::optional<types::rngState> seed) {
  if (!seed.has_value()) {
    static std::atomic_uint64_t state = 1U;
    seed = state;

    types::rngState newState = state;
    internal::nextRandomValue(newState);
    state = newState;
  }
  handle.emplace<RngSeed>(seed.value());
}

void BattleStateSetup::setActionQueue(const std::vector<types::entity>& queue) {
  handle.emplace<ActionQueue>(queue);
}

void BattleStateSetup::setTurn(types::battleTurn turn) {
  handle.emplace<Turn>(turn);
}

void BattleStateSetup::setCurrentActionTarget(types::targets<types::entity> actionTargets) {
  handle.emplace<CurrentActionTargets>(actionTargets);
  for (types::entity entity : actionTargets) {
    handle.registry()->emplace<tags::CurrentActionMoveTarget>(entity);
  }
}

void BattleStateSetup::setCurrentActionSource(types::entity actionSource) {
  handle.emplace<CurrentActionSource>(actionSource);
  handle.registry()->emplace<tags::CurrentActionMoveSource>(actionSource);
}

void BattleStateSetup::setCurrentActionMove(types::entity actionMove) {
  handle.registry()->emplace<tags::CurrentActionMove>(actionMove);
  const auto* source = handle.try_get<CurrentActionSource>();
  const auto* targets = handle.try_get<CurrentActionTargets>();
  if (source) {
    handle.registry()->emplace<CurrentActionMovesAsSource>(source->val).val.push_back(actionMove);
  }
  if (targets) {
    for (types::entity target : targets->val) {
      handle.registry()->emplace<CurrentActionMovesAsTarget>(target).val.push_back(actionMove);
    }
  }
}

void BattleStateSetup::setProbability(types::probability probability) {
  handle.emplace<Probability>(probability);
}
}  // namespace pokesim

///////////////// END OF src/Battle/Setup/BattleStateSetup.cpp /////////////////

////////////// START OF src/Battle/Pokemon/ManagePokemonState.cpp //////////////

namespace pokesim {
namespace {
template <typename ItemTag>
struct RemoveItem {
  static void run(types::handle handle) { handle.remove<ItemTag>(); }
};

void removeItemTags(types::handle handle, ItemName item) {
  item::tags::enumToTag<RemoveItem>(item.name, handle);
}

template <typename SelectionTag>
void removeItemComponents(Simulation& simulation) {
  simulation.viewForSelectedPokemon<removeItemTags>();
  simulation.removeFromEntities<ItemName, SelectionTag>();
  simulation.registry.clear<SelectionTag>();
}

template <typename EffectiveStat, typename BoostType>
void applyBoostToEffectiveStat(EffectiveStat& effectiveStat, BoostType boost) {
  applyStatBoost(effectiveStat.val, boost.val);
}

template <typename BoostType, typename StatUpdateRequired>
void boostStat(types::registry& registry, CurrentEffectTarget target, BoostType& boost) {
  BoostType& currentBoost = registry.get_or_emplace<BoostType>(target.val);
  currentBoost.val += boost.val;

  if (boost.val) {
    registry.emplace_or_replace<StatUpdateRequired>(target.val);
  }
  if (currentBoost.val == MechanicConstants::PokemonStatBoost::BASE) {
    registry.remove<BoostType>(target.val);
  }
}

template <typename BoostType>
void clampBoost(types::registry& registry, CurrentEffectTarget target, BoostType& boost) {
  BoostType* currentBoost = registry.try_get<BoostType>(target.val);
  if (!currentBoost) {
    return;
  }

  using BoostLimits = MechanicConstants::PokemonStatBoost;

  types::boost combinedBoost = currentBoost->val + boost.val;
  if (combinedBoost > BoostLimits::MAX) {
    boost.val = BoostLimits::MAX - currentBoost->val;
  }
  else if (combinedBoost < BoostLimits::MIN) {
    boost.val = BoostLimits::MIN - currentBoost->val;
  }
}

template <typename BoostType, typename StatUpdateRequired>
void boost(Simulation& simulation) {
  simulation.view<boostStat<BoostType, StatUpdateRequired>>();
  runAfterEachBoostEvent<BoostType>(simulation);
}

template <typename Type>
void checkTypeStatusImmunity(types::handle handle, CurrentEffectTarget target) {
  if (handle.registry()->all_of<Type>(target.val)) {
    handle.remove<tags::CanSetStatus>();
  }
}

template <typename StatusType>
struct CheckIfStatusIsSettable {
  static void run(Simulation& simulation) {
    simulation.addToEntities<tags::CanSetStatus, StatusType, CurrentEffectSource, CurrentEffectTarget>();
    simulation.view<checkIfTargetHasStatus, Tags<StatusType>>();
    if constexpr (std::is_same_v<StatusType, status::tags::Burn>) {
      simulation.view<checkTypeStatusImmunity<type::tags::Fire>, Tags<StatusType>>();
    }
    if constexpr (std::is_same_v<StatusType, status::tags::Freeze>) {
      simulation.view<checkTypeStatusImmunity<type::tags::Ice>, Tags<StatusType>>();
    }
    if constexpr (std::is_same_v<StatusType, status::tags::Paralysis>) {  // And simulation is using a mechanic where
                                                                          // electric types cannot be paralyzed
      simulation.view<checkTypeStatusImmunity<type::tags::Electric>, Tags<StatusType>>();
    }

    if constexpr (std::is_same_v<StatusType, status::tags::Poison> || std::is_same_v<StatusType, status::tags::Toxic>) {
      simulation.view<checkTypeStatusImmunity<type::tags::Poison>, Tags<StatusType>>();
      simulation.view<checkTypeStatusImmunity<type::tags::Steel>, Tags<StatusType>>();
    }

    runStatusImmunityEvent<StatusType>(simulation);
  }

  static void checkIfTargetHasStatus(types::handle handle, CurrentEffectTarget target) {
    if (handle.registry()->all_of<StatusName>(target.val)) {
      handle.remove<tags::CanSetStatus>();
    }
  };
};

template <typename StatusType>
struct RemoveNotSettableStatus {
  static void run(Simulation& simulation) {
    simulation.removeFromEntities<StatusType, CurrentEffectSource, CurrentEffectTarget>(
      entt::exclude<tags::CanSetStatus>);
  }
};

template <typename StatusType>
void setStatus(types::registry& registry, CurrentEffectTarget target, dex::Status status) {
  registry.emplace<StatusName>(target.val, status);
  registry.emplace<StatusType>(target.val);
  if constexpr (std::is_same_v<StatusType, status::tags::Paralysis>) {
    registry.emplace<tags::SpeStatUpdateRequired>(target.val);
  }
}

void setSpeedSortNeeded(types::registry& registry, Battle battle) {
  registry.emplace_or_replace<simulate_turn::tags::SpeedSortNeeded>(battle.val);
}
}  // namespace

void checkIfCanUseItem(Simulation& simulation) {
  simulation.removeFromEntities<tags::CanUseItem>(entt::exclude<ItemName>);
}

void useItem(Simulation& simulation) {
  pokesim::internal::SelectForPokemonView<tags::CanUseItem> selectedPokemon{simulation};
  if (selectedPokemon.hasNoneSelected()) {
    return;
  }

  runAfterUseItemEvent(simulation);
  removeItemComponents<tags::CanUseItem>(simulation);
}

void tryUseItem(Simulation& simulation) {
  checkIfCanUseItem(simulation);
  useItem(simulation);
}

void checkIfCanRemoveItem(Simulation& simulation) {
  simulation.removeFromEntities<tags::CanRemoveItem>(entt::exclude<ItemName>);
  runTryTakeItemEvent(simulation);
}

void removeItem(Simulation& simulation) {
  pokesim::internal::SelectForPokemonView<tags::CanRemoveItem> selectedPokemon{simulation};
  if (selectedPokemon.hasNoneSelected()) {
    return;
  }

  runEndItemEvent(simulation);
  removeItemComponents<tags::CanRemoveItem>(simulation);
}

void tryRemoveItem(Simulation& simulation) {
  checkIfCanRemoveItem(simulation);
  removeItem(simulation);
}

void checkIfCanSetStatus(Simulation& simulation) {
  status::tags::forEach<CheckIfStatusIsSettable>(simulation);
}

void setStatus(Simulation& simulation) {
  status::tags::forEach<RemoveNotSettableStatus>(simulation);
  simulation.registry.clear<tags::CanSetStatus>();

  simulation.view<setStatus<status::tags::Burn>, Tags<status::tags::Burn>>(dex::Status::BRN);
  simulation.view<setStatus<status::tags::Freeze>, Tags<status::tags::Freeze>>(dex::Status::FRZ);
  simulation.view<setStatus<status::tags::Paralysis>, Tags<status::tags::Paralysis>>(dex::Status::PAR);
  simulation.view<setStatus<status::tags::Poison>, Tags<status::tags::Poison>>(dex::Status::PSN);
  simulation.view<setStatus<status::tags::Sleep>, Tags<status::tags::Sleep>>(dex::Status::SLP);
  simulation.view<setStatus<status::tags::Toxic>, Tags<status::tags::Toxic>>(dex::Status::TOX);

  runStartSleep(simulation);
  runStartFreeze(simulation);

  runAfterSetStatusEvent(simulation);
}

void trySetStatus(Simulation& simulation) {
  checkIfCanSetStatus(simulation);
  setStatus(simulation);
}

void clearStatus(types::handle pokemonHandle) {
  pokemonHandle.remove<
    StatusName,
    status::tags::Burn,
    status::tags::Freeze,
    status::tags::Paralysis,
    status::tags::Poison,
    status::tags::Sleep,
    status::tags::Toxic>();
}

void clearVolatiles(types::handle pokemonHandle) {
  pokemonHandle.remove<AtkBoost, DefBoost, SpaBoost, SpdBoost, SpeBoost>();
  pokemonHandle.remove<LastUsedMove>();

  // TODO(aed3): Make auto-generated
  pokemonHandle.remove<ChoiceLock>();
}

void deductPp(Pp& pp) {
  if (pp.val) {
    pp.val -= 1U;  // TODO(aed3): Make this into a mechanic constant
  }
}

void setLastMoveUsed(types::registry& registry, CurrentActionSource source, const CurrentActionMoveSlot& move) {
  registry.emplace<LastUsedMove>(source.val, move.val);
}

void resetEffectiveAtk(types::handle handle, stat::Atk atk) {
  handle.emplace_or_replace<stat::EffectiveAtk>(atk.val);
}

void resetEffectiveDef(types::handle handle, stat::Def def) {
  handle.emplace_or_replace<stat::EffectiveDef>(def.val);
}

void resetEffectiveSpa(types::handle handle, stat::Spa spa) {
  handle.emplace_or_replace<stat::EffectiveSpa>(spa.val);
}

void resetEffectiveSpd(types::handle handle, stat::Spd spd) {
  handle.emplace_or_replace<stat::EffectiveSpd>(spd.val);
}

void resetEffectiveSpe(types::handle handle, stat::Spe spe) {
  handle.emplace_or_replace<stat::EffectiveSpe>(spe.val);
}

void faint(types::handle pokemonHandle, Battle battle) {
  types::registry& registry = *pokemonHandle.registry();
  FaintQueue& faintQueue = registry.get_or_emplace<FaintQueue>(battle.val);
  faintQueue.val.push_back(pokemonHandle.entity());
}

void applyDamage(types::handle pokemonHandle, types::damage damage) {
  stat::CurrentHp& hp = pokemonHandle.get<stat::CurrentHp>();
  if (damage < hp.val) {
    hp.val -= damage;
  }
  else {
    hp.val = MechanicConstants::PokemonCurrentHpStat::MIN;
    faint(pokemonHandle, pokemonHandle.get<Battle>());
  }
}

void applyStatBoost(types::stat& stat, types::boost boost) {
  if (boost >= 0) {
    stat = types::stat(stat * MechanicConstants::STAT_BOOST_STAGES[boost]);
  }
  else {
    stat = types::stat(stat / MechanicConstants::STAT_BOOST_STAGES[-boost]);
  }
}

void tryBoost(Simulation& simulation) {
  runChangeBoostEvent(simulation);
  simulation.view<clampBoost<AtkBoost>>();
  simulation.view<clampBoost<DefBoost>>();
  simulation.view<clampBoost<SpaBoost>>();
  simulation.view<clampBoost<SpdBoost>>();
  simulation.view<clampBoost<SpeBoost>>();
  runTryBoostEvent(simulation);

  boost<AtkBoost, tags::AtkStatUpdateRequired>(simulation);
  boost<DefBoost, tags::DefStatUpdateRequired>(simulation);
  boost<SpaBoost, tags::SpaStatUpdateRequired>(simulation);
  boost<SpdBoost, tags::SpdStatUpdateRequired>(simulation);
  boost<SpeBoost, tags::SpeStatUpdateRequired>(simulation);
  runAfterBoostEvent(simulation);
}

void updateAllStats(Simulation& simulation) {
  updateAtk(simulation);
  updateDef(simulation);
  updateSpa(simulation);
  updateSpd(simulation);
  updateSpe(simulation);
}

void updateAtk(Simulation& simulation) {
  internal::SelectForPokemonView<tags::AtkStatUpdateRequired> selectedAtkUpdateRequired{simulation};
  if (selectedAtkUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveAtk>();

  simulation.viewForSelectedPokemon<applyBoostToEffectiveStat<stat::EffectiveAtk, AtkBoost>>();
  runModifyAtk(simulation);

  simulation.registry.clear<tags::AtkStatUpdateRequired>();
}

void updateDef(Simulation& simulation) {
  internal::SelectForPokemonView<tags::DefStatUpdateRequired> selectedDefUpdateRequired{simulation};
  if (selectedDefUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveDef>();

  simulation.viewForSelectedPokemon<applyBoostToEffectiveStat<stat::EffectiveDef, DefBoost>>();
  runModifyDef(simulation);

  simulation.registry.clear<tags::DefStatUpdateRequired>();
}

void updateSpa(Simulation& simulation) {
  internal::SelectForPokemonView<tags::SpaStatUpdateRequired> selectedSpaUpdateRequired{simulation};
  if (selectedSpaUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveSpa>();

  simulation.viewForSelectedPokemon<applyBoostToEffectiveStat<stat::EffectiveSpa, SpaBoost>>();
  runModifySpa(simulation);

  simulation.registry.clear<tags::SpaStatUpdateRequired>();
}

void updateSpd(Simulation& simulation) {
  internal::SelectForPokemonView<tags::SpdStatUpdateRequired> selectedSpdUpdateRequired{simulation};
  if (selectedSpdUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveSpd>();

  simulation.viewForSelectedPokemon<applyBoostToEffectiveStat<stat::EffectiveSpd, SpdBoost>>();
  runModifySpd(simulation);

  simulation.registry.clear<tags::SpdStatUpdateRequired>();
}

void updateSpe(Simulation& simulation) {
  internal::SelectForPokemonView<tags::SpeStatUpdateRequired> selectedSpeUpdateRequired{simulation};
  if (selectedSpeUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveSpe>();

  simulation.viewForSelectedPokemon<applyBoostToEffectiveStat<stat::EffectiveSpe, SpeBoost>>();
  runModifySpe(simulation);
  // trick room

  simulation.viewForSelectedPokemon<setSpeedSortNeeded, Tags<tags::SimulateTurn>>();
  simulation.registry.clear<tags::SpeStatUpdateRequired>();
}
}  // namespace pokesim

/////////////// END OF src/Battle/Pokemon/ManagePokemonState.cpp ///////////////

////////////////// START OF src/Battle/ManageBattleState.cpp ///////////////////

namespace pokesim {
void assignRootBattle(types::handle battleHandle) {
  const ParentBattle* parentBattle = battleHandle.try_get<ParentBattle>();
  types::entity rootBattle = parentBattle == nullptr ? battleHandle.entity() : parentBattle->val;
  battleHandle.emplace<RootBattle>(rootBattle);
}

void collectTurnOutcomeBattles(types::handle leafBattleHandle, const RootBattle& root) {
  leafBattleHandle.registry()->get_or_emplace<simulate_turn::TurnOutcomeBattles>(root.val).val.push_back(
    leafBattleHandle.entity());
}

void setCurrentActionSource(types::handle battleHandle, const Sides& sides, CurrentAction action) {
  types::registry& registry = *battleHandle.registry();
  const SourceSlotName& sourceSlotName = registry.get<SourceSlotName>(action.val);
  types::entity sourceEntity = slotToPokemonEntity(registry, sides, sourceSlotName.name);

  battleHandle.emplace<CurrentActionSource>(sourceEntity);
  registry.emplace<tags::CurrentActionMoveSource>(sourceEntity);
}

void setCurrentActionTarget(
  types::handle battleHandle, const Sides& sides, CurrentAction action, CurrentActionSource source) {
  types::registry& registry = *battleHandle.registry();
  const TargetSlotName& targetSlotName = registry.get<TargetSlotName>(action.val);
  types::entity targetEntity = slotToPokemonEntity(registry, sides, targetSlotName.name);

  if (!registry.any_of<tags::Fainted>(targetEntity)) {
    battleHandle.emplace<CurrentActionTargets>(types::targets<types::entity>{targetEntity});
    registry.emplace<tags::CurrentActionMoveTarget>(targetEntity);
    registry.emplace<CurrentActionSource>(targetEntity, source);
  }
  else {
    // Set tag to get random target after this function exits
  }
}

void setCurrentActionMove(
  types::handle battleHandle, CurrentActionSource source, const CurrentActionTargets& targets, CurrentAction action,
  const Pokedex& pokedex) {
  types::registry& registry = *battleHandle.registry();
  const MoveName& move = registry.get<MoveName>(action.val);
  const MoveSlots& moveSlots = registry.get<MoveSlots>(source.val);

  for (types::entity target : targets.val) {
    createActionMoveForTarget({registry, target}, battleHandle.entity(), source.val, move.name, pokedex);
  }

  types::entity moveSlotEntity = moveToEntity(registry, moveSlots, move.name);
  battleHandle.emplace<CurrentActionMoveSlot>(moveSlotEntity);
  registry.emplace<tags::CurrentActionMoveSlot>(moveSlotEntity);
}

void setFailedActionMove(types::handle moveHandle, Battle battle) {
  moveHandle.remove<tags::CurrentActionMove>();
  moveHandle.emplace<tags::FailedCurrentActionMove>();

  types::registry& registry = *moveHandle.registry();

  types::entity moveSlotEntity = registry.get<CurrentActionMoveSlot>(battle.val).val;
  registry.erase<CurrentActionMoveSlot>(battle.val);
  registry.erase<tags::CurrentActionMoveSlot>(moveSlotEntity);
}

void clearCurrentAction(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  registry.clear<CurrentAction>();
  registry.clear<CurrentActionTargets>();
  registry.clear<CurrentActionTarget>();
  registry.clear<CurrentActionSource>();
  registry.clear<CurrentActionMovesAsSource>();
  registry.clear<CurrentActionMovesAsTarget>();
  registry.clear<CurrentActionMoveSlot>();

  registry.clear<tags::CurrentActionMoveTarget>();
  registry.clear<tags::CurrentActionMoveSource>();
  registry.clear<tags::CurrentActionMoveSlot>();

  auto actionMoves = registry.view<tags::CurrentActionMove>();
  registry.destroy(actionMoves.begin(), actionMoves.end());
  auto failedActionMoves = registry.view<tags::FailedCurrentActionMove>();
  registry.destroy(failedActionMoves.begin(), failedActionMoves.end());
  auto currentActions = registry.view<action::tags::Current>();
  registry.destroy(currentActions.begin(), currentActions.end());

  auto battles = simulation.selectedBattleEntities();
  registry.remove<
    action::tags::Item,
    ItemName,
    action::tags::Move,
    MoveName,
    action::tags::BeforeTurn,
    action::tags::Dynamax,
    action::tags::MegaEvolve,
    action::tags::MidTurnSwitchIn,
    action::tags::PostFoeFaintSwitchIn,
    action::tags::PreSwitchOut,
    action::tags::PreTurnSwitchIn,
    action::tags::PrimalRevert,
    action::tags::Residual,
    action::tags::RevivalBlessing,
    action::tags::Switch,
    action::tags::SwitchOut,
    action::tags::Terastallize>(battles.begin(), battles.end());
}
}  // namespace pokesim

/////////////////// END OF src/Battle/ManageBattleState.cpp ////////////////////

/////////////////// START OF src/Battle/Helpers/Helpers.cpp ////////////////////

namespace pokesim {
types::entity slotToSideEntity(const Sides& sides, Slot targetSlot) {
  POKESIM_REQUIRE(targetSlot != Slot::NONE, "Can only get entity from valid target slot.");
  types::entity sideEntity = sides.val[((types::teamPositionIndex)targetSlot - 1U) % 2U];
  return sideEntity;
}

types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot targetSlot) {
  POKESIM_REQUIRE(targetSlot != Slot::NONE, "Can only get entity from valid target slot.");
  types::teamPositionIndex index = ((types::teamPositionIndex)targetSlot - 1U) / 2U;

  const Team& team = registry.get<Team>(sideEntity);
  POKESIM_REQUIRE(team.val.size() > index, "Choosing a target slot for team member that does not exist.");
  return team.val[index];
}

types::entity slotToPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot) {
  POKESIM_REQUIRE(targetSlot != Slot::NONE, "Can only get entity from valid target slot.");
  return slotToPokemonEntity(registry, slotToSideEntity(sides, targetSlot), targetSlot);
}

types::entity slotToAllyPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot) {
  POKESIM_REQUIRE(targetSlot != Slot::NONE, "Can only get entity from valid target slot.");
  Slot allySlot = Slot::NONE;
  types::teamPositionIndex index = 0U;

  switch (targetSlot) {
    case Slot::P1A: {
      allySlot = Slot::P1B;
      index = 1U;
      break;
    }
    case Slot::P1B:
    case Slot::P1C:
    case Slot::P1D:
    case Slot::P1E:
    case Slot::P1F: {
      allySlot = Slot::P1A;
      break;
    }
    case Slot::P2A: {
      allySlot = Slot::P2B;
      index = 1U;
      break;
    }
    case Slot::P2B:
    case Slot::P2C:
    case Slot::P2D:
    case Slot::P2E:
    case Slot::P2F: {
      allySlot = Slot::P2A;
      break;
    }
    default: break;
  }

  types::entity sideEntity = slotToSideEntity(sides, allySlot);
  const Team& team = registry.get<Team>(sideEntity);
  if (index >= team.val.size()) {
    return entt::null;
  }

  types::entity allyEntity = team.val[index];
  if (registry.any_of<tags::Fainted>(allyEntity)) {
    return entt::null;
  }

  return allyEntity;
}

types::entity moveToEntity(const types::registry& registry, const MoveSlots& moveSlots, dex::Move move) {
  for (types::entity moveSlot : moveSlots.val) {
    if (registry.get<MoveName>(moveSlot).name == move) {
      return moveSlot;
    }
  }

  POKESIM_REQUIRE_FAIL("No move of entity found.");
  return entt::null;
}

types::entity createActionMoveForTarget(
  types::handle targetHandle, types::entity battleEntity, types::entity sourceEntity, dex::Move move,
  const Pokedex& pokedex) {
  types::registry& registry = *targetHandle.registry();
  types::entity moveEntity = pokedex.buildActionMove(move, registry);

  registry.emplace<tags::CurrentActionMove>(moveEntity);
  registry.emplace<Battle>(moveEntity, battleEntity);
  registry.emplace<CurrentActionSource>(moveEntity, sourceEntity);
  registry.emplace<CurrentActionTarget>(moveEntity, targetHandle.entity());

  targetHandle.get_or_emplace<CurrentActionMovesAsTarget>().val.push_back(moveEntity);
  registry.get_or_emplace<CurrentActionMovesAsSource>(sourceEntity).val.push_back(moveEntity);

  if (registry.all_of<tags::SimulateTurn>(battleEntity)) {
    registry.emplace<tags::SimulateTurn>(moveEntity);
  }
  if (registry.all_of<tags::CalculateDamage>(battleEntity)) {
    registry.emplace<tags::CalculateDamage>(moveEntity);
  }
  if (registry.all_of<tags::AnalyzeEffect>(battleEntity)) {
    registry.emplace<tags::AnalyzeEffect>(moveEntity);
  }

  return moveEntity;
}
}  // namespace pokesim

//////////////////// END OF src/Battle/Helpers/Helpers.cpp /////////////////////

///////////////////// START OF src/Battle/Clone/Clone.cpp //////////////////////

namespace pokesim {
namespace {
void cloneEntity(
  types::entity src, types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  for (auto [id, storage] : registry.storage()) {
    if (storage.contains(src)) {
      POKESIM_REQUIRE(
        std::find(srcEntityStorages[id].begin(), srcEntityStorages[id].end(), src) == std::end(srcEntityStorages[id]),
        "Adding an entity twice here means an entity will be duplicated more than it should.");
      srcEntityStorages[id].push_back(src);
    }
  }

  auto& destinations = entityMap[src] = types::entityVector{cloneCount};
  registry.create(destinations.begin(), destinations.end());
}

template <typename VisitEntity = void*>
void traverseBattle(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  for (const auto [entity, sides, actionQueue] : registry.view<Tag, tags::Battle, Sides, ActionQueue>().each()) {
    for (auto side : sides.val) {
      registry.emplace<Tag>(side);
    }
    for (auto queueItem : actionQueue.val) {
      registry.emplace<Tag>(queueItem);
    }

    if constexpr (ForCloning) {
      visitEntity(entity);
    }
  }
  for (const auto [entity, currentAction] : registry.view<Tag, tags::Battle, CurrentAction>().each()) {
    registry.emplace<Tag>(currentAction.val);
  }
}

template <typename VisitEntity = void*>
void traverseSide(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  for (const auto [entity, team] : registry.view<Tag, tags::Side, Team>().each()) {
    for (auto pokemon : team.val) {
      registry.emplace<Tag>(pokemon);
    }

    if constexpr (ForCloning) {
      visitEntity(entity);
    }
  }
}

template <typename VisitEntity = void*>
void traverseAction(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  if constexpr (ForCloning) {
    for (types::entity entity : registry.view<Tag, SpeedSort>()) {
      visitEntity(entity);
    }
  }
}

template <typename VisitEntity = void*>
void traverseCurrentActionMove(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  if constexpr (ForCloning) {
    for (types::entity entity : registry.view<Tag, tags::CurrentActionMove>()) {
      visitEntity(entity);
    }
  }
}

template <typename VisitEntity = void*>
void traversePokemon(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  for (const auto [entity, moveSlots] : registry.view<Tag, tags::Pokemon, MoveSlots>().each()) {
    for (auto move : moveSlots.val) {
      registry.emplace<Tag>(move);
    }

    if constexpr (ForCloning) {
      visitEntity(entity);
    }
  }

  for (const auto [entity, moves] : registry.view<Tag, CurrentActionMovesAsSource>().each()) {
    for (types::entity move : moves.val) {
      registry.emplace_or_replace<Tag>(move);
    }
  }
  for (const auto [entity, moves] : registry.view<Tag, CurrentActionMovesAsTarget>().each()) {
    for (types::entity move : moves.val) {
      registry.emplace_or_replace<Tag>(move);
    }
  }
}

template <typename VisitEntity = void*>
void traverseMove(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  if constexpr (ForCloning) {
    for (types::entity entity : registry.view<Tag, MoveName, Pp>()) {
      visitEntity(entity);
    }
  }
}

void cloneBattle(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  traverseBattle(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void cloneSide(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  traverseSide(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void cloneAction(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  traverseAction(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void cloneCurrentActionMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  traverseCurrentActionMove(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void clonePokemon(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  traversePokemon(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void cloneMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  traverseMove(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void deleteBattle(types::registry& registry) {
  traverseBattle(registry);
}

void deleteSide(types::registry& registry) {
  traverseSide(registry);
}

void deleteAction(types::registry& registry) {
  traverseAction(registry);
}

void deleteCurrentActionMove(types::registry& registry) {
  traverseCurrentActionMove(registry);
}

void deletePokemon(types::registry& registry) {
  traversePokemon(registry);
}

void deleteMove(types::registry& registry) {
  traverseMove(registry);
}

void remapEntity(types::entity& entity, const CloneTo& cloneTo, const types::ClonedEntityMap& entityMap) {
  POKESIM_REQUIRE(entityMap.contains(entity), "Source node was not loaded into the map.");
  POKESIM_REQUIRE(
    entityMap.at(entity).size() > cloneTo.val,
    "More entities are trying to be copied to than were copied.");
  entity = entityMap.at(entity)[cloneTo.val];
}

template <typename Component>
void remapEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  for (auto [clonedEntity, cloneTo, component] : registry.view<CloneTo, Component>().each()) {
    remapEntity(component.val, cloneTo, entityMap);
  }
}

template <typename Component>
void remapEntityListMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  for (auto [clonedEntity, cloneTo, component] : registry.view<CloneTo, Component>().each()) {
    for (types::entity& entity : component.val) {
      remapEntity(entity, cloneTo, entityMap);
    }
  }
}

template <typename Component>
void remapComponentEntities(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  if constexpr (std::is_same_v<decltype(Component::val), types::entity>) {
    remapEntityMembers<Component>(registry, entityMap);
  }
  else {
    remapEntityListMembers<Component>(registry, entityMap);
  }
}
}  // namespace

types::ClonedEntityMap clone(types::registry& registry, std::optional<types::entityIndex> cloneCount) {
  types::entityIndex count = cloneCount.value_or(1U);
  types::ClonedEntityMap entityMap, battleMap;
  if (count == 0U) {
    return entityMap;
  }

  entt::dense_map<entt::id_type, types::entityVector> srcEntityStorages;

  cloneBattle(registry, entityMap, srcEntityStorages, count);
  battleMap = entityMap;
  cloneSide(registry, entityMap, srcEntityStorages, count);
  cloneAction(registry, entityMap, srcEntityStorages, count);
  clonePokemon(registry, entityMap, srcEntityStorages, count);
  cloneCurrentActionMove(registry, entityMap, srcEntityStorages, count);
  cloneMove(registry, entityMap, srcEntityStorages, count);

  for (auto [id, storage] : registry.storage()) {
    if (srcEntityStorages.contains(id)) {
      const auto& sources = srcEntityStorages.at(id);
      storage.reserve(storage.size() + (sources.size() * count));
      for (types::entity src : sources) {
        auto* value = storage.value(src);
        for (types::entityIndex i = 0U; i < count; i++) {
          storage.push(entityMap[src][i], value);
        }
      }
    }
  }

  auto& cloneToStorage = registry.storage<CloneTo>();
  cloneToStorage.reserve(entityMap.size() * count);

  for (const auto& [src, destinations] : entityMap) {
    cloneToStorage.insert(destinations.begin(), destinations.end());
  }

  for (const auto& [src, destinations] : entityMap) {
    for (types::entityIndex i = 0U; i < count; i++) {
      cloneToStorage.get(destinations[i]).val = i;
    }
  }

  // Not simplified further to a, for example, packed template type list, to make debugging what type went wrong easier
  remapComponentEntities<ActionQueue>(registry, entityMap);
  remapComponentEntities<Battle>(registry, entityMap);
  remapComponentEntities<ChoiceLock>(registry, entityMap);
  remapComponentEntities<CurrentAction>(registry, entityMap);
  remapComponentEntities<CurrentActionMovesAsSource>(registry, entityMap);
  remapComponentEntities<CurrentActionMovesAsTarget>(registry, entityMap);
  remapComponentEntities<CurrentActionMoveSlot>(registry, entityMap);
  remapComponentEntities<CurrentActionSource>(registry, entityMap);
  remapComponentEntities<CurrentActionTarget>(registry, entityMap);
  remapComponentEntities<CurrentActionTargets>(registry, entityMap);
  remapComponentEntities<CurrentEffectsAsSource>(registry, entityMap);
  remapComponentEntities<CurrentEffectsAsTarget>(registry, entityMap);
  remapComponentEntities<CurrentEffectSource>(registry, entityMap);
  remapComponentEntities<CurrentEffectTarget>(registry, entityMap);
  remapComponentEntities<FaintQueue>(registry, entityMap);
  remapComponentEntities<FoeSide>(registry, entityMap);
  remapComponentEntities<LastUsedMove>(registry, entityMap);
  remapComponentEntities<MoveSlots>(registry, entityMap);
  remapComponentEntities<NextAction>(registry, entityMap);
  remapComponentEntities<Pokemon>(registry, entityMap);
  remapComponentEntities<Side>(registry, entityMap);
  remapComponentEntities<Sides>(registry, entityMap);
  remapComponentEntities<Team>(registry, entityMap);

  registry.clear<CloneTo, tags::CloneFrom>();

  for (const auto& [originalBattle, clonedBattles] : battleMap) {
    registry.remove<ParentBattle>(clonedBattles.begin(), clonedBattles.end());
    registry.insert<ParentBattle>(clonedBattles.begin(), clonedBattles.end(), {originalBattle});
  }

#ifdef POKESIM_DEBUG_CHECK_UTILITIES
  for (const auto& [src, destinations] : entityMap) {
    registry.remove<ParentEntity>(destinations.begin(), destinations.end());
    registry.insert<ParentEntity>(destinations.begin(), destinations.end(), {src});
  }
#endif

  return entityMap;
}

void deleteClones(types::registry& registry) {
  deleteBattle(registry);
  deleteSide(registry);
  deleteAction(registry);
  deletePokemon(registry);
  deleteCurrentActionMove(registry);
  deleteMove(registry);
  auto remove = registry.view<tags::CloneToRemove>();
  registry.destroy(remove.begin(), remove.end());
}
}  // namespace pokesim

////////////////////// END OF src/Battle/Clone/Clone.cpp ///////////////////////

///////// START OF src/AnalyzeEffect/Setup/AnalyzeEffectInputSetup.cpp /////////

namespace pokesim::analyze_effect {
InputSetup::InputSetup(types::registry& registry, types::entity entity) : handle(registry, entity) {
  handle.emplace<tags::Input>();
}

void InputSetup::setAttacker(types::entity entity) {
  handle.emplace<Attacker>(entity);
}

void InputSetup::setEffectTarget(types::entity entity) {
  handle.emplace<EffectTarget>(entity);
}

void InputSetup::setDefender(types::entity entity) {
  handle.emplace<Defender>(entity);
}

void InputSetup::setEffectMove(dex::Move move) {
  handle.emplace<EffectMove>(move);
}

void InputSetup::setEffect(types::effectEnum effect) {
  if (effect.holds<dex::PseudoWeather>()) {
    handle.emplace<PseudoWeatherName>(effect.get<dex::PseudoWeather>());
  }
  else if (effect.holds<dex::SideCondition>()) {
    handle.emplace<SideConditionName>(effect.get<dex::SideCondition>());
  }
  else if (effect.holds<dex::Status>()) {
    handle.emplace<StatusName>(effect.get<dex::Status>());
  }
  else if (effect.holds<dex::Terrain>()) {
    handle.emplace<TerrainName>(effect.get<dex::Terrain>());
  }
  else if (effect.holds<dex::Volatile>()) {
    handle.emplace<VolatileName>(effect.get<dex::Volatile>());
  }
  else if (effect.holds<dex::Weather>()) {
    handle.emplace<WeatherName>(effect.get<dex::Weather>());
  }
  else {
    POKESIM_REQUIRE_FAIL("Effect does not contain a valid enum.");
  }
}

void InputSetup::setBoostEffect(dex::Stat stat, types::boost boost) {
  switch (stat) {
    case dex::Stat::ATK: {
      handle.emplace<AtkBoost>(boost);
      break;
    }
    case dex::Stat::DEF: {
      handle.emplace<DefBoost>(boost);
      break;
    }
    case dex::Stat::SPA: {
      handle.emplace<SpaBoost>(boost);
      break;
    }
    case dex::Stat::SPD: {
      handle.emplace<SpdBoost>(boost);
      break;
    }
    case dex::Stat::SPE: {
      handle.emplace<SpeBoost>(boost);
      break;
    }
    default: {
      POKESIM_REQUIRE_FAIL("Using a stat enum that doesn't have boost.");
    }
  }
}

void InputSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
  handle.registry()->get_or_emplace<Inputs>(entity).val.push_back(handle.entity());
}
}  // namespace pokesim::analyze_effect

////////// END OF src/AnalyzeEffect/Setup/AnalyzeEffectInputSetup.cpp //////////

///////////////// START OF src/AnalyzeEffect/AnalyzeEffect.cpp /////////////////

namespace pokesim::analyze_effect {
namespace {
enum class EffectPresentCheck : std::uint8_t {
  NOT_PRESENT,
  PRESENT_AND_NOT_APPLIED,
  PRESENT_AND_APPLIED,
};

template <typename BattleEffectType>
EffectPresentCheck hasBattleEffect(types::registry& registry, Battle battle, BattleEffectType battleEffect) {
  BattleEffectType* currentEffect = registry.try_get<BattleEffectType>(battle.val);
  if (!currentEffect) {
    return EffectPresentCheck::NOT_PRESENT;
  }

  if (currentEffect->name == battleEffect.name) {
    return EffectPresentCheck::PRESENT_AND_APPLIED;
  }

  return EffectPresentCheck::PRESENT_AND_NOT_APPLIED;
}

EffectPresentCheck hasSideEffect(types::registry&, EffectTarget, dex::SideCondition) {
  return EffectPresentCheck::NOT_PRESENT;
}

EffectPresentCheck hasVolatileEffect(types::registry&, EffectTarget, dex::Volatile) {
  return EffectPresentCheck::NOT_PRESENT;
}

EffectPresentCheck hasStatusEffect(types::registry& registry, EffectTarget effectTarget, StatusName status) {
  StatusName* currentStatus = registry.try_get<StatusName>(effectTarget.val);
  if (!currentStatus) {
    return EffectPresentCheck::NOT_PRESENT;
  }

  if (currentStatus->name == status.name) {
    return EffectPresentCheck::PRESENT_AND_APPLIED;
  }

  return EffectPresentCheck::PRESENT_AND_NOT_APPLIED;
}

template <typename BoostType>
EffectPresentCheck hasBoostEffect(types::registry& registry, EffectTarget effectTarget, BoostType boost) {
  BoostType* currentBoost = registry.try_get<BoostType>(effectTarget.val);
  if (!currentBoost) {
    return EffectPresentCheck::NOT_PRESENT;
  }

  if (currentBoost->val == boost.val) {
    return EffectPresentCheck::PRESENT_AND_APPLIED;
  }

  return EffectPresentCheck::PRESENT_AND_NOT_APPLIED;
}

template <typename T, typename EffectTuple>
bool namedEffectPointerMatch(const EffectTuple& current, const EffectTuple& other) {
  const T* currentProperty = std::get<const T*>(current);
  const T* otherProperty = std::get<const T*>(other);

  if ((currentProperty == nullptr) != (otherProperty == nullptr)) return false;
  if (currentProperty == nullptr) return true;
  return currentProperty->name == otherProperty->name;
}

template <typename T, typename EffectTuple>
bool valuedEffectPointerMatch(const EffectTuple& current, const EffectTuple& other) {
  const T* currentProperty = std::get<const T*>(current);
  const T* otherProperty = std::get<const T*>(other);

  if ((currentProperty == nullptr) != (otherProperty == nullptr)) return false;
  if (currentProperty == nullptr) return true;
  return currentProperty->val == otherProperty->val;
}

template <typename EffectTuple>
bool canInputsShareABattle(
  const EffectTuple& currentEffects, const EffectTarget& currentEffectTarget, types::entity otherInput,
  const types::registry& registry) {
  const auto otherEffects = tryGetAllInputEffects(otherInput, registry);

  if (!namedEffectPointerMatch<PseudoWeatherName>(currentEffects, otherEffects)) return false;
  if (!namedEffectPointerMatch<TerrainName>(currentEffects, otherEffects)) return false;
  if (!namedEffectPointerMatch<WeatherName>(currentEffects, otherEffects)) return false;

  EffectTarget otherEffectTarget = registry.get<EffectTarget>(otherInput);
  if (registry.get<Side>(currentEffectTarget.val).val != registry.get<Side>(otherEffectTarget.val).val) return false;
  if (!namedEffectPointerMatch<SideConditionName>(currentEffects, otherEffects)) return false;

  if (currentEffectTarget.val != otherEffectTarget.val) return false;
  if (!namedEffectPointerMatch<StatusName>(currentEffects, otherEffects)) return false;
  if (!namedEffectPointerMatch<VolatileName>(currentEffects, otherEffects)) return false;
  if (!valuedEffectPointerMatch<AtkBoost>(currentEffects, otherEffects)) return false;
  if (!valuedEffectPointerMatch<DefBoost>(currentEffects, otherEffects)) return false;
  if (!valuedEffectPointerMatch<SpaBoost>(currentEffects, otherEffects)) return false;
  if (!valuedEffectPointerMatch<SpdBoost>(currentEffects, otherEffects)) return false;
  if (!valuedEffectPointerMatch<SpeBoost>(currentEffects, otherEffects)) return false;

  return true;
}

void groupSimilarInputs(types::handle battleHandle, const Inputs& inputs) {
  types::registry& registry = *battleHandle.registry();
  types::entityIndex inputCount = inputs.val.size();
  std::vector<bool> checkedIndexes(inputCount, false);

  for (types::entityIndex currentIndex = 0U; currentIndex < inputCount; currentIndex++) {
    if (checkedIndexes[currentIndex]) continue;

    types::entity currentInput = inputs.val[currentIndex];
    GroupedInputs& groupedInputs = registry.emplace<GroupedInputs>(currentInput);

    const auto currentEffects = tryGetAllInputEffects(currentInput, registry);
    EffectTarget currentEffectTarget = registry.get<EffectTarget>(currentInput);

    groupedInputs.val.push_back(currentInput);

    for (types::entityIndex otherIndex = currentIndex + 1U; otherIndex < inputCount; otherIndex++) {
      if (checkedIndexes[otherIndex]) continue;

      types::entity otherInput = inputs.val[otherIndex];
      if (!canInputsShareABattle(currentEffects, currentEffectTarget, otherInput, registry)) continue;

      groupedInputs.val.push_back(otherInput);
      registry.emplace<tags::GroupedWithOtherInput>(otherInput);
      battleHandle.get_or_emplace<SkippedInputCount>().val++;

      checkedIndexes[otherIndex] = true;
    }

    checkedIndexes[currentIndex] = true;
  }
}

void setRunOneCalculation(types::handle inputHandle, Battle battle) {
  inputHandle.emplace<tags::RunOneCalculation>();
  inputHandle.registry()->get_or_emplace<SkippedInputCount>(battle.val).val++;
}

void setInvertFinalAnswer(types::handle inputHandle) {
  for (types::entity input : inputHandle.get<GroupedInputs>().val) {
    inputHandle.registry()->emplace<tags::InvertFinalAnswer>(input);
  }
}

template <typename BattleEffectType>
void ignoreWithBattleEffect(types::handle inputHandle, Battle battle, BattleEffectType battleEffect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck present = hasBattleEffect(registry, battle, battleEffect);
  if (present == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

void ignoreWithSideConditionEffect(
  types::handle inputHandle, Battle battle, EffectTarget effectTarget, SideConditionName effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck present = hasSideEffect(registry, effectTarget, effect.name);
  if (present == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

void ignoreWithStatusEffect(types::handle inputHandle, Battle battle, EffectTarget effectTarget, StatusName effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck present = hasStatusEffect(registry, effectTarget, effect);
  if (present == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

void ignoreWithVolatileEffect(
  types::handle inputHandle, Battle battle, EffectTarget effectTarget, VolatileName effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck present = hasVolatileEffect(registry, effectTarget, effect.name);
  if (present == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

template <typename BoostType>
void ignoreWithBoostEffect(types::handle inputHandle, Battle battle, EffectTarget effectTarget, BoostType effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck boostCheck = hasBoostEffect(registry, effectTarget, effect);
  if (boostCheck == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setRunOneCalculation(inputHandle, battle);
  }
}

void ignoreBattlesWithEffectActive(Simulation& simulation) {
  using Included = Tags<tags::Input>;
  using Excluded = entt::exclude_t<tags::GroupedWithOtherInput>;
  simulation.view<ignoreWithBattleEffect<PseudoWeatherName>, Included, Excluded>();
  simulation.view<ignoreWithBattleEffect<TerrainName>, Included, Excluded>();
  simulation.view<ignoreWithBattleEffect<WeatherName>, Included, Excluded>();
  simulation.view<ignoreWithSideConditionEffect, Included, Excluded>();
  simulation.view<ignoreWithStatusEffect, Included, Excluded>();
  simulation.view<ignoreWithVolatileEffect, Included, Excluded>();

  simulation.view<ignoreWithBoostEffect<AtkBoost>, Included, Excluded>();
  simulation.view<ignoreWithBoostEffect<DefBoost>, Included, Excluded>();
  simulation.view<ignoreWithBoostEffect<SpaBoost>, Included, Excluded>();
  simulation.view<ignoreWithBoostEffect<SpdBoost>, Included, Excluded>();
  simulation.view<ignoreWithBoostEffect<SpeBoost>, Included, Excluded>();
}

types::entity createAnalyzeEffectMove(
  types::registry& registry, dex::Move move, types::entity battleEntity, types::entity attackerEntity,
  types::entity defenderEntity, const Pokedex& pokedex) {
  types::entity moveEntity =
    createActionMoveForTarget({registry, defenderEntity}, battleEntity, attackerEntity, move, pokedex);
  registry.emplace<MoveName>(moveEntity, move);
  registry.emplace_or_replace<tags::Attacker>(attackerEntity);
  registry.emplace_or_replace<tags::Defender>(defenderEntity);

  return moveEntity;
}

void createOneCalculationMovePair(
  types::handle inputHandle, Battle battle, EffectMove move, Attacker attacker, Defender defender,
  const Pokedex& pokedex) {
  entt::entity moveEntity =
    createAnalyzeEffectMove(*inputHandle.registry(), move.val, battle.val, attacker.val, defender.val, pokedex);
  inputHandle.emplace<MovePair>(moveEntity, moveEntity);
}

void createTwoCalculationsMovePair(
  types::handle inputHandle, Battle battle, EffectMove move, Attacker attacker, Defender defender,
  const OriginalInputEntities& originals, const Pokedex& pokedex) {
  types::registry& registry = *inputHandle.registry();

  entt::entity originalEntity =
    createAnalyzeEffectMove(registry, move.val, originals.battle, originals.attacker, originals.defender, pokedex);
  entt::entity copyEntity =
    createAnalyzeEffectMove(registry, move.val, battle.val, attacker.val, defender.val, pokedex);

  // All active pokemon in should have their stats refreshed in doubles for moves like Beat Up which rely on the stats
  // of Pokemon outside of the attacker and defender
  for (types::entity pokemon : {attacker.val, defender.val}) {
    registry.emplace_or_replace<pokesim::tags::AtkStatUpdateRequired>(pokemon);
    registry.emplace_or_replace<pokesim::tags::DefStatUpdateRequired>(pokemon);
    registry.emplace_or_replace<pokesim::tags::SpaStatUpdateRequired>(pokemon);
    registry.emplace_or_replace<pokesim::tags::SpdStatUpdateRequired>(pokemon);
    registry.emplace_or_replace<pokesim::tags::SpeStatUpdateRequired>(pokemon);
  }

  inputHandle.emplace<MovePair>(originalEntity, copyEntity);
}

void assignInputsToClones(
  Simulation& simulation, types::entityIndex cloneCount, types::entity originalBattleEntity,
  const types::ClonedEntityMap& clonedEntityMap) {
  types::registry& registry = simulation.registry;
  const Inputs& inputs = registry.get<Inputs>(originalBattleEntity);

  POKESIM_REQUIRE(
    (cloneCount == 0U) == clonedEntityMap.empty(),
    "There should be no cloned entities if no clones are needed.");

  POKESIM_REQUIRE(
    cloneCount <= inputs.val.size(),
    "Not all inputs need clones, so there shouldn't be more clones than inputs.");

  const auto& battleClones = cloneCount == 0U ? types::entityVector{} : clonedEntityMap.at(originalBattleEntity);

  POKESIM_REQUIRE(
    cloneCount == battleClones.size(),
    "Each input must have a clone and no more clones than inputs should be made.");

  types::entityIndex cloneIndex = 0U;
  for (types::entity input : inputs.val) {
    if (!registry.all_of<GroupedInputs>(input)) {
      continue;
    }

    if (registry.all_of<tags::RunOneCalculation>(input)) {
      continue;
    }

    POKESIM_REQUIRE(battleClones.size() > cloneIndex, "More inputs want clones than clones made.");
    registry.emplace<tags::BattleCloneForCalculation>(battleClones[cloneIndex]);

    const GroupedInputs& groupedInputs = registry.get<GroupedInputs>(input);
    for (types::entity originalInput : groupedInputs.val) {
      const auto& [battle, move, attacker, defender, effectTarget] =
        registry.get<Battle, EffectMove, Attacker, Defender, EffectTarget>(originalInput);

      registry.emplace<OriginalInputEntities>(
        originalInput,
        OriginalInputEntities{battle.val, attacker.val, defender.val, effectTarget.val});

      const auto& clonedAttackers = clonedEntityMap.at(attacker.val);
      POKESIM_REQUIRE(
        battleClones.size() == clonedAttackers.size(),
        "Each attacker must have a clone and no more clones than inputs should be made.");

      const auto& clonedDefenders = clonedEntityMap.at(defender.val);
      POKESIM_REQUIRE(
        battleClones.size() == clonedDefenders.size(),
        "Each defender must have a clone and no more clones than inputs should be made.");

      const auto& clonedEffectTarget = clonedEntityMap.at(effectTarget.val);
      POKESIM_REQUIRE(
        battleClones.size() == clonedEffectTarget.size(),
        "Each effect target must have a clone and no more clones than inputs should be made.");

      battle.val = battleClones[cloneIndex];
      attacker.val = clonedAttackers[cloneIndex];
      defender.val = clonedDefenders[cloneIndex];
      effectTarget.val = clonedEffectTarget[cloneIndex];
    }

    cloneIndex++;
  }
}

void createAppliedEffectBattles(Simulation& simulation) {
  entt::dense_map<types::entityIndex, types::entityVector> battlesByCloneCount{};

  simulation.registry.view<Inputs>(entt::exclude<SkippedInputCount>)
    .each([&](types::entity battleEntity, const Inputs& inputs) {
      POKESIM_REQUIRE(!inputs.val.empty(), "Battles with input components should have input entities.");
      types::entityIndex cloneCount = inputs.val.size();

      battlesByCloneCount[cloneCount].push_back(battleEntity);
    });

  simulation.registry.view<Inputs, SkippedInputCount>().each(
    [&](types::entity battleEntity, const Inputs& inputs, SkippedInputCount skippedInputCount) {
      POKESIM_REQUIRE(!inputs.val.empty(), "Battles with input components should have input entities.");
      types::entityIndex cloneCount = inputs.val.size();

      POKESIM_REQUIRE(cloneCount >= skippedInputCount.val, "Must have more inputs than inputs ignored.");
      battlesByCloneCount[cloneCount - skippedInputCount.val].push_back(battleEntity);
    });

  for (const auto& [cloneCount, battleEntities] : battlesByCloneCount) {
    if (cloneCount) {
      simulation.registry.insert<pokesim::tags::CloneFrom>(battleEntities.begin(), battleEntities.end());
    }

    auto clonedEntityMap = clone(simulation.registry, cloneCount);

    for (types::entity battleEntity : battleEntities) {
      assignInputsToClones(simulation, cloneCount, battleEntity, clonedEntityMap);
    }
  }

  simulation.view<createOneCalculationMovePair, Tags<>, entt::exclude_t<OriginalInputEntities>>(simulation.pokedex());
  simulation.view<createTwoCalculationsMovePair>(simulation.pokedex());
}

void applyPseudoWeatherEffect(types::handle, Battle, PseudoWeatherName) {}

void applyTerrainEffect(types::handle, Battle, TerrainName) {}

void applyWeatherEffect(types::handle, Battle, WeatherName) {}

void applySideConditionEffect(types::handle, EffectTarget, SideConditionName) {}

void applyStatusEffect(types::handle inputHandle, EffectTarget effectTarget, StatusName effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck statusCheck = hasStatusEffect(registry, effectTarget, effect);
  types::handle pokemonHandle{registry, effectTarget.val};
  clearStatus(pokemonHandle);
  if (statusCheck == EffectPresentCheck::PRESENT_AND_APPLIED) {
    setInvertFinalAnswer(inputHandle);
  }
  else {
    pokemonHandle.emplace<StatusName>(effect.name);
    status::tags::emplaceTagFromEnum(effect.name, pokemonHandle);
  }
}

void applyVolatileEffect(types::handle, EffectTarget, VolatileName) {}

template <typename BoostType>
void applyBoostEffect(types::handle inputHandle, EffectTarget effectTarget, BoostType effect) {
  types::registry& registry = *inputHandle.registry();

  EffectPresentCheck boostCheck = hasBoostEffect(registry, effectTarget, effect);
  if (boostCheck == EffectPresentCheck::PRESENT_AND_APPLIED) {
    registry.remove<BoostType>(effectTarget.val);
    setInvertFinalAnswer(inputHandle);
  }
  else {
    registry.emplace_or_replace<BoostType>(effectTarget.val, effect);
  }
}

void createOutput(types::handle inputHandle, const MovePair& movePairs) {
  bool invert = inputHandle.all_of<tags::InvertFinalAnswer>();
  types::registry& registry = *inputHandle.registry();

  auto [parentBattleMove, childBattleMove] = movePairs;
  const auto [childDamage, childDamageRolls] = registry.get<Damage, DamageRolls>(childBattleMove);
  auto [parentDamage, parentDamageRolls] = registry.get<Damage, DamageRolls>(parentBattleMove);

  if (invert) {
    if (childDamage.val == 0U) {
      inputHandle.emplace<tags::InfiniteMultiplier>();
    }
    else {
      inputHandle.emplace<EffectMultiplier>((types::effectMultiplier)parentDamage.val / childDamage.val);
    }

    inputHandle.emplace<MultipliedDamageRolls>(parentDamageRolls);
    auto* const parentKoChances = registry.try_get<calc_damage::UsesUntilKo>(parentBattleMove);
    if (parentKoChances != nullptr) {
      inputHandle.emplace<MultipliedUsesUntilKo>(*parentKoChances);
    }
  }
  else {
    if (parentDamage.val == 0U) {
      inputHandle.emplace<tags::InfiniteMultiplier>();
    }
    else {
      inputHandle.emplace<EffectMultiplier>((types::effectMultiplier)childDamage.val / parentDamage.val);
    }

    inputHandle.emplace<MultipliedDamageRolls>(childDamageRolls);
    auto* const childKoChances = registry.try_get<calc_damage::UsesUntilKo>(childBattleMove);
    if (childKoChances != nullptr) {
      inputHandle.emplace<MultipliedUsesUntilKo>(*childKoChances);
    }
  }
}

void restoreInputs(
  const OriginalInputEntities& originalEntities, Battle& battle, Attacker& attacker, Defender& defender,
  EffectTarget& effectTarget) {
  battle.val = originalEntities.battle;
  attacker.val = originalEntities.attacker;
  defender.val = originalEntities.defender;
  effectTarget.val = originalEntities.effectTarget;
}

template <typename UsedMovesType>
void removeUsedMoves(types::registry& registry, const MovePair& movePair, types::entity pokemon) {
  UsedMovesType* moves = registry.try_get<UsedMovesType>(pokemon);

  if (moves == nullptr) {
    return;
  }

  auto end = moves->val.end();
  end = std::remove_if(moves->val.begin(), end, [&movePair](types::entity entity) {
    return entity == movePair.original || entity == movePair.copy;
  });

  moves->val.resize(end - moves->val.begin());
  if (moves->val.empty()) {
    registry.remove<UsedMovesType>(pokemon);
    if constexpr (std::is_same_v<UsedMovesType, UsedMovesAsAttacker>) {
      registry.remove<tags::Attacker>(pokemon);
    }
    else {
      registry.remove<tags::Defender>(pokemon);
    }
  }
}

void removeMovePairs(types::handle inputHandle, MovePair& movePair, Attacker& attacker, Defender& defender) {
  types::registry& registry = *inputHandle.registry();
  removeUsedMoves<UsedMovesAsAttacker>(registry, movePair, attacker.val);
  removeUsedMoves<UsedMovesAsDefender>(registry, movePair, defender.val);

  const OriginalInputEntities* original = inputHandle.try_get<OriginalInputEntities>();
  if (original) {
    removeUsedMoves<UsedMovesAsAttacker>(registry, movePair, original->attacker);
    removeUsedMoves<UsedMovesAsDefender>(registry, movePair, original->defender);
  }

  registry.destroy(movePair.original);
  if (movePair.original != movePair.copy) {
    registry.destroy(movePair.copy);
  }

  movePair.original = movePair.copy = entt::null;
}

void clearRunVariables(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  simulation.view<removeMovePairs>();
  simulation.view<restoreInputs>();

  simulation.addToEntities<pokesim::tags::CloneToRemove, tags::BattleCloneForCalculation>();
  deleteClones(registry);

  registry.clear<
    GroupedInputs,
    MovePair,
    OriginalInputEntities,
    tags::RunOneCalculation,
    tags::InvertFinalAnswer,
    tags::GroupedWithOtherInput,
    SkippedInputCount,
    Damage>();
}

void analyzeEffect(Simulation& simulation) {
  pokesim::internal::SelectForPokemonView<pokesim::tags::AnalyzeEffect> selectedPokemon(simulation);
  pokesim::internal::SelectForBattleView<pokesim::tags::AnalyzeEffect> selectedBattle(simulation);

  if (selectedPokemon.hasNoneSelected() || selectedBattle.hasNoneSelected()) {
    return;
  }

  simulation.viewForSelectedBattles<groupSimilarInputs>();

  if (!simulation.analyzeEffectOptions.reconsiderActiveEffects) {
    ignoreBattlesWithEffectActive(simulation);
  }

  createAppliedEffectBattles(simulation);

  using Included = Tags<tags::Input>;
  using Excluded = entt::exclude_t<tags::RunOneCalculation, tags::GroupedWithOtherInput>;
  simulation.view<applyPseudoWeatherEffect, Included, Excluded>();
  simulation.view<applyTerrainEffect, Included, Excluded>();
  simulation.view<applyWeatherEffect, Included, Excluded>();
  simulation.view<applySideConditionEffect, Included, Excluded>();
  simulation.view<applyStatusEffect, Included, Excluded>();
  simulation.view<applyVolatileEffect, Included, Excluded>();

  simulation.view<applyBoostEffect<AtkBoost>, Included, Excluded>();
  simulation.view<applyBoostEffect<DefBoost>, Included, Excluded>();
  simulation.view<applyBoostEffect<SpaBoost>, Included, Excluded>();
  simulation.view<applyBoostEffect<SpdBoost>, Included, Excluded>();
  simulation.view<applyBoostEffect<SpeBoost>, Included, Excluded>();

  updateAllStats(simulation);

  calc_damage::run(simulation);

  simulation.view<createOutput, Included>();
  clearRunVariables(simulation);
}
}  // namespace

void run(Simulation& simulation) {
  debug::Checks debugChecks(simulation);
  debugChecks.checkInputs();

  analyzeEffect(simulation);

  debugChecks.checkOutputs();
}
}  // namespace pokesim::analyze_effect

////////////////// END OF src/AnalyzeEffect/AnalyzeEffect.cpp //////////////////