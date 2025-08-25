#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <AnalyzeEffect/Setup/AnalyzeEffectInputSetup.hpp>
#include <Battle/Setup/BattleStateSetup.hpp>
#include <CalcDamage/Setup/CalcDamageInputSetup.hpp>
#include <Components/AnalyzeEffect/Aliases.hpp>
#include <Components/AnalyzeEffect/AnalyzeEffectInputs.hpp>
#include <Components/Boosts.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/FoeSide.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/ID.hpp>
#include <Components/Level.hpp>
#include <Components/Names/AbilityNames.hpp>
#include <Components/Names/GenderNames.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/NatureNames.hpp>
#include <Components/Names/PseudoWeatherNames.hpp>
#include <Components/Names/SideConditionNames.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/Names/TerrainNames.hpp>
#include <Components/Names/VolatileNames.hpp>
#include <Components/Names/WeatherNames.hpp>
#include <Components/PP.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Turn.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/PseudoWeather.hpp>
#include <Types/Enums/SideCondition.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Enums/Terrain.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Enums/Weather.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <Utilities/DebugChecks.hpp>
#include <cstddef>

#include "Simulation.hpp"

namespace pokesim::debug {
struct SimulationSetupChecks {
 private:
  const types::registry* registry;
  const std::vector<Simulation::BattleCreationInfo>* battleInfoList;

  struct SetupEntities {
    types::entity battle;
    types::entity setup;
  };

  entt::dense_map<const Simulation::BattleCreationInfo*, types::entityVector> createdBattles;
  entt::dense_map<const Simulation::TurnDecisionInfo*, types::entity> createdTurnDecisions;

  entt::dense_map<const Simulation::CalcDamageInputInfo*, SetupEntities> createdCalcDamageInputs;
  entt::dense_map<const Simulation::AnalyzeEffectInputInfo*, SetupEntities> createdAnalyzeEffectInputs;

  static types::entity targetSlotToEntity(
    Slot targetSlot, const types::teamPositions<types::entity>& p1Team,
    const types::teamPositions<types::entity>& p2Team) {
    switch (targetSlot) {
      case Slot::P1A: {
        return p1Team[0];
      }
      case Slot::P1B: {
        return p1Team[1];
      }
      case Slot::P1C: {
        return p1Team[2];
      }
      case Slot::P1D: {
        return p1Team[3];
      }
      case Slot::P1E: {
        return p1Team[4];
      }
      case Slot::P1F: {
        return p1Team[5];
      }
      case Slot::P2A: {
        return p2Team[0];
      }
      case Slot::P2B: {
        return p2Team[1];
      }
      case Slot::P2C: {
        return p2Team[2];
      }
      case Slot::P2D: {
        return p2Team[3];
      }
      case Slot::P2E: {
        return p2Team[4];
      }
      case Slot::P2F: {
        return p2Team[5];
      }
      default: break;
    }

    POKESIM_REQUIRE_FAIL("Given target slot does not exist in a team.");
    return entt::null;
  }

