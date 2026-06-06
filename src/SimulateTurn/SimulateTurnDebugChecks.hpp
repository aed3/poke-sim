#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Registry.hpp>
#include <Utilities/DebugChecks.hpp>

namespace pokesim::simulate_turn::debug {
struct Checks : pokesim::debug::Checks {
  Options options;
  Checks(const Simulation& _simulation)
      : pokesim::debug::Checks(_simulation), options(_simulation.simulateTurnOptions) {}

  void checkInputs() const {
    pokesim::debug::check(options.getDamageRollsConsidered());
    pokesim::debug::checkPercentChance(options.getRandomChanceLowerLimit());
    pokesim::debug::checkPercentChance(options.getRandomChanceUpperLimit());
    if (options.hasBranchProbabilityLowerLimit()) {
      pokesim::debug::check(Probability{options.getBranchProbabilityLowerLimit()});
    }

    check();
  }

  void checkOutputs() const {
    POKESIM_REQUIRE_NM(options == simulation->simulateTurnOptions);

    check();
  }

 private:
  void check() const {
    for (types::entity battleEntity : simulation->battleEntities()) {
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
