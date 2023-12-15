#include <Components/Boosts.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <entt/entity/fwd.hpp>
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
  static entt::entity buildEffect(Pokedex* pokedex, bool effectsTarget) {
    dex::internal::MoveEffectSetup effect(pokedex);

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
  static entt::entity build(Pokedex* pokedex) {
    dex::internal::MoveDexDataSetup move(pokedex);

    move.setName(T::name);
    move.setType(T::type);
    move.setBasePp(T::basePp);
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

    if constexpr (has<Optional::sourcePrimaryEffect, T>::value) {
      move.setPrimaryEffect(buildEffect<typename T::sourcePrimaryEffect>(pokedex, false));
    }

    if constexpr (has<Optional::targetPrimaryEffect, T>::value) {
      move.setPrimaryEffect(buildEffect<typename T::targetPrimaryEffect>(pokedex, true));
    }

    if constexpr (has<Optional::sourceSecondaryEffect, T>::value) {
      move.setSecondaryEffect(buildEffect<typename T::sourceSecondaryEffect>(pokedex, false));
    }

    if constexpr (has<Optional::targetSecondaryEffect, T>::value) {
      move.setSecondaryEffect(buildEffect<typename T::targetSecondaryEffect>(pokedex, true));
    }

    move.setProperties(T::moveTags);

    return move.entity();
  }
};

template <template <GameMechanics> class T>
auto buildMoveSV(Pokedex* pokedex) {
  return BuildMove<T<GameMechanics::SCARLET_VIOLET>>::build(pokedex);
}
};  // namespace internal

entt::entity Pokedex::buildMove(dex::Move move) {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (move) {
        case Move::FURY_ATTACK: return buildMoveSV<FuryAttack>(this);
        case Move::THUNDERBOLT: return buildMoveSV<Thunderbolt>(this);
        case Move::WILL_O_WISP: return buildMoveSV<WillOWisp>(this);
        case Move::KNOCK_OFF: return buildMoveSV<KnockOff>(this);
        case Move::QUIVER_DANCE: return buildMoveSV<QuiverDance>(this);
        case Move::MOONBLAST: return buildMoveSV<Moonblast>(this);
        default: break;
      }
      break;
    }
    default: break;
  }

  ENTT_FAIL("Building a move that does not exist");
  return entt::entity{};
}
};  // namespace pokesim