  void checkCreatedPokemon(types::entity pokemonEntity, const Simulation::PokemonCreationInfo& creationInfo) const {
    const auto& [id, side, battle, speciesName, abilityName, level, moveSlots, evs, ivs] =
      registry->get<Id, Side, Battle, SpeciesName, AbilityName, Level, MoveSlots, Evs, Ivs>(pokemonEntity);
    const auto& [hp, atk, def, spa, spd, spe] =
      registry->get<stat::Hp, stat::Atk, stat::Def, stat::Spa, stat::Spd, stat::Spe>(pokemonEntity);

    if (creationInfo.id.has_value()) {
      POKESIM_REQUIRE_NM(id.val == creationInfo.id.value());
    }
    else {
      POKESIM_REQUIRE_NM(id.val != 0U);
    }

    POKESIM_REQUIRE_NM(speciesName.name == creationInfo.species);
    POKESIM_REQUIRE_NM(abilityName.name == creationInfo.ability);
    POKESIM_REQUIRE_NM(level.val == creationInfo.level);

    if (creationInfo.item == dex::Item::NO_ITEM) {
      POKESIM_REQUIRE_NM(!registry->all_of<ItemName>(pokemonEntity));
    }
    else {
      POKESIM_REQUIRE_NM(registry->get<ItemName>(pokemonEntity).name == creationInfo.item);
    }

    if (creationInfo.gender == dex::Gender::NO_GENDER) {
      POKESIM_REQUIRE_NM(!registry->all_of<GenderName>(pokemonEntity));
    }
    else {
      POKESIM_REQUIRE_NM(registry->get<GenderName>(pokemonEntity).name == creationInfo.gender);
    }

    if (creationInfo.status == dex::Status::NO_STATUS) {
      POKESIM_REQUIRE_NM(!registry->all_of<StatusName>(pokemonEntity));
    }
    else {
      POKESIM_REQUIRE_NM(registry->get<StatusName>(pokemonEntity).name == creationInfo.status);
    }

    if (creationInfo.nature == dex::Nature::NO_NATURE) {
      POKESIM_REQUIRE_NM(!registry->all_of<NatureName>(pokemonEntity));
    }
    else {
      POKESIM_REQUIRE_NM(registry->get<NatureName>(pokemonEntity).name == creationInfo.nature);
    }

    POKESIM_REQUIRE_NM(hp.val == creationInfo.stats.hp);
    POKESIM_REQUIRE_NM(atk.val == creationInfo.stats.atk);
    POKESIM_REQUIRE_NM(def.val == creationInfo.stats.def);
    POKESIM_REQUIRE_NM(spa.val == creationInfo.stats.spa);
    POKESIM_REQUIRE_NM(spd.val == creationInfo.stats.spd);
    POKESIM_REQUIRE_NM(spe.val == creationInfo.stats.spe);

    POKESIM_REQUIRE_NM(evs.hp == creationInfo.evs.hp);
    POKESIM_REQUIRE_NM(evs.atk == creationInfo.evs.atk);
    POKESIM_REQUIRE_NM(evs.def == creationInfo.evs.def);
    POKESIM_REQUIRE_NM(evs.spa == creationInfo.evs.spa);
    POKESIM_REQUIRE_NM(evs.spd == creationInfo.evs.spd);
    POKESIM_REQUIRE_NM(evs.spe == creationInfo.evs.spe);

    POKESIM_REQUIRE_NM(ivs.hp == creationInfo.ivs.hp);
    POKESIM_REQUIRE_NM(ivs.atk == creationInfo.ivs.atk);
    POKESIM_REQUIRE_NM(ivs.def == creationInfo.ivs.def);
    POKESIM_REQUIRE_NM(ivs.spa == creationInfo.ivs.spa);
    POKESIM_REQUIRE_NM(ivs.spd == creationInfo.ivs.spd);
    POKESIM_REQUIRE_NM(ivs.spe == creationInfo.ivs.spe);

    POKESIM_REQUIRE_NM(moveSlots.val.size() == creationInfo.moves.size());

    for (std::size_t i = 0U; i < creationInfo.moves.size(); i++) {
      const Simulation::MoveCreationInfo& move = creationInfo.moves[i];
      types::entity moveEntity = moveSlots.val[(types::moveSlotIndex)i];
      POKESIM_REQUIRE_NM((registry->all_of<MoveName, Pp, MaxPp>(moveEntity)));
      POKESIM_REQUIRE_NM(registry->get<MoveName>(moveEntity).name == move.name);
      POKESIM_REQUIRE_NM(registry->get<Pp>(moveEntity).val == move.pp);
      POKESIM_REQUIRE_NM(registry->get<MaxPp>(moveEntity).val == move.maxPp);
      pokesim::debug::checkMoveSlot(moveEntity, *registry);
    }

    if (creationInfo.currentHp.has_value()) {
      POKESIM_REQUIRE_NM(registry->get<stat::CurrentHp>(pokemonEntity).val == creationInfo.currentHp);
      if (creationInfo.currentHp == MechanicConstants::PokemonCurrentHpStat::MIN) {
        POKESIM_REQUIRE_NM(registry->all_of<tags::Fainted>(pokemonEntity));
      }
      else {
        POKESIM_REQUIRE_NM(!registry->all_of<tags::Fainted>(pokemonEntity));
      }
    }
    else {
      POKESIM_REQUIRE_NM(registry->get<stat::CurrentHp>(pokemonEntity).val == hp.val);
      POKESIM_REQUIRE_NM(!registry->all_of<tags::Fainted>(pokemonEntity));
    }

    if (creationInfo.currentTypes.has_value()) {
      POKESIM_REQUIRE_NM(registry->get<SpeciesTypes>(pokemonEntity) == creationInfo.currentTypes);
    }

    const auto& currentBoosts = creationInfo.currentBoosts;
    if (currentBoosts.atk.has_value()) {
      POKESIM_REQUIRE_NM(registry->get<AtkBoost>(pokemonEntity).val == currentBoosts.atk.value());
    }
    if (currentBoosts.def.has_value()) {
      POKESIM_REQUIRE_NM(registry->get<DefBoost>(pokemonEntity).val == currentBoosts.def.value());
    }
    if (currentBoosts.spa.has_value()) {
      POKESIM_REQUIRE_NM(registry->get<SpaBoost>(pokemonEntity).val == currentBoosts.spa.value());
    }
    if (currentBoosts.spd.has_value()) {
      POKESIM_REQUIRE_NM(registry->get<SpdBoost>(pokemonEntity).val == currentBoosts.spd.value());
    }
    if (currentBoosts.spe.has_value()) {
      POKESIM_REQUIRE_NM(registry->get<SpeBoost>(pokemonEntity).val == currentBoosts.spe.value());
    }

    pokesim::debug::checkPokemon(pokemonEntity, *registry);
  }

