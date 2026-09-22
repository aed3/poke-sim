#pragma once

#ifdef POKESIM_USE_COMBINED_SOURCE_AND_HEADER_FILES
#include "../extras/PokeSim.hpp"
#else
#include <PokeSim.hpp>
#endif

#include <Catch2/catch_amalgamated.hpp>
#include <entt/entt.hpp>

#ifndef POKESIM_ENABLE_TESTING
static_assert(false, "Rebuild PokeSim with the flag POKESIM_ENABLE_TESTING to enable testing.");
#endif

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
struct TestChecks : debug::Checks {
  using debug::Checks::checkOptions;
  using debug::Checks::copyEntity;
  using debug::Checks::getFinalEntityCount;
  using debug::Checks::getInitialEntity;
  using debug::Checks::has;
  using debug::Checks::initialEntityCount;

  void checkEntityForChanges(types::entity entity, const debug::TypesToIgnore& typesToIgnore) const {
    types::entity initialEntity = getInitialEntity(entity);
    debug::areEntitiesEqual(*registry, entity, registryOnInput, initialEntity, typesToIgnore);
  }

  template <typename Selector, typename... TypesToIgnore>
  void checkViewForChanges() const {
    for (types::entity entity : registry->view<Selector>()) {
      checkEntityForChanges<TypesToIgnore...>(entity);
    }
  }

  template <typename... TypesToIgnore>
  void checkEntityForChanges(types::entity entity) const {
    debug::TypesToIgnore typesToIgnore;
    typesToIgnore.add<TypesToIgnore...>();
    checkEntityForChanges(entity, typesToIgnore);
  }

  types::entity getParentEntity(types::entity battle) const {
    return debug::findCopyParent(currentEntitiesToInitial, *registry, battle);
  }

  template <typename... T>
  auto getInitialComponents(types::entity entity) const {
    return registryOnInput.get<T...>(getInitialEntity(entity));
  }

  template <typename Component>
  auto getInitialAndCurrent(types::entity entity) const {
    struct {
      Component initial;
      Component current;
    } initialAndCurrent;

    initialAndCurrent.initial = getInitialComponents<Component>(entity);
    initialAndCurrent.current = registry->get<Component>(entity);
    return initialAndCurrent;
  }

  template <typename... BattleTypesToIgnore>
  void checkPostSimulateTurnBattles(bool usesParentBattle) const {
    if (usesParentBattle) {
      checkViewForChanges<
        tags::Battle,
        Turn,
        simulate_turn::tags::BattleOutcome,
        simulate_turn::tags::SpeedSortNeeded,
        ParentBattle,
        RootBattle,
        BattleTypesToIgnore...>();
    }
    else {
      checkViewForChanges<
        tags::Battle,
        Turn,
        simulate_turn::tags::BattleOutcome,
        simulate_turn::tags::SpeedSortNeeded,
        BattleTypesToIgnore...>();
    }
  }

  template <typename... SideTypesToIgnore>
  void checkPostSimulateTurnSides() const {
    checkViewForChanges<tags::Side, SideDecision, SideTypesToIgnore...>();
  }

  void checkMovePpUsage(types::entity entity, types::moveSlotIndex moveSlotIndex = 0U) const {
    auto [initial, current] = getInitialAndCurrent<MoveSlots>(entity);
    auto initialMovePp = initial.val[moveSlotIndex].pp;
    auto movePp = current.val[moveSlotIndex].pp;
    REQUIRE(movePp == initialMovePp - Constants::PP_USE_DEDUCTION);
  }

  template <typename... ComponentsExpectedToChange>
  void checkUsedMovePokemon(types::entity pokemon, types::moveSlotIndex usedMoveSlot = 0U) const {
    checkEntityForChanges<LastUsedMove, MoveSlots, ComponentsExpectedToChange...>(pokemon);
    checkMovePpUsage(pokemon, usedMoveSlot);

    LastUsedMove lastUsedMove = registry->get<LastUsedMove>(pokemon);
    REQUIRE(lastUsedMove.val == usedMoveSlot);
  }

