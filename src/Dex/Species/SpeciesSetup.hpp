#pragma once

#include <Components/Names/Species.hpp>
#include <Components/Names/Type.hpp>
#include <Dex/Dex.hpp>

namespace pokesim::dex::internal {
class SpeciesSetup {
  entt::handle handle;

 public:
  SpeciesSetup(Dex& pokedex) : handle(pokedex.createEntry()) {}

  void setName(Species species);
  void setType(Type type1, Type type2 = NO_TYPE);
  void setBaseStats(
    std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe);

  template <typename Tag>
  void setProperty();
};
}  // namespace pokesim::dex::internal