  void checkCreatedSide(types::entity sideEntity, const Simulation::SideCreationInfo& creationInfo) const {
    POKESIM_REQUIRE_NM((registry->all_of<Team, FoeSide, Battle>(sideEntity)));

    const auto& team = registry->get<Team>(sideEntity).val;
    POKESIM_REQUIRE_NM(team.size() == creationInfo.team.size());

    for (std::size_t i = 0U; i < creationInfo.team.size(); i++) {
      types::entity pokemonEntity = team[(types::teamPositionIndex)i];
      checkCreatedPokemon(pokemonEntity, creationInfo.team[i]);

      POKESIM_REQUIRE_NM(registry->get<Side>(pokemonEntity).val == sideEntity);
      POKESIM_REQUIRE_NM(registry->get<Battle>(pokemonEntity).val == registry->get<Battle>(sideEntity).val);
    }

    pokesim::debug::checkSide(sideEntity, *registry);
  }

  void checkBattle(types::entity battleEntity, const Simulation::BattleCreationInfo& creationInfo) const {
    POKESIM_REQUIRE_NM((registry->all_of<Sides, ActionQueue, Turn, Probability, RngSeed>(battleEntity)));
    const auto& [sides, turn, probability, rngSeed] = registry->get<Sides, Turn, Probability, RngSeed>(battleEntity);

    POKESIM_REQUIRE_NM(turn.val == creationInfo.turn);
    POKESIM_REQUIRE_NM(probability.val == creationInfo.probability);

    if (creationInfo.rngSeed) {
      POKESIM_REQUIRE_NM(rngSeed.val == creationInfo.rngSeed);
    }
    else {
      POKESIM_REQUIRE_NM(rngSeed.val != 0U);
    }

    POKESIM_REQUIRE(sides.val.size() == MechanicConstants::SIDE_COUNT, "Both sides should be have entities.");
    auto [p1SideEntity, p2SideEntity] = sides.val;

    checkCreatedSide(p1SideEntity, creationInfo.p1);
    checkCreatedSide(p2SideEntity, creationInfo.p2);

    POKESIM_REQUIRE_NM(registry->get<Battle>(p1SideEntity).val == battleEntity);
    POKESIM_REQUIRE_NM(registry->get<Battle>(p2SideEntity).val == battleEntity);
    POKESIM_REQUIRE_NM(registry->get<FoeSide>(p1SideEntity).val == p2SideEntity);
    POKESIM_REQUIRE_NM(registry->get<FoeSide>(p2SideEntity).val == p1SideEntity);

    pokesim::debug::checkBattle(battleEntity, *registry);
  }

