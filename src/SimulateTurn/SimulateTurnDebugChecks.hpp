#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/RecycledEntities.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/SimulateTurn/SimulateTurnTags.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/RecycledEntities.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Winner.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Registry.hpp>
#include <Utilities/DebugChecks.hpp>

namespace pokesim::simulate_turn::debug {
struct Checks : pokesim::debug::Checks {
  Checks(const Simulation& _simulation) : pokesim::debug::Checks(_simulation) {}

  void checkInputs() {
    const auto& options = simulateTurnOptionsOnInput;
    pokesim::debug::check(options.getDamageRollsConsidered());
    pokesim::debug::checkPercentChance(options.getRandomChanceLowerLimit());
    pokesim::debug::checkPercentChance(options.getRandomChanceUpperLimit());
    if (options.hasBranchProbabilityLowerLimit()) {
      pokesim::debug::check(Probability{options.getBranchProbabilityLowerLimit()});
    }

    copyBattles();
    check();
  }

  void checkOutputs() const {
    checkOptions();
    checkBattleOutputs();
    check();
  }

 private:
  auto getBattleView() const { return registry->view<pokesim::tags::Battle, pokesim::tags::SimulateTurn>(); }

  void copyBattles() {
    for (types::entity entity : getBattleView()) {
      copyEntity(entity);
      copyEntity(registry->get<RecycledAction>(entity).val);
    }
  }

  void checkBattleOutputs() const {
    pokesim::debug::TypesToIgnore typesToIgnore;
    typesToIgnore.add<simulate_turn::TurnOutcomeBattles, simulate_turn::tags::SpeedSortNeeded>();

    pokesim::debug::TypesToIgnore typesIgnoredOnConstants = typesToIgnore;
    typesToIgnore.add<Probability, ParentBattle, Turn, RootBattle, pokesim::tags::BattleMidTurn>();

    if (!simulateTurnOptionsOnInput.getMakeBranchesOnRandomEvents()) {
      typesToIgnore.add<RngSeed>();
    }

    for (types::entity currentEntity : getBattleView()) {
      types::entity original = pokesim::debug::findCopyParent(currentEntitiesToInitial, *registry, currentEntity);
      bool shouldNotChange = !simulateTurnOptionsOnInput.getApplyChangesToInputBattle() && original == currentEntity;
      if (!registryOnInput.all_of<Winner>(original)) {
        typesToIgnore.add<Winner>();
      }

      types::entity initialEntity = getInitialEntity(currentEntity);
      pokesim::debug::areEntitiesEqual(
        *registry,
        currentEntity,
        registryOnInput,
        initialEntity,
        shouldNotChange ? typesIgnoredOnConstants : typesToIgnore);

      bool initialIsMidTurn = registryOnInput.all_of<pokesim::tags::BattleMidTurn>(initialEntity);
      bool currentIsMidTurn = registry->all_of<pokesim::tags::BattleMidTurn>(currentEntity);
      types::entity currAction = registry->get<RecycledAction>(currentEntity).val;
      if (!initialIsMidTurn && !currentIsMidTurn) {
        pokesim::debug::areEntitiesEqual(*registry, currAction, registryOnInput, getInitialEntity(currAction));
      }

      if (!currentIsMidTurn) {
        types::registry blankRegistry;
        types::entity idealRecycledAction = blankRegistry.create();
        blankRegistry.emplace<pokesim::tags::RecycledAction>(idealRecycledAction);
        pokesim::debug::hasSameComponents(*registry, currAction, blankRegistry, idealRecycledAction);
      }
    }
  }

  void check() const {
    for (types::entity battleEntity : getBattleView()) {
      checkBattle(battleEntity);
      for (types::entity sideEntity : registry->get<Sides>(battleEntity).val) {
        checkSide(sideEntity);
        for (types::entity pokemonEntity : registry->get<Team>(sideEntity).val) {
          checkPokemon(pokemonEntity);
        }
      }
    }

    for (types::entity entity : registry->view<pokesim::tags::CurrentActionMove>()) {
      POKESIM_REQUIRE_NM(!registry->all_of<pokesim::tags::SimulateTurn>(entity));
    }
    for (types::entity entity : registry->view<pokesim::tags::FailedCurrentActionMove>()) {
      POKESIM_REQUIRE_NM(!registry->all_of<pokesim::tags::SimulateTurn>(entity));
    }
  }
};
}  // namespace pokesim::simulate_turn::debug
#else
namespace pokesim {
class Simulation;
namespace simulate_turn::debug {
struct Checks {
  Checks(const Simulation&) {}
  void checkInputs() const {}
  void checkOutputs() const {}
};
}  // namespace simulate_turn::debug
}  // namespace pokesim
#endif
