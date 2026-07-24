#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Decidueye {
  static constexpr Species name(GameMechanics) { return Species::DECIDUEYE; }
  static constexpr types::baseStat hp(GameMechanics) { return 78U; }
  static constexpr types::baseStat atk(GameMechanics) { return 107U; }
  static constexpr types::baseStat def(GameMechanics) { return 75U; }
  static constexpr types::baseStat spa(GameMechanics) { return 100U; }
  static constexpr types::baseStat spd(GameMechanics) { return 100U; }
  static constexpr types::baseStat spe(GameMechanics) { return 70U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::GRASS, Type::GHOST}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::OVERGROW; }
  static constexpr Ability hiddenAbility(GameMechanics) { return Ability::LONG_REACH; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Decidueye"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Decidueye"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "decidueye"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};

struct HisuianDecidueye {
  static constexpr Species name(GameMechanics) { return Species::HISUIAN_DECIDUEYE; }
  static constexpr types::baseStat hp(GameMechanics) { return 88U; }
  static constexpr types::baseStat atk(GameMechanics) { return 112U; }
  static constexpr types::baseStat def(GameMechanics) { return 80U; }
  static constexpr types::baseStat spa(GameMechanics) { return 95U; }
  static constexpr types::baseStat spd(GameMechanics) { return 95U; }
  static constexpr types::baseStat spe(GameMechanics) { return 60U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::GRASS, Type::FIGHTING}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::OVERGROW; }
  static constexpr Ability hiddenAbility(GameMechanics) { return Ability::SCRAPPY; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Hisuian Decidueye"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Decidueye-Hisui"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "decidueyehisui"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