  void checkTurnDecision(types::entity battleEntity, const Simulation::TurnDecisionInfo& turnDecisionInfo) const {
    const auto& sides = registry->get<Sides>(battleEntity).val;

    POKESIM_REQUIRE(sides.size() == MechanicConstants::SIDE_COUNT, "Both sides should be have entities.");
    for (std::uint8_t side = 0U; side < MechanicConstants::SIDE_COUNT; side++) {
      const auto& sideDecision = registry->get<SideDecision>(sides[side]);
      const auto& sideDecisionInfo = side ? turnDecisionInfo.p2 : turnDecisionInfo.p1;

      POKESIM_REQUIRE_NM(sideDecision.sideId == sideDecisionInfo.sideId);

      if (sideDecisionInfo.decisions.holds<types::slotDecisions>()) {
        POKESIM_REQUIRE_NM(sideDecision.decisions.holds<types::slotDecisions>());

        const auto& slotDecisions = sideDecision.decisions.get<types::slotDecisions>();
        const auto& slotDecisionsInfo = sideDecisionInfo.decisions.get<types::slotDecisions>();
        POKESIM_REQUIRE_NM(slotDecisions.size() == slotDecisionsInfo.size());
        POKESIM_REQUIRE_NM(!slotDecisions.empty());
        for (std::uint8_t slot = 0U; slot < slotDecisions.size(); slot++) {
          const auto& slotDecision = slotDecisions[slot];
          const auto& slotDecisionInfo = slotDecisionsInfo[slot];

          POKESIM_REQUIRE_NM(slotDecision.sourceSlot == slotDecisionInfo.sourceSlot);
          POKESIM_REQUIRE_NM(slotDecision.targetSlot == slotDecisionInfo.targetSlot);
          POKESIM_REQUIRE_NM(slotDecision.megaEvolve == slotDecisionInfo.megaEvolve);
          POKESIM_REQUIRE_NM(slotDecision.primalRevert == slotDecisionInfo.primalRevert);
          POKESIM_REQUIRE_NM(slotDecision.dynamax == slotDecisionInfo.dynamax);
          POKESIM_REQUIRE_NM(slotDecision.terastallize == slotDecisionInfo.terastallize);
          POKESIM_REQUIRE_NM(slotDecision.moveChoice == slotDecisionInfo.moveChoice);
          POKESIM_REQUIRE_NM(slotDecision.itemChoice == slotDecisionInfo.itemChoice);
        }
      }
      else if (sideDecisionInfo.decisions.holds<types::teamOrder>()) {
        POKESIM_REQUIRE_NM(sideDecision.decisions.holds<types::teamOrder>());

        const auto& teamOrder = sideDecision.decisions.get<types::teamOrder>();
        const auto& teamOrderInfo = sideDecisionInfo.decisions.get<types::teamOrder>();
        POKESIM_REQUIRE_NM(teamOrder.size() == teamOrderInfo.size());
        POKESIM_REQUIRE_NM(!teamOrder.empty());

        for (std::uint8_t position = 0U; position < teamOrder.size(); position++) {
          POKESIM_REQUIRE_NM(teamOrder[position] == teamOrderInfo[position]);
        }
      }
      else {
        POKESIM_REQUIRE_FAIL(
          "Decision kind of index " + std::to_string(sideDecisionInfo.decisions.index()) + "not implemented yet.");
      }

      pokesim::debug::check(sideDecision);
    }
  }

