#include <AnalyzeEffect/Setup/AnalyzeEffectInputSetup.hpp>
#include <Battle/Clone/Clone.hpp>
#include <Battle/Helpers/Helpers.hpp>
#include <Battle/Setup/PokemonStateSetup.hpp>
#include <Battle/Setup/headers.hpp>
#include <CalcDamage/Setup/CalcDamageInputSetup.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/Decisions.hpp>
#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/PP.hpp>
#include <Components/Pokedex/Abilities.hpp>
#include <Components/Pokedex/BaseStats.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/State.hpp>
#include <Utilities/Variant.hpp>
#include <cstddef>
#include <entt/entity/registry.hpp>
#include <vector>

#include "BattleCreationInfo.hpp"
#include "Formulas.hpp"
#include "Simulation.hpp"
#include "SimulationSetupDebugChecks.hpp"

namespace pokesim {
namespace {
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
    types::baseStat baseStat = MechanicConstants::PokemonBaseStat::DEFAULT;
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
}  // namespace

types::entityVector Simulation::createInitialMoves(const std::vector<MoveCreationInfo>& moveInfoList) {
  types::entityVector moveEntities{};
  moveEntities.reserve((types::entityVector::size_type)moveInfoList.size());

  for (const MoveCreationInfo& moveInfo : moveInfoList) {
    MoveStateSetup moveSetup(registry);
    moveSetup.setName(moveInfo.name);
    types::pp maxPp = MechanicConstants::MoveMaxPp::DEFAULT;
    if (!moveInfo.pp.has_value() || !moveInfo.maxPp.has_value()) {
      maxPp = pokedex().getMoveData<Pp>(moveInfo.name).val;
    }
    maxPp = moveInfo.maxPp.value_or(maxPp);

    moveSetup.setPP(moveInfo.pp.value_or(maxPp));
    moveSetup.setMaxPP(maxPp);
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
  setPokemonAbility(pokemonInfo, pokemonSetup, pokedex());

  types::level level = pokemonInfo.level.value_or(MechanicConstants::PokemonLevel::DEFAULT);
  dex::Nature nature = setPokemonNature(pokemonInfo, pokemonSetup);
  Evs evs = setPokemonEvs(pokemonInfo, pokemonSetup);
  Ivs ivs = setPokemonIvs(pokemonInfo, pokemonSetup);
  types::stat hp = setPokemonStats(pokemonInfo, pokemonSetup, pokedex(), level, nature, evs, ivs);

  pokemonSetup.setLevel(level);

  if (pokemonInfo.currentTypes.has_value()) {
    pokemonSetup.setTypes(pokemonInfo.currentTypes.value());
  }
  else {
    pokemonSetup.setTypes(pokedex().getSpeciesData<SpeciesTypes>(pokemonInfo.species));
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

  if (pokemonInfo.currentHp.has_value() && pokemonInfo.currentHp == MechanicConstants::PokemonCurrentHpStat::MIN) {
    pokemonSetup.setProperty<tags::Fainted>();
  }

  setPokemonCurrentBoosts(pokemonInfo, pokemonSetup);

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
    bool inActiveSlot = (isBattleFormat(BattleFormat::SINGLES) ? 1U : 2U) > i;
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

types::sides<SideStateSetup> Simulation::createInitialBattle(
  BattleStateSetup battleStateSetup, const BattleCreationInfo& battleInfo) {
  battleStateSetup.setAutoID();
  battleStateSetup.setTurn(battleInfo.turn.value_or(MechanicConstants::TurnCount::DEFAULT));
  battleStateSetup.setRNGSeed(battleInfo.rngSeed);
  battleStateSetup.setProbability(battleInfo.probability.value_or(MechanicConstants::Probability::DEFAULT));

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

  for (types::sideIndex i = 0U; i < sides.val.size(); i++) {
    registry.emplace<SideDecision>(sides.val[i], turnDecisionInfo[i]);
  }
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
    types::sides<SideStateSetup> sideSetup = createInitialBattle(battleStateSetup, battleInfo);

    for (types::sideIndex i = 0U; i < sideSetup.size(); i++) {
      createInitialSide(sideSetup[i], battleInfo.sides[i], battleInfo);
    }

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
