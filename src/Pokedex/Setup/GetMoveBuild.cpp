#include <Components/Boosts.hpp>
#include <Components/Tags/TargetTags.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/AddedTargets.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Registry.hpp>
#include <cstdint>
#include <type_traits>

#include "../Moves/headers.hpp"
#include "MoveDexDataSetup.hpp"

// TODO(aed3): Make this and the individual move files auto generated

namespace pokesim {
namespace {
template <typename T>
struct BuildMove {
 private:
  enum class Optional : std::uint8_t {
    accuracy,
    basePower,
    hitCount,
    targetPrimaryEffect,
    targetSecondaryEffect,
    sourcePrimaryEffect,
    sourceSecondaryEffect,
    chance,
    atkBoost,
    defBoost,
    spaBoost,
    spdBoost,
    speBoost,

    // moveTags and effectTags are not optional because setting them as optional does not work with clang
  };

  enum class MoveEffectKind : std::uint8_t {
    primary,
    secondary,
  };

  template <auto Member>
  using void_t = std::void_t<decltype(Member)>;

  template <Optional, typename, typename V = void>
  struct has : std::false_type {};
  template <typename Type>
  struct has<Optional::accuracy, Type, void_t<Type::accuracy>> : std::true_type {};
  template <typename Type>
  struct has<Optional::basePower, Type, void_t<Type::basePower>> : std::true_type {};
  template <typename Type>
  struct has<Optional::hitCount, Type, void_t<Type::hitCount>> : std::true_type {};
  template <typename Type>
  struct has<Optional::targetPrimaryEffect, Type, std::void_t<typename Type::targetPrimaryEffect>> : std::true_type {};
  template <typename Type>
  struct has<Optional::targetSecondaryEffect, Type, std::void_t<typename Type::targetSecondaryEffect>>
      : std::true_type {};
  template <typename Type>
  struct has<Optional::sourcePrimaryEffect, Type, std::void_t<typename Type::sourcePrimaryEffect>> : std::true_type {};
  template <typename Type>
  struct has<Optional::sourceSecondaryEffect, Type, std::void_t<typename Type::sourceSecondaryEffect>>
      : std::true_type {};
  template <typename Type>
  struct has<Optional::chance, Type, void_t<Type::chance>> : std::true_type {};
  template <typename Type>
  struct has<Optional::atkBoost, Type, void_t<Type::atkBoost>> : std::true_type {};
  template <typename Type>
  struct has<Optional::defBoost, Type, void_t<Type::defBoost>> : std::true_type {};
  template <typename Type>
  struct has<Optional::spaBoost, Type, void_t<Type::spaBoost>> : std::true_type {};
  template <typename Type>
  struct has<Optional::spdBoost, Type, void_t<Type::spdBoost>> : std::true_type {};
  template <typename Type>
  struct has<Optional::speBoost, Type, void_t<Type::speBoost>> : std::true_type {};

  template <typename EffectType, typename EffectData, MoveEffectKind moveEffectKind, typename... EffectValues>
  static void setEffect(dex::internal::MoveDexDataSetup& move, const EffectValues&... effectValues) {
    if constexpr (moveEffectKind == MoveEffectKind::primary) {
      static_assert(
        !has<Optional::chance, EffectData>::value,
        "Primary effects cannot have a chance to happen because they always happen if the move is successful.");
      move.setPrimaryEffect<EffectType>(effectValues...);
    }
    else {
      types::baseEffectChance chance = MechanicConstants::MoveBaseEffectChance::MAX;
      if constexpr (has<Optional::chance, EffectData>::value) {
        chance = EffectData::chance;
      }
      move.setSecondaryEffect<EffectType>(chance, effectValues...);
    }
  }

  template <typename EffectData, MoveEffectKind moveEffectKind, typename... EffectTypes>
  static void setEffectTags(dex::internal::MoveDexDataSetup& move, Tags<EffectTypes...>) {
    if constexpr (moveEffectKind == MoveEffectKind::primary) {
      static_assert(
        !has<Optional::chance, EffectData>::value,
        "Primary effects cannot have a chance to happen because they always happen if the move is successful.");
      (move.setPrimaryEffect<EffectTypes>(), ...);
    }
    else {
      types::baseEffectChance chance = MechanicConstants::MoveBaseEffectChance::MAX;
      if constexpr (has<Optional::chance, EffectData>::value) {
        chance = EffectData::chance;
      }
      (move.setSecondaryEffect<EffectTypes>(chance), ...);
    }
  }

  template <typename EffectData, MoveEffectKind moveEffectKind>
  static void buildEffect(dex::internal::MoveDexDataSetup& move) {
    if constexpr (has<Optional::atkBoost, EffectData>::value) {
      setEffect<AtkBoost, EffectData, moveEffectKind>(move, EffectData::atkBoost);
    }

    if constexpr (has<Optional::defBoost, EffectData>::value) {
      setEffect<DefBoost, EffectData, moveEffectKind>(move, EffectData::defBoost);
    }

    if constexpr (has<Optional::spaBoost, EffectData>::value) {
      setEffect<SpaBoost, EffectData, moveEffectKind>(move, EffectData::spaBoost);
    }

    if constexpr (has<Optional::spdBoost, EffectData>::value) {
      setEffect<SpdBoost, EffectData, moveEffectKind>(move, EffectData::spdBoost);
    }

    if constexpr (has<Optional::speBoost, EffectData>::value) {
      setEffect<SpeBoost, EffectData, moveEffectKind>(move, EffectData::speBoost);
    }

    setEffectTags<EffectData, moveEffectKind>(move, EffectData::effectTags);
  }

