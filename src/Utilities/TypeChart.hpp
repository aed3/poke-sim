#pragma once

#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <array>
#include <cstdint>
#include <initializer_list>

namespace pokesim::internal {
using TypeChartBase =
  std::array<std::array<pokesim::TypeEffectiveness, pokesim::dex::TOTAL_TYPE_COUNT>, pokesim::dex::TOTAL_TYPE_COUNT>;

struct TypeChart : private TypeChartBase {
 private:
  using constructorType = std::initializer_list<
    std::pair<pokesim::dex::Type, std::initializer_list<std::pair<pokesim::dex::Type, pokesim::TypeEffectiveness>>>>;

 public:
  constexpr TypeChart(const constructorType partialChart) : TypeChartBase() {
    for (auto& ratios : *this) {
      for (auto& effectiveness : ratios) {
        effectiveness = pokesim::TypeEffectiveness::NEUTRAL;
      }
    }

    for (const auto& [defending, ratios] : partialChart) {
      for (const auto& [attacking, effectiveness] : ratios) {
        at((std::uint8_t)attacking).at((std::uint8_t)defending) = effectiveness;
      }
    }
  }

  constexpr pokesim::TypeEffectiveness effectiveness(pokesim::dex::Type attacking, pokesim::dex::Type defending) const {
    return at((std::uint8_t)attacking).at((std::uint8_t)defending);
  }
};
}  // namespace pokesim::internal