  void checkCalcDamage(
    types::entity battleEntity, types::entity calcDamageEntity,
    const Simulation::CalcDamageInputInfo& calcDamageInputInfo) const {
    POKESIM_REQUIRE_NM(registry->all_of<calc_damage::tags::UsedMove>(calcDamageEntity));
    POKESIM_REQUIRE_NM(registry->all_of<tags::CalculateDamage>(calcDamageEntity));

    const auto& p1Team = registry->get<Team>(registry->get<Sides>(battleEntity).p1()).val;
    const auto& p2Team = registry->get<Team>(registry->get<Sides>(battleEntity).p2()).val;
    const auto& [battle, moveName, attacker, defenders] =
      registry->get<Battle, MoveName, calc_damage::Attacker, calc_damage::Defenders>(calcDamageEntity);
    types::entity setupInfoAttacker = targetSlotToEntity(calcDamageInputInfo.attackerSlot, p1Team, p2Team);
    types::entity setupInfoDefender = targetSlotToEntity(calcDamageInputInfo.defenderSlot, p1Team, p2Team);

    POKESIM_REQUIRE_NM(battle.val == battleEntity);
    POKESIM_REQUIRE_NM(attacker.val == setupInfoAttacker);
    POKESIM_REQUIRE_NM(defenders.only() == setupInfoDefender);
    POKESIM_REQUIRE_NM(
      std::find(calcDamageInputInfo.moves.begin(), calcDamageInputInfo.moves.end(), moveName.name) !=
      calcDamageInputInfo.moves.end());

    POKESIM_REQUIRE_NM(registry->all_of<calc_damage::tags::Attacker>(setupInfoAttacker));
    POKESIM_REQUIRE_NM(registry->all_of<calc_damage::tags::Defender>(setupInfoDefender));

    const auto& attackerMoves = registry->get<calc_damage::UsedMovesAsAttacker>(setupInfoAttacker).val;
    const auto& defenderMoves = registry->get<calc_damage::UsedMovesAsDefender>(setupInfoDefender).val;

    POKESIM_REQUIRE_NM(std::find(attackerMoves.begin(), attackerMoves.end(), calcDamageEntity) != attackerMoves.end());
    POKESIM_REQUIRE_NM(std::find(defenderMoves.begin(), defenderMoves.end(), calcDamageEntity) != defenderMoves.end());

    checkActionMove(calcDamageEntity, *registry);
  }

