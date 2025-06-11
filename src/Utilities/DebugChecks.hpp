#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Components/EVsIVs.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/FoeSide.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/ID.hpp>
#include <Components/Level.hpp>
#include <Components/Names/AbilityNames.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/PP.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/AssertComponentsEqual.hpp>
#include <cstddef>
#include <entt/container/dense_map.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/core/type_info.hpp>

namespace pokesim::debug {
struct Checks {
  Checks(const Simulation& _simulation) : simulation(&_simulation), registry(&_simulation.registry) {}

 protected:
  const Simulation* simulation;
  const types::registry* registry;
  types::registry registryOnInput;
  entt::dense_map<types::entity, types::entity> originalToCopy;
  entt::dense_set<types::entity> specificallyChecked;
  types::cloneIndex initialEntityCount = 0;

  template <typename T>
  bool has(types::entity entity) const {
    return registry->all_of<T>(entity);
  }

  void copyRemainingEntities() {
    for (types::entity entity : registry->view<types::entity>()) {
      if (!registry->orphan(entity)) {
        initialEntityCount++;
        if (originalToCopy.contains(entity)) {
          specificallyChecked.emplace(entity);
        }
        else {
          originalToCopy[entity] = createEntityCopy(entity, *registry, registryOnInput);
        }
      }
    }
  }

  void checkRemainingOutputs() const {
    for (auto [original, copy] : originalToCopy) {
      if (!specificallyChecked.contains(original)) {
        areEntitiesEqual(*registry, original, registryOnInput, copy);
      }
    }
  }

  types::cloneIndex getFinalEntityCount() const {
    types::cloneIndex finalEntityCount = 0;
    for (types::entity entity : registry->view<types::entity>()) {
      if (!registry->orphan(entity)) {
        finalEntityCount++;
      }
    }
    return finalEntityCount;
  }

  static void checkBoost(types::boost boost) {
    POKESIM_REQUIRE_NM(boost >= MechanicConstants::MinValues::POKEMON_STAT_BOOST);
    POKESIM_REQUIRE_NM(boost <= MechanicConstants::MaxValues::POKEMON_STAT_BOOST);
  }

  static void checkStat(types::stat stat) {
    POKESIM_REQUIRE_NM(stat >= MechanicConstants::MinValues::POKEMON_STAT);
    POKESIM_REQUIRE_NM(stat <= MechanicConstants::MaxValues::POKEMON_STAT);
  }

  static void checkEv(types::ev ev) {
    POKESIM_REQUIRE_NM(ev >= MechanicConstants::MinValues::POKEMON_EV);
    POKESIM_REQUIRE_NM(ev <= MechanicConstants::MaxValues::POKEMON_EV);
  }

  static void checkIv(types::iv iv) {
    POKESIM_REQUIRE_NM(iv >= MechanicConstants::MinValues::POKEMON_IV);
    POKESIM_REQUIRE_NM(iv <= MechanicConstants::MaxValues::POKEMON_IV);
  }

  void checkMoveSlot(types::entity moveEntity) const {
    POKESIM_REQUIRE_NM(moveEntity != entt::null);
    POKESIM_REQUIRE_NM(has<MoveName>(moveEntity));
    POKESIM_REQUIRE_NM(has<Pp>(moveEntity));
    POKESIM_REQUIRE_NM(has<MaxPp>(moveEntity));

    const auto& [move, pp, maxPp] = registry->get<MoveName, Pp, MaxPp>(moveEntity);

    POKESIM_REQUIRE_NM(move.name != dex::Move::NO_MOVE);
    POKESIM_REQUIRE_NM(pp.val >= MechanicConstants::MinValues::MOVE_PP);
    POKESIM_REQUIRE_NM(maxPp.val >= MechanicConstants::MinValues::MOVE_MAX_PP);
    POKESIM_REQUIRE_NM(pp.val <= MechanicConstants::MaxValues::MOVE_PP);
    POKESIM_REQUIRE_NM(maxPp.val <= MechanicConstants::MaxValues::MOVE_MAX_PP);
  }