  void reset() {
    registryOnInput.clear();
    currentEntitiesToInitial.clear();
    specificallyChecked.clear();
    simulateTurnOptionsOnInput = simulation->simulateTurnOptions;
    calcDamageOptionsOnInput = simulation->calculateDamageOptions;
    analyzeEffectOptionsOnInput = simulation->analyzeEffectOptions;

    copyRemainingEntities();
  }

  TestChecks(const Simulation& _simulation) : debug::Checks(_simulation) {}
};

struct TestSimulation {
 private:
  using EvsInfo = decltype(PokemonCreationInfo::evs);
  using IvsInfo = decltype(PokemonCreationInfo::ivs);
  using StatsInfo = decltype(PokemonCreationInfo::stats);
  using CurrentBoostsInfo = decltype(PokemonCreationInfo::currentBoosts);
  bool simulationInitialized = false;

 public:
  Pokedex pokedex;
  Simulation simulation;
  std::vector<BattleCreationInfo> battleInfoList;
  TestChecks checks;

  struct BattleEntities {
    types::entity battle;
    types::entity p1Side;
    types::entity p2Side;
    types::entity p1A;
    types::entity p1B;
    types::entity p2A;
    types::entity p2B;
  };

  TestSimulation(GameMechanics mechanics, BattleFormat battleFormat)
      : pokedex(mechanics), simulation(pokedex, battleFormat), checks(simulation) {
    DamageRollKind simplestDamageRoll = DamageRollKind::MAX_DAMAGE | DamageRollKind::NO_CRIT_CHANCE;
    simulateTurnOptions().setDamageRollsConsidered({simplestDamageRoll});
    calcDamageOptions().setDamageRollOptions({simplestDamageRoll});
    analyzeEffectOptions().setDamageRollOptions({simplestDamageRoll});
  }

  types::registry& registry() { return simulation.registry; }
  simulate_turn::Options& simulateTurnOptions() { return simulation.simulateTurnOptions; }
  calc_damage::Options& calcDamageOptions() { return simulation.calculateDamageOptions; }
  analyze_effect::Options& analyzeEffectOptions() { return simulation.analyzeEffectOptions; }
  bool usesParentBattle() const {
    return simulation.simulateTurnOptions.getMakeBranchesOnRandomEvents() ||
           !simulation.simulateTurnOptions.getApplyChangesToInputBattle();
  }

  template <auto DataFunction, typename... Args>
  auto dexValue(Args&&... args) {
    return pokedex.getStaticValue<DataFunction>(std::forward<Args>(args)...);
  }

  template <typename... Types>
  BattleCreationInfo& setupBattle(Types... values) {
    BattleCreationInfo& info = battleInfoList.emplace_back();
    (assignByType(info, values), ...);
    return info;
  }

  template <typename... Types>
  SideCreationInfo side(Types... values) {
    SideCreationInfo info;
    (assignByType(info, values), ...);
    return info;
  }

  template <typename... DecisionTypes>
  TurnDecisionInfo turnDecision(DecisionTypes... values) {
    static constexpr auto InputCount = sizeof...(DecisionTypes);
    static constexpr bool ForSingles =
      InputCount == Constants::ActivePokemonSlotsPerSide::SINGLES * Constants::SIDE_COUNT;
    static constexpr bool ForDoubles =
      InputCount == Constants::ActivePokemonSlotsPerSide::DOUBLES * Constants::SIDE_COUNT;

    if constexpr (ForSingles) {
      REQUIRE(simulation.isBattleFormat(BattleFormat::SINGLES));
      return singleBattleTurnDecision(values...);
    }
    else if constexpr (ForDoubles) {
      REQUIRE(simulation.isBattleFormat(BattleFormat::DOUBLES));
      return doubleBattleTurnDecision(values...);
    }

    FAIL("Incorrect number of arguments passed in.");
    return {};
  }

