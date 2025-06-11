#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Registry.hpp>
#include <Utilities/DebugChecks.hpp>

namespace pokesim::simulate_turn::debug {
struct Checks : pokesim::debug::Checks {
  Checks(const Simulation& _simulation) : pokesim::debug::Checks(_simulation) {}

  void checkInputs() const { check(); }

  void checkOutputs() const { check(); }

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