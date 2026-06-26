#include <AnalyzeEffect/Setup/AnalyzeEffectInputSetup.hpp>
#include <Battle/Clone/Clone.hpp>
#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Setup/PokemonStateSetup.hpp>
#include <Battle/Setup/headers.hpp>
#include <CalcDamage/Setup/CalcDamageInputSetup.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Pokedex/Abilities.hpp>
#include <Components/Pokedex/BaseStats.hpp>
#include <Components/Pokedex/PP.hpp>
#include <Components/SideDecisions.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Constants.hpp>
#include <Types/Decisions.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/State.hpp>
#include <Utilities/Variant.hpp>
#include <cmath>
#include <cstddef>
#include <entt/entity/registry.hpp>
#include <vector>

#include "BattleCreationInfo.hpp"
#include "Formulas.hpp"
#include "Simulation.hpp"
#include "SimulationSetupDebugChecks.hpp"

namespace pokesim {
namespace {
std::size_t getBattleCreationCount(const BattleCreationInfo& battleInfo) {
  return std::max((std::size_t)1UL, battleInfo.decisionsToSimulate.size());
}

struct EntityLists {
  struct EntityList {
   private:
    types::entityVector list{};
    types::entityIndex index = 0U;

   public:
    EntityList() {}
    EntityList(types::registry& registry, types::entityIndex size) : list(size) {
      registry.create(list.begin(), list.end());
    }

    types::entity getNext() {
      POKESIM_REQUIRE(index < list.size(), "More entities are being asked for than were created.");
      types::entity nextEntity = list[index];
      index++;
      return nextEntity;
    }
  };

  EntityList battles;
  EntityList sides;
  EntityList pokemon;
  EntityList recycledActions;
  EntityList recycledActionMoves;
  EntityList addedRecycledActionMoves;
  EntityList calcDamageInputs;
  EntityList analyzeEffectInputs;