 public:
  static types::entity build(types::registry& registry, bool forActiveMove) {
    dex::internal::MoveDexDataSetup move(registry);

    if (!forActiveMove) {
      move.setName(T::name);
      move.setBasePp(T::basePp);
    }

    move.setType(T::type);
    switch (T::category) {
      case dex::MoveCategory::PHYSICAL: {
        move.setCategoryPhysical();
        break;
      }
      case dex::MoveCategory::SPECIAL: {
        move.setCategorySpecial();
        break;
      }
      case dex::MoveCategory::STATUS: {
        move.setCategoryStatus();
        break;
      }
    }

    if constexpr (has<Optional::accuracy, T>::value) {
      move.setAccuracy(T::accuracy);
    }
    if constexpr (has<Optional::basePower, T>::value) {
      move.setBasePower(T::basePower);
    }
    if constexpr (has<Optional::hitCount, T>::value) {
      move.setHitCount(T::hitCount);
    }

    if constexpr (has<Optional::sourcePrimaryEffect, T>::value) {
      buildEffect<typename T::sourcePrimaryEffect, MoveEffectKind::primary>(move);
      move.setEffectTargetsMoveSource();
    }

    if constexpr (has<Optional::targetPrimaryEffect, T>::value) {
      buildEffect<typename T::targetPrimaryEffect, MoveEffectKind::primary>(move);
      move.setEffectTargetsMoveTarget();
    }

    if constexpr (has<Optional::sourceSecondaryEffect, T>::value) {
      buildEffect<typename T::sourceSecondaryEffect, MoveEffectKind::secondary>(move);
      move.setEffectTargetsMoveSource();
    }

    if constexpr (has<Optional::targetSecondaryEffect, T>::value) {
      buildEffect<typename T::targetSecondaryEffect, MoveEffectKind::secondary>(move);
      move.setEffectTargetsMoveTarget();
    }

    move.setProperties(T::moveTags);

    switch (T::target) {
      case MoveTarget::ANY_SINGLE_TARGET: {
        move.setProperty<move::tags::AnySingleTarget>();
        break;
      }
      case MoveTarget::ANY_SINGLE_FOE: {
        move.setProperty<move::tags::AnySingleFoe>();
        break;
      }
      case MoveTarget::ANY_SINGLE_ALLY: {
        move.setProperty<move::tags::AnySingleAlly>();
        break;
      }
      case MoveTarget::ALLY_OR_SELF: {
        move.setProperty<move::tags::AllyOrSelf>();
        break;
      }
      case MoveTarget::SELF: {
        move.setProperty<move::tags::Self>();
        break;
      }
      case MoveTarget::ALL_FOES: {
        move.setProperty<move::tags::AllFoes>();
        move.addAddedTargets(AddedTargetOptions::TARGET_ALLY);
        break;
      }
      case MoveTarget::ALLIES_AND_FOES: {
        move.setProperty<move::tags::AlliesAndFoes>();
        move.addAddedTargets(AddedTargetOptions::TARGET_ALLY);
        move.addAddedTargets(AddedTargetOptions::USER_ALLY);
        break;
      }
      case MoveTarget::ALLIES_AND_SELF: {
        move.setProperty<move::tags::AlliesAndSelf>();
        // Deliberately not USER_ALLY as the target of AlliesAndSelf moves is the user
        move.addAddedTargets(AddedTargetOptions::TARGET_ALLY);
        break;
      }
      case MoveTarget::FOE_SIDE: {
        move.setProperty<move::tags::FoeSide>();
        move.addAddedTargets(AddedTargetOptions::TARGET_SIDE);
        break;
      }
      case MoveTarget::ALLY_SIDE: {
        move.setProperty<move::tags::AllySide>();
        move.addAddedTargets(AddedTargetOptions::USER_SIDE);
        break;
      }
      case MoveTarget::FIELD: {
        move.setProperty<move::tags::Field>();
        move.addAddedTargets(AddedTargetOptions::FIELD);
        break;
      }
      case MoveTarget::ALLY_TEAM: {
        move.setProperty<move::tags::AllyTeam>();
        move.addAddedTargets(AddedTargetOptions::USER_SIDE);
        break;
      }
      case MoveTarget::RETALIATION: {
        move.setProperty<move::tags::Retaliation>();
        break;
      }
      case MoveTarget::RANDOM_FOE: {
        move.setProperty<move::tags::RandomFoe>();
        break;
      }
      default: break;
    }

    return move.entity();
  }
};

template <template <GameMechanics> class T>
auto buildMoveSV(types::registry& registry, bool forActiveMove) {
  return BuildMove<T<GameMechanics::SCARLET_VIOLET>>::build(registry, forActiveMove);
}
}  // namespace

types::entity Pokedex::buildMove(dex::Move move, types::registry& registry, bool forActiveMove) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics()) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (move) {
        case Move::FURY_ATTACK:  return buildMoveSV<FuryAttack>(registry, forActiveMove);
        case Move::THUNDERBOLT:  return buildMoveSV<Thunderbolt>(registry, forActiveMove);
        case Move::WILL_O_WISP:  return buildMoveSV<WillOWisp>(registry, forActiveMove);
        case Move::KNOCK_OFF:    return buildMoveSV<KnockOff>(registry, forActiveMove);
        case Move::QUIVER_DANCE: return buildMoveSV<QuiverDance>(registry, forActiveMove);
        case Move::MOONBLAST:    return buildMoveSV<Moonblast>(registry, forActiveMove);

        default: break;
      }
      break;
    }
    default: break;
  }

  POKESIM_REQUIRE_FAIL("Building a move that does not exist.");
  return types::entity{};
}
}  // namespace pokesim