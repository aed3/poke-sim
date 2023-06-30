#include "PokemonStateSetup.hpp"

#include <Components/EVsIVs.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/ID.hpp>
#include <Components/Level.hpp>
#include <Components/Names/Ability.hpp>
#include <Components/Names/Gender.hpp>
#include <Components/Names/Item.hpp>
#include <Components/Names/Nature.hpp>
#include <Components/Names/Species.hpp>
#include <Components/Names/Status.hpp>
#include <Components/Position.hpp>
#include <Components/Tags/AbilityTags.hpp>
#include <Components/Tags/ItemTags.hpp>
#include <Components/Tags/NatureTags.hpp>
#include <Components/Tags/StatusTags.hpp>

namespace pokesim {
void PokemonStateSetup::initBlank() {
  handle.emplace<SpeciesName>();
  handle.emplace<Side>();
  handle.emplace<Battle>();
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

void PokemonStateSetup::addMove(entt::entity entity) {
  MoveSlots& moveSlots = handle.get_or_emplace<MoveSlots>();
  moveSlots.moveSlots.push_back(entity);
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

void PokemonStateSetup::setIVs(
  std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe) {
  handle.emplace<IVs>(hp, atk, def, spa, spd, spe);
}
}  // namespace pokesim