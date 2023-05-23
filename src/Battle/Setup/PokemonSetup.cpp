#include "PokemonSetup.hpp"

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

namespace pokesim {
void PokemonSetup::initBlank() {
  handle.emplace<SpeciesName>();
  handle.emplace<Side>();
  handle.emplace<Battle>();
}

void PokemonSetup::setID(std::uint16_t id) {
  handle.emplace<ID>(id);
}

void PokemonSetup::setSpecies(dex::Species speciesName) {
  handle.emplace<SpeciesName>(speciesName);
}

void PokemonSetup::setSide(entt::entity entity) {
  handle.emplace<Side>(entity);
}

void PokemonSetup::setBattle(entt::entity entity) {
  handle.emplace<Battle>(entity);
}

void PokemonSetup::setLevel(std::uint8_t level) {
  handle.emplace<Level>(level);
}

void PokemonSetup::setGender(dex::Gender gender) {
  handle.emplace<GenderName>(gender);
  // TODO(aed3): Add Gender Tag as well
}

void PokemonSetup::setAbility(dex::Ability ability) {
  handle.emplace<AbilityName>(ability);
  // TODO(aed3): Add Ability Tag as well
}

void PokemonSetup::setItem(dex::Item item) {
  handle.emplace<ItemName>(item);
  // TODO(aed3): Add Item Tag as well
}

void PokemonSetup::addMove(entt::entity entity) {
  MoveSlots& moveSlots = handle.get_or_emplace<MoveSlots>();
  moveSlots.moveSlots.push_back(entity);
}

void PokemonSetup::setPostion(std::uint8_t position) {
  handle.emplace<Position>(position);
}

void PokemonSetup::setStatus(dex::Status status) {
  handle.emplace<StatusName>(status);
  // TODO(aed3): Add Status Tag as well
}

void PokemonSetup::setNature(dex::Nature nature) {
  handle.emplace<NatureName>(nature);
  // TODO(aed3): Add Nature Tag
}

void PokemonSetup::setEVs(
  std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe) {
  handle.emplace<EVs>(hp, atk, def, spa, spd, spe);
}

void PokemonSetup::setIVs(
  std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe) {
  handle.emplace<IVs>(hp, atk, def, spa, spd, spe);
}
}  // namespace pokesim