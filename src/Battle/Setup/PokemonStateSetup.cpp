#include "PokemonStateSetup.hpp"

#include <Components/EVsIVs.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/ID.hpp>
#include <Components/Level.hpp>
#include <Components/Names/AbilityNames.hpp>
#include <Components/Names/GenderNames.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Names/NatureNames.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/Names/StatusNames.hpp>
#include <Components/Position.hpp>
#include <Components/Tags/AbilityTags.hpp>
#include <Components/Tags/ItemTags.hpp>
#include <Components/Tags/NatureTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Gender.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Nature.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/State.hpp>
#include <Types/Stats.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/view.hpp>

namespace pokesim {
void PokemonStateSetup::initBlank() {
  handle.emplace<SpeciesName>();
  handle.emplace<Side>();
  handle.emplace<Battle>();
  setAutoID();
}

void PokemonStateSetup::setAutoID() {
  setID((uint16_t)handle.registry()->view<SpeciesName>().size() + 1);
}

void PokemonStateSetup::setID(types::stateId id) {
  handle.emplace<Id>(id);
}

void PokemonStateSetup::setSpecies(dex::Species speciesName) {
  handle.emplace<SpeciesName>(speciesName);
}

void PokemonStateSetup::setSide(types::entity entity) {
  handle.emplace<Side>(entity);
}

void PokemonStateSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
}

void PokemonStateSetup::setLevel(types::level level) {
  handle.emplace<Level>(level);
}

void PokemonStateSetup::setGender(dex::Gender gender) {
  handle.emplace<GenderName>(gender);
}

void PokemonStateSetup::setAbility(dex::Ability ability) {
  handle.emplace<AbilityName>(ability);
  ability::tags::enumToTag(ability, handle);
}

void PokemonStateSetup::setItem(dex::Item item) {
  handle.emplace<ItemName>(item);
  item::tags::enumToTag(item, handle);
}

void PokemonStateSetup::setMoves(const std::vector<types::entity>& moveSlots) {
  MoveSlots& moveEntities = handle.emplace<MoveSlots>();
  ENTT_ASSERT(
    moveSlots.size() <= moveEntities.moveSlots.max_size(),
    "Cannot add more moves to a Pokemon than types::internal::MAX_MOVE_SLOTS");
  for (types::entity moveSlot : moveSlots) {
    moveEntities.moveSlots.push_back(moveSlot);
  }
}

void PokemonStateSetup::setPostion(types::teamPositionIndex position) {
  handle.emplace<Position>(position);
}

void PokemonStateSetup::setStatus(dex::Status status) {
  handle.emplace<StatusName>(status);
  status::tags::enumToTag(status, handle);
}

void PokemonStateSetup::setNature(dex::Nature nature) {
  handle.emplace<NatureName>(nature);
  nature::tags::enumToTag(nature, handle);
}

void PokemonStateSetup::setEVs(
  types::ev hp, types::ev atk, types::ev def, types::ev spa, types::ev spd, types::ev spe) {
  handle.emplace<Evs>(hp, atk, def, spa, spd, spe);
}

void PokemonStateSetup::setEVs(const Evs& evs) {
  handle.emplace<Evs>(evs);
}

void PokemonStateSetup::setIVs(
  types::iv hp, types::iv atk, types::iv def, types::iv spa, types::iv spd, types::iv spe) {
  handle.emplace<Ivs>(hp, atk, def, spa, spd, spe);
}

void PokemonStateSetup::setIVs(const Ivs& ivs) {
  handle.emplace<Ivs>(ivs);
}
}  // namespace pokesim