  template <typename DecisionType>
  types::slotDecision slotDecision(DecisionType value, Slot slot) {
    if constexpr (types::slotDecision::canHoldType<DecisionType>()) {
      return types::slotDecision{value};
    }
    else if constexpr (std::is_same_v<dex::Move, DecisionType>) {
      MoveDecision decision{slot, slot, value};

      entt::dense_set<dex::Move> moveSet;
      moveSet.insert(value);
      pokedex.loadMoves(moveSet);

      if (!pokedex.moveHasAll<move::singles_target::tags::Self>(value)) {
        PlayerSideId foeSide = sideIdToFoeSideId(slotToSideId(slot));
        Slot directFoe = sideIdAndPositionToSlot(foeSide, internal::slotToIndex(slot));
        decision.targetSlot = directFoe;
      }

      return decision;
    }
    else if constexpr (std::is_same_v<Slot, DecisionType>) {
      SwitchDecision decision{slot, value};
      return decision;
    }
    else {
      return {};
    }
  }

  template <typename... Types>
  PokemonCreationInfo pokemon(Types... values) {
    PokemonCreationInfo info;
    (assignByType(info, values), ...);
    return info;
  }

  EvsInfo evs(std::initializer_list<std::pair<dex::Stat, types::ev>> list) {
    EvsInfo info;
    for (const auto& [stat, value] : list) {
      assignByEnum(info, stat, value);
    }
    return info;
  }

  IvsInfo ivs(std::initializer_list<std::pair<dex::Stat, types::iv>> list) {
    IvsInfo info;
    for (const auto& [stat, value] : list) {
      assignByEnum(info, stat, value);
    }
    return info;
  }

  StatsInfo stats(std::initializer_list<std::pair<dex::Stat, types::stat>> list) {
    StatsInfo info;
    for (const auto& [stat, value] : list) {
      assignByEnum(info, stat, value);
    }
    return info;
  }

  CurrentBoostsInfo boosts(std::initializer_list<std::pair<dex::Stat, types::boost>> list) {
    CurrentBoostsInfo info;
    for (const auto& [stat, value] : list) {
      assignByEnum(info, stat, value);
    }
    return info;
  }

 public:
  static PokemonCreationInfo& getPokemonCreationInfo(BattleCreationInfo& info, Slot slot) {
    auto& team = info.sides.at(slotToSideId(slot)).team;
    types::teamPositionIndex index = internal::slotToIndex(slot);

    REQUIRE(index < team.size());
    return team[index];
  }

  PokemonCreationInfo& getPokemonCreationInfo(Slot slot) { return getPokemonCreationInfo(battleInfoList.back(), slot); }

  template <typename... Slots>
  static auto getPokemonCreationInfo(BattleCreationInfo& info, Slots... slots) {
    if constexpr (sizeof...(slots) == 1U) {
      return getPokemonCreationInfo(info, slots...);
    }
    return std::forward_as_tuple(getPokemonCreationInfo(info, slots)...);
  }

  template <typename... Slots>
  auto getPokemonCreationInfo(Slots... slots) {
    return getPokemonCreationInfo(battleInfoList.back(), slots...);
  }

  types::entity getPokemonEntity(types::entity battleEntity, Slot slot) {
    return slotToPokemonEntity(registry(), registry().get<Sides>(battleEntity), slot);
  }

  BattleEntities getBattleEntities(types::entity battleEntity) {
    BattleEntities entities{battleEntity};

    auto sides = registry().get<Sides>(battleEntity).val;
    entities.p1Side = sides.p1();
    entities.p2Side = sides.p2();
    const Team& p1Team = registry().get<Team>(sides.p1());
    const Team& p2Team = registry().get<Team>(sides.p2());
    entities.p1A = p1Team.val[0];
    entities.p2A = p2Team.val[0];

    if (p1Team.val.size() >= 2U) {
      entities.p1B = p1Team.val[1];
    }
    if (p2Team.val.size() >= 2U) {
      entities.p2B = p2Team.val[1];
    }

    return entities;
  }

