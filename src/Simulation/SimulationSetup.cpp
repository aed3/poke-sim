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
#include <utility>
#include <vector>

#include "Simulation.hpp"
#include "SimulationSetupDebugChecks.hpp"

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
  if (pokemonInfo.types.has_value()) {
    pokemonSetup.setTypes(pokemonInfo.types.value());
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
  pokemonSetup.setHp(pokemonInfo.hp.value_or(pokemonInfo.stats.hp));
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

  for (std::size_t i = 0; i < sideInfo.team.size(); i++) {
    const PokemonCreationInfo& pokemonInfo = sideInfo.team[i];
    PokemonStateSetup pokemonSetup = createInitialPokemon(pokemonInfo);
    if (
      battleInfo.turn > 0 && !pokemonInfo.fainted &&
      (i == 0 || (battleFormat() == BattleFormat::SINGLES_BATTLE_FORMAT && i == 1))) {
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
  if (battleInfo.runWithCalculateDamage) {
    battleStateSetup.setProperty<tags::CalculateDamage>();
  }
  if (battleInfo.runWithAnalyzeEffect) {
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
  BattleStateSetup battleStateSetup, calc_damage::InputSetup& inputSetup, const CalcDamageInputInfo& inputInfo) {
  POKESIM_REQUIRE(inputInfo.attackerSlot != Slot::NONE, "A damage calculation must have a attacker.");
  POKESIM_REQUIRE(inputInfo.defenderSlot != Slot::NONE, "A damage calculation must have a defender.");
  POKESIM_REQUIRE(inputInfo.move != dex::Move::NO_MOVE, "A damage calculation must have a move.");

  const Sides& sides = registry.get<Sides>(battleStateSetup.entity());
  types::entity attackerEntity = slotToPokemonEntity(registry, sides, inputInfo.attackerSlot);
  types::entity defenderEntity = slotToPokemonEntity(registry, sides, inputInfo.defenderSlot);

  inputSetup.setup(battleStateSetup.entity(), attackerEntity, defenderEntity, inputInfo.move, pokedex());
}

void Simulation::createAnalyzeEffectInput(
  BattleStateSetup battleStateSetup, analyze_effect::InputSetup& inputSetup, const AnalyzeEffectInputInfo& inputInfo) {
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

  inputSetup.setAttacker(attackerEntity);
  inputSetup.setDefender(defenderEntity);
  inputSetup.setEffectTarget(effectTargetEntity);
  inputSetup.setEffectMoves(inputInfo.moves);
  inputSetup.setBattle(battleStateSetup.entity());

  if (effect.has_value()) {
    inputSetup.setEffect(effect.value());
  }
  if (boostEffect.has_value()) {
    inputSetup.setBoostEffect(boostEffect.value().stat, boostEffect.value().boost);
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
        battleInfo.decisionsToSimulate.size() < std::numeric_limits<types::cloneIndex>::max(),
        "Cannot make more clones than there are entities available.");

      types::cloneIndex cloneCount = (types::cloneIndex)(battleInfo.decisionsToSimulate.size() - 1);
      if (cloneCount) {
        battleStateSetup.setProperty<tags::CloneFrom>();
        const types::ClonedEntityMap entityMap = pokesim::clone(registry, cloneCount);

        const auto& clonedBattles = entityMap.at(battleStateSetup.entity());
        internal::maxSizedVector<BattleStateSetup> clones;
        clones.reserve(clonedBattles.size());

        for (types::entity entity : clonedBattles) {
          clones.emplace_back(registry, entity);
        }

        for (types::cloneIndex i = 0; i < cloneCount; i++) {
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
      calc_damage::InputSetup inputSetup(registry);
      createCalcDamageInput(battleStateSetup, inputSetup, calcDamageInputInfo);
      debugChecks.addToCalcDamageChecklist(battleStateSetup, inputSetup, calcDamageInputInfo);
    }

    for (const AnalyzeEffectInputInfo& analyzeEffectInputInfo : battleInfo.effectsToAnalyze) {
      analyze_effect::InputSetup inputSetup(registry);
      createAnalyzeEffectInput(battleStateSetup, inputSetup, analyzeEffectInputInfo);
      debugChecks.addToAnalyzeEffectChecklist(battleStateSetup, inputSetup, analyzeEffectInputInfo);
    }
  }

  debugChecks.checkOutputs();
}
}  // namespace pokesim