  void checkAnalyzeEffect(
    types::entity battleEntity, types::entity analyzeEffectEntity,
    const Simulation::AnalyzeEffectInputInfo& analyzeEffectInputInfo) const {
    POKESIM_REQUIRE_NM(registry->all_of<analyze_effect::tags::Input>(analyzeEffectEntity));
    POKESIM_REQUIRE(
      !registry->all_of<tags::AnalyzeEffect>(analyzeEffectEntity),
      "This should not be set on the input entity as it's used for individual move calculations.");

    const auto& p1Team = registry->get<Team>(registry->get<Sides>(battleEntity).p1()).val;
    const auto& p2Team = registry->get<Team>(registry->get<Sides>(battleEntity).p2()).val;
    const auto& [battle, effectMove, attacker, defenders, effectTarget] = registry->get<
      Battle,
      analyze_effect::EffectMove,
      analyze_effect::Attacker,
      analyze_effect::Defenders,
      analyze_effect::EffectTarget>(analyzeEffectEntity);
    types::entity setupInfoAttacker = targetSlotToEntity(analyzeEffectInputInfo.attackerSlot, p1Team, p2Team);
    types::entity setupInfoDefender = targetSlotToEntity(analyzeEffectInputInfo.defenderSlot, p1Team, p2Team);
    types::entity setupInfoEffectTarget = targetSlotToEntity(analyzeEffectInputInfo.effectTarget, p1Team, p2Team);

    POKESIM_REQUIRE_NM(battle.val == battleEntity);
    POKESIM_REQUIRE_NM(attacker.val == setupInfoAttacker);
    POKESIM_REQUIRE_NM(defenders.only() == setupInfoDefender);
    POKESIM_REQUIRE_NM(effectTarget.val == setupInfoEffectTarget);

    POKESIM_REQUIRE_NM(
      std::find(analyzeEffectInputInfo.moves.begin(), analyzeEffectInputInfo.moves.end(), effectMove.val) !=
      analyzeEffectInputInfo.moves.end());

    POKESIM_REQUIRE_NM(registry->all_of<analyze_effect::Inputs>(battle.val));
    const auto& battleInputs = registry->get<analyze_effect::Inputs>(battle.val).val;
    POKESIM_REQUIRE_NM(std::find(battleInputs.begin(), battleInputs.end(), analyzeEffectEntity) != battleInputs.end());

    if (analyzeEffectInputInfo.effect.has_value()) {
      const auto& effect = analyzeEffectInputInfo.effect.value();
      if (effect.holds<dex::PseudoWeather>()) {
        POKESIM_REQUIRE_NM(registry->all_of<PseudoWeatherName>(analyzeEffectEntity));
        POKESIM_REQUIRE_NM(
          registry->get<PseudoWeatherName>(analyzeEffectEntity).name == effect.get<dex::PseudoWeather>());
      }
      else if (effect.holds<dex::SideCondition>()) {
        POKESIM_REQUIRE_NM(registry->all_of<SideConditionName>(analyzeEffectEntity));
        POKESIM_REQUIRE_NM(
          registry->get<SideConditionName>(analyzeEffectEntity).name == effect.get<dex::SideCondition>());
      }
      else if (effect.holds<dex::Status>()) {
        POKESIM_REQUIRE_NM(registry->all_of<StatusName>(analyzeEffectEntity));
        POKESIM_REQUIRE_NM(registry->get<StatusName>(analyzeEffectEntity).name == effect.get<dex::Status>());
      }
      else if (effect.holds<dex::Terrain>()) {
        POKESIM_REQUIRE_NM(registry->all_of<TerrainName>(analyzeEffectEntity));
        POKESIM_REQUIRE_NM(registry->get<TerrainName>(analyzeEffectEntity).name == effect.get<dex::Terrain>());
      }
      else if (effect.holds<dex::Volatile>()) {
        POKESIM_REQUIRE_NM(registry->all_of<VolatileName>(analyzeEffectEntity));
        POKESIM_REQUIRE_NM(registry->get<VolatileName>(analyzeEffectEntity).name == effect.get<dex::Volatile>());
      }
      else if (effect.holds<dex::Weather>()) {
        POKESIM_REQUIRE_NM(registry->all_of<WeatherName>(analyzeEffectEntity));
        POKESIM_REQUIRE_NM(registry->get<WeatherName>(analyzeEffectEntity).name == effect.get<dex::Weather>());
      }
      else {
        POKESIM_REQUIRE_FAIL("Effect does not contain a valid enum.");
      }
    }

    if (analyzeEffectInputInfo.boostEffect.has_value()) {
      const auto [stat, boostValue] = analyzeEffectInputInfo.boostEffect.value();
      switch (stat) {
        case dex::Stat::ATK: {
          POKESIM_REQUIRE_NM(registry->all_of<AtkBoost>(analyzeEffectEntity));
          POKESIM_REQUIRE_NM(registry->get<AtkBoost>(analyzeEffectEntity).val == boostValue);
          break;
        }
        case dex::Stat::DEF: {
          POKESIM_REQUIRE_NM(registry->all_of<DefBoost>(analyzeEffectEntity));
          POKESIM_REQUIRE_NM(registry->get<DefBoost>(analyzeEffectEntity).val == boostValue);
          break;
        }
        case dex::Stat::SPA: {
          POKESIM_REQUIRE_NM(registry->all_of<SpaBoost>(analyzeEffectEntity));
          POKESIM_REQUIRE_NM(registry->get<SpaBoost>(analyzeEffectEntity).val == boostValue);
          break;
        }
        case dex::Stat::SPD: {
          POKESIM_REQUIRE_NM(registry->all_of<SpdBoost>(analyzeEffectEntity));
          POKESIM_REQUIRE_NM(registry->get<SpdBoost>(analyzeEffectEntity).val == boostValue);
          break;
        }
        case dex::Stat::SPE: {
          POKESIM_REQUIRE_NM(registry->all_of<SpeBoost>(analyzeEffectEntity));
          POKESIM_REQUIRE_NM(registry->get<SpeBoost>(analyzeEffectEntity).val == boostValue);
          break;
        }
        default: {
          POKESIM_REQUIRE_FAIL("Using a stat enum that doesn't have boost.");
        }
      }
    }
  }

 public:
  SimulationSetupChecks(
    const Simulation* simulation, const std::vector<Simulation::BattleCreationInfo>& _battleInfoList)
      : registry(&simulation->registry), battleInfoList(&_battleInfoList) {}

