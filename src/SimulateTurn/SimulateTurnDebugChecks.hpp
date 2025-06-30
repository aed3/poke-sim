#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Registry.hpp>
#include <Utilities/DebugChecks.hpp>

namespace pokesim::simulate_turn::debug {
struct Checks : pokesim::debug::Checks {
  Options options;
  Checks(const Simulation& _simulation)
      : pokesim::debug::Checks(_simulation), options(_simulation.simulateTurnOptions) {}

  void checkInputs() const {
    pokesim::debug::check(options.damageRollsConsidered);
    if (options.randomChanceLowerLimit.has_value()) {
      pokesim::debug::check(RandomBinaryChance{options.randomChanceLowerLimit.value()});
    }
    if (options.randomChanceUpperLimit.has_value()) {
      pokesim::debug::check(RandomBinaryChance{options.randomChanceUpperLimit.value()});
    }
    if (options.branchProbabilityLowerLimit.has_value()) {
      pokesim::debug::check(Probability{options.branchProbabilityLowerLimit.value()});
    }

    check();
  }

  void checkOutputs() const {
    POKESIM_REQUIRE_NM(options == simulation->simulateTurnOptions);

    check();
  }

 private:
  void check() const {
    for (types::entity battleEntity : simulation->selectedBattleEntities()) {
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