  void initializeSimulation() {
    simulation.registry.clear();
    pokedex.loadForBattleInfo(battleInfoList);
    simulation.createInitialStates(battleInfoList);
    simulationInitialized = true;
  }

  template <typename... BattleTypesToIgnore, typename... SideTypesToIgnore>
  simulate_turn::Results simulateTurn(Tags<BattleTypesToIgnore...> = {}, Tags<SideTypesToIgnore...> = {}) {
    if (!simulationInitialized) initializeSimulation();
    checks.reset();

    simulate_turn::Results results = simulation.simulateTurn();

    checks.checkPostSimulateTurnBattles<BattleTypesToIgnore...>(usesParentBattle());
    checks.checkPostSimulateTurnSides<SideTypesToIgnore...>();

    return results;
  }

  calc_damage::Results calculateDamage() {
    if (!simulationInitialized) initializeSimulation();
    return simulation.calculateDamage();
  }

  analyze_effect::Results analyzeEffect() {
    if (!simulationInitialized) initializeSimulation();
    return simulation.analyzeEffect();
  }

  template <typename... BattleTypesToIgnore, typename... SideTypesToIgnore>
  simulate_turn::Results simulateOneBattle(
    Tags<BattleTypesToIgnore...> battleIgnoredTypes = {}, Tags<SideTypesToIgnore...> sideIgnoredTypes = {}) {
    REQUIRE(battleInfoList.size() == 1U);

    simulate_turn::Results results = simulateTurn(battleIgnoredTypes, sideIgnoredTypes);
    REQUIRE(results.rootBattles().size() == 1U);

    return results;
  }

  template <typename... BattleTypesToIgnore, typename... SideTypesToIgnore>
  auto simulateOneBranchingBattle(
    Tags<BattleTypesToIgnore...> battleIgnoredTypes = {}, Tags<SideTypesToIgnore...> sideIgnoredTypes = {}) {
    auto battleOutcomes = simulateOneBattle(battleIgnoredTypes, sideIgnoredTypes).battleOutcomes();
    std::vector<BattleEntities> battleEntitiesList;
    battleEntitiesList.reserve(battleOutcomes.size());

    for (types::entity battle : battleOutcomes) {
      battleEntitiesList.push_back(getBattleEntities(battle));
    }

    return battleEntitiesList;
  }

  template <typename... BattleTypesToIgnore, typename... SideTypesToIgnore>
  BattleEntities simulateOneNonBranchingBattle(
    Tags<BattleTypesToIgnore...> battleIgnoredTypes = {}, Tags<SideTypesToIgnore...> sideIgnoredTypes = {}) {
    simulate_turn::Results results = simulateOneBattle(battleIgnoredTypes, sideIgnoredTypes);
    auto outcomes = results.battleOutcomes();

    REQUIRE(outcomes.size() == 1U);

    return getBattleEntities(outcomes.front());
  }

  void applyDecision(types::entity battle, TurnDecisionInfo turnDecision) {
    Sides sides = registry().get<Sides>(battle);
    registry().emplace<SideDecision>(sides.val.at(PlayerSideId::P1), turnDecision.at(PlayerSideId::P1));
    registry().emplace<SideDecision>(sides.val.at(PlayerSideId::P2), turnDecision.at(PlayerSideId::P2));
  }

 private:
  template <typename... DecisionTypes>
  TurnDecisionInfo singleBattleTurnDecision(DecisionTypes... values) {
    types::slotDecision p1A = slotDecision(std::get<0U>(std::tie(values...)), Slot::P1A);
    types::slotDecision p2A = slotDecision(std::get<1U>(std::tie(values...)), Slot::P2A);

    TurnDecisionInfo turnDecision = {
      {PlayerSideId::P1, types::slotDecisions{p1A}},
      {PlayerSideId::P2, types::slotDecisions{p2A}},
    };
    return turnDecision;
  }