  EntityLists(Simulation* simulation, const std::vector<BattleCreationInfo>& battleInfoList) {
    types::entityIndex battleCount = 0U;
    types::entityIndex pokemonCount = 0U;
    types::entityIndex calcDamageInputCount = 0U;
    types::entityIndex analyzeEffectInputCount = 0U;
    for (const BattleCreationInfo& battleInfo : battleInfoList) {
      types::entityIndex battleCountIncrease = (types::entityIndex)getBattleCreationCount(battleInfo);
      battleCount += battleCountIncrease;

      for (const SideCreationInfo& side : battleInfo.sides) {
        pokemonCount += (types::teamPositionIndex)side.team.size() * battleCountIncrease;
      }

      for (const CalcDamageInputInfo& calcDamageInputInfo : battleInfo.damageCalculations) {
        calcDamageInputCount += (types::entityIndex)calcDamageInputInfo.moves.size();
      }
      for (const AnalyzeEffectInputInfo& analyzeEffectInputInfo : battleInfo.effectsToAnalyze) {
        analyzeEffectInputCount += (types::entityIndex)analyzeEffectInputInfo.moves.size();
      }
    }

    types::entityIndex sideCount = battleCount * 2U;
    types::entityIndex recycledActionCount = battleCount;
    types::entityIndex recycledActionMoveCount = battleCount;
    types::entityIndex addedRecycledActionMoveCount =
      simulation->isBattleFormat(BattleFormat::DOUBLES) ? battleCount * 2U : 0U;

    POKESIM_REQUIRE(
      battleCount + sideCount + recycledActionCount + pokemonCount + calcDamageInputCount + analyzeEffectInputCount <
        std::numeric_limits<types::entityIndex>::max(),
      "More entities than can be created would be made for this input.");

    types::registry& registry = simulation->registry;
    battles = {registry, battleCount};
    sides = {registry, sideCount};
    pokemon = {registry, pokemonCount};
    recycledActions = {registry, recycledActionCount};
    recycledActionMoves = {registry, recycledActionMoveCount};
    addedRecycledActionMoves = {registry, addedRecycledActionMoveCount};
    calcDamageInputs = {registry, calcDamageInputCount};
    analyzeEffectInputs = {registry, analyzeEffectInputCount};
  }
};

void setPokemonAbility(
  const PokemonCreationInfo& pokemonInfo, PokemonStateSetup& pokemonSetup, const Pokedex& pokedex) {
  if (pokemonInfo.ability != dex::Ability::NO_ABILITY) {
    if (pokemonInfo.ability.has_value()) {
      pokemonSetup.setAbility(pokemonInfo.ability.value());
    }
    else if (pokedex.speciesHas<PrimaryAbility>(pokemonInfo.species)) {
      pokemonSetup.setAbility(pokedex.getSpeciesData<PrimaryAbility>(pokemonInfo.species).val);
    }
  }
}

Evs setPokemonEvs(const PokemonCreationInfo& pokemonInfo, PokemonStateSetup& pokemonSetup) {
  Evs evs;
  if (pokemonInfo.evs.hp.has_value()) evs.hp = pokemonInfo.evs.hp.value();
  if (pokemonInfo.evs.atk.has_value()) evs.atk = pokemonInfo.evs.atk.value();
  if (pokemonInfo.evs.def.has_value()) evs.def = pokemonInfo.evs.def.value();
  if (pokemonInfo.evs.spa.has_value()) evs.spa = pokemonInfo.evs.spa.value();
  if (pokemonInfo.evs.spd.has_value()) evs.spd = pokemonInfo.evs.spd.value();
  if (pokemonInfo.evs.spe.has_value()) evs.spe = pokemonInfo.evs.spe.value();

  pokemonSetup.setEVs(evs);
  return evs;
}

Ivs setPokemonIvs(const PokemonCreationInfo& pokemonInfo, PokemonStateSetup& pokemonSetup) {
  Ivs ivs;
  if (pokemonInfo.ivs.hp.has_value()) ivs.hp = pokemonInfo.ivs.hp.value();
  if (pokemonInfo.ivs.atk.has_value()) ivs.atk = pokemonInfo.ivs.atk.value();
  if (pokemonInfo.ivs.def.has_value()) ivs.def = pokemonInfo.ivs.def.value();
  if (pokemonInfo.ivs.spa.has_value()) ivs.spa = pokemonInfo.ivs.spa.value();
  if (pokemonInfo.ivs.spd.has_value()) ivs.spd = pokemonInfo.ivs.spd.value();
  if (pokemonInfo.ivs.spe.has_value()) ivs.spe = pokemonInfo.ivs.spe.value();

  pokemonSetup.setIVs(ivs);
  return ivs;
}

dex::Nature setPokemonNature(const PokemonCreationInfo& pokemonInfo, PokemonStateSetup& pokemonSetup) {
  dex::Nature nature = dex::Nature::NO_NATURE;
  if (pokemonInfo.nature.has_value() && pokemonInfo.nature != dex::Nature::NO_NATURE) {
    nature = pokemonInfo.nature.value();
    pokemonSetup.setNature(nature);
  }

  return nature;
}

types::stat setPokemonStats(
  const PokemonCreationInfo& pokemonInfo, PokemonStateSetup& pokemonSetup, const Pokedex& pokedex, types::level level,
  dex::Nature nature, const Evs& evs, const Ivs& ivs) {
  const BaseStats& baseStats = pokedex.getSpeciesData<BaseStats>(pokemonInfo.species);

  auto getStat = [&](dex::Stat statName) {
    types::baseStat baseStat = Constants::PokemonBaseStat::DEFAULT;
    std::optional<types::stat> givenStat = std::nullopt;

    if (statName == dex::Stat::HP) {
      baseStat = baseStats.hp;
      givenStat = pokemonInfo.stats.hp;
    }
    else if (statName == dex::Stat::ATK) {
      baseStat = baseStats.atk;
      givenStat = pokemonInfo.stats.atk;
    }
    else if (statName == dex::Stat::DEF) {
      baseStat = baseStats.def;
      givenStat = pokemonInfo.stats.def;
    }
    else if (statName == dex::Stat::SPA) {
      baseStat = baseStats.spa;
      givenStat = pokemonInfo.stats.spa;
    }
    else if (statName == dex::Stat::SPD) {
      baseStat = baseStats.spd;
      givenStat = pokemonInfo.stats.spd;
    }
    else if (statName == dex::Stat::SPE) {
      baseStat = baseStats.spe;
      givenStat = pokemonInfo.stats.spe;
    }
    else {
      POKESIM_REQUIRE_FAIL("Using a stat that cannot be chosen or calculated.");
    }

    if (givenStat.has_value()) {
      return givenStat.value();
    }

    return computeStatFromBaseStat(statName, baseStat, level, nature, evs, ivs);
  };

  types::stat hp = getStat(dex::Stat::HP);

  pokemonSetup.setStat<stat::Hp>(hp);
  pokemonSetup.setStat<stat::Atk>(getStat(dex::Stat::ATK));
  pokemonSetup.setStat<stat::Def>(getStat(dex::Stat::DEF));
  pokemonSetup.setStat<stat::Spa>(getStat(dex::Stat::SPA));
  pokemonSetup.setStat<stat::Spd>(getStat(dex::Stat::SPD));
  pokemonSetup.setStat<stat::Spe>(getStat(dex::Stat::SPE));

  return hp;
}

void setPokemonCurrentBoosts(const PokemonCreationInfo& pokemonInfo, PokemonStateSetup& pokemonSetup) {
  const auto& pokemonInfoBoosts = pokemonInfo.currentBoosts;
  if (pokemonInfoBoosts.atk.has_value()) pokemonSetup.setBoost<AtkBoost>(pokemonInfoBoosts.atk.value());
  if (pokemonInfoBoosts.def.has_value()) pokemonSetup.setBoost<DefBoost>(pokemonInfoBoosts.def.value());
  if (pokemonInfoBoosts.spa.has_value()) pokemonSetup.setBoost<SpaBoost>(pokemonInfoBoosts.spa.value());
  if (pokemonInfoBoosts.spd.has_value()) pokemonSetup.setBoost<SpdBoost>(pokemonInfoBoosts.spd.value());
  if (pokemonInfoBoosts.spe.has_value()) pokemonSetup.setBoost<SpeBoost>(pokemonInfoBoosts.spe.value());
}

void createInitialPokemon(
  const PokemonCreationInfo& pokemonInfo, PokemonStateSetup& pokemonSetup, const Pokedex& pokedex) {
  if (pokemonInfo.id.has_value()) {
    pokemonSetup.setID(pokemonInfo.id.value());
  }
  else {
    pokemonSetup.setAutoID();
  }

  pokemonSetup.setSpecies(pokemonInfo.species);
  setPokemonAbility(pokemonInfo, pokemonSetup, pokedex);

  types::level level = pokemonInfo.level.value_or(Constants::PokemonLevel::DEFAULT);
  dex::Nature nature = setPokemonNature(pokemonInfo, pokemonSetup);
  Evs evs = setPokemonEvs(pokemonInfo, pokemonSetup);
  Ivs ivs = setPokemonIvs(pokemonInfo, pokemonSetup);
  types::stat hp = setPokemonStats(pokemonInfo, pokemonSetup, pokedex, level, nature, evs, ivs);

  pokemonSetup.setLevel(level);

  if (pokemonInfo.currentTypes.has_value()) {
    pokemonSetup.setTypes(pokemonInfo.currentTypes.value());
  }
  else {
    pokemonSetup.setTypes(pokedex.getSpeciesData<SpeciesTypes>(pokemonInfo.species));
  }

  if (pokemonInfo.gender.has_value() && pokemonInfo.gender != dex::Gender::NO_GENDER) {
    pokemonSetup.setGender(pokemonInfo.gender.value());
  }

  if (pokemonInfo.item.has_value() && pokemonInfo.item != dex::Item::NO_ITEM) {
    pokemonSetup.setItem(pokemonInfo.item.value());
  }

  if (pokemonInfo.status.has_value() && pokemonInfo.status != dex::Status::NO_STATUS) {
    pokemonSetup.setStatus(pokemonInfo.status.value());
  }

  pokemonSetup.setCurrentHp(pokemonInfo.currentHp.value_or(hp));

  if (pokemonInfo.currentHp.has_value() && pokemonInfo.currentHp == Constants::PokemonCurrentHpStat::MIN) {
    pokemonSetup.setProperty<tags::Fainted>();
  }

  setPokemonCurrentBoosts(pokemonInfo, pokemonSetup);

  pokemonSetup.setProperty<tags::AtkStatUpdateRequired>();
  pokemonSetup.setProperty<tags::DefStatUpdateRequired>();
  pokemonSetup.setProperty<tags::SpaStatUpdateRequired>();
  pokemonSetup.setProperty<tags::SpdStatUpdateRequired>();
  pokemonSetup.setProperty<tags::SpeStatUpdateRequired>();
}

void createCalcDamageInput(
  const CalcDamageInputInfo& inputInfo, BattleStateSetup& battleSetup, types::registry& registry,
  EntityLists& entityLists, debug::SimulationSetupChecks& debugChecks) {
  POKESIM_REQUIRE(inputInfo.attackerSlot != Slot::NONE, "A damage calculation must have a attacker.");
  POKESIM_REQUIRE(inputInfo.defenderSlot != Slot::NONE, "A damage calculation must have a defender.");
  POKESIM_REQUIRE(!inputInfo.moves.empty(), "A damage calculation must have a move.");

  const Sides& sides = registry.get<Sides>(battleSetup.entity());
  types::entity attackerEntity = slotToPokemonEntity(registry, sides, inputInfo.attackerSlot);
  types::entity defenderEntity = slotToPokemonEntity(registry, sides, inputInfo.defenderSlot);

  for (dex::Move move : inputInfo.moves) {
    calc_damage::InputSetup inputSetup{registry, entityLists.calcDamageInputs.getNext()};
    POKESIM_REQUIRE(move != dex::Move::NO_MOVE, "A damage calculation must have a move.");

    inputSetup.setup(battleSetup.entity(), attackerEntity, defenderEntity, move);
    debugChecks.addToCalcDamageChecklist(battleSetup, inputSetup, inputInfo);
  }
}

void createAnalyzeEffectInput(
  const AnalyzeEffectInputInfo& inputInfo, BattleStateSetup& battleSetup, types::registry& registry,
  EntityLists& entityLists, debug::SimulationSetupChecks& debugChecks) {
  POKESIM_REQUIRE(inputInfo.attackerSlot != Slot::NONE, "An effect analysis must have a attacker.");
  POKESIM_REQUIRE(inputInfo.defenderSlot != Slot::NONE, "An effect analysis must have a defender.");
  POKESIM_REQUIRE(inputInfo.effectTarget != Slot::NONE, "An effect analysis must have a effect target.");
  POKESIM_REQUIRE(!inputInfo.moves.empty(), "An effect analysis must include a move.");

  const auto& effect = inputInfo.effect;
  const auto& boostEffect = inputInfo.boostEffect;
  POKESIM_REQUIRE(
    boostEffect.has_value() || (effect.has_value() && !effect.value().empty()),
    "An effect analysis must have an effect.");

  const Sides& sides = registry.get<Sides>(battleSetup.entity());
  types::entity attackerEntity = slotToPokemonEntity(registry, sides, inputInfo.attackerSlot);
  types::entity defenderEntity = slotToPokemonEntity(registry, sides, inputInfo.defenderSlot);
  types::entity effectTargetEntity = slotToPokemonEntity(registry, sides, inputInfo.effectTarget);

  for (dex::Move move : inputInfo.moves) {
    analyze_effect::InputSetup inputSetup{registry, entityLists.analyzeEffectInputs.getNext()};

    inputSetup.setAttacker(attackerEntity);
    inputSetup.setDefender(defenderEntity);
    inputSetup.setEffectTarget(effectTargetEntity);
    inputSetup.setEffectMove(move);
    inputSetup.setBattle(battleSetup.entity());

    if (effect.has_value()) {
      inputSetup.setEffect(effect.value());
    }
    if (boostEffect.has_value()) {
      inputSetup.setBoostEffect(boostEffect.value().stat, boostEffect.value().boost);
    }

    debugChecks.addToAnalyzeEffectChecklist(battleSetup, inputSetup, inputInfo);
  }
}

void createInitialTurnDecision(const TurnDecisionInfo& turnDecisionInfo, types::sides<SideStateSetup>& sideSetupList) {
  for (types::sideIndex i = 0U; i < sideSetupList.size(); i++) {
    sideSetupList[i].setSideDecision(turnDecisionInfo[i]);
  }
}

void createInitialSide(
  const SideCreationInfo& sideInfo, SideStateSetup& sideSetup, const BattleCreationInfo& battleInfo,
  Simulation* simulation, internal::maxSizedVector<PokemonStateSetup, Constants::TeamSize::MAX>& pokemonSetupList) {
  for (std::size_t i = 0U; i < sideInfo.team.size(); i++) {
    PokemonStateSetup& pokemonSetup = pokemonSetupList[i];
    const PokemonCreationInfo& pokemonInfo = sideInfo.team[i];

    bool battleStarted = battleInfo.turn > Constants::TurnCount::MIN;
    bool inActiveSlot = (simulation->isBattleFormat(BattleFormat::SINGLES) ? 1U : 2U) > i;
    bool isFainted = pokemonInfo.currentHp.has_value() && pokemonInfo.currentHp == Constants::PokemonCurrentHpStat::MIN;
    if (battleStarted && inActiveSlot && !isFainted) {
      pokemonSetup.setProperty<tags::ActivePokemon>();
    }

    std::vector<MoveSlot> moveSlots;
    for (const MoveCreationInfo& moveInfo : pokemonInfo.moves) {
      types::pp maxPp = Constants::MoveMaxPp::DEFAULT;
      if (!moveInfo.pp.has_value() || !moveInfo.maxPp.has_value()) {
        maxPp = simulation->pokedex().getMoveData<Pp>(moveInfo.name).val;
      }
      maxPp = moveInfo.maxPp.value_or(maxPp);

      moveSlots.push_back({moveInfo.name, moveInfo.pp.value_or(maxPp), maxPp});
    }

    pokemonSetup.setMoves(moveSlots);
  }

  types::registry& registry = simulation->registry;
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

void createInitialBattle(
  const BattleCreationInfo& battleInfo, BattleStateSetup& battleSetup, types::sides<SideStateSetup>& sideSetupList) {
  battleSetup.setTurn(battleInfo.turn.value_or(Constants::TurnCount::DEFAULT));
  battleSetup.setRNGSeed(battleInfo.rngSeed);
  battleSetup.setProbability(battleInfo.probability.value_or(Constants::Probability::DEFAULT));

  if (battleInfo.runWithSimulateTurn) {
    battleSetup.setProperty<tags::SimulateTurn>();
  }
  if (battleInfo.runWithCalculateDamage) {
    battleSetup.setProperty<tags::CalculateDamage>();
  }
  if (battleInfo.runWithAnalyzeEffect) {
    battleSetup.setProperty<tags::AnalyzeEffect>();
  }

  SideStateSetup& p1SideSetup = sideSetupList.p1();
  SideStateSetup& p2SideSetup = sideSetupList.p2();

  p1SideSetup.setPlayerSide(PlayerSideId::P1);
  p2SideSetup.setPlayerSide(PlayerSideId::P2);

  types::entity battleEntity = battleSetup.entity();
  types::entity p1Entity = p1SideSetup.entity();
  types::entity p2Entity = p2SideSetup.entity();

  battleSetup.setSide(p1Entity);
  battleSetup.setSide(p2Entity);

  p1SideSetup.setOpponent(p2Entity);
  p2SideSetup.setOpponent(p1Entity);

  p1SideSetup.setBattle(battleEntity);
  p2SideSetup.setBattle(battleEntity);
}

void createInitialState(
  const BattleCreationInfo& battleInfo, Simulation* simulation, EntityLists& entityLists,
  debug::SimulationSetupChecks& debugChecks) {
  internal::maxSizedVector<BattleStateSetup> battleSetupList;
  internal::maxSizedVector<types::sides<SideStateSetup>> sideSetupLists;
  internal::maxSizedVector<types::sides<internal::maxSizedVector<PokemonStateSetup, Constants::TeamSize::MAX>>>
    pokemonSetupLists;
  battleSetupList.resize(getBattleCreationCount(battleInfo));
  sideSetupLists.resize(battleSetupList.size());
  pokemonSetupLists.resize(battleSetupList.size());

  const Pokedex& pokedex = simulation->pokedex();
  types::registry& registry = simulation->registry;

  for (BattleStateSetup& battleSetup : battleSetupList) {
    battleSetup = {registry, entityLists.battles.getNext()};
  }

  for (types::sideIndex sideIndex = 0; sideIndex < battleInfo.sides.size(); sideIndex++) {
    for (auto& sideSetupList : sideSetupLists) {
      sideSetupList[sideIndex] = {registry, entityLists.sides.getNext()};
    }
  }

  for (types::entityIndex battleIndex = 0; battleIndex < battleSetupList.size(); battleIndex++) {
    BattleStateSetup& battleSetup = battleSetupList[battleIndex];
    createInitialBattle(battleInfo, battleSetup, sideSetupLists[battleIndex]);
    battleSetup.setRecycledAction(entityLists.recycledActions.getNext(), entityLists.recycledActionMoves.getNext());
    if (simulation->isBattleFormat(BattleFormat::DOUBLES)) {
      battleSetup.setAddedRecycledActionMoves(
        entityLists.addedRecycledActionMoves.getNext(),
        entityLists.addedRecycledActionMoves.getNext());
    }
  }

  for (types::sideIndex sideIndex = 0; sideIndex < battleInfo.sides.size(); sideIndex++) {
    for (const PokemonCreationInfo& pokemonInfo : battleInfo.sides[sideIndex].team) {
      for (auto& pokemonSetupList : pokemonSetupLists) {
        PokemonStateSetup& pokemonSetup =
          pokemonSetupList[sideIndex].emplace_back(registry, entityLists.pokemon.getNext());
        createInitialPokemon(pokemonInfo, pokemonSetup, pokedex);
      }
    }
  }

  for (types::entityIndex battleIndex = 0; battleIndex < battleSetupList.size(); battleIndex++) {
    BattleStateSetup& battleSetup = battleSetupList[battleIndex];
    auto& sideSetupList = sideSetupLists[battleIndex];
    for (types::sideIndex sideIndex = 0; sideIndex < battleInfo.sides.size(); sideIndex++) {
      createInitialSide(
        battleInfo.sides[sideIndex],
        sideSetupList[sideIndex],
        battleInfo,
        simulation,
        pokemonSetupLists[battleIndex][sideIndex]);
    }

    if (battleIndex < battleInfo.decisionsToSimulate.size()) {
      const TurnDecisionInfo& turnDecisionInfo = battleInfo.decisionsToSimulate[battleIndex];
      createInitialTurnDecision(turnDecisionInfo, sideSetupList);
      debugChecks.addToTurnDecisionChecklist(battleSetup, turnDecisionInfo);
    }
    battleSetup.setID(battleIndex);
    debugChecks.addToBattleChecklist(battleSetup, battleInfo);
  }

  BattleStateSetup& battleSetup = battleSetupList[0];
  for (const CalcDamageInputInfo& calcDamageInputInfo : battleInfo.damageCalculations) {
    createCalcDamageInput(calcDamageInputInfo, battleSetup, registry, entityLists, debugChecks);
  }

  for (const AnalyzeEffectInputInfo& analyzeEffectInputInfo : battleInfo.effectsToAnalyze) {
    createAnalyzeEffectInput(analyzeEffectInputInfo, battleSetup, registry, entityLists, debugChecks);
  }
}
}  // namespace

void Simulation::createInitialStates(const std::vector<BattleCreationInfo>& battleInfoList) {
  debug::SimulationSetupChecks debugChecks(this, battleInfoList);

  EntityLists entityLists{this, battleInfoList};

  for (const BattleCreationInfo& battleInfo : battleInfoList) {
    createInitialState(battleInfo, this, entityLists, debugChecks);
  }

  pokedex().buildMoves(registry);
  registry.clear<internal::tags::BuildActionMove>();

  debugChecks.checkOutputs();
}
}  // namespace pokesim
