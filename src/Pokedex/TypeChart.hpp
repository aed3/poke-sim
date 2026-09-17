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
    setTypeData<dex::NormalType>(gameMechanic);
    setTypeData<dex::GrassType>(gameMechanic);
    setTypeData<dex::WaterType>(gameMechanic);
    setTypeData<dex::FireType>(gameMechanic);
    setTypeData<dex::BugType>(gameMechanic);
    setTypeData<dex::FlyingType>(gameMechanic);
    setTypeData<dex::PoisonType>(gameMechanic);
    setTypeData<dex::ElectricType>(gameMechanic);
    setTypeData<dex::GroundType>(gameMechanic);
    setTypeData<dex::FightingType>(gameMechanic);
    setTypeData<dex::PsychicType>(gameMechanic);
    setTypeData<dex::RockType>(gameMechanic);
    setTypeData<dex::IceType>(gameMechanic);
    setTypeData<dex::GhostType>(gameMechanic);
    setTypeData<dex::DragonType>(gameMechanic);
    setTypeData<dex::DarkType>(gameMechanic);
    setTypeData<dex::SteelType>(gameMechanic);
    setTypeData<dex::FairyType>(gameMechanic);
  }

  constexpr TypeEffectiveness effectiveness(dex::Type attacking, dex::Type defending) const {
    return data.at((enumType)attacking).at((enumType)defending);
  }
};
}  // namespace pokesim
