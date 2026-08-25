#pragma once

#include <Pokedex/Types/headers.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <array>
#include <type_traits>

namespace pokesim {
struct TypeChart {
 private:
  // The extra array element is for NO_TYPE
  std::array<std::array<TypeEffectiveness, dex::TOTAL_TYPE_COUNT + 1U>, dex::TOTAL_TYPE_COUNT + 1U> data{};

  using enumType = std::underlying_type_t<dex::Type>;

  template <typename Type>
  constexpr void setTypeData(GameMechanics gameMechanic) {
    auto& attacking = data.at((enumType)Type::name(gameMechanic));
    for (enumType defending = 0U; defending <= dex::TOTAL_TYPE_COUNT; defending++) {
      attacking.at(defending) = Type::attacking(gameMechanic, (dex::Type)defending);
    }
  }

 public:
  constexpr TypeChart(GameMechanics gameMechanic) {
    for (TypeEffectiveness& effectiveness : data.at((enumType)(dex::Type::NO_TYPE))) {
      effectiveness = TypeEffectiveness::NEUTRAL;
    }
    setTypeData<dex::Normal>(gameMechanic);
    setTypeData<dex::Grass>(gameMechanic);
    setTypeData<dex::Water>(gameMechanic);
    setTypeData<dex::Fire>(gameMechanic);
    setTypeData<dex::Bug>(gameMechanic);
    setTypeData<dex::Flying>(gameMechanic);
    setTypeData<dex::Poison>(gameMechanic);
    setTypeData<dex::Electric>(gameMechanic);
    setTypeData<dex::Ground>(gameMechanic);
    setTypeData<dex::Fighting>(gameMechanic);
    setTypeData<dex::Psychic>(gameMechanic);
    setTypeData<dex::Rock>(gameMechanic);
    setTypeData<dex::Ice>(gameMechanic);
    setTypeData<dex::Ghost>(gameMechanic);
    setTypeData<dex::Dragon>(gameMechanic);
    setTypeData<dex::Dark>(gameMechanic);
    setTypeData<dex::Steel>(gameMechanic);
    setTypeData<dex::Fairy>(gameMechanic);
  }

  constexpr TypeEffectiveness effectiveness(dex::Type attacking, dex::Type defending) const {
    return data.at((enumType)attacking).at((enumType)defending);
  }
};
}  // namespace pokesim