  template <typename... DecisionTypes>
  TurnDecisionInfo doubleBattleTurnDecision(DecisionTypes... values) {
    types::slotDecision p1A = slotDecision(std::get<0U>(std::tie(values...)), Slot::P1A);
    types::slotDecision p1B = slotDecision(std::get<1U>(std::tie(values...)), Slot::P1B);
    types::slotDecision p2A = slotDecision(std::get<2U>(std::tie(values...)), Slot::P2A);
    types::slotDecision p2B = slotDecision(std::get<3U>(std::tie(values...)), Slot::P2B);

    TurnDecisionInfo turnDecision = {
      {PlayerSideId::P1, types::slotDecisions{p1A, p1B}},
      {PlayerSideId::P2, types::slotDecisions{p2A, p2B}},
    };
    return turnDecision;
  }

  template <typename Info, typename StatType>
  void assignByEnum(Info& info, dex::Stat stat, StatType value) {
    if constexpr (!std::is_same_v<CurrentBoostsInfo, Info>) {
      if (stat == dex::Stat::HP) {
        info.hp = value;
      }
    }
    switch (stat) {
      case dex::Stat::ATK: info.atk = value; break;
      case dex::Stat::DEF: info.def = value; break;
      case dex::Stat::SPA: info.spa = value; break;
      case dex::Stat::SPD: info.spd = value; break;
      case dex::Stat::SPE: info.spe = value; break;
      default:             break;
    }
  }

  template <typename Type>
  void assignByType(BattleCreationInfo& info, Type value) {
    if constexpr (std::is_same_v<Turn, Type>) {
      info.turn = value.val;
    }
    else if constexpr (std::is_same_v<RngSeed, Type>) {
      info.rngSeed = value.val;
    }
    else if constexpr (std::is_same_v<Probability, Type>) {
      info.probability = value.val;
    }
    else if constexpr (std::is_same_v<types::sides<SideCreationInfo>, Type>) {
      info.sides = value;
    }
    else if constexpr (std::is_same_v<SideCreationInfo, Type>) {
      if (info.sides.p1().team.empty()) {
        info.sides.p1() = value;
      }
      else if (info.sides.p2().team.empty()) {
        info.sides.p2() = value;
      }
      else {
        FAIL("Too many sides are being added to a battle.");
      }
    }
    else if constexpr (std::is_same_v<std::vector<TurnDecisionInfo>, Type>) {
      info.runWithSimulateTurn = true;
      info.decisionsToSimulate = value;
    }
    else if constexpr (std::is_same_v<TurnDecisionInfo, Type>) {
      info.runWithSimulateTurn = true;
      info.decisionsToSimulate.push_back(value);
    }
    else if constexpr (std::is_same_v<std::vector<CalcDamageInputInfo>, Type>) {
      info.runWithCalculateDamage = true;
      info.damageCalculations = value;
    }
    else if constexpr (std::is_same_v<CalcDamageInputInfo, Type>) {
      info.runWithCalculateDamage = true;
      info.damageCalculations.push_back(value);
    }
    else if constexpr (std::is_same_v<std::vector<AnalyzeEffectInputInfo>, Type>) {
      info.runWithAnalyzeEffect = true;
      info.effectsToAnalyze = value;
    }
    else if constexpr (std::is_same_v<AnalyzeEffectInputInfo, Type>) {
      info.runWithAnalyzeEffect = true;
      info.effectsToAnalyze.push_back(value);
    }
  }

  template <typename Type>
  void assignByType(SideCreationInfo& info, Type value) {
    if constexpr (std::is_same_v<std::vector<PokemonCreationInfo>, Type>) {
      info.team = value;
    }
    if constexpr (std::is_same_v<PokemonCreationInfo, Type>) {
      info.team.push_back(value);
    }
  }

