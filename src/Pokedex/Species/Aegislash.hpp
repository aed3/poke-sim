#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Aegislash {
  static constexpr Species name(GameMechanics) { return Species::AEGISLASH; }
  static constexpr types::baseStat hp(GameMechanics) { return 60U; }
  static constexpr types::baseStat atk(GameMechanics) { return 50U; }
  static constexpr types::baseStat def(GameMechanics) { return 140U; }
  static constexpr types::baseStat spa(GameMechanics) { return 50U; }
  static constexpr types::baseStat spd(GameMechanics) { return 140U; }
  static constexpr types::baseStat spe(GameMechanics) { return 60U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::STEEL, Type::GHOST}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::STANCE_CHANGE; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Aegislash"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Aegislash"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "aegislash"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};

struct ShieldAegislash : Aegislash {
  static constexpr Species name(GameMechanics) { return Species::SHIELD_AEGISLASH; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Shield Form Aegislash"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Aegislash"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "aegislash"; }
  };
};

struct BladeAegislash : Aegislash {
  static constexpr Species name(GameMechanics) { return Species::BLADE_AEGISLASH; }

  static constexpr types::baseStat atk(GameMechanics) { return 140U; }
  static constexpr types::baseStat def(GameMechanics) { return 50U; }
  static constexpr types::baseStat spa(GameMechanics) { return 140U; }
  static constexpr types::baseStat spd(GameMechanics) { return 50U; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Blade Form Aegislash"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Aegislash-Blade"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "aegislashblade"; }
  };
};

}  // namespace pokesim::dex