  void checkPokemon(types::entity pokemonEntity) const {
    POKESIM_REQUIRE_NM(pokemonEntity != entt::null);
    POKESIM_REQUIRE_NM(has<tags::Pokemon>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<Id>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<Side>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<Battle>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<SpeciesName>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<AbilityName>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<Level>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<MoveSlots>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<Evs>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<Ivs>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<SpeciesTypes>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<stat::Hp>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<stat::Atk>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<stat::Def>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<stat::Spa>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<stat::Spd>(pokemonEntity));
    POKESIM_REQUIRE_NM(has<stat::Spe>(pokemonEntity));

    const auto& [side, battle, speciesName, speciesTypes, abilityName, level, moveSlots, evs, ivs] =
      registry->get<Side, Battle, SpeciesName, SpeciesTypes, AbilityName, Level, MoveSlots, Evs, Ivs>(pokemonEntity);

    const auto& [hp, atk, def, spa, spd, spe] =
      registry->get<stat::Hp, stat::Atk, stat::Def, stat::Spa, stat::Spd, stat::Spe>(pokemonEntity);

    POKESIM_REQUIRE_NM(has<tags::Battle>(battle.val));
    POKESIM_REQUIRE_NM(has<tags::Side>(side.val));
    POKESIM_REQUIRE_NM(speciesName.name != dex::Species::NO_SPECIES);
    POKESIM_REQUIRE_NM(speciesTypes.size() != 0);
    POKESIM_REQUIRE_NM(abilityName.name != dex::Ability::NO_ABILITY);

    POKESIM_REQUIRE_NM(level.val >= MechanicConstants::MinValues::POKEMON_LEVEL);
    POKESIM_REQUIRE_NM(level.val <= MechanicConstants::MaxValues::POKEMON_LEVEL);

    checkStat(hp.val);
    checkStat(atk.val);
    checkStat(def.val);
    checkStat(spa.val);
    checkStat(spd.val);
    checkStat(spe.val);

    checkEv(evs.hp);
    checkEv(evs.atk);
    checkEv(evs.def);
    checkEv(evs.spa);
    checkEv(evs.spd);
    checkEv(evs.spe);

    checkIv(ivs.hp);
    checkIv(ivs.atk);
    checkIv(ivs.def);
    checkIv(ivs.spa);
    checkIv(ivs.spd);
    checkIv(ivs.spe);

    POKESIM_REQUIRE_NM(moveSlots.val.size() >= MechanicConstants::MinValues::MOVE_SLOTS);
    POKESIM_REQUIRE_NM(moveSlots.val.size() <= MechanicConstants::MaxValues::MOVE_SLOTS);
  }

  void checkSide(types::entity sideEntity) const {
    POKESIM_REQUIRE_NM(sideEntity != entt::null);
    POKESIM_REQUIRE_NM(has<tags::Side>(sideEntity));
    POKESIM_REQUIRE_NM(has<Team>(sideEntity));
    POKESIM_REQUIRE_NM(has<FoeSide>(sideEntity));
    POKESIM_REQUIRE_NM(has<Battle>(sideEntity));
    POKESIM_REQUIRE_NM(has<PlayerSide>(sideEntity));

    const auto& [battle, team] = registry->get<Battle, Team>(sideEntity);
    POKESIM_REQUIRE_NM(team.val.size() >= MechanicConstants::MinValues::TEAM_SIZE);
    POKESIM_REQUIRE_NM(team.val.size() <= MechanicConstants::MaxValues::TEAM_SIZE);

    for (types::entity pokemonEntity : team.val) {
      POKESIM_REQUIRE_NM(has<tags::Pokemon>(pokemonEntity));
      POKESIM_REQUIRE_NM(has<Battle>(pokemonEntity));
      POKESIM_REQUIRE_NM(has<Side>(pokemonEntity));
      POKESIM_REQUIRE_NM(registry->get<Battle>(pokemonEntity).val == battle.val);
      POKESIM_REQUIRE_NM(registry->get<Side>(pokemonEntity).val == sideEntity);
    }
  }

  void checkBattle(types::entity battleEntity) const {
    POKESIM_REQUIRE_NM(battleEntity != entt::null);
    POKESIM_REQUIRE_NM(has<tags::Battle>(battleEntity));
    POKESIM_REQUIRE_NM(has<Sides>(battleEntity));
    POKESIM_REQUIRE_NM(has<Probability>(battleEntity));
    POKESIM_REQUIRE_NM(has<RngSeed>(battleEntity));
    const auto& [sides, probability] = registry->get<Sides, Probability>(battleEntity);

    POKESIM_REQUIRE_NM(probability.val >= MechanicConstants::MinValues::PROBABILITY);
    POKESIM_REQUIRE_NM(probability.val <= MechanicConstants::MaxValues::PROBABILITY);

    POKESIM_REQUIRE(sides.val.size() == MechanicConstants::SIDE_COUNT, "Both sides should be have entities.");
    auto [p1SideEntity, p2SideEntity] = sides.val;

    POKESIM_REQUIRE_NM(registry->get<Battle>(p1SideEntity).val == battleEntity);
    POKESIM_REQUIRE_NM(registry->get<Battle>(p2SideEntity).val == battleEntity);
    POKESIM_REQUIRE_NM(has<tags::Side>(p1SideEntity));
    POKESIM_REQUIRE_NM(has<tags::Side>(p2SideEntity));

    POKESIM_REQUIRE_NM(registry->get<PlayerSide>(p1SideEntity).val == PlayerSideId::P1);
    POKESIM_REQUIRE_NM(registry->get<PlayerSide>(p2SideEntity).val == PlayerSideId::P2);

    POKESIM_REQUIRE_NM(registry->get<FoeSide>(p1SideEntity).val == p2SideEntity);
    POKESIM_REQUIRE_NM(registry->get<FoeSide>(p2SideEntity).val == p1SideEntity);
  }
};
}  // namespace pokesim::debug

#else
namespace pokesim::debug {
struct Checks {};
}  // namespace pokesim::debug
#endif