  template <typename Type>
  void assignByType(PokemonCreationInfo& info, Type value) {
    if constexpr (std::is_same_v<dex::Species, Type>) {
      info.species = value;
    }
    else if constexpr (std::is_same_v<Level, Type>) {
      info.level = value.val;
    }
    else if constexpr (std::is_same_v<dex::Item, Type>) {
      info.item = value;
    }
    else if constexpr (std::is_same_v<dex::Ability, Type>) {
      info.ability = value;
    }
    else if constexpr (std::is_same_v<dex::Gender, Type>) {
      info.gender = value;
    }
    else if constexpr (std::is_same_v<dex::Nature, Type>) {
      info.nature = value;
    }
    else if constexpr (std::is_same_v<Id, Type>) {
      info.id = value.val;
    }
    else if constexpr (std::is_same_v<EvsInfo, Type>) {
      info.evs = value;
    }
    else if constexpr (std::is_same_v<Evs, Type>) {
      info.evs.hp = value.hp;
      info.evs.atk = value.atk;
      info.evs.def = value.def;
      info.evs.spa = value.spa;
      info.evs.spd = value.spd;
      info.evs.spe = value.spe;
    }
    else if constexpr (std::is_same_v<IvsInfo, Type>) {
      info.ivs = value;
    }
    else if constexpr (std::is_same_v<Ivs, Type>) {
      info.ivs.hp = value.hp;
      info.ivs.atk = value.atk;
      info.ivs.def = value.def;
      info.ivs.spa = value.spa;
      info.ivs.spd = value.spd;
      info.ivs.spe = value.spe;
    }
    else if constexpr (std::is_same_v<StatsInfo, Type>) {
      info.stats = value;
    }
    else if constexpr (std::is_same_v<std::vector<MoveCreationInfo>, Type>) {
      info.moves = value;
    }
    else if constexpr (std::is_same_v<dex::Move, Type> || std::is_same_v<MoveCreationInfo, Type>) {
      info.moves.push_back(MoveCreationInfo{value});
    }
    else if constexpr (std::is_same_v<stat::CurrentHp, Type>) {
      info.currentHp = value;
    }
    else if constexpr (std::is_same_v<SpeciesTypes, Type>) {
      info.currentTypes = value;
    }
    else if constexpr (std::is_same_v<dex::Status, Type>) {
      info.status = value;
    }
    else if constexpr (std::is_same_v<CurrentBoostsInfo, Type>) {
      info.currentBoosts = value;
    }
  }
};
}  // namespace pokesim

namespace Catch {
template <>
struct StringMaker<pokesim::DamageRollKind> {
  static void append(std::string& fullName, const std::string& part) {
    if (fullName.empty()) {
      fullName = part;
    }
    else {
      fullName += " | " + part;
    }
  }

  static std::string convert(const pokesim::DamageRollKind& value) {
    std::string fullName;
    if (value & pokesim::DamageRollKind::AVERAGE_DAMAGE) {
      append(fullName, "AVERAGE_DAMAGE");
    }
    if (value & pokesim::DamageRollKind::MIN_DAMAGE) {
      append(fullName, "MIN_DAMAGE");
    }
    if (value & pokesim::DamageRollKind::MAX_DAMAGE) {
      append(fullName, "MAX_DAMAGE");
    }
    if (value & pokesim::DamageRollKind::GUARANTEED_CRIT_CHANCE) {
      append(fullName, "GUARANTEED_CRIT_CHANCE");
    }
    if (value & pokesim::DamageRollKind::NO_CRIT_CHANCE) {
      append(fullName, "NO_CRIT_CHANCE");
    }
    if (value & pokesim::DamageRollKind::ALL_DAMAGE_ROLLS) {
      append(fullName, "ALL_DAMAGE_ROLLS");
    }

    return fullName;
  }
};

template <>
struct StringMaker<std::nullopt_t> {
  static std::string convert(const std::nullopt_t&) { return "nullopt"; }
};
}  // namespace Catch
