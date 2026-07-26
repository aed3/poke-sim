#pragma once

#include <Pokedex/Types/headers.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <array>
#include <type_traits>

namespace pokesim {
namespace internal {
// The extra array element is for NO_TYPE
using TypeChartBase = std::array<std::array<TypeEffectiveness, dex::TOTAL_TYPE_COUNT + 1U>, dex::TOTAL_TYPE_COUNT + 1U>;
}  // namespace internal

struct TypeChart : private internal::TypeChartBase {
 private:
  using enumType = std::underlying_type_t<dex::Type>;

  template <typename Type>
  constexpr void setSetData(GameMechanics gameMechanic) {
    auto& attacking = at((enumType)Type::name(gameMechanic));
    for (enumType defending = 0U; defending <= dex::TOTAL_TYPE_COUNT; defending++) {
      attacking.at(defending) = Type::attacking(gameMechanic, (dex::Type)defending);
    }
  }

 public:
  constexpr TypeChart(GameMechanics gameMechanic) : internal::TypeChartBase() {
    setSetData<dex::Normal>(gameMechanic);
    setSetData<dex::Grass>(gameMechanic);
    setSetData<dex::Water>(gameMechanic);
    setSetData<dex::Fire>(gameMechanic);
    setSetData<dex::Bug>(gameMechanic);
    setSetData<dex::Flying>(gameMechanic);
    setSetData<dex::Poison>(gameMechanic);
    setSetData<dex::Electric>(gameMechanic);
    setSetData<dex::Ground>(gameMechanic);
    setSetData<dex::Fighting>(gameMechanic);
    setSetData<dex::Psychic>(gameMechanic);
    setSetData<dex::Rock>(gameMechanic);
    setSetData<dex::Ice>(gameMechanic);
    setSetData<dex::Ghost>(gameMechanic);
    setSetData<dex::Dragon>(gameMechanic);
    setSetData<dex::Dark>(gameMechanic);
    setSetData<dex::Steel>(gameMechanic);
    setSetData<dex::Fairy>(gameMechanic);
  }

  constexpr TypeEffectiveness effectiveness(dex::Type attacking, dex::Type defending) const {
    return at((enumType)attacking).at((enumType)defending);
  }
};
}  // namespace pokesim
