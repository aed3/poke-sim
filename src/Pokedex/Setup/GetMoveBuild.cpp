#include <Components/Boosts.hpp>
#include <Components/Tags/TargetTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/AddedTargets.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <type_traits>

#include "../Moves/headers.hpp"
#include "MoveDexDataSetup.hpp"

// TODO(aed3): Make this and the individual move files auto generated

namespace pokesim {
namespace internal {
template <typename T>
struct BuildMove {
 private:
  enum class Optional {
    accuracy,
    basePower,
    minHits,
    maxHits,
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

  template <auto Member>
  using void_t = std::void_t<decltype(Member)>;

  template <Optional, typename, typename V = void>
  struct has : std::false_type {};
  template <typename Type>
  struct has<Optional::accuracy, Type, void_t<Type::accuracy>> : std::true_type {};
  template <typename Type>
  struct has<Optional::basePower, Type, void_t<Type::basePower>> : std::true_type {};
  template <typename Type>
  struct has<Optional::minHits, Type, void_t<Type::minHits>> : std::true_type {};
  template <typename Type>
  struct has<Optional::maxHits, Type, void_t<Type::maxHits>> : std::true_type {};
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

  template <typename EffectData>
  static types::entity buildEffect(types::registry& registry, bool effectsTarget) {
    dex::internal::MoveEffectSetup effect(registry);

    if constexpr (has<Optional::chance, EffectData>::value) {
      effect.setChance(EffectData::chance);
    }

    if (effectsTarget) {
      effect.setEffectsTarget();
    }
    else {
      effect.setEffectsSelf();
    }

    if constexpr (has<Optional::atkBoost, EffectData>::value) {
      effect.setBoost<AtkBoost>(EffectData::atkBoost);
    }

    if constexpr (has<Optional::defBoost, EffectData>::value) {
      effect.setBoost<DefBoost>(EffectData::defBoost);
    }

    if constexpr (has<Optional::spaBoost, EffectData>::value) {
      effect.setBoost<SpaBoost>(EffectData::spaBoost);
    }

    if constexpr (has<Optional::spdBoost, EffectData>::value) {
      effect.setBoost<SpdBoost>(EffectData::spdBoost);
    }

    if constexpr (has<Optional::speBoost, EffectData>::value) {
      effect.setBoost<SpeBoost>(EffectData::speBoost);
    }

    effect.setProperties(EffectData::effectTags);

    return effect.entity();
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
    if constexpr (has<Optional::maxHits, T>::value && has<Optional::minHits, T>::value) {
      move.setMultiHit(T::minHits, T::maxHits);
    }

    if (!forActiveMove) {
      if constexpr (has<Optional::sourcePrimaryEffect, T>::value) {
        move.setPrimaryEffect(buildEffect<typename T::sourcePrimaryEffect>(registry, false));
      }

      if constexpr (has<Optional::targetPrimaryEffect, T>::value) {
        move.setPrimaryEffect(buildEffect<typename T::targetPrimaryEffect>(registry, true));
      }

      if constexpr (has<Optional::sourceSecondaryEffect, T>::value) {
        move.setSecondaryEffect(buildEffect<typename T::sourceSecondaryEffect>(registry, false));
      }

      if constexpr (has<Optional::targetSecondaryEffect, T>::value) {
        move.setSecondaryEffect(buildEffect<typename T::targetSecondaryEffect>(registry, true));
      }
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
};  // namespace internal

types::entity Pokedex::buildMove(dex::Move move, types::registry& registry, bool forActiveMove) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (move) {
        case Move::FURY_ATTACK: return buildMoveSV<FuryAttack>(registry, forActiveMove);
        case Move::THUNDERBOLT: return buildMoveSV<Thunderbolt>(registry, forActiveMove);
        case Move::WILL_O_WISP: return buildMoveSV<WillOWisp>(registry, forActiveMove);
        case Move::KNOCK_OFF: return buildMoveSV<KnockOff>(registry, forActiveMove);
        case Move::QUIVER_DANCE: return buildMoveSV<QuiverDance>(registry, forActiveMove);
        case Move::MOONBLAST: return buildMoveSV<Moonblast>(registry, forActiveMove);
        default: break;
      }
      break;
    }
    default: break;
  }

  ENTT_FAIL("Building a move that does not exist");
  return types::entity{};
}
};  // namespace pokesim