#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/SimulateTurn/SimulateTurnTags.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/SimulationTags.hpp>
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
    }
  }

  void checkBattleOutputs() const {
    pokesim::debug::TypesToIgnore typesToIgnore;
    typesToIgnore.add<simulate_turn::TurnOutcomeBattles, simulate_turn::tags::SpeedSortNeeded>();

    pokesim::debug::TypesToIgnore typesIgnoredOnConstants = typesToIgnore;
    typesToIgnore.add<Probability, ParentBattle, Turn, RootBattle>();

    if (!simulateTurnOptionsOnInput.getMakeBranchesOnRandomEvents()) {
      typesToIgnore.add<RngSeed>();
    }

    for (types::entity entity : getBattleView()) {
      types::entity original = pokesim::debug::findCopyParent(currentEntitiesToInitial, *registry, entity);
      bool shouldNotChange = !simulateTurnOptionsOnInput.getApplyChangesToInputBattle() && original == entity;
      if (!registryOnInput.all_of<Winner>(original)) {
        typesToIgnore.add<Winner>();
      }

      pokesim::debug::areEntitiesEqual(
        *registry,
        entity,
        registryOnInput,
        getInitialEntity(entity),
        shouldNotChange ? typesIgnoredOnConstants : typesToIgnore);
    }
  }

  void check() const {
    for (types::entity battleEntity : getBattleView()) {
      checkBattle(battleEntity);
      for (types::entity sideEntity : registry->get<Sides>(battleEntity).val) {
        checkSide(sideEntity);
        for (types::entity pokemonEntity : registry->get<Team>(sideEntity).val) {
          checkPokemon(pokemonEntity);
          for (types::entity moveEntity : registry->get<MoveSlots>(pokemonEntity).val) {
            checkMoveSlot(moveEntity);
          }
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