  void checkOutputs() const {
    POKESIM_REQUIRE_NM(battleInfoList->size() <= internal::maxSizedVector<Simulation::BattleCreationInfo>::max());
    for (const auto& battleInfo : *battleInfoList) {
      POKESIM_REQUIRE_NM(createdBattles.contains(&battleInfo));
      const types::entityVector& battleEntities = createdBattles.at(&battleInfo);

      std::size_t idealBattleCount =
        battleInfo.decisionsToSimulate.empty() ? 1U : battleInfo.decisionsToSimulate.size();
      POKESIM_REQUIRE_NM(idealBattleCount == battleEntities.size());
      for (types::entity entity : battleEntities) {
        checkBattle(entity, battleInfo);
      }

      for (const auto& turnDecisionInfo : battleInfo.decisionsToSimulate) {
        POKESIM_REQUIRE_NM(createdTurnDecisions.contains(&turnDecisionInfo));
        types::entity battleEntity = createdTurnDecisions.at(&turnDecisionInfo);
        checkTurnDecision(battleEntity, turnDecisionInfo);
      }

      for (const auto& calcDamageInputInfo : battleInfo.damageCalculations) {
        POKESIM_REQUIRE_NM(createdCalcDamageInputs.contains(&calcDamageInputInfo));
        auto [battleEntity, setupEntity] = createdCalcDamageInputs.at(&calcDamageInputInfo);
        checkCalcDamage(battleEntity, setupEntity, calcDamageInputInfo);
      }

      analyze_effect::Inputs analyzeEffectInputs;
      for (const auto& analyzeEffectInputInfo : battleInfo.effectsToAnalyze) {
        POKESIM_REQUIRE_NM(createdAnalyzeEffectInputs.contains(&analyzeEffectInputInfo));
        auto [battleEntity, setupEntity] = createdAnalyzeEffectInputs.at(&analyzeEffectInputInfo);
        checkAnalyzeEffect(battleEntity, setupEntity, analyzeEffectInputInfo);
        analyzeEffectInputs.val.push_back(setupEntity);
      }
      pokesim::debug::check(analyzeEffectInputs, *registry);
    }
  }

  void addToBattleChecklist(
    const BattleStateSetup& battleStateSetup, const Simulation::BattleCreationInfo& creationInfo) {
    createdBattles[&creationInfo].push_back(battleStateSetup.entity());
  }

  void addToTurnDecisionChecklist(
    const BattleStateSetup& battleStateSetup, const Simulation::TurnDecisionInfo& turnDecisionInfo) {
    createdTurnDecisions[&turnDecisionInfo] = battleStateSetup.entity();
  }

  void addToCalcDamageChecklist(
    const BattleStateSetup& battleStateSetup, const calc_damage::InputSetup& inputSetup,
    const Simulation::CalcDamageInputInfo& calcDamageInputInfo) {
    createdCalcDamageInputs[&calcDamageInputInfo] = {battleStateSetup.entity(), inputSetup.entity()};
  }

  void addToAnalyzeEffectChecklist(
    const BattleStateSetup& battleStateSetup, const analyze_effect::InputSetup& inputSetup,
    const Simulation::AnalyzeEffectInputInfo& analyzeEffectInputInfo) {
    createdAnalyzeEffectInputs[&analyzeEffectInputInfo] = {battleStateSetup.entity(), inputSetup.entity()};
  }

  static void checkBattle(
    const Simulation& simulation, types::entity battleEntity,
    const Simulation::BattleCreationInfo& battleCreationInfo) {
    SimulationSetupChecks(&simulation, {}).checkBattle(battleEntity, battleCreationInfo);
  }
};
}  // namespace pokesim::debug
#else

#include "Simulation.hpp"

namespace pokesim {
namespace debug {
struct SimulationSetupChecks {
  SimulationSetupChecks(const Simulation*, const std::vector<Simulation::BattleCreationInfo>&) {}
  void checkOutputs() const {}
  void addToBattleChecklist(const BattleStateSetup&, const Simulation::BattleCreationInfo&) const {}
  void addToTurnDecisionChecklist(const BattleStateSetup&, const Simulation::TurnDecisionInfo&) const {}
  void addToCalcDamageChecklist(
    const BattleStateSetup&, const calc_damage::InputSetup&, const Simulation::CalcDamageInputInfo&) const {}
  void addToAnalyzeEffectChecklist(
    const BattleStateSetup&, const analyze_effect::InputSetup&, const Simulation::AnalyzeEffectInputInfo&) const {}
  static void checkBattle(const Simulation&, types::entity, const Simulation::BattleCreationInfo&) {}
};
}  // namespace debug
}  // namespace pokesim
#endif