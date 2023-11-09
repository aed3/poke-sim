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
#include <Types/Ability.hpp>
#include <Types/Gender.hpp>
#include <Types/Item.hpp>
#include <Types/Nature.hpp>
#include <Types/Species.hpp>
#include <Types/Status.hpp>
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

void PokemonStateSetup::setID(std::uint16_t id) {
  handle.emplace<ID>(id);
}

void PokemonStateSetup::setSpecies(dex::Species speciesName) {
  handle.emplace<SpeciesName>(speciesName);
}

void PokemonStateSetup::setSide(entt::entity entity) {
  handle.emplace<Side>(entity);
}

void PokemonStateSetup::setBattle(entt::entity entity) {
  handle.emplace<Battle>(entity);
}

void PokemonStateSetup::setLevel(std::uint8_t level) {
  handle.emplace<Level>(level);
}

void PokemonStateSetup::setGender(dex::Gender gender) {
  handle.emplace<GenderName>(gender);
}

void PokemonStateSetup::setAbility(dex::Ability ability) {
  handle.emplace<AbilityName>(ability);
  tags::ability::enumToTag(ability, handle);
}

void PokemonStateSetup::setItem(dex::Item item) {
  handle.emplace<ItemName>(item);
  tags::item::enumToTag(item, handle);
}

void PokemonStateSetup::setMoves(const std::vector<entt::entity>& moveSlots) {
  handle.emplace<MoveSlots>(moveSlots);
}

void PokemonStateSetup::setPostion(std::uint8_t position) {
  handle.emplace<Position>(position);
}

void PokemonStateSetup::setStatus(dex::Status status) {
  handle.emplace<StatusName>(status);
  tags::status::enumToTag(status, handle);
}

void PokemonStateSetup::setNature(dex::Nature nature) {
  handle.emplace<NatureName>(nature);
  tags::nature::enumToTag(nature, handle);
}

void PokemonStateSetup::setEVs(
  std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe) {
  handle.emplace<EVs>(hp, atk, def, spa, spd, spe);
}

void PokemonStateSetup::setEVs(const EVs& evs) {
  handle.emplace<EVs>(evs);
}

void PokemonStateSetup::setIVs(
  std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe) {
  handle.emplace<IVs>(hp, atk, def, spa, spd, spe);
}

void PokemonStateSetup::setIVs(const IVs& ivs) {
  handle.emplace<IVs>(ivs);
}
}  // namespace pokesim