#pragma once

/**
 * FILE ORDER
 * external/entt/config/macro.h
 * external/entt/config/version.h
 * external/entt/config/config.h
 * external/entt/container/fwd.hpp
 * external/entt/core/fwd.hpp
 * external/entt/core/type_traits.hpp
 * external/entt/core/compressed_pair.hpp
 * external/entt/core/iterator.hpp
 * external/entt/core/memory.hpp
 * external/entt/container/dense_map.hpp
 * external/entt/core/utility.hpp
 * external/entt/core/algorithm.hpp
 * external/entt/core/attribute.h
 * external/entt/core/hashed_string.hpp
 * external/entt/core/type_info.hpp
 * external/entt/core/any.hpp
 * external/entt/entity/fwd.hpp
 * external/entt/entity/entity.hpp
 * external/entt/entity/sparse_set.hpp
 * external/entt/entity/component.hpp
 * external/entt/entity/storage.hpp
 * external/entt/entity/group.hpp
 * external/entt/signal/fwd.hpp
 * external/entt/signal/delegate.hpp
 * external/entt/signal/sigh.hpp
 * external/entt/entity/mixin.hpp
 * external/entt/entity/view.hpp
 * external/entt/entity/registry.hpp
 * external/entt/entity/handle.hpp
 * src/Types/Enums/PseudoWeather.hpp
 * src/Types/Enums/SideCondition.hpp
 * src/Types/Enums/Status.hpp
 * src/Types/Enums/Terrain.hpp
 * src/Types/Enums/Volatile.hpp
 * src/Types/Enums/Weather.hpp
 * src/Utilities/Variant.hpp
 * src/Types/Effect.hpp
 * src/Types/Entity.hpp
 * src/AnalyzeEffect/Setup/AnalyzeEffectInputSetup.hpp
 * src/Types/Enums/Move.hpp
 * src/Types/Enums/Slot.hpp
 * src/Battle/Helpers/Helpers.hpp
 * src/Battle/Setup/StateSetupBase.hpp
 * src/Types/Stats.hpp
 * src/Components/Boosts.hpp
 * src/Components/EVsIVs.hpp
 * src/Components/Stats.hpp
 * src/Types/Enums/Ability.hpp
 * src/Types/Enums/Gender.hpp
 * src/Types/Enums/Item.hpp
 * src/Types/Enums/Nature.hpp
 * src/Types/Enums/Species.hpp
 * src/Utilities/MaxSizedVector.hpp
 * src/Types/State.hpp
 * src/Battle/Setup/PokemonStateSetup.hpp
 * src/Types/Enums/PlayerSideId.hpp
 * src/Types/Random.hpp
 * src/Battle/Setup/BattleStateSetup.hpp
 * src/Types/Move.hpp
 * src/Battle/Setup/MoveStateSetup.hpp
 * src/Battle/Setup/SideStateSetup.hpp
 * src/CalcDamage/Setup/CalcDamageInputSetup.hpp
 * src/Components/CalcDamage/AttackerDefender.hpp
 * src/Components/Decisions.hpp
 * src/Components/EntityHolders/ActionQueue.hpp
 * src/Components/EntityHolders/Battle.hpp
 * src/Components/EntityHolders/Side.hpp
 * src/Components/EntityHolders/Sides.hpp
 * src/Components/Tags/PokemonTags.hpp
 * src/Components/Tags/SimulationTags.hpp
 * src/Components/EntityHolders/MoveEffect.hpp
 * src/Types/Enums/ActionOrder.hpp
 * src/Types/Enums/AddedTargets.hpp
 * src/Types/Enums/BattleFormat.hpp
 * src/Types/Enums/DamageRollKind.hpp
 * src/Types/Enums/GameMechanics.hpp
 * src/Types/Enums/MoveCategory.hpp
 * src/Types/Enums/MoveTarget.hpp
 * src/Types/Enums/Stat.hpp
 * src/Types/Enums/Type.hpp
 * src/Pokedex/Pokedex.hpp
 * src/Components/Selection.hpp
 * src/Simulation/SimulationOptions.hpp
 * src/Types/Damage.hpp
 * src/Utilities/RegistryLoop.hpp
 * src/Simulation/Simulation.hpp
 * src/Simulation/SimulationSetup.cpp
 * src/Simulation/SimulationResults.hpp
 * src/Simulation/SimulationResults.cpp
 * src/AnalyzeEffect/AnalyzeEffect.hpp
 * src/CalcDamage/CalcDamage.hpp
 * src/SimulateTurn/SimulateTurn.hpp
 * src/Simulation/Simulation.cpp
 * src/Components/Tags/StatusTags.hpp
 * src/Pokedex/Abilities/Static.hpp
 * src/Simulation/RunEvent.hpp
 * src/Simulation/RunEvent.cpp
 * src/Battle/Clone/Clone.hpp
 * src/Components/CloneFromCloneTo.hpp
 * src/Components/RNGSeed.hpp
 * src/Components/RandomEventCheck.hpp
 * src/Components/Tags/RandomChanceTags.hpp
 * src/Simulation/RandomChance.hpp
 * src/Utilities/RNG.hpp
 * src/Simulation/RandomChance.cpp
 * src/Components/Accuracy.hpp
 * src/Components/EntityHolders/Current.hpp
 * src/Components/MultiHit.hpp
 * src/Components/SimulateTurn/MoveHitStepTags.hpp
 * src/Components/Tags/BattleTags.hpp
 * src/Simulation/MoveHitSteps.hpp
 * src/Simulation/MoveHitSteps.cpp
 * src/Battle/ManageBattleState.hpp
 * src/Battle/Pokemon/ManagePokemonState.hpp
 * src/Components/AddedTargets.hpp
 * src/Components/Names/SourceSlotName.hpp
 * src/Components/Names/TargetSlotName.hpp
 * src/Components/PP.hpp
 * src/Components/Names/ItemNames.hpp
 * src/Components/Names/MoveNames.hpp
 * src/Components/SimulateTurn/ActionNames.hpp
 * src/Components/SimulateTurn/ActionTags.hpp
 * src/Components/SpeedSort.hpp
 * src/Components/Tags/TargetTags.hpp
 * src/Components/Turn.hpp
 * src/SimulateTurn/ManageActionQueue.hpp
 * src/Components/EntityHolders/Team.hpp
 * src/Components/Names/SpeciesNames.hpp
 * src/Utilities/SelectForView.hpp
 * src/SimulateTurn/SimulateTurn.cpp
 * src/Components/SimulateTurn/SpeedTieIndexes.hpp
 * src/Components/SimulateTurn/TeamAction.hpp
 * src/SimulateTurn/ManageActionQueue.cpp
 * src/Components/Pokedex/Abilities.hpp
 * src/Components/Pokedex/BaseStats.hpp
 * src/Components/Pokedex/SpeciesTypes.hpp
 * src/Pokedex/Setup/DexDataTags.hpp
 * src/Pokedex/Setup/DexDataSetup.hpp
 * src/Pokedex/Setup/SpeciesDexDataSetup.hpp
 * src/Pokedex/Setup/SpeciesDexDataSetup.cpp
 * src/Components/BasePower.hpp
 * src/Components/Chance.hpp
 * src/Components/Names/TypeNames.hpp
 * src/Components/Priority.hpp
 * src/Components/Tags/MoveTags.hpp
 * src/Pokedex/Setup/MoveDexDataSetup.hpp
 * src/Pokedex/Setup/MoveDexDataSetup.cpp
 * src/Pokedex/Setup/ItemDexDataSetup.hpp
 * src/Pokedex/Setup/ItemDexDataSetup.cpp
 * src/Pokedex/Species/Ampharos.hpp
 * src/Pokedex/Species/Dragapult.hpp
 * src/Pokedex/Species/Empoleon.hpp
 * src/Pokedex/Species/Gardevoir.hpp
 * src/Pokedex/Species/Pangoro.hpp
 * src/Pokedex/Species/Ribombee.hpp
 * src/Pokedex/Setup/GetSpeciesBuild.cpp
 * src/Pokedex/Moves/FuryAttack.hpp
 * src/Pokedex/Moves/KnockOff.hpp
 * src/Pokedex/Moves/Moonblast.hpp
 * src/Pokedex/Moves/QuiverDance.hpp
 * src/Pokedex/Moves/Thunderbolt.hpp
 * src/Pokedex/Moves/WillOWisp.hpp
 * src/Pokedex/Setup/GetMoveBuild.cpp
 * src/Pokedex/Items/AssaultVest.hpp
 * src/Pokedex/Items/BrightPowder.hpp
 * src/Pokedex/Items/ChoiceScarf.hpp
 * src/Pokedex/Items/ChoiceSpecs.hpp
 * src/Pokedex/Items/FocusSash.hpp
 * src/Pokedex/Items/LifeOrb.hpp
 * src/Pokedex/Setup/GetItemBuild.cpp
 * src/Pokedex/Setup/AbilityDexDataSetup.hpp
 * src/Pokedex/Setup/GetAbilityBuild.cpp
 * src/Components/Names/AbilityNames.hpp
 * src/Pokedex/Setup/AbilityDexDataSetup.cpp
 * external/entt/container/dense_set.hpp
 * src/Pokedex/Pokedex.cpp
 * src/Pokedex/Abilities/AbilityEvents.cpp
 * src/Components/Tags/StatusTags.cpp
 * src/Components/Tags/NatureTags.hpp
 * src/Components/Tags/NatureTags.cpp
 * src/Components/Tags/ItemTags.hpp
 * src/Components/Tags/ItemTags.cpp
 * src/Components/Tags/AbilityTags.hpp
 * src/Components/Tags/AbilityTags.cpp
 * src/CalcDamage/Setup/CalcDamageInputSetup.cpp
 * src/CalcDamage/CalcDamage.cpp
 * src/Components/EntityHolders/FoeSide.hpp
 * src/Battle/Setup/SideStateSetup.cpp
 * src/Components/EntityHolders/MoveSlots.hpp
 * src/Components/ID.hpp
 * src/Components/Level.hpp
 * src/Components/Names/GenderNames.hpp
 * src/Components/Names/NatureNames.hpp
 * src/Components/Names/StatusNames.hpp
 * src/Components/Position.hpp
 * src/Battle/Setup/PokemonStateSetup.cpp
 * src/Battle/Setup/MoveStateSetup.cpp
 * src/Components/Probability.hpp
 * src/Battle/Setup/BattleStateSetup.cpp
 * src/Components/EntityHolders/LastUsedMove.hpp
 * src/Battle/Pokemon/ManagePokemonState.cpp
 * src/Battle/ManageBattleState.cpp
 * src/Battle/Helpers/Helpers.cpp
 * src/Components/EntityHolders/Pokemon.hpp
 * src/Battle/Clone/Clone.cpp
 * src/Components/AnalyzeEffect/AttackerDefender.hpp
 * src/Components/Names/PseudoWeatherNames.hpp
 * src/Components/Names/SideConditionNames.hpp
 * src/Components/Names/TerrainNames.hpp
 * src/Components/Names/VolatileNames.hpp
 * src/Components/Names/WeatherNames.hpp
 * src/AnalyzeEffect/Setup/AnalyzeEffectInputSetup.cpp
 * src/Components/AnalyzeEffect/RemovedEffect.hpp
 * src/AnalyzeEffect/AnalyzeEffect.cpp
 * src/Components/Names/StatNames.hpp
 * src/Components/Tags/TypeTags.hpp
 * src/Pokedex/Names.hpp
 * src/PokeSim.hpp
 */


//////////////////// START OF external/entt/config/macro.h /////////////////////

#ifndef ENTT_CONFIG_MACRO_H
#define ENTT_CONFIG_MACRO_H

#define ENTT_STR(arg) #arg
#define ENTT_XSTR(arg) ENTT_STR(arg)

#endif

///////////////////// END OF external/entt/config/macro.h //////////////////////

/////////////////// START OF external/entt/config/version.h ////////////////////

#ifndef ENTT_CONFIG_VERSION_H
#define ENTT_CONFIG_VERSION_H


#define ENTT_VERSION_MAJOR 3
#define ENTT_VERSION_MINOR 12
#define ENTT_VERSION_PATCH 2

#define ENTT_VERSION \
    ENTT_XSTR(ENTT_VERSION_MAJOR) \
    "." ENTT_XSTR(ENTT_VERSION_MINOR) "." ENTT_XSTR(ENTT_VERSION_PATCH)

#endif

//////////////////// END OF external/entt/config/version.h /////////////////////

//////////////////// START OF external/entt/config/config.h ////////////////////

#ifndef ENTT_CONFIG_CONFIG_H
#define ENTT_CONFIG_CONFIG_H


#if defined(__cpp_exceptions) && !defined(ENTT_NOEXCEPTION)
#    define ENTT_CONSTEXPR
#    define ENTT_THROW throw
#    define ENTT_TRY try
#    define ENTT_CATCH catch(...)
#else
#    define ENTT_CONSTEXPR constexpr // use only with throwing functions (waiting for C++20)
#    define ENTT_THROW
#    define ENTT_TRY if(true)
#    define ENTT_CATCH if(false)
#endif

#ifdef ENTT_USE_ATOMIC
#    include <atomic>
#    define ENTT_MAYBE_ATOMIC(Type) std::atomic<Type>
#else
#    define ENTT_MAYBE_ATOMIC(Type) Type
#endif

#ifndef ENTT_ID_TYPE
#    include <cstdint>
#    define ENTT_ID_TYPE std::uint32_t
#endif

#ifndef ENTT_SPARSE_PAGE
#    define ENTT_SPARSE_PAGE 4096
#endif

#ifndef ENTT_PACKED_PAGE
#    define ENTT_PACKED_PAGE 1024
#endif

#ifdef ENTT_DISABLE_ASSERT
#    undef ENTT_ASSERT
#    define ENTT_ASSERT(condition, msg) (void(0))
#elif !defined ENTT_ASSERT
#    include <cassert>
#    define ENTT_ASSERT(condition, msg) assert(condition)
#endif

#ifdef ENTT_DISABLE_ASSERT
#    undef ENTT_ASSERT_CONSTEXPR
#    define ENTT_ASSERT_CONSTEXPR(condition, msg) (void(0))
#elif !defined ENTT_ASSERT_CONSTEXPR
#    define ENTT_ASSERT_CONSTEXPR(condition, msg) ENTT_ASSERT(condition, msg)
#endif

#define ENTT_FAIL(msg) ENTT_ASSERT(false, msg);

#ifdef ENTT_NO_ETO
#    define ENTT_ETO_TYPE(Type) void
#else
#    define ENTT_ETO_TYPE(Type) Type
#endif

#ifdef ENTT_STANDARD_CPP
#    define ENTT_NONSTD false
#else
#    define ENTT_NONSTD true
#    if defined __clang__ || defined __GNUC__
#        define ENTT_PRETTY_FUNCTION __PRETTY_FUNCTION__
#        define ENTT_PRETTY_FUNCTION_PREFIX '='
#        define ENTT_PRETTY_FUNCTION_SUFFIX ']'
#    elif defined _MSC_VER
#        define ENTT_PRETTY_FUNCTION __FUNCSIG__
#        define ENTT_PRETTY_FUNCTION_PREFIX '<'
#        define ENTT_PRETTY_FUNCTION_SUFFIX '>'
#    endif
#endif

#if defined _MSC_VER
#    pragma detect_mismatch("entt.version", ENTT_VERSION)
#    pragma detect_mismatch("entt.noexcept", ENTT_XSTR(ENTT_TRY))
#    pragma detect_mismatch("entt.id", ENTT_XSTR(ENTT_ID_TYPE))
#    pragma detect_mismatch("entt.nonstd", ENTT_XSTR(ENTT_NONSTD))
#endif

#endif

///////////////////// END OF external/entt/config/config.h /////////////////////

/////////////////// START OF external/entt/container/fwd.hpp ///////////////////

#ifndef ENTT_CONTAINER_FWD_HPP
#define ENTT_CONTAINER_FWD_HPP

#include <functional>
#include <memory>

namespace entt {

template<
    typename Key,
    typename Type,
    typename = std::hash<Key>,
    typename = std::equal_to<Key>,
    typename = std::allocator<std::pair<const Key, Type>>>
class dense_map;

template<
    typename Type,
    typename = std::hash<Type>,
    typename = std::equal_to<Type>,
    typename = std::allocator<Type>>
class dense_set;

} // namespace entt

#endif

//////////////////// END OF external/entt/container/fwd.hpp ////////////////////

///////////////////// START OF external/entt/core/fwd.hpp //////////////////////

#ifndef ENTT_CORE_FWD_HPP
#define ENTT_CORE_FWD_HPP

#include <cstddef>

namespace entt {

template<std::size_t Len = sizeof(double[2]), std::size_t = alignof(double[2])>
class basic_any;

/*! @brief Alias declaration for type identifiers. */
using id_type = ENTT_ID_TYPE;

/*! @brief Alias declaration for the most common use case. */
using any = basic_any<>;

} // namespace entt

#endif

////////////////////// END OF external/entt/core/fwd.hpp ///////////////////////

///////////////// START OF external/entt/core/type_traits.hpp //////////////////

#ifndef ENTT_CORE_TYPE_TRAITS_HPP
#define ENTT_CORE_TYPE_TRAITS_HPP

#include <cstddef>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace entt {

/**
 * @brief Utility class to disambiguate overloaded functions.
 * @tparam N Number of choices available.
 */
template<std::size_t N>
struct choice_t
    // Unfortunately, doxygen cannot parse such a construct.
    : /*! @cond TURN_OFF_DOXYGEN */ choice_t<N - 1> /*! @endcond */
{};

/*! @copybrief choice_t */
template<>
struct choice_t<0> {};

/**
 * @brief Variable template for the choice trick.
 * @tparam N Number of choices available.
 */
template<std::size_t N>
inline constexpr choice_t<N> choice{};

/**
 * @brief Identity type trait.
 *
 * Useful to establish non-deduced contexts in template argument deduction
 * (waiting for C++20) or to provide types through function arguments.
 *
 * @tparam Type A type.
 */
template<typename Type>
struct type_identity {
    /*! @brief Identity type. */
    using type = Type;
};

/**
 * @brief Helper type.
 * @tparam Type A type.
 */
template<typename Type>
using type_identity_t = typename type_identity<Type>::type;

/**
 * @brief A type-only `sizeof` wrapper that returns 0 where `sizeof` complains.
 * @tparam Type The type of which to return the size.
 */
template<typename Type, typename = void>
struct size_of: std::integral_constant<std::size_t, 0u> {};

/*! @copydoc size_of */
template<typename Type>
struct size_of<Type, std::void_t<decltype(sizeof(Type))>>
    : std::integral_constant<std::size_t, sizeof(Type)> {};

/**
 * @brief Helper variable template.
 * @tparam Type The type of which to return the size.
 */
template<typename Type>
inline constexpr std::size_t size_of_v = size_of<Type>::value;

/**
 * @brief Using declaration to be used to _repeat_ the same type a number of
 * times equal to the size of a given parameter pack.
 * @tparam Type A type to repeat.
 */
template<typename Type, typename>
using unpack_as_type = Type;

/**
 * @brief Helper variable template to be used to _repeat_ the same value a
 * number of times equal to the size of a given parameter pack.
 * @tparam Value A value to repeat.
 */
template<auto Value, typename>
inline constexpr auto unpack_as_value = Value;

/**
 * @brief Wraps a static constant.
 * @tparam Value A static constant.
 */
template<auto Value>
using integral_constant = std::integral_constant<decltype(Value), Value>;

/**
 * @brief Alias template to facilitate the creation of named values.
 * @tparam Value A constant value at least convertible to `id_type`.
 */
template<id_type Value>
using tag = integral_constant<Value>;

/**
 * @brief A class to use to push around lists of types, nothing more.
 * @tparam Type Types provided by the type list.
 */
template<typename... Type>
struct type_list {
    /*! @brief Type list type. */
    using type = type_list;
    /*! @brief Compile-time number of elements in the type list. */
    static constexpr auto size = sizeof...(Type);
};

/*! @brief Primary template isn't defined on purpose. */
template<std::size_t, typename>
struct type_list_element;

/**
 * @brief Provides compile-time indexed access to the types of a type list.
 * @tparam Index Index of the type to return.
 * @tparam First First type provided by the type list.
 * @tparam Other Other types provided by the type list.
 */
template<std::size_t Index, typename First, typename... Other>
struct type_list_element<Index, type_list<First, Other...>>
    : type_list_element<Index - 1u, type_list<Other...>> {};

/**
 * @brief Provides compile-time indexed access to the types of a type list.
 * @tparam First First type provided by the type list.
 * @tparam Other Other types provided by the type list.
 */
template<typename First, typename... Other>
struct type_list_element<0u, type_list<First, Other...>> {
    /*! @brief Searched type. */
    using type = First;
};

/**
 * @brief Helper type.
 * @tparam Index Index of the type to return.
 * @tparam List Type list to search into.
 */
template<std::size_t Index, typename List>
using type_list_element_t = typename type_list_element<Index, List>::type;

/*! @brief Primary template isn't defined on purpose. */
template<typename, typename>
struct type_list_index;

/**
 * @brief Provides compile-time type access to the types of a type list.
 * @tparam Type Type to look for and for which to return the index.
 * @tparam First First type provided by the type list.
 * @tparam Other Other types provided by the type list.
 */
template<typename Type, typename First, typename... Other>
struct type_list_index<Type, type_list<First, Other...>> {
    /*! @brief Unsigned integer type. */
    using value_type = std::size_t;
    /*! @brief Compile-time position of the given type in the sublist. */
    static constexpr value_type value = 1u + type_list_index<Type, type_list<Other...>>::value;
};

/**
 * @brief Provides compile-time type access to the types of a type list.
 * @tparam Type Type to look for and for which to return the index.
 * @tparam Other Other types provided by the type list.
 */
template<typename Type, typename... Other>
struct type_list_index<Type, type_list<Type, Other...>> {
    static_assert(type_list_index<Type, type_list<Other...>>::value == sizeof...(Other), "Non-unique type");
    /*! @brief Unsigned integer type. */
    using value_type = std::size_t;
    /*! @brief Compile-time position of the given type in the sublist. */
    static constexpr value_type value = 0u;
};

/**
 * @brief Provides compile-time type access to the types of a type list.
 * @tparam Type Type to look for and for which to return the index.
 */
template<typename Type>
struct type_list_index<Type, type_list<>> {
    /*! @brief Unsigned integer type. */
    using value_type = std::size_t;
    /*! @brief Compile-time position of the given type in the sublist. */
    static constexpr value_type value = 0u;
};

/**
 * @brief Helper variable template.
 * @tparam List Type list.
 * @tparam Type Type to look for and for which to return the index.
 */
template<typename Type, typename List>
inline constexpr std::size_t type_list_index_v = type_list_index<Type, List>::value;

/**
 * @brief Concatenates multiple type lists.
 * @tparam Type Types provided by the first type list.
 * @tparam Other Types provided by the second type list.
 * @return A type list composed by the types of both the type lists.
 */
template<typename... Type, typename... Other>
constexpr type_list<Type..., Other...> operator+(type_list<Type...>, type_list<Other...>) {
    return {};
}

/*! @brief Primary template isn't defined on purpose. */
template<typename...>
struct type_list_cat;

/*! @brief Concatenates multiple type lists. */
template<>
struct type_list_cat<> {
    /*! @brief A type list composed by the types of all the type lists. */
    using type = type_list<>;
};

/**
 * @brief Concatenates multiple type lists.
 * @tparam Type Types provided by the first type list.
 * @tparam Other Types provided by the second type list.
 * @tparam List Other type lists, if any.
 */
template<typename... Type, typename... Other, typename... List>
struct type_list_cat<type_list<Type...>, type_list<Other...>, List...> {
    /*! @brief A type list composed by the types of all the type lists. */
    using type = typename type_list_cat<type_list<Type..., Other...>, List...>::type;
};

/**
 * @brief Concatenates multiple type lists.
 * @tparam Type Types provided by the type list.
 */
template<typename... Type>
struct type_list_cat<type_list<Type...>> {
    /*! @brief A type list composed by the types of all the type lists. */
    using type = type_list<Type...>;
};

/**
 * @brief Helper type.
 * @tparam List Type lists to concatenate.
 */
template<typename... List>
using type_list_cat_t = typename type_list_cat<List...>::type;

/*! @brief Primary template isn't defined on purpose. */
template<typename>
struct type_list_unique;

/**
 * @brief Removes duplicates types from a type list.
 * @tparam Type One of the types provided by the given type list.
 * @tparam Other The other types provided by the given type list.
 */
template<typename Type, typename... Other>
struct type_list_unique<type_list<Type, Other...>> {
    /*! @brief A type list without duplicate types. */
    using type = std::conditional_t<
        (std::is_same_v<Type, Other> || ...),
        typename type_list_unique<type_list<Other...>>::type,
        type_list_cat_t<type_list<Type>, typename type_list_unique<type_list<Other...>>::type>>;
};

/*! @brief Removes duplicates types from a type list. */
template<>
struct type_list_unique<type_list<>> {
    /*! @brief A type list without duplicate types. */
    using type = type_list<>;
};

/**
 * @brief Helper type.
 * @tparam Type A type list.
 */
template<typename Type>
using type_list_unique_t = typename type_list_unique<Type>::type;

/**
 * @brief Provides the member constant `value` to true if a type list contains a
 * given type, false otherwise.
 * @tparam List Type list.
 * @tparam Type Type to look for.
 */
template<typename List, typename Type>
struct type_list_contains;

/**
 * @copybrief type_list_contains
 * @tparam Type Types provided by the type list.
 * @tparam Other Type to look for.
 */
template<typename... Type, typename Other>
struct type_list_contains<type_list<Type...>, Other>
    : std::bool_constant<(std::is_same_v<Type, Other> || ...)> {};

/**
 * @brief Helper variable template.
 * @tparam List Type list.
 * @tparam Type Type to look for.
 */
template<typename List, typename Type>
inline constexpr bool type_list_contains_v = type_list_contains<List, Type>::value;

/*! @brief Primary template isn't defined on purpose. */
template<typename...>
struct type_list_diff;

/**
 * @brief Computes the difference between two type lists.
 * @tparam Type Types provided by the first type list.
 * @tparam Other Types provided by the second type list.
 */
template<typename... Type, typename... Other>
struct type_list_diff<type_list<Type...>, type_list<Other...>> {
    /*! @brief A type list that is the difference between the two type lists. */
    using type = type_list_cat_t<std::conditional_t<type_list_contains_v<type_list<Other...>, Type>, type_list<>, type_list<Type>>...>;
};

/**
 * @brief Helper type.
 * @tparam List Type lists between which to compute the difference.
 */
template<typename... List>
using type_list_diff_t = typename type_list_diff<List...>::type;

/*! @brief Primary template isn't defined on purpose. */
template<typename, template<typename...> class>
struct type_list_transform;

/**
 * @brief Applies a given _function_ to a type list and generate a new list.
 * @tparam Type Types provided by the type list.
 * @tparam Op Unary operation as template class with a type member named `type`.
 */
template<typename... Type, template<typename...> class Op>
struct type_list_transform<type_list<Type...>, Op> {
    /*! @brief Resulting type list after applying the transform function. */
    using type = type_list<typename Op<Type>::type...>;
};

/**
 * @brief Helper type.
 * @tparam List Type list.
 * @tparam Op Unary operation as template class with a type member named `type`.
 */
template<typename List, template<typename...> class Op>
using type_list_transform_t = typename type_list_transform<List, Op>::type;

/**
 * @brief A class to use to push around lists of constant values, nothing more.
 * @tparam Value Values provided by the value list.
 */
template<auto... Value>
struct value_list {
    /*! @brief Value list type. */
    using type = value_list;
    /*! @brief Compile-time number of elements in the value list. */
    static constexpr auto size = sizeof...(Value);
};

/*! @brief Primary template isn't defined on purpose. */
template<std::size_t, typename>
struct value_list_element;

/**
 * @brief Provides compile-time indexed access to the values of a value list.
 * @tparam Index Index of the value to return.
 * @tparam Value First value provided by the value list.
 * @tparam Other Other values provided by the value list.
 */
template<std::size_t Index, auto Value, auto... Other>
struct value_list_element<Index, value_list<Value, Other...>>
    : value_list_element<Index - 1u, value_list<Other...>> {};

/**
 * @brief Provides compile-time indexed access to the types of a type list.
 * @tparam Value First value provided by the value list.
 * @tparam Other Other values provided by the value list.
 */
template<auto Value, auto... Other>
struct value_list_element<0u, value_list<Value, Other...>> {
    /*! @brief Searched type. */
    using type = decltype(Value);
    /*! @brief Searched value. */
    static constexpr auto value = Value;
};

/**
 * @brief Helper type.
 * @tparam Index Index of the type to return.
 * @tparam List Value list to search into.
 */
template<std::size_t Index, typename List>
using value_list_element_t = typename value_list_element<Index, List>::type;

/**
 * @brief Helper type.
 * @tparam Index Index of the value to return.
 * @tparam List Value list to search into.
 */
template<std::size_t Index, typename List>
inline constexpr auto value_list_element_v = value_list_element<Index, List>::value;

/*! @brief Primary template isn't defined on purpose. */
template<auto, typename>
struct value_list_index;

/**
 * @brief Provides compile-time type access to the values of a value list.
 * @tparam Value Value to look for and for which to return the index.
 * @tparam First First value provided by the value list.
 * @tparam Other Other values provided by the value list.
 */
template<auto Value, auto First, auto... Other>
struct value_list_index<Value, value_list<First, Other...>> {
    /*! @brief Unsigned integer type. */
    using value_type = std::size_t;
    /*! @brief Compile-time position of the given value in the sublist. */
    static constexpr value_type value = 1u + value_list_index<Value, value_list<Other...>>::value;
};

/**
 * @brief Provides compile-time type access to the values of a value list.
 * @tparam Value Value to look for and for which to return the index.
 * @tparam Other Other values provided by the value list.
 */
template<auto Value, auto... Other>
struct value_list_index<Value, value_list<Value, Other...>> {
    static_assert(value_list_index<Value, value_list<Other...>>::value == sizeof...(Other), "Non-unique type");
    /*! @brief Unsigned integer type. */
    using value_type = std::size_t;
    /*! @brief Compile-time position of the given value in the sublist. */
    static constexpr value_type value = 0u;
};

/**
 * @brief Provides compile-time type access to the values of a value list.
 * @tparam Value Value to look for and for which to return the index.
 */
template<auto Value>
struct value_list_index<Value, value_list<>> {
    /*! @brief Unsigned integer type. */
    using value_type = std::size_t;
    /*! @brief Compile-time position of the given type in the sublist. */
    static constexpr value_type value = 0u;
};

/**
 * @brief Helper variable template.
 * @tparam List Value list.
 * @tparam Value Value to look for and for which to return the index.
 */
template<auto Value, typename List>
inline constexpr std::size_t value_list_index_v = value_list_index<Value, List>::value;

/**
 * @brief Concatenates multiple value lists.
 * @tparam Value Values provided by the first value list.
 * @tparam Other Values provided by the second value list.
 * @return A value list composed by the values of both the value lists.
 */
template<auto... Value, auto... Other>
constexpr value_list<Value..., Other...> operator+(value_list<Value...>, value_list<Other...>) {
    return {};
}

/*! @brief Primary template isn't defined on purpose. */
template<typename...>
struct value_list_cat;

/*! @brief Concatenates multiple value lists. */
template<>
struct value_list_cat<> {
    /*! @brief A value list composed by the values of all the value lists. */
    using type = value_list<>;
};

/**
 * @brief Concatenates multiple value lists.
 * @tparam Value Values provided by the first value list.
 * @tparam Other Values provided by the second value list.
 * @tparam List Other value lists, if any.
 */
template<auto... Value, auto... Other, typename... List>
struct value_list_cat<value_list<Value...>, value_list<Other...>, List...> {
    /*! @brief A value list composed by the values of all the value lists. */
    using type = typename value_list_cat<value_list<Value..., Other...>, List...>::type;
};

/**
 * @brief Concatenates multiple value lists.
 * @tparam Value Values provided by the value list.
 */
template<auto... Value>
struct value_list_cat<value_list<Value...>> {
    /*! @brief A value list composed by the values of all the value lists. */
    using type = value_list<Value...>;
};

/**
 * @brief Helper type.
 * @tparam List Value lists to concatenate.
 */
template<typename... List>
using value_list_cat_t = typename value_list_cat<List...>::type;

/*! @brief Primary template isn't defined on purpose. */
template<typename>
struct value_list_unique;

/**
 * @brief Removes duplicates values from a value list.
 * @tparam Value One of the values provided by the given value list.
 * @tparam Other The other values provided by the given value list.
 */
template<auto Value, auto... Other>
struct value_list_unique<value_list<Value, Other...>> {
    /*! @brief A value list without duplicate types. */
    using type = std::conditional_t<
        ((Value == Other) || ...),
        typename value_list_unique<value_list<Other...>>::type,
        value_list_cat_t<value_list<Value>, typename value_list_unique<value_list<Other...>>::type>>;
};

/*! @brief Removes duplicates values from a value list. */
template<>
struct value_list_unique<value_list<>> {
    /*! @brief A value list without duplicate types. */
    using type = value_list<>;
};

/**
 * @brief Helper type.
 * @tparam Type A value list.
 */
template<typename Type>
using value_list_unique_t = typename value_list_unique<Type>::type;

/**
 * @brief Provides the member constant `value` to true if a value list contains
 * a given value, false otherwise.
 * @tparam List Value list.
 * @tparam Value Value to look for.
 */
template<typename List, auto Value>
struct value_list_contains;

/**
 * @copybrief value_list_contains
 * @tparam Value Values provided by the value list.
 * @tparam Other Value to look for.
 */
template<auto... Value, auto Other>
struct value_list_contains<value_list<Value...>, Other>
    : std::bool_constant<((Value == Other) || ...)> {};

/**
 * @brief Helper variable template.
 * @tparam List Value list.
 * @tparam Value Value to look for.
 */
template<typename List, auto Value>
inline constexpr bool value_list_contains_v = value_list_contains<List, Value>::value;

/*! @brief Primary template isn't defined on purpose. */
template<typename...>
class value_list_diff;

/**
 * @brief Computes the difference between two value lists.
 * @tparam Value Values provided by the first value list.
 * @tparam Other Values provided by the second value list.
 */
template<auto... Value, auto... Other>
class value_list_diff<value_list<Value...>, value_list<Other...>> {
    using v141_toolset_workaround = value_list<Other...>;

public:
    /*! @brief A value list that is the difference between the two value lists. */
    using type = value_list_cat_t<std::conditional_t<value_list_contains_v<v141_toolset_workaround, Value>, value_list<>, value_list<Value>>...>;
};

/**
 * @brief Helper type.
 * @tparam List Value lists between which to compute the difference.
 */
template<typename... List>
using value_list_diff_t = typename value_list_diff<List...>::type;

/*! @brief Same as std::is_invocable, but with tuples. */
template<typename, typename>
struct is_applicable: std::false_type {};

/**
 * @copybrief is_applicable
 * @tparam Func A valid function type.
 * @tparam Tuple Tuple-like type.
 * @tparam Args The list of arguments to use to probe the function type.
 */
template<typename Func, template<typename...> class Tuple, typename... Args>
struct is_applicable<Func, Tuple<Args...>>: std::is_invocable<Func, Args...> {};

/**
 * @copybrief is_applicable
 * @tparam Func A valid function type.
 * @tparam Tuple Tuple-like type.
 * @tparam Args The list of arguments to use to probe the function type.
 */
template<typename Func, template<typename...> class Tuple, typename... Args>
struct is_applicable<Func, const Tuple<Args...>>: std::is_invocable<Func, Args...> {};

/**
 * @brief Helper variable template.
 * @tparam Func A valid function type.
 * @tparam Args The list of arguments to use to probe the function type.
 */
template<typename Func, typename Args>
inline constexpr bool is_applicable_v = is_applicable<Func, Args>::value;

/*! @brief Same as std::is_invocable_r, but with tuples for arguments. */
template<typename, typename, typename>
struct is_applicable_r: std::false_type {};

/**
 * @copybrief is_applicable_r
 * @tparam Ret The type to which the return type of the function should be
 * convertible.
 * @tparam Func A valid function type.
 * @tparam Args The list of arguments to use to probe the function type.
 */
template<typename Ret, typename Func, typename... Args>
struct is_applicable_r<Ret, Func, std::tuple<Args...>>: std::is_invocable_r<Ret, Func, Args...> {};

/**
 * @brief Helper variable template.
 * @tparam Ret The type to which the return type of the function should be
 * convertible.
 * @tparam Func A valid function type.
 * @tparam Args The list of arguments to use to probe the function type.
 */
template<typename Ret, typename Func, typename Args>
inline constexpr bool is_applicable_r_v = is_applicable_r<Ret, Func, Args>::value;

/**
 * @brief Provides the member constant `value` to true if a given type is
 * complete, false otherwise.
 * @tparam Type The type to test.
 */
template<typename Type, typename = void>
struct is_complete: std::false_type {};

/*! @copydoc is_complete */
template<typename Type>
struct is_complete<Type, std::void_t<decltype(sizeof(Type))>>: std::true_type {};

/**
 * @brief Helper variable template.
 * @tparam Type The type to test.
 */
template<typename Type>
inline constexpr bool is_complete_v = is_complete<Type>::value;

/**
 * @brief Provides the member constant `value` to true if a given type is an
 * iterator, false otherwise.
 * @tparam Type The type to test.
 */
template<typename Type, typename = void>
struct is_iterator: std::false_type {};

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename, typename = void>
struct has_iterator_category: std::false_type {};

template<typename Type>
struct has_iterator_category<Type, std::void_t<typename std::iterator_traits<Type>::iterator_category>>: std::true_type {};

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/*! @copydoc is_iterator */
template<typename Type>
struct is_iterator<Type, std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_pointer_t<Type>>, void>>>
    : internal::has_iterator_category<Type> {};

/**
 * @brief Helper variable template.
 * @tparam Type The type to test.
 */
template<typename Type>
inline constexpr bool is_iterator_v = is_iterator<Type>::value;

/**
 * @brief Provides the member constant `value` to true if a given type is both
 * an empty and non-final class, false otherwise.
 * @tparam Type The type to test
 */
template<typename Type>
struct is_ebco_eligible
    : std::bool_constant<std::is_empty_v<Type> && !std::is_final_v<Type>> {};

/**
 * @brief Helper variable template.
 * @tparam Type The type to test.
 */
template<typename Type>
inline constexpr bool is_ebco_eligible_v = is_ebco_eligible<Type>::value;

/**
 * @brief Provides the member constant `value` to true if `Type::is_transparent`
 * is valid and denotes a type, false otherwise.
 * @tparam Type The type to test.
 */
template<typename Type, typename = void>
struct is_transparent: std::false_type {};

/*! @copydoc is_transparent */
template<typename Type>
struct is_transparent<Type, std::void_t<typename Type::is_transparent>>: std::true_type {};

/**
 * @brief Helper variable template.
 * @tparam Type The type to test.
 */
template<typename Type>
inline constexpr bool is_transparent_v = is_transparent<Type>::value;

/**
 * @brief Provides the member constant `value` to true if a given type is
 * equality comparable, false otherwise.
 * @tparam Type The type to test.
 */
template<typename Type, typename = void>
struct is_equality_comparable: std::false_type {};

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename, typename = void>
struct has_tuple_size_value: std::false_type {};

template<typename Type>
struct has_tuple_size_value<Type, std::void_t<decltype(std::tuple_size<const Type>::value)>>: std::true_type {};

template<typename Type, std::size_t... Index>
[[nodiscard]] constexpr bool unpack_maybe_equality_comparable(std::index_sequence<Index...>) {
    return (is_equality_comparable<std::tuple_element_t<Index, Type>>::value && ...);
}

template<typename>
[[nodiscard]] constexpr bool maybe_equality_comparable(choice_t<0>) {
    return true;
}

template<typename Type>
[[nodiscard]] constexpr auto maybe_equality_comparable(choice_t<1>) -> decltype(std::declval<typename Type::value_type>(), bool{}) {
    if constexpr(is_iterator_v<Type>) {
        return true;
    } else if constexpr(std::is_same_v<typename Type::value_type, Type>) {
        return maybe_equality_comparable<Type>(choice<0>);
    } else {
        return is_equality_comparable<typename Type::value_type>::value;
    }
}

template<typename Type>
[[nodiscard]] constexpr std::enable_if_t<is_complete_v<std::tuple_size<std::remove_cv_t<Type>>>, bool> maybe_equality_comparable(choice_t<2>) {
    if constexpr(has_tuple_size_value<Type>::value) {
        return unpack_maybe_equality_comparable<Type>(std::make_index_sequence<std::tuple_size<Type>::value>{});
    } else {
        return maybe_equality_comparable<Type>(choice<1>);
    }
}

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/*! @copydoc is_equality_comparable */
template<typename Type>
struct is_equality_comparable<Type, std::void_t<decltype(std::declval<Type>() == std::declval<Type>())>>
    : std::bool_constant<internal::maybe_equality_comparable<Type>(choice<2>)> {};

/*! @copydoc is_equality_comparable */
template<typename Type, auto N>
struct is_equality_comparable<Type[N]>: std::false_type {};

/**
 * @brief Helper variable template.
 * @tparam Type The type to test.
 */
template<typename Type>
inline constexpr bool is_equality_comparable_v = is_equality_comparable<Type>::value;

/**
 * @brief Transcribes the constness of a type to another type.
 * @tparam To The type to which to transcribe the constness.
 * @tparam From The type from which to transcribe the constness.
 */
template<typename To, typename From>
struct constness_as {
    /*! @brief The type resulting from the transcription of the constness. */
    using type = std::remove_const_t<To>;
};

/*! @copydoc constness_as */
template<typename To, typename From>
struct constness_as<To, const From> {
    /*! @brief The type resulting from the transcription of the constness. */
    using type = const To;
};

/**
 * @brief Alias template to facilitate the transcription of the constness.
 * @tparam To The type to which to transcribe the constness.
 * @tparam From The type from which to transcribe the constness.
 */
template<typename To, typename From>
using constness_as_t = typename constness_as<To, From>::type;

/**
 * @brief Extracts the class of a non-static member object or function.
 * @tparam Member A pointer to a non-static member object or function.
 */
template<typename Member>
class member_class {
    static_assert(std::is_member_pointer_v<Member>, "Invalid pointer type to non-static member object or function");

    template<typename Class, typename Ret, typename... Args>
    static Class *clazz(Ret (Class::*)(Args...));

    template<typename Class, typename Ret, typename... Args>
    static Class *clazz(Ret (Class::*)(Args...) const);

    template<typename Class, typename Type>
    static Class *clazz(Type Class::*);

public:
    /*! @brief The class of the given non-static member object or function. */
    using type = std::remove_pointer_t<decltype(clazz(std::declval<Member>()))>;
};

/**
 * @brief Helper type.
 * @tparam Member A pointer to a non-static member object or function.
 */
template<typename Member>
using member_class_t = typename member_class<Member>::type;

/**
 * @brief Extracts the n-th argument of a given function or member function.
 * @tparam Index The index of the argument to extract.
 * @tparam Candidate A valid function, member function or data member.
 */
template<std::size_t Index, auto Candidate>
class nth_argument {
    template<typename Ret, typename... Args>
    static constexpr type_list<Args...> pick_up(Ret (*)(Args...));

    template<typename Ret, typename Class, typename... Args>
    static constexpr type_list<Args...> pick_up(Ret (Class ::*)(Args...));

    template<typename Ret, typename Class, typename... Args>
    static constexpr type_list<Args...> pick_up(Ret (Class ::*)(Args...) const);

    template<typename Type, typename Class>
    static constexpr type_list<Type> pick_up(Type Class ::*);

public:
    /*! @brief N-th argument of the given function or member function. */
    using type = type_list_element_t<Index, decltype(pick_up(Candidate))>;
};

/**
 * @brief Helper type.
 * @tparam Index The index of the argument to extract.
 * @tparam Candidate A valid function, member function or data member.
 */
template<std::size_t Index, auto Candidate>
using nth_argument_t = typename nth_argument<Index, Candidate>::type;

} // namespace entt

template<typename... Type>
struct std::tuple_size<entt::type_list<Type...>>: std::integral_constant<std::size_t, entt::type_list<Type...>::size> {};

template<std::size_t Index, typename... Type>
struct std::tuple_element<Index, entt::type_list<Type...>>: entt::type_list_element<Index, entt::type_list<Type...>> {};

template<auto... Value>
struct std::tuple_size<entt::value_list<Value...>>: std::integral_constant<std::size_t, entt::value_list<Value...>::size> {};

template<std::size_t Index, auto... Value>
struct std::tuple_element<Index, entt::value_list<Value...>>: entt::value_list_element<Index, entt::value_list<Value...>> {};

#endif

////////////////// END OF external/entt/core/type_traits.hpp ///////////////////

/////////////// START OF external/entt/core/compressed_pair.hpp ////////////////

#ifndef ENTT_CORE_COMPRESSED_PAIR_HPP
#define ENTT_CORE_COMPRESSED_PAIR_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename Type, std::size_t, typename = void>
struct compressed_pair_element {
    using reference = Type &;
    using const_reference = const Type &;

    template<bool Dummy = true, typename = std::enable_if_t<Dummy && std::is_default_constructible_v<Type>>>
    constexpr compressed_pair_element() noexcept(std::is_nothrow_default_constructible_v<Type>)
        : value{} {}

    template<typename Arg, typename = std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Arg>>, compressed_pair_element>>>
    constexpr compressed_pair_element(Arg &&arg) noexcept(std::is_nothrow_constructible_v<Type, Arg>)
        : value{std::forward<Arg>(arg)} {}

    template<typename... Args, std::size_t... Index>
    constexpr compressed_pair_element(std::tuple<Args...> args, std::index_sequence<Index...>) noexcept(std::is_nothrow_constructible_v<Type, Args...>)
        : value{std::forward<Args>(std::get<Index>(args))...} {}

    [[nodiscard]] constexpr reference get() noexcept {
        return value;
    }

    [[nodiscard]] constexpr const_reference get() const noexcept {
        return value;
    }

private:
    Type value;
};

template<typename Type, std::size_t Tag>
struct compressed_pair_element<Type, Tag, std::enable_if_t<is_ebco_eligible_v<Type>>>: Type {
    using reference = Type &;
    using const_reference = const Type &;
    using base_type = Type;

    template<bool Dummy = true, typename = std::enable_if_t<Dummy && std::is_default_constructible_v<base_type>>>
    constexpr compressed_pair_element() noexcept(std::is_nothrow_default_constructible_v<base_type>)
        : base_type{} {}

    template<typename Arg, typename = std::enable_if_t<!std::is_same_v<std::remove_cv_t<std::remove_reference_t<Arg>>, compressed_pair_element>>>
    constexpr compressed_pair_element(Arg &&arg) noexcept(std::is_nothrow_constructible_v<base_type, Arg>)
        : base_type{std::forward<Arg>(arg)} {}

    template<typename... Args, std::size_t... Index>
    constexpr compressed_pair_element(std::tuple<Args...> args, std::index_sequence<Index...>) noexcept(std::is_nothrow_constructible_v<base_type, Args...>)
        : base_type{std::forward<Args>(std::get<Index>(args))...} {}

    [[nodiscard]] constexpr reference get() noexcept {
        return *this;
    }

    [[nodiscard]] constexpr const_reference get() const noexcept {
        return *this;
    }
};

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief A compressed pair.
 *
 * A pair that exploits the _Empty Base Class Optimization_ (or _EBCO_) to
 * reduce its final size to a minimum.
 *
 * @tparam First The type of the first element that the pair stores.
 * @tparam Second The type of the second element that the pair stores.
 */
template<typename First, typename Second>
class compressed_pair final
    : internal::compressed_pair_element<First, 0u>,
      internal::compressed_pair_element<Second, 1u> {
    using first_base = internal::compressed_pair_element<First, 0u>;
    using second_base = internal::compressed_pair_element<Second, 1u>;

public:
    /*! @brief The type of the first element that the pair stores. */
    using first_type = First;
    /*! @brief The type of the second element that the pair stores. */
    using second_type = Second;

    /**
     * @brief Default constructor, conditionally enabled.
     *
     * This constructor is only available when the types that the pair stores
     * are both at least default constructible.
     *
     * @tparam Dummy Dummy template parameter used for internal purposes.
     */
    template<bool Dummy = true, typename = std::enable_if_t<Dummy && std::is_default_constructible_v<first_type> && std::is_default_constructible_v<second_type>>>
    constexpr compressed_pair() noexcept(std::is_nothrow_default_constructible_v<first_base> &&std::is_nothrow_default_constructible_v<second_base>)
        : first_base{},
          second_base{} {}

    /**
     * @brief Copy constructor.
     * @param other The instance to copy from.
     */
    constexpr compressed_pair(const compressed_pair &other) noexcept(std::is_nothrow_copy_constructible_v<first_base> &&std::is_nothrow_copy_constructible_v<second_base>) = default;

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    constexpr compressed_pair(compressed_pair &&other) noexcept(std::is_nothrow_move_constructible_v<first_base> &&std::is_nothrow_move_constructible_v<second_base>) = default;

    /**
     * @brief Constructs a pair from its values.
     * @tparam Arg Type of value to use to initialize the first element.
     * @tparam Other Type of value to use to initialize the second element.
     * @param arg Value to use to initialize the first element.
     * @param other Value to use to initialize the second element.
     */
    template<typename Arg, typename Other>
    constexpr compressed_pair(Arg &&arg, Other &&other) noexcept(std::is_nothrow_constructible_v<first_base, Arg> &&std::is_nothrow_constructible_v<second_base, Other>)
        : first_base{std::forward<Arg>(arg)},
          second_base{std::forward<Other>(other)} {}

    /**
     * @brief Constructs a pair by forwarding the arguments to its parts.
     * @tparam Args Types of arguments to use to initialize the first element.
     * @tparam Other Types of arguments to use to initialize the second element.
     * @param args Arguments to use to initialize the first element.
     * @param other Arguments to use to initialize the second element.
     */
    template<typename... Args, typename... Other>
    constexpr compressed_pair(std::piecewise_construct_t, std::tuple<Args...> args, std::tuple<Other...> other) noexcept(std::is_nothrow_constructible_v<first_base, Args...> &&std::is_nothrow_constructible_v<second_base, Other...>)
        : first_base{std::move(args), std::index_sequence_for<Args...>{}},
          second_base{std::move(other), std::index_sequence_for<Other...>{}} {}

    /**
     * @brief Copy assignment operator.
     * @param other The instance to copy from.
     * @return This compressed pair object.
     */
    constexpr compressed_pair &operator=(const compressed_pair &other) noexcept(std::is_nothrow_copy_assignable_v<first_base> &&std::is_nothrow_copy_assignable_v<second_base>) = default;

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This compressed pair object.
     */
    constexpr compressed_pair &operator=(compressed_pair &&other) noexcept(std::is_nothrow_move_assignable_v<first_base> &&std::is_nothrow_move_assignable_v<second_base>) = default;

    /**
     * @brief Returns the first element that a pair stores.
     * @return The first element that a pair stores.
     */
    [[nodiscard]] constexpr first_type &first() noexcept {
        return static_cast<first_base &>(*this).get();
    }

    /*! @copydoc first */
    [[nodiscard]] constexpr const first_type &first() const noexcept {
        return static_cast<const first_base &>(*this).get();
    }

    /**
     * @brief Returns the second element that a pair stores.
     * @return The second element that a pair stores.
     */
    [[nodiscard]] constexpr second_type &second() noexcept {
        return static_cast<second_base &>(*this).get();
    }

    /*! @copydoc second */
    [[nodiscard]] constexpr const second_type &second() const noexcept {
        return static_cast<const second_base &>(*this).get();
    }

    /**
     * @brief Swaps two compressed pair objects.
     * @param other The compressed pair to swap with.
     */
    constexpr void swap(compressed_pair &other) noexcept(std::is_nothrow_swappable_v<first_type> &&std::is_nothrow_swappable_v<second_type>) {
        using std::swap;
        swap(first(), other.first());
        swap(second(), other.second());
    }

    /**
     * @brief Extracts an element from the compressed pair.
     * @tparam Index An integer value that is either 0 or 1.
     * @return Returns a reference to the first element if `Index` is 0 and a
     * reference to the second element if `Index` is 1.
     */
    template<std::size_t Index>
    constexpr decltype(auto) get() noexcept {
        if constexpr(Index == 0u) {
            return first();
        } else {
            static_assert(Index == 1u, "Index out of bounds");
            return second();
        }
    }

    /*! @copydoc get */
    template<std::size_t Index>
    constexpr decltype(auto) get() const noexcept {
        if constexpr(Index == 0u) {
            return first();
        } else {
            static_assert(Index == 1u, "Index out of bounds");
            return second();
        }
    }
};

/**
 * @brief Deduction guide.
 * @tparam Type Type of value to use to initialize the first element.
 * @tparam Other Type of value to use to initialize the second element.
 */
template<typename Type, typename Other>
compressed_pair(Type &&, Other &&) -> compressed_pair<std::decay_t<Type>, std::decay_t<Other>>;

/**
 * @brief Swaps two compressed pair objects.
 * @tparam First The type of the first element that the pairs store.
 * @tparam Second The type of the second element that the pairs store.
 * @param lhs A valid compressed pair object.
 * @param rhs A valid compressed pair object.
 */
template<typename First, typename Second>
inline constexpr void swap(compressed_pair<First, Second> &lhs, compressed_pair<First, Second> &rhs) {
    lhs.swap(rhs);
}

} // namespace entt

// disable structured binding support for clang 6, it messes when specializing tuple_size
#if !defined __clang_major__ || __clang_major__ > 6
namespace std {

/**
 * @brief `std::tuple_size` specialization for `compressed_pair`s.
 * @tparam First The type of the first element that the pair stores.
 * @tparam Second The type of the second element that the pair stores.
 */
template<typename First, typename Second>
struct tuple_size<entt::compressed_pair<First, Second>>: integral_constant<size_t, 2u> {};

/**
 * @brief `std::tuple_element` specialization for `compressed_pair`s.
 * @tparam Index The index of the type to return.
 * @tparam First The type of the first element that the pair stores.
 * @tparam Second The type of the second element that the pair stores.
 */
template<size_t Index, typename First, typename Second>
struct tuple_element<Index, entt::compressed_pair<First, Second>>: conditional<Index == 0u, First, Second> {
    static_assert(Index < 2u, "Index out of bounds");
};

} // namespace std
#endif

#endif

//////////////// END OF external/entt/core/compressed_pair.hpp /////////////////

/////////////////// START OF external/entt/core/iterator.hpp ///////////////////

#ifndef ENTT_CORE_ITERATOR_HPP
#define ENTT_CORE_ITERATOR_HPP

#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

namespace entt {

/**
 * @brief Helper type to use as pointer with input iterators.
 * @tparam Type of wrapped value.
 */
template<typename Type>
struct input_iterator_pointer final {
    /*! @brief Value type. */
    using value_type = Type;
    /*! @brief Pointer type. */
    using pointer = Type *;
    /*! @brief Reference type. */
    using reference = Type &;

    /**
     * @brief Constructs a proxy object by move.
     * @param val Value to use to initialize the proxy object.
     */
    constexpr input_iterator_pointer(value_type &&val) noexcept(std::is_nothrow_move_constructible_v<value_type>)
        : value{std::move(val)} {}

    /**
     * @brief Access operator for accessing wrapped values.
     * @return A pointer to the wrapped value.
     */
    [[nodiscard]] constexpr pointer operator->() noexcept {
        return std::addressof(value);
    }

    /**
     * @brief Dereference operator for accessing wrapped values.
     * @return A reference to the wrapped value.
     */
    [[nodiscard]] constexpr reference operator*() noexcept {
        return value;
    }

private:
    Type value;
};

/**
 * @brief Plain iota iterator (waiting for C++20).
 * @tparam Type Value type.
 */
template<typename Type>
class iota_iterator final {
    static_assert(std::is_integral_v<Type>, "Not an integral type");

public:
    /*! @brief Value type, likely an integral one. */
    using value_type = Type;
    /*! @brief Invalid pointer type. */
    using pointer = void;
    /*! @brief Non-reference type, same as value type. */
    using reference = value_type;
    /*! @brief Difference type. */
    using difference_type = std::ptrdiff_t;
    /*! @brief Iterator category. */
    using iterator_category = std::input_iterator_tag;

    /*! @brief Default constructor. */
    constexpr iota_iterator() noexcept
        : current{} {}

    /**
     * @brief Constructs an iota iterator from a given value.
     * @param init The initial value assigned to the iota iterator.
     */
    constexpr iota_iterator(const value_type init) noexcept
        : current{init} {}

    /**
     * @brief Pre-increment operator.
     * @return This iota iterator.
     */
    constexpr iota_iterator &operator++() noexcept {
        return ++current, *this;
    }

    /**
     * @brief Post-increment operator.
     * @return This iota iterator.
     */
    constexpr iota_iterator operator++(int) noexcept {
        iota_iterator orig = *this;
        return ++(*this), orig;
    }

    /**
     * @brief Dereference operator.
     * @return The underlying value.
     */
    [[nodiscard]] constexpr reference operator*() const noexcept {
        return current;
    }

private:
    value_type current;
};

/**
 * @brief Comparison operator.
 * @tparam Type Value type of the iota iterator.
 * @param lhs A properly initialized iota iterator.
 * @param rhs A properly initialized iota iterator.
 * @return True if the two iterators are identical, false otherwise.
 */
template<typename Type>
[[nodiscard]] constexpr bool operator==(const iota_iterator<Type> &lhs, const iota_iterator<Type> &rhs) noexcept {
    return *lhs == *rhs;
}

/**
 * @brief Comparison operator.
 * @tparam Type Value type of the iota iterator.
 * @param lhs A properly initialized iota iterator.
 * @param rhs A properly initialized iota iterator.
 * @return True if the two iterators differ, false otherwise.
 */
template<typename Type>
[[nodiscard]] constexpr bool operator!=(const iota_iterator<Type> &lhs, const iota_iterator<Type> &rhs) noexcept {
    return !(lhs == rhs);
}

/**
 * @brief Utility class to create an iterable object from a pair of iterators.
 * @tparam It Type of iterator.
 * @tparam Sentinel Type of sentinel.
 */
template<typename It, typename Sentinel = It>
struct iterable_adaptor final {
    /*! @brief Value type. */
    using value_type = typename std::iterator_traits<It>::value_type;
    /*! @brief Iterator type. */
    using iterator = It;
    /*! @brief Sentinel type. */
    using sentinel = Sentinel;

    /*! @brief Default constructor. */
    constexpr iterable_adaptor() noexcept(std::is_nothrow_default_constructible_v<iterator> &&std::is_nothrow_default_constructible_v<sentinel>)
        : first{},
          last{} {}

    /**
     * @brief Creates an iterable object from a pair of iterators.
     * @param from Begin iterator.
     * @param to End iterator.
     */
    constexpr iterable_adaptor(iterator from, sentinel to) noexcept(std::is_nothrow_move_constructible_v<iterator> &&std::is_nothrow_move_constructible_v<sentinel>)
        : first{std::move(from)},
          last{std::move(to)} {}

    /**
     * @brief Returns an iterator to the beginning.
     * @return An iterator to the first element of the range.
     */
    [[nodiscard]] constexpr iterator begin() const noexcept {
        return first;
    }

    /**
     * @brief Returns an iterator to the end.
     * @return An iterator to the element following the last element of the
     * range.
     */
    [[nodiscard]] constexpr sentinel end() const noexcept {
        return last;
    }

    /*! @copydoc begin */
    [[nodiscard]] constexpr iterator cbegin() const noexcept {
        return begin();
    }

    /*! @copydoc end */
    [[nodiscard]] constexpr sentinel cend() const noexcept {
        return end();
    }

private:
    It first;
    Sentinel last;
};

} // namespace entt

#endif

//////////////////// END OF external/entt/core/iterator.hpp ////////////////////

//////////////////// START OF external/entt/core/memory.hpp ////////////////////

#ifndef ENTT_CORE_MEMORY_HPP
#define ENTT_CORE_MEMORY_HPP

#include <cstddef>
#include <limits>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace entt {

/**
 * @brief Checks whether a value is a power of two or not.
 * @param value A value that may or may not be a power of two.
 * @return True if the value is a power of two, false otherwise.
 */
[[nodiscard]] inline constexpr bool is_power_of_two(const std::size_t value) noexcept {
    return value && ((value & (value - 1)) == 0);
}

/**
 * @brief Computes the smallest power of two greater than or equal to a value.
 * @param value The value to use.
 * @return The smallest power of two greater than or equal to the given value.
 */
[[nodiscard]] inline constexpr std::size_t next_power_of_two(const std::size_t value) noexcept {
    ENTT_ASSERT_CONSTEXPR(value < (std::size_t{1u} << (std::numeric_limits<std::size_t>::digits - 1)), "Numeric limits exceeded");
    std::size_t curr = value - (value != 0u);

    for(int next = 1; next < std::numeric_limits<std::size_t>::digits; next = next * 2) {
        curr |= curr >> next;
    }

    return ++curr;
}

/**
 * @brief Fast module utility function (powers of two only).
 * @param value A value for which to calculate the modulus.
 * @param mod _Modulus_, it must be a power of two.
 * @return The common remainder.
 */
[[nodiscard]] inline constexpr std::size_t fast_mod(const std::size_t value, const std::size_t mod) noexcept {
    ENTT_ASSERT_CONSTEXPR(is_power_of_two(mod), "Value must be a power of two");
    return value & (mod - 1u);
}

/**
 * @brief Unwraps fancy pointers, does nothing otherwise (waiting for C++20).
 * @tparam Type Pointer type.
 * @param ptr Fancy or raw pointer.
 * @return A raw pointer that represents the address of the original pointer.
 */
template<typename Type>
[[nodiscard]] constexpr auto to_address(Type &&ptr) noexcept {
    if constexpr(std::is_pointer_v<std::decay_t<Type>>) {
        return ptr;
    } else {
        return to_address(std::forward<Type>(ptr).operator->());
    }
}

/**
 * @brief Utility function to design allocation-aware containers.
 * @tparam Allocator Type of allocator.
 * @param lhs A valid allocator.
 * @param rhs Another valid allocator.
 */
template<typename Allocator>
constexpr void propagate_on_container_copy_assignment([[maybe_unused]] Allocator &lhs, [[maybe_unused]] Allocator &rhs) noexcept {
    if constexpr(std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value) {
        lhs = rhs;
    }
}

/**
 * @brief Utility function to design allocation-aware containers.
 * @tparam Allocator Type of allocator.
 * @param lhs A valid allocator.
 * @param rhs Another valid allocator.
 */
template<typename Allocator>
constexpr void propagate_on_container_move_assignment([[maybe_unused]] Allocator &lhs, [[maybe_unused]] Allocator &rhs) noexcept {
    if constexpr(std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value) {
        lhs = std::move(rhs);
    }
}

/**
 * @brief Utility function to design allocation-aware containers.
 * @tparam Allocator Type of allocator.
 * @param lhs A valid allocator.
 * @param rhs Another valid allocator.
 */
template<typename Allocator>
constexpr void propagate_on_container_swap([[maybe_unused]] Allocator &lhs, [[maybe_unused]] Allocator &rhs) noexcept {
    if constexpr(std::allocator_traits<Allocator>::propagate_on_container_swap::value) {
        using std::swap;
        swap(lhs, rhs);
    } else {
        ENTT_ASSERT_CONSTEXPR(lhs == rhs, "Cannot swap the containers");
    }
}

/**
 * @brief Deleter for allocator-aware unique pointers (waiting for C++20).
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Allocator>
struct allocation_deleter: private Allocator {
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Pointer type. */
    using pointer = typename std::allocator_traits<Allocator>::pointer;

    /**
     * @brief Inherited constructors.
     * @param alloc The allocator to use.
     */
    constexpr allocation_deleter(const allocator_type &alloc) noexcept(std::is_nothrow_copy_constructible_v<allocator_type>)
        : Allocator{alloc} {}

    /**
     * @brief Destroys the pointed object and deallocates its memory.
     * @param ptr A valid pointer to an object of the given type.
     */
    constexpr void operator()(pointer ptr) noexcept(std::is_nothrow_destructible_v<typename allocator_type::value_type>) {
        using alloc_traits = std::allocator_traits<Allocator>;
        alloc_traits::destroy(*this, to_address(ptr));
        alloc_traits::deallocate(*this, ptr, 1u);
    }
};

/**
 * @brief Allows `std::unique_ptr` to use allocators (waiting for C++20).
 * @tparam Type Type of object to allocate for and to construct.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 * @tparam Args Types of arguments to use to construct the object.
 * @param allocator The allocator to use.
 * @param args Parameters to use to construct the object.
 * @return A properly initialized unique pointer with a custom deleter.
 */
template<typename Type, typename Allocator, typename... Args>
ENTT_CONSTEXPR auto allocate_unique(Allocator &allocator, Args &&...args) {
    static_assert(!std::is_array_v<Type>, "Array types are not supported");

    using alloc_traits = typename std::allocator_traits<Allocator>::template rebind_traits<Type>;
    using allocator_type = typename alloc_traits::allocator_type;

    allocator_type alloc{allocator};
    auto ptr = alloc_traits::allocate(alloc, 1u);

    ENTT_TRY {
        alloc_traits::construct(alloc, to_address(ptr), std::forward<Args>(args)...);
    }
    ENTT_CATCH {
        alloc_traits::deallocate(alloc, ptr, 1u);
        ENTT_THROW;
    }

    return std::unique_ptr<Type, allocation_deleter<allocator_type>>{ptr, alloc};
}

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename Type>
struct uses_allocator_construction {
    template<typename Allocator, typename... Params>
    static constexpr auto args([[maybe_unused]] const Allocator &allocator, Params &&...params) noexcept {
        if constexpr(!std::uses_allocator_v<Type, Allocator> && std::is_constructible_v<Type, Params...>) {
            return std::forward_as_tuple(std::forward<Params>(params)...);
        } else {
            static_assert(std::uses_allocator_v<Type, Allocator>, "Ill-formed request");

            if constexpr(std::is_constructible_v<Type, std::allocator_arg_t, const Allocator &, Params...>) {
                return std::tuple<std::allocator_arg_t, const Allocator &, Params &&...>{std::allocator_arg, allocator, std::forward<Params>(params)...};
            } else {
                static_assert(std::is_constructible_v<Type, Params..., const Allocator &>, "Ill-formed request");
                return std::forward_as_tuple(std::forward<Params>(params)..., allocator);
            }
        }
    }
};

template<typename Type, typename Other>
struct uses_allocator_construction<std::pair<Type, Other>> {
    using type = std::pair<Type, Other>;

    template<typename Allocator, typename First, typename Second>
    static constexpr auto args(const Allocator &allocator, std::piecewise_construct_t, First &&first, Second &&second) noexcept {
        return std::make_tuple(
            std::piecewise_construct,
            std::apply([&allocator](auto &&...curr) { return uses_allocator_construction<Type>::args(allocator, std::forward<decltype(curr)>(curr)...); }, std::forward<First>(first)),
            std::apply([&allocator](auto &&...curr) { return uses_allocator_construction<Other>::args(allocator, std::forward<decltype(curr)>(curr)...); }, std::forward<Second>(second)));
    }

    template<typename Allocator>
    static constexpr auto args(const Allocator &allocator) noexcept {
        return uses_allocator_construction<type>::args(allocator, std::piecewise_construct, std::tuple<>{}, std::tuple<>{});
    }

    template<typename Allocator, typename First, typename Second>
    static constexpr auto args(const Allocator &allocator, First &&first, Second &&second) noexcept {
        return uses_allocator_construction<type>::args(allocator, std::piecewise_construct, std::forward_as_tuple(std::forward<First>(first)), std::forward_as_tuple(std::forward<Second>(second)));
    }

    template<typename Allocator, typename First, typename Second>
    static constexpr auto args(const Allocator &allocator, const std::pair<First, Second> &value) noexcept {
        return uses_allocator_construction<type>::args(allocator, std::piecewise_construct, std::forward_as_tuple(value.first), std::forward_as_tuple(value.second));
    }

    template<typename Allocator, typename First, typename Second>
    static constexpr auto args(const Allocator &allocator, std::pair<First, Second> &&value) noexcept {
        return uses_allocator_construction<type>::args(allocator, std::piecewise_construct, std::forward_as_tuple(std::move(value.first)), std::forward_as_tuple(std::move(value.second)));
    }
};

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Uses-allocator construction utility (waiting for C++20).
 *
 * Primarily intended for internal use. Prepares the argument list needed to
 * create an object of a given type by means of uses-allocator construction.
 *
 * @tparam Type Type to return arguments for.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 * @tparam Args Types of arguments to use to construct the object.
 * @param allocator The allocator to use.
 * @param args Parameters to use to construct the object.
 * @return The arguments needed to create an object of the given type.
 */
template<typename Type, typename Allocator, typename... Args>
constexpr auto uses_allocator_construction_args(const Allocator &allocator, Args &&...args) noexcept {
    return internal::uses_allocator_construction<Type>::args(allocator, std::forward<Args>(args)...);
}

/**
 * @brief Uses-allocator construction utility (waiting for C++20).
 *
 * Primarily intended for internal use. Creates an object of a given type by
 * means of uses-allocator construction.
 *
 * @tparam Type Type of object to create.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 * @tparam Args Types of arguments to use to construct the object.
 * @param allocator The allocator to use.
 * @param args Parameters to use to construct the object.
 * @return A newly created object of the given type.
 */
template<typename Type, typename Allocator, typename... Args>
constexpr Type make_obj_using_allocator(const Allocator &allocator, Args &&...args) {
    return std::make_from_tuple<Type>(internal::uses_allocator_construction<Type>::args(allocator, std::forward<Args>(args)...));
}

/**
 * @brief Uses-allocator construction utility (waiting for C++20).
 *
 * Primarily intended for internal use. Creates an object of a given type by
 * means of uses-allocator construction at an uninitialized memory location.
 *
 * @tparam Type Type of object to create.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 * @tparam Args Types of arguments to use to construct the object.
 * @param value Memory location in which to place the object.
 * @param allocator The allocator to use.
 * @param args Parameters to use to construct the object.
 * @return A pointer to the newly created object of the given type.
 */
template<typename Type, typename Allocator, typename... Args>
constexpr Type *uninitialized_construct_using_allocator(Type *value, const Allocator &allocator, Args &&...args) {
    return std::apply([value](auto &&...curr) { return new(value) Type(std::forward<decltype(curr)>(curr)...); }, internal::uses_allocator_construction<Type>::args(allocator, std::forward<Args>(args)...));
}

} // namespace entt

#endif

///////////////////// END OF external/entt/core/memory.hpp /////////////////////

//////////////// START OF external/entt/container/dense_map.hpp ////////////////

#ifndef ENTT_CONTAINER_DENSE_MAP_HPP
#define ENTT_CONTAINER_DENSE_MAP_HPP

#include <cmath>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename Key, typename Type>
struct dense_map_node final {
    using value_type = std::pair<Key, Type>;

    template<typename... Args>
    dense_map_node(const std::size_t pos, Args &&...args)
        : next{pos},
          element{std::forward<Args>(args)...} {}

    template<typename Allocator, typename... Args>
    dense_map_node(std::allocator_arg_t, const Allocator &allocator, const std::size_t pos, Args &&...args)
        : next{pos},
          element{entt::make_obj_using_allocator<value_type>(allocator, std::forward<Args>(args)...)} {}

    template<typename Allocator>
    dense_map_node(std::allocator_arg_t, const Allocator &allocator, const dense_map_node &other)
        : next{other.next},
          element{entt::make_obj_using_allocator<value_type>(allocator, other.element)} {}

    template<typename Allocator>
    dense_map_node(std::allocator_arg_t, const Allocator &allocator, dense_map_node &&other)
        : next{other.next},
          element{entt::make_obj_using_allocator<value_type>(allocator, std::move(other.element))} {}

    std::size_t next;
    value_type element;
};

template<typename It>
class dense_map_iterator final {
    template<typename>
    friend class dense_map_iterator;

    using first_type = decltype(std::as_const(std::declval<It>()->element.first));
    using second_type = decltype((std::declval<It>()->element.second));

public:
    using value_type = std::pair<first_type, second_type>;
    using pointer = input_iterator_pointer<value_type>;
    using reference = value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    constexpr dense_map_iterator() noexcept
        : it{} {}

    constexpr dense_map_iterator(const It iter) noexcept
        : it{iter} {}

    template<typename Other, typename = std::enable_if_t<!std::is_same_v<It, Other> && std::is_constructible_v<It, Other>>>
    constexpr dense_map_iterator(const dense_map_iterator<Other> &other) noexcept
        : it{other.it} {}

    constexpr dense_map_iterator &operator++() noexcept {
        return ++it, *this;
    }

    constexpr dense_map_iterator operator++(int) noexcept {
        dense_map_iterator orig = *this;
        return ++(*this), orig;
    }

    constexpr dense_map_iterator &operator--() noexcept {
        return --it, *this;
    }

    constexpr dense_map_iterator operator--(int) noexcept {
        dense_map_iterator orig = *this;
        return operator--(), orig;
    }

    constexpr dense_map_iterator &operator+=(const difference_type value) noexcept {
        it += value;
        return *this;
    }

    constexpr dense_map_iterator operator+(const difference_type value) const noexcept {
        dense_map_iterator copy = *this;
        return (copy += value);
    }

    constexpr dense_map_iterator &operator-=(const difference_type value) noexcept {
        return (*this += -value);
    }

    constexpr dense_map_iterator operator-(const difference_type value) const noexcept {
        return (*this + -value);
    }

    [[nodiscard]] constexpr reference operator[](const difference_type value) const noexcept {
        return {it[value].element.first, it[value].element.second};
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept {
        return operator*();
    }

    [[nodiscard]] constexpr reference operator*() const noexcept {
        return {it->element.first, it->element.second};
    }

    template<typename Lhs, typename Rhs>
    friend constexpr std::ptrdiff_t operator-(const dense_map_iterator<Lhs> &, const dense_map_iterator<Rhs> &) noexcept;

    template<typename Lhs, typename Rhs>
    friend constexpr bool operator==(const dense_map_iterator<Lhs> &, const dense_map_iterator<Rhs> &) noexcept;

    template<typename Lhs, typename Rhs>
    friend constexpr bool operator<(const dense_map_iterator<Lhs> &, const dense_map_iterator<Rhs> &) noexcept;

private:
    It it;
};

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr std::ptrdiff_t operator-(const dense_map_iterator<Lhs> &lhs, const dense_map_iterator<Rhs> &rhs) noexcept {
    return lhs.it - rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator==(const dense_map_iterator<Lhs> &lhs, const dense_map_iterator<Rhs> &rhs) noexcept {
    return lhs.it == rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator!=(const dense_map_iterator<Lhs> &lhs, const dense_map_iterator<Rhs> &rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator<(const dense_map_iterator<Lhs> &lhs, const dense_map_iterator<Rhs> &rhs) noexcept {
    return lhs.it < rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator>(const dense_map_iterator<Lhs> &lhs, const dense_map_iterator<Rhs> &rhs) noexcept {
    return rhs < lhs;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator<=(const dense_map_iterator<Lhs> &lhs, const dense_map_iterator<Rhs> &rhs) noexcept {
    return !(lhs > rhs);
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator>=(const dense_map_iterator<Lhs> &lhs, const dense_map_iterator<Rhs> &rhs) noexcept {
    return !(lhs < rhs);
}

template<typename It>
class dense_map_local_iterator final {
    template<typename>
    friend class dense_map_local_iterator;

    using first_type = decltype(std::as_const(std::declval<It>()->element.first));
    using second_type = decltype((std::declval<It>()->element.second));

public:
    using value_type = std::pair<first_type, second_type>;
    using pointer = input_iterator_pointer<value_type>;
    using reference = value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    constexpr dense_map_local_iterator() noexcept
        : it{},
          offset{} {}

    constexpr dense_map_local_iterator(It iter, const std::size_t pos) noexcept
        : it{iter},
          offset{pos} {}

    template<typename Other, typename = std::enable_if_t<!std::is_same_v<It, Other> && std::is_constructible_v<It, Other>>>
    constexpr dense_map_local_iterator(const dense_map_local_iterator<Other> &other) noexcept
        : it{other.it},
          offset{other.offset} {}

    constexpr dense_map_local_iterator &operator++() noexcept {
        return offset = it[offset].next, *this;
    }

    constexpr dense_map_local_iterator operator++(int) noexcept {
        dense_map_local_iterator orig = *this;
        return ++(*this), orig;
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept {
        return operator*();
    }

    [[nodiscard]] constexpr reference operator*() const noexcept {
        return {it[offset].element.first, it[offset].element.second};
    }

    [[nodiscard]] constexpr std::size_t index() const noexcept {
        return offset;
    }

private:
    It it;
    std::size_t offset;
};

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator==(const dense_map_local_iterator<Lhs> &lhs, const dense_map_local_iterator<Rhs> &rhs) noexcept {
    return lhs.index() == rhs.index();
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator!=(const dense_map_local_iterator<Lhs> &lhs, const dense_map_local_iterator<Rhs> &rhs) noexcept {
    return !(lhs == rhs);
}

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Associative container for key-value pairs with unique keys.
 *
 * Internally, elements are organized into buckets. Which bucket an element is
 * placed into depends entirely on the hash of its key. Keys with the same hash
 * code appear in the same bucket.
 *
 * @tparam Key Key type of the associative container.
 * @tparam Type Mapped type of the associative container.
 * @tparam Hash Type of function to use to hash the keys.
 * @tparam KeyEqual Type of function to use to compare the keys for equality.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Key, typename Type, typename Hash, typename KeyEqual, typename Allocator>
class dense_map {
    static constexpr float default_threshold = 0.875f;
    static constexpr std::size_t minimum_capacity = 8u;

    using node_type = internal::dense_map_node<Key, Type>;
    using alloc_traits = std::allocator_traits<Allocator>;
    static_assert(std::is_same_v<typename alloc_traits::value_type, std::pair<const Key, Type>>, "Invalid value type");
    using sparse_container_type = std::vector<std::size_t, typename alloc_traits::template rebind_alloc<std::size_t>>;
    using packed_container_type = std::vector<node_type, typename alloc_traits::template rebind_alloc<node_type>>;

    template<typename Other>
    [[nodiscard]] std::size_t key_to_bucket(const Other &key) const noexcept {
        return fast_mod(static_cast<size_type>(sparse.second()(key)), bucket_count());
    }

    template<typename Other>
    [[nodiscard]] auto constrained_find(const Other &key, std::size_t bucket) {
        for(auto it = begin(bucket), last = end(bucket); it != last; ++it) {
            if(packed.second()(it->first, key)) {
                return begin() + static_cast<typename iterator::difference_type>(it.index());
            }
        }

        return end();
    }

    template<typename Other>
    [[nodiscard]] auto constrained_find(const Other &key, std::size_t bucket) const {
        for(auto it = cbegin(bucket), last = cend(bucket); it != last; ++it) {
            if(packed.second()(it->first, key)) {
                return cbegin() + static_cast<typename iterator::difference_type>(it.index());
            }
        }

        return cend();
    }

    template<typename Other, typename... Args>
    [[nodiscard]] auto insert_or_do_nothing(Other &&key, Args &&...args) {
        const auto index = key_to_bucket(key);

        if(auto it = constrained_find(key, index); it != end()) {
            return std::make_pair(it, false);
        }

        packed.first().emplace_back(sparse.first()[index], std::piecewise_construct, std::forward_as_tuple(std::forward<Other>(key)), std::forward_as_tuple(std::forward<Args>(args)...));
        sparse.first()[index] = packed.first().size() - 1u;
        rehash_if_required();

        return std::make_pair(--end(), true);
    }

    template<typename Other, typename Arg>
    [[nodiscard]] auto insert_or_overwrite(Other &&key, Arg &&value) {
        const auto index = key_to_bucket(key);

        if(auto it = constrained_find(key, index); it != end()) {
            it->second = std::forward<Arg>(value);
            return std::make_pair(it, false);
        }

        packed.first().emplace_back(sparse.first()[index], std::forward<Other>(key), std::forward<Arg>(value));
        sparse.first()[index] = packed.first().size() - 1u;
        rehash_if_required();

        return std::make_pair(--end(), true);
    }

    void move_and_pop(const std::size_t pos) {
        if(const auto last = size() - 1u; pos != last) {
            size_type *curr = sparse.first().data() + key_to_bucket(packed.first().back().element.first);
            packed.first()[pos] = std::move(packed.first().back());
            for(; *curr != last; curr = &packed.first()[*curr].next) {}
            *curr = pos;
        }

        packed.first().pop_back();
    }

    void rehash_if_required() {
        if(size() > (bucket_count() * max_load_factor())) {
            rehash(bucket_count() * 2u);
        }
    }

public:
    /*! @brief Key type of the container. */
    using key_type = Key;
    /*! @brief Mapped type of the container. */
    using mapped_type = Type;
    /*! @brief Key-value type of the container. */
    using value_type = std::pair<const Key, Type>;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Type of function to use to hash the keys. */
    using hasher = Hash;
    /*! @brief Type of function to use to compare the keys for equality. */
    using key_equal = KeyEqual;
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Input iterator type. */
    using iterator = internal::dense_map_iterator<typename packed_container_type::iterator>;
    /*! @brief Constant input iterator type. */
    using const_iterator = internal::dense_map_iterator<typename packed_container_type::const_iterator>;
    /*! @brief Input iterator type. */
    using local_iterator = internal::dense_map_local_iterator<typename packed_container_type::iterator>;
    /*! @brief Constant input iterator type. */
    using const_local_iterator = internal::dense_map_local_iterator<typename packed_container_type::const_iterator>;

    /*! @brief Default constructor. */
    dense_map()
        : dense_map{minimum_capacity} {}

    /**
     * @brief Constructs an empty container with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit dense_map(const allocator_type &allocator)
        : dense_map{minimum_capacity, hasher{}, key_equal{}, allocator} {}

    /**
     * @brief Constructs an empty container with a given allocator and user
     * supplied minimal number of buckets.
     * @param cnt Minimal number of buckets.
     * @param allocator The allocator to use.
     */
    dense_map(const size_type cnt, const allocator_type &allocator)
        : dense_map{cnt, hasher{}, key_equal{}, allocator} {}

    /**
     * @brief Constructs an empty container with a given allocator, hash
     * function and user supplied minimal number of buckets.
     * @param cnt Minimal number of buckets.
     * @param hash Hash function to use.
     * @param allocator The allocator to use.
     */
    dense_map(const size_type cnt, const hasher &hash, const allocator_type &allocator)
        : dense_map{cnt, hash, key_equal{}, allocator} {}

    /**
     * @brief Constructs an empty container with a given allocator, hash
     * function, compare function and user supplied minimal number of buckets.
     * @param cnt Minimal number of buckets.
     * @param hash Hash function to use.
     * @param equal Compare function to use.
     * @param allocator The allocator to use.
     */
    explicit dense_map(const size_type cnt, const hasher &hash = hasher{}, const key_equal &equal = key_equal{}, const allocator_type &allocator = allocator_type{})
        : sparse{allocator, hash},
          packed{allocator, equal},
          threshold{default_threshold} {
        rehash(cnt);
    }

    /*! @brief Default copy constructor. */
    dense_map(const dense_map &) = default;

    /**
     * @brief Allocator-extended copy constructor.
     * @param other The instance to copy from.
     * @param allocator The allocator to use.
     */
    dense_map(const dense_map &other, const allocator_type &allocator)
        : sparse{std::piecewise_construct, std::forward_as_tuple(other.sparse.first(), allocator), std::forward_as_tuple(other.sparse.second())},
          packed{std::piecewise_construct, std::forward_as_tuple(other.packed.first(), allocator), std::forward_as_tuple(other.packed.second())},
          threshold{other.threshold} {}

    /*! @brief Default move constructor. */
    dense_map(dense_map &&) noexcept(std::is_nothrow_move_constructible_v<compressed_pair<sparse_container_type, hasher>> &&std::is_nothrow_move_constructible_v<compressed_pair<packed_container_type, key_equal>>) = default;

    /**
     * @brief Allocator-extended move constructor.
     * @param other The instance to move from.
     * @param allocator The allocator to use.
     */
    dense_map(dense_map &&other, const allocator_type &allocator)
        : sparse{std::piecewise_construct, std::forward_as_tuple(std::move(other.sparse.first()), allocator), std::forward_as_tuple(std::move(other.sparse.second()))},
          packed{std::piecewise_construct, std::forward_as_tuple(std::move(other.packed.first()), allocator), std::forward_as_tuple(std::move(other.packed.second()))},
          threshold{other.threshold} {}

    /**
     * @brief Default copy assignment operator.
     * @return This container.
     */
    dense_map &operator=(const dense_map &) = default;

    /**
     * @brief Default move assignment operator.
     * @return This container.
     */
    dense_map &operator=(dense_map &&) noexcept(std::is_nothrow_move_assignable_v<compressed_pair<sparse_container_type, hasher>> &&std::is_nothrow_move_assignable_v<compressed_pair<packed_container_type, key_equal>>) = default;

    /**
     * @brief Returns the associated allocator.
     * @return The associated allocator.
     */
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return sparse.first().get_allocator();
    }

    /**
     * @brief Returns an iterator to the beginning.
     *
     * If the array is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first instance of the internal array.
     */
    [[nodiscard]] const_iterator cbegin() const noexcept {
        return packed.first().begin();
    }

    /*! @copydoc cbegin */
    [[nodiscard]] const_iterator begin() const noexcept {
        return cbegin();
    }

    /*! @copydoc begin */
    [[nodiscard]] iterator begin() noexcept {
        return packed.first().begin();
    }

    /**
     * @brief Returns an iterator to the end.
     * @return An iterator to the element following the last instance of the
     * internal array.
     */
    [[nodiscard]] const_iterator cend() const noexcept {
        return packed.first().end();
    }

    /*! @copydoc cend */
    [[nodiscard]] const_iterator end() const noexcept {
        return cend();
    }

    /*! @copydoc end */
    [[nodiscard]] iterator end() noexcept {
        return packed.first().end();
    }

    /**
     * @brief Checks whether a container is empty.
     * @return True if the container is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return packed.first().empty();
    }

    /**
     * @brief Returns the number of elements in a container.
     * @return Number of elements in a container.
     */
    [[nodiscard]] size_type size() const noexcept {
        return packed.first().size();
    }

    /**
     * @brief Returns the maximum possible number of elements.
     * @return Maximum possible number of elements.
     */
    [[nodiscard]] size_type max_size() const noexcept {
        return packed.first().max_size();
    }

    /*! @brief Clears the container. */
    void clear() noexcept {
        sparse.first().clear();
        packed.first().clear();
        rehash(0u);
    }

    /**
     * @brief Inserts an element into the container, if the key does not exist.
     * @param value A key-value pair eventually convertible to the value type.
     * @return A pair consisting of an iterator to the inserted element (or to
     * the element that prevented the insertion) and a bool denoting whether the
     * insertion took place.
     */
    std::pair<iterator, bool> insert(const value_type &value) {
        return insert_or_do_nothing(value.first, value.second);
    }

    /*! @copydoc insert */
    std::pair<iterator, bool> insert(value_type &&value) {
        return insert_or_do_nothing(std::move(value.first), std::move(value.second));
    }

    /**
     * @copydoc insert
     * @tparam Arg Type of the key-value pair to insert into the container.
     */
    template<typename Arg>
    std::enable_if_t<std::is_constructible_v<value_type, Arg &&>, std::pair<iterator, bool>>
    insert(Arg &&value) {
        return insert_or_do_nothing(std::forward<Arg>(value).first, std::forward<Arg>(value).second);
    }

    /**
     * @brief Inserts elements into the container, if their keys do not exist.
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of elements.
     * @param last An iterator past the last element of the range of elements.
     */
    template<typename It>
    void insert(It first, It last) {
        for(; first != last; ++first) {
            insert(*first);
        }
    }

    /**
     * @brief Inserts an element into the container or assigns to the current
     * element if the key already exists.
     * @tparam Arg Type of the value to insert or assign.
     * @param key A key used both to look up and to insert if not found.
     * @param value A value to insert or assign.
     * @return A pair consisting of an iterator to the element and a bool
     * denoting whether the insertion took place.
     */
    template<typename Arg>
    std::pair<iterator, bool> insert_or_assign(const key_type &key, Arg &&value) {
        return insert_or_overwrite(key, std::forward<Arg>(value));
    }

    /*! @copydoc insert_or_assign */
    template<typename Arg>
    std::pair<iterator, bool> insert_or_assign(key_type &&key, Arg &&value) {
        return insert_or_overwrite(std::move(key), std::forward<Arg>(value));
    }

    /**
     * @brief Constructs an element in-place, if the key does not exist.
     *
     * The element is also constructed when the container already has the key,
     * in which case the newly constructed object is destroyed immediately.
     *
     * @tparam Args Types of arguments to forward to the constructor of the
     * element.
     * @param args Arguments to forward to the constructor of the element.
     * @return A pair consisting of an iterator to the inserted element (or to
     * the element that prevented the insertion) and a bool denoting whether the
     * insertion took place.
     */
    template<typename... Args>
    std::pair<iterator, bool> emplace([[maybe_unused]] Args &&...args) {
        if constexpr(sizeof...(Args) == 0u) {
            return insert_or_do_nothing(key_type{});
        } else if constexpr(sizeof...(Args) == 1u) {
            return insert_or_do_nothing(std::forward<Args>(args).first..., std::forward<Args>(args).second...);
        } else if constexpr(sizeof...(Args) == 2u) {
            return insert_or_do_nothing(std::forward<Args>(args)...);
        } else {
            auto &node = packed.first().emplace_back(packed.first().size(), std::forward<Args>(args)...);
            const auto index = key_to_bucket(node.element.first);

            if(auto it = constrained_find(node.element.first, index); it != end()) {
                packed.first().pop_back();
                return std::make_pair(it, false);
            }

            std::swap(node.next, sparse.first()[index]);
            rehash_if_required();

            return std::make_pair(--end(), true);
        }
    }

    /**
     * @brief Inserts in-place if the key does not exist, does nothing if the
     * key exists.
     * @tparam Args Types of arguments to forward to the constructor of the
     * element.
     * @param key A key used both to look up and to insert if not found.
     * @param args Arguments to forward to the constructor of the element.
     * @return A pair consisting of an iterator to the inserted element (or to
     * the element that prevented the insertion) and a bool denoting whether the
     * insertion took place.
     */
    template<typename... Args>
    std::pair<iterator, bool> try_emplace(const key_type &key, Args &&...args) {
        return insert_or_do_nothing(key, std::forward<Args>(args)...);
    }

    /*! @copydoc try_emplace */
    template<typename... Args>
    std::pair<iterator, bool> try_emplace(key_type &&key, Args &&...args) {
        return insert_or_do_nothing(std::move(key), std::forward<Args>(args)...);
    }

    /**
     * @brief Removes an element from a given position.
     * @param pos An iterator to the element to remove.
     * @return An iterator following the removed element.
     */
    iterator erase(const_iterator pos) {
        const auto diff = pos - cbegin();
        erase(pos->first);
        return begin() + diff;
    }

    /**
     * @brief Removes the given elements from a container.
     * @param first An iterator to the first element of the range of elements.
     * @param last An iterator past the last element of the range of elements.
     * @return An iterator following the last removed element.
     */
    iterator erase(const_iterator first, const_iterator last) {
        const auto dist = first - cbegin();

        for(auto from = last - cbegin(); from != dist; --from) {
            erase(packed.first()[from - 1u].element.first);
        }

        return (begin() + dist);
    }

    /**
     * @brief Removes the element associated with a given key.
     * @param key A key value of an element to remove.
     * @return Number of elements removed (either 0 or 1).
     */
    size_type erase(const key_type &key) {
        for(size_type *curr = sparse.first().data() + key_to_bucket(key); *curr != (std::numeric_limits<size_type>::max)(); curr = &packed.first()[*curr].next) {
            if(packed.second()(packed.first()[*curr].element.first, key)) {
                const auto index = *curr;
                *curr = packed.first()[*curr].next;
                move_and_pop(index);
                return 1u;
            }
        }

        return 0u;
    }

    /**
     * @brief Exchanges the contents with those of a given container.
     * @param other Container to exchange the content with.
     */
    void swap(dense_map &other) {
        using std::swap;
        swap(sparse, other.sparse);
        swap(packed, other.packed);
        swap(threshold, other.threshold);
    }

    /**
     * @brief Accesses a given element with bounds checking.
     * @param key A key of an element to find.
     * @return A reference to the mapped value of the requested element.
     */
    [[nodiscard]] mapped_type &at(const key_type &key) {
        auto it = find(key);
        ENTT_ASSERT(it != end(), "Invalid key");
        return it->second;
    }

    /*! @copydoc at */
    [[nodiscard]] const mapped_type &at(const key_type &key) const {
        auto it = find(key);
        ENTT_ASSERT(it != cend(), "Invalid key");
        return it->second;
    }

    /**
     * @brief Accesses or inserts a given element.
     * @param key A key of an element to find or insert.
     * @return A reference to the mapped value of the requested element.
     */
    [[nodiscard]] mapped_type &operator[](const key_type &key) {
        return insert_or_do_nothing(key).first->second;
    }

    /**
     * @brief Accesses or inserts a given element.
     * @param key A key of an element to find or insert.
     * @return A reference to the mapped value of the requested element.
     */
    [[nodiscard]] mapped_type &operator[](key_type &&key) {
        return insert_or_do_nothing(std::move(key)).first->second;
    }

    /**
     * @brief Returns the number of elements matching a key (either 1 or 0).
     * @param key Key value of an element to search for.
     * @return Number of elements matching the key (either 1 or 0).
     */
    [[nodiscard]] size_type count(const key_type &key) const {
        return find(key) != end();
    }

    /**
     * @brief Returns the number of elements matching a key (either 1 or 0).
     * @tparam Other Type of the key value of an element to search for.
     * @param key Key value of an element to search for.
     * @return Number of elements matching the key (either 1 or 0).
     */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, size_type>>
    count(const Other &key) const {
        return find(key) != end();
    }

    /**
     * @brief Finds an element with a given key.
     * @param key Key value of an element to search for.
     * @return An iterator to an element with the given key. If no such element
     * is found, a past-the-end iterator is returned.
     */
    [[nodiscard]] iterator find(const key_type &key) {
        return constrained_find(key, key_to_bucket(key));
    }

    /*! @copydoc find */
    [[nodiscard]] const_iterator find(const key_type &key) const {
        return constrained_find(key, key_to_bucket(key));
    }

    /**
     * @brief Finds an element with a key that compares _equivalent_ to a given
     * key.
     * @tparam Other Type of the key value of an element to search for.
     * @param key Key value of an element to search for.
     * @return An iterator to an element with the given key. If no such element
     * is found, a past-the-end iterator is returned.
     */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, iterator>>
    find(const Other &key) {
        return constrained_find(key, key_to_bucket(key));
    }

    /*! @copydoc find */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, const_iterator>>
    find(const Other &key) const {
        return constrained_find(key, key_to_bucket(key));
    }

    /**
     * @brief Returns a range containing all elements with a given key.
     * @param key Key value of an element to search for.
     * @return A pair of iterators pointing to the first element and past the
     * last element of the range.
     */
    [[nodiscard]] std::pair<iterator, iterator> equal_range(const key_type &key) {
        const auto it = find(key);
        return {it, it + !(it == end())};
    }

    /*! @copydoc equal_range */
    [[nodiscard]] std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const {
        const auto it = find(key);
        return {it, it + !(it == cend())};
    }

    /**
     * @brief Returns a range containing all elements that compare _equivalent_
     * to a given key.
     * @tparam Other Type of an element to search for.
     * @param key Key value of an element to search for.
     * @return A pair of iterators pointing to the first element and past the
     * last element of the range.
     */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, std::pair<iterator, iterator>>>
    equal_range(const Other &key) {
        const auto it = find(key);
        return {it, it + !(it == end())};
    }

    /*! @copydoc equal_range */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, std::pair<const_iterator, const_iterator>>>
    equal_range(const Other &key) const {
        const auto it = find(key);
        return {it, it + !(it == cend())};
    }

    /**
     * @brief Checks if the container contains an element with a given key.
     * @param key Key value of an element to search for.
     * @return True if there is such an element, false otherwise.
     */
    [[nodiscard]] bool contains(const key_type &key) const {
        return (find(key) != cend());
    }

    /**
     * @brief Checks if the container contains an element with a key that
     * compares _equivalent_ to a given value.
     * @tparam Other Type of the key value of an element to search for.
     * @param key Key value of an element to search for.
     * @return True if there is such an element, false otherwise.
     */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, bool>>
    contains(const Other &key) const {
        return (find(key) != cend());
    }

    /**
     * @brief Returns an iterator to the beginning of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the beginning of the given bucket.
     */
    [[nodiscard]] const_local_iterator cbegin(const size_type index) const {
        return {packed.first().begin(), sparse.first()[index]};
    }

    /**
     * @brief Returns an iterator to the beginning of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the beginning of the given bucket.
     */
    [[nodiscard]] const_local_iterator begin(const size_type index) const {
        return cbegin(index);
    }

    /**
     * @brief Returns an iterator to the beginning of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the beginning of the given bucket.
     */
    [[nodiscard]] local_iterator begin(const size_type index) {
        return {packed.first().begin(), sparse.first()[index]};
    }

    /**
     * @brief Returns an iterator to the end of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the end of the given bucket.
     */
    [[nodiscard]] const_local_iterator cend([[maybe_unused]] const size_type index) const {
        return {packed.first().begin(), (std::numeric_limits<size_type>::max)()};
    }

    /**
     * @brief Returns an iterator to the end of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the end of the given bucket.
     */
    [[nodiscard]] const_local_iterator end(const size_type index) const {
        return cend(index);
    }

    /**
     * @brief Returns an iterator to the end of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the end of the given bucket.
     */
    [[nodiscard]] local_iterator end([[maybe_unused]] const size_type index) {
        return {packed.first().begin(), (std::numeric_limits<size_type>::max)()};
    }

    /**
     * @brief Returns the number of buckets.
     * @return The number of buckets.
     */
    [[nodiscard]] size_type bucket_count() const {
        return sparse.first().size();
    }

    /**
     * @brief Returns the maximum number of buckets.
     * @return The maximum number of buckets.
     */
    [[nodiscard]] size_type max_bucket_count() const {
        return sparse.first().max_size();
    }

    /**
     * @brief Returns the number of elements in a given bucket.
     * @param index The index of the bucket to examine.
     * @return The number of elements in the given bucket.
     */
    [[nodiscard]] size_type bucket_size(const size_type index) const {
        return static_cast<size_type>(std::distance(begin(index), end(index)));
    }

    /**
     * @brief Returns the bucket for a given key.
     * @param key The value of the key to examine.
     * @return The bucket for the given key.
     */
    [[nodiscard]] size_type bucket(const key_type &key) const {
        return key_to_bucket(key);
    }

    /**
     * @brief Returns the average number of elements per bucket.
     * @return The average number of elements per bucket.
     */
    [[nodiscard]] float load_factor() const {
        return size() / static_cast<float>(bucket_count());
    }

    /**
     * @brief Returns the maximum average number of elements per bucket.
     * @return The maximum average number of elements per bucket.
     */
    [[nodiscard]] float max_load_factor() const {
        return threshold;
    }

    /**
     * @brief Sets the desired maximum average number of elements per bucket.
     * @param value A desired maximum average number of elements per bucket.
     */
    void max_load_factor(const float value) {
        ENTT_ASSERT(value > 0.f, "Invalid load factor");
        threshold = value;
        rehash(0u);
    }

    /**
     * @brief Reserves at least the specified number of buckets and regenerates
     * the hash table.
     * @param cnt New number of buckets.
     */
    void rehash(const size_type cnt) {
        auto value = cnt > minimum_capacity ? cnt : minimum_capacity;
        const auto cap = static_cast<size_type>(size() / max_load_factor());
        value = value > cap ? value : cap;

        if(const auto sz = next_power_of_two(value); sz != bucket_count()) {
            sparse.first().resize(sz);

            for(auto &&elem: sparse.first()) {
                elem = std::numeric_limits<size_type>::max();
            }

            for(size_type pos{}, last = size(); pos < last; ++pos) {
                const auto index = key_to_bucket(packed.first()[pos].element.first);
                packed.first()[pos].next = std::exchange(sparse.first()[index], pos);
            }
        }
    }

    /**
     * @brief Reserves space for at least the specified number of elements and
     * regenerates the hash table.
     * @param cnt New number of elements.
     */
    void reserve(const size_type cnt) {
        packed.first().reserve(cnt);
        rehash(static_cast<size_type>(std::ceil(cnt / max_load_factor())));
    }

    /**
     * @brief Returns the function used to hash the keys.
     * @return The function used to hash the keys.
     */
    [[nodiscard]] hasher hash_function() const {
        return sparse.second();
    }

    /**
     * @brief Returns the function used to compare keys for equality.
     * @return The function used to compare keys for equality.
     */
    [[nodiscard]] key_equal key_eq() const {
        return packed.second();
    }

private:
    compressed_pair<sparse_container_type, hasher> sparse;
    compressed_pair<packed_container_type, key_equal> packed;
    float threshold;
};

} // namespace entt

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace std {

template<typename Key, typename Value, typename Allocator>
struct uses_allocator<entt::internal::dense_map_node<Key, Value>, Allocator>
    : std::true_type {};

} // namespace std

/**
 * Internal details not to be documented.
 * @endcond
 */

#endif

///////////////// END OF external/entt/container/dense_map.hpp /////////////////

/////////////////// START OF external/entt/core/utility.hpp ////////////////////

#ifndef ENTT_CORE_UTILITY_HPP
#define ENTT_CORE_UTILITY_HPP

#include <type_traits>
#include <utility>

namespace entt {

/*! @brief Identity function object (waiting for C++20). */
struct identity {
    /*! @brief Indicates that this is a transparent function object. */
    using is_transparent = void;

    /**
     * @brief Returns its argument unchanged.
     * @tparam Type Type of the argument.
     * @param value The actual argument.
     * @return The submitted value as-is.
     */
    template<typename Type>
    [[nodiscard]] constexpr Type &&operator()(Type &&value) const noexcept {
        return std::forward<Type>(value);
    }
};

/**
 * @brief Constant utility to disambiguate overloaded members of a class.
 * @tparam Type Type of the desired overload.
 * @tparam Class Type of class to which the member belongs.
 * @param member A valid pointer to a member.
 * @return Pointer to the member.
 */
template<typename Type, typename Class>
[[nodiscard]] constexpr auto overload(Type Class::*member) noexcept {
    return member;
}

/**
 * @brief Constant utility to disambiguate overloaded functions.
 * @tparam Func Function type of the desired overload.
 * @param func A valid pointer to a function.
 * @return Pointer to the function.
 */
template<typename Func>
[[nodiscard]] constexpr auto overload(Func *func) noexcept {
    return func;
}

/**
 * @brief Helper type for visitors.
 * @tparam Func Types of function objects.
 */
template<typename... Func>
struct overloaded: Func... {
    using Func::operator()...;
};

/**
 * @brief Deduction guide.
 * @tparam Func Types of function objects.
 */
template<typename... Func>
overloaded(Func...) -> overloaded<Func...>;

/**
 * @brief Basic implementation of a y-combinator.
 * @tparam Func Type of a potentially recursive function.
 */
template<typename Func>
struct y_combinator {
    /**
     * @brief Constructs a y-combinator from a given function.
     * @param recursive A potentially recursive function.
     */
    constexpr y_combinator(Func recursive) noexcept(std::is_nothrow_move_constructible_v<Func>)
        : func{std::move(recursive)} {}

    /**
     * @brief Invokes a y-combinator and therefore its underlying function.
     * @tparam Args Types of arguments to use to invoke the underlying function.
     * @param args Parameters to use to invoke the underlying function.
     * @return Return value of the underlying function, if any.
     */
    template<typename... Args>
    constexpr decltype(auto) operator()(Args &&...args) const noexcept(std::is_nothrow_invocable_v<Func, const y_combinator &, Args...>) {
        return func(*this, std::forward<Args>(args)...);
    }

    /*! @copydoc operator()() */
    template<typename... Args>
    constexpr decltype(auto) operator()(Args &&...args) noexcept(std::is_nothrow_invocable_v<Func, y_combinator &, Args...>) {
        return func(*this, std::forward<Args>(args)...);
    }

private:
    Func func;
};

} // namespace entt

#endif

//////////////////// END OF external/entt/core/utility.hpp /////////////////////

////////////////// START OF external/entt/core/algorithm.hpp ///////////////////

#ifndef ENTT_CORE_ALGORITHM_HPP
#define ENTT_CORE_ALGORITHM_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>

namespace entt {

/**
 * @brief Function object to wrap `std::sort` in a class type.
 *
 * Unfortunately, `std::sort` cannot be passed as template argument to a class
 * template or a function template.<br/>
 * This class fills the gap by wrapping some flavors of `std::sort` in a
 * function object.
 */
struct std_sort {
    /**
     * @brief Sorts the elements in a range.
     *
     * Sorts the elements in a range using the given binary comparison function.
     *
     * @tparam It Type of random access iterator.
     * @tparam Compare Type of comparison function object.
     * @tparam Args Types of arguments to forward to the sort function.
     * @param first An iterator to the first element of the range to sort.
     * @param last An iterator past the last element of the range to sort.
     * @param compare A valid comparison function object.
     * @param args Arguments to forward to the sort function, if any.
     */
    template<typename It, typename Compare = std::less<>, typename... Args>
    void operator()(It first, It last, Compare compare = Compare{}, Args &&...args) const {
        std::sort(std::forward<Args>(args)..., std::move(first), std::move(last), std::move(compare));
    }
};

/*! @brief Function object for performing insertion sort. */
struct insertion_sort {
    /**
     * @brief Sorts the elements in a range.
     *
     * Sorts the elements in a range using the given binary comparison function.
     *
     * @tparam It Type of random access iterator.
     * @tparam Compare Type of comparison function object.
     * @param first An iterator to the first element of the range to sort.
     * @param last An iterator past the last element of the range to sort.
     * @param compare A valid comparison function object.
     */
    template<typename It, typename Compare = std::less<>>
    void operator()(It first, It last, Compare compare = Compare{}) const {
        if(first < last) {
            for(auto it = first + 1; it < last; ++it) {
                auto value = std::move(*it);
                auto pre = it;

                for(; pre > first && compare(value, *(pre - 1)); --pre) {
                    *pre = std::move(*(pre - 1));
                }

                *pre = std::move(value);
            }
        }
    }
};

/**
 * @brief Function object for performing LSD radix sort.
 * @tparam Bit Number of bits processed per pass.
 * @tparam N Maximum number of bits to sort.
 */
template<std::size_t Bit, std::size_t N>
struct radix_sort {
    static_assert((N % Bit) == 0, "The maximum number of bits to sort must be a multiple of the number of bits processed per pass");

    /**
     * @brief Sorts the elements in a range.
     *
     * Sorts the elements in a range using the given _getter_ to access the
     * actual data to be sorted.
     *
     * This implementation is inspired by the online book
     * [Physically Based Rendering](http://www.pbr-book.org/3ed-2018/Primitives_and_Intersection_Acceleration/Bounding_Volume_Hierarchies.html#RadixSort).
     *
     * @tparam It Type of random access iterator.
     * @tparam Getter Type of _getter_ function object.
     * @param first An iterator to the first element of the range to sort.
     * @param last An iterator past the last element of the range to sort.
     * @param getter A valid _getter_ function object.
     */
    template<typename It, typename Getter = identity>
    void operator()(It first, It last, Getter getter = Getter{}) const {
        if(first < last) {
            constexpr auto passes = N / Bit;

            using value_type = typename std::iterator_traits<It>::value_type;
            std::vector<value_type> aux(std::distance(first, last));

            auto part = [getter = std::move(getter)](auto from, auto to, auto out, auto start) {
                constexpr auto mask = (1 << Bit) - 1;
                constexpr auto buckets = 1 << Bit;

                std::size_t index[buckets]{};
                std::size_t count[buckets]{};

                for(auto it = from; it != to; ++it) {
                    ++count[(getter(*it) >> start) & mask];
                }

                for(std::size_t pos{}, end = buckets - 1u; pos < end; ++pos) {
                    index[pos + 1u] = index[pos] + count[pos];
                }

                for(auto it = from; it != to; ++it) {
                    out[index[(getter(*it) >> start) & mask]++] = std::move(*it);
                }
            };

            for(std::size_t pass = 0; pass < (passes & ~1); pass += 2) {
                part(first, last, aux.begin(), pass * Bit);
                part(aux.begin(), aux.end(), first, (pass + 1) * Bit);
            }

            if constexpr(passes & 1) {
                part(first, last, aux.begin(), (passes - 1) * Bit);
                std::move(aux.begin(), aux.end(), first);
            }
        }
    }
};

} // namespace entt

#endif

/////////////////// END OF external/entt/core/algorithm.hpp ////////////////////

/////////////////// START OF external/entt/core/attribute.h ////////////////////

#ifndef ENTT_CORE_ATTRIBUTE_H
#define ENTT_CORE_ATTRIBUTE_H

#ifndef ENTT_EXPORT
#    if defined _WIN32 || defined __CYGWIN__ || defined _MSC_VER
#        define ENTT_EXPORT __declspec(dllexport)
#        define ENTT_IMPORT __declspec(dllimport)
#        define ENTT_HIDDEN
#    elif defined __GNUC__ && __GNUC__ >= 4
#        define ENTT_EXPORT __attribute__((visibility("default")))
#        define ENTT_IMPORT __attribute__((visibility("default")))
#        define ENTT_HIDDEN __attribute__((visibility("hidden")))
#    else /* Unsupported compiler */
#        define ENTT_EXPORT
#        define ENTT_IMPORT
#        define ENTT_HIDDEN
#    endif
#endif

#ifndef ENTT_API
#    if defined ENTT_API_EXPORT
#        define ENTT_API ENTT_EXPORT
#    elif defined ENTT_API_IMPORT
#        define ENTT_API ENTT_IMPORT
#    else /* No API */
#        define ENTT_API
#    endif
#endif

#endif

//////////////////// END OF external/entt/core/attribute.h /////////////////////

//////////////// START OF external/entt/core/hashed_string.hpp /////////////////

#ifndef ENTT_CORE_HASHED_STRING_HPP
#define ENTT_CORE_HASHED_STRING_HPP

#include <cstddef>
#include <cstdint>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename>
struct fnv1a_traits;

template<>
struct fnv1a_traits<std::uint32_t> {
    using type = std::uint32_t;
    static constexpr std::uint32_t offset = 2166136261;
    static constexpr std::uint32_t prime = 16777619;
};

template<>
struct fnv1a_traits<std::uint64_t> {
    using type = std::uint64_t;
    static constexpr std::uint64_t offset = 14695981039346656037ull;
    static constexpr std::uint64_t prime = 1099511628211ull;
};

template<typename Char>
struct basic_hashed_string {
    using value_type = Char;
    using size_type = std::size_t;
    using hash_type = id_type;

    const value_type *repr;
    size_type length;
    hash_type hash;
};

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Zero overhead unique identifier.
 *
 * A hashed string is a compile-time tool that allows users to use
 * human-readable identifiers in the codebase while using their numeric
 * counterparts at runtime.<br/>
 * Because of that, a hashed string can also be used in constant expressions if
 * required.
 *
 * @warning
 * This class doesn't take ownership of user-supplied strings nor does it make a
 * copy of them.
 *
 * @tparam Char Character type.
 */
template<typename Char>
class basic_hashed_string: internal::basic_hashed_string<Char> {
    using base_type = internal::basic_hashed_string<Char>;
    using traits_type = internal::fnv1a_traits<id_type>;

    struct const_wrapper {
        // non-explicit constructor on purpose
        constexpr const_wrapper(const Char *str) noexcept
            : repr{str} {}

        const Char *repr;
    };

    // Fowler–Noll–Vo hash function v. 1a - the good
    [[nodiscard]] static constexpr auto helper(const Char *str) noexcept {
        base_type base{str, 0u, traits_type::offset};

        for(; str[base.length]; ++base.length) {
            base.hash = (base.hash ^ static_cast<traits_type::type>(str[base.length])) * traits_type::prime;
        }

        return base;
    }

    // Fowler–Noll–Vo hash function v. 1a - the good
    [[nodiscard]] static constexpr auto helper(const Char *str, const std::size_t len) noexcept {
        base_type base{str, len, traits_type::offset};

        for(size_type pos{}; pos < len; ++pos) {
            base.hash = (base.hash ^ static_cast<traits_type::type>(str[pos])) * traits_type::prime;
        }

        return base;
    }

public:
    /*! @brief Character type. */
    using value_type = typename base_type::value_type;
    /*! @brief Unsigned integer type. */
    using size_type = typename base_type::size_type;
    /*! @brief Unsigned integer type. */
    using hash_type = typename base_type::hash_type;

    /**
     * @brief Returns directly the numeric representation of a string view.
     * @param str Human-readable identifier.
     * @param len Length of the string to hash.
     * @return The numeric representation of the string.
     */
    [[nodiscard]] static constexpr hash_type value(const value_type *str, const size_type len) noexcept {
        return basic_hashed_string{str, len};
    }

    /**
     * @brief Returns directly the numeric representation of a string.
     * @tparam N Number of characters of the identifier.
     * @param str Human-readable identifier.
     * @return The numeric representation of the string.
     */
    template<std::size_t N>
    [[nodiscard]] static constexpr hash_type value(const value_type (&str)[N]) noexcept {
        return basic_hashed_string{str};
    }

    /**
     * @brief Returns directly the numeric representation of a string.
     * @param wrapper Helps achieving the purpose by relying on overloading.
     * @return The numeric representation of the string.
     */
    [[nodiscard]] static constexpr hash_type value(const_wrapper wrapper) noexcept {
        return basic_hashed_string{wrapper};
    }

    /*! @brief Constructs an empty hashed string. */
    constexpr basic_hashed_string() noexcept
        : base_type{} {}

    /**
     * @brief Constructs a hashed string from a string view.
     * @param str Human-readable identifier.
     * @param len Length of the string to hash.
     */
    constexpr basic_hashed_string(const value_type *str, const size_type len) noexcept
        : base_type{helper(str, len)} {}

    /**
     * @brief Constructs a hashed string from an array of const characters.
     * @tparam N Number of characters of the identifier.
     * @param str Human-readable identifier.
     */
    template<std::size_t N>
    constexpr basic_hashed_string(const value_type (&str)[N]) noexcept
        : base_type{helper(str)} {}

    /**
     * @brief Explicit constructor on purpose to avoid constructing a hashed
     * string directly from a `const value_type *`.
     *
     * @warning
     * The lifetime of the string is not extended nor is it copied.
     *
     * @param wrapper Helps achieving the purpose by relying on overloading.
     */
    explicit constexpr basic_hashed_string(const_wrapper wrapper) noexcept
        : base_type{helper(wrapper.repr)} {}

    /**
     * @brief Returns the size a hashed string.
     * @return The size of the hashed string.
     */
    [[nodiscard]] constexpr size_type size() const noexcept {
        return base_type::length;
    }

    /**
     * @brief Returns the human-readable representation of a hashed string.
     * @return The string used to initialize the hashed string.
     */
    [[nodiscard]] constexpr const value_type *data() const noexcept {
        return base_type::repr;
    }

    /**
     * @brief Returns the numeric representation of a hashed string.
     * @return The numeric representation of the hashed string.
     */
    [[nodiscard]] constexpr hash_type value() const noexcept {
        return base_type::hash;
    }

    /*! @copydoc data */
    [[nodiscard]] constexpr operator const value_type *() const noexcept {
        return data();
    }

    /**
     * @brief Returns the numeric representation of a hashed string.
     * @return The numeric representation of the hashed string.
     */
    [[nodiscard]] constexpr operator hash_type() const noexcept {
        return value();
    }
};

/**
 * @brief Deduction guide.
 * @tparam Char Character type.
 * @param str Human-readable identifier.
 * @param len Length of the string to hash.
 */
template<typename Char>
basic_hashed_string(const Char *str, const std::size_t len) -> basic_hashed_string<Char>;

/**
 * @brief Deduction guide.
 * @tparam Char Character type.
 * @tparam N Number of characters of the identifier.
 * @param str Human-readable identifier.
 */
template<typename Char, std::size_t N>
basic_hashed_string(const Char (&str)[N]) -> basic_hashed_string<Char>;

/**
 * @brief Compares two hashed strings.
 * @tparam Char Character type.
 * @param lhs A valid hashed string.
 * @param rhs A valid hashed string.
 * @return True if the two hashed strings are identical, false otherwise.
 */
template<typename Char>
[[nodiscard]] constexpr bool operator==(const basic_hashed_string<Char> &lhs, const basic_hashed_string<Char> &rhs) noexcept {
    return lhs.value() == rhs.value();
}

/**
 * @brief Compares two hashed strings.
 * @tparam Char Character type.
 * @param lhs A valid hashed string.
 * @param rhs A valid hashed string.
 * @return True if the two hashed strings differ, false otherwise.
 */
template<typename Char>
[[nodiscard]] constexpr bool operator!=(const basic_hashed_string<Char> &lhs, const basic_hashed_string<Char> &rhs) noexcept {
    return !(lhs == rhs);
}

/**
 * @brief Compares two hashed strings.
 * @tparam Char Character type.
 * @param lhs A valid hashed string.
 * @param rhs A valid hashed string.
 * @return True if the first element is less than the second, false otherwise.
 */
template<typename Char>
[[nodiscard]] constexpr bool operator<(const basic_hashed_string<Char> &lhs, const basic_hashed_string<Char> &rhs) noexcept {
    return lhs.value() < rhs.value();
}

/**
 * @brief Compares two hashed strings.
 * @tparam Char Character type.
 * @param lhs A valid hashed string.
 * @param rhs A valid hashed string.
 * @return True if the first element is less than or equal to the second, false
 * otherwise.
 */
template<typename Char>
[[nodiscard]] constexpr bool operator<=(const basic_hashed_string<Char> &lhs, const basic_hashed_string<Char> &rhs) noexcept {
    return !(rhs < lhs);
}

/**
 * @brief Compares two hashed strings.
 * @tparam Char Character type.
 * @param lhs A valid hashed string.
 * @param rhs A valid hashed string.
 * @return True if the first element is greater than the second, false
 * otherwise.
 */
template<typename Char>
[[nodiscard]] constexpr bool operator>(const basic_hashed_string<Char> &lhs, const basic_hashed_string<Char> &rhs) noexcept {
    return rhs < lhs;
}

/**
 * @brief Compares two hashed strings.
 * @tparam Char Character type.
 * @param lhs A valid hashed string.
 * @param rhs A valid hashed string.
 * @return True if the first element is greater than or equal to the second,
 * false otherwise.
 */
template<typename Char>
[[nodiscard]] constexpr bool operator>=(const basic_hashed_string<Char> &lhs, const basic_hashed_string<Char> &rhs) noexcept {
    return !(lhs < rhs);
}

/*! @brief Aliases for common character types. */
using hashed_string = basic_hashed_string<char>;

/*! @brief Aliases for common character types. */
using hashed_wstring = basic_hashed_string<wchar_t>;

inline namespace literals {

/**
 * @brief User defined literal for hashed strings.
 * @param str The literal without its suffix.
 * @return A properly initialized hashed string.
 */
[[nodiscard]] constexpr hashed_string operator"" _hs(const char *str, std::size_t) noexcept {
    return hashed_string{str};
}

/**
 * @brief User defined literal for hashed wstrings.
 * @param str The literal without its suffix.
 * @return A properly initialized hashed wstring.
 */
[[nodiscard]] constexpr hashed_wstring operator"" _hws(const wchar_t *str, std::size_t) noexcept {
    return hashed_wstring{str};
}

} // namespace literals

} // namespace entt

#endif

///////////////// END OF external/entt/core/hashed_string.hpp //////////////////

////////////////// START OF external/entt/core/type_info.hpp ///////////////////

#ifndef ENTT_CORE_TYPE_INFO_HPP
#define ENTT_CORE_TYPE_INFO_HPP

#include <string_view>
#include <type_traits>
#include <utility>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

struct ENTT_API type_index final {
    [[nodiscard]] static id_type next() noexcept {
        static ENTT_MAYBE_ATOMIC(id_type) value{};
        return value++;
    }
};

template<typename Type>
[[nodiscard]] constexpr auto stripped_type_name() noexcept {
#if defined ENTT_PRETTY_FUNCTION
    std::string_view pretty_function{ENTT_PRETTY_FUNCTION};
    auto first = pretty_function.find_first_not_of(' ', pretty_function.find_first_of(ENTT_PRETTY_FUNCTION_PREFIX) + 1);
    auto value = pretty_function.substr(first, pretty_function.find_last_of(ENTT_PRETTY_FUNCTION_SUFFIX) - first);
    return value;
#else
    return std::string_view{""};
#endif
}

template<typename Type, auto = stripped_type_name<Type>().find_first_of('.')>
[[nodiscard]] static constexpr std::string_view type_name(int) noexcept {
    constexpr auto value = stripped_type_name<Type>();
    return value;
}

template<typename Type>
[[nodiscard]] static std::string_view type_name(char) noexcept {
    static const auto value = stripped_type_name<Type>();
    return value;
}

template<typename Type, auto = stripped_type_name<Type>().find_first_of('.')>
[[nodiscard]] static constexpr id_type type_hash(int) noexcept {
    constexpr auto stripped = stripped_type_name<Type>();
    constexpr auto value = hashed_string::value(stripped.data(), stripped.size());
    return value;
}

template<typename Type>
[[nodiscard]] static id_type type_hash(char) noexcept {
    static const auto value = [](const auto stripped) {
        return hashed_string::value(stripped.data(), stripped.size());
    }(stripped_type_name<Type>());
    return value;
}

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Type sequential identifier.
 * @tparam Type Type for which to generate a sequential identifier.
 */
template<typename Type, typename = void>
struct ENTT_API type_index final {
    /**
     * @brief Returns the sequential identifier of a given type.
     * @return The sequential identifier of a given type.
     */
    [[nodiscard]] static id_type value() noexcept {
        static const id_type value = internal::type_index::next();
        return value;
    }

    /*! @copydoc value */
    [[nodiscard]] constexpr operator id_type() const noexcept {
        return value();
    }
};

/**
 * @brief Type hash.
 * @tparam Type Type for which to generate a hash value.
 */
template<typename Type, typename = void>
struct type_hash final {
    /**
     * @brief Returns the numeric representation of a given type.
     * @return The numeric representation of the given type.
     */
#if defined ENTT_PRETTY_FUNCTION
    [[nodiscard]] static constexpr id_type value() noexcept {
        return internal::type_hash<Type>(0);
#else
    [[nodiscard]] static constexpr id_type value() noexcept {
        return type_index<Type>::value();
#endif
    }

    /*! @copydoc value */
    [[nodiscard]] constexpr operator id_type() const noexcept {
        return value();
    }
};

/**
 * @brief Type name.
 * @tparam Type Type for which to generate a name.
 */
template<typename Type, typename = void>
struct type_name final {
    /**
     * @brief Returns the name of a given type.
     * @return The name of the given type.
     */
    [[nodiscard]] static constexpr std::string_view value() noexcept {
        return internal::type_name<Type>(0);
    }

    /*! @copydoc value */
    [[nodiscard]] constexpr operator std::string_view() const noexcept {
        return value();
    }
};

/*! @brief Implementation specific information about a type. */
struct type_info final {
    /**
     * @brief Constructs a type info object for a given type.
     * @tparam Type Type for which to construct a type info object.
     */
    template<typename Type>
    constexpr type_info(std::in_place_type_t<Type>) noexcept
        : seq{type_index<std::remove_cv_t<std::remove_reference_t<Type>>>::value()},
          identifier{type_hash<std::remove_cv_t<std::remove_reference_t<Type>>>::value()},
          alias{type_name<std::remove_cv_t<std::remove_reference_t<Type>>>::value()} {}

    /**
     * @brief Type index.
     * @return Type index.
     */
    [[nodiscard]] constexpr id_type index() const noexcept {
        return seq;
    }

    /**
     * @brief Type hash.
     * @return Type hash.
     */
    [[nodiscard]] constexpr id_type hash() const noexcept {
        return identifier;
    }

    /**
     * @brief Type name.
     * @return Type name.
     */
    [[nodiscard]] constexpr std::string_view name() const noexcept {
        return alias;
    }

private:
    id_type seq;
    id_type identifier;
    std::string_view alias;
};

/**
 * @brief Compares the contents of two type info objects.
 * @param lhs A type info object.
 * @param rhs A type info object.
 * @return True if the two type info objects are identical, false otherwise.
 */
[[nodiscard]] inline constexpr bool operator==(const type_info &lhs, const type_info &rhs) noexcept {
    return lhs.hash() == rhs.hash();
}

/**
 * @brief Compares the contents of two type info objects.
 * @param lhs A type info object.
 * @param rhs A type info object.
 * @return True if the two type info objects differ, false otherwise.
 */
[[nodiscard]] inline constexpr bool operator!=(const type_info &lhs, const type_info &rhs) noexcept {
    return !(lhs == rhs);
}

/**
 * @brief Compares two type info objects.
 * @param lhs A valid type info object.
 * @param rhs A valid type info object.
 * @return True if the first element is less than the second, false otherwise.
 */
[[nodiscard]] constexpr bool operator<(const type_info &lhs, const type_info &rhs) noexcept {
    return lhs.index() < rhs.index();
}

/**
 * @brief Compares two type info objects.
 * @param lhs A valid type info object.
 * @param rhs A valid type info object.
 * @return True if the first element is less than or equal to the second, false
 * otherwise.
 */
[[nodiscard]] constexpr bool operator<=(const type_info &lhs, const type_info &rhs) noexcept {
    return !(rhs < lhs);
}

/**
 * @brief Compares two type info objects.
 * @param lhs A valid type info object.
 * @param rhs A valid type info object.
 * @return True if the first element is greater than the second, false
 * otherwise.
 */
[[nodiscard]] constexpr bool operator>(const type_info &lhs, const type_info &rhs) noexcept {
    return rhs < lhs;
}

/**
 * @brief Compares two type info objects.
 * @param lhs A valid type info object.
 * @param rhs A valid type info object.
 * @return True if the first element is greater than or equal to the second,
 * false otherwise.
 */
[[nodiscard]] constexpr bool operator>=(const type_info &lhs, const type_info &rhs) noexcept {
    return !(lhs < rhs);
}

/**
 * @brief Returns the type info object associated to a given type.
 *
 * The returned element refers to an object with static storage duration.<br/>
 * The type doesn't need to be a complete type. If the type is a reference, the
 * result refers to the referenced type. In all cases, top-level cv-qualifiers
 * are ignored.
 *
 * @tparam Type Type for which to generate a type info object.
 * @return A reference to a properly initialized type info object.
 */
template<typename Type>
[[nodiscard]] const type_info &type_id() noexcept {
    if constexpr(std::is_same_v<Type, std::remove_cv_t<std::remove_reference_t<Type>>>) {
        static type_info instance{std::in_place_type<Type>};
        return instance;
    } else {
        return type_id<std::remove_cv_t<std::remove_reference_t<Type>>>();
    }
}

/*! @copydoc type_id */
template<typename Type>
[[nodiscard]] const type_info &type_id(Type &&) noexcept {
    return type_id<std::remove_cv_t<std::remove_reference_t<Type>>>();
}

} // namespace entt

#endif

/////////////////// END OF external/entt/core/type_info.hpp ////////////////////

///////////////////// START OF external/entt/core/any.hpp //////////////////////

#ifndef ENTT_CORE_ANY_HPP
#define ENTT_CORE_ANY_HPP

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

enum class any_operation : std::uint8_t {
    copy,
    move,
    transfer,
    assign,
    destroy,
    compare,
    get
};

enum class any_policy : std::uint8_t {
    owner,
    ref,
    cref
};

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief A SBO friendly, type-safe container for single values of any type.
 * @tparam Len Size of the storage reserved for the small buffer optimization.
 * @tparam Align Optional alignment requirement.
 */
template<std::size_t Len, std::size_t Align>
class basic_any {
    using operation = internal::any_operation;
    using policy = internal::any_policy;
    using vtable_type = const void *(const operation, const basic_any &, const void *);

    struct storage_type {
        alignas(Align) std::byte data[Len + !Len];
    };

    template<typename Type>
    static constexpr bool in_situ = Len && alignof(Type) <= Align && sizeof(Type) <= Len && std::is_nothrow_move_constructible_v<Type>;

    template<typename Type>
    static const void *basic_vtable(const operation op, const basic_any &value, const void *other) {
        static_assert(!std::is_same_v<Type, void> && std::is_same_v<std::remove_cv_t<std::remove_reference_t<Type>>, Type>, "Invalid type");
        const Type *element = nullptr;

        if constexpr(in_situ<Type>) {
            element = value.owner() ? reinterpret_cast<const Type *>(&value.storage) : static_cast<const Type *>(value.instance);
        } else {
            element = static_cast<const Type *>(value.instance);
        }

        switch(op) {
        case operation::copy:
            if constexpr(std::is_copy_constructible_v<Type>) {
                static_cast<basic_any *>(const_cast<void *>(other))->initialize<Type>(*element);
            }
            break;
        case operation::move:
            if constexpr(in_situ<Type>) {
                if(value.owner()) {
                    return new(&static_cast<basic_any *>(const_cast<void *>(other))->storage) Type{std::move(*const_cast<Type *>(element))};
                }
            }

            return (static_cast<basic_any *>(const_cast<void *>(other))->instance = std::exchange(const_cast<basic_any &>(value).instance, nullptr));
        case operation::transfer:
            if constexpr(std::is_move_assignable_v<Type>) {
                *const_cast<Type *>(element) = std::move(*static_cast<Type *>(const_cast<void *>(other)));
                return other;
            }
            [[fallthrough]];
        case operation::assign:
            if constexpr(std::is_copy_assignable_v<Type>) {
                *const_cast<Type *>(element) = *static_cast<const Type *>(other);
                return other;
            }
            break;
        case operation::destroy:
            if constexpr(in_situ<Type>) {
                element->~Type();
            } else if constexpr(std::is_array_v<Type>) {
                delete[] element;
            } else {
                delete element;
            }
            break;
        case operation::compare:
            if constexpr(!std::is_function_v<Type> && !std::is_array_v<Type> && is_equality_comparable_v<Type>) {
                return *element == *static_cast<const Type *>(other) ? other : nullptr;
            } else {
                return (element == other) ? other : nullptr;
            }
        case operation::get:
            return element;
        }

        return nullptr;
    }

    template<typename Type, typename... Args>
    void initialize([[maybe_unused]] Args &&...args) {
        info = &type_id<std::remove_cv_t<std::remove_reference_t<Type>>>();

        if constexpr(!std::is_void_v<Type>) {
            vtable = basic_vtable<std::remove_cv_t<std::remove_reference_t<Type>>>;

            if constexpr(std::is_lvalue_reference_v<Type>) {
                static_assert((std::is_lvalue_reference_v<Args> && ...) && (sizeof...(Args) == 1u), "Invalid arguments");
                mode = std::is_const_v<std::remove_reference_t<Type>> ? policy::cref : policy::ref;
                instance = (std::addressof(args), ...);
            } else if constexpr(in_situ<std::remove_cv_t<std::remove_reference_t<Type>>>) {
                if constexpr(std::is_aggregate_v<std::remove_cv_t<std::remove_reference_t<Type>>> && (sizeof...(Args) != 0u || !std::is_default_constructible_v<std::remove_cv_t<std::remove_reference_t<Type>>>)) {
                    new(&storage) std::remove_cv_t<std::remove_reference_t<Type>>{std::forward<Args>(args)...};
                } else {
                    new(&storage) std::remove_cv_t<std::remove_reference_t<Type>>(std::forward<Args>(args)...);
                }
            } else {
                if constexpr(std::is_aggregate_v<std::remove_cv_t<std::remove_reference_t<Type>>> && (sizeof...(Args) != 0u || !std::is_default_constructible_v<std::remove_cv_t<std::remove_reference_t<Type>>>)) {
                    instance = new std::remove_cv_t<std::remove_reference_t<Type>>{std::forward<Args>(args)...};
                } else {
                    instance = new std::remove_cv_t<std::remove_reference_t<Type>>(std::forward<Args>(args)...);
                }
            }
        }
    }

    basic_any(const basic_any &other, const policy pol) noexcept
        : instance{other.data()},
          info{other.info},
          vtable{other.vtable},
          mode{pol} {}

public:
    /*! @brief Size of the internal storage. */
    static constexpr auto length = Len;
    /*! @brief Alignment requirement. */
    static constexpr auto alignment = Align;

    /*! @brief Default constructor. */
    constexpr basic_any() noexcept
        : basic_any{std::in_place_type<void>} {}

    /**
     * @brief Constructs a wrapper by directly initializing the new object.
     * @tparam Type Type of object to use to initialize the wrapper.
     * @tparam Args Types of arguments to use to construct the new instance.
     * @param args Parameters to use to construct the instance.
     */
    template<typename Type, typename... Args>
    explicit basic_any(std::in_place_type_t<Type>, Args &&...args)
        : instance{},
          info{},
          vtable{},
          mode{policy::owner} {
        initialize<Type>(std::forward<Args>(args)...);
    }

    /**
     * @brief Constructs a wrapper from a given value.
     * @tparam Type Type of object to use to initialize the wrapper.
     * @param value An instance of an object to use to initialize the wrapper.
     */
    template<typename Type, typename = std::enable_if_t<!std::is_same_v<std::decay_t<Type>, basic_any>>>
    basic_any(Type &&value)
        : basic_any{std::in_place_type<std::decay_t<Type>>, std::forward<Type>(value)} {}

    /**
     * @brief Copy constructor.
     * @param other The instance to copy from.
     */
    basic_any(const basic_any &other)
        : basic_any{} {
        if(other.vtable) {
            other.vtable(operation::copy, other, this);
        }
    }

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    basic_any(basic_any &&other) noexcept
        : instance{},
          info{other.info},
          vtable{other.vtable},
          mode{other.mode} {
        if(other.vtable) {
            other.vtable(operation::move, other, this);
        }
    }

    /*! @brief Frees the internal storage, whatever it means. */
    ~basic_any() {
        if(vtable && owner()) {
            vtable(operation::destroy, *this, nullptr);
        }
    }

    /**
     * @brief Copy assignment operator.
     * @param other The instance to copy from.
     * @return This any object.
     */
    basic_any &operator=(const basic_any &other) {
        reset();

        if(other.vtable) {
            other.vtable(operation::copy, other, this);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This any object.
     */
    basic_any &operator=(basic_any &&other) noexcept {
        reset();

        if(other.vtable) {
            other.vtable(operation::move, other, this);
            info = other.info;
            vtable = other.vtable;
            mode = other.mode;
        }

        return *this;
    }

    /**
     * @brief Value assignment operator.
     * @tparam Type Type of object to use to initialize the wrapper.
     * @param value An instance of an object to use to initialize the wrapper.
     * @return This any object.
     */
    template<typename Type>
    std::enable_if_t<!std::is_same_v<std::decay_t<Type>, basic_any>, basic_any &>
    operator=(Type &&value) {
        emplace<std::decay_t<Type>>(std::forward<Type>(value));
        return *this;
    }

    /**
     * @brief Returns the object type if any, `type_id<void>()` otherwise.
     * @return The object type if any, `type_id<void>()` otherwise.
     */
    [[nodiscard]] const type_info &type() const noexcept {
        return *info;
    }

    /**
     * @brief Returns an opaque pointer to the contained instance.
     * @return An opaque pointer the contained instance, if any.
     */
    [[nodiscard]] const void *data() const noexcept {
        return vtable ? vtable(operation::get, *this, nullptr) : nullptr;
    }

    /**
     * @brief Returns an opaque pointer to the contained instance.
     * @param req Expected type.
     * @return An opaque pointer the contained instance, if any.
     */
    [[nodiscard]] const void *data(const type_info &req) const noexcept {
        return *info == req ? data() : nullptr;
    }

    /**
     * @brief Returns an opaque pointer to the contained instance.
     * @return An opaque pointer the contained instance, if any.
     */
    [[nodiscard]] void *data() noexcept {
        return mode == policy::cref ? nullptr : const_cast<void *>(std::as_const(*this).data());
    }

    /**
     * @brief Returns an opaque pointer to the contained instance.
     * @param req Expected type.
     * @return An opaque pointer the contained instance, if any.
     */
    [[nodiscard]] void *data(const type_info &req) noexcept {
        return mode == policy::cref ? nullptr : const_cast<void *>(std::as_const(*this).data(req));
    }

    /**
     * @brief Replaces the contained object by creating a new instance directly.
     * @tparam Type Type of object to use to initialize the wrapper.
     * @tparam Args Types of arguments to use to construct the new instance.
     * @param args Parameters to use to construct the instance.
     */
    template<typename Type, typename... Args>
    void emplace(Args &&...args) {
        reset();
        initialize<Type>(std::forward<Args>(args)...);
    }

    /**
     * @brief Assigns a value to the contained object without replacing it.
     * @param other The value to assign to the contained object.
     * @return True in case of success, false otherwise.
     */
    bool assign(const basic_any &other) {
        if(vtable && mode != policy::cref && *info == *other.info) {
            return (vtable(operation::assign, *this, other.data()) != nullptr);
        }

        return false;
    }

    /*! @copydoc assign */
    bool assign(basic_any &&other) {
        if(vtable && mode != policy::cref && *info == *other.info) {
            if(auto *val = other.data(); val) {
                return (vtable(operation::transfer, *this, val) != nullptr);
            } else {
                return (vtable(operation::assign, *this, std::as_const(other).data()) != nullptr);
            }
        }

        return false;
    }

    /*! @brief Destroys contained object */
    void reset() {
        if(vtable && owner()) {
            vtable(operation::destroy, *this, nullptr);
        }

        // unnecessary but it helps to detect nasty bugs
        ENTT_ASSERT((instance = nullptr) == nullptr, "");
        info = &type_id<void>();
        vtable = nullptr;
        mode = policy::owner;
    }

    /**
     * @brief Returns false if a wrapper is empty, true otherwise.
     * @return False if the wrapper is empty, true otherwise.
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return vtable != nullptr;
    }

    /**
     * @brief Checks if two wrappers differ in their content.
     * @param other Wrapper with which to compare.
     * @return False if the two objects differ in their content, true otherwise.
     */
    [[nodiscard]] bool operator==(const basic_any &other) const noexcept {
        if(vtable && *info == *other.info) {
            return (vtable(operation::compare, *this, other.data()) != nullptr);
        }

        return (!vtable && !other.vtable);
    }

    /**
     * @brief Checks if two wrappers differ in their content.
     * @param other Wrapper with which to compare.
     * @return True if the two objects differ in their content, false otherwise.
     */
    [[nodiscard]] bool operator!=(const basic_any &other) const noexcept {
        return !(*this == other);
    }

    /**
     * @brief Aliasing constructor.
     * @return A wrapper that shares a reference to an unmanaged object.
     */
    [[nodiscard]] basic_any as_ref() noexcept {
        return basic_any{*this, (mode == policy::cref ? policy::cref : policy::ref)};
    }

    /*! @copydoc as_ref */
    [[nodiscard]] basic_any as_ref() const noexcept {
        return basic_any{*this, policy::cref};
    }

    /**
     * @brief Returns true if a wrapper owns its object, false otherwise.
     * @return True if the wrapper owns its object, false otherwise.
     */
    [[nodiscard]] bool owner() const noexcept {
        return (mode == policy::owner);
    }

private:
    union {
        const void *instance;
        storage_type storage;
    };
    const type_info *info;
    vtable_type *vtable;
    policy mode;
};

/**
 * @brief Performs type-safe access to the contained object.
 * @tparam Type Type to which conversion is required.
 * @tparam Len Size of the storage reserved for the small buffer optimization.
 * @tparam Align Alignment requirement.
 * @param data Target any object.
 * @return The element converted to the requested type.
 */
template<typename Type, std::size_t Len, std::size_t Align>
[[nodiscard]] Type any_cast(const basic_any<Len, Align> &data) noexcept {
    const auto *const instance = any_cast<std::remove_reference_t<Type>>(&data);
    ENTT_ASSERT(instance, "Invalid instance");
    return static_cast<Type>(*instance);
}

/*! @copydoc any_cast */
template<typename Type, std::size_t Len, std::size_t Align>
[[nodiscard]] Type any_cast(basic_any<Len, Align> &data) noexcept {
    // forces const on non-reference types to make them work also with wrappers for const references
    auto *const instance = any_cast<std::remove_reference_t<const Type>>(&data);
    ENTT_ASSERT(instance, "Invalid instance");
    return static_cast<Type>(*instance);
}

/*! @copydoc any_cast */
template<typename Type, std::size_t Len, std::size_t Align>
[[nodiscard]] Type any_cast(basic_any<Len, Align> &&data) noexcept {
    if constexpr(std::is_copy_constructible_v<std::remove_cv_t<std::remove_reference_t<Type>>>) {
        if(auto *const instance = any_cast<std::remove_reference_t<Type>>(&data); instance) {
            return static_cast<Type>(std::move(*instance));
        } else {
            return any_cast<Type>(data);
        }
    } else {
        auto *const instance = any_cast<std::remove_reference_t<Type>>(&data);
        ENTT_ASSERT(instance, "Invalid instance");
        return static_cast<Type>(std::move(*instance));
    }
}

/*! @copydoc any_cast */
template<typename Type, std::size_t Len, std::size_t Align>
[[nodiscard]] const Type *any_cast(const basic_any<Len, Align> *data) noexcept {
    const auto &info = type_id<std::remove_cv_t<Type>>();
    return static_cast<const Type *>(data->data(info));
}

/*! @copydoc any_cast */
template<typename Type, std::size_t Len, std::size_t Align>
[[nodiscard]] Type *any_cast(basic_any<Len, Align> *data) noexcept {
    if constexpr(std::is_const_v<Type>) {
        // last attempt to make wrappers for const references return their values
        return any_cast<Type>(&std::as_const(*data));
    } else {
        const auto &info = type_id<std::remove_cv_t<Type>>();
        return static_cast<Type *>(data->data(info));
    }
}

/**
 * @brief Constructs a wrapper from a given type, passing it all arguments.
 * @tparam Type Type of object to use to initialize the wrapper.
 * @tparam Len Size of the storage reserved for the small buffer optimization.
 * @tparam Align Optional alignment requirement.
 * @tparam Args Types of arguments to use to construct the new instance.
 * @param args Parameters to use to construct the instance.
 * @return A properly initialized wrapper for an object of the given type.
 */
template<typename Type, std::size_t Len = basic_any<>::length, std::size_t Align = basic_any<Len>::alignment, typename... Args>
[[nodiscard]] basic_any<Len, Align> make_any(Args &&...args) {
    return basic_any<Len, Align>{std::in_place_type<Type>, std::forward<Args>(args)...};
}

/**
 * @brief Forwards its argument and avoids copies for lvalue references.
 * @tparam Len Size of the storage reserved for the small buffer optimization.
 * @tparam Align Optional alignment requirement.
 * @tparam Type Type of argument to use to construct the new instance.
 * @param value Parameter to use to construct the instance.
 * @return A properly initialized and not necessarily owning wrapper.
 */
template<std::size_t Len = basic_any<>::length, std::size_t Align = basic_any<Len>::alignment, typename Type>
[[nodiscard]] basic_any<Len, Align> forward_as_any(Type &&value) {
    return basic_any<Len, Align>{std::in_place_type<Type &&>, std::forward<Type>(value)};
}

} // namespace entt

#endif

////////////////////// END OF external/entt/core/any.hpp ///////////////////////

//////////////////// START OF external/entt/entity/fwd.hpp /////////////////////

#ifndef ENTT_ENTITY_FWD_HPP
#define ENTT_ENTITY_FWD_HPP

#include <cstdint>
#include <memory>
#include <type_traits>

namespace entt {

/*! @brief Default entity identifier. */
enum class entity : id_type {};

/*! @brief Storage deletion policy. */
enum class deletion_policy : std::uint8_t {
    /*! @brief Swap-and-pop deletion policy. */
    swap_and_pop = 0u,
    /*! @brief In-place deletion policy. */
    in_place = 1u
};

template<typename Entity = entity, typename = std::allocator<Entity>>
class basic_sparse_set;

template<typename Type, typename = entity, typename = std::allocator<Type>, typename = void>
class basic_storage;

template<typename Type>
class sigh_mixin;

/**
 * @brief Provides a common way to define storage types.
 * @tparam Type Storage value type.
 * @tparam Entity A valid entity type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Type, typename Entity = entity, typename Allocator = std::allocator<Type>, typename = void>
struct storage_type {
    /*! @brief Type-to-storage conversion result. */
    using type = sigh_mixin<basic_storage<Type, Entity, Allocator>>;
};

/**
 * @brief Helper type.
 * @tparam Args Arguments to forward.
 */
template<typename... Args>
using storage_type_t = typename storage_type<Args...>::type;

/**
 * Type-to-storage conversion utility that preserves constness.
 * @tparam Type Storage value type, eventually const.
 * @tparam Entity A valid entity type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Type, typename Entity = entity, typename Allocator = std::allocator<std::remove_const_t<Type>>>
struct storage_for {
    /*! @brief Type-to-storage conversion result. */
    using type = constness_as_t<storage_type_t<std::remove_const_t<Type>, Entity, Allocator>, Type>;
};

/**
 * @brief Helper type.
 * @tparam Args Arguments to forward.
 */
template<typename... Args>
using storage_for_t = typename storage_for<Args...>::type;

template<typename Entity = entity, typename = std::allocator<Entity>>
class basic_registry;

template<typename, typename, typename = void>
class basic_view;

template<typename Type, typename = std::allocator<Type *>>
class basic_runtime_view;

template<typename, typename, typename>
class basic_group;

template<typename, typename Mask = std::uint32_t, typename = std::allocator<Mask>>
class basic_observer;

template<typename>
class basic_organizer;

template<typename, typename...>
struct basic_handle;

template<typename>
class basic_snapshot;

template<typename>
class basic_snapshot_loader;

template<typename>
class basic_continuous_loader;

/**
 * @brief Alias for exclusion lists.
 * @tparam Type List of types.
 */
template<typename... Type>
struct exclude_t final: type_list<Type...> {
    /*! @brief Default constructor. */
    explicit constexpr exclude_t() {}
};

/**
 * @brief Variable template for exclusion lists.
 * @tparam Type List of types.
 */
template<typename... Type>
inline constexpr exclude_t<Type...> exclude{};

/**
 * @brief Alias for lists of observed components.
 * @tparam Type List of types.
 */
template<typename... Type>
struct get_t final: type_list<Type...> {
    /*! @brief Default constructor. */
    explicit constexpr get_t() {}
};

/**
 * @brief Variable template for lists of observed components.
 * @tparam Type List of types.
 */
template<typename... Type>
inline constexpr get_t<Type...> get{};

/**
 * @brief Alias for lists of owned components.
 * @tparam Type List of types.
 */
template<typename... Type>
struct owned_t final: type_list<Type...> {
    /*! @brief Default constructor. */
    explicit constexpr owned_t() {}
};

/**
 * @brief Variable template for lists of owned components.
 * @tparam Type List of types.
 */
template<typename... Type>
inline constexpr owned_t<Type...> owned{};

/**
 * @brief Applies a given _function_ to a get list and generate a new list.
 * @tparam Type Types provided by the get list.
 * @tparam Op Unary operation as template class with a type member named `type`.
 */
template<typename... Type, template<typename...> class Op>
struct type_list_transform<get_t<Type...>, Op> {
    /*! @brief Resulting get list after applying the transform function. */
    using type = get_t<typename Op<Type>::type...>;
};

/**
 * @brief Applies a given _function_ to an exclude list and generate a new list.
 * @tparam Type Types provided by the exclude list.
 * @tparam Op Unary operation as template class with a type member named `type`.
 */
template<typename... Type, template<typename...> class Op>
struct type_list_transform<exclude_t<Type...>, Op> {
    /*! @brief Resulting exclude list after applying the transform function. */
    using type = exclude_t<typename Op<Type>::type...>;
};

/**
 * @brief Applies a given _function_ to an owned list and generate a new list.
 * @tparam Type Types provided by the owned list.
 * @tparam Op Unary operation as template class with a type member named `type`.
 */
template<typename... Type, template<typename...> class Op>
struct type_list_transform<owned_t<Type...>, Op> {
    /*! @brief Resulting owned list after applying the transform function. */
    using type = owned_t<typename Op<Type>::type...>;
};

/*! @brief Alias declaration for the most common use case. */
using sparse_set = basic_sparse_set<>;

/**
 * @brief Alias declaration for the most common use case.
 * @tparam Type Type of objects assigned to the entities.
 */
template<typename Type>
using storage = basic_storage<Type>;

/*! @brief Alias declaration for the most common use case. */
using registry = basic_registry<>;

/*! @brief Alias declaration for the most common use case. */
using observer = basic_observer<registry>;

/*! @brief Alias declaration for the most common use case. */
using organizer = basic_organizer<registry>;

/*! @brief Alias declaration for the most common use case. */
using handle = basic_handle<registry>;

/*! @brief Alias declaration for the most common use case. */
using const_handle = basic_handle<const registry>;

/**
 * @brief Alias declaration for the most common use case.
 * @tparam Args Other template parameters.
 */
template<typename... Args>
using handle_view = basic_handle<registry, Args...>;

/**
 * @brief Alias declaration for the most common use case.
 * @tparam Args Other template parameters.
 */
template<typename... Args>
using const_handle_view = basic_handle<const registry, Args...>;

/*! @brief Alias declaration for the most common use case. */
using snapshot = basic_snapshot<registry>;

/*! @brief Alias declaration for the most common use case. */
using snapshot_loader = basic_snapshot_loader<registry>;

/*! @brief Alias declaration for the most common use case. */
using continuous_loader = basic_continuous_loader<registry>;

/**
 * @brief Alias declaration for the most common use case.
 * @tparam Get Types of storage iterated by the view.
 * @tparam Exclude Types of storage used to filter the view.
 */
template<typename Get, typename Exclude = exclude_t<>>
using view = basic_view<type_list_transform_t<Get, storage_for>, type_list_transform_t<Exclude, storage_for>>;

/*! @brief Alias declaration for the most common use case. */
using runtime_view = basic_runtime_view<sparse_set>;

/*! @brief Alias declaration for the most common use case. */
using const_runtime_view = basic_runtime_view<const sparse_set>;

/**
 * @brief Alias declaration for the most common use case.
 * @tparam Owned Types of storage _owned_ by the group.
 * @tparam Get Types of storage _observed_ by the group.
 * @tparam Exclude Types of storage used to filter the group.
 */
template<typename Owned, typename Get, typename Exclude>
using group = basic_group<type_list_transform_t<Owned, storage_for>, type_list_transform_t<Get, storage_for>, type_list_transform_t<Exclude, storage_for>>;

} // namespace entt

#endif

///////////////////// END OF external/entt/entity/fwd.hpp //////////////////////

/////////////////// START OF external/entt/entity/entity.hpp ///////////////////

#ifndef ENTT_ENTITY_ENTITY_HPP
#define ENTT_ENTITY_ENTITY_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

// waiting for C++20 (and std::popcount)
template<typename Type>
static constexpr int popcount(Type value) noexcept {
    return value ? (int(value & 1) + popcount(value >> 1)) : 0;
}

template<typename, typename = void>
struct entt_traits;

template<typename Type>
struct entt_traits<Type, std::enable_if_t<std::is_enum_v<Type>>>
    : entt_traits<std::underlying_type_t<Type>> {
    using value_type = Type;
};

template<typename Type>
struct entt_traits<Type, std::enable_if_t<std::is_class_v<Type>>>
    : entt_traits<typename Type::entity_type> {
    using value_type = Type;
};

template<>
struct entt_traits<std::uint32_t> {
    using value_type = std::uint32_t;

    using entity_type = std::uint32_t;
    using version_type = std::uint16_t;

    static constexpr entity_type entity_mask = 0xFFFFF;
    static constexpr entity_type version_mask = 0xFFF;
};

template<>
struct entt_traits<std::uint64_t> {
    using value_type = std::uint64_t;

    using entity_type = std::uint64_t;
    using version_type = std::uint32_t;

    static constexpr entity_type entity_mask = 0xFFFFFFFF;
    static constexpr entity_type version_mask = 0xFFFFFFFF;
};

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Common basic entity traits implementation.
 * @tparam Traits Actual entity traits to use.
 */
template<typename Traits>
class basic_entt_traits {
    static constexpr auto length = internal::popcount(Traits::entity_mask);

    static_assert(Traits::entity_mask && ((typename Traits::entity_type{1} << length) == (Traits::entity_mask + 1)), "Invalid entity mask");
    static_assert((typename Traits::entity_type{1} << internal::popcount(Traits::version_mask)) == (Traits::version_mask + 1), "Invalid version mask");

public:
    /*! @brief Value type. */
    using value_type = typename Traits::value_type;
    /*! @brief Underlying entity type. */
    using entity_type = typename Traits::entity_type;
    /*! @brief Underlying version type. */
    using version_type = typename Traits::version_type;

    /*! @brief Entity mask size. */
    static constexpr entity_type entity_mask = Traits::entity_mask;
    /*! @brief Version mask size */
    static constexpr entity_type version_mask = Traits::version_mask;

    /**
     * @brief Converts an entity to its underlying type.
     * @param value The value to convert.
     * @return The integral representation of the given value.
     */
    [[nodiscard]] static constexpr entity_type to_integral(const value_type value) noexcept {
        return static_cast<entity_type>(value);
    }

    /**
     * @brief Returns the entity part once converted to the underlying type.
     * @param value The value to convert.
     * @return The integral representation of the entity part.
     */
    [[nodiscard]] static constexpr entity_type to_entity(const value_type value) noexcept {
        return (to_integral(value) & entity_mask);
    }

    /**
     * @brief Returns the version part once converted to the underlying type.
     * @param value The value to convert.
     * @return The integral representation of the version part.
     */
    [[nodiscard]] static constexpr version_type to_version(const value_type value) noexcept {
        return static_cast<version_type>(to_integral(value) >> length);
    }

    /**
     * @brief Returns the successor of a given identifier.
     * @param value The identifier of which to return the successor.
     * @return The successor of the given identifier.
     */
    [[nodiscard]] static constexpr value_type next(const value_type value) noexcept {
        const auto vers = to_version(value) + 1;
        return construct(to_entity(value), static_cast<version_type>(vers + (vers == version_mask)));
    }

    /**
     * @brief Constructs an identifier from its parts.
     *
     * If the version part is not provided, a tombstone is returned.<br/>
     * If the entity part is not provided, a null identifier is returned.
     *
     * @param entity The entity part of the identifier.
     * @param version The version part of the identifier.
     * @return A properly constructed identifier.
     */
    [[nodiscard]] static constexpr value_type construct(const entity_type entity, const version_type version) noexcept {
        return value_type{(entity & entity_mask) | (static_cast<entity_type>(version) << length)};
    }

    /**
     * @brief Combines two identifiers in a single one.
     *
     * The returned identifier is a copy of the first element except for its
     * version, which is taken from the second element.
     *
     * @param lhs The identifier from which to take the entity part.
     * @param rhs The identifier from which to take the version part.
     * @return A properly constructed identifier.
     */
    [[nodiscard]] static constexpr value_type combine(const entity_type lhs, const entity_type rhs) noexcept {
        constexpr auto mask = (version_mask << length);
        return value_type{(lhs & entity_mask) | (rhs & mask)};
    }
};

/**
 * @brief Entity traits.
 * @tparam Type Type of identifier.
 */
template<typename Type>
struct entt_traits: basic_entt_traits<internal::entt_traits<Type>> {
    /*! @brief Base type. */
    using base_type = basic_entt_traits<internal::entt_traits<Type>>;
    /*! @brief Page size, default is `ENTT_SPARSE_PAGE`. */
    static constexpr std::size_t page_size = ENTT_SPARSE_PAGE;
};

/**
 * @copydoc entt_traits<Entity>::to_integral
 * @tparam Entity The value type.
 */
template<typename Entity>
[[nodiscard]] constexpr typename entt_traits<Entity>::entity_type to_integral(const Entity value) noexcept {
    return entt_traits<Entity>::to_integral(value);
}

/**
 * @copydoc entt_traits<Entity>::to_entity
 * @tparam Entity The value type.
 */
template<typename Entity>
[[nodiscard]] constexpr typename entt_traits<Entity>::entity_type to_entity(const Entity value) noexcept {
    return entt_traits<Entity>::to_entity(value);
}

/**
 * @copydoc entt_traits<Entity>::to_version
 * @tparam Entity The value type.
 */
template<typename Entity>
[[nodiscard]] constexpr typename entt_traits<Entity>::version_type to_version(const Entity value) noexcept {
    return entt_traits<Entity>::to_version(value);
}

/*! @brief Null object for all identifiers.  */
struct null_t {
    /**
     * @brief Converts the null object to identifiers of any type.
     * @tparam Entity Type of identifier.
     * @return The null representation for the given type.
     */
    template<typename Entity>
    [[nodiscard]] constexpr operator Entity() const noexcept {
        using traits_type = entt_traits<Entity>;
        constexpr auto value = traits_type::construct(traits_type::entity_mask, traits_type::version_mask);
        return value;
    }

    /**
     * @brief Compares two null objects.
     * @param other A null object.
     * @return True in all cases.
     */
    [[nodiscard]] constexpr bool operator==([[maybe_unused]] const null_t other) const noexcept {
        return true;
    }

    /**
     * @brief Compares two null objects.
     * @param other A null object.
     * @return False in all cases.
     */
    [[nodiscard]] constexpr bool operator!=([[maybe_unused]] const null_t other) const noexcept {
        return false;
    }

    /**
     * @brief Compares a null object and an identifier of any type.
     * @tparam Entity Type of identifier.
     * @param entity Identifier with which to compare.
     * @return False if the two elements differ, true otherwise.
     */
    template<typename Entity>
    [[nodiscard]] constexpr bool operator==(const Entity entity) const noexcept {
        using traits_type = entt_traits<Entity>;
        return traits_type::to_entity(entity) == traits_type::to_entity(*this);
    }

    /**
     * @brief Compares a null object and an identifier of any type.
     * @tparam Entity Type of identifier.
     * @param entity Identifier with which to compare.
     * @return True if the two elements differ, false otherwise.
     */
    template<typename Entity>
    [[nodiscard]] constexpr bool operator!=(const Entity entity) const noexcept {
        return !(entity == *this);
    }
};

/**
 * @brief Compares a null object and an identifier of any type.
 * @tparam Entity Type of identifier.
 * @param entity Identifier with which to compare.
 * @param other A null object yet to be converted.
 * @return False if the two elements differ, true otherwise.
 */
template<typename Entity>
[[nodiscard]] constexpr bool operator==(const Entity entity, const null_t other) noexcept {
    return other.operator==(entity);
}

/**
 * @brief Compares a null object and an identifier of any type.
 * @tparam Entity Type of identifier.
 * @param entity Identifier with which to compare.
 * @param other A null object yet to be converted.
 * @return True if the two elements differ, false otherwise.
 */
template<typename Entity>
[[nodiscard]] constexpr bool operator!=(const Entity entity, const null_t other) noexcept {
    return !(other == entity);
}

/*! @brief Tombstone object for all identifiers.  */
struct tombstone_t {
    /**
     * @brief Converts the tombstone object to identifiers of any type.
     * @tparam Entity Type of identifier.
     * @return The tombstone representation for the given type.
     */
    template<typename Entity>
    [[nodiscard]] constexpr operator Entity() const noexcept {
        using traits_type = entt_traits<Entity>;
        constexpr auto value = traits_type::construct(traits_type::entity_mask, traits_type::version_mask);
        return value;
    }

    /**
     * @brief Compares two tombstone objects.
     * @param other A tombstone object.
     * @return True in all cases.
     */
    [[nodiscard]] constexpr bool operator==([[maybe_unused]] const tombstone_t other) const noexcept {
        return true;
    }

    /**
     * @brief Compares two tombstone objects.
     * @param other A tombstone object.
     * @return False in all cases.
     */
    [[nodiscard]] constexpr bool operator!=([[maybe_unused]] const tombstone_t other) const noexcept {
        return false;
    }

    /**
     * @brief Compares a tombstone object and an identifier of any type.
     * @tparam Entity Type of identifier.
     * @param entity Identifier with which to compare.
     * @return False if the two elements differ, true otherwise.
     */
    template<typename Entity>
    [[nodiscard]] constexpr bool operator==(const Entity entity) const noexcept {
        using traits_type = entt_traits<Entity>;
        return traits_type::to_version(entity) == traits_type::to_version(*this);
    }

    /**
     * @brief Compares a tombstone object and an identifier of any type.
     * @tparam Entity Type of identifier.
     * @param entity Identifier with which to compare.
     * @return True if the two elements differ, false otherwise.
     */
    template<typename Entity>
    [[nodiscard]] constexpr bool operator!=(const Entity entity) const noexcept {
        return !(entity == *this);
    }
};

/**
 * @brief Compares a tombstone object and an identifier of any type.
 * @tparam Entity Type of identifier.
 * @param entity Identifier with which to compare.
 * @param other A tombstone object yet to be converted.
 * @return False if the two elements differ, true otherwise.
 */
template<typename Entity>
[[nodiscard]] constexpr bool operator==(const Entity entity, const tombstone_t other) noexcept {
    return other.operator==(entity);
}

/**
 * @brief Compares a tombstone object and an identifier of any type.
 * @tparam Entity Type of identifier.
 * @param entity Identifier with which to compare.
 * @param other A tombstone object yet to be converted.
 * @return True if the two elements differ, false otherwise.
 */
template<typename Entity>
[[nodiscard]] constexpr bool operator!=(const Entity entity, const tombstone_t other) noexcept {
    return !(other == entity);
}

/**
 * @brief Compile-time constant for null entities.
 *
 * There exist implicit conversions from this variable to identifiers of any
 * allowed type. Similarly, there exist comparison operators between the null
 * entity and any other identifier.
 */
inline constexpr null_t null{};

/**
 * @brief Compile-time constant for tombstone entities.
 *
 * There exist implicit conversions from this variable to identifiers of any
 * allowed type. Similarly, there exist comparison operators between the
 * tombstone entity and any other identifier.
 */
inline constexpr tombstone_t tombstone{};

} // namespace entt

#endif

//////////////////// END OF external/entt/entity/entity.hpp ////////////////////

///////////////// START OF external/entt/entity/sparse_set.hpp /////////////////

#ifndef ENTT_ENTITY_SPARSE_SET_HPP
#define ENTT_ENTITY_SPARSE_SET_HPP

#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename Container>
struct sparse_set_iterator final {
    using value_type = typename Container::value_type;
    using pointer = typename Container::const_pointer;
    using reference = typename Container::const_reference;
    using difference_type = typename Container::difference_type;
    using iterator_category = std::random_access_iterator_tag;

    constexpr sparse_set_iterator() noexcept
        : packed{},
          offset{} {}

    constexpr sparse_set_iterator(const Container &ref, const difference_type idx) noexcept
        : packed{std::addressof(ref)},
          offset{idx} {}

    constexpr sparse_set_iterator &operator++() noexcept {
        return --offset, *this;
    }

    constexpr sparse_set_iterator operator++(int) noexcept {
        sparse_set_iterator orig = *this;
        return ++(*this), orig;
    }

    constexpr sparse_set_iterator &operator--() noexcept {
        return ++offset, *this;
    }

    constexpr sparse_set_iterator operator--(int) noexcept {
        sparse_set_iterator orig = *this;
        return operator--(), orig;
    }

    constexpr sparse_set_iterator &operator+=(const difference_type value) noexcept {
        offset -= value;
        return *this;
    }

    constexpr sparse_set_iterator operator+(const difference_type value) const noexcept {
        sparse_set_iterator copy = *this;
        return (copy += value);
    }

    constexpr sparse_set_iterator &operator-=(const difference_type value) noexcept {
        return (*this += -value);
    }

    constexpr sparse_set_iterator operator-(const difference_type value) const noexcept {
        return (*this + -value);
    }

    [[nodiscard]] constexpr reference operator[](const difference_type value) const noexcept {
        return packed->data()[index() - value];
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept {
        return packed->data() + index();
    }

    [[nodiscard]] constexpr reference operator*() const noexcept {
        return *operator->();
    }

    [[nodiscard]] constexpr pointer data() const noexcept {
        return packed ? packed->data() : nullptr;
    }

    [[nodiscard]] constexpr difference_type index() const noexcept {
        return offset - 1;
    }

private:
    const Container *packed;
    difference_type offset;
};

template<typename Container>
[[nodiscard]] constexpr std::ptrdiff_t operator-(const sparse_set_iterator<Container> &lhs, const sparse_set_iterator<Container> &rhs) noexcept {
    return rhs.index() - lhs.index();
}

template<typename Container>
[[nodiscard]] constexpr bool operator==(const sparse_set_iterator<Container> &lhs, const sparse_set_iterator<Container> &rhs) noexcept {
    return lhs.index() == rhs.index();
}

template<typename Container>
[[nodiscard]] constexpr bool operator!=(const sparse_set_iterator<Container> &lhs, const sparse_set_iterator<Container> &rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Container>
[[nodiscard]] constexpr bool operator<(const sparse_set_iterator<Container> &lhs, const sparse_set_iterator<Container> &rhs) noexcept {
    return lhs.index() > rhs.index();
}

template<typename Container>
[[nodiscard]] constexpr bool operator>(const sparse_set_iterator<Container> &lhs, const sparse_set_iterator<Container> &rhs) noexcept {
    return rhs < lhs;
}

template<typename Container>
[[nodiscard]] constexpr bool operator<=(const sparse_set_iterator<Container> &lhs, const sparse_set_iterator<Container> &rhs) noexcept {
    return !(lhs > rhs);
}

template<typename Container>
[[nodiscard]] constexpr bool operator>=(const sparse_set_iterator<Container> &lhs, const sparse_set_iterator<Container> &rhs) noexcept {
    return !(lhs < rhs);
}

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Basic sparse set implementation.
 *
 * Sparse set or packed array or whatever is the name users give it.<br/>
 * Two arrays: an _external_ one and an _internal_ one; a _sparse_ one and a
 * _packed_ one; one used for direct access through contiguous memory, the other
 * one used to get the data through an extra level of indirection.<br/>
 * This type of data structure is widely documented in the literature and on the
 * web. This is nothing more than a customized implementation suitable for the
 * purpose of the framework.
 *
 * @note
 * Internal data structures arrange elements to maximize performance. There are
 * no guarantees that entities are returned in the insertion order when iterate
 * a sparse set. Do not make assumption on the order in any case.
 *
 * @tparam Entity A valid entity type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Entity, typename Allocator>
class basic_sparse_set {
    using alloc_traits = std::allocator_traits<Allocator>;
    static_assert(std::is_same_v<typename alloc_traits::value_type, Entity>, "Invalid value type");
    using sparse_container_type = std::vector<typename alloc_traits::pointer, typename alloc_traits::template rebind_alloc<typename alloc_traits::pointer>>;
    using packed_container_type = std::vector<Entity, Allocator>;

    [[nodiscard]] auto sparse_ptr(const Entity entt) const {
        const auto pos = static_cast<size_type>(traits_type::to_entity(entt));
        const auto page = pos / traits_type::page_size;
        return (page < sparse.size() && sparse[page]) ? (sparse[page] + fast_mod(pos, traits_type::page_size)) : nullptr;
    }

    [[nodiscard]] auto &sparse_ref(const Entity entt) const {
        ENTT_ASSERT(sparse_ptr(entt), "Invalid element");
        const auto pos = static_cast<size_type>(traits_type::to_entity(entt));
        return sparse[pos / traits_type::page_size][fast_mod(pos, traits_type::page_size)];
    }

    [[nodiscard]] auto to_iterator(const Entity entt) const {
        return --(end() - index(entt));
    }

    [[nodiscard]] auto &assure_at_least(const Entity entt) {
        const auto pos = static_cast<size_type>(traits_type::to_entity(entt));
        const auto page = pos / traits_type::page_size;

        if(!(page < sparse.size())) {
            sparse.resize(page + 1u, nullptr);
        }

        if(!sparse[page]) {
            auto page_allocator{packed.get_allocator()};
            sparse[page] = alloc_traits::allocate(page_allocator, traits_type::page_size);
            std::uninitialized_fill(sparse[page], sparse[page] + traits_type::page_size, null);
        }

        auto &elem = sparse[page][fast_mod(pos, traits_type::page_size)];
        ENTT_ASSERT(elem == null, "Slot not available");
        return elem;
    }

    void release_sparse_pages() {
        auto page_allocator{packed.get_allocator()};

        for(auto &&page: sparse) {
            if(page != nullptr) {
                std::destroy(page, page + traits_type::page_size);
                alloc_traits::deallocate(page_allocator, page, traits_type::page_size);
                page = nullptr;
            }
        }
    }

private:
    virtual const void *get_at(const std::size_t) const {
        return nullptr;
    }

    virtual void swap_or_move(const std::size_t, const std::size_t) {}

protected:
    /*! @brief Random access iterator type. */
    using basic_iterator = internal::sparse_set_iterator<packed_container_type>;

    /**
     * @brief Swaps two items at specific locations.
     * @param lhs A position to move from.
     * @param rhs The other position to move from.
     */
    void swap_at(const std::size_t lhs, const std::size_t rhs) {
        const auto entity = static_cast<typename traits_type::entity_type>(lhs);
        const auto other = static_cast<typename traits_type::entity_type>(rhs);

        sparse_ref(packed[lhs]) = traits_type::combine(other, traits_type::to_integral(packed[lhs]));
        sparse_ref(packed[rhs]) = traits_type::combine(entity, traits_type::to_integral(packed[rhs]));

        using std::swap;
        swap(packed[lhs], packed[rhs]);
    }

    /**
     * @brief Erases an entity from a sparse set.
     * @param it An iterator to the element to pop.
     */
    void swap_and_pop(const basic_iterator it) {
        ENTT_ASSERT(mode == deletion_policy::swap_and_pop, "Deletion policy mismatched");
        auto &self = sparse_ref(*it);
        const auto entt = traits_type::to_entity(self);
        sparse_ref(packed.back()) = traits_type::combine(entt, traits_type::to_integral(packed.back()));
        packed[static_cast<size_type>(entt)] = packed.back();
        // unnecessary but it helps to detect nasty bugs
        ENTT_ASSERT((packed.back() = null, true), "");
        // lazy self-assignment guard
        self = null;
        packed.pop_back();
    }

    /**
     * @brief Erases an entity from a sparse set.
     * @param it An iterator to the element to pop.
     */
    void in_place_pop(const basic_iterator it) {
        ENTT_ASSERT(mode == deletion_policy::in_place, "Deletion policy mismatched");
        const auto entt = traits_type::to_entity(std::exchange(sparse_ref(*it), null));
        packed[static_cast<size_type>(entt)] = std::exchange(free_list, traits_type::combine(entt, tombstone));
    }

protected:
    /**
     * @brief Erases entities from a sparse set.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     */
    virtual void pop(basic_iterator first, basic_iterator last) {
        if(mode == deletion_policy::swap_and_pop) {
            for(; first != last; ++first) {
                swap_and_pop(first);
            }
        } else {
            for(; first != last; ++first) {
                in_place_pop(first);
            }
        }
    }

    /*! @brief Erases all entities of a sparse set. */
    virtual void pop_all() {
        if(const auto prev = std::exchange(free_list, tombstone); prev == null) {
            for(auto first = begin(); !(first.index() < 0); ++first) {
                sparse_ref(*first) = null;
            }
        } else {
            for(auto first = begin(); !(first.index() < 0); ++first) {
                if(*first != tombstone) {
                    sparse_ref(*first) = null;
                }
            }
        }

        packed.clear();
    }

    /**
     * @brief Assigns an entity to a sparse set.
     * @param entt A valid identifier.
     * @param force_back Force back insertion.
     * @return Iterator pointing to the emplaced element.
     */
    virtual basic_iterator try_emplace(const Entity entt, const bool force_back, const void * = nullptr) {
        ENTT_ASSERT(!contains(entt), "Set already contains entity");

        if(auto &elem = assure_at_least(entt); free_list == null || force_back) {
            packed.push_back(entt);
            elem = traits_type::combine(static_cast<typename traits_type::entity_type>(packed.size() - 1u), traits_type::to_integral(entt));
            return begin();
        } else {
            const auto pos = static_cast<size_type>(traits_type::to_entity(free_list));
            elem = traits_type::combine(traits_type::to_integral(free_list), traits_type::to_integral(entt));
            free_list = std::exchange(packed[pos], entt);
            return --(end() - pos);
        }
    }

public:
    /*! @brief Entity traits. */
    using traits_type = entt_traits<Entity>;
    /*! @brief Underlying entity identifier. */
    using entity_type = typename traits_type::value_type;
    /*! @brief Underlying version type. */
    using version_type = typename traits_type::version_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Pointer type to contained entities. */
    using pointer = typename packed_container_type::const_pointer;
    /*! @brief Random access iterator type. */
    using iterator = basic_iterator;
    /*! @brief Constant random access iterator type. */
    using const_iterator = iterator;
    /*! @brief Reverse iterator type. */
    using reverse_iterator = std::reverse_iterator<iterator>;
    /*! @brief Constant reverse iterator type. */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /*! @brief Default constructor. */
    basic_sparse_set()
        : basic_sparse_set{type_id<void>()} {}

    /**
     * @brief Constructs an empty container with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit basic_sparse_set(const allocator_type &allocator)
        : basic_sparse_set{type_id<void>(), deletion_policy::swap_and_pop, allocator} {}

    /**
     * @brief Constructs an empty container with the given policy and allocator.
     * @param pol Type of deletion policy.
     * @param allocator The allocator to use (possibly default-constructed).
     */
    explicit basic_sparse_set(deletion_policy pol, const allocator_type &allocator = {})
        : basic_sparse_set{type_id<void>(), pol, allocator} {}

    /**
     * @brief Constructs an empty container with the given value type, policy
     * and allocator.
     * @param elem Returned value type, if any.
     * @param pol Type of deletion policy.
     * @param allocator The allocator to use (possibly default-constructed).
     */
    explicit basic_sparse_set(const type_info &elem, deletion_policy pol = deletion_policy::swap_and_pop, const allocator_type &allocator = {})
        : sparse{allocator},
          packed{allocator},
          info{&elem},
          free_list{tombstone},
          mode{pol} {}

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    basic_sparse_set(basic_sparse_set &&other) noexcept
        : sparse{std::move(other.sparse)},
          packed{std::move(other.packed)},
          info{other.info},
          free_list{std::exchange(other.free_list, tombstone)},
          mode{other.mode} {}

    /**
     * @brief Allocator-extended move constructor.
     * @param other The instance to move from.
     * @param allocator The allocator to use.
     */
    basic_sparse_set(basic_sparse_set &&other, const allocator_type &allocator) noexcept
        : sparse{std::move(other.sparse), allocator},
          packed{std::move(other.packed), allocator},
          info{other.info},
          free_list{std::exchange(other.free_list, tombstone)},
          mode{other.mode} {
        ENTT_ASSERT(alloc_traits::is_always_equal::value || packed.get_allocator() == other.packed.get_allocator(), "Copying a sparse set is not allowed");
    }

    /*! @brief Default destructor. */
    virtual ~basic_sparse_set() {
        release_sparse_pages();
    }

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This sparse set.
     */
    basic_sparse_set &operator=(basic_sparse_set &&other) noexcept {
        ENTT_ASSERT(alloc_traits::is_always_equal::value || packed.get_allocator() == other.packed.get_allocator(), "Copying a sparse set is not allowed");

        release_sparse_pages();
        sparse = std::move(other.sparse);
        packed = std::move(other.packed);
        info = other.info;
        free_list = std::exchange(other.free_list, tombstone);
        mode = other.mode;
        return *this;
    }

    /**
     * @brief Exchanges the contents with those of a given sparse set.
     * @param other Sparse set to exchange the content with.
     */
    void swap(basic_sparse_set &other) {
        using std::swap;
        swap(sparse, other.sparse);
        swap(packed, other.packed);
        swap(info, other.info);
        swap(free_list, other.free_list);
        swap(mode, other.mode);
    }

    /**
     * @brief Returns the associated allocator.
     * @return The associated allocator.
     */
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return packed.get_allocator();
    }

    /**
     * @brief Returns the deletion policy of a sparse set.
     * @return The deletion policy of the sparse set.
     */
    [[nodiscard]] deletion_policy policy() const noexcept {
        return mode;
    }

    /**
     * @brief Increases the capacity of a sparse set.
     *
     * If the new capacity is greater than the current capacity, new storage is
     * allocated, otherwise the method does nothing.
     *
     * @param cap Desired capacity.
     */
    virtual void reserve(const size_type cap) {
        packed.reserve(cap);
    }

    /**
     * @brief Returns the number of elements that a sparse set has currently
     * allocated space for.
     * @return Capacity of the sparse set.
     */
    [[nodiscard]] virtual size_type capacity() const noexcept {
        return packed.capacity();
    }

    /*! @brief Requests the removal of unused capacity. */
    virtual void shrink_to_fit() {
        packed.shrink_to_fit();
    }

    /**
     * @brief Returns the extent of a sparse set.
     *
     * The extent of a sparse set is also the size of the internal sparse array.
     * There is no guarantee that the internal packed array has the same size.
     * Usually the size of the internal sparse array is equal or greater than
     * the one of the internal packed array.
     *
     * @return Extent of the sparse set.
     */
    [[nodiscard]] size_type extent() const noexcept {
        return sparse.size() * traits_type::page_size;
    }

    /**
     * @brief Returns the number of elements in a sparse set.
     *
     * The number of elements is also the size of the internal packed array.
     * There is no guarantee that the internal sparse array has the same size.
     * Usually the size of the internal sparse array is equal or greater than
     * the one of the internal packed array.
     *
     * @return Number of elements.
     */
    [[nodiscard]] size_type size() const noexcept {
        return packed.size();
    }

    /**
     * @brief Checks whether a sparse set is empty.
     * @return True if the sparse set is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return packed.empty();
    }

    /**
     * @brief Checks whether a sparse set is fully packed.
     * @return True if the sparse set is fully packed, false otherwise.
     */
    [[nodiscard]] bool contiguous() const noexcept {
        return (free_list == null);
    }

    /**
     * @brief Direct access to the internal packed array.
     * @return A pointer to the internal packed array.
     */
    [[nodiscard]] pointer data() const noexcept {
        return packed.data();
    }

    /**
     * @brief Returns an iterator to the beginning.
     *
     * If the sparse set is empty, the returned iterator will be equal to
     * `end()`.
     *
     * @return An iterator to the first entity of the sparse set.
     */
    [[nodiscard]] const_iterator begin() const noexcept {
        const auto pos = static_cast<typename iterator::difference_type>(packed.size());
        return iterator{packed, pos};
    }

    /*! @copydoc begin */
    [[nodiscard]] const_iterator cbegin() const noexcept {
        return begin();
    }

    /**
     * @brief Returns an iterator to the end.
     * @return An iterator to the element following the last entity of a sparse
     * set.
     */
    [[nodiscard]] iterator end() const noexcept {
        return iterator{packed, {}};
    }

    /*! @copydoc end */
    [[nodiscard]] const_iterator cend() const noexcept {
        return end();
    }

    /**
     * @brief Returns a reverse iterator to the beginning.
     *
     * If the sparse set is empty, the returned iterator will be equal to
     * `rend()`.
     *
     * @return An iterator to the first entity of the reversed internal packed
     * array.
     */
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept {
        return std::make_reverse_iterator(end());
    }

    /*! @copydoc rbegin */
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    /**
     * @brief Returns a reverse iterator to the end.
     * @return An iterator to the element following the last entity of the
     * reversed sparse set.
     */
    [[nodiscard]] reverse_iterator rend() const noexcept {
        return std::make_reverse_iterator(begin());
    }

    /*! @copydoc rend */
    [[nodiscard]] const_reverse_iterator crend() const noexcept {
        return rend();
    }

    /**
     * @brief Finds an entity.
     * @param entt A valid identifier.
     * @return An iterator to the given entity if it's found, past the end
     * iterator otherwise.
     */
    [[nodiscard]] iterator find(const entity_type entt) const noexcept {
        return contains(entt) ? to_iterator(entt) : end();
    }

    /**
     * @brief Checks if a sparse set contains an entity.
     * @param entt A valid identifier.
     * @return True if the sparse set contains the entity, false otherwise.
     */
    [[nodiscard]] bool contains(const entity_type entt) const noexcept {
        const auto elem = sparse_ptr(entt);
        constexpr auto cap = traits_type::to_entity(null);
        // testing versions permits to avoid accessing the packed array
        return elem && (((~cap & traits_type::to_integral(entt)) ^ traits_type::to_integral(*elem)) < cap);
    }

    /**
     * @brief Returns the contained version for an identifier.
     * @param entt A valid identifier.
     * @return The version for the given identifier if present, the tombstone
     * version otherwise.
     */
    [[nodiscard]] version_type current(const entity_type entt) const noexcept {
        const auto elem = sparse_ptr(entt);
        constexpr auto fallback = traits_type::to_version(tombstone);
        return elem ? traits_type::to_version(*elem) : fallback;
    }

    /**
     * @brief Returns the position of an entity in a sparse set.
     *
     * @warning
     * Attempting to get the position of an entity that doesn't belong to the
     * sparse set results in undefined behavior.
     *
     * @param entt A valid identifier.
     * @return The position of the entity in the sparse set.
     */
    [[nodiscard]] size_type index(const entity_type entt) const noexcept {
        ENTT_ASSERT(contains(entt), "Set does not contain entity");
        return static_cast<size_type>(traits_type::to_entity(sparse_ref(entt)));
    }

    /**
     * @brief Returns the entity at specified location, with bounds checking.
     * @param pos The position for which to return the entity.
     * @return The entity at specified location if any, a null entity otherwise.
     */
    [[nodiscard]] entity_type at(const size_type pos) const noexcept {
        return pos < packed.size() ? packed[pos] : null;
    }

    /**
     * @brief Returns the entity at specified location, without bounds checking.
     * @param pos The position for which to return the entity.
     * @return The entity at specified location.
     */
    [[nodiscard]] entity_type operator[](const size_type pos) const noexcept {
        ENTT_ASSERT(pos < packed.size(), "Position is out of bounds");
        return packed[pos];
    }

    /**
     * @brief Returns the element assigned to an entity, if any.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the sparse set results
     * in undefined behavior.
     *
     * @param entt A valid identifier.
     * @return An opaque pointer to the element assigned to the entity, if any.
     */
    [[nodiscard]] const void *value(const entity_type entt) const noexcept {
        return get_at(index(entt));
    }

    /*! @copydoc value */
    [[nodiscard]] void *value(const entity_type entt) noexcept {
        return const_cast<void *>(std::as_const(*this).value(entt));
    }

    /**
     * @brief Assigns an entity to a sparse set.
     *
     * @warning
     * Attempting to assign an entity that already belongs to the sparse set
     * results in undefined behavior.
     *
     * @param entt A valid identifier.
     * @param elem Optional opaque element to forward to mixins, if any.
     * @return Iterator pointing to the emplaced element in case of success, the
     * `end()` iterator otherwise.
     */
    iterator push(const entity_type entt, const void *elem = nullptr) {
        return try_emplace(entt, false, elem);
    }

    /**
     * @brief Assigns one or more entities to a sparse set.
     *
     * @warning
     * Attempting to assign an entity that already belongs to the sparse set
     * results in undefined behavior.
     *
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     * @return Iterator pointing to the first element inserted in case of
     * success, the `end()` iterator otherwise.
     */
    template<typename It>
    iterator push(It first, It last) {
        for(auto it = first; it != last; ++it) {
            try_emplace(*it, true);
        }

        return first == last ? end() : find(*first);
    }

    /**
     * @brief Bump the version number of an entity.
     *
     * @warning
     * Attempting to bump the version of an entity that doesn't belong to the
     * sparse set results in undefined behavior.
     *
     * @param entt A valid identifier.
     * @return The version of the given identifier.
     */
    version_type bump(const entity_type entt) {
        auto &entity = sparse_ref(entt);
        ENTT_ASSERT(entt != tombstone && entity != null, "Cannot set the required version");
        entity = traits_type::combine(traits_type::to_integral(entity), traits_type::to_integral(entt));
        packed[static_cast<size_type>(traits_type::to_entity(entity))] = entt;
        return traits_type::to_version(entt);
    }

    /**
     * @brief Erases an entity from a sparse set.
     *
     * @warning
     * Attempting to erase an entity that doesn't belong to the sparse set
     * results in undefined behavior.
     *
     * @param entt A valid identifier.
     */
    void erase(const entity_type entt) {
        const auto it = to_iterator(entt);
        pop(it, it + 1u);
    }

    /**
     * @brief Erases entities from a set.
     *
     * @sa erase
     *
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     */
    template<typename It>
    void erase(It first, It last) {
        if constexpr(std::is_same_v<It, basic_iterator>) {
            pop(first, last);
        } else {
            for(; first != last; ++first) {
                erase(*first);
            }
        }
    }

    /**
     * @brief Removes an entity from a sparse set if it exists.
     * @param entt A valid identifier.
     * @return True if the entity is actually removed, false otherwise.
     */
    bool remove(const entity_type entt) {
        return contains(entt) && (erase(entt), true);
    }

    /**
     * @brief Removes entities from a sparse set if they exist.
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     * @return The number of entities actually removed.
     */
    template<typename It>
    size_type remove(It first, It last) {
        size_type count{};

        if constexpr(std::is_same_v<It, basic_iterator>) {
            while(first != last) {
                while(first != last && !contains(*first)) {
                    ++first;
                }

                const auto it = first;

                while(first != last && contains(*first)) {
                    ++first;
                }

                count += std::distance(it, first);
                erase(it, first);
            }
        } else {
            for(; first != last; ++first) {
                count += remove(*first);
            }
        }

        return count;
    }

    /*! @brief Removes all tombstones from a sparse set. */
    void compact() {
        size_type from = packed.size();
        for(; from && packed[from - 1u] == tombstone; --from) {}

        for(auto *it = &free_list; *it != null && from; it = std::addressof(packed[traits_type::to_entity(*it)])) {
            if(const size_type to = traits_type::to_entity(*it); to < from) {
                --from;
                swap_or_move(from, to);

                packed[to] = std::exchange(packed[from], tombstone);
                const auto entity = static_cast<typename traits_type::entity_type>(to);
                sparse_ref(packed[to]) = traits_type::combine(entity, traits_type::to_integral(packed[to]));

                *it = traits_type::combine(static_cast<typename traits_type::entity_type>(from), tombstone);
                for(; from && packed[from - 1u] == tombstone; --from) {}
            }
        }

        free_list = tombstone;
        packed.resize(from);
    }

    /**
     * @brief Swaps two entities in a sparse set.
     *
     * For what it's worth, this function affects both the internal sparse array
     * and the internal packed array. Users should not care of that anyway.
     *
     * @warning
     * Attempting to swap entities that don't belong to the sparse set results
     * in undefined behavior.
     *
     * @param lhs A valid identifier.
     * @param rhs A valid identifier.
     */
    void swap_elements(const entity_type lhs, const entity_type rhs) {
        const auto from = index(lhs);
        const auto to = index(rhs);

        // basic no-leak guarantee if swapping throws
        swap_or_move(from, to);
        swap_at(from, to);
    }

    /**
     * @brief Sort the first count elements according to the given comparison
     * function.
     *
     * The comparison function object must return `true` if the first element
     * is _less_ than the second one, `false` otherwise. The signature of the
     * comparison function should be equivalent to the following:
     *
     * @code{.cpp}
     * bool(const Entity, const Entity);
     * @endcode
     *
     * Moreover, the comparison function object shall induce a
     * _strict weak ordering_ on the values.
     *
     * The sort function object must offer a member function template
     * `operator()` that accepts three arguments:
     *
     * * An iterator to the first element of the range to sort.
     * * An iterator past the last element of the range to sort.
     * * A comparison function to use to compare the elements.
     *
     * @tparam Compare Type of comparison function object.
     * @tparam Sort Type of sort function object.
     * @tparam Args Types of arguments to forward to the sort function object.
     * @param length Number of elements to sort.
     * @param compare A valid comparison function object.
     * @param algo A valid sort function object.
     * @param args Arguments to forward to the sort function object, if any.
     */
    template<typename Compare, typename Sort = std_sort, typename... Args>
    void sort_n(const size_type length, Compare compare, Sort algo = Sort{}, Args &&...args) {
        ENTT_ASSERT(!(length > packed.size()), "Length exceeds the number of elements");
        ENTT_ASSERT(free_list == null, "Partial sorting with tombstones is not supported");

        algo(packed.rend() - length, packed.rend(), std::move(compare), std::forward<Args>(args)...);

        for(size_type pos{}; pos < length; ++pos) {
            auto curr = pos;
            auto next = index(packed[curr]);

            while(curr != next) {
                const auto idx = index(packed[next]);
                const auto entt = packed[curr];

                swap_or_move(next, idx);
                const auto entity = static_cast<typename traits_type::entity_type>(curr);
                sparse_ref(entt) = traits_type::combine(entity, traits_type::to_integral(packed[curr]));
                curr = std::exchange(next, idx);
            }
        }
    }

    /**
     * @brief Sort all elements according to the given comparison function.
     *
     * @sa sort_n
     *
     * @tparam Compare Type of comparison function object.
     * @tparam Sort Type of sort function object.
     * @tparam Args Types of arguments to forward to the sort function object.
     * @param compare A valid comparison function object.
     * @param algo A valid sort function object.
     * @param args Arguments to forward to the sort function object, if any.
     */
    template<typename Compare, typename Sort = std_sort, typename... Args>
    void sort(Compare compare, Sort algo = Sort{}, Args &&...args) {
        compact();
        sort_n(packed.size(), std::move(compare), std::move(algo), std::forward<Args>(args)...);
    }

    /**
     * @brief Sort entities according to their order in another sparse set.
     *
     * Entities that are part of both the sparse sets are ordered internally
     * according to the order they have in `other`.<br/>
     * All the other entities goes to the end of the list and there are no
     * guarantees on their order.
     *
     * @param other The sparse sets that imposes the order of the entities.
     */
    void sort_as(const basic_sparse_set &other) {
        compact();

        const auto to = other.end();
        auto from = other.begin();

        for(auto it = begin(); it.index() && from != to; ++from) {
            if(const auto curr = *from; contains(curr)) {
                if(const auto entt = *it; entt != curr) {
                    // basic no-leak guarantee (with invalid state) if swapping throws
                    swap_elements(entt, curr);
                }

                ++it;
            }
        }
    }

    /*! @brief Clears a sparse set. */
    void clear() {
        pop_all();
        // sanity check to avoid subtle issues due to storage classes
        ENTT_ASSERT((compact(), size()) == 0u, "Non-empty set");
        free_list = tombstone;
        packed.clear();
    }

    /**
     * @brief Returned value type, if any.
     * @return Returned value type, if any.
     */
    const type_info &type() const noexcept {
        return *info;
    }

    /*! @brief Forwards variables to derived classes, if any. */
    virtual void bind(any) noexcept {}

private:
    sparse_container_type sparse;
    packed_container_type packed;
    const type_info *info;
    entity_type free_list;
    deletion_policy mode;
};

} // namespace entt

#endif

////////////////// END OF external/entt/entity/sparse_set.hpp //////////////////

///////////////// START OF external/entt/entity/component.hpp //////////////////

#ifndef ENTT_ENTITY_COMPONENT_HPP
#define ENTT_ENTITY_COMPONENT_HPP

#include <cstddef>
#include <type_traits>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename Type, typename = void>
struct in_place_delete: std::bool_constant<!(std::is_move_constructible_v<Type> && std::is_move_assignable_v<Type>)> {};

template<>
struct in_place_delete<void>: std::false_type {};

template<typename Type>
struct in_place_delete<Type, std::enable_if_t<Type::in_place_delete>>
    : std::true_type {};

template<typename Type, typename = void>
struct page_size: std::integral_constant<std::size_t, !std::is_empty_v<ENTT_ETO_TYPE(Type)> * ENTT_PACKED_PAGE> {};

template<>
struct page_size<void>: std::integral_constant<std::size_t, 0u> {};

template<typename Type>
struct page_size<Type, std::enable_if_t<std::is_convertible_v<decltype(Type::page_size), std::size_t>>>
    : std::integral_constant<std::size_t, Type::page_size> {};

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Common way to access various properties of components.
 * @tparam Type Type of component.
 */
template<typename Type, typename = void>
struct component_traits {
    static_assert(std::is_same_v<std::decay_t<Type>, Type>, "Unsupported type");

    /*! @brief Component type. */
    using type = Type;

    /*! @brief Pointer stability, default is `false`. */
    static constexpr bool in_place_delete = internal::in_place_delete<Type>::value;
    /*! @brief Page size, default is `ENTT_PACKED_PAGE` for non-empty types. */
    static constexpr std::size_t page_size = internal::page_size<Type>::value;
};

} // namespace entt

#endif

////////////////// END OF external/entt/entity/component.hpp ///////////////////

////////////////// START OF external/entt/entity/storage.hpp ///////////////////

#ifndef ENTT_ENTITY_STORAGE_HPP
#define ENTT_ENTITY_STORAGE_HPP

#include <cstddef>
#include <iterator>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename Container, std::size_t Size>
class storage_iterator final {
    friend storage_iterator<const Container, Size>;

    using container_type = std::remove_const_t<Container>;
    using alloc_traits = std::allocator_traits<typename container_type::allocator_type>;

    using iterator_traits = std::iterator_traits<std::conditional_t<
        std::is_const_v<Container>,
        typename alloc_traits::template rebind_traits<typename std::pointer_traits<typename container_type::value_type>::element_type>::const_pointer,
        typename alloc_traits::template rebind_traits<typename std::pointer_traits<typename container_type::value_type>::element_type>::pointer>>;

public:
    using value_type = typename iterator_traits::value_type;
    using pointer = typename iterator_traits::pointer;
    using reference = typename iterator_traits::reference;
    using difference_type = typename iterator_traits::difference_type;
    using iterator_category = std::random_access_iterator_tag;

    constexpr storage_iterator() noexcept = default;

    constexpr storage_iterator(Container *ref, const difference_type idx) noexcept
        : payload{ref},
          offset{idx} {}

    template<bool Const = std::is_const_v<Container>, typename = std::enable_if_t<Const>>
    constexpr storage_iterator(const storage_iterator<std::remove_const_t<Container>, Size> &other) noexcept
        : storage_iterator{other.payload, other.offset} {}

    constexpr storage_iterator &operator++() noexcept {
        return --offset, *this;
    }

    constexpr storage_iterator operator++(int) noexcept {
        storage_iterator orig = *this;
        return ++(*this), orig;
    }

    constexpr storage_iterator &operator--() noexcept {
        return ++offset, *this;
    }

    constexpr storage_iterator operator--(int) noexcept {
        storage_iterator orig = *this;
        return operator--(), orig;
    }

    constexpr storage_iterator &operator+=(const difference_type value) noexcept {
        offset -= value;
        return *this;
    }

    constexpr storage_iterator operator+(const difference_type value) const noexcept {
        storage_iterator copy = *this;
        return (copy += value);
    }

    constexpr storage_iterator &operator-=(const difference_type value) noexcept {
        return (*this += -value);
    }

    constexpr storage_iterator operator-(const difference_type value) const noexcept {
        return (*this + -value);
    }

    [[nodiscard]] constexpr reference operator[](const difference_type value) const noexcept {
        const auto pos = index() - value;
        return (*payload)[pos / Size][fast_mod(pos, Size)];
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept {
        const auto pos = index();
        return (*payload)[pos / Size] + fast_mod(pos, Size);
    }

    [[nodiscard]] constexpr reference operator*() const noexcept {
        return *operator->();
    }

    [[nodiscard]] constexpr difference_type index() const noexcept {
        return offset - 1;
    }

private:
    Container *payload;
    difference_type offset;
};

template<typename Lhs, typename Rhs, std::size_t Size>
[[nodiscard]] constexpr std::ptrdiff_t operator-(const storage_iterator<Lhs, Size> &lhs, const storage_iterator<Rhs, Size> &rhs) noexcept {
    return rhs.index() - lhs.index();
}

template<typename Lhs, typename Rhs, std::size_t Size>
[[nodiscard]] constexpr bool operator==(const storage_iterator<Lhs, Size> &lhs, const storage_iterator<Rhs, Size> &rhs) noexcept {
    return lhs.index() == rhs.index();
}

template<typename Lhs, typename Rhs, std::size_t Size>
[[nodiscard]] constexpr bool operator!=(const storage_iterator<Lhs, Size> &lhs, const storage_iterator<Rhs, Size> &rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Lhs, typename Rhs, std::size_t Size>
[[nodiscard]] constexpr bool operator<(const storage_iterator<Lhs, Size> &lhs, const storage_iterator<Rhs, Size> &rhs) noexcept {
    return lhs.index() > rhs.index();
}

template<typename Lhs, typename Rhs, std::size_t Size>
[[nodiscard]] constexpr bool operator>(const storage_iterator<Lhs, Size> &lhs, const storage_iterator<Rhs, Size> &rhs) noexcept {
    return rhs < lhs;
}

template<typename Lhs, typename Rhs, std::size_t Size>
[[nodiscard]] constexpr bool operator<=(const storage_iterator<Lhs, Size> &lhs, const storage_iterator<Rhs, Size> &rhs) noexcept {
    return !(lhs > rhs);
}

template<typename Lhs, typename Rhs, std::size_t Size>
[[nodiscard]] constexpr bool operator>=(const storage_iterator<Lhs, Size> &lhs, const storage_iterator<Rhs, Size> &rhs) noexcept {
    return !(lhs < rhs);
}

template<typename It, typename... Other>
class extended_storage_iterator final {
    template<typename Iter, typename... Args>
    friend class extended_storage_iterator;

public:
    using iterator_type = It;
    using difference_type = std::ptrdiff_t;
    using value_type = decltype(std::tuple_cat(std::make_tuple(*std::declval<It>()), std::forward_as_tuple(*std::declval<Other>()...)));
    using pointer = input_iterator_pointer<value_type>;
    using reference = value_type;
    using iterator_category = std::input_iterator_tag;

    constexpr extended_storage_iterator()
        : it{} {}

    constexpr extended_storage_iterator(It base, Other... other)
        : it{base, other...} {}

    template<typename... Args, typename = std::enable_if_t<(!std::is_same_v<Other, Args> && ...) && (std::is_constructible_v<Other, Args> && ...)>>
    constexpr extended_storage_iterator(const extended_storage_iterator<It, Args...> &other)
        : it{other.it} {}

    constexpr extended_storage_iterator &operator++() noexcept {
        return ++std::get<It>(it), (++std::get<Other>(it), ...), *this;
    }

    constexpr extended_storage_iterator operator++(int) noexcept {
        extended_storage_iterator orig = *this;
        return ++(*this), orig;
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept {
        return operator*();
    }

    [[nodiscard]] constexpr reference operator*() const noexcept {
        return {*std::get<It>(it), *std::get<Other>(it)...};
    }

    [[nodiscard]] constexpr iterator_type base() const noexcept {
        return std::get<It>(it);
    }

    template<typename... Lhs, typename... Rhs>
    friend constexpr bool operator==(const extended_storage_iterator<Lhs...> &, const extended_storage_iterator<Rhs...> &) noexcept;

private:
    std::tuple<It, Other...> it;
};

template<typename... Lhs, typename... Rhs>
[[nodiscard]] constexpr bool operator==(const extended_storage_iterator<Lhs...> &lhs, const extended_storage_iterator<Rhs...> &rhs) noexcept {
    return std::get<0>(lhs.it) == std::get<0>(rhs.it);
}

template<typename... Lhs, typename... Rhs>
[[nodiscard]] constexpr bool operator!=(const extended_storage_iterator<Lhs...> &lhs, const extended_storage_iterator<Rhs...> &rhs) noexcept {
    return !(lhs == rhs);
}

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Basic storage implementation.
 *
 * Internal data structures arrange elements to maximize performance. There are
 * no guarantees that objects are returned in the insertion order when iterate
 * a storage. Do not make assumption on the order in any case.
 *
 * @warning
 * Empty types aren't explicitly instantiated. Therefore, many of the functions
 * normally available for non-empty types will not be available for empty ones.
 *
 * @tparam Type Type of objects assigned to the entities.
 * @tparam Entity A valid entity type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Type, typename Entity, typename Allocator, typename>
class basic_storage: public basic_sparse_set<Entity, typename std::allocator_traits<Allocator>::template rebind_alloc<Entity>> {
    using alloc_traits = std::allocator_traits<Allocator>;
    static_assert(std::is_same_v<typename alloc_traits::value_type, Type>, "Invalid value type");
    using container_type = std::vector<typename alloc_traits::pointer, typename alloc_traits::template rebind_alloc<typename alloc_traits::pointer>>;
    using underlying_type = basic_sparse_set<Entity, typename alloc_traits::template rebind_alloc<Entity>>;
    using underlying_iterator = typename underlying_type::basic_iterator;

    static constexpr bool is_pinned_type_v = !(std::is_move_constructible_v<Type> && std::is_move_assignable_v<Type>);

    [[nodiscard]] auto &element_at(const std::size_t pos) const {
        return payload[pos / traits_type::page_size][fast_mod(pos, traits_type::page_size)];
    }

    auto assure_at_least(const std::size_t pos) {
        const auto idx = pos / traits_type::page_size;

        if(!(idx < payload.size())) {
            auto curr = payload.size();
            allocator_type allocator{get_allocator()};
            payload.resize(idx + 1u, nullptr);

            ENTT_TRY {
                for(const auto last = payload.size(); curr < last; ++curr) {
                    payload[curr] = alloc_traits::allocate(allocator, traits_type::page_size);
                }
            }
            ENTT_CATCH {
                payload.resize(curr);
                ENTT_THROW;
            }
        }

        return payload[idx] + fast_mod(pos, traits_type::page_size);
    }

    template<typename... Args>
    auto emplace_element(const Entity entt, const bool force_back, Args &&...args) {
        const auto it = base_type::try_emplace(entt, force_back);

        ENTT_TRY {
            auto elem = assure_at_least(static_cast<size_type>(it.index()));
            entt::uninitialized_construct_using_allocator(to_address(elem), get_allocator(), std::forward<Args>(args)...);
        }
        ENTT_CATCH {
            base_type::pop(it, it + 1u);
            ENTT_THROW;
        }

        return it;
    }

    void shrink_to_size(const std::size_t sz) {
        const auto from = (sz + traits_type::page_size - 1u) / traits_type::page_size;
        allocator_type allocator{get_allocator()};

        for(auto pos = sz, length = base_type::size(); pos < length; ++pos) {
            if constexpr(traits_type::in_place_delete) {
                if(base_type::at(pos) != tombstone) {
                    alloc_traits::destroy(allocator, std::addressof(element_at(pos)));
                }
            } else {
                alloc_traits::destroy(allocator, std::addressof(element_at(pos)));
            }
        }

        for(auto pos = from, last = payload.size(); pos < last; ++pos) {
            alloc_traits::deallocate(allocator, payload[pos], traits_type::page_size);
        }

        payload.resize(from);
    }

private:
    const void *get_at(const std::size_t pos) const final {
        return std::addressof(element_at(pos));
    }

    void swap_or_move([[maybe_unused]] const std::size_t from, [[maybe_unused]] const std::size_t to) override {
        // use a runtime value to avoid compile-time suppression that drives the code coverage tool crazy
        ENTT_ASSERT((from + 1u) && !is_pinned_type_v, "Pinned type");

        if constexpr(!is_pinned_type_v) {
            auto &elem = element_at(from);

            if constexpr(traits_type::in_place_delete) {
                if(base_type::operator[](to) == tombstone) {
                    allocator_type allocator{get_allocator()};
                    entt::uninitialized_construct_using_allocator(to_address(assure_at_least(to)), allocator, std::move(elem));
                    alloc_traits::destroy(allocator, std::addressof(elem));
                    return;
                }
            }

            using std::swap;
            swap(elem, element_at(to));
        }
    }

protected:
    /**
     * @brief Erases entities from a storage.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     */
    void pop(underlying_iterator first, underlying_iterator last) override {
        for(allocator_type allocator{get_allocator()}; first != last; ++first) {
            // cannot use first.index() because it would break with cross iterators
            auto &elem = element_at(base_type::index(*first));

            if constexpr(traits_type::in_place_delete) {
                base_type::in_place_pop(first);
                alloc_traits::destroy(allocator, std::addressof(elem));
            } else {
                auto &other = element_at(base_type::size() - 1u);
                // destroying on exit allows reentrant destructors
                [[maybe_unused]] auto unused = std::exchange(elem, std::move(other));
                alloc_traits::destroy(allocator, std::addressof(other));
                base_type::swap_and_pop(first);
            }
        }
    }

    /*! @brief Erases all entities of a storage. */
    void pop_all() override {
        allocator_type allocator{get_allocator()};

        for(auto first = base_type::begin(); !(first.index() < 0); ++first) {
            if constexpr(traits_type::in_place_delete) {
                if(*first != tombstone) {
                    base_type::in_place_pop(first);
                    alloc_traits::destroy(allocator, std::addressof(element_at(static_cast<size_type>(first.index()))));
                }
            } else {
                base_type::swap_and_pop(first);
                alloc_traits::destroy(allocator, std::addressof(element_at(static_cast<size_type>(first.index()))));
            }
        }
    }

    /**
     * @brief Assigns an entity to a storage.
     * @param entt A valid identifier.
     * @param value Optional opaque value.
     * @param force_back Force back insertion.
     * @return Iterator pointing to the emplaced element.
     */
    underlying_iterator try_emplace([[maybe_unused]] const Entity entt, [[maybe_unused]] const bool force_back, const void *value) override {
        if(value) {
            if constexpr(std::is_copy_constructible_v<value_type>) {
                return emplace_element(entt, force_back, *static_cast<const value_type *>(value));
            } else {
                return base_type::end();
            }
        } else {
            if constexpr(std::is_default_constructible_v<value_type>) {
                return emplace_element(entt, force_back);
            } else {
                return base_type::end();
            }
        }
    }

public:
    /*! @brief Base type. */
    using base_type = underlying_type;
    /*! @brief Type of the objects assigned to entities. */
    using value_type = Type;
    /*! @brief Component traits. */
    using traits_type = component_traits<value_type>;
    /*! @brief Underlying entity identifier. */
    using entity_type = Entity;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Pointer type to contained elements. */
    using pointer = typename container_type::pointer;
    /*! @brief Constant pointer type to contained elements. */
    using const_pointer = typename alloc_traits::template rebind_traits<typename alloc_traits::const_pointer>::const_pointer;
    /*! @brief Random access iterator type. */
    using iterator = internal::storage_iterator<container_type, traits_type::page_size>;
    /*! @brief Constant random access iterator type. */
    using const_iterator = internal::storage_iterator<const container_type, traits_type::page_size>;
    /*! @brief Reverse iterator type. */
    using reverse_iterator = std::reverse_iterator<iterator>;
    /*! @brief Constant reverse iterator type. */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    /*! @brief Extended iterable storage proxy. */
    using iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::iterator, iterator>>;
    /*! @brief Constant extended iterable storage proxy. */
    using const_iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::const_iterator, const_iterator>>;
    /*! @brief Extended reverse iterable storage proxy. */
    using reverse_iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::reverse_iterator, reverse_iterator>>;
    /*! @brief Constant extended reverse iterable storage proxy. */
    using const_reverse_iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::const_reverse_iterator, const_reverse_iterator>>;

    /*! @brief Default constructor. */
    basic_storage()
        : basic_storage{allocator_type{}} {}

    /**
     * @brief Constructs an empty storage with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit basic_storage(const allocator_type &allocator)
        : base_type{type_id<value_type>(), deletion_policy{traits_type::in_place_delete}, allocator},
          payload{allocator} {}

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    basic_storage(basic_storage &&other) noexcept
        : base_type{std::move(other)},
          payload{std::move(other.payload)} {}

    /**
     * @brief Allocator-extended move constructor.
     * @param other The instance to move from.
     * @param allocator The allocator to use.
     */
    basic_storage(basic_storage &&other, const allocator_type &allocator) noexcept
        : base_type{std::move(other), allocator},
          payload{std::move(other.payload), allocator} {
        ENTT_ASSERT(alloc_traits::is_always_equal::value || payload.get_allocator() == other.payload.get_allocator(), "Copying a storage is not allowed");
    }

    /*! @brief Default destructor. */
    ~basic_storage() override {
        shrink_to_size(0u);
    }

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This storage.
     */
    basic_storage &operator=(basic_storage &&other) noexcept {
        ENTT_ASSERT(alloc_traits::is_always_equal::value || payload.get_allocator() == other.payload.get_allocator(), "Copying a storage is not allowed");

        shrink_to_size(0u);
        base_type::operator=(std::move(other));
        payload = std::move(other.payload);
        return *this;
    }

    /**
     * @brief Exchanges the contents with those of a given storage.
     * @param other Storage to exchange the content with.
     */
    void swap(basic_storage &other) {
        using std::swap;
        base_type::swap(other);
        swap(payload, other.payload);
    }

    /**
     * @brief Returns the associated allocator.
     * @return The associated allocator.
     */
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return payload.get_allocator();
    }

    /**
     * @brief Increases the capacity of a storage.
     *
     * If the new capacity is greater than the current capacity, new storage is
     * allocated, otherwise the method does nothing.
     *
     * @param cap Desired capacity.
     */
    void reserve(const size_type cap) override {
        if(cap != 0u) {
            base_type::reserve(cap);
            assure_at_least(cap - 1u);
        }
    }

    /**
     * @brief Returns the number of elements that a storage has currently
     * allocated space for.
     * @return Capacity of the storage.
     */
    [[nodiscard]] size_type capacity() const noexcept override {
        return payload.size() * traits_type::page_size;
    }

    /*! @brief Requests the removal of unused capacity. */
    void shrink_to_fit() override {
        base_type::shrink_to_fit();
        shrink_to_size(base_type::size());
    }

    /**
     * @brief Direct access to the array of objects.
     * @return A pointer to the array of objects.
     */
    [[nodiscard]] const_pointer raw() const noexcept {
        return payload.data();
    }

    /*! @copydoc raw */
    [[nodiscard]] pointer raw() noexcept {
        return payload.data();
    }

    /**
     * @brief Returns an iterator to the beginning.
     *
     * If the storage is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first instance of the internal array.
     */
    [[nodiscard]] const_iterator cbegin() const noexcept {
        const auto pos = static_cast<typename iterator::difference_type>(base_type::size());
        return const_iterator{&payload, pos};
    }

    /*! @copydoc cbegin */
    [[nodiscard]] const_iterator begin() const noexcept {
        return cbegin();
    }

    /*! @copydoc begin */
    [[nodiscard]] iterator begin() noexcept {
        const auto pos = static_cast<typename iterator::difference_type>(base_type::size());
        return iterator{&payload, pos};
    }

    /**
     * @brief Returns an iterator to the end.
     * @return An iterator to the element following the last instance of the
     * internal array.
     */
    [[nodiscard]] const_iterator cend() const noexcept {
        return const_iterator{&payload, {}};
    }

    /*! @copydoc cend */
    [[nodiscard]] const_iterator end() const noexcept {
        return cend();
    }

    /*! @copydoc end */
    [[nodiscard]] iterator end() noexcept {
        return iterator{&payload, {}};
    }

    /**
     * @brief Returns a reverse iterator to the beginning.
     *
     * If the storage is empty, the returned iterator will be equal to `rend()`.
     *
     * @return An iterator to the first instance of the reversed internal array.
     */
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept {
        return std::make_reverse_iterator(cend());
    }

    /*! @copydoc crbegin */
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept {
        return crbegin();
    }

    /*! @copydoc rbegin */
    [[nodiscard]] reverse_iterator rbegin() noexcept {
        return std::make_reverse_iterator(end());
    }

    /**
     * @brief Returns a reverse iterator to the end.
     * @return An iterator to the element following the last instance of the
     * reversed internal array.
     */
    [[nodiscard]] const_reverse_iterator crend() const noexcept {
        return std::make_reverse_iterator(cbegin());
    }

    /*! @copydoc crend */
    [[nodiscard]] const_reverse_iterator rend() const noexcept {
        return crend();
    }

    /*! @copydoc rend */
    [[nodiscard]] reverse_iterator rend() noexcept {
        return std::make_reverse_iterator(begin());
    }

    /**
     * @brief Returns the object assigned to an entity.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the storage results in
     * undefined behavior.
     *
     * @param entt A valid identifier.
     * @return The object assigned to the entity.
     */
    [[nodiscard]] const value_type &get(const entity_type entt) const noexcept {
        return element_at(base_type::index(entt));
    }

    /*! @copydoc get */
    [[nodiscard]] value_type &get(const entity_type entt) noexcept {
        return const_cast<value_type &>(std::as_const(*this).get(entt));
    }

    /**
     * @brief Returns the object assigned to an entity as a tuple.
     * @param entt A valid identifier.
     * @return The object assigned to the entity as a tuple.
     */
    [[nodiscard]] std::tuple<const value_type &> get_as_tuple(const entity_type entt) const noexcept {
        return std::forward_as_tuple(get(entt));
    }

    /*! @copydoc get_as_tuple */
    [[nodiscard]] std::tuple<value_type &> get_as_tuple(const entity_type entt) noexcept {
        return std::forward_as_tuple(get(entt));
    }

    /**
     * @brief Assigns an entity to a storage and constructs its object.
     *
     * @warning
     * Attempting to use an entity that already belongs to the storage results
     * in undefined behavior.
     *
     * @tparam Args Types of arguments to use to construct the object.
     * @param entt A valid identifier.
     * @param args Parameters to use to construct an object for the entity.
     * @return A reference to the newly created object.
     */
    template<typename... Args>
    value_type &emplace(const entity_type entt, Args &&...args) {
        if constexpr(std::is_aggregate_v<value_type> && (sizeof...(Args) != 0u || !std::is_default_constructible_v<value_type>)) {
            const auto it = emplace_element(entt, false, Type{std::forward<Args>(args)...});
            return element_at(static_cast<size_type>(it.index()));
        } else {
            const auto it = emplace_element(entt, false, std::forward<Args>(args)...);
            return element_at(static_cast<size_type>(it.index()));
        }
    }

    /**
     * @brief Updates the instance assigned to a given entity in-place.
     * @tparam Func Types of the function objects to invoke.
     * @param entt A valid identifier.
     * @param func Valid function objects.
     * @return A reference to the updated instance.
     */
    template<typename... Func>
    value_type &patch(const entity_type entt, Func &&...func) {
        const auto idx = base_type::index(entt);
        auto &elem = element_at(idx);
        (std::forward<Func>(func)(elem), ...);
        return elem;
    }

    /**
     * @brief Assigns one or more entities to a storage and constructs their
     * objects from a given instance.
     *
     * @warning
     * Attempting to assign an entity that already belongs to the storage
     * results in undefined behavior.
     *
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     * @param value An instance of the object to construct.
     * @return Iterator pointing to the last element inserted, if any.
     */
    template<typename It>
    iterator insert(It first, It last, const value_type &value = {}) {
        for(; first != last; ++first) {
            emplace_element(*first, true, value);
        }

        return begin();
    }

    /**
     * @brief Assigns one or more entities to a storage and constructs their
     * objects from a given range.
     *
     * @sa construct
     *
     * @tparam EIt Type of input iterator.
     * @tparam CIt Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     * @param from An iterator to the first element of the range of objects.
     * @return Iterator pointing to the first element inserted, if any.
     */
    template<typename EIt, typename CIt, typename = std::enable_if_t<std::is_same_v<typename std::iterator_traits<CIt>::value_type, value_type>>>
    iterator insert(EIt first, EIt last, CIt from) {
        for(; first != last; ++first, ++from) {
            emplace_element(*first, true, *from);
        }

        return begin();
    }

    /**
     * @brief Returns an iterable object to use to _visit_ a storage.
     *
     * The iterable object returns a tuple that contains the current entity and
     * a reference to its component.
     *
     * @return An iterable object to use to _visit_ the storage.
     */
    [[nodiscard]] iterable each() noexcept {
        return {internal::extended_storage_iterator{base_type::begin(), begin()}, internal::extended_storage_iterator{base_type::end(), end()}};
    }

    /*! @copydoc each */
    [[nodiscard]] const_iterable each() const noexcept {
        return {internal::extended_storage_iterator{base_type::cbegin(), cbegin()}, internal::extended_storage_iterator{base_type::cend(), cend()}};
    }

    /**
     * @brief Returns a reverse iterable object to use to _visit_ a storage.
     *
     * @sa each
     *
     * @return A reverse iterable object to use to _visit_ the storage.
     */
    [[nodiscard]] reverse_iterable reach() noexcept {
        return {internal::extended_storage_iterator{base_type::rbegin(), rbegin()}, internal::extended_storage_iterator{base_type::rend(), rend()}};
    }

    /*! @copydoc reach */
    [[nodiscard]] const_reverse_iterable reach() const noexcept {
        return {internal::extended_storage_iterator{base_type::crbegin(), crbegin()}, internal::extended_storage_iterator{base_type::crend(), crend()}};
    }

private:
    container_type payload;
};

/*! @copydoc basic_storage */
template<typename Type, typename Entity, typename Allocator>
class basic_storage<Type, Entity, Allocator, std::enable_if_t<component_traits<Type>::page_size == 0u>>
    : public basic_sparse_set<Entity, typename std::allocator_traits<Allocator>::template rebind_alloc<Entity>> {
    using alloc_traits = std::allocator_traits<Allocator>;
    static_assert(std::is_same_v<typename alloc_traits::value_type, Type>, "Invalid value type");

public:
    /*! @brief Base type. */
    using base_type = basic_sparse_set<Entity, typename alloc_traits::template rebind_alloc<Entity>>;
    /*! @brief Type of the objects assigned to entities. */
    using value_type = Type;
    /*! @brief Component traits. */
    using traits_type = component_traits<value_type>;
    /*! @brief Underlying entity identifier. */
    using entity_type = Entity;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Extended iterable storage proxy. */
    using iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::iterator>>;
    /*! @brief Constant extended iterable storage proxy. */
    using const_iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::const_iterator>>;
    /*! @brief Extended reverse iterable storage proxy. */
    using reverse_iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::reverse_iterator>>;
    /*! @brief Constant extended reverse iterable storage proxy. */
    using const_reverse_iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::const_reverse_iterator>>;

    /*! @brief Default constructor. */
    basic_storage()
        : basic_storage{allocator_type{}} {}

    /**
     * @brief Constructs an empty container with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit basic_storage(const allocator_type &allocator)
        : base_type{type_id<value_type>(), deletion_policy{traits_type::in_place_delete}, allocator} {}

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    basic_storage(basic_storage &&other) noexcept = default;

    /**
     * @brief Allocator-extended move constructor.
     * @param other The instance to move from.
     * @param allocator The allocator to use.
     */
    basic_storage(basic_storage &&other, const allocator_type &allocator) noexcept
        : base_type{std::move(other), allocator} {}

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This storage.
     */
    basic_storage &operator=(basic_storage &&other) noexcept = default;

    /**
     * @brief Returns the associated allocator.
     * @return The associated allocator.
     */
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return allocator_type{base_type::get_allocator()};
    }

    /**
     * @brief Returns the object assigned to an entity, that is `void`.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the storage results in
     * undefined behavior.
     *
     * @param entt A valid identifier.
     */
    void get([[maybe_unused]] const entity_type entt) const noexcept {
        ENTT_ASSERT(base_type::contains(entt), "Storage does not contain entity");
    }

    /**
     * @brief Returns an empty tuple.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the storage results in
     * undefined behavior.
     *
     * @param entt A valid identifier.
     * @return Returns an empty tuple.
     */
    [[nodiscard]] std::tuple<> get_as_tuple([[maybe_unused]] const entity_type entt) const noexcept {
        ENTT_ASSERT(base_type::contains(entt), "Storage does not contain entity");
        return std::tuple{};
    }

    /**
     * @brief Assigns an entity to a storage and constructs its object.
     *
     * @warning
     * Attempting to use an entity that already belongs to the storage results
     * in undefined behavior.
     *
     * @tparam Args Types of arguments to use to construct the object.
     * @param entt A valid identifier.
     */
    template<typename... Args>
    void emplace(const entity_type entt, Args &&...) {
        base_type::try_emplace(entt, false);
    }

    /**
     * @brief Updates the instance assigned to a given entity in-place.
     * @tparam Func Types of the function objects to invoke.
     * @param entt A valid identifier.
     * @param func Valid function objects.
     */
    template<typename... Func>
    void patch([[maybe_unused]] const entity_type entt, Func &&...func) {
        ENTT_ASSERT(base_type::contains(entt), "Storage does not contain entity");
        (std::forward<Func>(func)(), ...);
    }

    /**
     * @brief Assigns entities to a storage.
     * @tparam It Type of input iterator.
     * @tparam Args Types of optional arguments.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     */
    template<typename It, typename... Args>
    void insert(It first, It last, Args &&...) {
        for(; first != last; ++first) {
            base_type::try_emplace(*first, true);
        }
    }

    /**
     * @brief Returns an iterable object to use to _visit_ a storage.
     *
     * The iterable object returns a tuple that contains the current entity.
     *
     * @return An iterable object to use to _visit_ the storage.
     */
    [[nodiscard]] iterable each() noexcept {
        return {internal::extended_storage_iterator{base_type::begin()}, internal::extended_storage_iterator{base_type::end()}};
    }

    /*! @copydoc each */
    [[nodiscard]] const_iterable each() const noexcept {
        return {internal::extended_storage_iterator{base_type::cbegin()}, internal::extended_storage_iterator{base_type::cend()}};
    }

    /**
     * @brief Returns a reverse iterable object to use to _visit_ a storage.
     *
     * @sa each
     *
     * @return A reverse iterable object to use to _visit_ the storage.
     */
    [[nodiscard]] reverse_iterable reach() noexcept {
        return {internal::extended_storage_iterator{base_type::rbegin()}, internal::extended_storage_iterator{base_type::rend()}};
    }

    /*! @copydoc reach */
    [[nodiscard]] const_reverse_iterable reach() const noexcept {
        return {internal::extended_storage_iterator{base_type::crbegin()}, internal::extended_storage_iterator{base_type::crend()}};
    }
};

/**
 * @brief Swap-only entity storage specialization.
 * @tparam Entity A valid entity type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Entity, typename Allocator>
class basic_storage<Entity, Entity, Allocator>
    : public basic_sparse_set<Entity, Allocator> {
    using alloc_traits = std::allocator_traits<Allocator>;
    static_assert(std::is_same_v<typename alloc_traits::value_type, Entity>, "Invalid value type");
    using underlying_type = basic_sparse_set<Entity, typename alloc_traits::template rebind_alloc<Entity>>;
    using underlying_iterator = typename underlying_type::basic_iterator;
    using local_traits_type = entt_traits<Entity>;

    auto entity_at(const std::size_t pos) const noexcept {
        ENTT_ASSERT(pos < local_traits_type::to_entity(null), "Invalid element");
        return local_traits_type::combine(static_cast<typename local_traits_type::entity_type>(pos), {});
    }

private:
    void swap_or_move([[maybe_unused]] const std::size_t lhs, [[maybe_unused]] const std::size_t rhs) override {
        ENTT_ASSERT(((lhs < length) + (rhs < length)) != 1u, "Cross swapping is not supported");
    }

protected:
    /**
     * @brief Erases entities from a storage.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     */
    void pop(underlying_iterator first, underlying_iterator last) override {
        for(; first != last; ++first) {
            if(const auto pos = base_type::index(*first); pos < length) {
                base_type::bump(local_traits_type::next(*first));

                if(pos != --length) {
                    base_type::swap_at(pos, length);
                }
            }
        }
    }

    /*! @brief Erases all entities of a sparse set. */
    void pop_all() override {
        length = 0u;
        base_type::pop_all();
    }

    /**
     * @brief Assigns an entity to a storage.
     * @param hint A valid identifier.
     * @return Iterator pointing to the emplaced element.
     */
    underlying_iterator try_emplace(const Entity hint, const bool, const void *) override {
        return base_type::find(emplace(hint));
    }

public:
    /*! @brief Base type. */
    using base_type = basic_sparse_set<Entity, Allocator>;
    /*! @brief Type of the objects assigned to entities. */
    using value_type = Entity;
    /*! @brief Component traits. */
    using traits_type = component_traits<value_type>;
    /*! @brief Underlying entity identifier. */
    using entity_type = Entity;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Extended iterable storage proxy. */
    using iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::iterator>>;
    /*! @brief Constant extended iterable storage proxy. */
    using const_iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::const_iterator>>;
    /*! @brief Extended reverse iterable storage proxy. */
    using reverse_iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::reverse_iterator>>;
    /*! @brief Constant extended reverse iterable storage proxy. */
    using const_reverse_iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::const_reverse_iterator>>;

    /*! @brief Default constructor. */
    basic_storage()
        : basic_storage{allocator_type{}} {
    }

    /**
     * @brief Constructs an empty container with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit basic_storage(const allocator_type &allocator)
        : base_type{type_id<value_type>(), deletion_policy::swap_and_pop, allocator},
          length{} {}

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    basic_storage(basic_storage &&other) noexcept
        : base_type{std::move(other)},
          length{std::exchange(other.length, size_type{})} {}

    /**
     * @brief Allocator-extended move constructor.
     * @param other The instance to move from.
     * @param allocator The allocator to use.
     */
    basic_storage(basic_storage &&other, const allocator_type &allocator) noexcept
        : base_type{std::move(other), allocator},
          length{std::exchange(other.length, size_type{})} {}

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This storage.
     */
    basic_storage &operator=(basic_storage &&other) noexcept {
        base_type::operator=(std::move(other));
        length = std::exchange(other.length, size_type{});
        return *this;
    }

    /**
     * @brief Returns the object assigned to an entity, that is `void`.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the storage results in
     * undefined behavior.
     *
     * @param entt A valid identifier.
     */
    void get([[maybe_unused]] const entity_type entt) const noexcept {
        ENTT_ASSERT(base_type::index(entt) < length, "The requested entity is not a live one");
    }

    /**
     * @brief Returns an empty tuple.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the storage results in
     * undefined behavior.
     *
     * @param entt A valid identifier.
     * @return Returns an empty tuple.
     */
    [[nodiscard]] std::tuple<> get_as_tuple([[maybe_unused]] const entity_type entt) const noexcept {
        ENTT_ASSERT(base_type::index(entt) < length, "The requested entity is not a live one");
        return std::tuple{};
    }

    /**
     * @brief Exchanges the contents with those of a given storage.
     * @param other Storage to exchange the content with.
     */
    void swap(basic_storage &other) {
        using std::swap;
        base_type::swap(other);
        swap(length, other.length);
    }

    /**
     * @brief Creates a new identifier or recycles a destroyed one.
     * @return A valid identifier.
     */
    entity_type emplace() {
        if(length == base_type::size()) {
            return *base_type::try_emplace(entity_at(length++), true);
        }

        return base_type::operator[](length++);
    }

    /**
     * @brief Creates a new identifier or recycles a destroyed one.
     *
     * If the requested identifier isn't in use, the suggested one is used.
     * Otherwise, a new identifier is returned.
     *
     * @param hint Required identifier.
     * @return A valid identifier.
     */
    entity_type emplace(const entity_type hint) {
        if(hint == null || hint == tombstone) {
            return emplace();
        } else if(const auto curr = local_traits_type::construct(local_traits_type::to_entity(hint), base_type::current(hint)); curr == tombstone) {
            const auto pos = static_cast<size_type>(local_traits_type::to_entity(hint));

            while(!(pos < base_type::size())) {
                base_type::try_emplace(entity_at(base_type::size()), true);
            }

            base_type::swap_at(pos, length++);
        } else if(const auto idx = base_type::index(curr); idx < length) {
            return emplace();
        } else {
            base_type::swap_at(idx, length++);
        }

        base_type::bump(hint);

        return hint;
    }

    /**
     * @brief Updates a given identifier.
     * @tparam Func Types of the function objects to invoke.
     * @param entt A valid identifier.
     * @param func Valid function objects.
     */
    template<typename... Func>
    void patch([[maybe_unused]] const entity_type entt, Func &&...func) {
        ENTT_ASSERT(base_type::contains(entt), "Storage does not contain entity");
        (std::forward<Func>(func)(), ...);
    }

    /**
     * @brief Assigns each element in a range an identifier.
     * @tparam It Type of mutable forward iterator.
     * @param first An iterator to the first element of the range to generate.
     * @param last An iterator past the last element of the range to generate.
     */
    template<typename It>
    void insert(It first, It last) {
        for(const auto sz = base_type::size(); first != last && length != sz; ++first, ++length) {
            *first = base_type::operator[](length);
        }

        for(; first != last; ++first) {
            *first = *base_type::try_emplace(entity_at(length++), true);
        }
    }

    /**
     * @brief Makes all elements in a range contiguous.
     * @tparam It Type of forward iterator.
     * @param first An iterator to the first element of the range to pack.
     * @param last An iterator past the last element of the range to pack.
     * @return The number of elements within the newly created range.
     */
    template<typename It>
    size_type pack(It first, It last) {
        size_type len = length;

        for(; first != last; ++first, --len) {
            const auto pos = base_type::index(*first);
            ENTT_ASSERT(pos < length, "Invalid element");
            base_type::swap_at(pos, static_cast<size_type>(len - 1u));
        }

        return (length - len);
    }

    /**
     * @brief Returns the number of elements considered still in use.
     * @return The number of elements considered still in use.
     */
    [[nodiscard]] size_type in_use() const noexcept {
        return length;
    }

    /**
     * @brief Sets the number of elements considered still in use.
     * @param len The number of elements considered still in use.
     */
    void in_use(const size_type len) noexcept {
        ENTT_ASSERT(!(len > base_type::size()), "Invalid length");
        length = len;
    }

    /**
     * @brief Returns an iterable object to use to _visit_ a storage.
     *
     * The iterable object returns a tuple that contains the current entity.
     *
     * @return An iterable object to use to _visit_ the storage.
     */
    [[nodiscard]] iterable each() noexcept {
        return {internal::extended_storage_iterator{base_type::end() - length}, internal::extended_storage_iterator{base_type::end()}};
    }

    /*! @copydoc each */
    [[nodiscard]] const_iterable each() const noexcept {
        return {internal::extended_storage_iterator{base_type::cend() - length}, internal::extended_storage_iterator{base_type::cend()}};
    }

    /**
     * @brief Returns a reverse iterable object to use to _visit_ a storage.
     *
     * @sa each
     *
     * @return A reverse iterable object to use to _visit_ the storage.
     */
    [[nodiscard]] reverse_iterable reach() noexcept {
        return {internal::extended_storage_iterator{base_type::rbegin()}, internal::extended_storage_iterator{base_type::rbegin() + length}};
    }

    /*! @copydoc reach */
    [[nodiscard]] const_reverse_iterable reach() const noexcept {
        return {internal::extended_storage_iterator{base_type::crbegin()}, internal::extended_storage_iterator{base_type::crbegin() + length}};
    }

private:
    size_type length;
};

} // namespace entt

#endif

/////////////////// END OF external/entt/entity/storage.hpp ////////////////////

/////////////////// START OF external/entt/entity/group.hpp ////////////////////

#ifndef ENTT_ENTITY_GROUP_HPP
#define ENTT_ENTITY_GROUP_HPP

#include <tuple>
#include <type_traits>
#include <utility>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename, typename, typename>
class extended_group_iterator;

template<typename It, typename... Owned, typename... Get>
class extended_group_iterator<It, owned_t<Owned...>, get_t<Get...>> {
    template<typename Type>
    auto index_to_element([[maybe_unused]] Type &cpool) const {
        if constexpr(Type::traits_type::page_size == 0u) {
            return std::make_tuple();
        } else {
            return std::forward_as_tuple(cpool.rbegin()[it.index()]);
        }
    }

public:
    using iterator_type = It;
    using difference_type = std::ptrdiff_t;
    using value_type = decltype(std::tuple_cat(std::make_tuple(*std::declval<It>()), std::declval<Owned>().get_as_tuple({})..., std::declval<Get>().get_as_tuple({})...));
    using pointer = input_iterator_pointer<value_type>;
    using reference = value_type;
    using iterator_category = std::input_iterator_tag;

    constexpr extended_group_iterator()
        : it{},
          pools{} {}

    extended_group_iterator(It from, const std::tuple<Owned *..., Get *...> &cpools)
        : it{from},
          pools{cpools} {}

    extended_group_iterator &operator++() noexcept {
        return ++it, *this;
    }

    extended_group_iterator operator++(int) noexcept {
        extended_group_iterator orig = *this;
        return ++(*this), orig;
    }

    [[nodiscard]] reference operator*() const noexcept {
        return std::tuple_cat(std::make_tuple(*it), index_to_element(*std::get<Owned *>(pools))..., std::get<Get *>(pools)->get_as_tuple(*it)...);
    }

    [[nodiscard]] pointer operator->() const noexcept {
        return operator*();
    }

    [[nodiscard]] constexpr iterator_type base() const noexcept {
        return it;
    }

    template<typename... Lhs, typename... Rhs>
    friend constexpr bool operator==(const extended_group_iterator<Lhs...> &, const extended_group_iterator<Rhs...> &) noexcept;

private:
    It it;
    std::tuple<Owned *..., Get *...> pools;
};

template<typename... Lhs, typename... Rhs>
[[nodiscard]] constexpr bool operator==(const extended_group_iterator<Lhs...> &lhs, const extended_group_iterator<Rhs...> &rhs) noexcept {
    return lhs.it == rhs.it;
}

template<typename... Lhs, typename... Rhs>
[[nodiscard]] constexpr bool operator!=(const extended_group_iterator<Lhs...> &lhs, const extended_group_iterator<Rhs...> &rhs) noexcept {
    return !(lhs == rhs);
}

struct group_descriptor {
    using size_type = std::size_t;
    virtual ~group_descriptor() = default;
    virtual size_type owned(const id_type *, const size_type) const noexcept {
        return 0u;
    }
};

template<typename, typename, typename>
class group_handler;

template<typename... Owned, typename... Get, typename... Exclude>
class group_handler<owned_t<Owned...>, get_t<Get...>, exclude_t<Exclude...>> final: public group_descriptor {
    // nasty workaround for an issue with the toolset v141 that doesn't accept a fold expression here
    static_assert(!std::disjunction_v<std::bool_constant<Owned::traits_type::in_place_delete>...>, "Groups do not support in-place delete");
    static_assert(!std::disjunction_v<std::is_const<Owned>..., std::is_const<Get>..., std::is_const<Exclude>...>, "Const storage type not allowed");

    using base_type = std::common_type_t<typename Owned::base_type..., typename Get::base_type..., typename Exclude::base_type...>;
    using entity_type = typename base_type::entity_type;

    void swap_elements(const std::size_t pos, const entity_type entt) {
        std::apply([pos, entt](auto *...cpool) { (cpool->swap_elements(cpool->data()[pos], entt), ...); }, pools);
    }

    void push_on_construct(const entity_type entt) {
        if(std::apply([entt, len = len](auto *cpool, auto *...other) { return cpool->contains(entt) && !(cpool->index(entt) < len) && (other->contains(entt) && ...); }, pools)
           && std::apply([entt](auto *...cpool) { return (!cpool->contains(entt) && ...); }, filter)) {
            swap_elements(len++, entt);
        }
    }

    void push_on_destroy(const entity_type entt) {
        if(std::apply([entt, len = len](auto *cpool, auto *...other) { return cpool->contains(entt) && !(cpool->index(entt) < len) && (other->contains(entt) && ...); }, pools)
           && std::apply([entt](auto *...cpool) { return (0u + ... + cpool->contains(entt)) == 1u; }, filter)) {
            swap_elements(len++, entt);
        }
    }

    void remove_if(const entity_type entt) {
        if(std::get<0>(pools)->contains(entt) && (std::get<0>(pools)->index(entt) < len)) {
            swap_elements(--len, entt);
        }
    }

public:
    using size_type = typename base_type::size_type;

    group_handler(Owned &...opool, Get &...gpool, Exclude &...epool)
        : pools{&opool..., &gpool...},
          filter{&epool...},
          len{} {
        std::apply([this](auto *...cpool) { ((cpool->on_construct().template connect<&group_handler::push_on_construct>(*this), cpool->on_destroy().template connect<&group_handler::remove_if>(*this)), ...); }, pools);
        std::apply([this](auto *...cpool) { ((cpool->on_construct().template connect<&group_handler::remove_if>(*this), cpool->on_destroy().template connect<&group_handler::push_on_destroy>(*this)), ...); }, filter);

        // we cannot iterate backwards because we want to leave behind valid entities in case of owned types
        for(auto *first = std::get<0>(pools)->data(), *last = first + std::get<0>(pools)->size(); first != last; ++first) {
            push_on_construct(*first);
        }
    }

    size_type owned(const id_type *elem, const size_type length) const noexcept final {
        size_type cnt = 0u;

        for(auto pos = 0u; pos < length; ++pos) {
            cnt += ((elem[pos] == entt::type_hash<typename Owned::value_type>::value()) || ...);
        }

        return cnt;
    }

    [[nodiscard]] size_type length() const noexcept {
        return len;
    }

    template<typename Type>
    Type pools_as() const noexcept {
        return pools;
    }

    template<typename Type>
    Type filter_as() const noexcept {
        return filter;
    }

private:
    std::tuple<Owned *..., Get *...> pools;
    std::tuple<Exclude *...> filter;
    std::size_t len;
};

template<typename... Get, typename... Exclude>
class group_handler<owned_t<>, get_t<Get...>, exclude_t<Exclude...>> final: public group_descriptor {
    // nasty workaround for an issue with the toolset v141 that doesn't accept a fold expression here
    static_assert(!std::disjunction_v<std::is_const<Get>..., std::is_const<Exclude>...>, "Const storage type not allowed");

    using base_type = std::common_type_t<typename Get::base_type..., typename Exclude::base_type...>;
    using entity_type = typename base_type::entity_type;

    void push_on_construct(const entity_type entt) {
        if(!elem.contains(entt)
           && std::apply([entt](auto *...cpool) { return (cpool->contains(entt) && ...); }, pools)
           && std::apply([entt](auto *...cpool) { return (!cpool->contains(entt) && ...); }, filter)) {
            elem.push(entt);
        }
    }

    void push_on_destroy(const entity_type entt) {
        if(!elem.contains(entt)
           && std::apply([entt](auto *...cpool) { return (cpool->contains(entt) && ...); }, pools)
           && std::apply([entt](auto *...cpool) { return (0u + ... + cpool->contains(entt)) == 1u; }, filter)) {
            elem.push(entt);
        }
    }

    void remove_if(const entity_type entt) {
        elem.remove(entt);
    }

public:
    using common_type = base_type;

    template<typename Alloc>
    group_handler(const Alloc &alloc, Get &...gpool, Exclude &...epool)
        : pools{&gpool...},
          filter{&epool...},
          elem{alloc} {
        std::apply([this](auto *...cpool) { ((cpool->on_construct().template connect<&group_handler::push_on_construct>(*this), cpool->on_destroy().template connect<&group_handler::remove_if>(*this)), ...); }, pools);
        std::apply([this](auto *...cpool) { ((cpool->on_construct().template connect<&group_handler::remove_if>(*this), cpool->on_destroy().template connect<&group_handler::push_on_destroy>(*this)), ...); }, filter);

        for(const auto entity: static_cast<base_type &>(*std::get<0>(pools))) {
            push_on_construct(entity);
        }
    }

    common_type &handle() noexcept {
        return elem;
    }

    const common_type &handle() const noexcept {
        return elem;
    }

    template<typename Type>
    Type pools_as() const noexcept {
        return pools;
    }

    template<typename Type>
    Type filter_as() const noexcept {
        return filter;
    }

private:
    std::tuple<Get *...> pools;
    std::tuple<Exclude *...> filter;
    base_type elem;
};

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Group.
 *
 * Primary template isn't defined on purpose. All the specializations give a
 * compile-time error, but for a few reasonable cases.
 */
template<typename, typename, typename>
class basic_group;

/**
 * @brief Non-owning group.
 *
 * A non-owning group returns all entities and only the entities that are at
 * least in the given storage. Moreover, it's guaranteed that the entity list is
 * tightly packed in memory for fast iterations.
 *
 * @b Important
 *
 * Iterators aren't invalidated if:
 *
 * * New elements are added to the storage.
 * * The entity currently pointed is modified (for example, components are added
 *   or removed from it).
 * * The entity currently pointed is destroyed.
 *
 * In all other cases, modifying the pools iterated by the group in any way
 * invalidates all the iterators.
 *
 * @tparam Get Types of storage _observed_ by the group.
 * @tparam Exclude Types of storage used to filter the group.
 */
template<typename... Get, typename... Exclude>
class basic_group<owned_t<>, get_t<Get...>, exclude_t<Exclude...>> {
    using base_type = std::common_type_t<typename Get::base_type..., typename Exclude::base_type...>;
    using underlying_type = typename base_type::entity_type;

    template<typename Type>
    static constexpr std::size_t index_of = type_list_index_v<std::remove_const_t<Type>, type_list<typename Get::value_type..., typename Exclude::value_type...>>;

    auto pools() const noexcept {
        using return_type = std::tuple<Get *...>;
        return descriptor ? descriptor->template pools_as<return_type>() : return_type{};
    }

    auto filter() const noexcept {
        using return_type = std::tuple<Exclude *...>;
        return descriptor ? descriptor->template filter_as<return_type>() : return_type{};
    }

public:
    /*! @brief Underlying entity identifier. */
    using entity_type = underlying_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Common type among all storage types. */
    using common_type = base_type;
    /*! @brief Random access iterator type. */
    using iterator = typename common_type::iterator;
    /*! @brief Reversed iterator type. */
    using reverse_iterator = typename common_type::reverse_iterator;
    /*! @brief Iterable group type. */
    using iterable = iterable_adaptor<internal::extended_group_iterator<iterator, owned_t<>, get_t<Get...>>>;
    /*! @brief Group handler type. */
    using handler = internal::group_handler<owned_t<>, get_t<std::remove_const_t<Get>...>, exclude_t<std::remove_const_t<Exclude>...>>;

    /*! @brief Default constructor to use to create empty, invalid groups. */
    basic_group() noexcept
        : descriptor{} {}

    /**
     * @brief Constructs a group from a set of storage classes.
     * @param ref A reference to a group handler.
     */
    basic_group(handler &ref) noexcept
        : descriptor{&ref} {}

    /**
     * @brief Returns the leading storage of a group.
     * @return The leading storage of the group.
     */
    [[nodiscard]] const common_type &handle() const noexcept {
        return descriptor->handle();
    }

    /**
     * @brief Returns the storage for a given component type, if any.
     * @tparam Type Type of component of which to return the storage.
     * @return The storage for the given component type.
     */
    template<typename Type>
    [[nodiscard]] auto *storage() const noexcept {
        return storage<index_of<Type>>();
    }

    /**
     * @brief Returns the storage for a given index, if any.
     * @tparam Index Index of the storage to return.
     * @return The storage for the given index.
     */
    template<std::size_t Index>
    [[nodiscard]] auto *storage() const noexcept {
        constexpr auto offset = sizeof...(Get);

        if constexpr(Index < offset) {
            return std::get<Index>(pools());
        } else {
            return std::get<Index - offset>(filter());
        }
    }

    /**
     * @brief Returns the number of entities that are part of the group.
     * @return Number of entities that are part of the group.
     */
    [[nodiscard]] size_type size() const noexcept {
        return *this ? handle().size() : size_type{};
    }

    /**
     * @brief Returns the number of elements that a group has currently
     * allocated space for.
     * @return Capacity of the group.
     */
    [[nodiscard]] size_type capacity() const noexcept {
        return *this ? handle().capacity() : size_type{};
    }

    /*! @brief Requests the removal of unused capacity. */
    void shrink_to_fit() {
        if(*this) {
            descriptor->handle().shrink_to_fit();
        }
    }

    /**
     * @brief Checks whether a group is empty.
     * @return True if the group is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return !*this || handle().empty();
    }

    /**
     * @brief Returns an iterator to the first entity of the group.
     *
     * If the group is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first entity of the group.
     */
    [[nodiscard]] iterator begin() const noexcept {
        return *this ? handle().begin() : iterator{};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the group.
     * @return An iterator to the entity following the last entity of the
     * group.
     */
    [[nodiscard]] iterator end() const noexcept {
        return *this ? handle().end() : iterator{};
    }

    /**
     * @brief Returns an iterator to the first entity of the reversed group.
     *
     * If the group is empty, the returned iterator will be equal to `rend()`.
     *
     * @return An iterator to the first entity of the reversed group.
     */
    [[nodiscard]] reverse_iterator rbegin() const noexcept {
        return *this ? handle().rbegin() : reverse_iterator{};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the reversed
     * group.
     * @return An iterator to the entity following the last entity of the
     * reversed group.
     */
    [[nodiscard]] reverse_iterator rend() const noexcept {
        return *this ? handle().rend() : reverse_iterator{};
    }

    /**
     * @brief Returns the first entity of the group, if any.
     * @return The first entity of the group if one exists, the null entity
     * otherwise.
     */
    [[nodiscard]] entity_type front() const noexcept {
        const auto it = begin();
        return it != end() ? *it : null;
    }

    /**
     * @brief Returns the last entity of the group, if any.
     * @return The last entity of the group if one exists, the null entity
     * otherwise.
     */
    [[nodiscard]] entity_type back() const noexcept {
        const auto it = rbegin();
        return it != rend() ? *it : null;
    }

    /**
     * @brief Finds an entity.
     * @param entt A valid identifier.
     * @return An iterator to the given entity if it's found, past the end
     * iterator otherwise.
     */
    [[nodiscard]] iterator find(const entity_type entt) const noexcept {
        return *this ? handle().find(entt) : iterator{};
    }

    /**
     * @brief Returns the identifier that occupies the given position.
     * @param pos Position of the element to return.
     * @return The identifier that occupies the given position.
     */
    [[nodiscard]] entity_type operator[](const size_type pos) const {
        return begin()[pos];
    }

    /**
     * @brief Checks if a group is properly initialized.
     * @return True if the group is properly initialized, false otherwise.
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return descriptor != nullptr;
    }

    /**
     * @brief Checks if a group contains an entity.
     * @param entt A valid identifier.
     * @return True if the group contains the given entity, false otherwise.
     */
    [[nodiscard]] bool contains(const entity_type entt) const noexcept {
        return *this && handle().contains(entt);
    }

    /**
     * @brief Returns the components assigned to the given entity.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the group results in
     * undefined behavior.
     *
     * @tparam Type Type of the component to get.
     * @tparam Other Other types of components to get.
     * @param entt A valid identifier.
     * @return The components assigned to the entity.
     */
    template<typename Type, typename... Other>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        return get<index_of<Type>, index_of<Other>...>(entt);
    }

    /**
     * @brief Returns the components assigned to the given entity.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the groups results in
     * undefined behavior.
     *
     * @tparam Index Indexes of the components to get.
     * @param entt A valid identifier.
     * @return The components assigned to the entity.
     */
    template<std::size_t... Index>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        const auto cpools = pools();

        if constexpr(sizeof...(Index) == 0) {
            return std::apply([entt](auto *...curr) { return std::tuple_cat(curr->get_as_tuple(entt)...); }, cpools);
        } else if constexpr(sizeof...(Index) == 1) {
            return (std::get<Index>(cpools)->get(entt), ...);
        } else {
            return std::tuple_cat(std::get<Index>(cpools)->get_as_tuple(entt)...);
        }
    }

    /**
     * @brief Iterates entities and components and applies the given function
     * object to them.
     *
     * The function object is invoked for each entity. It is provided with the
     * entity itself and a set of references to non-empty components. The
     * _constness_ of the components is as requested.<br/>
     * The signature of the function must be equivalent to one of the following
     * forms:
     *
     * @code{.cpp}
     * void(const entity_type, Type &...);
     * void(Type &...);
     * @endcode
     *
     * @note
     * Empty types aren't explicitly instantiated and therefore they are never
     * returned during iterations.
     *
     * @tparam Func Type of the function object to invoke.
     * @param func A valid function object.
     */
    template<typename Func>
    void each(Func func) const {
        for(const auto entt: *this) {
            if constexpr(is_applicable_v<Func, decltype(std::tuple_cat(std::tuple<entity_type>{}, std::declval<basic_group>().get({})))>) {
                std::apply(func, std::tuple_cat(std::make_tuple(entt), get(entt)));
            } else {
                std::apply(func, get(entt));
            }
        }
    }

    /**
     * @brief Returns an iterable object to use to _visit_ a group.
     *
     * The iterable object returns tuples that contain the current entity and a
     * set of references to its non-empty components. The _constness_ of the
     * components is as requested.
     *
     * @note
     * Empty types aren't explicitly instantiated and therefore they are never
     * returned during iterations.
     *
     * @return An iterable object to use to _visit_ the group.
     */
    [[nodiscard]] iterable each() const noexcept {
        const auto cpools = pools();
        return iterable{{begin(), cpools}, {end(), cpools}};
    }

    /**
     * @brief Sort a group according to the given comparison function.
     *
     * The comparison function object must return `true` if the first element
     * is _less_ than the second one, `false` otherwise. The signature of the
     * comparison function should be equivalent to one of the following:
     *
     * @code{.cpp}
     * bool(std::tuple<Type &...>, std::tuple<Type &...>);
     * bool(const Type &..., const Type &...);
     * bool(const Entity, const Entity);
     * @endcode
     *
     * Where `Type` are such that they are iterated by the group.<br/>
     * Moreover, the comparison function object shall induce a
     * _strict weak ordering_ on the values.
     *
     * The sort function object must offer a member function template
     * `operator()` that accepts three arguments:
     *
     * * An iterator to the first element of the range to sort.
     * * An iterator past the last element of the range to sort.
     * * A comparison function to use to compare the elements.
     *
     * @tparam Type Optional type of component to compare.
     * @tparam Other Other optional types of components to compare.
     * @tparam Compare Type of comparison function object.
     * @tparam Sort Type of sort function object.
     * @tparam Args Types of arguments to forward to the sort function object.
     * @param compare A valid comparison function object.
     * @param algo A valid sort function object.
     * @param args Arguments to forward to the sort function object, if any.
     */
    template<typename Type, typename... Other, typename Compare, typename Sort = std_sort, typename... Args>
    void sort(Compare compare, Sort algo = Sort{}, Args &&...args) {
        sort<index_of<Type>, index_of<Other>...>(std::move(compare), std::move(algo), std::forward<Args>(args)...);
    }

    /**
     * @brief Sort a group according to the given comparison function.
     *
     * @sa sort
     *
     * @tparam Index Optional indexes of components to compare.
     * @tparam Compare Type of comparison function object.
     * @tparam Sort Type of sort function object.
     * @tparam Args Types of arguments to forward to the sort function object.
     * @param compare A valid comparison function object.
     * @param algo A valid sort function object.
     * @param args Arguments to forward to the sort function object, if any.
     */
    template<std::size_t... Index, typename Compare, typename Sort = std_sort, typename... Args>
    void sort(Compare compare, Sort algo = Sort{}, Args &&...args) {
        if(*this) {
            if constexpr(sizeof...(Index) == 0) {
                static_assert(std::is_invocable_v<Compare, const entity_type, const entity_type>, "Invalid comparison function");
                descriptor->handle().sort(std::move(compare), std::move(algo), std::forward<Args>(args)...);
            } else {
                auto comp = [&compare, cpools = pools()](const entity_type lhs, const entity_type rhs) {
                    if constexpr(sizeof...(Index) == 1) {
                        return compare((std::get<Index>(cpools)->get(lhs), ...), (std::get<Index>(cpools)->get(rhs), ...));
                    } else {
                        return compare(std::forward_as_tuple(std::get<Index>(cpools)->get(lhs)...), std::forward_as_tuple(std::get<Index>(cpools)->get(rhs)...));
                    }
                };

                descriptor->handle().sort(std::move(comp), std::move(algo), std::forward<Args>(args)...);
            }
        }
    }

    /**
     * @brief Sort the shared pool of entities according to a given storage.
     *
     * The shared pool of entities and thus its order is affected by the changes
     * to each and every pool that it tracks.
     *
     * @param other The storage to use to impose the order.
     */
    void sort_as(const common_type &other) const {
        if(*this) {
            descriptor->handle().sort_as(other);
        }
    }

private:
    handler *descriptor;
};

/**
 * @brief Owning group.
 *
 * Owning groups returns all entities and only the entities that are at
 * least in the given storage. Moreover:
 *
 * * It's guaranteed that the entity list is tightly packed in memory for fast
 *   iterations.
 * * It's guaranteed that all components in the owned storage are tightly packed
 *   in memory for even faster iterations and to allow direct access.
 * * They stay true to the order of the owned storage and all instances have the
 *   same order in memory.
 *
 * The more types of storage are owned, the faster it is to iterate a group.
 *
 * @b Important
 *
 * Iterators aren't invalidated if:
 *
 * * New elements are added to the storage.
 * * The entity currently pointed is modified (for example, components are added
 *   or removed from it).
 * * The entity currently pointed is destroyed.
 *
 * In all other cases, modifying the pools iterated by the group in any way
 * invalidates all the iterators.
 *
 * @tparam Owned Types of storage _owned_ by the group.
 * @tparam Get Types of storage _observed_ by the group.
 * @tparam Exclude Types of storage used to filter the group.
 */
template<typename... Owned, typename... Get, typename... Exclude>
class basic_group<owned_t<Owned...>, get_t<Get...>, exclude_t<Exclude...>> {
    using base_type = std::common_type_t<typename Owned::base_type..., typename Get::base_type..., typename Exclude::base_type...>;
    using underlying_type = typename base_type::entity_type;

    template<typename Type>
    static constexpr std::size_t index_of = type_list_index_v<std::remove_const_t<Type>, type_list<typename Owned::value_type..., typename Get::value_type..., typename Exclude::value_type...>>;

    auto pools() const noexcept {
        using return_type = std::tuple<Owned *..., Get *...>;
        return descriptor ? descriptor->template pools_as<return_type>() : return_type{};
    }

    auto filter() const noexcept {
        using return_type = std::tuple<Exclude *...>;
        return descriptor ? descriptor->template filter_as<return_type>() : return_type{};
    }

public:
    /*! @brief Underlying entity identifier. */
    using entity_type = underlying_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Common type among all storage types. */
    using common_type = base_type;
    /*! @brief Random access iterator type. */
    using iterator = typename common_type::iterator;
    /*! @brief Reversed iterator type. */
    using reverse_iterator = typename common_type::reverse_iterator;
    /*! @brief Iterable group type. */
    using iterable = iterable_adaptor<internal::extended_group_iterator<iterator, owned_t<Owned...>, get_t<Get...>>>;
    /*! @brief Group handler type. */
    using handler = internal::group_handler<owned_t<std::remove_const_t<Owned>...>, get_t<std::remove_const_t<Get>...>, exclude_t<std::remove_const_t<Exclude>...>>;

    /*! @brief Default constructor to use to create empty, invalid groups. */
    basic_group() noexcept
        : descriptor{} {}

    /**
     * @brief Constructs a group from a set of storage classes.
     * @param ref A reference to a group handler.
     */
    basic_group(handler &ref) noexcept
        : descriptor{&ref} {}

    /**
     * @brief Returns the leading storage of a group.
     * @return The leading storage of the group.
     */
    [[nodiscard]] const common_type &handle() const noexcept {
        return *storage<0>();
    }

    /**
     * @brief Returns the storage for a given component type, if any.
     * @tparam Type Type of component of which to return the storage.
     * @return The storage for the given component type.
     */
    template<typename Type>
    [[nodiscard]] auto *storage() const noexcept {
        return storage<index_of<Type>>();
    }

    /**
     * @brief Returns the storage for a given index, if any.
     * @tparam Index Index of the storage to return.
     * @return The storage for the given index.
     */
    template<std::size_t Index>
    [[nodiscard]] auto *storage() const noexcept {
        constexpr auto offset = sizeof...(Owned) + sizeof...(Get);

        if constexpr(Index < offset) {
            return std::get<Index>(pools());
        } else {
            return std::get<Index - offset>(filter());
        }
    }

    /**
     * @brief Returns the number of entities that that are part of the group.
     * @return Number of entities that that are part of the group.
     */
    [[nodiscard]] size_type size() const noexcept {
        return *this ? descriptor->length() : size_type{};
    }

    /**
     * @brief Checks whether a group is empty.
     * @return True if the group is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return !*this || !descriptor->length();
    }

    /**
     * @brief Returns an iterator to the first entity of the group.
     *
     * If the group is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first entity of the group.
     */
    [[nodiscard]] iterator begin() const noexcept {
        return *this ? (handle().end() - descriptor->length()) : iterator{};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the group.
     * @return An iterator to the entity following the last entity of the
     * group.
     */
    [[nodiscard]] iterator end() const noexcept {
        return *this ? handle().end() : iterator{};
    }

    /**
     * @brief Returns an iterator to the first entity of the reversed group.
     *
     * If the group is empty, the returned iterator will be equal to `rend()`.
     *
     * @return An iterator to the first entity of the reversed group.
     */
    [[nodiscard]] reverse_iterator rbegin() const noexcept {
        return *this ? handle().rbegin() : reverse_iterator{};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the reversed
     * group.
     * @return An iterator to the entity following the last entity of the
     * reversed group.
     */
    [[nodiscard]] reverse_iterator rend() const noexcept {
        return *this ? (handle().rbegin() + descriptor->length()) : reverse_iterator{};
    }

    /**
     * @brief Returns the first entity of the group, if any.
     * @return The first entity of the group if one exists, the null entity
     * otherwise.
     */
    [[nodiscard]] entity_type front() const noexcept {
        const auto it = begin();
        return it != end() ? *it : null;
    }

    /**
     * @brief Returns the last entity of the group, if any.
     * @return The last entity of the group if one exists, the null entity
     * otherwise.
     */
    [[nodiscard]] entity_type back() const noexcept {
        const auto it = rbegin();
        return it != rend() ? *it : null;
    }

    /**
     * @brief Finds an entity.
     * @param entt A valid identifier.
     * @return An iterator to the given entity if it's found, past the end
     * iterator otherwise.
     */
    [[nodiscard]] iterator find(const entity_type entt) const noexcept {
        const auto it = *this ? handle().find(entt) : iterator{};
        return it >= begin() ? it : iterator{};
    }

    /**
     * @brief Returns the identifier that occupies the given position.
     * @param pos Position of the element to return.
     * @return The identifier that occupies the given position.
     */
    [[nodiscard]] entity_type operator[](const size_type pos) const {
        return begin()[pos];
    }

    /**
     * @brief Checks if a group is properly initialized.
     * @return True if the group is properly initialized, false otherwise.
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return descriptor != nullptr;
    }

    /**
     * @brief Checks if a group contains an entity.
     * @param entt A valid identifier.
     * @return True if the group contains the given entity, false otherwise.
     */
    [[nodiscard]] bool contains(const entity_type entt) const noexcept {
        return *this && handle().contains(entt) && (handle().index(entt) < (descriptor->length()));
    }

    /**
     * @brief Returns the components assigned to the given entity.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the group results in
     * undefined behavior.
     *
     * @tparam Type Type of the component to get.
     * @tparam Other Other types of components to get.
     * @param entt A valid identifier.
     * @return The components assigned to the entity.
     */
    template<typename Type, typename... Other>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        return get<index_of<Type>, index_of<Other>...>(entt);
    }

    /**
     * @brief Returns the components assigned to the given entity.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the groups results in
     * undefined behavior.
     *
     * @tparam Index Indexes of the components to get.
     * @param entt A valid identifier.
     * @return The components assigned to the entity.
     */
    template<std::size_t... Index>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        const auto cpools = pools();

        if constexpr(sizeof...(Index) == 0) {
            return std::apply([entt](auto *...curr) { return std::tuple_cat(curr->get_as_tuple(entt)...); }, cpools);
        } else if constexpr(sizeof...(Index) == 1) {
            return (std::get<Index>(cpools)->get(entt), ...);
        } else {
            return std::tuple_cat(std::get<Index>(cpools)->get_as_tuple(entt)...);
        }
    }

    /**
     * @brief Iterates entities and components and applies the given function
     * object to them.
     *
     * The function object is invoked for each entity. It is provided with the
     * entity itself and a set of references to non-empty components. The
     * _constness_ of the components is as requested.<br/>
     * The signature of the function must be equivalent to one of the following
     * forms:
     *
     * @code{.cpp}
     * void(const entity_type, Type &...);
     * void(Type &...);
     * @endcode
     *
     * @note
     * Empty types aren't explicitly instantiated and therefore they are never
     * returned during iterations.
     *
     * @tparam Func Type of the function object to invoke.
     * @param func A valid function object.
     */
    template<typename Func>
    void each(Func func) const {
        for(auto args: each()) {
            if constexpr(is_applicable_v<Func, decltype(std::tuple_cat(std::tuple<entity_type>{}, std::declval<basic_group>().get({})))>) {
                std::apply(func, args);
            } else {
                std::apply([&func](auto, auto &&...less) { func(std::forward<decltype(less)>(less)...); }, args);
            }
        }
    }

    /**
     * @brief Returns an iterable object to use to _visit_ a group.
     *
     * The iterable object returns tuples that contain the current entity and a
     * set of references to its non-empty components. The _constness_ of the
     * components is as requested.
     *
     * @note
     * Empty types aren't explicitly instantiated and therefore they are never
     * returned during iterations.
     *
     * @return An iterable object to use to _visit_ the group.
     */
    [[nodiscard]] iterable each() const noexcept {
        const auto cpools = pools();
        return {{begin(), cpools}, {end(), cpools}};
    }

    /**
     * @brief Sort a group according to the given comparison function.
     *
     * The comparison function object must return `true` if the first element
     * is _less_ than the second one, `false` otherwise. The signature of the
     * comparison function should be equivalent to one of the following:
     *
     * @code{.cpp}
     * bool(std::tuple<Type &...>, std::tuple<Type &...>);
     * bool(const Type &, const Type &);
     * bool(const Entity, const Entity);
     * @endcode
     *
     * Where `Type` are either owned types or not but still such that they are
     * iterated by the group.<br/>
     * Moreover, the comparison function object shall induce a
     * _strict weak ordering_ on the values.
     *
     * The sort function object must offer a member function template
     * `operator()` that accepts three arguments:
     *
     * * An iterator to the first element of the range to sort.
     * * An iterator past the last element of the range to sort.
     * * A comparison function to use to compare the elements.
     *
     * @tparam Type Optional type of component to compare.
     * @tparam Other Other optional types of components to compare.
     * @tparam Compare Type of comparison function object.
     * @tparam Sort Type of sort function object.
     * @tparam Args Types of arguments to forward to the sort function object.
     * @param compare A valid comparison function object.
     * @param algo A valid sort function object.
     * @param args Arguments to forward to the sort function object, if any.
     */
    template<typename Type, typename... Other, typename Compare, typename Sort = std_sort, typename... Args>
    void sort(Compare compare, Sort algo = Sort{}, Args &&...args) const {
        sort<index_of<Type>, index_of<Other>...>(std::move(compare), std::move(algo), std::forward<Args>(args)...);
    }

    /**
     * @brief Sort a group according to the given comparison function.
     *
     * @sa sort
     *
     * @tparam Index Optional indexes of components to compare.
     * @tparam Compare Type of comparison function object.
     * @tparam Sort Type of sort function object.
     * @tparam Args Types of arguments to forward to the sort function object.
     * @param compare A valid comparison function object.
     * @param algo A valid sort function object.
     * @param args Arguments to forward to the sort function object, if any.
     */
    template<std::size_t... Index, typename Compare, typename Sort = std_sort, typename... Args>
    void sort(Compare compare, Sort algo = Sort{}, Args &&...args) const {
        const auto cpools = pools();

        if constexpr(sizeof...(Index) == 0) {
            static_assert(std::is_invocable_v<Compare, const entity_type, const entity_type>, "Invalid comparison function");
            storage<0>()->sort_n(descriptor->length(), std::move(compare), std::move(algo), std::forward<Args>(args)...);
        } else {
            auto comp = [&compare, &cpools](const entity_type lhs, const entity_type rhs) {
                if constexpr(sizeof...(Index) == 1) {
                    return compare((std::get<Index>(cpools)->get(lhs), ...), (std::get<Index>(cpools)->get(rhs), ...));
                } else {
                    return compare(std::forward_as_tuple(std::get<Index>(cpools)->get(lhs)...), std::forward_as_tuple(std::get<Index>(cpools)->get(rhs)...));
                }
            };

            storage<0>()->sort_n(descriptor->length(), std::move(comp), std::move(algo), std::forward<Args>(args)...);
        }

        auto cb = [this](auto *head, auto *...other) {
            for(auto next = descriptor->length(); next; --next) {
                const auto pos = next - 1;
                [[maybe_unused]] const auto entt = head->data()[pos];
                (other->swap_elements(other->data()[pos], entt), ...);
            }
        };

        std::apply(cb, cpools);
    }

private:
    handler *descriptor;
};

} // namespace entt

#endif

//////////////////// END OF external/entt/entity/group.hpp /////////////////////

//////////////////// START OF external/entt/signal/fwd.hpp /////////////////////

#ifndef ENTT_SIGNAL_FWD_HPP
#define ENTT_SIGNAL_FWD_HPP

#include <memory>

namespace entt {

template<typename>
class delegate;

template<typename = std::allocator<void>>
class basic_dispatcher;

template<typename, typename = std::allocator<void>>
class emitter;

class connection;

struct scoped_connection;

template<typename>
class sink;

template<typename Type, typename = std::allocator<void>>
class sigh;

/*! @brief Alias declaration for the most common use case. */
using dispatcher = basic_dispatcher<>;

/*! @brief Disambiguation tag for constructors and the like. */
template<auto>
struct connect_arg_t {
    /*! @brief Default constructor. */
    explicit connect_arg_t() = default;
};

/**
 * @brief Constant of type connect_arg_t used to disambiguate calls.
 * @tparam Candidate Element to connect (likely a free or member function).
 */
template<auto Candidate>
inline constexpr connect_arg_t<Candidate> connect_arg{};

} // namespace entt

#endif

///////////////////// END OF external/entt/signal/fwd.hpp //////////////////////

////////////////// START OF external/entt/signal/delegate.hpp //////////////////

#ifndef ENTT_SIGNAL_DELEGATE_HPP
#define ENTT_SIGNAL_DELEGATE_HPP

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename Ret, typename... Args>
constexpr auto function_pointer(Ret (*)(Args...)) -> Ret (*)(Args...);

template<typename Ret, typename Type, typename... Args, typename Other>
constexpr auto function_pointer(Ret (*)(Type, Args...), Other &&) -> Ret (*)(Args...);

template<typename Class, typename Ret, typename... Args, typename... Other>
constexpr auto function_pointer(Ret (Class::*)(Args...), Other &&...) -> Ret (*)(Args...);

template<typename Class, typename Ret, typename... Args, typename... Other>
constexpr auto function_pointer(Ret (Class::*)(Args...) const, Other &&...) -> Ret (*)(Args...);

template<typename Class, typename Type, typename... Other>
constexpr auto function_pointer(Type Class::*, Other &&...) -> Type (*)();

template<typename... Type>
using function_pointer_t = decltype(function_pointer(std::declval<Type>()...));

template<typename... Class, typename Ret, typename... Args>
[[nodiscard]] constexpr auto index_sequence_for(Ret (*)(Args...)) {
    return std::index_sequence_for<Class..., Args...>{};
}

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Basic delegate implementation.
 *
 * Primary template isn't defined on purpose. All the specializations give a
 * compile-time error unless the template parameter is a function type.
 */
template<typename>
class delegate;

/**
 * @brief Utility class to use to send around functions and members.
 *
 * Unmanaged delegate for function pointers and members. Users of this class are
 * in charge of disconnecting instances before deleting them.
 *
 * A delegate can be used as a general purpose invoker without memory overhead
 * for free functions possibly with payloads and bound or unbound members.
 *
 * @tparam Ret Return type of a function type.
 * @tparam Args Types of arguments of a function type.
 */
template<typename Ret, typename... Args>
class delegate<Ret(Args...)> {
    template<auto Candidate, std::size_t... Index>
    [[nodiscard]] auto wrap(std::index_sequence<Index...>) noexcept {
        return [](const void *, Args... args) -> Ret {
            [[maybe_unused]] const auto arguments = std::forward_as_tuple(std::forward<Args>(args)...);

            if constexpr(std::is_invocable_r_v<Ret, decltype(Candidate), type_list_element_t<Index, type_list<Args...>>...>) {
                return static_cast<Ret>(std::invoke(Candidate, std::forward<type_list_element_t<Index, type_list<Args...>>>(std::get<Index>(arguments))...));
            } else {
                constexpr auto offset = sizeof...(Args) - sizeof...(Index);
                return static_cast<Ret>(std::invoke(Candidate, std::forward<type_list_element_t<Index + offset, type_list<Args...>>>(std::get<Index + offset>(arguments))...));
            }
        };
    }

    template<auto Candidate, typename Type, std::size_t... Index>
    [[nodiscard]] auto wrap(Type &, std::index_sequence<Index...>) noexcept {
        return [](const void *payload, Args... args) -> Ret {
            [[maybe_unused]] const auto arguments = std::forward_as_tuple(std::forward<Args>(args)...);
            Type *curr = static_cast<Type *>(const_cast<constness_as_t<void, Type> *>(payload));

            if constexpr(std::is_invocable_r_v<Ret, decltype(Candidate), Type &, type_list_element_t<Index, type_list<Args...>>...>) {
                return static_cast<Ret>(std::invoke(Candidate, *curr, std::forward<type_list_element_t<Index, type_list<Args...>>>(std::get<Index>(arguments))...));
            } else {
                constexpr auto offset = sizeof...(Args) - sizeof...(Index);
                return static_cast<Ret>(std::invoke(Candidate, *curr, std::forward<type_list_element_t<Index + offset, type_list<Args...>>>(std::get<Index + offset>(arguments))...));
            }
        };
    }

    template<auto Candidate, typename Type, std::size_t... Index>
    [[nodiscard]] auto wrap(Type *, std::index_sequence<Index...>) noexcept {
        return [](const void *payload, Args... args) -> Ret {
            [[maybe_unused]] const auto arguments = std::forward_as_tuple(std::forward<Args>(args)...);
            Type *curr = static_cast<Type *>(const_cast<constness_as_t<void, Type> *>(payload));

            if constexpr(std::is_invocable_r_v<Ret, decltype(Candidate), Type *, type_list_element_t<Index, type_list<Args...>>...>) {
                return static_cast<Ret>(std::invoke(Candidate, curr, std::forward<type_list_element_t<Index, type_list<Args...>>>(std::get<Index>(arguments))...));
            } else {
                constexpr auto offset = sizeof...(Args) - sizeof...(Index);
                return static_cast<Ret>(std::invoke(Candidate, curr, std::forward<type_list_element_t<Index + offset, type_list<Args...>>>(std::get<Index + offset>(arguments))...));
            }
        };
    }

public:
    /*! @brief Function type of the contained target. */
    using function_type = Ret(const void *, Args...);
    /*! @brief Function type of the delegate. */
    using type = Ret(Args...);
    /*! @brief Return type of the delegate. */
    using result_type = Ret;

    /*! @brief Default constructor. */
    delegate() noexcept
        : instance{nullptr},
          fn{nullptr} {}

    /**
     * @brief Constructs a delegate with a given object or payload, if any.
     * @tparam Candidate Function or member to connect to the delegate.
     * @tparam Type Type of class or type of payload, if any.
     * @param value_or_instance Optional valid object that fits the purpose.
     */
    template<auto Candidate, typename... Type>
    delegate(connect_arg_t<Candidate>, Type &&...value_or_instance) noexcept {
        connect<Candidate>(std::forward<Type>(value_or_instance)...);
    }

    /**
     * @brief Constructs a delegate and connects an user defined function with
     * optional payload.
     * @param function Function to connect to the delegate.
     * @param payload User defined arbitrary data.
     */
    delegate(function_type *function, const void *payload = nullptr) noexcept {
        connect(function, payload);
    }

    /**
     * @brief Connects a free function or an unbound member to a delegate.
     * @tparam Candidate Function or member to connect to the delegate.
     */
    template<auto Candidate>
    void connect() noexcept {
        instance = nullptr;

        if constexpr(std::is_invocable_r_v<Ret, decltype(Candidate), Args...>) {
            fn = [](const void *, Args... args) -> Ret {
                return Ret(std::invoke(Candidate, std::forward<Args>(args)...));
            };
        } else if constexpr(std::is_member_pointer_v<decltype(Candidate)>) {
            fn = wrap<Candidate>(internal::index_sequence_for<type_list_element_t<0, type_list<Args...>>>(internal::function_pointer_t<decltype(Candidate)>{}));
        } else {
            fn = wrap<Candidate>(internal::index_sequence_for(internal::function_pointer_t<decltype(Candidate)>{}));
        }
    }

    /**
     * @brief Connects a free function with payload or a bound member to a
     * delegate.
     *
     * The delegate isn't responsible for the connected object or the payload.
     * Users must always guarantee that the lifetime of the instance overcomes
     * the one of the delegate.<br/>
     * When used to connect a free function with payload, its signature must be
     * such that the instance is the first argument before the ones used to
     * define the delegate itself.
     *
     * @tparam Candidate Function or member to connect to the delegate.
     * @tparam Type Type of class or type of payload.
     * @param value_or_instance A valid reference that fits the purpose.
     */
    template<auto Candidate, typename Type>
    void connect(Type &value_or_instance) noexcept {
        instance = &value_or_instance;

        if constexpr(std::is_invocable_r_v<Ret, decltype(Candidate), Type &, Args...>) {
            fn = [](const void *payload, Args... args) -> Ret {
                Type *curr = static_cast<Type *>(const_cast<constness_as_t<void, Type> *>(payload));
                return Ret(std::invoke(Candidate, *curr, std::forward<Args>(args)...));
            };
        } else {
            fn = wrap<Candidate>(value_or_instance, internal::index_sequence_for(internal::function_pointer_t<decltype(Candidate), Type>{}));
        }
    }

    /**
     * @brief Connects a free function with payload or a bound member to a
     * delegate.
     *
     * @sa connect(Type &)
     *
     * @tparam Candidate Function or member to connect to the delegate.
     * @tparam Type Type of class or type of payload.
     * @param value_or_instance A valid pointer that fits the purpose.
     */
    template<auto Candidate, typename Type>
    void connect(Type *value_or_instance) noexcept {
        instance = value_or_instance;

        if constexpr(std::is_invocable_r_v<Ret, decltype(Candidate), Type *, Args...>) {
            fn = [](const void *payload, Args... args) -> Ret {
                Type *curr = static_cast<Type *>(const_cast<constness_as_t<void, Type> *>(payload));
                return Ret(std::invoke(Candidate, curr, std::forward<Args>(args)...));
            };
        } else {
            fn = wrap<Candidate>(value_or_instance, internal::index_sequence_for(internal::function_pointer_t<decltype(Candidate), Type>{}));
        }
    }

    /**
     * @brief Connects an user defined function with optional payload to a
     * delegate.
     *
     * The delegate isn't responsible for the connected object or the payload.
     * Users must always guarantee that the lifetime of an instance overcomes
     * the one of the delegate.<br/>
     * The payload is returned as the first argument to the target function in
     * all cases.
     *
     * @param function Function to connect to the delegate.
     * @param payload User defined arbitrary data.
     */
    void connect(function_type *function, const void *payload = nullptr) noexcept {
        ENTT_ASSERT(function != nullptr, "Uninitialized function pointer");
        instance = payload;
        fn = function;
    }

    /**
     * @brief Resets a delegate.
     *
     * After a reset, a delegate cannot be invoked anymore.
     */
    void reset() noexcept {
        instance = nullptr;
        fn = nullptr;
    }

    /**
     * @brief Returns a pointer to the stored callable function target, if any.
     * @return An opaque pointer to the stored callable function target.
     */
    [[nodiscard]] function_type *target() const noexcept {
        return fn;
    }

    /**
     * @brief Returns the instance or the payload linked to a delegate, if any.
     * @return An opaque pointer to the underlying data.
     */
    [[nodiscard]] const void *data() const noexcept {
        return instance;
    }

    /**
     * @brief Triggers a delegate.
     *
     * The delegate invokes the underlying function and returns the result.
     *
     * @warning
     * Attempting to trigger an invalid delegate results in undefined
     * behavior.
     *
     * @param args Arguments to use to invoke the underlying function.
     * @return The value returned by the underlying function.
     */
    Ret operator()(Args... args) const {
        ENTT_ASSERT(static_cast<bool>(*this), "Uninitialized delegate");
        return fn(instance, std::forward<Args>(args)...);
    }

    /**
     * @brief Checks whether a delegate actually stores a listener.
     * @return False if the delegate is empty, true otherwise.
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        // no need to also test instance
        return !(fn == nullptr);
    }

    /**
     * @brief Compares the contents of two delegates.
     * @param other Delegate with which to compare.
     * @return False if the two contents differ, true otherwise.
     */
    [[nodiscard]] bool operator==(const delegate<Ret(Args...)> &other) const noexcept {
        return fn == other.fn && instance == other.instance;
    }

private:
    const void *instance;
    function_type *fn;
};

/**
 * @brief Compares the contents of two delegates.
 * @tparam Ret Return type of a function type.
 * @tparam Args Types of arguments of a function type.
 * @param lhs A valid delegate object.
 * @param rhs A valid delegate object.
 * @return True if the two contents differ, false otherwise.
 */
template<typename Ret, typename... Args>
[[nodiscard]] bool operator!=(const delegate<Ret(Args...)> &lhs, const delegate<Ret(Args...)> &rhs) noexcept {
    return !(lhs == rhs);
}

/**
 * @brief Deduction guide.
 * @tparam Candidate Function or member to connect to the delegate.
 */
template<auto Candidate>
delegate(connect_arg_t<Candidate>) -> delegate<std::remove_pointer_t<internal::function_pointer_t<decltype(Candidate)>>>;

/**
 * @brief Deduction guide.
 * @tparam Candidate Function or member to connect to the delegate.
 * @tparam Type Type of class or type of payload.
 */
template<auto Candidate, typename Type>
delegate(connect_arg_t<Candidate>, Type &&) -> delegate<std::remove_pointer_t<internal::function_pointer_t<decltype(Candidate), Type>>>;

/**
 * @brief Deduction guide.
 * @tparam Ret Return type of a function type.
 * @tparam Args Types of arguments of a function type.
 */
template<typename Ret, typename... Args>
delegate(Ret (*)(const void *, Args...), const void * = nullptr) -> delegate<Ret(Args...)>;

} // namespace entt

#endif

/////////////////// END OF external/entt/signal/delegate.hpp ///////////////////

//////////////////// START OF external/entt/signal/sigh.hpp ////////////////////

#ifndef ENTT_SIGNAL_SIGH_HPP
#define ENTT_SIGNAL_SIGH_HPP

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace entt {

/**
 * @brief Sink class.
 *
 * Primary template isn't defined on purpose. All the specializations give a
 * compile-time error unless the template parameter is a function type.
 *
 * @tparam Type A valid signal handler type.
 */
template<typename Type>
class sink;

/**
 * @brief Unmanaged signal handler.
 *
 * Primary template isn't defined on purpose. All the specializations give a
 * compile-time error unless the template parameter is a function type.
 *
 * @tparam Type A valid function type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Type, typename Allocator>
class sigh;

/**
 * @brief Unmanaged signal handler.
 *
 * It works directly with references to classes and pointers to member functions
 * as well as pointers to free functions. Users of this class are in charge of
 * disconnecting instances before deleting them.
 *
 * This class serves mainly two purposes:
 *
 * * Creating signals to use later to notify a bunch of listeners.
 * * Collecting results from a set of functions like in a voting system.
 *
 * @tparam Ret Return type of a function type.
 * @tparam Args Types of arguments of a function type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Ret, typename... Args, typename Allocator>
class sigh<Ret(Args...), Allocator> {
    /*! @brief A sink is allowed to modify a signal. */
    friend class sink<sigh<Ret(Args...), Allocator>>;

    using alloc_traits = std::allocator_traits<Allocator>;
    using delegate_type = delegate<Ret(Args...)>;
    using container_type = std::vector<delegate_type, typename alloc_traits::template rebind_alloc<delegate_type>>;

public:
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Sink type. */
    using sink_type = sink<sigh<Ret(Args...), Allocator>>;

    /*! @brief Default constructor. */
    sigh() noexcept(std::is_nothrow_default_constructible_v<allocator_type> &&std::is_nothrow_constructible_v<container_type, const allocator_type &>)
        : sigh{allocator_type{}} {}

    /**
     * @brief Constructs a signal handler with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit sigh(const allocator_type &allocator) noexcept(std::is_nothrow_constructible_v<container_type, const allocator_type &>)
        : calls{allocator} {}

    /**
     * @brief Copy constructor.
     * @param other The instance to copy from.
     */
    sigh(const sigh &other) noexcept(std::is_nothrow_copy_constructible_v<container_type>)
        : calls{other.calls} {}

    /**
     * @brief Allocator-extended copy constructor.
     * @param other The instance to copy from.
     * @param allocator The allocator to use.
     */
    sigh(const sigh &other, const allocator_type &allocator) noexcept(std::is_nothrow_constructible_v<container_type, const container_type &, const allocator_type &>)
        : calls{other.calls, allocator} {}

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    sigh(sigh &&other) noexcept(std::is_nothrow_move_constructible_v<container_type>)
        : calls{std::move(other.calls)} {}

    /**
     * @brief Allocator-extended move constructor.
     * @param other The instance to move from.
     * @param allocator The allocator to use.
     */
    sigh(sigh &&other, const allocator_type &allocator) noexcept(std::is_nothrow_constructible_v<container_type, container_type &&, const allocator_type &>)
        : calls{std::move(other.calls), allocator} {}

    /**
     * @brief Copy assignment operator.
     * @param other The instance to copy from.
     * @return This signal handler.
     */
    sigh &operator=(const sigh &other) noexcept(std::is_nothrow_copy_assignable_v<container_type>) {
        calls = other.calls;
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This signal handler.
     */
    sigh &operator=(sigh &&other) noexcept(std::is_nothrow_move_assignable_v<container_type>) {
        calls = std::move(other.calls);
        return *this;
    }

    /**
     * @brief Exchanges the contents with those of a given signal handler.
     * @param other Signal handler to exchange the content with.
     */
    void swap(sigh &other) noexcept(std::is_nothrow_swappable_v<container_type>) {
        using std::swap;
        swap(calls, other.calls);
    }

    /**
     * @brief Returns the associated allocator.
     * @return The associated allocator.
     */
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return calls.get_allocator();
    }

    /**
     * @brief Number of listeners connected to the signal.
     * @return Number of listeners currently connected.
     */
    [[nodiscard]] size_type size() const noexcept {
        return calls.size();
    }

    /**
     * @brief Returns false if at least a listener is connected to the signal.
     * @return True if the signal has no listeners connected, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return calls.empty();
    }

    /**
     * @brief Triggers a signal.
     *
     * All the listeners are notified. Order isn't guaranteed.
     *
     * @param args Arguments to use to invoke listeners.
     */
    void publish(Args... args) const {
        for(auto pos = calls.size(); pos; --pos) {
            calls[pos - 1u](args...);
        }
    }

    /**
     * @brief Collects return values from the listeners.
     *
     * The collector must expose a call operator with the following properties:
     *
     * * The return type is either `void` or such that it's convertible to
     *   `bool`. In the second case, a true value will stop the iteration.
     * * The list of parameters is empty if `Ret` is `void`, otherwise it
     *   contains a single element such that `Ret` is convertible to it.
     *
     * @tparam Func Type of collector to use, if any.
     * @param func A valid function object.
     * @param args Arguments to use to invoke listeners.
     */
    template<typename Func>
    void collect(Func func, Args... args) const {
        for(auto pos = calls.size(); pos; --pos) {
            if constexpr(std::is_void_v<Ret> || !std::is_invocable_v<Func, Ret>) {
                calls[pos - 1u](args...);

                if constexpr(std::is_invocable_r_v<bool, Func>) {
                    if(func()) {
                        break;
                    }
                } else {
                    func();
                }
            } else {
                if constexpr(std::is_invocable_r_v<bool, Func, Ret>) {
                    if(func(calls[pos - 1u](args...))) {
                        break;
                    }
                } else {
                    func(calls[pos - 1u](args...));
                }
            }
        }
    }

private:
    container_type calls;
};

/**
 * @brief Connection class.
 *
 * Opaque object the aim of which is to allow users to release an already
 * estabilished connection without having to keep a reference to the signal or
 * the sink that generated it.
 */
class connection {
    /*! @brief A sink is allowed to create connection objects. */
    template<typename>
    friend class sink;

    connection(delegate<void(void *)> fn, void *ref)
        : disconnect{fn}, signal{ref} {}

public:
    /*! @brief Default constructor. */
    connection()
        : disconnect{},
          signal{} {}

    /**
     * @brief Checks whether a connection is properly initialized.
     * @return True if the connection is properly initialized, false otherwise.
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return static_cast<bool>(disconnect);
    }

    /*! @brief Breaks the connection. */
    void release() {
        if(disconnect) {
            disconnect(signal);
            disconnect.reset();
        }
    }

private:
    delegate<void(void *)> disconnect;
    void *signal;
};

/**
 * @brief Scoped connection class.
 *
 * Opaque object the aim of which is to allow users to release an already
 * estabilished connection without having to keep a reference to the signal or
 * the sink that generated it.<br/>
 * A scoped connection automatically breaks the link between the two objects
 * when it goes out of scope.
 */
struct scoped_connection {
    /*! @brief Default constructor. */
    scoped_connection() = default;

    /**
     * @brief Constructs a scoped connection from a basic connection.
     * @param other A valid connection object.
     */
    scoped_connection(const connection &other)
        : conn{other} {}

    /*! @brief Default copy constructor, deleted on purpose. */
    scoped_connection(const scoped_connection &) = delete;

    /**
     * @brief Move constructor.
     * @param other The scoped connection to move from.
     */
    scoped_connection(scoped_connection &&other) noexcept
        : conn{std::exchange(other.conn, {})} {}

    /*! @brief Automatically breaks the link on destruction. */
    ~scoped_connection() {
        conn.release();
    }

    /**
     * @brief Default copy assignment operator, deleted on purpose.
     * @return This scoped connection.
     */
    scoped_connection &operator=(const scoped_connection &) = delete;

    /**
     * @brief Move assignment operator.
     * @param other The scoped connection to move from.
     * @return This scoped connection.
     */
    scoped_connection &operator=(scoped_connection &&other) noexcept {
        conn = std::exchange(other.conn, {});
        return *this;
    }

    /**
     * @brief Acquires a connection.
     * @param other The connection object to acquire.
     * @return This scoped connection.
     */
    scoped_connection &operator=(connection other) {
        conn = std::move(other);
        return *this;
    }

    /**
     * @brief Checks whether a scoped connection is properly initialized.
     * @return True if the connection is properly initialized, false otherwise.
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return static_cast<bool>(conn);
    }

    /*! @brief Breaks the connection. */
    void release() {
        conn.release();
    }

private:
    connection conn;
};

/**
 * @brief Sink class.
 *
 * A sink is used to connect listeners to signals and to disconnect them.<br/>
 * The function type for a listener is the one of the signal to which it
 * belongs.
 *
 * The clear separation between a signal and a sink permits to store the former
 * as private data member without exposing the publish functionality to the
 * users of the class.
 *
 * @warning
 * Lifetime of a sink must not overcome that of the signal to which it refers.
 * In any other case, attempting to use a sink results in undefined behavior.
 *
 * @tparam Ret Return type of a function type.
 * @tparam Args Types of arguments of a function type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Ret, typename... Args, typename Allocator>
class sink<sigh<Ret(Args...), Allocator>> {
    using signal_type = sigh<Ret(Args...), Allocator>;
    using delegate_type = typename signal_type::delegate_type;
    using difference_type = typename signal_type::container_type::difference_type;

    template<auto Candidate, typename Type>
    static void release(Type value_or_instance, void *signal) {
        sink{*static_cast<signal_type *>(signal)}.disconnect<Candidate>(value_or_instance);
    }

    template<auto Candidate>
    static void release(void *signal) {
        sink{*static_cast<signal_type *>(signal)}.disconnect<Candidate>();
    }

    template<typename Func>
    void disconnect_if(Func callback) {
        for(auto pos = signal->calls.size(); pos; --pos) {
            if(auto &elem = signal->calls[pos - 1u]; callback(elem)) {
                elem = std::move(signal->calls.back());
                signal->calls.pop_back();
            }
        }
    }

public:
    /**
     * @brief Constructs a sink that is allowed to modify a given signal.
     * @param ref A valid reference to a signal object.
     */
    sink(sigh<Ret(Args...), Allocator> &ref) noexcept
        : signal{&ref} {}

    /**
     * @brief Returns false if at least a listener is connected to the sink.
     * @return True if the sink has no listeners connected, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return signal->calls.empty();
    }

    /**
     * @brief Connects a free function (with or without payload), a bound or an
     * unbound member to a signal.
     * @tparam Candidate Function or member to connect to the signal.
     * @tparam Type Type of class or type of payload, if any.
     * @param value_or_instance A valid object that fits the purpose, if any.
     * @return A properly initialized connection object.
     */
    template<auto Candidate, typename... Type>
    connection connect(Type &&...value_or_instance) {
        disconnect<Candidate>(value_or_instance...);

        delegate_type call{};
        call.template connect<Candidate>(value_or_instance...);
        signal->calls.push_back(std::move(call));

        delegate<void(void *)> conn{};
        conn.template connect<&release<Candidate, Type...>>(value_or_instance...);
        return {std::move(conn), signal};
    }

    /**
     * @brief Disconnects a free function (with or without payload), a bound or
     * an unbound member from a signal.
     * @tparam Candidate Function or member to disconnect from the signal.
     * @tparam Type Type of class or type of payload, if any.
     * @param value_or_instance A valid object that fits the purpose, if any.
     */
    template<auto Candidate, typename... Type>
    void disconnect(Type &&...value_or_instance) {
        delegate_type call{};
        call.template connect<Candidate>(value_or_instance...);
        disconnect_if([&call](const auto &elem) { return elem == call; });
    }

    /**
     * @brief Disconnects free functions with payload or bound members from a
     * signal.
     * @param value_or_instance A valid object that fits the purpose.
     */
    void disconnect(const void *value_or_instance) {
        if(value_or_instance) {
            disconnect_if([value_or_instance](const auto &elem) { return elem.data() == value_or_instance; });
        }
    }

    /*! @brief Disconnects all the listeners from a signal. */
    void disconnect() {
        signal->calls.clear();
    }

private:
    signal_type *signal;
};

/**
 * @brief Deduction guide.
 *
 * It allows to deduce the signal handler type of a sink directly from the
 * signal it refers to.
 *
 * @tparam Ret Return type of a function type.
 * @tparam Args Types of arguments of a function type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Ret, typename... Args, typename Allocator>
sink(sigh<Ret(Args...), Allocator> &) -> sink<sigh<Ret(Args...), Allocator>>;

} // namespace entt

#endif

///////////////////// END OF external/entt/signal/sigh.hpp /////////////////////

/////////////////// START OF external/entt/entity/mixin.hpp ////////////////////

#ifndef ENTT_ENTITY_MIXIN_HPP
#define ENTT_ENTITY_MIXIN_HPP

#include <type_traits>
#include <utility>

namespace entt {

/**
 * @brief Mixin type used to add signal support to storage types.
 *
 * The function type of a listener is equivalent to:
 *
 * @code{.cpp}
 * void(basic_registry<entity_type> &, entity_type);
 * @endcode
 *
 * This applies to all signals made available.
 *
 * @tparam Type The type of the underlying storage.
 */
template<typename Type>
class sigh_mixin final: public Type {
    using underlying_type = Type;
    using basic_registry_type = basic_registry<typename underlying_type::entity_type, typename underlying_type::base_type::allocator_type>;
    using sigh_type = sigh<void(basic_registry_type &, const typename underlying_type::entity_type), typename underlying_type::allocator_type>;
    using underlying_iterator = typename underlying_type::base_type::basic_iterator;

    basic_registry_type &owner_or_assert() const noexcept {
        ENTT_ASSERT(owner != nullptr, "Invalid pointer to registry");
        return *owner;
    }

    void pop(underlying_iterator first, underlying_iterator last) final {
        if(auto &reg = owner_or_assert(); destruction.empty()) {
            underlying_type::pop(first, last);
        } else {
            for(; first != last; ++first) {
                const auto entt = *first;
                destruction.publish(reg, entt);
                const auto it = underlying_type::find(entt);
                underlying_type::pop(it, it + 1u);
            }
        }
    }

    void pop_all() final {
        if(auto &reg = owner_or_assert(); !destruction.empty()) {
            for(auto pos = underlying_type::each().begin().base().index(); !(pos < 0); --pos) {
                if constexpr(underlying_type::traits_type::in_place_delete) {
                    if(const auto entt = underlying_type::operator[](static_cast<typename underlying_type::size_type>(pos)); entt != tombstone) {
                        destruction.publish(reg, entt);
                    }
                } else {
                    destruction.publish(reg, underlying_type::operator[](static_cast<typename underlying_type::size_type>(pos)));
                }
            }
        }

        underlying_type::pop_all();
    }

    underlying_iterator try_emplace(const typename underlying_type::entity_type entt, const bool force_back, const void *value) final {
        const auto it = underlying_type::try_emplace(entt, force_back, value);

        if(auto &reg = owner_or_assert(); it != underlying_type::base_type::end()) {
            construction.publish(reg, *it);
        }

        return it;
    }

public:
    /*! @brief Allocator type. */
    using allocator_type = typename underlying_type::allocator_type;
    /*! @brief Underlying entity identifier. */
    using entity_type = typename underlying_type::entity_type;
    /*! @brief Expected registry type. */
    using registry_type = basic_registry_type;

    /*! @brief Default constructor. */
    sigh_mixin()
        : sigh_mixin{allocator_type{}} {}

    /**
     * @brief Constructs an empty storage with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit sigh_mixin(const allocator_type &allocator)
        : underlying_type{allocator},
          owner{},
          construction{allocator},
          destruction{allocator},
          update{allocator} {}

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    sigh_mixin(sigh_mixin &&other) noexcept
        : underlying_type{std::move(other)},
          owner{other.owner},
          construction{std::move(other.construction)},
          destruction{std::move(other.destruction)},
          update{std::move(other.update)} {}

    /**
     * @brief Allocator-extended move constructor.
     * @param other The instance to move from.
     * @param allocator The allocator to use.
     */
    sigh_mixin(sigh_mixin &&other, const allocator_type &allocator) noexcept
        : underlying_type{std::move(other), allocator},
          owner{other.owner},
          construction{std::move(other.construction), allocator},
          destruction{std::move(other.destruction), allocator},
          update{std::move(other.update), allocator} {}

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This storage.
     */
    sigh_mixin &operator=(sigh_mixin &&other) noexcept {
        underlying_type::operator=(std::move(other));
        owner = other.owner;
        construction = std::move(other.construction);
        destruction = std::move(other.destruction);
        update = std::move(other.update);
        return *this;
    }

    /**
     * @brief Exchanges the contents with those of a given storage.
     * @param other Storage to exchange the content with.
     */
    void swap(sigh_mixin &other) {
        using std::swap;
        underlying_type::swap(other);
        swap(owner, other.owner);
        swap(construction, other.construction);
        swap(destruction, other.destruction);
        swap(update, other.update);
    }

    /**
     * @brief Returns a sink object.
     *
     * The sink returned by this function can be used to receive notifications
     * whenever a new instance is created and assigned to an entity.<br/>
     * Listeners are invoked after the object has been assigned to the entity.
     *
     * @sa sink
     *
     * @return A temporary sink object.
     */
    [[nodiscard]] auto on_construct() noexcept {
        return sink{construction};
    }

    /**
     * @brief Returns a sink object.
     *
     * The sink returned by this function can be used to receive notifications
     * whenever an instance is explicitly updated.<br/>
     * Listeners are invoked after the object has been updated.
     *
     * @sa sink
     *
     * @return A temporary sink object.
     */
    [[nodiscard]] auto on_update() noexcept {
        return sink{update};
    }

    /**
     * @brief Returns a sink object.
     *
     * The sink returned by this function can be used to receive notifications
     * whenever an instance is removed from an entity and thus destroyed.<br/>
     * Listeners are invoked before the object has been removed from the entity.
     *
     * @sa sink
     *
     * @return A temporary sink object.
     */
    [[nodiscard]] auto on_destroy() noexcept {
        return sink{destruction};
    }

    /**
     * @brief Emplace elements into a storage.
     *
     * The behavior of this operation depends on the underlying storage type
     * (for example, components vs entities).<br/>
     * Refer to the specific documentation for more details.
     *
     * @return A return value as returned by the underlying storage.
     */
    auto emplace() {
        const auto entt = underlying_type::emplace();
        construction.publish(owner_or_assert(), entt);
        return entt;
    }

    /**
     * @brief Emplace elements into a storage.
     *
     * The behavior of this operation depends on the underlying storage type
     * (for example, components vs entities).<br/>
     * Refer to the specific documentation for more details.
     *
     * @tparam Args Types of arguments to forward to the underlying storage.
     * @param hint A valid identifier.
     * @param args Parameters to forward to the underlying storage.
     * @return A return value as returned by the underlying storage.
     */
    template<typename... Args>
    decltype(auto) emplace(const entity_type hint, Args &&...args) {
        if constexpr(std::is_same_v<typename underlying_type::value_type, typename underlying_type::entity_type>) {
            const auto entt = underlying_type::emplace(hint, std::forward<Args>(args)...);
            construction.publish(owner_or_assert(), entt);
            return entt;
        } else {
            underlying_type::emplace(hint, std::forward<Args>(args)...);
            construction.publish(owner_or_assert(), hint);
            return this->get(hint);
        }
    }

    /**
     * @brief Patches the given instance for an entity.
     * @tparam Func Types of the function objects to invoke.
     * @param entt A valid identifier.
     * @param func Valid function objects.
     * @return A reference to the patched instance.
     */
    template<typename... Func>
    decltype(auto) patch(const entity_type entt, Func &&...func) {
        underlying_type::patch(entt, std::forward<Func>(func)...);
        update.publish(owner_or_assert(), entt);
        return this->get(entt);
    }

    /**
     * @brief Emplace elements into a storage.
     *
     * The behavior of this operation depends on the underlying storage type
     * (for example, components vs entities).<br/>
     * Refer to the specific documentation for more details.
     *
     * @tparam It Iterator type (as required by the underlying storage type).
     * @tparam Args Types of arguments to forward to the underlying storage.
     * @param first An iterator to the first element of the range.
     * @param last An iterator past the last element of the range.
     * @param args Parameters to use to forward to the underlying storage.
     */
    template<typename It, typename... Args>
    void insert(It first, It last, Args &&...args) {
        underlying_type::insert(first, last, std::forward<Args>(args)...);

        if(auto &reg = owner_or_assert(); !construction.empty()) {
            for(; first != last; ++first) {
                construction.publish(reg, *first);
            }
        }
    }

    /**
     * @brief Forwards variables to derived classes, if any.
     * @param value A variable wrapped in an opaque container.
     */
    void bind(any value) noexcept final {
        auto *reg = any_cast<basic_registry_type>(&value);
        owner = reg ? reg : owner;
        underlying_type::bind(std::move(value));
    }

private:
    basic_registry_type *owner;
    sigh_type construction;
    sigh_type destruction;
    sigh_type update;
};

} // namespace entt

#endif

//////////////////// END OF external/entt/entity/mixin.hpp /////////////////////

//////////////////// START OF external/entt/entity/view.hpp ////////////////////

#ifndef ENTT_ENTITY_VIEW_HPP
#define ENTT_ENTITY_VIEW_HPP

#include <array>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename... Args, typename Type, std::size_t N>
[[nodiscard]] auto filter_as_tuple(const std::array<const Type *, N> &filter) noexcept {
    return std::apply([](const auto *...curr) { return std::make_tuple(static_cast<Args *>(const_cast<constness_as_t<Type, Args> *>(curr))...); }, filter);
}

template<typename Type, std::size_t N>
[[nodiscard]] auto none_of(const std::array<const Type *, N> &filter, const typename Type::entity_type entt) noexcept {
    return std::apply([entt](const auto *...curr) { return (!(curr && curr->contains(entt)) && ...); }, filter);
}

template<typename... Get, typename... Exclude, std::size_t... Index>
[[nodiscard]] auto view_pack(const std::tuple<Get *...> value, const std::tuple<Exclude *...> excl, std::index_sequence<Index...>) {
    const auto pools = std::tuple_cat(value, excl);
    basic_view<get_t<Get...>, exclude_t<Exclude...>> elem{};
    (((std::get<Index>(pools) != nullptr) ? elem.template storage<Index>(*std::get<Index>(pools)) : void()), ...);
    return elem;
}

template<typename Type, std::size_t Get, std::size_t Exclude>
class view_iterator final {
    using iterator_type = typename Type::const_iterator;

    [[nodiscard]] bool valid() const noexcept {
        return ((Get != 0u) || (*it != tombstone))
               && std::apply([entt = *it](const auto *...curr) { return (curr->contains(entt) && ...); }, pools)
               && none_of(filter, *it);
    }

public:
    using value_type = typename iterator_type::value_type;
    using pointer = typename iterator_type::pointer;
    using reference = typename iterator_type::reference;
    using difference_type = typename iterator_type::difference_type;
    using iterator_category = std::forward_iterator_tag;

    constexpr view_iterator() noexcept
        : it{},
          last{},
          pools{},
          filter{} {}

    view_iterator(iterator_type curr, iterator_type to, std::array<const Type *, Get> value, std::array<const Type *, Exclude> excl) noexcept
        : it{curr},
          last{to},
          pools{value},
          filter{excl} {
        while(it != last && !valid()) {
            ++it;
        }
    }

    view_iterator &operator++() noexcept {
        while(++it != last && !valid()) {}
        return *this;
    }

    view_iterator operator++(int) noexcept {
        view_iterator orig = *this;
        return ++(*this), orig;
    }

    [[nodiscard]] pointer operator->() const noexcept {
        return &*it;
    }

    [[nodiscard]] reference operator*() const noexcept {
        return *operator->();
    }

    template<typename LhsType, auto... LhsArgs, typename RhsType, auto... RhsArgs>
    friend constexpr bool operator==(const view_iterator<LhsType, LhsArgs...> &, const view_iterator<RhsType, RhsArgs...> &) noexcept;

private:
    iterator_type it;
    iterator_type last;
    std::array<const Type *, Get> pools;
    std::array<const Type *, Exclude> filter;
};

template<typename LhsType, auto... LhsArgs, typename RhsType, auto... RhsArgs>
[[nodiscard]] constexpr bool operator==(const view_iterator<LhsType, LhsArgs...> &lhs, const view_iterator<RhsType, RhsArgs...> &rhs) noexcept {
    return lhs.it == rhs.it;
}

template<typename LhsType, auto... LhsArgs, typename RhsType, auto... RhsArgs>
[[nodiscard]] constexpr bool operator!=(const view_iterator<LhsType, LhsArgs...> &lhs, const view_iterator<RhsType, RhsArgs...> &rhs) noexcept {
    return !(lhs == rhs);
}

template<typename It, typename... Type>
struct extended_view_iterator final {
    using iterator_type = It;
    using difference_type = std::ptrdiff_t;
    using value_type = decltype(std::tuple_cat(std::make_tuple(*std::declval<It>()), std::declval<Type>().get_as_tuple({})...));
    using pointer = input_iterator_pointer<value_type>;
    using reference = value_type;
    using iterator_category = std::input_iterator_tag;

    constexpr extended_view_iterator()
        : it{},
          pools{} {}

    extended_view_iterator(It from, std::tuple<Type *...> value)
        : it{from},
          pools{value} {}

    extended_view_iterator &operator++() noexcept {
        return ++it, *this;
    }

    extended_view_iterator operator++(int) noexcept {
        extended_view_iterator orig = *this;
        return ++(*this), orig;
    }

    [[nodiscard]] reference operator*() const noexcept {
        return std::apply([entt = *it](auto *...curr) { return std::tuple_cat(std::make_tuple(entt), curr->get_as_tuple(entt)...); }, pools);
    }

    [[nodiscard]] pointer operator->() const noexcept {
        return operator*();
    }

    [[nodiscard]] constexpr iterator_type base() const noexcept {
        return it;
    }

    template<typename... Lhs, typename... Rhs>
    friend bool constexpr operator==(const extended_view_iterator<Lhs...> &, const extended_view_iterator<Rhs...> &) noexcept;

private:
    It it;
    std::tuple<Type *...> pools;
};

template<typename... Lhs, typename... Rhs>
[[nodiscard]] constexpr bool operator==(const extended_view_iterator<Lhs...> &lhs, const extended_view_iterator<Rhs...> &rhs) noexcept {
    return lhs.it == rhs.it;
}

template<typename... Lhs, typename... Rhs>
[[nodiscard]] constexpr bool operator!=(const extended_view_iterator<Lhs...> &lhs, const extended_view_iterator<Rhs...> &rhs) noexcept {
    return !(lhs == rhs);
}

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief View implementation.
 *
 * Primary template isn't defined on purpose. All the specializations give a
 * compile-time error, but for a few reasonable cases.
 */
template<typename, typename, typename>
class basic_view;

/**
 * @brief Multi component view.
 *
 * Multi component views iterate over those entities that are at least in the
 * given storage. During initialization, a multi component view looks at the
 * number of entities available for each component and uses the smallest set in
 * order to get a performance boost when iterating.
 *
 * @b Important
 *
 * Iterators aren't invalidated if:
 *
 * * New elements are added to the storage.
 * * The entity currently pointed is modified (for example, components are added
 *   or removed from it).
 * * The entity currently pointed is destroyed.
 *
 * In all other cases, modifying the storage iterated by the view in any way
 * invalidates all the iterators.
 *
 * @tparam Get Types of storage iterated by the view.
 * @tparam Exclude Types of storage used to filter the view.
 */
template<typename... Get, typename... Exclude>
class basic_view<get_t<Get...>, exclude_t<Exclude...>> {
    static constexpr auto offset = sizeof...(Get);
    using base_type = std::common_type_t<typename Get::base_type..., typename Exclude::base_type...>;
    using underlying_type = typename base_type::entity_type;

    template<typename, typename, typename>
    friend class basic_view;

    template<typename Type>
    static constexpr std::size_t index_of = type_list_index_v<std::remove_const_t<Type>, type_list<typename Get::value_type..., typename Exclude::value_type...>>;

    [[nodiscard]] auto opaque_check_set() const noexcept {
        std::array<const common_type *, sizeof...(Get) - 1u> other{};
        std::apply([&other, pos = 0u, view = view](const auto *...curr) mutable { ((curr == view ? void() : void(other[pos++] = curr)), ...); }, pools);
        return other;
    }

    void unchecked_refresh() noexcept {
        view = std::get<0>(pools);
        std::apply([this](auto *, auto *...other) { ((this->view = other->size() < this->view->size() ? other : this->view), ...); }, pools);
    }

    template<std::size_t Curr, std::size_t Other, typename... Args>
    [[nodiscard]] auto dispatch_get(const std::tuple<underlying_type, Args...> &curr) const {
        if constexpr(Curr == Other) {
            return std::forward_as_tuple(std::get<Args>(curr)...);
        } else {
            return std::get<Other>(pools)->get_as_tuple(std::get<0>(curr));
        }
    }

    template<std::size_t Curr, typename Func, std::size_t... Index>
    void each(Func &func, std::index_sequence<Index...>) const {
        for(const auto curr: std::get<Curr>(pools)->each()) {
            if(const auto entt = std::get<0>(curr); ((sizeof...(Get) != 1u) || (entt != tombstone)) && ((Curr == Index || std::get<Index>(pools)->contains(entt)) && ...) && internal::none_of(filter, entt)) {
                if constexpr(is_applicable_v<Func, decltype(std::tuple_cat(std::tuple<entity_type>{}, std::declval<basic_view>().get({})))>) {
                    std::apply(func, std::tuple_cat(std::make_tuple(entt), dispatch_get<Curr, Index>(curr)...));
                } else {
                    std::apply(func, std::tuple_cat(dispatch_get<Curr, Index>(curr)...));
                }
            }
        }
    }

    template<typename Func, std::size_t... Index>
    void pick_and_each(Func &func, std::index_sequence<Index...> seq) const {
        ((std::get<Index>(pools) == view ? each<Index>(func, seq) : void()), ...);
    }

public:
    /*! @brief Underlying entity identifier. */
    using entity_type = underlying_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Common type among all storage types. */
    using common_type = base_type;
    /*! @brief Bidirectional iterator type. */
    using iterator = internal::view_iterator<common_type, sizeof...(Get) - 1u, sizeof...(Exclude)>;
    /*! @brief Iterable view type. */
    using iterable = iterable_adaptor<internal::extended_view_iterator<iterator, Get...>>;

    /*! @brief Default constructor to use to create empty, invalid views. */
    basic_view() noexcept
        : pools{},
          filter{},
          view{} {}

    /**
     * @brief Constructs a multi-type view from a set of storage classes.
     * @param value The storage for the types to iterate.
     * @param excl The storage for the types used to filter the view.
     */
    basic_view(Get &...value, Exclude &...excl) noexcept
        : pools{&value...},
          filter{&excl...},
          view{} {
        unchecked_refresh();
    }

    /**
     * @brief Constructs a multi-type view from a set of storage classes.
     * @param value The storage for the types to iterate.
     * @param excl The storage for the types used to filter the view.
     */
    basic_view(std::tuple<Get &...> value, std::tuple<Exclude &...> excl = {}) noexcept
        : basic_view{std::make_from_tuple<basic_view>(std::tuple_cat(value, excl))} {}

    /**
     * @brief Forces a view to use a given component to drive iterations
     * @tparam Type Type of component to use to drive iterations.
     */
    template<typename Type>
    void use() noexcept {
        use<index_of<Type>>();
    }

    /**
     * @brief Forces a view to use a given component to drive iterations
     * @tparam Index Index of the component to use to drive iterations.
     */
    template<std::size_t Index>
    void use() noexcept {
        if(view) {
            view = std::get<Index>(pools);
        }
    }

    /*! @brief Updates the internal leading view if required. */
    void refresh() noexcept {
        if(view || std::apply([](const auto *...curr) { return ((curr != nullptr) && ...); }, pools)) {
            unchecked_refresh();
        }
    }

    /**
     * @brief Returns the leading storage of a view, if any.
     * @return The leading storage of the view.
     */
    [[nodiscard]] const common_type *handle() const noexcept {
        return view;
    }

    /**
     * @brief Returns the storage for a given component type, if any.
     * @tparam Type Type of component of which to return the storage.
     * @return The storage for the given component type.
     */
    template<typename Type>
    [[nodiscard]] auto *storage() const noexcept {
        return storage<index_of<Type>>();
    }

    /**
     * @brief Returns the storage for a given index, if any.
     * @tparam Index Index of the storage to return.
     * @return The storage for the given index.
     */
    template<std::size_t Index>
    [[nodiscard]] auto *storage() const noexcept {
        if constexpr(Index < offset) {
            return std::get<Index>(pools);
        } else {
            return std::get<Index - offset>(internal::filter_as_tuple<Exclude...>(filter));
        }
    }

    /**
     * @brief Assigns a storage to a view.
     * @tparam Type Type of storage to assign to the view.
     * @param elem A storage to assign to the view.
     */
    template<typename Type>
    void storage(Type &elem) noexcept {
        storage<index_of<typename Type::value_type>>(elem);
    }

    /**
     * @brief Assigns a storage to a view.
     * @tparam Index Index of the storage to assign to the view.
     * @tparam Type Type of storage to assign to the view.
     * @param elem A storage to assign to the view.
     */
    template<std::size_t Index, typename Type>
    void storage(Type &elem) noexcept {
        if constexpr(Index < offset) {
            std::get<Index>(pools) = &elem;
            refresh();
        } else {
            std::get<Index - offset>(filter) = &elem;
        }
    }

    /**
     * @brief Estimates the number of entities iterated by the view.
     * @return Estimated number of entities iterated by the view.
     */
    [[nodiscard]] size_type size_hint() const noexcept {
        return view ? view->size() : size_type{};
    }

    /**
     * @brief Returns an iterator to the first entity of the view.
     *
     * If the view is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first entity of the view.
     */
    [[nodiscard]] iterator begin() const noexcept {
        return view ? iterator{view->begin(), view->end(), opaque_check_set(), filter} : iterator{};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the view.
     * @return An iterator to the entity following the last entity of the view.
     */
    [[nodiscard]] iterator end() const noexcept {
        return view ? iterator{view->end(), view->end(), opaque_check_set(), filter} : iterator{};
    }

    /**
     * @brief Returns the first entity of the view, if any.
     * @return The first entity of the view if one exists, the null entity
     * otherwise.
     */
    [[nodiscard]] entity_type front() const noexcept {
        const auto it = begin();
        return it != end() ? *it : null;
    }

    /**
     * @brief Returns the last entity of the view, if any.
     * @return The last entity of the view if one exists, the null entity
     * otherwise.
     */
    [[nodiscard]] entity_type back() const noexcept {
        if(view) {
            auto it = view->rbegin();
            for(const auto last = view->rend(); it != last && !contains(*it); ++it) {}
            return it == view->rend() ? null : *it;
        }

        return null;
    }

    /**
     * @brief Finds an entity.
     * @param entt A valid identifier.
     * @return An iterator to the given entity if it's found, past the end
     * iterator otherwise.
     */
    [[nodiscard]] iterator find(const entity_type entt) const noexcept {
        return contains(entt) ? iterator{view->find(entt), view->end(), opaque_check_set(), filter} : end();
    }

    /**
     * @brief Returns the components assigned to the given entity.
     * @param entt A valid identifier.
     * @return The components assigned to the given entity.
     */
    [[nodiscard]] decltype(auto) operator[](const entity_type entt) const {
        return get(entt);
    }

    /**
     * @brief Checks if a view is fully initialized.
     * @return True if the view is fully initialized, false otherwise.
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return std::apply([](const auto *...curr) { return ((curr != nullptr) && ...); }, pools)
               && std::apply([](const auto *...curr) { return ((curr != nullptr) && ...); }, filter);
    }

    /**
     * @brief Checks if a view contains an entity.
     * @param entt A valid identifier.
     * @return True if the view contains the given entity, false otherwise.
     */
    [[nodiscard]] bool contains(const entity_type entt) const noexcept {
        return view && std::apply([entt](const auto *...curr) { return (curr->contains(entt) && ...); }, pools) && internal::none_of(filter, entt);
    }

    /**
     * @brief Returns the components assigned to the given entity.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the view results in
     * undefined behavior.
     *
     * @tparam Type Type of the component to get.
     * @tparam Other Other types of components to get.
     * @param entt A valid identifier.
     * @return The components assigned to the entity.
     */
    template<typename Type, typename... Other>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        return get<index_of<Type>, index_of<Other>...>(entt);
    }

    /**
     * @brief Returns the components assigned to the given entity.
     *
     * @sa get
     *
     * @tparam Index Indexes of the components to get.
     * @param entt A valid identifier.
     * @return The components assigned to the entity.
     */
    template<std::size_t... Index>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        if constexpr(sizeof...(Index) == 0) {
            return std::apply([entt](auto *...curr) { return std::tuple_cat(curr->get_as_tuple(entt)...); }, pools);
        } else if constexpr(sizeof...(Index) == 1) {
            return (std::get<Index>(pools)->get(entt), ...);
        } else {
            return std::tuple_cat(std::get<Index>(pools)->get_as_tuple(entt)...);
        }
    }

    /**
     * @brief Iterates entities and components and applies the given function
     * object to them.
     *
     * The function object is invoked for each entity. It is provided with the
     * entity itself and a set of references to non-empty components. The
     * _constness_ of the components is as requested.<br/>
     * The signature of the function must be equivalent to one of the following
     * forms:
     *
     * @code{.cpp}
     * void(const entity_type, Type &...);
     * void(Type &...);
     * @endcode
     *
     * @tparam Func Type of the function object to invoke.
     * @param func A valid function object.
     */
    template<typename Func>
    void each(Func func) const {
        view ? pick_and_each(func, std::index_sequence_for<Get...>{}) : void();
    }

    /**
     * @brief Returns an iterable object to use to _visit_ a view.
     *
     * The iterable object returns a tuple that contains the current entity and
     * a set of references to its non-empty components. The _constness_ of the
     * components is as requested.
     *
     * @return An iterable object to use to _visit_ the view.
     */
    [[nodiscard]] iterable each() const noexcept {
        return {internal::extended_view_iterator{begin(), pools}, internal::extended_view_iterator{end(), pools}};
    }

    /**
     * @brief Combines two views in a _more specific_ one (friend function).
     * @tparam OGet Component list of the view to combine with.
     * @tparam OExclude Filter list of the view to combine with.
     * @param other The view to combine with.
     * @return A more specific view.
     */
    template<typename... OGet, typename... OExclude>
    [[nodiscard]] auto operator|(const basic_view<get_t<OGet...>, exclude_t<OExclude...>> &other) const noexcept {
        return internal::view_pack(
            std::tuple_cat(pools, other.pools),
            std::tuple_cat(internal::filter_as_tuple<Exclude...>(filter), internal::filter_as_tuple<OExclude...>(other.filter)),
            std::index_sequence_for<Get..., OGet..., Exclude..., OExclude...>{});
    }

private:
    std::tuple<Get *...> pools;
    std::array<const common_type *, sizeof...(Exclude)> filter;
    const common_type *view;
};

/**
 * @brief Single component view specialization.
 *
 * Single component views are specialized in order to get a boost in terms of
 * performance. This kind of views can access the underlying data structure
 * directly and avoid superfluous checks.
 *
 * @b Important
 *
 * Iterators aren't invalidated if:
 *
 * * New elements are added to the storage.
 * * The entity currently pointed is modified (for example, components are added
 *   or removed from it).
 * * The entity currently pointed is destroyed.
 *
 * In all other cases, modifying the storage iterated by the view in any way
 * invalidates all the iterators.
 *
 * @tparam Get Type of storage iterated by the view.
 */
template<typename Get>
class basic_view<get_t<Get>, exclude_t<>, std::void_t<std::enable_if_t<!Get::traits_type::in_place_delete>>> {
    template<typename, typename, typename>
    friend class basic_view;

public:
    /*! @brief Underlying entity identifier. */
    using entity_type = typename Get::entity_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Common type among all storage types. */
    using common_type = typename Get::base_type;
    /*! @brief Random access iterator type. */
    using iterator = typename common_type::iterator;
    /*! @brief Reversed iterator type. */
    using reverse_iterator = typename common_type::reverse_iterator;
    /*! @brief Iterable view type. */
    using iterable = decltype(std::declval<Get>().each());

    /*! @brief Default constructor to use to create empty, invalid views. */
    basic_view() noexcept
        : pools{},
          filter{},
          view{} {}

    /**
     * @brief Constructs a single-type view from a storage class.
     * @param value The storage for the type to iterate.
     */
    basic_view(Get &value) noexcept
        : pools{&value},
          filter{},
          view{&value} {}

    /**
     * @brief Constructs a single-type view from a storage class.
     * @param value The storage for the type to iterate.
     */
    basic_view(std::tuple<Get &> value, std::tuple<> = {}) noexcept
        : basic_view{std::get<0>(value)} {}

    /**
     * @brief Returns the leading storage of a view, if any.
     * @return The leading storage of the view.
     */
    [[nodiscard]] const common_type *handle() const noexcept {
        return view;
    }

    /**
     * @brief Returns the storage for a given component type, if any.
     * @tparam Type Type of component of which to return the storage.
     * @return The storage for the given component type.
     */
    template<typename Type = typename Get::value_type>
    [[nodiscard]] auto *storage() const noexcept {
        static_assert(std::is_same_v<std::remove_const_t<Type>, typename Get::value_type>, "Invalid component type");
        return storage<0>();
    }

    /**
     * @brief Returns the storage for a given index, if any.
     * @tparam Index Index of the storage to return.
     * @return The storage for the given index.
     */
    template<std::size_t Index>
    [[nodiscard]] auto *storage() const noexcept {
        return std::get<Index>(pools);
    }

    /**
     * @brief Assigns a storage to a view.
     * @param elem A storage to assign to the view.
     */
    void storage(Get &elem) noexcept {
        storage<0>(elem);
    }

    /**
     * @brief Assigns a storage to a view.
     * @tparam Index Index of the storage to assign to the view.
     * @param elem A storage to assign to the view.
     */
    template<std::size_t Index>
    void storage(Get &elem) noexcept {
        view = std::get<Index>(pools) = &elem;
    }

    /**
     * @brief Returns the number of entities that have the given component.
     * @return Number of entities that have the given component.
     */
    [[nodiscard]] size_type size() const noexcept {
        return view ? view->size() : size_type{};
    }

    /**
     * @brief Checks whether a view is empty.
     * @return True if the view is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return !view || view->empty();
    }

    /**
     * @brief Returns an iterator to the first entity of the view.
     *
     * If the view is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first entity of the view.
     */
    [[nodiscard]] iterator begin() const noexcept {
        return view ? view->begin() : iterator{};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the view.
     * @return An iterator to the entity following the last entity of the view.
     */
    [[nodiscard]] iterator end() const noexcept {
        return view ? view->end() : iterator{};
    }

    /**
     * @brief Returns an iterator to the first entity of the reversed view.
     *
     * If the view is empty, the returned iterator will be equal to `rend()`.
     *
     * @return An iterator to the first entity of the reversed view.
     */
    [[nodiscard]] reverse_iterator rbegin() const noexcept {
        return view ? view->rbegin() : reverse_iterator{};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the reversed
     * view.
     * @return An iterator to the entity following the last entity of the
     * reversed view.
     */
    [[nodiscard]] reverse_iterator rend() const noexcept {
        return view ? view->rend() : reverse_iterator{};
    }

    /**
     * @brief Returns the first entity of the view, if any.
     * @return The first entity of the view if one exists, the null entity
     * otherwise.
     */
    [[nodiscard]] entity_type front() const noexcept {
        return (!view || view->empty()) ? null : *view->begin();
    }

    /**
     * @brief Returns the last entity of the view, if any.
     * @return The last entity of the view if one exists, the null entity
     * otherwise.
     */
    [[nodiscard]] entity_type back() const noexcept {
        return (!view || view->empty()) ? null : *view->rbegin();
    }

    /**
     * @brief Finds an entity.
     * @param entt A valid identifier.
     * @return An iterator to the given entity if it's found, past the end
     * iterator otherwise.
     */
    [[nodiscard]] iterator find(const entity_type entt) const noexcept {
        return view ? view->find(entt) : iterator{};
    }

    /**
     * @brief Returns the identifier that occupies the given position.
     * @param pos Position of the element to return.
     * @return The identifier that occupies the given position.
     */
    [[nodiscard]] entity_type operator[](const size_type pos) const {
        return begin()[pos];
    }

    /**
     * @brief Returns the component assigned to the given entity.
     * @param entt A valid identifier.
     * @return The component assigned to the given entity.
     */
    [[nodiscard]] decltype(auto) operator[](const entity_type entt) const {
        return std::get<0>(pools)->get(entt);
    }

    /**
     * @brief Checks if a view is fully initialized.
     * @return True if the view is fully initialized, false otherwise.
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return (std::get<0>(pools) != nullptr);
    }

    /**
     * @brief Checks if a view contains an entity.
     * @param entt A valid identifier.
     * @return True if the view contains the given entity, false otherwise.
     */
    [[nodiscard]] bool contains(const entity_type entt) const noexcept {
        return view && view->contains(entt);
    }

    /**
     * @brief Returns the component assigned to the given entity.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the view results in
     * undefined behavior.
     *
     * @tparam Elem Type or index of the component to get.
     * @param entt A valid identifier.
     * @return The component assigned to the entity.
     */
    template<typename Elem>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        static_assert(std::is_same_v<std::remove_const_t<Elem>, typename Get::value_type>, "Invalid component type");
        return get<0>(entt);
    }

    /*! @copydoc get */
    template<std::size_t... Elem>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        if constexpr(sizeof...(Elem) == 0) {
            return std::get<0>(pools)->get_as_tuple(entt);
        } else {
            return std::get<Elem...>(pools)->get(entt);
        }
    }

    /**
     * @brief Iterates entities and components and applies the given function
     * object to them.
     *
     * The function object is invoked for each entity. It is provided with the
     * entity itself and a reference to the component if it's a non-empty one.
     * The _constness_ of the component is as requested.<br/>
     * The signature of the function must be equivalent to one of the following
     * forms:
     *
     * @code{.cpp}
     * void(const entity_type, Type &);
     * void(typename Type &);
     * @endcode
     *
     * @note
     * Empty types aren't explicitly instantiated and therefore they are never
     * returned during iterations.
     *
     * @tparam Func Type of the function object to invoke.
     * @param func A valid function object.
     */
    template<typename Func>
    void each(Func func) const {
        if(view) {
            if constexpr(is_applicable_v<Func, decltype(*each().begin())>) {
                for(const auto pack: each()) {
                    std::apply(func, pack);
                }
            } else if constexpr(Get::traits_type::page_size == 0u) {
                for(size_type pos{}, last = size(); pos < last; ++pos) {
                    func();
                }
            } else {
                for(auto &&component: *std::get<0>(pools)) {
                    func(component);
                }
            }
        }
    }

    /**
     * @brief Returns an iterable object to use to _visit_ a view.
     *
     * The iterable object returns a tuple that contains the current entity and
     * a reference to its component if it's a non-empty one. The _constness_ of
     * the component is as requested.
     *
     * @return An iterable object to use to _visit_ the view.
     */
    [[nodiscard]] iterable each() const noexcept {
        return view ? std::get<0>(pools)->each() : iterable{};
    }

    /**
     * @brief Combines two views in a _more specific_ one (friend function).
     * @tparam OGet Component list of the view to combine with.
     * @tparam OExclude Filter list of the view to combine with.
     * @param other The view to combine with.
     * @return A more specific view.
     */
    template<typename... OGet, typename... OExclude>
    [[nodiscard]] auto operator|(const basic_view<get_t<OGet...>, exclude_t<OExclude...>> &other) const noexcept {
        return internal::view_pack(
            std::tuple_cat(pools, other.pools),
            internal::filter_as_tuple<OExclude...>(other.filter),
            std::index_sequence_for<Get, OGet..., OExclude...>{});
    }

private:
    std::tuple<Get *> pools;
    std::array<const common_type *, 0u> filter;
    const common_type *view;
};

/**
 * @brief Deduction guide.
 * @tparam Type Type of storage classes used to create the view.
 * @param storage The storage for the types to iterate.
 */
template<typename... Type>
basic_view(Type &...storage) -> basic_view<get_t<Type...>, exclude_t<>>;

/**
 * @brief Deduction guide.
 * @tparam Get Types of components iterated by the view.
 * @tparam Exclude Types of components used to filter the view.
 */
template<typename... Get, typename... Exclude>
basic_view(std::tuple<Get &...>, std::tuple<Exclude &...> = {}) -> basic_view<get_t<Get...>, exclude_t<Exclude...>>;

} // namespace entt

#endif

///////////////////// END OF external/entt/entity/view.hpp /////////////////////

////////////////// START OF external/entt/entity/registry.hpp //////////////////

#ifndef ENTT_ENTITY_REGISTRY_HPP
#define ENTT_ENTITY_REGISTRY_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename It>
class registry_storage_iterator final {
    template<typename Other>
    friend class registry_storage_iterator;

    using mapped_type = std::remove_reference_t<decltype(std::declval<It>()->second)>;

public:
    using value_type = std::pair<id_type, constness_as_t<typename mapped_type::element_type, mapped_type> &>;
    using pointer = input_iterator_pointer<value_type>;
    using reference = value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    constexpr registry_storage_iterator() noexcept
        : it{} {}

    constexpr registry_storage_iterator(It iter) noexcept
        : it{iter} {}

    template<typename Other, typename = std::enable_if_t<!std::is_same_v<It, Other> && std::is_constructible_v<It, Other>>>
    constexpr registry_storage_iterator(const registry_storage_iterator<Other> &other) noexcept
        : registry_storage_iterator{other.it} {}

    constexpr registry_storage_iterator &operator++() noexcept {
        return ++it, *this;
    }

    constexpr registry_storage_iterator operator++(int) noexcept {
        registry_storage_iterator orig = *this;
        return ++(*this), orig;
    }

    constexpr registry_storage_iterator &operator--() noexcept {
        return --it, *this;
    }

    constexpr registry_storage_iterator operator--(int) noexcept {
        registry_storage_iterator orig = *this;
        return operator--(), orig;
    }

    constexpr registry_storage_iterator &operator+=(const difference_type value) noexcept {
        it += value;
        return *this;
    }

    constexpr registry_storage_iterator operator+(const difference_type value) const noexcept {
        registry_storage_iterator copy = *this;
        return (copy += value);
    }

    constexpr registry_storage_iterator &operator-=(const difference_type value) noexcept {
        return (*this += -value);
    }

    constexpr registry_storage_iterator operator-(const difference_type value) const noexcept {
        return (*this + -value);
    }

    [[nodiscard]] constexpr reference operator[](const difference_type value) const noexcept {
        return {it[value].first, *it[value].second};
    }

    [[nodiscard]] constexpr reference operator*() const noexcept {
        return {it->first, *it->second};
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept {
        return operator*();
    }

    template<typename Lhs, typename Rhs>
    friend constexpr std::ptrdiff_t operator-(const registry_storage_iterator<Lhs> &, const registry_storage_iterator<Rhs> &) noexcept;

    template<typename Lhs, typename Rhs>
    friend constexpr bool operator==(const registry_storage_iterator<Lhs> &, const registry_storage_iterator<Rhs> &) noexcept;

    template<typename Lhs, typename Rhs>
    friend constexpr bool operator<(const registry_storage_iterator<Lhs> &, const registry_storage_iterator<Rhs> &) noexcept;

private:
    It it;
};

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr std::ptrdiff_t operator-(const registry_storage_iterator<Lhs> &lhs, const registry_storage_iterator<Rhs> &rhs) noexcept {
    return lhs.it - rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator==(const registry_storage_iterator<Lhs> &lhs, const registry_storage_iterator<Rhs> &rhs) noexcept {
    return lhs.it == rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator!=(const registry_storage_iterator<Lhs> &lhs, const registry_storage_iterator<Rhs> &rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator<(const registry_storage_iterator<Lhs> &lhs, const registry_storage_iterator<Rhs> &rhs) noexcept {
    return lhs.it < rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator>(const registry_storage_iterator<Lhs> &lhs, const registry_storage_iterator<Rhs> &rhs) noexcept {
    return rhs < lhs;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator<=(const registry_storage_iterator<Lhs> &lhs, const registry_storage_iterator<Rhs> &rhs) noexcept {
    return !(lhs > rhs);
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator>=(const registry_storage_iterator<Lhs> &lhs, const registry_storage_iterator<Rhs> &rhs) noexcept {
    return !(lhs < rhs);
}

template<typename Allocator>
class registry_context {
    using alloc_traits = std::allocator_traits<Allocator>;
    using allocator_type = typename alloc_traits::template rebind_alloc<std::pair<const id_type, basic_any<0u>>>;

public:
    explicit registry_context(const allocator_type &allocator)
        : ctx{allocator} {}

    template<typename Type, typename... Args>
    Type &emplace_as(const id_type id, Args &&...args) {
        return any_cast<Type &>(ctx.try_emplace(id, std::in_place_type<Type>, std::forward<Args>(args)...).first->second);
    }

    template<typename Type, typename... Args>
    Type &emplace(Args &&...args) {
        return emplace_as<Type>(type_id<Type>().hash(), std::forward<Args>(args)...);
    }

    template<typename Type>
    Type &insert_or_assign(const id_type id, Type &&value) {
        return any_cast<std::remove_cv_t<std::remove_reference_t<Type>> &>(ctx.insert_or_assign(id, std::forward<Type>(value)).first->second);
    }

    template<typename Type>
    Type &insert_or_assign(Type &&value) {
        return insert_or_assign(type_id<Type>().hash(), std::forward<Type>(value));
    }

    template<typename Type>
    bool erase(const id_type id = type_id<Type>().hash()) {
        const auto it = ctx.find(id);
        return it != ctx.end() && it->second.type() == type_id<Type>() ? (ctx.erase(it), true) : false;
    }

    template<typename Type>
    [[nodiscard]] const Type &get(const id_type id = type_id<Type>().hash()) const {
        return any_cast<const Type &>(ctx.at(id));
    }

    template<typename Type>
    [[nodiscard]] Type &get(const id_type id = type_id<Type>().hash()) {
        return any_cast<Type &>(ctx.at(id));
    }

    template<typename Type>
    [[nodiscard]] const Type *find(const id_type id = type_id<Type>().hash()) const {
        const auto it = ctx.find(id);
        return it != ctx.cend() ? any_cast<const Type>(&it->second) : nullptr;
    }

    template<typename Type>
    [[nodiscard]] Type *find(const id_type id = type_id<Type>().hash()) {
        const auto it = ctx.find(id);
        return it != ctx.end() ? any_cast<Type>(&it->second) : nullptr;
    }

    template<typename Type>
    [[nodiscard]] bool contains(const id_type id = type_id<Type>().hash()) const {
        const auto it = ctx.find(id);
        return it != ctx.cend() && it->second.type() == type_id<Type>();
    }

private:
    dense_map<id_type, basic_any<0u>, identity, std::equal_to<id_type>, allocator_type> ctx;
};

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Fast and reliable entity-component system.
 * @tparam Entity A valid entity type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Entity, typename Allocator>
class basic_registry {
    using base_type = basic_sparse_set<Entity, Allocator>;

    using alloc_traits = std::allocator_traits<Allocator>;
    static_assert(std::is_same_v<typename alloc_traits::value_type, Entity>, "Invalid value type");

    // std::shared_ptr because of its type erased allocator which is useful here
    using pool_container_type = dense_map<id_type, std::shared_ptr<base_type>, identity, std::equal_to<id_type>, typename alloc_traits::template rebind_alloc<std::pair<const id_type, std::shared_ptr<base_type>>>>;
    using group_container_type = dense_map<id_type, std::shared_ptr<internal::group_descriptor>, identity, std::equal_to<id_type>, typename alloc_traits::template rebind_alloc<std::pair<const id_type, std::shared_ptr<internal::group_descriptor>>>>;

    template<typename Type>
    [[nodiscard]] auto &assure([[maybe_unused]] const id_type id = type_hash<Type>::value()) {
        if constexpr(std::is_same_v<Type, entity_type>) {
            return entities;
        } else {
            static_assert(std::is_same_v<Type, std::decay_t<Type>>, "Non-decayed types not allowed");
            auto &cpool = pools[id];

            if(!cpool) {
                using storage_type = storage_for_type<Type>;
                using alloc_type = typename storage_type::allocator_type;

                if constexpr(std::is_same_v<Type, void> && !std::is_constructible_v<alloc_type, allocator_type>) {
                    // std::allocator<void> has no cross constructors (waiting for C++20)
                    cpool = std::allocate_shared<storage_type>(get_allocator(), alloc_type{});
                } else {
                    cpool = std::allocate_shared<storage_type>(get_allocator(), get_allocator());
                }

                cpool->bind(forward_as_any(*this));
            }

            ENTT_ASSERT(cpool->type() == type_id<Type>(), "Unexpected type");
            return static_cast<storage_for_type<Type> &>(*cpool);
        }
    }

    template<typename Type>
    [[nodiscard]] const auto *assure([[maybe_unused]] const id_type id = type_hash<Type>::value()) const {
        if constexpr(std::is_same_v<Type, entity_type>) {
            return &entities;
        } else {
            static_assert(std::is_same_v<Type, std::decay_t<Type>>, "Non-decayed types not allowed");

            if(const auto it = pools.find(id); it != pools.cend()) {
                ENTT_ASSERT(it->second->type() == type_id<Type>(), "Unexpected type");
                return static_cast<const storage_for_type<Type> *>(it->second.get());
            }

            return static_cast<const storage_for_type<Type> *>(nullptr);
        }
    }

    void rebind() {
        entities.bind(forward_as_any(*this));

        for(auto &&curr: pools) {
            curr.second->bind(forward_as_any(*this));
        }
    }

public:
    /*! @brief Entity traits. */
    using traits_type = typename base_type::traits_type;
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Underlying entity identifier. */
    using entity_type = typename traits_type::value_type;
    /*! @brief Underlying version type. */
    using version_type = typename traits_type::version_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Common type among all storage types. */
    using common_type = base_type;
    /*! @brief Context type. */
    using context = internal::registry_context<allocator_type>;

    /**
     * @copybrief storage_for
     * @tparam Type Storage value type, eventually const.
     */
    template<typename Type>
    using storage_for_type = typename storage_for<Type, Entity, typename alloc_traits::template rebind_alloc<std::remove_const_t<Type>>>::type;

    /*! @brief Default constructor. */
    basic_registry()
        : basic_registry{allocator_type{}} {}

    /**
     * @brief Constructs an empty registry with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit basic_registry(const allocator_type &allocator)
        : basic_registry{0u, allocator} {}

    /**
     * @brief Allocates enough memory upon construction to store `count` pools.
     * @param count The number of pools to allocate memory for.
     * @param allocator The allocator to use.
     */
    basic_registry(const size_type count, const allocator_type &allocator = allocator_type{})
        : vars{allocator},
          pools{allocator},
          groups{allocator},
          entities{allocator} {
        pools.reserve(count);
        rebind();
    }

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    basic_registry(basic_registry &&other) noexcept
        : vars{std::move(other.vars)},
          pools{std::move(other.pools)},
          groups{std::move(other.groups)},
          entities{std::move(other.entities)} {
        rebind();
    }

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This registry.
     */
    basic_registry &operator=(basic_registry &&other) noexcept {
        vars = std::move(other.vars);
        pools = std::move(other.pools);
        groups = std::move(other.groups);
        entities = std::move(other.entities);

        rebind();

        return *this;
    }

    /**
     * @brief Exchanges the contents with those of a given registry.
     * @param other Registry to exchange the content with.
     */
    void swap(basic_registry &other) {
        using std::swap;

        swap(vars, other.vars);
        swap(pools, other.pools);
        swap(groups, other.groups);
        swap(entities, other.entities);

        rebind();
        other.rebind();
    }

    /**
     * @brief Returns the associated allocator.
     * @return The associated allocator.
     */
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return pools.get_allocator();
    }

    /**
     * @brief Returns an iterable object to use to _visit_ a registry.
     *
     * The iterable object returns a pair that contains the name and a reference
     * to the current storage.
     *
     * @return An iterable object to use to _visit_ the registry.
     */
    [[nodiscard]] auto storage() noexcept {
        return iterable_adaptor{internal::registry_storage_iterator{pools.begin()}, internal::registry_storage_iterator{pools.end()}};
    }

    /*! @copydoc storage */
    [[nodiscard]] auto storage() const noexcept {
        return iterable_adaptor{internal::registry_storage_iterator{pools.cbegin()}, internal::registry_storage_iterator{pools.cend()}};
    }

    /**
     * @brief Finds the storage associated with a given name, if any.
     * @param id Name used to map the storage within the registry.
     * @return A pointer to the storage if it exists, a null pointer otherwise.
     */
    [[nodiscard]] common_type *storage(const id_type id) {
        return const_cast<common_type *>(std::as_const(*this).storage(id));
    }

    /**
     * @brief Finds the storage associated with a given name, if any.
     * @param id Name used to map the storage within the registry.
     * @return A pointer to the storage if it exists, a null pointer otherwise.
     */
    [[nodiscard]] const common_type *storage(const id_type id) const {
        const auto it = pools.find(id);
        return it == pools.cend() ? nullptr : it->second.get();
    }

    /**
     * @brief Returns the storage for a given component type.
     * @tparam Type Type of component of which to return the storage.
     * @param id Optional name used to map the storage within the registry.
     * @return The storage for the given component type.
     */
    template<typename Type>
    storage_for_type<Type> &storage(const id_type id = type_hash<Type>::value()) {
        return assure<Type>(id);
    }

    /**
     * @brief Returns the storage for a given component type, if any.
     * @tparam Type Type of component of which to return the storage.
     * @param id Optional name used to map the storage within the registry.
     * @return The storage for the given component type.
     */
    template<typename Type>
    const storage_for_type<Type> *storage(const id_type id = type_hash<Type>::value()) const {
        return assure<Type>(id);
    }

    /**
     * @brief Returns the number of entities created so far.
     * @return Number of entities created so far.
     */
    [[deprecated("use .storage<Entity>().size() instead")]] [[nodiscard]] size_type size() const noexcept {
        return entities.size();
    }

    /**
     * @brief Returns the number of entities still in use.
     * @return Number of entities still in use.
     */
    [[deprecated("use .storage<Entity>().in_use() instead")]] [[nodiscard]] size_type alive() const {
        return entities.in_use();
    }

    /**
     * @brief Increases the capacity (number of entities) of the registry.
     * @param cap Desired capacity.
     */
    [[deprecated("use .storage<Entity>().reserve(cap) instead")]] void reserve(const size_type cap) {
        entities.reserve(cap);
    }

    /**
     * @brief Returns the number of entities that a registry has currently
     * allocated space for.
     * @return Capacity of the registry.
     */
    [[deprecated("use .storage<Entity>().capacity() instead")]] [[nodiscard]] size_type capacity() const noexcept {
        return entities.capacity();
    }

    /**
     * @brief Checks whether the registry is empty (no entities still in use).
     * @return True if the registry is empty, false otherwise.
     */
    [[deprecated("use .storage<Entity>().in_use() instead")]] [[nodiscard]] bool empty() const {
        return !alive();
    }

    /**
     * @brief Direct access to the list of entities of a registry.
     *
     * The returned pointer is such that range `[data(), data() + size())` is
     * always a valid range, even if the registry is empty.
     *
     * @warning
     * This list contains both valid and destroyed entities and isn't suitable
     * for direct use.
     *
     * @return A pointer to the array of entities.
     */
    [[deprecated("use .storage<Entity>().data() instead")]] [[nodiscard]] const entity_type *data() const noexcept {
        return entities.data();
    }

    /**
     * @brief Returns the number of released entities.
     * @return The number of released entities.
     */
    [[deprecated("use .storage<Entity>().size() and .storage<Entity>().in_use() instead")]] [[nodiscard]] size_type released() const noexcept {
        return (entities.size() - entities.in_use());
    }

    /**
     * @brief Checks if an identifier refers to a valid entity.
     * @param entt An identifier, either valid or not.
     * @return True if the identifier is valid, false otherwise.
     */
    [[nodiscard]] bool valid(const entity_type entt) const {
        return entities.contains(entt) && (entities.index(entt) < entities.in_use());
    }

    /**
     * @brief Returns the actual version for an identifier.
     * @param entt A valid identifier.
     * @return The version for the given identifier if valid, the tombstone
     * version otherwise.
     */
    [[nodiscard]] version_type current(const entity_type entt) const {
        return entities.current(entt);
    }

    /**
     * @brief Creates a new entity or recycles a destroyed one.
     * @return A valid identifier.
     */
    [[nodiscard]] entity_type create() {
        return entities.emplace();
    }

    /**
     * @copybrief create
     *
     * If the requested entity isn't in use, the suggested identifier is used.
     * Otherwise, a new identifier is generated.
     *
     * @param hint Required identifier.
     * @return A valid identifier.
     */
    [[nodiscard]] entity_type create(const entity_type hint) {
        return entities.emplace(hint);
    }

    /**
     * @brief Assigns each element in a range an identifier.
     *
     * @sa create
     *
     * @tparam It Type of forward iterator.
     * @param first An iterator to the first element of the range to generate.
     * @param last An iterator past the last element of the range to generate.
     */
    template<typename It>
    void create(It first, It last) {
        entities.insert(std::move(first), std::move(last));
    }

    /**
     * @brief Assigns identifiers to an empty registry.
     *
     * This function is intended for use in conjunction with `data`, `size` and
     * `released`.<br/>
     * Don't try to inject ranges of randomly generated entities nor the _wrong_
     * head for the list of destroyed entities. There is no guarantee that a
     * registry will continue to work properly in this case.
     *
     * @warning
     * There must be no entities still alive for this to work properly.
     *
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     * @param destroyed The number of released entities.
     */
    template<typename It>
    [[deprecated("use .storage<Entity>().push(first, last) and .storage<Entity>().in_use(len) instead")]] void assign(It first, It last, const size_type destroyed) {
        ENTT_ASSERT(!entities.in_use(), "Non-empty registry");
        entities.push(first, last);
        entities.in_use(entities.size() - destroyed);
    }

    /**
     * @brief Releases an identifier.
     *
     * The version is updated and the identifier can be recycled at any time.
     *
     * @param entt A valid identifier.
     * @return The version of the recycled entity.
     */
    [[deprecated("use .orphan(entt) and .storage<Entity>().erase(entt) instead")]] version_type release(const entity_type entt) {
        ENTT_ASSERT(orphan(entt), "Non-orphan entity");
        entities.erase(entt);
        return entities.current(entt);
    }

    /**
     * @brief Releases an identifier.
     *
     * The suggested version or the valid version closest to the suggested one
     * is used instead of the implicitly generated version.
     *
     * @param entt A valid identifier.
     * @param version A desired version upon destruction.
     * @return The version actually assigned to the entity.
     */
    [[deprecated("use .orphan(entt), then .storage<Entity>().erase(entt)/.bump(next) instead")]] version_type release(const entity_type entt, const version_type version) {
        ENTT_ASSERT(orphan(entt), "Non-orphan entity");
        entities.erase(entt);
        const auto elem = traits_type::construct(traits_type::to_entity(entt), version);
        return entities.bump((elem == tombstone) ? traits_type::next(elem) : elem);
    }

    /**
     * @brief Releases all identifiers in a range.
     *
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     */
    template<typename It>
    [[deprecated("use .orphan(entt) and .storage<Entity>().erase(first, last) instead")]] void release(It first, It last) {
        ENTT_ASSERT(std::all_of(first, last, [this](const auto entt) { return orphan(entt); }), "Non-orphan entity");
        entities.erase(std::move(first), std::move(last));
    }

    /**
     * @brief Destroys an entity and releases its identifier.
     *
     * @warning
     * Adding or removing components to an entity that is being destroyed can
     * result in undefined behavior.
     *
     * @param entt A valid identifier.
     * @return The version of the recycled entity.
     */
    version_type destroy(const entity_type entt) {
        for(size_type pos = pools.size(); pos; --pos) {
            pools.begin()[pos - 1u].second->remove(entt);
        }

        entities.erase(entt);
        return entities.current(entt);
    }

    /**
     * @brief Destroys an entity and releases its identifier.
     *
     * The suggested version or the valid version closest to the suggested one
     * is used instead of the implicitly generated version.
     *
     * @sa destroy
     *
     * @param entt A valid identifier.
     * @param version A desired version upon destruction.
     * @return The version actually assigned to the entity.
     */
    version_type destroy(const entity_type entt, const version_type version) {
        destroy(entt);
        const auto elem = traits_type::construct(traits_type::to_entity(entt), version);
        return entities.bump((elem == tombstone) ? traits_type::next(elem) : elem);
    }

    /**
     * @brief Destroys all entities in a range and releases their identifiers.
     *
     * @sa destroy
     *
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     */
    template<typename It>
    void destroy(It first, It last) {
        const auto from = entities.each().cbegin().base();
        const auto to = from + entities.pack(first, last);

        for(size_type pos = pools.size(); pos; --pos) {
            pools.begin()[pos - 1u].second->remove(from, to);
        }

        entities.erase(from, to);
    }

    /**
     * @brief Assigns the given component to an entity.
     *
     * The component must have a proper constructor or be of aggregate type.
     *
     * @warning
     * Attempting to assign a component to an entity that already owns it
     * results in undefined behavior.
     *
     * @tparam Type Type of component to create.
     * @tparam Args Types of arguments to use to construct the component.
     * @param entt A valid identifier.
     * @param args Parameters to use to initialize the component.
     * @return A reference to the newly created component.
     */
    template<typename Type, typename... Args>
    decltype(auto) emplace(const entity_type entt, Args &&...args) {
        return assure<Type>().emplace(entt, std::forward<Args>(args)...);
    }

    /**
     * @brief Assigns each entity in a range the given component.
     *
     * @sa emplace
     *
     * @tparam Type Type of component to create.
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     * @param value An instance of the component to assign.
     */
    template<typename Type, typename It>
    void insert(It first, It last, const Type &value = {}) {
        assure<Type>().insert(std::move(first), std::move(last), value);
    }

    /**
     * @brief Assigns each entity in a range the given components.
     *
     * @sa emplace
     *
     * @tparam Type Type of component to create.
     * @tparam EIt Type of input iterator.
     * @tparam CIt Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     * @param from An iterator to the first element of the range of components.
     */
    template<typename Type, typename EIt, typename CIt, typename = std::enable_if_t<std::is_same_v<typename std::iterator_traits<CIt>::value_type, Type>>>
    void insert(EIt first, EIt last, CIt from) {
        assure<Type>().insert(first, last, from);
    }

    /**
     * @brief Assigns or replaces the given component for an entity.
     *
     * @sa emplace
     * @sa replace
     *
     * @tparam Type Type of component to assign or replace.
     * @tparam Args Types of arguments to use to construct the component.
     * @param entt A valid identifier.
     * @param args Parameters to use to initialize the component.
     * @return A reference to the newly created component.
     */
    template<typename Type, typename... Args>
    decltype(auto) emplace_or_replace(const entity_type entt, Args &&...args) {
        if(auto &cpool = assure<Type>(); cpool.contains(entt)) {
            return cpool.patch(entt, [&args...](auto &...curr) { ((curr = Type{std::forward<Args>(args)...}), ...); });
        } else {
            return cpool.emplace(entt, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Patches the given component for an entity.
     *
     * The signature of the function should be equivalent to the following:
     *
     * @code{.cpp}
     * void(Type &);
     * @endcode
     *
     * @warning
     * Attempting to patch a component of an entity that doesn't own it
     * results in undefined behavior.
     *
     * @tparam Type Type of component to patch.
     * @tparam Func Types of the function objects to invoke.
     * @param entt A valid identifier.
     * @param func Valid function objects.
     * @return A reference to the patched component.
     */
    template<typename Type, typename... Func>
    decltype(auto) patch(const entity_type entt, Func &&...func) {
        return assure<Type>().patch(entt, std::forward<Func>(func)...);
    }

    /**
     * @brief Replaces the given component for an entity.
     *
     * The component must have a proper constructor or be of aggregate type.
     *
     * @warning
     * Attempting to replace a component of an entity that doesn't own it
     * results in undefined behavior.
     *
     * @tparam Type Type of component to replace.
     * @tparam Args Types of arguments to use to construct the component.
     * @param entt A valid identifier.
     * @param args Parameters to use to initialize the component.
     * @return A reference to the component being replaced.
     */
    template<typename Type, typename... Args>
    decltype(auto) replace(const entity_type entt, Args &&...args) {
        return patch<Type>(entt, [&args...](auto &...curr) { ((curr = Type{std::forward<Args>(args)...}), ...); });
    }

    /**
     * @brief Removes the given components from an entity.
     * @tparam Type Type of component to remove.
     * @tparam Other Other types of components to remove.
     * @param entt A valid identifier.
     * @return The number of components actually removed.
     */
    template<typename Type, typename... Other>
    size_type remove(const entity_type entt) {
        return (assure<Type>().remove(entt) + ... + assure<Other>().remove(entt));
    }

    /**
     * @brief Removes the given components from all the entities in a range.
     *
     * @sa remove
     *
     * @tparam Type Type of component to remove.
     * @tparam Other Other types of components to remove.
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     * @return The number of components actually removed.
     */
    template<typename Type, typename... Other, typename It>
    size_type remove(It first, It last) {
        size_type count{};

        if constexpr(std::is_same_v<It, typename common_type::iterator>) {
            common_type *cpools[sizeof...(Other) + 1u]{&assure<Type>(), &assure<Other>()...};

            for(size_type pos{}, len = sizeof...(Other) + 1u; pos < len; ++pos) {
                if constexpr(sizeof...(Other) != 0u) {
                    if(cpools[pos]->data() == first.data()) {
                        std::swap(cpools[pos], cpools[sizeof...(Other)]);
                    }
                }

                count += cpools[pos]->remove(first, last);
            }
        } else {
            for(auto cpools = std::forward_as_tuple(assure<Type>(), assure<Other>()...); first != last; ++first) {
                count += std::apply([entt = *first](auto &...curr) { return (curr.remove(entt) + ... + 0u); }, cpools);
            }
        }

        return count;
    }

    /**
     * @brief Erases the given components from an entity.
     *
     * @warning
     * Attempting to erase a component from an entity that doesn't own it
     * results in undefined behavior.
     *
     * @tparam Type Types of components to erase.
     * @tparam Other Other types of components to erase.
     * @param entt A valid identifier.
     */
    template<typename Type, typename... Other>
    void erase(const entity_type entt) {
        (assure<Type>().erase(entt), (assure<Other>().erase(entt), ...));
    }

    /**
     * @brief Erases the given components from all the entities in a range.
     *
     * @sa erase
     *
     * @tparam Type Types of components to erase.
     * @tparam Other Other types of components to erase.
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     */
    template<typename Type, typename... Other, typename It>
    void erase(It first, It last) {
        if constexpr(std::is_same_v<It, typename common_type::iterator>) {
            common_type *cpools[sizeof...(Other) + 1u]{&assure<Type>(), &assure<Other>()...};

            for(size_type pos{}, len = sizeof...(Other) + 1u; pos < len; ++pos) {
                if constexpr(sizeof...(Other) != 0u) {
                    if(cpools[pos]->data() == first.data()) {
                        std::swap(cpools[pos], cpools[sizeof...(Other)]);
                    }
                }

                cpools[pos]->erase(first, last);
            }
        } else {
            for(auto cpools = std::forward_as_tuple(assure<Type>(), assure<Other>()...); first != last; ++first) {
                std::apply([entt = *first](auto &...curr) { (curr.erase(entt), ...); }, cpools);
            }
        }
    }

    /**
     * @brief Erases components satisfying specific criteria from an entity.
     *
     * The function type is equivalent to:
     *
     * @code{.cpp}
     * void(const id_type, typename basic_registry<Entity>::base_type &);
     * @endcode
     *
     * Only storage where the entity exists are passed to the function.
     *
     * @tparam Func Type of the function object to invoke.
     * @param entt A valid identifier.
     * @param func A valid function object.
     */
    template<typename Func>
    void erase_if(const entity_type entt, Func func) {
        for(auto [id, cpool]: storage()) {
            if(cpool.contains(entt) && func(id, std::as_const(cpool))) {
                cpool.erase(entt);
            }
        }
    }

    /**
     * @brief Removes all tombstones from a registry or only the pools for the
     * given components.
     * @tparam Type Types of components for which to clear all tombstones.
     */
    template<typename... Type>
    void compact() {
        if constexpr(sizeof...(Type) == 0u) {
            for(auto &&curr: pools) {
                curr.second->compact();
            }
        } else {
            (assure<Type>().compact(), ...);
        }
    }

    /**
     * @brief Check if an entity is part of all the given storage.
     * @tparam Type Type of storage to check for.
     * @param entt A valid identifier.
     * @return True if the entity is part of all the storage, false otherwise.
     */
    template<typename... Type>
    [[nodiscard]] bool all_of(const entity_type entt) const {
        if constexpr(sizeof...(Type) == 1u) {
            auto *cpool = assure<std::remove_const_t<Type>...>();
            return cpool && cpool->contains(entt);
        } else {
            return (all_of<Type>(entt) && ...);
        }
    }

    /**
     * @brief Check if an entity is part of at least one given storage.
     * @tparam Type Type of storage to check for.
     * @param entt A valid identifier.
     * @return True if the entity is part of at least one storage, false
     * otherwise.
     */
    template<typename... Type>
    [[nodiscard]] bool any_of(const entity_type entt) const {
        return (all_of<Type>(entt) || ...);
    }

    /**
     * @brief Returns references to the given components for an entity.
     *
     * @warning
     * Attempting to get a component from an entity that doesn't own it results
     * in undefined behavior.
     *
     * @tparam Type Types of components to get.
     * @param entt A valid identifier.
     * @return References to the components owned by the entity.
     */
    template<typename... Type>
    [[nodiscard]] decltype(auto) get([[maybe_unused]] const entity_type entt) const {
        if constexpr(sizeof...(Type) == 1u) {
            return (assure<std::remove_const_t<Type>>()->get(entt), ...);
        } else {
            return std::forward_as_tuple(get<Type>(entt)...);
        }
    }

    /*! @copydoc get */
    template<typename... Type>
    [[nodiscard]] decltype(auto) get([[maybe_unused]] const entity_type entt) {
        if constexpr(sizeof...(Type) == 1u) {
            return (static_cast<storage_for_type<Type> &>(assure<std::remove_const_t<Type>>()).get(entt), ...);
        } else {
            return std::forward_as_tuple(get<Type>(entt)...);
        }
    }

    /**
     * @brief Returns a reference to the given component for an entity.
     *
     * In case the entity doesn't own the component, the parameters provided are
     * used to construct it.
     *
     * @sa get
     * @sa emplace
     *
     * @tparam Type Type of component to get.
     * @tparam Args Types of arguments to use to construct the component.
     * @param entt A valid identifier.
     * @param args Parameters to use to initialize the component.
     * @return Reference to the component owned by the entity.
     */
    template<typename Type, typename... Args>
    [[nodiscard]] decltype(auto) get_or_emplace(const entity_type entt, Args &&...args) {
        if(auto &cpool = assure<Type>(); cpool.contains(entt)) {
            return cpool.get(entt);
        } else {
            return cpool.emplace(entt, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Returns pointers to the given components for an entity.
     *
     * @note
     * The registry retains ownership of the pointed-to components.
     *
     * @tparam Type Types of components to get.
     * @param entt A valid identifier.
     * @return Pointers to the components owned by the entity.
     */
    template<typename... Type>
    [[nodiscard]] auto try_get([[maybe_unused]] const entity_type entt) const {
        if constexpr(sizeof...(Type) == 1u) {
            const auto *cpool = assure<std::remove_const_t<Type>...>();
            return (cpool && cpool->contains(entt)) ? std::addressof(cpool->get(entt)) : nullptr;
        } else {
            return std::make_tuple(try_get<Type>(entt)...);
        }
    }

    /*! @copydoc try_get */
    template<typename... Type>
    [[nodiscard]] auto try_get([[maybe_unused]] const entity_type entt) {
        if constexpr(sizeof...(Type) == 1u) {
            auto &cpool = assure<std::remove_const_t<Type>...>();
            return (static_cast<Type *>(cpool.contains(entt) ? std::addressof(cpool.get(entt)) : nullptr), ...);
        } else {
            return std::make_tuple(try_get<Type>(entt)...);
        }
    }

    /**
     * @brief Clears a whole registry or the pools for the given components.
     * @tparam Type Types of components to remove from their entities.
     */
    template<typename... Type>
    void clear() {
        if constexpr(sizeof...(Type) == 0u) {
            for(size_type pos = pools.size(); pos; --pos) {
                pools.begin()[pos - 1u].second->clear();
            }

            const auto iterable = entities.each();
            entities.erase(iterable.begin().base(), iterable.end().base());
        } else {
            (assure<Type>().clear(), ...);
        }
    }

    /**
     * @brief Iterates all the entities that are still in use.
     *
     * The signature of the function should be equivalent to the following:
     *
     * @code{.cpp}
     * void(const Entity);
     * @endcode
     *
     * It's not defined whether entities created during iteration are returned.
     *
     * @tparam Func Type of the function object to invoke.
     * @param func A valid function object.
     */
    template<typename Func>
    [[deprecated("use .storage<Entity>().each() instead")]] void each(Func func) const {
        for(auto [entt]: entities.each()) {
            func(entt);
        }
    }

    /**
     * @brief Checks if an entity has components assigned.
     * @param entt A valid identifier.
     * @return True if the entity has no components assigned, false otherwise.
     */
    [[nodiscard]] bool orphan(const entity_type entt) const {
        return std::none_of(pools.cbegin(), pools.cend(), [entt](auto &&curr) { return curr.second->contains(entt); });
    }

    /**
     * @brief Returns a sink object for the given component.
     *
     * Use this function to receive notifications whenever a new instance of the
     * given component is created and assigned to an entity.<br/>
     * The function type for a listener is equivalent to:
     *
     * @code{.cpp}
     * void(basic_registry<Entity> &, Entity);
     * @endcode
     *
     * Listeners are invoked **after** assigning the component to the entity.
     *
     * @sa sink
     *
     * @tparam Type Type of component of which to get the sink.
     * @param id Optional name used to map the storage within the registry.
     * @return A temporary sink object.
     */
    template<typename Type>
    [[nodiscard]] auto on_construct(const id_type id = type_hash<Type>::value()) {
        return assure<Type>(id).on_construct();
    }

    /**
     * @brief Returns a sink object for the given component.
     *
     * Use this function to receive notifications whenever an instance of the
     * given component is explicitly updated.<br/>
     * The function type for a listener is equivalent to:
     *
     * @code{.cpp}
     * void(basic_registry<Entity> &, Entity);
     * @endcode
     *
     * Listeners are invoked **after** updating the component.
     *
     * @sa sink
     *
     * @tparam Type Type of component of which to get the sink.
     * @param id Optional name used to map the storage within the registry.
     * @return A temporary sink object.
     */
    template<typename Type>
    [[nodiscard]] auto on_update(const id_type id = type_hash<Type>::value()) {
        return assure<Type>(id).on_update();
    }

    /**
     * @brief Returns a sink object for the given component.
     *
     * Use this function to receive notifications whenever an instance of the
     * given component is removed from an entity and thus destroyed.<br/>
     * The function type for a listener is equivalent to:
     *
     * @code{.cpp}
     * void(basic_registry<Entity> &, Entity);
     * @endcode
     *
     * Listeners are invoked **before** removing the component from the entity.
     *
     * @sa sink
     *
     * @tparam Type Type of component of which to get the sink.
     * @param id Optional name used to map the storage within the registry.
     * @return A temporary sink object.
     */
    template<typename Type>
    [[nodiscard]] auto on_destroy(const id_type id = type_hash<Type>::value()) {
        return assure<Type>(id).on_destroy();
    }

    /**
     * @brief Returns a view for the given components.
     * @tparam Type Type of component used to construct the view.
     * @tparam Other Other types of components used to construct the view.
     * @tparam Exclude Types of components used to filter the view.
     * @return A newly created view.
     */
    template<typename Type, typename... Other, typename... Exclude>
    [[nodiscard]] basic_view<get_t<storage_for_type<const Type>, storage_for_type<const Other>...>, exclude_t<storage_for_type<const Exclude>...>>
    view(exclude_t<Exclude...> = exclude_t{}) const {
        const auto cpools = std::make_tuple(assure<std::remove_const_t<Type>>(), assure<std::remove_const_t<Other>>()..., assure<std::remove_const_t<Exclude>>()...);
        basic_view<get_t<storage_for_type<const Type>, storage_for_type<const Other>...>, exclude_t<storage_for_type<const Exclude>...>> elem{};
        std::apply([&elem](const auto *...curr) { ((curr ? elem.storage(*curr) : void()), ...); }, cpools);
        return elem;
    }

    /*! @copydoc view */
    template<typename Type, typename... Other, typename... Exclude>
    [[nodiscard]] basic_view<get_t<storage_for_type<Type>, storage_for_type<Other>...>, exclude_t<storage_for_type<Exclude>...>>
    view(exclude_t<Exclude...> = exclude_t{}) {
        return {assure<std::remove_const_t<Type>>(), assure<std::remove_const_t<Other>>()..., assure<std::remove_const_t<Exclude>>()...};
    }

    /**
     * @brief Returns a group for the given components.
     * @tparam Owned Types of storage _owned_ by the group.
     * @tparam Get Types of storage _observed_ by the group, if any.
     * @tparam Exclude Types of storage used to filter the group, if any.
     * @return A newly created group.
     */
    template<typename... Owned, typename... Get, typename... Exclude>
    basic_group<owned_t<storage_for_type<Owned>...>, get_t<storage_for_type<Get>...>, exclude_t<storage_for_type<Exclude>...>>
    group(get_t<Get...> = get_t{}, exclude_t<Exclude...> = exclude_t{}) {
        using handler_type = typename basic_group<owned_t<storage_for_type<Owned>...>, get_t<storage_for_type<Get>...>, exclude_t<storage_for_type<Exclude>...>>::handler;

        if(auto it = groups.find(type_hash<handler_type>::value()); it != groups.cend()) {
            return {*std::static_pointer_cast<handler_type>(it->second)};
        }

        std::shared_ptr<handler_type> handler{};

        if constexpr(sizeof...(Owned) == 0u) {
            handler = std::allocate_shared<handler_type>(get_allocator(), get_allocator(), assure<std::remove_const_t<Get>>()..., assure<std::remove_const_t<Exclude>>()...);
        } else {
            handler = std::allocate_shared<handler_type>(get_allocator(), assure<std::remove_const_t<Owned>>()..., assure<std::remove_const_t<Get>>()..., assure<std::remove_const_t<Exclude>>()...);
            [[maybe_unused]] const id_type elem[]{type_hash<std::remove_const_t<Owned>>::value()..., type_hash<std::remove_const_t<Get>>::value()..., type_hash<std::remove_const_t<Exclude>>::value()...};
            ENTT_ASSERT(std::all_of(groups.cbegin(), groups.cend(), [&elem](const auto &data) { return data.second->owned(elem, sizeof...(Owned)) == 0u; }), "Conflicting groups");
        }

        groups.emplace(type_hash<handler_type>::value(), handler);
        return {*handler};
    }

    /*! @copydoc group */
    template<typename... Owned, typename... Get, typename... Exclude>
    basic_group<owned_t<storage_for_type<const Owned>...>, get_t<storage_for_type<const Get>...>, exclude_t<storage_for_type<const Exclude>...>>
    group_if_exists(get_t<Get...> = get_t{}, exclude_t<Exclude...> = exclude_t{}) const {
        using handler_type = typename basic_group<owned_t<storage_for_type<const Owned>...>, get_t<storage_for_type<const Get>...>, exclude_t<storage_for_type<const Exclude>...>>::handler;

        if(auto it = groups.find(type_hash<handler_type>::value()); it != groups.cend()) {
            return {*std::static_pointer_cast<handler_type>(it->second)};
        }

        return {};
    }

    /**
     * @brief Checks whether the given components belong to any group.
     * @tparam Type Type of component in which one is interested.
     * @tparam Other Other types of components in which one is interested.
     * @return True if the pools of the given components are _free_, false
     * otherwise.
     */
    template<typename Type, typename... Other>
    [[nodiscard]] bool owned() const {
        const id_type elem[]{type_hash<std::remove_const_t<Type>>::value(), type_hash<std::remove_const_t<Other>>::value()...};
        return std::any_of(groups.cbegin(), groups.cend(), [&elem](auto &&data) { return data.second->owned(elem, 1u + sizeof...(Other)); });
    }

    /**
     * @brief Sorts the elements of a given component.
     *
     * The comparison function object returns `true` if the first element is
     * _less_ than the second one, `false` otherwise. Its signature is also
     * equivalent to one of the following:
     *
     * @code{.cpp}
     * bool(const Entity, const Entity);
     * bool(const Type &, const Type &);
     * @endcode
     *
     * Moreover, it shall induce a _strict weak ordering_ on the values.<br/>
     * The sort function object offers an `operator()` that accepts:
     *
     * * An iterator to the first element of the range to sort.
     * * An iterator past the last element of the range to sort.
     * * A comparison function object to use to compare the elements.
     *
     * The comparison function object hasn't necessarily the type of the one
     * passed along with the other parameters to this member function.
     *
     * @warning
     * Pools of components owned by a group cannot be sorted.
     *
     * @tparam Type Type of components to sort.
     * @tparam Compare Type of comparison function object.
     * @tparam Sort Type of sort function object.
     * @tparam Args Types of arguments to forward to the sort function object.
     * @param compare A valid comparison function object.
     * @param algo A valid sort function object.
     * @param args Arguments to forward to the sort function object, if any.
     */
    template<typename Type, typename Compare, typename Sort = std_sort, typename... Args>
    void sort(Compare compare, Sort algo = Sort{}, Args &&...args) {
        ENTT_ASSERT(!owned<Type>(), "Cannot sort owned storage");
        auto &cpool = assure<Type>();

        if constexpr(std::is_invocable_v<Compare, decltype(cpool.get({})), decltype(cpool.get({}))>) {
            auto comp = [&cpool, compare = std::move(compare)](const auto lhs, const auto rhs) { return compare(std::as_const(cpool.get(lhs)), std::as_const(cpool.get(rhs))); };
            cpool.sort(std::move(comp), std::move(algo), std::forward<Args>(args)...);
        } else {
            cpool.sort(std::move(compare), std::move(algo), std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Sorts two pools of components in the same way.
     *
     * Entities and components in `To` which are part of both storage are sorted
     * internally with the order they have in `From`. The others follow in no
     * particular order.
     *
     * @warning
     * Pools of components owned by a group cannot be sorted.
     *
     * @tparam To Type of components to sort.
     * @tparam From Type of components to use to sort.
     */
    template<typename To, typename From>
    void sort() {
        ENTT_ASSERT(!owned<To>(), "Cannot sort owned storage");
        assure<To>().sort_as(assure<From>());
    }

    /**
     * @brief Returns the context object, that is, a general purpose container.
     * @return The context object, that is, a general purpose container.
     */
    context &ctx() noexcept {
        return vars;
    }

    /*! @copydoc ctx */
    const context &ctx() const noexcept {
        return vars;
    }

private:
    context vars;
    pool_container_type pools;
    group_container_type groups;
    storage_for_type<entity_type> entities;
};

} // namespace entt

#endif

/////////////////// END OF external/entt/entity/registry.hpp ///////////////////

/////////////////// START OF external/entt/entity/handle.hpp ///////////////////

#ifndef ENTT_ENTITY_HANDLE_HPP
#define ENTT_ENTITY_HANDLE_HPP

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename It>
class handle_storage_iterator final {
    template<typename Other>
    friend class handle_storage_iterator;

    using underlying_type = std::remove_reference_t<typename It::value_type::second_type>;
    using entity_type = typename underlying_type::entity_type;

public:
    using value_type = typename std::iterator_traits<It>::value_type;
    using pointer = input_iterator_pointer<value_type>;
    using reference = value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    constexpr handle_storage_iterator() noexcept
        : entt{null},
          it{},
          last{} {}

    constexpr handle_storage_iterator(entity_type value, It from, It to) noexcept
        : entt{value},
          it{from},
          last{to} {
        while(it != last && !it->second.contains(entt)) {
            ++it;
        }
    }

    constexpr handle_storage_iterator &operator++() noexcept {
        while(++it != last && !it->second.contains(entt)) {}
        return *this;
    }

    constexpr handle_storage_iterator operator++(int) noexcept {
        handle_storage_iterator orig = *this;
        return ++(*this), orig;
    }

    [[nodiscard]] constexpr reference operator*() const noexcept {
        return *it;
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept {
        return operator*();
    }

    template<typename ILhs, typename IRhs>
    friend constexpr bool operator==(const handle_storage_iterator<ILhs> &, const handle_storage_iterator<IRhs> &) noexcept;

private:
    entity_type entt;
    It it;
    It last;
};

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator==(const handle_storage_iterator<ILhs> &lhs, const handle_storage_iterator<IRhs> &rhs) noexcept {
    return lhs.it == rhs.it;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator!=(const handle_storage_iterator<ILhs> &lhs, const handle_storage_iterator<IRhs> &rhs) noexcept {
    return !(lhs == rhs);
}

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Non-owning handle to an entity.
 *
 * Tiny wrapper around a registry and an entity.
 *
 * @tparam Registry Basic registry type.
 * @tparam Scope Types to which to restrict the scope of a handle.
 */
template<typename Registry, typename... Scope>
struct basic_handle {
    /*! @brief Type of registry accepted by the handle. */
    using registry_type = Registry;
    /*! @brief Underlying entity identifier. */
    using entity_type = typename registry_type::entity_type;
    /*! @brief Underlying version type. */
    using version_type = typename registry_type::version_type;
    /*! @brief Unsigned integer type. */
    using size_type = typename registry_type::size_type;

    /*! @brief Constructs an invalid handle. */
    basic_handle() noexcept
        : reg{},
          entt{null} {}

    /**
     * @brief Constructs a handle from a given registry and entity.
     * @param ref An instance of the registry class.
     * @param value A valid identifier.
     */
    basic_handle(registry_type &ref, entity_type value) noexcept
        : reg{&ref},
          entt{value} {}

    /**
     * @brief Returns an iterable object to use to _visit_ a handle.
     *
     * The iterable object returns a pair that contains the name and a reference
     * to the current storage.<br/>
     * Returned storage are those that contain the entity associated with the
     * handle.
     *
     * @return An iterable object to use to _visit_ the handle.
     */
    [[nodiscard]] auto storage() const noexcept {
        auto iterable = reg->storage();
        using iterator_type = internal::handle_storage_iterator<typename decltype(iterable)::iterator>;
        return iterable_adaptor{iterator_type{entt, iterable.begin(), iterable.end()}, iterator_type{entt, iterable.end(), iterable.end()}};
    }

    /**
     * @brief Constructs a const handle from a non-const one.
     * @tparam Other A valid entity type.
     * @tparam Args Scope of the handle to construct.
     * @return A const handle referring to the same registry and the same
     * entity.
     */
    template<typename Other, typename... Args>
    operator basic_handle<Other, Args...>() const noexcept {
        static_assert(std::is_same_v<Other, Registry> || std::is_same_v<std::remove_const_t<Other>, Registry>, "Invalid conversion between different handles");
        static_assert((sizeof...(Scope) == 0 || ((sizeof...(Args) != 0 && sizeof...(Args) <= sizeof...(Scope)) && ... && (type_list_contains_v<type_list<Scope...>, Args>))), "Invalid conversion between different handles");

        return reg ? basic_handle<Other, Args...>{*reg, entt} : basic_handle<Other, Args...>{};
    }

    /**
     * @brief Converts a handle to its underlying entity.
     * @return The contained identifier.
     */
    [[nodiscard]] operator entity_type() const noexcept {
        return entity();
    }

    /**
     * @brief Checks if a handle refers to non-null registry pointer and entity.
     * @return True if the handle refers to non-null registry and entity, false otherwise.
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return reg && reg->valid(entt);
    }

    /**
     * @brief Checks if a handle refers to a valid entity or not.
     * @return True if the handle refers to a valid entity, false otherwise.
     */
    [[nodiscard]] bool valid() const {
        return reg->valid(entt);
    }

    /**
     * @brief Returns a pointer to the underlying registry, if any.
     * @return A pointer to the underlying registry, if any.
     */
    [[nodiscard]] registry_type *registry() const noexcept {
        return reg;
    }

    /**
     * @brief Returns the entity associated with a handle.
     * @return The entity associated with the handle.
     */
    [[nodiscard]] entity_type entity() const noexcept {
        return entt;
    }

    /*! @brief Destroys the entity associated with a handle. */
    void destroy() {
        reg->destroy(std::exchange(entt, null));
    }

    /**
     * @brief Destroys the entity associated with a handle.
     * @param version A desired version upon destruction.
     */
    void destroy(const version_type version) {
        reg->destroy(std::exchange(entt, null), version);
    }

    /**
     * @brief Assigns the given component to a handle.
     * @tparam Component Type of component to create.
     * @tparam Args Types of arguments to use to construct the component.
     * @param args Parameters to use to initialize the component.
     * @return A reference to the newly created component.
     */
    template<typename Component, typename... Args>
    decltype(auto) emplace(Args &&...args) const {
        static_assert(((sizeof...(Scope) == 0) || ... || std::is_same_v<Component, Scope>), "Invalid type");
        return reg->template emplace<Component>(entt, std::forward<Args>(args)...);
    }

    /**
     * @brief Assigns or replaces the given component for a handle.
     * @tparam Component Type of component to assign or replace.
     * @tparam Args Types of arguments to use to construct the component.
     * @param args Parameters to use to initialize the component.
     * @return A reference to the newly created component.
     */
    template<typename Component, typename... Args>
    decltype(auto) emplace_or_replace(Args &&...args) const {
        static_assert(((sizeof...(Scope) == 0) || ... || std::is_same_v<Component, Scope>), "Invalid type");
        return reg->template emplace_or_replace<Component>(entt, std::forward<Args>(args)...);
    }

    /**
     * @brief Patches the given component for a handle.
     * @tparam Component Type of component to patch.
     * @tparam Func Types of the function objects to invoke.
     * @param func Valid function objects.
     * @return A reference to the patched component.
     */
    template<typename Component, typename... Func>
    decltype(auto) patch(Func &&...func) const {
        static_assert(((sizeof...(Scope) == 0) || ... || std::is_same_v<Component, Scope>), "Invalid type");
        return reg->template patch<Component>(entt, std::forward<Func>(func)...);
    }

    /**
     * @brief Replaces the given component for a handle.
     * @tparam Component Type of component to replace.
     * @tparam Args Types of arguments to use to construct the component.
     * @param args Parameters to use to initialize the component.
     * @return A reference to the component being replaced.
     */
    template<typename Component, typename... Args>
    decltype(auto) replace(Args &&...args) const {
        static_assert(((sizeof...(Scope) == 0) || ... || std::is_same_v<Component, Scope>), "Invalid type");
        return reg->template replace<Component>(entt, std::forward<Args>(args)...);
    }

    /**
     * @brief Removes the given components from a handle.
     * @tparam Component Types of components to remove.
     * @return The number of components actually removed.
     */
    template<typename... Component>
    size_type remove() const {
        static_assert(sizeof...(Scope) == 0 || (type_list_contains_v<type_list<Scope...>, Component> && ...), "Invalid type");
        return reg->template remove<Component...>(entt);
    }

    /**
     * @brief Erases the given components from a handle.
     * @tparam Component Types of components to erase.
     */
    template<typename... Component>
    void erase() const {
        static_assert(sizeof...(Scope) == 0 || (type_list_contains_v<type_list<Scope...>, Component> && ...), "Invalid type");
        reg->template erase<Component...>(entt);
    }

    /**
     * @brief Checks if a handle has all the given components.
     * @tparam Component Components for which to perform the check.
     * @return True if the handle has all the components, false otherwise.
     */
    template<typename... Component>
    [[nodiscard]] decltype(auto) all_of() const {
        return reg->template all_of<Component...>(entt);
    }

    /**
     * @brief Checks if a handle has at least one of the given components.
     * @tparam Component Components for which to perform the check.
     * @return True if the handle has at least one of the given components,
     * false otherwise.
     */
    template<typename... Component>
    [[nodiscard]] decltype(auto) any_of() const {
        return reg->template any_of<Component...>(entt);
    }

    /**
     * @brief Returns references to the given components for a handle.
     * @tparam Component Types of components to get.
     * @return References to the components owned by the handle.
     */
    template<typename... Component>
    [[nodiscard]] decltype(auto) get() const {
        static_assert(sizeof...(Scope) == 0 || (type_list_contains_v<type_list<Scope...>, Component> && ...), "Invalid type");
        return reg->template get<Component...>(entt);
    }

    /**
     * @brief Returns a reference to the given component for a handle.
     * @tparam Component Type of component to get.
     * @tparam Args Types of arguments to use to construct the component.
     * @param args Parameters to use to initialize the component.
     * @return Reference to the component owned by the handle.
     */
    template<typename Component, typename... Args>
    [[nodiscard]] decltype(auto) get_or_emplace(Args &&...args) const {
        static_assert(((sizeof...(Scope) == 0) || ... || std::is_same_v<Component, Scope>), "Invalid type");
        return reg->template get_or_emplace<Component>(entt, std::forward<Args>(args)...);
    }

    /**
     * @brief Returns pointers to the given components for a handle.
     * @tparam Component Types of components to get.
     * @return Pointers to the components owned by the handle.
     */
    template<typename... Component>
    [[nodiscard]] auto try_get() const {
        static_assert(sizeof...(Scope) == 0 || (type_list_contains_v<type_list<Scope...>, Component> && ...), "Invalid type");
        return reg->template try_get<Component...>(entt);
    }

    /**
     * @brief Checks if a handle has components assigned.
     * @return True if the handle has no components assigned, false otherwise.
     */
    [[nodiscard]] bool orphan() const {
        return reg->orphan(entt);
    }

private:
    registry_type *reg;
    entity_type entt;
};

/**
 * @brief Compares two handles.
 * @tparam Args Scope of the first handle.
 * @tparam Other Scope of the second handle.
 * @param lhs A valid handle.
 * @param rhs A valid handle.
 * @return True if both handles refer to the same registry and the same
 * entity, false otherwise.
 */
template<typename... Args, typename... Other>
[[nodiscard]] bool operator==(const basic_handle<Args...> &lhs, const basic_handle<Other...> &rhs) noexcept {
    return lhs.registry() == rhs.registry() && lhs.entity() == rhs.entity();
}

/**
 * @brief Compares two handles.
 * @tparam Args Scope of the first handle.
 * @tparam Other Scope of the second handle.
 * @param lhs A valid handle.
 * @param rhs A valid handle.
 * @return False if both handles refer to the same registry and the same
 * entity, true otherwise.
 */
template<typename... Args, typename... Other>
[[nodiscard]] bool operator!=(const basic_handle<Args...> &lhs, const basic_handle<Other...> &rhs) noexcept {
    return !(lhs == rhs);
}

} // namespace entt

#endif

//////////////////// END OF external/entt/entity/handle.hpp ////////////////////

////////////////// START OF src/Types/Enums/PseudoWeather.hpp //////////////////

#include <cstdint>

namespace pokesim::dex {
enum class PseudoWeather : std::uint8_t {
  // clang-format off
  NO_PSEUDO_WEATHER = 0,
  // clang-format on
};
}  // namespace pokesim::dex

/////////////////// END OF src/Types/Enums/PseudoWeather.hpp ///////////////////

////////////////// START OF src/Types/Enums/SideCondition.hpp //////////////////

#include <cstdint>

namespace pokesim::dex {
enum class SideCondition : std::uint8_t {
  // clang-format off
  NO_SIDE_CONDITION = 0
  // clang-format on
};
}  // namespace pokesim::dex

/////////////////// END OF src/Types/Enums/SideCondition.hpp ///////////////////

///////////////////// START OF src/Types/Enums/Status.hpp //////////////////////

#include <cstdint>

namespace pokesim::dex {
// Pokemon status condition name
enum class Status : std::uint8_t {
  NO_STATUS = 0,
  BRN,
  FRZ,
  PAR,
  PSN,
  SLP,
  TOX,
  /*, FRB, DRO, */ STATUS_TOTAL,
};
}  // namespace pokesim::dex

////////////////////// END OF src/Types/Enums/Status.hpp ///////////////////////

///////////////////// START OF src/Types/Enums/Terrain.hpp /////////////////////

#include <cstdint>

namespace pokesim::dex {
enum class Terrain : std::uint8_t {
  // clang-format off
  NO_TERRAIN = 0
  // clang-format on
};
}  // namespace pokesim::dex

////////////////////// END OF src/Types/Enums/Terrain.hpp //////////////////////

//////////////////// START OF src/Types/Enums/Volatile.hpp /////////////////////

#include <cstdint>

namespace pokesim::dex {
enum class Volatile : std::uint8_t {
  // clang-format off
  NO_VOLATILE = 0
  // clang-format on
};
}  // namespace pokesim::dex

///////////////////// END OF src/Types/Enums/Volatile.hpp //////////////////////

///////////////////// START OF src/Types/Enums/Weather.hpp /////////////////////

#include <cstdint>

namespace pokesim::dex {
enum class Weather : std::uint8_t {
  // clang-format off
  NO_WEATHER = 0
  // clang-format on
};
}  // namespace pokesim::dex

////////////////////// END OF src/Types/Enums/Weather.hpp //////////////////////

////////////////////// START OF src/Utilities/Variant.hpp //////////////////////

#include <variant>

namespace pokesim::internal {
template <typename... Types>
class variant : public std::variant<Types...> {
 public:
  using std::variant<Types...>::variant;

  template <typename T>
  variant& operator=(T&& rhs) {
    std::variant<Types...>::operator=(rhs);
    return *this;
  }

  bool empty() const { return holds<std::monostate>(); }

  template <typename Type>
  bool holds() const {
    return std::holds_alternative<Type>(*this);
  }

  template <typename Type>
  auto& get() const {
    return std::get<Type>(*this);
  }

  template <typename Type>
  auto& get() {
    return std::get<Type>(*this);
  }
};
}  // namespace pokesim::internal

/////////////////////// END OF src/Utilities/Variant.hpp ///////////////////////

//////////////////////// START OF src/Types/Effect.hpp /////////////////////////

namespace pokesim::types {
using effectEnum = pokesim::internal::variant<
  std::monostate, dex::PseudoWeather, dex::SideCondition, dex::Status, dex::Terrain, dex::Volatile, dex::Weather>;
}  // namespace pokesim::types

///////////////////////// END OF src/Types/Effect.hpp //////////////////////////

//////////////////////// START OF src/Types/Entity.hpp /////////////////////////

namespace pokesim::types {
using registry = entt::registry;
}  // namespace pokesim::types

namespace pokesim::types {
template <typename T, typename... Other>
using view = entt::view<entt::get_t<const T, const Other...>>;

using handle = entt::basic_handle<registry>;

using entity = entt::entity;

using ClonedEntityMap = entt::dense_map<entity, std::vector<entity>>;
}  // namespace pokesim::types

///////////////////////// END OF src/Types/Entity.hpp //////////////////////////

///////// START OF src/AnalyzeEffect/Setup/AnalyzeEffectInputSetup.hpp /////////

namespace pokesim::analyze_effect {
struct InputSetup {
 protected:
  types::handle handle;

 public:
  InputSetup(types::registry& registry, types::entity entity) : handle(registry, entity) {}
  InputSetup(types::registry& registry) : InputSetup(registry, registry.create()) {}

  inline void setAttacker(types::entity entity);
  inline void setDefender(types::entity entity);
  inline void setEffect(types::effectEnum effect);
  inline void setBattle(types::entity entity);

  types::entity entity() { return handle.entity(); }
};
}  // namespace pokesim::analyze_effect

////////// END OF src/AnalyzeEffect/Setup/AnalyzeEffectInputSetup.hpp //////////

////////////////////// START OF src/Types/Enums/Move.hpp ///////////////////////

#include <cstdint>

namespace pokesim::dex {
// Pokemon move name
enum class Move : std::uint16_t {
  // clang-format off
  NO_MOVE = 0, ONE_MILLION_VOLT_THUNDERBOLT, ABSORB, ACCELEROCK, ACID, ACID_ARMOR, ACID_DOWNPOUR, ACID_SPRAY, ACROBATICS, ACUPRESSURE, AERIAL_ACE, AEROBLAST, AFTER_YOU, AGILITY, AIR_CUTTER, AIR_SLASH, ALL_OUT_PUMMELING, ALLY_SWITCH, AMNESIA, ANCHOR_SHOT, ANCIENT_POWER, APPLE_ACID, AQUA_CUTTER, AQUA_JET, AQUA_RING, AQUA_STEP, AQUA_TAIL, ARMOR_CANNON, ARM_THRUST, AROMATHERAPY, AROMATIC_MIST, ASSIST, ASSURANCE, ASTONISH, ASTRAL_BARRAGE, ATTACK_ORDER, ATTRACT, AURA_SPHERE, AURA_WHEEL, AURORA_BEAM, AURORA_VEIL, AUTOTOMIZE, AVALANCHE, AXE_KICK, BABY_DOLL_EYES, BADDY_BAD, BANEFUL_BUNKER, BARB_BARRAGE, BARRAGE, BARRIER, BATON_PASS, BEAK_BLAST, BEAT_UP, BEHEMOTH_BASH, BEHEMOTH_BLADE, BELCH, BELLY_DRUM, BESTOW, BIDE, BIND, BITE, BITTER_BLADE, BITTER_MALICE, BLACK_HOLE_ECLIPSE, BLAST_BURN, BLAZE_KICK, BLAZING_TORQUE, BLEAKWIND_STORM, BLIZZARD, BLOCK, BLOOM_DOOM, BLUE_FLARE, BODY_PRESS, BODY_SLAM, BOLT_BEAK, BOLT_STRIKE, BONE_CLUB, BONEMERANG, BONE_RUSH, BOOMBURST, BOUNCE, BOUNCY_BUBBLE, BRANCH_POKE, BRAVE_BIRD, BREAKING_SWIPE, BREAKNECK_BLITZ, BRICK_BREAK, BRINE, BRUTAL_SWING, BUBBLE, BUBBLE_BEAM, BUG_BITE, BUG_BUZZ, BULK_UP, BULLDOZE, BULLET_PUNCH, BULLET_SEED, BURNING_JEALOUSY, BURN_UP, BUZZY_BUZZ, CALM_MIND, CAMOUFLAGE, CAPTIVATE, CATASTROPIKA, CEASELESS_EDGE, CELEBRATE, CHARGE, CHARGE_BEAM, CHARM, CHATTER, CHILLING_WATER, CHILLY_RECEPTION, CHIP_AWAY, CHLOROBLAST, CIRCLE_THROW, CLAMP, CLANGING_SCALES, CLANGOROUS_SOUL, CLANGOROUS_SOULBLAZE, CLEAR_SMOG, CLOSE_COMBAT, COACHING, COIL, COLLISION_COURSE, COMBAT_TORQUE, COMET_PUNCH, COMEUPPANCE, CONFIDE, CONFUSE_RAY, CONFUSION, CONSTRICT, CONTINENTAL_CRUSH, CONVERSION, CONVERSION_2, COPYCAT, CORE_ENFORCER, CORKSCREW_CRASH, CORROSIVE_GAS, COSMIC_POWER, COTTON_GUARD, COTTON_SPORE, COUNTER, COURT_CHANGE, COVET, CRABHAMMER, CRAFTY_SHIELD, CROSS_CHOP, CROSS_POISON, CRUNCH, CRUSH_CLAW, CRUSH_GRIP, CURSE, CUT, DARKEST_LARIAT, DARK_PULSE, DARK_VOID, DAZZLING_GLEAM, DECORATE, DEFEND_ORDER, DEFENSE_CURL, DEFOG, DESTINY_BOND, DETECT, DEVASTATING_DRAKE, DIAMOND_STORM, DIG, DISABLE, DISARMING_VOICE, DISCHARGE, DIRE_CLAW, DIVE, DIZZY_PUNCH, DOODLE, DOOM_DESIRE, DOUBLE_EDGE, DOUBLE_HIT, DOUBLE_IRON_BASH, DOUBLE_KICK, DOUBLE_SHOCK, DOUBLE_SLAP, DOUBLE_TEAM, DRACO_METEOR, DRAGON_ASCENT, DRAGON_BREATH, DRAGON_CLAW, DRAGON_DANCE, DRAGON_DARTS, DRAGON_ENERGY, DRAGON_HAMMER, DRAGON_PULSE, DRAGON_RAGE, DRAGON_RUSH, DRAGON_TAIL, DRAINING_KISS, DRAIN_PUNCH, DREAM_EATER, DRILL_PECK, DRILL_RUN, DRUM_BEATING, DUAL_CHOP, DUAL_WINGBEAT, DYNAMAX_CANNON, DYNAMIC_PUNCH, EARTH_POWER, EARTHQUAKE, ECHOED_VOICE, EERIE_IMPULSE, EERIE_SPELL, EGG_BOMB, ELECTRIC_TERRAIN, ELECTRIFY, ELECTRO_BALL, ELECTRO_DRIFT, ELECTROWEB, EMBARGO, EMBER, ENCORE, ENDEAVOR, ENDURE, ENERGY_BALL, ENTRAINMENT, ERUPTION, ESPER_WING, ETERNABEAM, EXPANDING_FORCE, EXPLOSION, EXTRASENSORY, EXTREME_EVOBOOST, EXTREME_SPEED, FACADE, FAIRY_LOCK, FAIRY_WIND, FAKE_OUT, FAKE_TEARS, FALSE_SURRENDER, FALSE_SWIPE, FEATHER_DANCE, FEINT, FEINT_ATTACK, FELL_STINGER, FIERY_DANCE, FIERY_WRATH, FILLET_AWAY, FINAL_GAMBIT, FIRE_BLAST, FIRE_FANG, FIRE_LASH, FIRE_PLEDGE, FIRE_PUNCH, FIRE_SPIN, FIRST_IMPRESSION, FISHIOUS_REND, FISSURE, FLAIL, FLAME_BURST, FLAME_CHARGE, FLAME_WHEEL, FLAMETHROWER, FLARE_BLITZ, FLASH, FLASH_CANNON, FLATTER, FLEUR_CANNON, FLING, FLIP_TURN, FLOATY_FALL, FLORAL_HEALING, FLOWER_SHIELD, FLOWER_TRICK, FLY, FLYING_PRESS, FOCUS_BLAST, FOCUS_ENERGY, FOCUS_PUNCH, FOLLOW_ME, FORCE_PALM, FORESIGHT, FORESTS_CURSE, FOUL_PLAY, FREEZE_DRY, FREEZE_SHOCK, FREEZING_GLARE, FREEZY_FROST, FRENZY_PLANT, FROST_BREATH, FRUSTRATION, FURY_ATTACK, FURY_CUTTER, FURY_SWIPES, FUSION_BOLT, FUSION_FLARE, FUTURE_SIGHT, GASTRO_ACID, GEAR_GRIND, GEAR_UP, GENESIS_SUPERNOVA, GEOMANCY, GIGA_DRAIN, GIGA_IMPACT, GIGATON_HAMMER, GIGAVOLT_HAVOC, GLACIAL_LANCE, GLACIATE, GLAIVE_RUSH, GLARE, GLITZY_GLOW, G_MAX_BEFUDDLE, G_MAX_CANNONADE, G_MAX_CENTIFERNO, G_MAX_CHI_STRIKE, G_MAX_CUDDLE, G_MAX_DEPLETION, G_MAX_DRUM_SOLO, G_MAX_FINALE, G_MAX_FIREBALL, G_MAX_FOAM_BURST, G_MAX_GOLD_RUSH, G_MAX_GRAVITAS, G_MAX_HYDROSNIPE, G_MAX_MALODOR, G_MAX_MELTDOWN, G_MAX_ONE_BLOW, G_MAX_RAPID_FLOW, G_MAX_REPLENISH, G_MAX_RESONANCE, G_MAX_SANDBLAST, G_MAX_SMITE, G_MAX_SNOOZE, G_MAX_STEELSURGE, G_MAX_STONESURGE, G_MAX_STUN_SHOCK, G_MAX_SWEETNESS, G_MAX_TARTNESS, G_MAX_TERROR, G_MAX_VINE_LASH, G_MAX_VOLCALITH, G_MAX_VOLT_CRASH, G_MAX_WILDFIRE, G_MAX_WIND_RAGE, GRASS_KNOT, GRASS_PLEDGE, GRASS_WHISTLE, GRASSY_GLIDE, GRASSY_TERRAIN, GRAV_APPLE, GRAVITY, GROWL, GROWTH, GRUDGE, GUARDIAN_OF_ALOLA, GUARD_SPLIT, GUARD_SWAP, GUILLOTINE, GUNK_SHOT, GUST, GYRO_BALL, HAIL, HAMMER_ARM, HAPPY_HOUR, HARDEN, HAZE, HEADBUTT, HEAD_CHARGE, HEADLONG_RUSH, HEAD_SMASH, HEAL_BELL, HEAL_BLOCK, HEALING_WISH, HEAL_ORDER, HEAL_PULSE, HEART_STAMP, HEART_SWAP, HEAT_CRASH, HEAT_WAVE, HEAVY_SLAM, HELPING_HAND, HEX, HIDDEN_POWER, HIDDEN_POWER_BUG, HIDDEN_POWER_DARK, HIDDEN_POWER_DRAGON, HIDDEN_POWER_ELECTRIC, HIDDEN_POWER_FIGHTING, HIDDEN_POWER_FIRE, HIDDEN_POWER_FLYING, HIDDEN_POWER_GHOST, HIDDEN_POWER_GRASS, HIDDEN_POWER_GROUND, HIDDEN_POWER_ICE, HIDDEN_POWER_POISON, HIDDEN_POWER_PSYCHIC, HIDDEN_POWER_ROCK, HIDDEN_POWER_STEEL, HIDDEN_POWER_WATER, HIGH_HORSEPOWER, HIGH_JUMP_KICK, HOLD_BACK, HOLD_HANDS, HONE_CLAWS, HORN_ATTACK, HORN_DRILL, HORN_LEECH, HOWL, HURRICANE, HYDRO_CANNON, HYDRO_PUMP, HYDRO_STEAM, HYDRO_VORTEX, HYPER_BEAM, HYPER_DRILL, HYPER_FANG, HYPERSPACE_FURY, HYPERSPACE_HOLE, HYPER_VOICE, HYPNOSIS, ICE_BALL, ICE_BEAM, ICE_BURN, ICE_FANG, ICE_HAMMER, ICE_PUNCH, ICE_SHARD, ICE_SPINNER, ICICLE_CRASH, ICICLE_SPEAR, ICY_WIND, IMPRISON, INCINERATE, INFERNAL_PARADE, INFERNO, INFERNO_OVERDRIVE, INFESTATION, INGRAIN, INSTRUCT, ION_DELUGE, IRON_DEFENSE, IRON_HEAD, IRON_TAIL, JAW_LOCK, JET_PUNCH, JUDGMENT, JUMP_KICK, JUNGLE_HEALING, KARATE_CHOP, KINESIS, KINGS_SHIELD, KNOCK_OFF, KOWTOW_CLEAVE, LANDS_WRATH, LASER_FOCUS, LASH_OUT, LAST_RESORT, LAST_RESPECTS, LAVA_PLUME, LEAFAGE, LEAF_BLADE, LEAF_STORM, LEAF_TORNADO, LEECH_LIFE, LEECH_SEED, LEER, LETS_SNUGGLE_FOREVER, LICK, LIFE_DEW, LIGHT_OF_RUIN, LIGHT_SCREEN, LIGHT_THAT_BURNS_THE_SKY, LIQUIDATION, LOCK_ON, LOVELY_KISS, LOW_KICK, LOW_SWEEP, LUCKY_CHANT, LUMINA_CRASH, LUNAR_BLESSING, LUNAR_DANCE, LUNGE, LUSTER_PURGE, MACH_PUNCH, MAGICAL_LEAF, MAGICAL_TORQUE, MAGIC_COAT, MAGIC_POWDER, MAGIC_ROOM, MAGMA_STORM, MAGNET_BOMB, MAGNETIC_FLUX, MAGNET_RISE, MAGNITUDE, MAKE_IT_RAIN, MALICIOUS_MOONSAULT, MAT_BLOCK, MAX_AIRSTREAM, MAX_DARKNESS, MAX_FLARE, MAX_FLUTTERBY, MAX_GEYSER, MAX_GUARD, MAX_HAILSTORM, MAX_KNUCKLE, MAX_LIGHTNING, MAX_MINDSTORM, MAX_OOZE, MAX_OVERGROWTH, MAX_PHANTASM, MAX_QUAKE, MAX_ROCKFALL, MAX_STARFALL, MAX_STEELSPIKE, MAX_STRIKE, MAX_WYRMWIND, MEAN_LOOK, MEDITATE, ME_FIRST, MEGA_DRAIN, MEGAHORN, MEGA_KICK, MEGA_PUNCH, MEMENTO, MENACING_MOONRAZE_MAELSTROM, METAL_BURST, METAL_CLAW, METAL_SOUND, METEOR_ASSAULT, METEOR_BEAM, METEOR_MASH, METRONOME, MILK_DRINK, MIMIC, MIND_BLOWN, MIND_READER, MINIMIZE, MIRACLE_EYE, MIRROR_COAT, MIRROR_MOVE, MIRROR_SHOT, MIST, MIST_BALL, MISTY_EXPLOSION, MISTY_TERRAIN, MOONBLAST, MOONGEIST_BEAM, MOONLIGHT, MORNING_SUN, MORTAL_SPIN, MOUNTAIN_GALE, MUD_BOMB, MUD_SHOT, MUD_SLAP, MUD_SPORT, MUDDY_WATER, MULTI_ATTACK, MYSTICAL_FIRE, MYSTICAL_POWER, NASTY_PLOT, NATURAL_GIFT, NATURE_POWER, NATURES_MADNESS, NEEDLE_ARM, NEVER_ENDING_NIGHTMARE, NIGHT_DAZE, NIGHTMARE, NIGHT_SHADE, NIGHT_SLASH, NOBLE_ROAR, NO_RETREAT, NOXIOUS_TORQUE, NUZZLE, OBLIVION_WING, OBSTRUCT, OCEANIC_OPERETTA, OCTAZOOKA, OCTOLOCK, ODOR_SLEUTH, OMINOUS_WIND, ORDER_UP, ORIGIN_PULSE, OUTRAGE, OVERDRIVE, OVERHEAT, PAIN_SPLIT, PARABOLIC_CHARGE, PARTING_SHOT, PAYBACK, PAY_DAY, PECK, PERISH_SONG, PETAL_BLIZZARD, PETAL_DANCE, PHANTOM_FORCE, PHOTON_GEYSER, PIKA_PAPOW, PIN_MISSILE, PLASMA_FISTS, PLAY_NICE, PLAY_ROUGH, PLUCK, POISON_FANG, POISON_GAS, POISON_JAB, POISON_POWDER, POISON_STING, POISON_TAIL, POLLEN_PUFF, POLTERGEIST, POPULATION_BOMB, POUNCE, POUND, POWDER, POWDER_SNOW, POWER_GEM, POWER_SHIFT, POWER_SPLIT, POWER_SWAP, POWER_TRICK, POWER_TRIP, POWER_UP_PUNCH, POWER_WHIP, PRECIPICE_BLADES, PRESENT, PRISMATIC_LASER, PROTECT, PSYBEAM, PSYBLADE, PSYCH_UP, PSYCHIC, PSYCHIC_FANGS, PSYCHIC_TERRAIN, PSYCHO_BOOST, PSYCHO_CUT, PSYCHO_SHIFT, PSYSHIELD_BASH, PSYSHOCK, PSYSTRIKE, PSYWAVE, PULVERIZING_PANCAKE, PUNISHMENT, PURIFY, PURSUIT, PYRO_BALL, QUASH, QUICK_ATTACK, QUICK_GUARD, QUIVER_DANCE, RAGE, RAGE_FIST, RAGE_POWDER, RAGING_BULL, RAGING_FURY, RAIN_DANCE, RAPID_SPIN, RAZOR_LEAF, RAZOR_SHELL, RAZOR_WIND, RECOVER, RECYCLE, REFLECT, REFLECT_TYPE, REFRESH, RELIC_SONG, REST, RETALIATE, RETURN, REVELATION_DANCE, REVENGE, REVERSAL, REVIVAL_BLESSING, RISING_VOLTAGE, ROAR, ROAR_OF_TIME, ROCK_BLAST, ROCK_CLIMB, ROCK_POLISH, ROCK_SLIDE, ROCK_SMASH, ROCK_THROW, ROCK_TOMB, ROCK_WRECKER, ROLE_PLAY, ROLLING_KICK, ROLLOUT, ROOST, ROTOTILLER, ROUND, RUINATION, SACRED_FIRE, SACRED_SWORD, SAFEGUARD, SALT_CURE, SAND_ATTACK, SANDSEAR_STORM, SANDSTORM, SAND_TOMB, SAPPY_SEED, SAVAGE_SPIN_OUT, SCALD, SCALE_SHOT, SCARY_FACE, SCORCHING_SANDS, SCRATCH, SCREECH, SEARING_SHOT, SEARING_SUNRAZE_SMASH, SECRET_POWER, SECRET_SWORD, SEED_BOMB, SEED_FLARE, SEISMIC_TOSS, SELF_DESTRUCT, SHADOW_BALL, SHADOW_BONE, SHADOW_CLAW, SHADOW_FORCE, SHADOW_PUNCH, SHADOW_SNEAK, SHARPEN, SHATTERED_PSYCHE, SHED_TAIL, SHEER_COLD, SHELL_SIDE_ARM, SHELL_SMASH, SHELL_TRAP, SHELTER, SHIFT_GEAR, SHOCK_WAVE, SHORE_UP, SIGNAL_BEAM, SILK_TRAP, SILVER_WIND, SIMPLE_BEAM, SING_MOVE /*Many math libraries define SING as a macro*/, SINISTER_ARROW_RAID, SIZZLY_SLIDE, SKETCH, SKILL_SWAP, SKITTER_SMACK, SKULL_BASH, SKY_ATTACK, SKY_DROP, SKY_UPPERCUT, SLACK_OFF, SLAM, SLASH, SLEEP_POWDER, SLEEP_TALK, SLUDGE, SLUDGE_BOMB, SLUDGE_WAVE, SMACK_DOWN, SMART_STRIKE, SMELLING_SALTS, SMOG, SMOKESCREEN, SNAP_TRAP, SNARL, SNATCH, SNIPE_SHOT, SNORE, SNOWSCAPE, SOAK, SOFT_BOILED, SOLAR_BEAM, SOLAR_BLADE, SONIC_BOOM, SOUL_STEALING_7_STAR_STRIKE, SPACIAL_REND, SPARK, SPARKLING_ARIA, SPARKLY_SWIRL, SPECTRAL_THIEF, SPEED_SWAP, SPICY_EXTRACT, SPIDER_WEB, SPIKE_CANNON, SPIKES, SPIKY_SHIELD, SPIN_OUT, SPIRIT_BREAK, SPIRIT_SHACKLE, SPIT_UP, SPITE, SPLASH, SPLINTERED_STORMSHARDS, SPLISHY_SPLASH, SPORE, SPOTLIGHT, SPRINGTIDE_STORM, STEALTH_ROCK, STEAM_ERUPTION, STEAMROLLER, STEEL_BEAM, STEEL_ROLLER, STEEL_WING, STICKY_WEB, STOCKPILE, STOKED_SPARKSURFER, STOMP, STOMPING_TANTRUM, STONE_AXE, STONE_EDGE, STORED_POWER, STORM_THROW, STRANGE_STEAM, STRENGTH, STRENGTH_SAP, STRING_SHOT, STRUGGLE, STRUGGLE_BUG, STUFF_CHEEKS, STUN_SPORE, SUBMISSION, SUBSTITUTE, SUBZERO_SLAMMER, SUCKER_PUNCH, SUNNY_DAY, SUNSTEEL_STRIKE, SUPER_FANG, SUPERPOWER, SUPERSONIC, SUPERSONIC_SKYSTRIKE, SURF, SURGING_STRIKES, SWAGGER, SWALLOW, SWEET_KISS, SWEET_SCENT, SWIFT, SWITCHEROO, SWORDS_DANCE, SYNCHRONOISE, SYNTHESIS, TACKLE, TAIL_GLOW, TAIL_SLAP, TAIL_WHIP, TAILWIND, TAKE_DOWN, TAKE_HEART, TAR_SHOT, TAUNT, TEARFUL_LOOK, TEATIME, TECHNO_BLAST, TECTONIC_RAGE, TEETER_DANCE, TELEKINESIS, TELEPORT, TERA_BLAST, TERRAIN_PULSE, THIEF, THOUSAND_ARROWS, THOUSAND_WAVES, THRASH, THROAT_CHOP, THUNDER, THUNDERBOLT, THUNDER_CAGE, THUNDER_FANG, THUNDEROUS_KICK, THUNDER_PUNCH, THUNDER_SHOCK, THUNDER_WAVE, TICKLE, TIDY_UP, TOPSY_TURVY, TORCH_SONG, TORMENT, TOXIC, TOXIC_SPIKES, TOXIC_THREAD, TRAILBLAZE, TRANSFORM, TRI_ATTACK, TRICK, TRICK_OR_TREAT, TRICK_ROOM, TRIPLE_ARROWS, TRIPLE_AXEL, TRIPLE_DIVE, TRIPLE_KICK, TROP_KICK, TRUMP_CARD, TWIN_BEAM, TWINEEDLE, TWINKLE_TACKLE, TWISTER, U_TURN, UPROAR, VACUUM_WAVE, V_CREATE, VEEVEE_VOLLEY, VENOM_DRENCH, VENOSHOCK, VICTORY_DANCE, VINE_WHIP, VISE_GRIP, VITAL_THROW, VOLT_SWITCH, VOLT_TACKLE, WAKE_UP_SLAP, WATERFALL, WATER_GUN, WATER_PLEDGE, WATER_PULSE, WATER_SHURIKEN, WATER_SPORT, WATER_SPOUT, WAVE_CRASH, WEATHER_BALL, WHIRLPOOL, WHIRLWIND, WICKED_BLOW, WICKED_TORQUE, WIDE_GUARD, WILDBOLT_STORM, WILD_CHARGE, WILL_O_WISP, WING_ATTACK, WISH, WITHDRAW, WONDER_ROOM, WOOD_HAMMER, WORK_UP, WORRY_SEED, WRAP, WRING_OUT, X_SCISSOR, YAWN, ZAP_CANNON, ZEN_HEADBUTT, ZING_ZAP, ZIPPY_ZAP, MOVE_TOTAL,
  // clang-format on
};
}  // namespace pokesim::dex

/////////////////////// END OF src/Types/Enums/Move.hpp ////////////////////////

////////////////////// START OF src/Types/Enums/Slot.hpp ///////////////////////

#include <cstdint>

namespace pokesim {
enum class Slot : std::uint8_t {
  NONE,
  P1A,
  P2A,
  P1B,
  P2B,

  P1C,
  P2C,
  P1D,
  P2D,
  P1E,
  P2E,
  P1F,
  P2F,
};
}

/////////////////////// END OF src/Types/Enums/Slot.hpp ////////////////////////

/////////////////// START OF src/Battle/Helpers/Helpers.hpp ////////////////////

namespace pokesim {
struct Sides;
struct MoveSlots;

inline types::entity slotToSideEntity(const Sides& sides, Slot targetSlot);
inline types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot targetSlot);
inline types::entity slotToPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot);
inline types::entity slotToAllyPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot);
inline types::entity moveToEntity(const types::registry& registry, const MoveSlots& moveSlots, dex::Move move);
}  // namespace pokesim

//////////////////// END OF src/Battle/Helpers/Helpers.hpp /////////////////////

///////////////// START OF src/Battle/Setup/StateSetupBase.hpp /////////////////

namespace pokesim::internal {
/**
 * @brief Class that holds the shared methods used between the various battle state setup tools.
 *
 * It is intended to be extended by classes that set up the state of a battle, such as StateSetupSide.
 */
struct StateSetupBase {
 protected:
  types::handle handle;

 public:
  StateSetupBase(types::registry& registry, types::entity entity) : handle(registry, entity) {}

  /**
   * @brief Sets a property on the entity.
   *
   * @tparam Tag The tag type of the property to set.
   */
  template <typename Tag>
  void setProperty() {
    handle.emplace<Tag>();
  }

  types::entity entity() { return handle.entity(); }
};

}  // namespace pokesim::internal

////////////////// END OF src/Battle/Setup/StateSetupBase.hpp //////////////////

///////////////////////// START OF src/Types/Stats.hpp /////////////////////////

#include <cstdint>

namespace pokesim::types {
using level = std::uint8_t;

using stat = std::uint16_t;
using baseStat = std::uint8_t;

using ev = std::uint8_t;
using iv = std::uint8_t;

using boost = std::int8_t;
}  // namespace pokesim::types

////////////////////////// END OF src/Types/Stats.hpp //////////////////////////

////////////////////// START OF src/Components/Boosts.hpp //////////////////////

namespace pokesim {
struct AtkBoost {
  types::boost val = 0;
};

struct DefBoost {
  types::boost val = 0;
};

struct SpaBoost {
  types::boost val = 0;
};

struct SpdBoost {
  types::boost val = 0;
};

struct SpeBoost {
  types::boost val = 0;
};
}  // namespace pokesim

/////////////////////// END OF src/Components/Boosts.hpp ///////////////////////

////////////////////// START OF src/Components/EVsIVs.hpp //////////////////////

namespace pokesim {
struct Evs {
  types::ev hp = 0;
  types::ev atk = 0;
  types::ev def = 0;
  types::ev spa = 0;
  types::ev spd = 0;
  types::ev spe = 0;
};

struct Ivs {
  types::iv hp = 0;
  types::iv atk = 0;
  types::iv def = 0;
  types::iv spa = 0;
  types::iv spd = 0;
  types::iv spe = 0;
};
}  // namespace pokesim

/////////////////////// END OF src/Components/EVsIVs.hpp ///////////////////////

////////////////////// START OF src/Components/Stats.hpp ///////////////////////

namespace pokesim::stat {
struct Hp {
  types::stat val = 1;
};

struct Atk {
  types::stat val = 1;
};

struct Def {
  types::stat val = 1;
};

struct Spa {
  types::stat val = 1;
};

struct Spd {
  types::stat val = 1;
};

struct Spe {
  types::stat val = 1;
};

struct EffectiveSpeed {
  types::stat effectiveSpeed = 1;
};
}  // namespace pokesim::stat

/////////////////////// END OF src/Components/Stats.hpp ////////////////////////

///////////////////// START OF src/Types/Enums/Ability.hpp /////////////////////

#include <cstdint>

namespace pokesim::dex {
// Pokemon ability name
enum class Ability : std::uint16_t {
  // clang-format off
  NO_ABILITY = 0, ADAPTABILITY, AERILATE, AFTERMATH, AIR_LOCK, ANALYTIC, ANGER_POINT, ANGER_SHELL, ANTICIPATION, ARENA_TRAP, ARMOR_TAIL, AROMA_VEIL, AS_ONE, AURA_BREAK, BAD_DREAMS, BALL_FETCH, BATTERY, BATTLE_ARMOR, BATTLE_BOND, BEADS_OF_RUIN, BEAST_BOOST, BERSERK, BIG_PECKS, BLAZE, BULLETPROOF, CHEEK_POUCH, CHILLING_NEIGH, CHLOROPHYLL, CLEAR_BODY, CLOUD_NINE, COLOR_CHANGE, COMATOSE, COMMANDER, COMPETITIVE, COMPOUND_EYES, CONTRARY, CORROSION, COSTAR, COTTON_DOWN, CUD_CHEW, CURIOUS_MEDICINE, CURSED_BODY, CUTE_CHARM, DAMP, DANCER, DARK_AURA, DAUNTLESS_SHIELD, DAZZLING, DEFEATIST, DEFIANT, DELTA_STREAM, DESOLATE_LAND, DISGUISE, DOWNLOAD, DRAGONS_MAW, DRIZZLE, DROUGHT, DRY_SKIN, EARTH_EATER, EARLY_BIRD, EFFECT_SPORE, ELECTRIC_SURGE, ELECTROMORPHOSIS, EMERGENCY_EXIT, FAIRY_AURA, FILTER, FLAME_BODY, FLARE_BOOST, FLASH_FIRE, FLOWER_GIFT, FLOWER_VEIL, FLUFFY, FORECAST, FOREWARN, FRIEND_GUARD, FRISK, FULL_METAL_BODY, FUR_COAT, GALE_WINGS, GALVANIZE, GLUTTONY, GOOD_AS_GOLD, GOOEY, GORILLA_TACTICS, GRASS_PELT, GRASSY_SURGE, GRIM_NEIGH, GUARD_DOG, GULP_MISSILE, GUTS, HADRON_ENGINE, HARVEST, HEALER, HEATPROOF, HEAVY_METAL, HONEY_GATHER, HUGE_POWER, HUNGER_SWITCH, HUSTLE, HYDRATION, HYPER_CUTTER, ICE_BODY, ICE_FACE, ICE_SCALES, ILLUMINATE, ILLUSION, IMMUNITY, IMPOSTER, INFILTRATOR, INNARDS_OUT, INNER_FOCUS, INSOMNIA, INTIMIDATE, INTREPID_SWORD, IRON_BARBS, IRON_FIST, JUSTIFIED, KEEN_EYE, KLUTZ, LEAF_GUARD, LEVITATE, LIBERO, LIGHT_METAL, LIGHTNING_ROD, LINGERING_AROMA, LIMBER, LIQUID_OOZE, LIQUID_VOICE, LONG_REACH, MAGIC_BOUNCE, MAGIC_GUARD, MAGICIAN, MAGMA_ARMOR, MAGNET_PULL, MARVEL_SCALE, MEGA_LAUNCHER, MERCILESS, MIMICRY, MINUS, MIRROR_ARMOR, MISTY_SURGE, MOLD_BREAKER, MOODY, MOTOR_DRIVE, MOXIE, MULTISCALE, MULTITYPE, MUMMY, MYCELIUM_MIGHT, NATURAL_CURE, NEUROFORCE, NEUTRALIZING_GAS, NO_GUARD, NORMALIZE, OBLIVIOUS, OPPORTUNIST, ORICHALCUM_PULSE, OVERCOAT, OVERGROW, OWN_TEMPO, PARENTAL_BOND, PASTEL_VEIL, PERISH_BODY, PICKPOCKET, PICKUP, PIXILATE, PLUS, POISON_HEAL, POISON_POINT, POISON_TOUCH, POWER_CONSTRUCT, POWER_OF_ALCHEMY, POWER_SPOT, PRANKSTER, PRESSURE, PRIMORDIAL_SEA, PRISM_ARMOR, PROPELLER_TAIL, PROTEAN, PROTOSYNTHESIS, PSYCHIC_SURGE, PUNK_ROCK, PURE_POWER, PURIFYING_SALT, QUARK_DRIVE, QUEENLY_MAJESTY, QUICK_DRAW, QUICK_FEET, RAIN_DISH, RATTLED, RECEIVER, RECKLESS, REFRIGERATE, REGENERATOR, RIPEN, RIVALRY, RKS_SYSTEM, ROCK_HEAD, ROCKY_PAYLOAD, ROUGH_SKIN, RUN_AWAY, SAND_FORCE, SAND_RUSH, SAND_SPIT, SAND_STREAM, SAND_VEIL, SAP_SIPPER, SCHOOLING, SCRAPPY, SCREEN_CLEANER, SEED_SOWER, SERENE_GRACE, SHADOW_SHIELD, SHADOW_TAG, SHARPNESS, SHED_SKIN, SHEER_FORCE, SHELL_ARMOR, SHIELD_DUST, SHIELDS_DOWN, SIMPLE, SKILL_LINK, SLOW_START, SLUSH_RUSH, SNIPER, SNOW_CLOAK, SNOW_WARNING, SOLAR_POWER, SOLID_ROCK, SOUL_HEART, SOUNDPROOF, SPEED_BOOST, STAKEOUT, STALL, STALWART, STAMINA, STANCE_CHANGE, STATIC, STEADFAST, STEAM_ENGINE, STEELWORKER, STEELY_SPIRIT, STENCH, STICKY_HOLD, STORM_DRAIN, STRONG_JAW, STURDY, SUCTION_CUPS, SUPER_LUCK, SUPREME_OVERLORD, SURGE_SURFER, SWARM, SWEET_VEIL, SWIFT_SWIM, SYMBIOSIS, SYNCHRONIZE, SWORD_OF_RUIN, TABLETS_OF_RUIN, TANGLED_FEET, TANGLING_HAIR, TECHNICIAN, TELEPATHY, TERAVOLT, THERMAL_EXCHANGE, THICK_FAT, TINTED_LENS, TORRENT, TOUGH_CLAWS, TOXIC_BOOST, TOXIC_DEBRIS, TRACE, TRANSISTOR, TRIAGE, TRUANT, TURBOBLAZE, UNAWARE, UNBURDEN, UNNERVE, UNSEEN_FIST, VESSEL_OF_RUIN, VICTORY_STAR, VITAL_SPIRIT, VOLT_ABSORB, WANDERING_SPIRIT, WATER_ABSORB, WATER_BUBBLE, WATER_COMPACTION, WATER_VEIL, WEAK_ARMOR, WELL_BAKED_BODY, WHITE_SMOKE, WIMP_OUT, WIND_POWER, WIND_RIDER, WONDER_GUARD, WONDER_SKIN, ZEN_MODE, ZERO_TO_HERO, ABILITY_TOTAL,
  // clang-format on
};
}  // namespace pokesim::dex

////////////////////// END OF src/Types/Enums/Ability.hpp //////////////////////

///////////////////// START OF src/Types/Enums/Gender.hpp //////////////////////

#include <cstdint>

namespace pokesim::dex {
// Pokemon gender name
enum class Gender : std::uint8_t { NO_GENDER = 0, FEMALE, MALE, GENDER_TOTAL };
}  // namespace pokesim::dex

////////////////////// END OF src/Types/Enums/Gender.hpp ///////////////////////

////////////////////// START OF src/Types/Enums/Item.hpp ///////////////////////

#include <cstdint>

namespace pokesim::dex {
// Name of items in Pokemon games
enum class Item : std::uint16_t {
  // clang-format off
  NO_ITEM = 0, ABILITY_CAPSULE, ABILITY_PATCH, ABILITY_SHIELD, ABOMASITE, ABRA_CANDY, ABSOLITE, ABSORB_BULB, ACRO_BIKE, ADAMANT_CRYSTAL, ADAMANT_MINT, ADAMANT_ORB, ADRENALINE_ORB, ADVENTURE_GUIDE, AERODACTYLITE, AERODACTYL_CANDY, AGGRONITE, AGUAV_BERRY, AIR_BALLOON, AIR_MAIL, ALAKAZITE, ALORAICHIUM_Z, ALTARIANITE, AMAZE_MULCH, AMPHAROSITE, AMULET_COIN, ANTIDOTE, APICOT_BERRY, APRICORN, APRICORN_BOX, AQUA_SUIT, ARMOR_FOSSIL, ARMOR_PASS, ARMORITE_ORE, ARTICUNO_CANDY, ASPEAR_BERRY, ASSAULT_VEST, AUDINITE, AURORATICKET, AUSPICIOUS_ARMOR, AUTOGRAPH, AUX_EVASION, AUX_GUARD, AUX_POWER, AUX_POWERGUARD, AWAKENING, AZELFS_FANG, AZURE_FLUTE, BABIRI_BERRY, BALL_OF_MUD, BALM_MUSHROOM, BAND_AUTOGRAPH, BANETTITE, BASCULEGION_FOOD, BASEMENT_KEY, BEACH_GLASS, BEAD_MAIL, BEAN_CAKE, BEAST_BALL, BEEDRILLITE, BELLSPROUT_CANDY, BELUE_BERRY, BERRY, BERRY_JUICE, BERRY_POTS, BERRY_POUCH, BERRY_SWEET, BERSERK_GENE, BICYCLE, BIG_BAMBOO_SHOOT, BIG_MALASADA, BIG_MUSHROOM, BIG_NUGGET, BIG_PEARL, BIG_ROOT, BIKE_VOUCHER, BINDING_BAND, BITTER_BERRY, BLACK_APRICORN, BLACK_AUGURITE, BLACK_BELT, BLACK_FLUTE, BLACK_GLASSES, BLACK_MANE_HAIR, BLACK_SLUDGE, BLACK_TUMBLESTONE, BLANK_PLATE, BLASTOISINITE, BLAZIKENITE, BLOOM_MAIL, BLUE_APRICORN, BLUE_CARD, BLUE_FLUTE, BLUE_ORB, BLUE_PETAL, BLUE_SCARF, BLUE_SHARD, BLUESKY_MAIL, BLU_ID_BADGE, BLUK_BERRY, BLUNDER_POLICY, BOLD_MINT, BONSLY_CARD, BONSLY_PHOTO, BOOST_MULCH, BOOSTER_ENERGY, BOTTLE_CAP, BRAVE_MINT, BRICK_MAIL, BRICK_PIECE, BRIDGE_MAIL_D, BRIDGE_MAIL_M, BRIDGE_MAIL_S, BRIDGE_MAIL_T, BRIDGE_MAIL_V, BRIGHT_POWDER, BUBBLE_MAIL, BUG_GEM, BUG_MEMORY, BUG_TERA_SHARD, BUGINIUM_Z, BUGWORT, BULBASAUR_CANDY, BURN_DRIVE, BURN_HEAL, BURNT_BERRY, CAKE_LURE_BASE, CALCIUM, CALM_MINT, CAMERUPTITE, CAMPING_GEAR, CANDY_TRUFFLE, CARBOS, CARD_KEY, CAREFUL_MINT, CARROT_SEEDS, CASTELIACONE, CASTER_FERN, CATCHING_CHARM, CATERPIE_CANDY, CELESTICA_FLUTE, CELL_BATTERY, CHALKY_STONE, CHANSEY_CANDY, CHARCOAL, CHARIZARDITE_X, CHARIZARDITE_Y, CHARMANDER_CANDY, CHARTI_BERRY, CHERI_BERRY, CHERISH_BALL, CHESTO_BERRY, CHILAN_BERRY, CHILL_DRIVE, CHIPPED_POT, CHOICE_BAND, CHOICE_DUMPLING, CHOICE_SCARF, CHOICE_SPECS, CHOPLE_BERRY, CLAW_FOSSIL, CLEANSE_TAG, CLEAR_AMULET, CLEAR_BELL, CLEFAIRY_CANDY, CLEVER_FEATHER, CLOVER_SWEET, COBA_BERRY, COIN_CASE, COLBUR_BERRY, COLOGNE_CASE, COLRESS_MACHINE, COMET_SHARD, COMMON_STONE, CONTEST_COSTUME, CONTEST_PASS, CORNN_BERRY, COUPON_1, COUPON_2, COUPON_3, COURAGE_CANDY, COURAGE_CANDY_L, COURAGE_CANDY_XL, COVER_FOSSIL, COVERT_CLOAK, CRACKED_POT, CRAFTING_KIT, CROWN_PASS, CRUNCHY_SALT, CUBONE_CANDY, CRY_ANALYZER, CUSTAP_BERRY, DAMP_MULCH, DAMP_ROCK, DARK_GEM, DARK_MEMORY, DARK_STONE, DARK_TERA_SHARD, DARKINIUM_Z, DATA_CARDS, DATA_ROM, DAWN_STONE, DAZZLING_HONEY, D_DISK, DECIDIUM_Z, DEEP_SEA_SCALE, DEEP_SEA_TOOTH, DESTINY_KNOT, DEVON_PARTS, DEVON_SCOPE, DEVON_SCUBA_GEAR, DIANCITE, DIGGER_DRILL, DIGLETT_CANDY, DIRE_HIT, DIRESHROOM, DISC_CASE, DISCOUNT_COUPON, DISCOVERY_SLATE, DISTORTION_SLATE, DITTO_CANDY, DIVE_BALL, DNA_SAMPLE, DNA_SPLICERS, DODUO_CANDY, DOME_FOSSIL, DOPPEL_BONNETS, DOUSE_DRIVE, DOWN_ST_KEY, DOWSING_MACHINE, DRACO_PLATE, DRAGON_FANG, DRAGON_GEM, DRAGON_MEMORY, DRAGON_SCALE, DRAGON_SKULL, DRAGON_TERA_SHARD, DRAGONIUM_Z, DRASH_BERRY, DRATINI_CANDY, DREAD_PLATE, DREAM_BALL, DREAM_MAIL, DROPPED_ITEM, DROWZEE_CANDY, DS_SOUNDS, DUBIOUS_DISC, DURIN_BERRY, DUSK_BALL, DUSK_STONE, DYNAMAX_BAND, DYNAMAX_CANDY, DYNAMAX_CRYSTALS, DYNITE_ORE, EARTH_PLATE, EEVEE_CANDY, EEVIUM_Z, EGG_TICKET, EGGANT_BERRY, EIN_FILE_C, EIN_FILE_F, EIN_FILE_H, EIN_FILE_P, EIN_FILE_S, EJECT_BUTTON, EJECT_PACK, EKANS_CANDY, ELECTABUZZ_CANDY, ELECTIRIZER, ELECTRIC_GEM, ELECTRIC_MEMORY, ELECTRIC_SEED, ELECTRIC_TERA_SHARD, ELECTRIUM_Z, ELEVATOR_KEY, ELIXIR, ENDORSEMENT, ENERGY_POWDER, ENERGY_ROOT, ENIGMA_BERRY, ENIGMA_STONE, ENIGMATIC_CARD, EON_FLUTE, EON_MAIL, EON_TICKET, ESCAPE_ROPE, ETERNAL_ICE, ETHER, EVERSTONE, EVIOLITE, EXCITE_SCENT, EXEGGCUTE_CANDY, EXP_CANDY_L, EXP_CANDY_M, EXP_CANDY_S, EXP_CANDY_XL, EXP_CANDY_XS, EXP_CHARM, EXP_SHARE, EXPERT_BELT, EXPLORER_KIT, FAB_MAIL, FAIRIUM_Z, FAIRY_GEM, FAIRY_MEMORY, FAIRY_TERA_SHARD, FAME_CHECKER, FARFETCHD_CANDY, FASHION_CASE, FAST_BALL, FAVORED_MAIL, F_DISK, FEATHER_BALL, FESTIVAL_TICKET, FIGHTING_GEM, FIGHTING_MEMORY, FIGHTING_TERA_SHARD, FIGHTINIUM_Z, FIGY_BERRY, FINE_REMEDY, FIRE_GEM, FIRE_MEMORY, FIRE_STONE, FIRE_TERA_SHARD, FIRIUM_Z, FISHING_ROD, FIST_PLATE, FLAME_MAIL, FLAME_ORB, FLAME_PLATE, FLOAT_STONE, FLOWER_MAIL, FLOWER_SWEET, FLUFFY_TAIL, FLYING_GEM, FLYING_MEMORY, FLYING_TERA_SHARD, FLYINIUM_Z, FOCUS_BAND, FOCUS_SASH, FORAGE_BAG, FOREST_BALM, FOSSILIZED_BIRD, FOSSILIZED_DINO, FOSSILIZED_DRAKE, FOSSILIZED_FISH, FRESH_WATER, FRIEND_BALL, FULL_HEAL, FULL_INCENSE, FULL_RESTORE, GALACTIC_KEY, GALARICA_CUFF, GALARICA_TWIG, GALARICA_WREATH, GALLADITE, GANLON_BERRY, GARCHOMPITE, GARDEVOIRITE, GASTLY_CANDY, GB_SOUNDS, GEAR, GENGARITE, GENIUS_FEATHER, GENOME_SLATE, GENTLE_MINT, GEODUDE_CANDY, GHOST_GEM, GHOST_MEMORY, GHOST_TERA_SHARD, GHOSTIUM_Z, GIGATON_BALL, GINEMA_BERRY, GLALITITE, GLITTER_MAIL, GO_GOGGLES, GOD_STONE, GOLD_BERRY, GOLD_BOTTLE_CAP, GOLD_LEAF, GOLD_TEETH, GOLDEEN_CANDY, GONZAPS_KEY, GOLDEN_NANAB_BERRY, GOLDEN_PINAP_BERRY, GOLDEN_RAZZ_BERRY, GOOD_ROD, GOOEY_MULCH, GORGEOUS_BOX, GRACIDEA, GRAIN_CAKE, GRAM_1, GRAM_2, GRAM_3, GRASS_GEM, GRASS_MEMORY, GRASS_TERA_SHARD, GRASSIUM_Z, GRASS_MAIL, GRASSY_SEED, GREAT_BALL, GREEN_APRICORN, GREEN_PETAL, GREEN_SCARF, GREEN_SHARD, GREET_MAIL, GREPA_BERRY, GRIMER_CANDY, GRIP_CLAW, GRIT_DUST, GRIT_GRAVEL, GRIT_PEBBLE, GRIT_ROCK, GRISEOUS_CORE, GRISEOUS_ORB, GRN_ID_BADGE, GROUND_GEM, GROUND_MEMORY, GROUND_TERA_SHARD, GROUNDIUM_Z, GROWLITHE_CANDY, GROWTH_MULCH, GRUBBY_HANKY, GS_BALL, GUARD_SPEC, GUIDEBOOK, GYARADOSITE, HABAN_BERRY, HARBOR_MAIL, HARD_STONE, HASTY_MINT, HEAL_BALL, HEAL_POWDER, HEALTH_CANDY, HEALTH_CANDY_L, HEALTH_CANDY_XL, HEALTH_FEATHER, HEART_MAIL, HEART_SCALE, HEARTY_GRAINS, HEAT_ROCK, HEAVY_BALL, HEAVY_DUTY_BOOTS, HELIX_FOSSIL, HERACRONITE, HI_TECH_EARBUDS, HITMONCHAN_CANDY, HITMONLEE_CANDY, HM01, HM02, HM03, HM04, HM05, HM06, HM07, HM08, HOLO_CASTER, HOMETOWN_MUFFIN, HONDEW_BERRY, HONEY, HONEY_CAKE, HONOR_OF_KALOS, HOPO_BERRY, HORSEA_CANDY, HOUNDOOMINITE, HP_UP, HYPER_POTION, IAPAPA_BERRY, ICE_BERRY, ICE_GEM, ICE_HEAL, ICE_MEMORY, ICE_STONE, ICE_TERA_SHARD, ICEROOT_CARROT, ICICLE_PLATE, ICIUM_Z, ICY_ROCK, ID_CARD, ILIMAS_NORMALIUM_Z, IMPISH_MINT, INCINIUM_Z, INQUIRY_MAIL, INSECT_PLATE, INTRIGUING_STONE, IRON, IRON_BALL, IRON_BARKTONGUE, IRON_CHUNK, IRON_PLATE, JABOCA_BERRY, JADE_ORB, JAIL_KEY, JAW_FOSSIL, JET_BALL, JIGGLYPUFF_CANDY, JOHTO_SLATE, JOLLY_MINT, JOY_SCENT, JUBILIFE_MUFFIN, JYNX_CANDY, KABUTO_CANDY, KANGASKHAN_CANDY, KANGASKHANITE, KANTO_SLATE, KASIB_BERRY, KEBIA_BERRY, KEE_BERRY, KELPSY_BERRY, KEY_STONE, KEY_TO_ROOM_1, KEY_TO_ROOM_2, KEY_TO_ROOM_4, KEY_TO_ROOM_6, KINGS_LEAF, KINGS_ROCK, KOFFING_CANDY, KOFUS_BOOK, KOMMONIUM_Z, KORAIDONS_POKE_BALL, KRABBY_CANDY, KRANE_MEMO_1, KRANE_MEMO_2, KRANE_MEMO_3, KRANE_MEMO_4, KRANE_MEMO_5, KUO_BERRY, LAGGING_TAIL, LANSAT_BERRY, LAPRAS_CANDY, LATIASITE, LATIOSITE, LAVA_COOKIE, LAX_INCENSE, LAX_MINT, L_DISK, LEADEN_BALL, LEADERS_CREST, LEAF_LETTER, LEAF_STONE, LEEK, LEFT_POKE_BALL, LEFTOVERS, LEGEND_PLATE, LEGENDARY_CLUE_1, LEGENDARY_CLUE_2, LEGENDARY_CLUE_3, LEGENDARY_CLUE, LEMONADE, LENS_CASE, LEPPA_BERRY, LETTER, LEVEL_BALL, LIBERTY_PASS, LICKITUNG_CANDY, LIECHI_BERRY, LIFE_ORB, LIFT_KEY, LIGHT_BALL, LIGHT_CLAY, LIGHT_STONE, LIKE_MAIL, LINKING_CORD, LITEBLUEMAIL, LOADED_DICE, LOCK_CAPSULE, LONE_EARRING, LONELY_MINT, LOOKER_TICKET, LOOT_SACK, LOPUNNITE, LOST_ITEM, LOST_SATCHEL, LOVE_BALL, LOVE_SWEET, LOVELY_MAIL, LUCARIONITE, LUCK_INCENSE, LUCKY_EGG, LUCKY_PUNCH, LUM_BERRY, LUMINOUS_MOSS, LUMIOSE_GALETTE, LUNALIUM_Z, LUNAR_FEATHER, LURE, LURE_BALL, LUSTROUS_GLOBE, LUSTROUS_ORB, LUXURY_BALL, LYCANIUM_Z, MACH_BIKE, MACHINE_PART, MACHO_BRACE, MACHOP_CANDY, MAGIKARP_CANDY, MAGMA_EMBLEM, MAGMA_STONE, MAGMA_SUIT, MAGMAR_CANDY, MAGMARIZER, MAGNEMITE_CANDY, MAGNET, MAGO_BERRY, MAGOST_BERRY, MAINGATE_KEY, MAKEUP_BAG, MALICIOUS_ARMOR, MANECTITE, MANKEY_CANDY, MARANGA_BERRY, MARBLE, MARK_CHARM, MARSHADIUM_Z, MARSH_BALM, MASTER_BALL, MAWILITE, MAX_ELIXIR, MAX_ETHER, MAX_LURE, MAX_HONEY, MAX_MUSHROOMS, MAX_POTION, MAX_REPEL, MAX_REVIVE, MAYORS_NOTE, MEADOW_PLATE, MECH_MAIL, MECHANICAL_BOX, MECHANICAL_CABINET, MECHANICAL_CIRCULAR_SAW, MECHANICAL_PINWHEEL, MECHANICAL_TUB, MEDAL_BOX, MEDICHAMITE, MEDICINAL_LEEK, MEGA_BRACELET, MEGA_RING, MELTAN_CANDY, MEMBER_CARD, MENTAL_HERB, MEOWTH_CANDY, MESPRITS_PLUME, METAGROSSITE, METAL_COAT, METAL_POWDER, METEORITE, METEORITE_SHARD, METRONOME, MEW_CANDY, MEWNIUM_Z, MEWTWO_CANDY, MEWTWONITE_X, MEWTWONITE_Y, MICLE_BERRY, MIGHTY_CANDY, MIGHTY_CANDY_L, MIGHTY_CANDY_XL, MILD_MINT, MIMIKIUM_Z, MIND_PLATE, MINT_BERRY, MIRACLEBERRY, MIRACLE_SEED, MIRAGE_MAIL, MIRAIDONS_POKE_BALL, MIROR_RADAR, MIRROR_HERB, MISTY_SEED, MODEST_MINT, MOLTRES_CANDY, MOOMOO_MILK, MOON_BALL, MOON_FLUTE, MOON_SHARD, MOON_STONE, MORPH_MAIL, MOSAIC_MAIL, MOUNTAIN_BALM, MR_MIME_CANDY, MUSCLE_BAND, MUSCLE_FEATHER, MUSHROOM_CAKE, MUSIC_DISC, MUSIC_MAIL, MYSTERIOUS_BALM, MYSTERIOUS_SHARD_S, MYSTERIOUS_SHARD_L, MYSTERYBERRY, MYSTERY_EGG, MYSTIC_WATER, MYSTICTICKET, NAIVE_MINT, NANAB_BERRY, NAUGHTY_MINT, NEST_BALL, NET_BALL, NEVER_MELT_ICE, NIDORAN_MALE_CANDY, NIDORAN_FEMALE_CANDY, NINIKU_BERRY, N_LUNARIZER, NOMEL_BERRY, NORMAL_BOX, NORMAL_GEM, NORMAL_TERA_SHARD, NORMALIUM_Z, N_SOLARIZER, NUGGET, NUTPEA_BERRY, OAKS_LETTER, OAKS_PARCEL, OCCA_BERRY, OCEANIC_SLATE, ODD_INCENSE, ODD_KEYSTONE, ODDISH_CANDY, OLD_AMBER, OLD_CHARM, OLD_GATEAU, OLD_JOURNAL, OLD_LETTER, OLD_ROD, OLD_SEA_MAP, OLD_VERSES, OMANYTE_CANDY, ONIX_CANDY, ORAN_BERRY, ORANGE_MAIL, ORANGE_PETAL, ORIGIN_BALL, ORIGIN_ORE, OVAL_CHARM, OVAL_STONE, PAIR_OF_TICKETS, PAL_PAD, PAMTRE_BERRY, PARALYZE_HEAL, PARAS_CANDY, PARCEL, PARK_BALL, PASS, PASS_ORB, PASSHO_BERRY, PAYAPA_BERRY, PEARL, PEARL_STRING, PEAT_BLOCK, PECHA_BERRY, PEP_UP_PLANT, PERMIT, PERSIM_BERRY, PETAYA_BERRY, PEWTER_CRUNCHIES, PICNIC_SET, PIDGEOTITE, PIDGEY_CANDY, PIKACHU_CANDY, PIKANIUM_Z, PIKASHUNIUM_Z, PINAP_BERRY, PINK_APRICORN, PINK_BOW, PINK_NECTAR, PINK_PETAL, PINK_SCARF, PINSIR_CANDY, PINSIRITE, PIXIE_PLATE, PLASMA_CARD, PLUME_FOSSIL, PLUMP_BEANS, POFFIN_CASE, POINT_CARD, POISON_BARB, POISON_GEM, POISON_MEMORY, POISON_TERA_SHARD, POISONIUM_Z, POKE_BALL, POKE_DOLL, POKE_FLUTE, POKE_RADAR, POKE_SNACK, POKE_TOY, POKEBLOCK_CASE, POKEBLOCK_KIT, POKEDEX, POKEMON_BOX_LINK, POKESHI_DOLL, POLKADOT_BOW, POLISHED_MUD_BALL, POLIWAG_CANDY, POMEG_BERRY, PONYTA_CANDY, POP_POD, PORTRAITMAIL, PORYGON_CANDY, POTION, POWER_ANKLET, POWER_BAND, POWER_BELT, POWER_BRACER, POWER_HERB, POWER_LENS, POWER_PLANT_PASS, POWER_WEIGHT, POWERUP_PART, POWDER_JAR, PP_MAX, PP_UP, PREMIER_BALL, PRETTY_FEATHER, PRIMARIUM_Z, PRISM_SCALE, PRISON_BOTTLE, PROFS_LETTER, PROFESSORS_MASK, PROP_CASE, PROTECTIVE_PADS, PROTECTOR, PROTEIN, PRZCUREBERRY, PSNCUREBERRY, PSYCHIC_GEM, PSYCHIC_MEMORY, PSYCHIC_SEED, PSYCHIC_TERA_SHARD, PSYCHIUM_Z, PSYDUCK_CANDY, PUMKIN_BERRY, PUNCHING_GLOVE, PURE_INCENSE, PURPLE_NECTAR, PURPLE_PETAL, QUALOT_BERRY, QUICK_BALL, QUICK_CANDY, QUICK_CANDY_L, QUICK_CANDY_XL, QUICK_CLAW, QUICK_POWDER, QUIET_MINT, RABUTA_BERRY, RADIANT_PETAL, RAGECANDYBAR, RAINBOW_FLOWER, RAINBOW_PASS, RAINBOW_SLATE, RAINBOW_WING, RARE_BONE, RARE_CANDY, RASH_MINT, RATTATA_CANDY, RAWST_BERRY, RAZOR_CLAW, RAZOR_FANG, RAZZ_BERRY, R_DISK, REAPER_CLOTH, RECIPES, RED_APRICORN, RED_CARD, RED_CHAIN, RED_FLUTE, RED_ID_BADGE, RED_NECTAR, RED_ORB, RED_PETAL, RED_SCALE, RED_SCARF, RED_SHARD, REINS_OF_UNITY, RELAXED_MINT, RELIC_BAND, RELIC_COPPER, RELIC_CROWN, RELIC_GOLD, RELIC_SILVER, RELIC_STATUE, RELIC_VASE, REMEDY, REPEAT_BALL, REPEL, REPLY_MAIL, RESIST_FEATHER, RETRO_MAIL, REVEAL_GLASS, REVIVAL_HERB, REVIVE, RHYHORN_CANDY, RIBBON_SWEET, RICH_MULCH, RIDE_PAGER, RINDO_BERRY, RING_TARGET, ROCK_GEM, ROCK_INCENSE, ROCK_MEMORY, ROCK_TERA_SHARD, ROCKIUM_Z, ROCKY_HELMET, ROLLER_SKATES, ROOM_SERVICE, ROOT_FOSSIL, ROSE_INCENSE, ROSELI_BERRY, ROTO_BARGAIN, ROTO_BOOST, ROTO_CATCH, ROTO_ENCOUNTER, ROTO_EXP_POINTS, ROTO_FRIENDSHIP, ROTO_HATCH, ROTO_HP_RESTORE, ROTO_PP_RESTORE, ROTO_PRIZE_MONEY, ROTO_STEALTH, ROTOM_BIKE, ROTOM_CATALOG, ROTOM_PHONE, ROWAP_BERRY, RSVP_MAIL, RUBY, RUNNING_SHOES, RUSTED_SHIELD, RUSTED_SWORD, S_S_TICKET, SABLENITE, SACHET, SACRED_ASH, SAFARI_BALL, SAFETY_GOGGLES, SAIL_FOSSIL, SALAC_BERRY, SALAMENCITE, SALT_CAKE, SAND_RADISH, SANDSHREW_CANDY, SANDWICH, SAPPHIRE, SASSY_MINT, SCANNER, SCARLET_BOOK, SCATTER_BANG, SCEPTILITE, SCIZORITE, SCOPE_LENS, SCROLL_OF_DARKNESS, SCROLL_OF_WATERS, SCYTHER_CANDY, SEA_INCENSE, SEAL_CASE, SECRET_KEY, SECRET_MEDICINE, SEED_OF_MASTERY, SEEL_CANDY, SERIOUS_MINT, SHADOW_MAIL, SHADEROOT_CARROT, SHALOUR_SABLE, SHARP_BEAK, SHARPEDONITE, SHED_SHELL, SHELL_BELL, SHELLDER_CANDY, SHINY_CHARM, SHINY_LEAF, SHINY_STONE, SHOAL_SALT, SHOAL_SHELL, SHOCK_DRIVE, SHUCA_BERRY, SILK_SCARF, SILPH_SCOPE, SILVER_LEAF, SILVER_NANAB_BERRY, SILVER_PINAP_BERRY, SILVER_POWDER, SILVER_RAZZ_BERRY, SILVER_WING, SITRUS_BERRY, SKULL_FOSSIL, SKY_PLATE, SKY_TUMBLESTONE, SLOWBRONITE, SLOWPOKE_CANDY, SLOWPOKE_TAIL, SMALL_BOUQUET, SMALL_TABLET, SMART_CANDY, SMART_CANDY_L, SMART_CANDY_XL, SMOKE_BALL, SMOKE_BOMB, SMOOTH_ROCK, SNORLAX_CANDY, SNORLIUM_Z, SNOWBALL, SNOW_BALM, SNOW_MAIL, SODA_POP, SOFT_SAND, SOLGANIUM_Z, SONIAS_BOOK, SOOT_SACK, SOOTFOOT_ROOT, SOOTHE_BELL, SOUL_DEW, SOUL_SLATE, SPACE_BALM, SPACE_MAIL, SPARKLING_STONE, SPEAROW_CANDY, SPELL_TAG, SPELON_BERRY, SPLASH_PLATE, SPOILED_APRICORN, SPOOKY_PLATE, SPORT_BALL, SPRAYDUCK, SPRINGY_MUSHROOM, SPRINKLOTAD, SQUALL_SLATE, SQUIRT_BOTTLE, SQUIRTLE_CANDY, STABLE_MULCH, STAR_PIECE, STAR_SWEET, STARDUST, STARF_BERRY, STARYU_CANDY, STEALTH_SPRAY, STEEL_GEM, STEEL_MAIL, STEEL_MEMORY, STEEL_TEETH, STEEL_TERA_SHARD, STEELIUM_Z, STEELIXITE, STICKY_BARB, STICKY_GLOB, STONE_PLATE, STORAGE_KEY, STRANGE_BALL, STRANGE_SOUVENIR, STRATOSPHERIC_SLATE, STRAWBERRY_SWEET, STRETCHY_SPRING, STRIB_BERRY, STYLE_CARD, SUBWAY_KEY, SUITE_KEY, SUN_FLUTE, SUN_SHARD, SUN_STONE, SUPER_LURE, SUPER_POTION, SUPER_REPEL, SUPER_ROD, SUPERB_REMEDY, SURFBOARD, SURF_MAIL, SURGE_BADGE, SURPRISE_MULCH, SURVIVAL_CHARM_B, SURVIVAL_CHARM_P, SURVIVAL_CHARM_R, SURVIVAL_CHARM_T, SURVIVAL_CHARM_Y, SWAMPERTITE, SWAP_SNACK, SWEET_APPLE, SWEET_HEART, SWIFT_FEATHER, SWORDCAP, SYSTEM_LEVER, TAMATO_BERRY, TANGA_BERRY, TANGELA_CANDY, TAPUNIUM_Z, TART_APPLE, TAUROS_CANDY, TEA, TEACHY_TV, TECTONIC_SLATE, TEMPTING_CHARM_B, TEMPTING_CHARM_P, TEMPTING_CHARM_R, TEMPTING_CHARM_T, TEMPTING_CHARM_Y, TENTACOOL_CANDY, TERA_ORB, TERRAIN_EXTENDER, TERU_SAMA, THANKS_MAIL, THICK_CLUB, THROAT_SPRAY, THUNDER_STONE, TIDAL_BELL, TIME_BALM, TIME_FLUTE, TIMER_BALL, TIMID_MINT, TINY_BAMBOO_SHOOT, TINY_MUSHROOM, TM01, TM02, TM03, TM04, TM05, TM06, TM07, TM08, TM09, TM10, TM11, TM12, TM13, TM14, TM15, TM16, TM17, TM18, TM19, TM20, TM21, TM22, TM23, TM24, TM25, TM26, TM27, TM28, TM29, TM30, TM31, TM32, TM33, TM34, TM35, TM36, TM37, TM38, TM39, TM40, TM41, TM42, TM43, TM44, TM45, TM46, TM47, TM48, TM49, TM50, TM51, TM52, TM53, TM54, TM55, TM56, TM57, TM58, TM59, TM60, TM61, TM62, TM63, TM64, TM65, TM66, TM67, TM68, TM69, TM70, TM71, TM72, TM73, TM74, TM75, TM76, TM77, TM78, TM79, TM80, TM81, TM82, TM83, TM84, TM85, TM86, TM87, TM88, TM89, TM90, TM91, TM92, TM93, TM94, TM95, TM96, TM97, TM98, TM99, TM_CASE, TM_MATERIALS, TR01, TR02, TR03, TR04, TR05, TR06, TR07, TR08, TR09, TR10, TR11, TR12, TR13, TR14, TR15, TR16, TR17, TR18, TR19, TR20, TR21, TR22, TR23, TR24, TR25, TR26, TR27, TR28, TR29, TR30, TR31, TR32, TR33, TR34, TR35, TR36, TR37, TR38, TR39, TR40, TR41, TR42, TR43, TR44, TR45, TR46, TR47, TR48, TR49, TR50, TR51, TR52, TR53, TR54, TR55, TR56, TR57, TR58, TR59, TR60, TR61, TR62, TR63, TR64, TR65, TR66, TR67, TR68, TR69, TR70, TR71, TR72, TR73, TR74, TR75, TR76, TR77, TR78, TR79, TR80, TR81, TR82, TR83, TR84, TR85, TR86, TR87, TR88, TR89, TR90, TR91, TR92, TR93, TR94, TR95, TR96, TR97, TR98, TR99, TMV_PASS, TOPO_BERRY, TORN_JOURNAL, TOUGA_BERRY, TOUGH_CANDY, TOUGH_CANDY_L, TOUGH_CANDY_XL, TOWN_MAP, TOXIC_ORB, TOXIC_PLATE, TRAVEL_TRUNK, TRI_PASS, TROPIC_MAIL, TROPICAL_SHELL, TUMBLESTONE, TUNNEL_MAIL, TWICE_SPICED_RADISH, TWISTED_SPOON, TYRANITARITE, U_DISK, ULTRA_BALL, ULTRANECROZIUM_Z, UNOWN_REPORT, UNUSUAL_SHOES, UPGRADE, UTILITY_UMBRELLA, UXIES_CLAW, VENONAT_CANDY, VENUSAURITE, VIOLET_BOOK, VIVICHOKE, VIVID_SCENT, VOICE_CASE_1, VOICE_CASE_2, VOICE_CASE_3, VOICE_CASE_4, VOICE_CASE_5, VOLCANO_BALM, VOLTORB_CANDY, VS_RECORDER, VS_SEEKER, VULPIX_CANDY, WACAN_BERRY, WAILMER_PAIL, WALL_FRAGMENT, WARDING_CHARM_B, WARDING_CHARM_P, WARDING_CHARM_R, WARDING_CHARM_T, WARDING_CHARM_Y, WATER_GEM, WATER_MEMORY, WATER_STONE, WATER_TERA_SHARD, WATERIUM_Z, WATMEL_BERRY, WAVE_INCENSE, WAVE_MAIL, WEAKNESS_POLICY, WEEDLE_CANDY, WEPEAR_BERRY, WHIPPED_DREAM, WHITE_APRICORN, WHITE_FLUTE, WHITE_HERB, WHITE_MANE_HAIR, WIDE_LENS, WIKI_BERRY, WING_BALL, WISE_GLASSES, WISHING_CHIP, WISHING_PIECE, WISHING_STAR, WOOD, WOOD_MAIL, WOODEN_CROWN, WORKS_KEY, X_ACCURACY, X_ATTACK, X_DEFENSE, X_SP_ATK, X_SP_DEF, X_SPEED, XTRANSCEIVER, YACHE_BERRY, YAGO_BERRY, YELLOW_APRICORN, YELLOW_FLUTE, YELLOW_NECTAR, YELLOW_PETAL, YELLOW_SCARF, YELLOW_SHARD, YLW_ID_BADGE, ZAP_PLATE, ZAPDOS_CANDY, ZINC, ZOOM_LENS, Z_POWER_RING, Z_RING, ZUBAT_CANDY, ZYGARDE_CUBE, ITEM_TOTAL
  // clang-format on
};
}  // namespace pokesim::dex

/////////////////////// END OF src/Types/Enums/Item.hpp ////////////////////////

///////////////////// START OF src/Types/Enums/Nature.hpp //////////////////////

#include <cstdint>

namespace pokesim::dex {
// Pokemon nature name
enum class Nature : std::uint8_t {
  // clang-format off
  NO_NATURE = 0, ADAMANT, BASHFUL, BOLD, BRAVE, CALM, CAREFUL, DOCILE, GENTLE, HARDY, HASTY, IMPISH, JOLLY, LAX, LONELY, MILD, MODEST, NAIVE, NAUGHTY, QUIET, QUIRKY, RASH, RELAXED, SASSY, SERIOUS, TIMID, NATURE_TOTAL
  // clang-format on
};
}  // namespace pokesim::dex

////////////////////// END OF src/Types/Enums/Nature.hpp ///////////////////////

///////////////////// START OF src/Types/Enums/Species.hpp /////////////////////

#include <cstdint>

namespace pokesim::dex {

/**
 * @brief Pokemon and Pokemon form name.
 *
 * @details Pokemon that have multiple forms will have their base form and alternate forms listed here.
 * However, if none of a Pokemon's forms are cosmetic (i.e. change nothing expect appearance), the forms cannot be
 * changed during battle, and no true base form exists, then the Pokemon's species name without a form specifier is
 * omitted. For example:
 *  - `VENUSAUR`, `MEGA_VENUSAUR`, and `GIGANTAMAX_VENUSAUR` are all listed because Venusaur changes into the other
 * forms mid-battle
 *  - `GASTRODON`, `WEST_SEA_GASTRODON`, and `EAST_SEA_GASTRODON` are all listed because although Gastrodon's forms
 * are permanent, their only difference is how they look
 *  - `PLANT_CLOAK_WORMADAM`, `SANDY_CLOAK_WORMADAM`, and `TRASH_CLOAK_WORMADAM` listed while `WORMADAM` is not
 * because the Wormadam forms have different types, stats, and moves; their forms are permanent; and there is no base
 * Wormadam.
 */
enum class Species : std::uint16_t {
  // clang-format off
  MISSING_NO = 0, BULBASAUR, IVYSAUR, VENUSAUR, MEGA_VENUSAUR, GIGANTAMAX_VENUSAUR, CHARMANDER, CHARMELEON, CHARIZARD, MEGA_CHARIZARD_X, MEGA_CHARIZARD_Y, GIGANTAMAX_CHARIZARD, SQUIRTLE, WARTORTLE, BLASTOISE, MEGA_BLASTOISE, GIGANTAMAX_BLASTOISE, CATERPIE, METAPOD, BUTTERFREE, GIGANTAMAX_BUTTERFREE, WEEDLE, KAKUNA, BEEDRILL, MEGA_BEEDRILL, PIDGEY, PIDGEOTTO, PIDGEOT, MEGA_PIDGEOT, RATTATA, ALOLAN_RATTATA, RATICATE, ALOLAN_RATICATE, TOTEM_ALOLAN_RATICATE, SPEAROW, FEAROW, EKANS, ARBOK, PIKACHU, COSPLAY_PIKACHU, PIKACHU_ROCK_STAR, PIKACHU_BELLE, PIKACHU_POP_STAR, PIKACHU_PHD, PIKACHU_LIBRE, ORIGINAL_CAP_PIKACHU, HOENN_CAP_PIKACHU, SINNOH_CAP_PIKACHU, UNOVA_CAP_PIKACHU, KALOS_CAP_PIKACHU, ALOLA_CAP_PIKACHU, PARTNER_CAP_PIKACHU, STARTER_PIKACHU, GIGANTAMAX_PIKACHU, WORLD_CAP_PIKACHU, RAICHU, ALOLAN_RAICHU, SANDSHREW, ALOLAN_SANDSHREW, SANDSLASH, ALOLAN_SANDSLASH, NIDORAN_FEMALE, NIDORINA, NIDOQUEEN, NIDORAN_MALE, NIDORINO, NIDOKING, CLEFAIRY, CLEFABLE, VULPIX, ALOLAN_VULPIX, NINETALES, ALOLAN_NINETALES, JIGGLYPUFF, WIGGLYTUFF, ZUBAT, GOLBAT, ODDISH, GLOOM, VILEPLUME, PARAS, PARASECT, VENONAT, VENOMOTH, DIGLETT, ALOLAN_DIGLETT, DUGTRIO, ALOLAN_DUGTRIO, MEOWTH, ALOLAN_MEOWTH, GALARIAN_MEOWTH, GIGANTAMAX_MEOWTH, PERSIAN, ALOLAN_PERSIAN, PSYDUCK, GOLDUCK, MANKEY, PRIMEAPE, HISUIAN_GROWLITHE, GROWLITHE, HISUIAN_ARCANINE, ARCANINE, POLIWAG, POLIWHIRL, POLIWRATH, ABRA, KADABRA, ALAKAZAM, MEGA_ALAKAZAM, MACHOP, MACHOKE, MACHAMP, GIGANTAMAX_MACHAMP, BELLSPROUT, WEEPINBELL, VICTREEBEL, TENTACOOL, TENTACRUEL, GEODUDE, ALOLAN_GEODUDE, GRAVELER, ALOLAN_GRAVELER, GOLEM, ALOLAN_GOLEM, PONYTA, GALARIAN_PONYTA, RAPIDASH, GALARIAN_RAPIDASH, SLOWPOKE, GALARIAN_SLOWPOKE, SLOWBRO, MEGA_SLOWBRO, GALARIAN_SLOWBRO, MAGNEMITE, MAGNETON, FARFETCH_D, GALARIAN_FARFETCH_D, DODUO, DODRIO, SEEL, DEWGONG, GRIMER, ALOLAN_GRIMER, MUK, ALOLAN_MUK, SHELLDER, CLOYSTER, GASTLY, HAUNTER, GENGAR, MEGA_GENGAR, GIGANTAMAX_GENGAR, ONIX, DROWZEE, HYPNO, KRABBY, KINGLER, GIGANTAMAX_KINGLER, VOLTORB, HISUIAN_VOLTORB, ELECTRODE, HISUIAN_ELECTRODE, EXEGGCUTE, EXEGGUTOR, ALOLAN_EXEGGUTOR, CUBONE, MAROWAK, ALOLAN_MAROWAK, TOTEM_ALOLAN_MAROWAK, HITMONLEE, HITMONCHAN, LICKITUNG, KOFFING, WEEZING, GALARIAN_WEEZING, RHYHORN, RHYDON, CHANSEY, TANGELA, KANGASKHAN, MEGA_KANGASKHAN, HORSEA, SEADRA, GOLDEEN, SEAKING, STARYU, STARMIE, MR_MIME, GALARIAN_MR_MIME, SCYTHER, JYNX, ELECTABUZZ, MAGMAR, PINSIR, MEGA_PINSIR, TAUROS, PALDEAN_TAUROS_COMBAT_BREAD, PALDEAN_TAUROS_BLAZE_BREAD, PALDEAN_TAUROS_AQUA_BREAD, MAGIKARP, GYARADOS, MEGA_GYARADOS, LAPRAS, GIGANTAMAX_LAPRAS, DITTO, EEVEE, STARTER_EEVEE, GIGANTAMAX_EEVEE, VAPOREON, JOLTEON, FLAREON, PORYGON, OMANYTE, OMASTAR, KABUTO, KABUTOPS, AERODACTYL, MEGA_AERODACTYL, SNORLAX, GIGANTAMAX_SNORLAX, ARTICUNO, GALARIAN_ARTICUNO, ZAPDOS, GALARIAN_ZAPDOS, MOLTRES, GALARIAN_MOLTRES, DRATINI, DRAGONAIR, DRAGONITE, MEWTWO, MEGA_MEWTWO_X, MEGA_MEWTWO_Y, MEW, CHIKORITA, BAYLEEF, MEGANIUM, CYNDAQUIL, QUILAVA, HISUIAN_TYPHLOSION, TYPHLOSION, TOTODILE, CROCONAW, FERALIGATR, SENTRET, FURRET, HOOTHOOT, NOCTOWL, LEDYBA, LEDIAN, SPINARAK, ARIADOS, CROBAT, CHINCHOU, LANTURN, PICHU, SPIKY_EARED_PICHU, CLEFFA, IGGLYBUFF, TOGEPI, TOGETIC, NATU, XATU, MAREEP, FLAAFFY, AMPHAROS, MEGA_AMPHAROS, BELLOSSOM, MARILL, AZUMARILL, SUDOWOODO, POLITOED, HOPPIP, SKIPLOOM, JUMPLUFF, AIPOM, SUNKERN, SUNFLORA, YANMA, WOOPER, PALDEAN_WOOPER, QUAGSIRE, ESPEON, UMBREON, MURKROW, SLOWKING, GALARIAN_SLOWKING, MISDREAVUS, UNOWN, UNOWN_A, UNOWN_B, UNOWN_C, UNOWN_D, UNOWN_E, UNOWN_F, UNOWN_G, UNOWN_H, UNOWN_I, UNOWN_J, UNOWN_K, UNOWN_L, UNOWN_M, UNOWN_N, UNOWN_O, UNOWN_P, UNOWN_Q, UNOWN_R, UNOWN_S, UNOWN_T, UNOWN_U, UNOWN_V, UNOWN_W, UNOWN_X, UNOWN_Y, UNOWN_Z, UNOWN_EXCLAMATION, UNOWN_QUESTION, WOBBUFFET, GIRAFARIG, PINECO, FORRETRESS, DUNSPARCE, GLIGAR, STEELIX, MEGA_STEELIX, SNUBBULL, GRANBULL, QWILFISH, HISUIAN_QWILFISH, SCIZOR, MEGA_SCIZOR, SHUCKLE, HERACROSS, MEGA_HERACROSS, HISUIAN_SNEASEL, SNEASEL, TEDDIURSA, URSARING, SLUGMA, MAGCARGO, SWINUB, PILOSWINE, CORSOLA, GALARIAN_CORSOLA, REMORAID, OCTILLERY, DELIBIRD, MANTINE, SKARMORY, HOUNDOUR, HOUNDOOM, MEGA_HOUNDOOM, KINGDRA, PHANPY, DONPHAN, PORYGON2, STANTLER, SMEARGLE, TYROGUE, HITMONTOP, SMOOCHUM, ELEKID, MAGBY, MILTANK, BLISSEY, RAIKOU, ENTEI, SUICUNE, LARVITAR, PUPITAR, TYRANITAR, MEGA_TYRANITAR, LUGIA, HO_OH, CELEBI, TREECKO, GROVYLE, SCEPTILE, MEGA_SCEPTILE, TORCHIC, COMBUSKEN, BLAZIKEN, MEGA_BLAZIKEN, MUDKIP, MARSHTOMP, SWAMPERT, MEGA_SWAMPERT, POOCHYENA, MIGHTYENA, ZIGZAGOON, GALARIAN_ZIGZAGOON, LINOONE, GALARIAN_LINOONE, WURMPLE, SILCOON, BEAUTIFLY, CASCOON, DUSTOX, LOTAD, LOMBRE, LUDICOLO, SEEDOT, NUZLEAF, SHIFTRY, TAILLOW, SWELLOW, WINGULL, PELIPPER, RALTS, KIRLIA, GARDEVOIR, MEGA_GARDEVOIR, SURSKIT, MASQUERAIN, SHROOMISH, BRELOOM, SLAKOTH, VIGOROTH, SLAKING, NINCADA, NINJASK, SHEDINJA, WHISMUR, LOUDRED, EXPLOUD, MAKUHITA, HARIYAMA, AZURILL, NOSEPASS, SKITTY, DELCATTY, SABLEYE, MEGA_SABLEYE, MAWILE, MEGA_MAWILE, ARON, LAIRON, AGGRON, MEGA_AGGRON, MEDITITE, MEDICHAM, MEGA_MEDICHAM, ELECTRIKE, MANECTRIC, MEGA_MANECTRIC, PLUSLE, MINUN, VOLBEAT, ILLUMISE, ROSELIA, GULPIN, SWALOT, CARVANHA, SHARPEDO, MEGA_SHARPEDO, WAILMER, WAILORD, NUMEL, CAMERUPT, MEGA_CAMERUPT, TORKOAL, SPOINK, GRUMPIG, SPINDA, TRAPINCH, VIBRAVA, FLYGON, CACNEA, CACTURNE, SWABLU, ALTARIA, MEGA_ALTARIA, ZANGOOSE, SEVIPER, LUNATONE, SOLROCK, BARBOACH, WHISCASH, CORPHISH, CRAWDAUNT, BALTOY, CLAYDOL, LILEEP, CRADILY, ANORITH, ARMALDO, FEEBAS, MILOTIC, CASTFORM, SUNNY_CASTFORM, RAINY_CASTFORM, SNOWY_CASTFORM, KECLEON, SHUPPET, BANETTE, MEGA_BANETTE, DUSKULL, DUSCLOPS, TROPIUS, CHIMECHO, ABSOL, MEGA_ABSOL, WYNAUT, SNORUNT, GLALIE, MEGA_GLALIE, SPHEAL, SEALEO, WALREIN, CLAMPERL, HUNTAIL, GOREBYSS, RELICANTH, LUVDISC, BAGON, SHELGON, SALAMENCE, MEGA_SALAMENCE, BELDUM, METANG, METAGROSS, MEGA_METAGROSS, REGIROCK, REGICE, REGISTEEL, LATIAS, MEGA_LATIAS, LATIOS, MEGA_LATIOS, KYOGRE, PRIMAL_KYOGRE, GROUDON, PRIMAL_GROUDON, RAYQUAZA, MEGA_RAYQUAZA, JIRACHI, DEOXYS, ATTACK_DEOXYS, DEFENSE_DEOXYS, SPEED_DEOXYS, TURTWIG, GROTLE, TORTERRA, CHIMCHAR, MONFERNO, INFERNAPE, PIPLUP, PRINPLUP, EMPOLEON, STARLY, STARAVIA, STARAPTOR, BIDOOF, BIBAREL, KRICKETOT, KRICKETUNE, SHINX, LUXIO, LUXRAY, BUDEW, ROSERADE, CRANIDOS, RAMPARDOS, SHIELDON, BASTIODON, BURMY, PLANT_CLOAK_BURMY, SANDY_CLOAK_BURMY, TRASH_CLOAK_BURMY, PLANT_CLOAK_WORMADAM, SANDY_CLOAK_WORMADAM, TRASH_CLOAK_WORMADAM, MOTHIM, COMBEE, VESPIQUEN, PACHIRISU, BUIZEL, FLOATZEL, CHERUBI, CHERRIM, CHERRIM_OVERCAST, CHERRIM_SUNSHINE, SHELLOS, WEST_SEA_SHELLOS, EAST_SEA_SHELLOS, GASTRODON, WEST_SEA_GASTRODON, EAST_SEA_GASTRODON, AMBIPOM, DRIFLOON, DRIFBLIM, BUNEARY, LOPUNNY, MEGA_LOPUNNY, MISMAGIUS, HONCHKROW, GLAMEOW, PURUGLY, CHINGLING, STUNKY, SKUNTANK, BRONZOR, BRONZONG, BONSLY, MIME_JR, HAPPINY, CHATOT, SPIRITOMB, GIBLE, GABITE, GARCHOMP, MEGA_GARCHOMP, MUNCHLAX, RIOLU, LUCARIO, MEGA_LUCARIO, HIPPOPOTAS, HIPPOWDON, SKORUPI, DRAPION, CROAGUNK, TOXICROAK, CARNIVINE, FINNEON, LUMINEON, MANTYKE, SNOVER, ABOMASNOW, MEGA_ABOMASNOW, WEAVILE, MAGNEZONE, LICKILICKY, RHYPERIOR, TANGROWTH, ELECTIVIRE, MAGMORTAR, TOGEKISS, YANMEGA, LEAFEON, GLACEON, GLISCOR, MAMOSWINE, PORYGON_Z, GALLADE, MEGA_GALLADE, PROBOPASS, DUSKNOIR, FROSLASS, ROTOM, HEAT_ROTOM, WASH_ROTOM, FROST_ROTOM, FAN_ROTOM, MOW_ROTOM, UXIE, MESPRIT, AZELF, DIALGA, DIALGA_ORIGIN, PALKIA, PALKIA_ORIGIN, HEATRAN, REGIGIGAS, GIRATINA_ALTERED, GIRATINA_ORIGIN, CRESSELIA, PHIONE, MANAPHY, DARKRAI, SHAYMIN, SHAYMIN_LAND, SHAYMIN_SKY, ARCEUS, ARCEUS_BUG, ARCEUS_DARK, ARCEUS_DRAGON, ARCEUS_ELECTRIC, ARCEUS_FAIRY, ARCEUS_FIGHTING, ARCEUS_FIRE, ARCEUS_FLYING, ARCEUS_GHOST, ARCEUS_GRASS, ARCEUS_GROUND, ARCEUS_ICE, ARCEUS_POISON, ARCEUS_PSYCHIC, ARCEUS_ROCK, ARCEUS_STEEL, ARCEUS_WATER, VICTINI, SNIVY, SERVINE, SERPERIOR, TEPIG, PIGNITE, EMBOAR, OSHAWOTT, DEWOTT, HISUIAN_SAMUROTT, SAMUROTT, PATRAT, WATCHOG, LILLIPUP, HERDIER, STOUTLAND, PURRLOIN, LIEPARD, PANSAGE, SIMISAGE, PANSEAR, SIMISEAR, PANPOUR, SIMIPOUR, MUNNA, MUSHARNA, PIDOVE, TRANQUILL, UNFEZANT, BLITZLE, ZEBSTRIKA, ROGGENROLA, BOLDORE, GIGALITH, WOOBAT, SWOOBAT, DRILBUR, EXCADRILL, AUDINO, MEGA_AUDINO, TIMBURR, GURDURR, CONKELDURR, TYMPOLE, PALPITOAD, SEISMITOAD, THROH, SAWK, SEWADDLE, SWADLOON, LEAVANNY, VENIPEDE, WHIRLIPEDE, SCOLIPEDE, COTTONEE, WHIMSICOTT, PETILIL, LILLIGANT, HISUIAN_LILLIGANT, RED_STRIPED_BASCULIN, BLUE_STRIPED_BASCULIN, WHITE_STRIPED_BASCULIN, SANDILE, KROKOROK, KROOKODILE, DARUMAKA, GALARIAN_DARUMAKA, DARMANITAN, ZEN_MODE_DARMANITAN, GALARIAN_DARMANITAN, GALARIAN_ZEN_MODE_DARMANITAN, MARACTUS, DWEBBLE, CRUSTLE, SCRAGGY, SCRAFTY, SIGILYPH, YAMASK, GALARIAN_YAMASK, COFAGRIGUS, TIRTOUGA, CARRACOSTA, ARCHEN, ARCHEOPS, TRUBBISH, GARBODOR, GIGANTAMAX_GARBODOR, ZORUA, HISUIAN_ZORUA, HISUIAN_ZOROARK, ZOROARK, MINCCINO, CINCCINO, GOTHITA, GOTHORITA, GOTHITELLE, SOLOSIS, DUOSION, REUNICLUS, DUCKLETT, SWANNA, VANILLITE, VANILLISH, VANILLUXE, DEERLING, DEERLING_SPRING, DEERLING_SUMMER, DEERLING_AUTUMN, DEERLING_WINTER, SAWSBUCK, SAWSBUCK_SPRING, SAWSBUCK_SUMMER, SAWSBUCK_AUTUMN, SAWSBUCK_WINTER, EMOLGA, KARRABLAST, ESCAVALIER, FOONGUS, AMOONGUSS, FRILLISH, JELLICENT, ALOMOMOLA, JOLTIK, GALVANTULA, FERROSEED, FERROTHORN, KLINK, KLANG, KLINKLANG, TYNAMO, EELEKTRIK, EELEKTROSS, ELGYEM, BEHEEYEM, LITWICK, LAMPENT, CHANDELURE, AXEW, FRAXURE, HAXORUS, CUBCHOO, BEARTIC, CRYOGONAL, SHELMET, ACCELGOR, STUNFISK, GALARIAN_STUNFISK, MIENFOO, MIENSHAO, DRUDDIGON, GOLETT, GOLURK, PAWNIARD, BISHARP, BOUFFALANT, RUFFLET, HISUIAN_BRAVIARY, BRAVIARY, VULLABY, MANDIBUZZ, HEATMOR, DURANT, DEINO, ZWEILOUS, HYDREIGON, LARVESTA, VOLCARONA, COBALION, TERRAKION, VIRIZION, INCARNATE_TORNADUS, TORNADUS_THERIAN, INCARNATE_THUNDURUS, THUNDURUS_THERIAN, RESHIRAM, ZEKROM, INCARNATE_LANDORUS, LANDORUS_THERIAN, KYUREM, BLACK_KYUREM, WHITE_KYUREM, KELDEO, RESOLUTE_KELDEO, ARIA_MELOETTA, PIROUETTE_MELOETTA, GENESECT, DOUSE_DRIVE_GENESECT, SHOCK_DRIVE_GENESECT, BURN_DRIVE_GENESECT, CHILL_DRIVE_GENESECT, CHESPIN, QUILLADIN, CHESNAUGHT, FENNEKIN, BRAIXEN, DELPHOX, FROAKIE, FROGADIER, GRENINJA, ASH_GRENINJA, BUNNELBY, DIGGERSBY, FLETCHLING, FLETCHINDER, TALONFLAME, SCATTERBUG, SPEWPA, VIVILLON, MEADOW_PATTERN_VIVILLON, ARCHIPELAGO_PATTERN_VIVILLON, CONTINENTAL_PATTERN_VIVILLON, ELEGANT_PATTERN_VIVILLON, GARDEN_PATTERN_VIVILLON, HIGH_PLAINS_PATTERN_VIVILLON, ICY_SNOW_PATTERN_VIVILLON, JUNGLE_PATTERN_VIVILLON, MARINE_PATTERN_VIVILLON, MODERN_PATTERN_VIVILLON, MONSOON_PATTERN_VIVILLON, OCEAN_PATTERN_VIVILLON, POLAR_PATTERN_VIVILLON, RIVER_PATTERN_VIVILLON, SANDSTORM_PATTERN_VIVILLON, SAVANNA_PATTERN_VIVILLON, SUN_PATTERN_VIVILLON, TUNDRA_PATTERN_VIVILLON, FANCY_PATTERN_VIVILLON, POKEBALL_PATTERN_VIVILLON, LITLEO, PYROAR, FLABEBE, RED_FLOWER_FLABEBE, BLUE_FLOWER_FLABEBE, ORANGE_FLOWER_FLABEBE, WHITE_FLOWER_FLABEBE, YELLOW_FLOWER_FLABEBE, FLOETTE, RED_FLOWER_FLOETTE, BLUE_FLOWER_FLOETTE, ORANGE_FLOWER_FLOETTE, WHITE_FLOWER_FLOETTE, YELLOW_FLOWER_FLOETTE, ETERNAL_FLOWER_FLOETTE, FLORGES, RED_FLOWER_FLORGES, BLUE_FLOWER_FLORGES, ORANGE_FLOWER_FLORGES, WHITE_FLOWER_FLORGES, YELLOW_FLOWER_FLORGES, SKIDDO, GOGOAT, PANCHAM, PANGORO, FURFROU, NATURAL_FURFROU, DANDY_TRIM_FURFROU, DEBUTANTE_TRIM_FURFROU, DIAMOND_TRIM_FURFROU, HEART_TRIM_FURFROU, KABUKI_TRIM_FURFROU, LA_REINE_TRIM_FURFROU, MATRON_TRIM_FURFROU, PHARAOH_TRIM_FURFROU, STAR_TRIM_FURFROU, ESPURR, MALE_MEOWSTIC, FEMALE_MEOWSTIC, HONEDGE, DOUBLADE, AEGISLASH, SHIELD_AEGISLASH, BLADE_AEGISLASH, SPRITZEE, AROMATISSE, SWIRLIX, SLURPUFF, INKAY, MALAMAR, BINACLE, BARBARACLE, SKRELP, DRAGALGE, CLAUNCHER, CLAWITZER, HELIOPTILE, HELIOLISK, TYRUNT, TYRANTRUM, AMAURA, AURORUS, SYLVEON, HAWLUCHA, DEDENNE, CARBINK, GOOMY, HISUIAN_SLIGGOO, SLIGGOO, HISUIAN_GOODRA, GOODRA, KLEFKI, PHANTUMP, TREVENANT, AVERAGE_SIZE_PUMPKABOO, SMALL_SIZE_PUMPKABOO, LARGE_SIZE_PUMPKABOO, SUPER_SIZE_PUMPKABOO, AVERAGE_SIZE_GOURGEIST, SMALL_SIZE_GOURGEIST, LARGE_SIZE_GOURGEIST, SUPER_SIZE_GOURGEIST, BERGMITE, AVALUGG, HISUIAN_AVALUGG, NOIBAT, NOIVERN, XERNEAS, YVELTAL, ZYGARDE_50, ZYGARDE_10, ZYGARDE_COMPLETE, DIANCIE, MEGA_DIANCIE, HOOPA_CONFINED, HOOPA_UNBOUND, VOLCANION, ROWLET, DARTRIX, HISUIAN_DECIDUEYE, DECIDUEYE, LITTEN, TORRACAT, INCINEROAR, POPPLIO, BRIONNE, PRIMARINA, PIKIPEK, TRUMBEAK, TOUCANNON, YUNGOOS, GUMSHOOS, TOTEM_GUMSHOOS, GRUBBIN, CHARJABUG, VIKAVOLT, TOTEM_VIKAVOLT, CRABRAWLER, CRABOMINABLE, BAILE_STYLE_ORICORIO, POM_POM_STYLE_ORICORIO, PA_U_STYLE_ORICORIO, SENSU_STYLE_ORICORIO, CUTIEFLY, RIBOMBEE, TOTEM_RIBOMBEE, ROCKRUFF, MIDDAY_ROCKRUFF, MIDDAY_LYCANROC, MIDNIGHT_LYCANROC, DUSK_LYCANROC, WISHIWASHI, SOLO_WISHIWASHI, SCHOOL_WISHIWASHI, MAREANIE, TOXAPEX, MUDBRAY, MUDSDALE, DEWPIDER, ARAQUANID, TOTEM_ARAQUANID, FOMANTIS, LURANTIS, TOTEM_LURANTIS, MORELULL, SHIINOTIC, SALANDIT, SALAZZLE, TOTEM_SALAZZLE, STUFFUL, BEWEAR, BOUNSWEET, STEENEE, TSAREENA, COMFEY, ORANGURU, PASSIMIAN, WIMPOD, GOLISOPOD, SANDYGAST, PALOSSAND, PYUKUMUKU, TYPE_NULL, SILVALLY, SILVALLY_BUG, SILVALLY_DARK, SILVALLY_DRAGON, SILVALLY_ELECTRIC, SILVALLY_FAIRY, SILVALLY_FIGHTING, SILVALLY_FIRE, SILVALLY_FLYING, SILVALLY_GHOST, SILVALLY_GRASS, SILVALLY_GROUND, SILVALLY_ICE, SILVALLY_POISON, SILVALLY_PSYCHIC, SILVALLY_ROCK, SILVALLY_STEEL, SILVALLY_WATER, MINIOR, CORE_MINIOR, RED_CORE_MINIOR, ORANGE_CORE_MINIOR, YELLOW_CORE_MINIOR, GREEN_CORE_MINIOR, BLUE_CORE_MINIOR, INDIGO_CORE_MINIOR, VIOLET_CORE_MINIOR, METEOR_MINIOR, KOMALA, TURTONATOR, TOGEDEMARU, TOTEM_TOGEDEMARU, MIMIKYU, MIMIKYU_BUSTED, TOTEM_MIMIKYU, BUSTED_TOTEM_MIMIKYU, BRUXISH, DRAMPA, DHELMISE, JANGMO_O, HAKAMO_O, KOMMO_O, TOTEM_KOMMO_O, TAPU_KOKO, TAPU_LELE, TAPU_BULU, TAPU_FINI, COSMOG, COSMOEM, SOLGALEO, LUNALA, NIHILEGO, BUZZWOLE, PHEROMOSA, XURKITREE, CELESTEELA, KARTANA, GUZZLORD, NECROZMA, DUSK_MANE_NECROZMA, DAWN_WINGS_NECROZMA, ULTRA_NECROZMA, MAGEARNA, ORIGINAL_COLOR_MAGEARNA, MARSHADOW, POIPOLE, NAGANADEL, STAKATAKA, BLACEPHALON, ZERAORA, MELTAN, MELMETAL, GIGANTAMAX_MELMETAL, GROOKEY, THWACKEY, RILLABOOM, GIGANTAMAX_RILLABOOM, SCORBUNNY, RABOOT, CINDERACE, GIGANTAMAX_CINDERACE, SOBBLE, DRIZZILE, INTELEON, GIGANTAMAX_INTELEON, SKWOVET, GREEDENT, ROOKIDEE, CORVISQUIRE, CORVIKNIGHT, GIGANTAMAX_CORVIKNIGHT, BLIPBUG, DOTTLER, ORBEETLE, GIGANTAMAX_ORBEETLE, NICKIT, THIEVUL, GOSSIFLEUR, ELDEGOSS, WOOLOO, DUBWOOL, CHEWTLE, DREDNAW, GIGANTAMAX_DREDNAW, YAMPER, BOLTUND, ROLYCOLY, CARKOL, COALOSSAL, GIGANTAMAX_COALOSSAL, APPLIN, FLAPPLE, GIGANTAMAX_FLAPPLE, APPLETUN, GIGANTAMAX_APPLETUN, SILICOBRA, SANDACONDA, GIGANTAMAX_SANDACONDA, CRAMORANT, CRAMORANT_GULPING, CRAMORANT_GORGING, ARROKUDA, BARRASKEWDA, TOXEL, TOXTRICITY_AMPED, TOXTRICITY_LOW_KEY, GIGANTAMAX_TOXTRICITY_AMPED, GIGANTAMAX_TOXTRICITY_LOW_KEY, SIZZLIPEDE, CENTISKORCH, GIGANTAMAX_CENTISKORCH, CLOBBOPUS, GRAPPLOCT, SINISTEA, ANTIQUE_SINISTEA, POLTEAGEIST, ANTIQUE_POLTEAGEIST, HATENNA, HATTREM, HATTERENE, GIGANTAMAX_HATTERENE, IMPIDIMP, MORGREM, GRIMMSNARL, GIGANTAMAX_GRIMMSNARL, OBSTAGOON, PERRSERKER, CURSOLA, SIRFETCH_D, MR_RIME, RUNERIGUS, MILCERY, ALCREMIE, VANILLA_CREAM_ALCREMIE, RUBY_CREAM_ALCREMIE, MATCHA_CREAM_ALCREMIE, MINT_CREAM_ALCREMIE, LEMON_CREAM_ALCREMIE, SALTED_CREAM_ALCREMIE, RUBY_SWIRL_ALCREMIE, CARAMEL_SWIRL_ALCREMIE, RAINBOW_SWIRL_ALCREMIE, GIGANTAMAX_ALCREMIE, FALINKS, PINCURCHIN, SNOM, FROSMOTH, STONJOURNER, EISCUE, ICE_FACE_EISCUE, NOICE_FACE_EISCUE, MALE_INDEEDEE, FEMALE_INDEEDEE, MORPEKO, FULL_BELLY_MODE_MORPEKO, HANGRY_MODE_MORPEKO, CUFANT, COPPERAJAH, GIGANTAMAX_COPPERAJAH, DRACOZOLT, ARCTOZOLT, DRACOVISH, ARCTOVISH, DURALUDON, GIGANTAMAX_DURALUDON, DREEPY, DRAKLOAK, DRAGAPULT, HERO_OF_MANY_BATTLES_ZACIAN, CROWNED_SWORD_ZACIAN, HERO_OF_MANY_BATTLES_ZAMAZENTA, CROWNED_SHIELD_ZAMAZENTA, ETERNATUS, ETERNAMAX_ETERNATUS, KUBFU, SINGLE_STRIKE_STYLE_URSHIFU, RAPID_STRIKE_STYLE_URSHIFU, GIGANTAMAX_SINGLE_STRIKE_STYLE_URSHIFU, GIGANTAMAX_RAPID_STRIKE_STYLE_URSHIFU, ZARUDE, DADA_ZARUDE, REGIELEKI, REGIDRAGO, GLASTRIER, SPECTRIER, CALYREX, ICE_RIDER_CALYREX, SHADOW_RIDER_CALYREX, WYRDEER, KLEAVOR, URSALUNA, MALE_BASCULEGION, FEMALE_BASCULEGION, SNEASLER, OVERQWIL, INCARNATE_ENAMORUS, ENAMORUS_THERIAN, SPRIGATITO, FLORAGATO, MEOWSCARADA, FUECOCO, CROCALOR, SKELEDIRGE, QUAXLY, QUAXWELL, QUAQUAVAL, LECHONK, MALE_OINKOLOGNE, FEMALE_OINKOLOGNE, TAROUNTULA, SPIDOPS, NYMBLE, LOKIX, PAWMI, PAWMO, PAWMOT, TANDEMAUS, MAUSHOLD, FAMILY_OF_THREE_MAUSHOLD, FAMILY_OF_FOUR_MAUSHOLD, FIDOUGH, DACHSBUN, SMOLIV, DOLLIV, ARBOLIVA, GREEN_PLUMAGE_SQUAWKABILLY, BLUE_PLUMAGE_SQUAWKABILLY, YELLOW_PLUMAGE_SQUAWKABILLY, WHITE_PLUMAGE_SQUAWKABILLY, NACLI, NACLSTACK, GARGANACL, CHARCADET, ARMAROUGE, CERULEDGE, TADBULB, BELLIBOLT, WATTREL, KILOWATTREL, MASCHIFF, MABOSSTIFF, SHROODLE, GRAFAIAI, BRAMBLIN, BRAMBLEGHAST, TOEDSCOOL, TOEDSCRUEL, KLAWF, CAPSAKID, SCOVILLAIN, RELLOR, RABSCA, FLITTLE, ESPATHRA, TINKATINK, TINKATUFF, TINKATON, WIGLETT, WUGTRIO, BOMBIRDIER, FINIZEN, ZERO_PALAFIN, HERO_PALAFIN, VAROOM, REVAVROOM, CYCLIZAR, ORTHWORM, GLIMMET, GLIMMORA, GREAVARD, HOUNDSTONE, FLAMIGO, CETODDLE, CETITAN, VELUZA, DONDOZO, TATSUGIRI, CURLY_TATSUGIRI, DROOPY_TATSUGIRI, STRETCHY_TATSUGIRI, ANNIHILAPE, CLODSIRE, FARIGIRAF, DUDUNSPARCE, TWO_SEGMENT_DUDUNSPARCE, THREE_SEGMENT_DUDUNSPARCE, KINGAMBIT, GREAT_TUSK, SCREAM_TAIL, BRUTE_BONNET, FLUTTER_MANE, SLITHER_WING, SANDY_SHOCKS, IRON_TREADS, IRON_BUNDLE, IRON_HANDS, IRON_JUGULIS, IRON_MOTH, IRON_THORNS, FRIGIBAX, ARCTIBAX, BAXCALIBUR, CHEST_GIMMIGHOUL, ROAMING_GIMMIGHOUL, GHOLDENGO, WO_CHIEN, CHIEN_PAO, TING_LU, CHI_YU, ROARING_MOON, IRON_VALIANT, KORAIDON, MIRAIDON, WALKING_WAKE, IRON_LEAVES, SPECIES_TOTAL
  // clang-format on
};
}  // namespace pokesim::dex

////////////////////// END OF src/Types/Enums/Species.hpp //////////////////////

////////////////// START OF src/Utilities/MaxSizedVector.hpp ///////////////////

#include <array>
#include <cassert>
#include <cstdint>

namespace pokesim::internal {
template <typename T, std::uint8_t N>
class maxSizedVector : private std::array<T, N> {
  using base = std::array<T, N>;
  std::uint8_t used = 0;

 public:
  using base::begin;
  using base::cbegin;
  using base::crbegin;
  using base::max_size;

  maxSizedVector() : base() {}
  maxSizedVector(std::initializer_list<T> list) : base() {
    for (const T& item : list) {
      push_back(item);
    }
  }

  constexpr std::uint8_t size() const noexcept { return used; }
  constexpr bool empty() const noexcept { return used == 0; }
  constexpr typename base::const_reference front() const noexcept { return *base::begin(); }
  constexpr typename base::const_reference back() const noexcept { return N ? *(end() - 1) : *end(); }

  constexpr typename base::const_reference at(std::uint8_t pos) const {
    assert(pos < used);
    return base::at(pos);
  }

  constexpr typename base::const_reference operator[](std::uint8_t pos) const noexcept {
    assert(pos < used);
    return base::operator[](pos);
  }

  typename base::reference at(std::uint8_t pos) {
    assert(pos < used);
    return base::at(pos);
  }

  typename base::reference operator[](std::uint8_t pos) noexcept {
    assert(pos < used);
    return base::operator[](pos);
  }

  void push_back(const T& value) {
    base::at(used) = value;
    used++;
  }

  void pop_back() {
    if (empty()) return;
    used--;
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    base::at(used) = {args...};
    used++;
  }

  typename base::const_iterator end() const noexcept { return base::begin() + used; }
  typename base::const_iterator cend() const noexcept { return end(); }
  typename base::const_reverse_iterator rend() const noexcept { return const_reverse_iterator(end()); }
  typename base::const_reverse_iterator crend() const noexcept { return rend(); }
};
}  // namespace pokesim::internal

/////////////////// END OF src/Utilities/MaxSizedVector.hpp ////////////////////

///////////////////////// START OF src/Types/State.hpp /////////////////////////

#include <cstdint>
#include <type_traits>


namespace pokesim::types {
namespace internal {
const std::uint8_t MAX_TEAM_SIZE = 6U;
const std::uint8_t MAX_ACTIVE_POKEMON_SLOTS = 2U;
const std::uint8_t MAX_MOVE_SLOTS = 4U;
const std::uint8_t MAX_TARGETS = 3U;
}  // namespace internal

using stateId = std::underlying_type_t<entity>;

using battleTurn = std::uint16_t;

using cloneIndex = std::underlying_type_t<entity>;

using teamPositionIndex = std::uint8_t;
using moveSlotPosition = std::uint8_t;

template <typename T>
using teamPositions = pokesim::internal::maxSizedVector<T, internal::MAX_TEAM_SIZE>;
using teamOrder = types::teamPositions<types::teamPositionIndex>;

template <typename T>
using moveSlots = pokesim::internal::maxSizedVector<T, internal::MAX_MOVE_SLOTS>;

template <typename T>
using sideSlots = pokesim::internal::maxSizedVector<T, internal::MAX_ACTIVE_POKEMON_SLOTS>;

template <typename T>
using targets = pokesim::internal::maxSizedVector<T, internal::MAX_TARGETS>;
}  // namespace pokesim::types

////////////////////////// END OF src/Types/State.hpp //////////////////////////

/////////////// START OF src/Battle/Setup/PokemonStateSetup.hpp ////////////////

#include <type_traits>
#include <vector>


namespace pokesim {
// Tool to set properties of a Pokemon's state to an entity.
struct PokemonStateSetup : internal::StateSetupBase {
  PokemonStateSetup(types::registry& registry) : PokemonStateSetup(registry, registry.create()) {}
  PokemonStateSetup(types::registry& registry, types::entity entity) : StateSetupBase(registry, entity) {}

  /**
   * @brief Applies the defaults to the required properties for a Pokemon state.
   *
   * Some of the required properties are a blank `SpeciesName`, `Side`, and `Battle` component along with an
   * automatically set Id.
   */
  inline void initBlank();

  inline void setAutoID();
  inline void setID(types::stateId id);
  inline void setSpecies(dex::Species speciesName);

  inline void setSide(types::entity entity);
  inline void setBattle(types::entity entity);

  inline void setLevel(types::level level);
  inline void setGender(dex::Gender gender);
  inline void setAbility(dex::Ability ability);
  inline void setItem(dex::Item item);
  inline void setMoves(const std::vector<types::entity>& moveSlots);

  inline void setPostion(types::teamPositionIndex position);
  inline void setStatus(dex::Status status);

  inline void setNature(dex::Nature nature);
  inline void setEVs(types::ev hp, types::ev atk, types::ev def, types::ev spa, types::ev spd, types::ev spe);
  inline void setEVs(const Evs& evs);
  inline void setIVs(types::iv hp, types::iv atk, types::iv def, types::iv spa, types::iv spd, types::iv spe);
  inline void setIVs(const Ivs& ivs);

  template <typename BoostType>
  void setBoost(types::boost boost) {
    static_assert(
      std::is_same<AtkBoost, BoostType>() || std::is_same<DefBoost, BoostType>() ||
      std::is_same<SpaBoost, BoostType>() || std::is_same<SpdBoost, BoostType>() ||
      std::is_same<SpeBoost, BoostType>());
    handle.emplace<BoostType>(boost);
  };

  template <typename StatType>
  void setStat(types::stat stat) {
    static_assert(
      std::is_same<stat::Hp, StatType>() || std::is_same<stat::Atk, StatType>() ||
      std::is_same<stat::Def, StatType>() || std::is_same<stat::Spa, StatType>() ||
      std::is_same<stat::Spd, StatType>() || std::is_same<stat::Spe, StatType>());
    handle.emplace<StatType>(stat);
  };
};
}  // namespace pokesim

//////////////// END OF src/Battle/Setup/PokemonStateSetup.hpp /////////////////

////////////////// START OF src/Types/Enums/PlayerSideId.hpp ///////////////////

#include <cstdint>

namespace pokesim {
enum class PlayerSideId : std::uint8_t {
  NONE = 0,
  P1 = 1,
  P2 = 2,
};
}

/////////////////// END OF src/Types/Enums/PlayerSideId.hpp ////////////////////

//////////////////////// START OF src/Types/Random.hpp /////////////////////////

#include <cstdint>

namespace pokesim::types {
using probability = float;
using rngState = std::uint64_t;
using rngResult = std::uint32_t;
using percentChance = std::uint32_t;
}  // namespace pokesim::types

///////////////////////// END OF src/Types/Random.hpp //////////////////////////

//////////////// START OF src/Battle/Setup/BattleStateSetup.hpp ////////////////

#include <optional>
#include <vector>


namespace pokesim {
struct SimulateTurnOptions;
struct CalculateDamageOptions;
struct AnalyzeEffectOptions;

// Tool to set properties of a battle's state to an entity.
struct BattleStateSetup : internal::StateSetupBase {
  BattleStateSetup(types::registry& registry) : BattleStateSetup(registry, registry.create()) {}
  inline BattleStateSetup(types::registry& registry, types::entity entity);

  /**
   * @brief Applies the defaults to the required properties for a battle state.
   *
   * @details Some of the required properties and their defaults:
   * - Turn: 0
   * - Probability: 1
   * - Id: The number of existing battle states
   * - Sides: Unassigned entities for P1 and P2
   * - ActionQueue: An empty queue
   */
  inline void initBlank();

  inline void setAutoID();
  inline void setID(types::stateId id);
  inline void setSide(PlayerSideId sideID, types::entity sideEntity);

  // If a seed is not provided, the seed is set to a random number based on the current time in nanoseconds.
  inline void setRNGSeed(std::optional<types::rngState> seed = std::nullopt);
  inline void setActionQueue(const std::vector<types::entity>& queue);
  inline void setTurn(types::battleTurn turn);
  inline void setActivePokemon(types::entity activePokemon);
  inline void setCurrentActionTarget(types::targets<types::entity> actionTargets);
  inline void setCurrentActionSource(types::entity actionSource);
  inline void setCurrentActionMove(types::entity actionMove);
  inline void setProbability(types::probability probability);

  inline std::vector<BattleStateSetup> clone(std::optional<types::cloneIndex> cloneCount = std::nullopt);
};
}  // namespace pokesim

///////////////// END OF src/Battle/Setup/BattleStateSetup.hpp /////////////////

///////////////////////// START OF src/Types/Move.hpp //////////////////////////

#include <cstdint>

namespace pokesim::types {
using pp = std::uint8_t;
using basePower = std::uint8_t;
using baseAccuracy = std::uint8_t;
using moveHits = std::uint8_t;
using baseEffectChance = std::uint8_t;

using priority = std::int8_t;
using fractionalPriority = bool;
}  // namespace pokesim::types

////////////////////////// END OF src/Types/Move.hpp ///////////////////////////

///////////////// START OF src/Battle/Setup/MoveStateSetup.hpp /////////////////

namespace pokesim {
// Tool to set properties of a move's state to an entity.
struct MoveStateSetup : internal::StateSetupBase {
  MoveStateSetup(types::registry& registry) : MoveStateSetup(registry, registry.create()) {}
  MoveStateSetup(types::registry& registry, types::entity entity) : StateSetupBase(registry, entity) {}

  /**
   * @brief Applies the defaults to the required properties for a move state.
   *
   * Some of the required properties are a blank `MoveName`, `Pp`, and `MaxPp` component.
   */
  inline void initBlank();

  inline void setName(dex::Move moveName);
  inline void setPP(types::pp pp);
  inline void setMaxPP(types::pp maxPp);
};
}  // namespace pokesim

////////////////// END OF src/Battle/Setup/MoveStateSetup.hpp //////////////////

///////////////// START OF src/Battle/Setup/SideStateSetup.hpp /////////////////

#include <vector>


namespace pokesim {
struct PokemonStateSetup;

// Tool to set properties of a player's side state to an entity.
struct SideStateSetup : internal::StateSetupBase {
  SideStateSetup(types::registry& registry) : SideStateSetup(registry, registry.create()) {}
  SideStateSetup(types::registry& registry, types::entity entity) : StateSetupBase(registry, entity) {}
  /**
   * @brief Applies the defaults to the required properties for a player side's state.
   *
   * Some of the required properties are a blank `Battle`, `Side`, and `FoeSide` component.
   */
  inline void initBlank();

  inline void setTeam(std::vector<PokemonStateSetup>& team);
  inline void setOpponent(types::entity entity);
  inline void setBattle(types::entity entity);
};
}  // namespace pokesim

////////////////// END OF src/Battle/Setup/SideStateSetup.hpp //////////////////

//////////// START OF src/CalcDamage/Setup/CalcDamageInputSetup.hpp ////////////

namespace pokesim::calc_damage {
struct InputSetup {
 protected:
  types::handle handle;

 public:
  InputSetup(types::registry& registry, types::entity entity) : handle(registry, entity) {}
  InputSetup(types::registry& registry) : InputSetup(registry, registry.create()) {}

  inline void setAttacker(types::entity entity);
  inline void setDefender(types::entity entity);
  inline void setMove(dex::Move move);
  inline void setBattle(types::entity entity);

  types::entity entity() { return handle.entity(); }
};
}  // namespace pokesim::calc_damage

///////////// END OF src/CalcDamage/Setup/CalcDamageInputSetup.hpp /////////////

/////////// START OF src/Components/CalcDamage/AttackerDefender.hpp ////////////

namespace pokesim::calc_damage {
struct Attacker {
  types::entity val;
};

struct Defender {
  types::entity val;
};
}  // namespace pokesim::calc_damage

//////////// END OF src/Components/CalcDamage/AttackerDefender.hpp /////////////

//////////////////// START OF src/Components/Decisions.hpp /////////////////////

#include <optional>

namespace pokesim {
struct SlotDecision;
namespace types {
using slotDecisions = types::sideSlots<SlotDecision>;
}

struct SlotDecision {
  Slot sourceSlot = Slot::NONE;
  Slot targetSlot = Slot::NONE;

  bool megaEvolve = false;
  bool primalRevert = false;
  bool dynamax = false;
  bool terastallize = false;

  std::optional<dex::Move> moveChoice = std::nullopt;
  std::optional<dex::Item> itemChoice = std::nullopt;
};

struct SideDecision {
  PlayerSideId sideId = PlayerSideId::NONE;
  internal::variant<types::slotDecisions, types::teamOrder> decisions{};
};
}  // namespace pokesim

///////////////////// END OF src/Components/Decisions.hpp //////////////////////

//////////// START OF src/Components/EntityHolders/ActionQueue.hpp /////////////

#include <vector>

namespace pokesim {
// Contains the list of action entities queued up to be simulated for a battle's current turn.
struct ActionQueue {
  std::vector<types::entity> val{};
};
}  // namespace pokesim

///////////// END OF src/Components/EntityHolders/ActionQueue.hpp //////////////

/////////////// START OF src/Components/EntityHolders/Battle.hpp ///////////////

namespace pokesim {
// Contains the entity of a simulation's battle.
struct Battle {
  types::entity val{};
};
}  // namespace pokesim

//////////////// END OF src/Components/EntityHolders/Battle.hpp ////////////////

//////////////// START OF src/Components/EntityHolders/Side.hpp ////////////////

namespace pokesim {
// Contains the entity pointing to the player 1 or player 2 side of a battle.
struct Side {
  types::entity val{};
};
}  // namespace pokesim

///////////////// END OF src/Components/EntityHolders/Side.hpp /////////////////

/////////////// START OF src/Components/EntityHolders/Sides.hpp ////////////////

#include <array>

namespace pokesim {
// Contains the entities pointing to the two sides of a battle.
struct Sides {
  std::array<types::entity, 2U> val{};

  types::entity& p1() { return val[0]; };
  types::entity& p2() { return val[1]; };
  const types::entity& p1() const { return val[0]; };
  const types::entity& p2() const { return val[1]; };
};
}  // namespace pokesim

//////////////// END OF src/Components/EntityHolders/Sides.hpp /////////////////

///////////////// START OF src/Components/Tags/PokemonTags.hpp /////////////////

namespace pokesim::tags {
// Indicates the Pokemon is currently in a battle
struct ActivePokemon {};

struct SpeedUpdateRequired {};

struct Fainted {};
}  // namespace pokesim::tags

////////////////// END OF src/Components/Tags/PokemonTags.hpp //////////////////

/////////////// START OF src/Components/Tags/SimulationTags.hpp ////////////////

namespace pokesim::tags {
struct SimulateTurn {};
struct CalculateDamage {};
struct AnalyzeEffect {};
}  // namespace pokesim::tags

//////////////// END OF src/Components/Tags/SimulationTags.hpp /////////////////

///////////// START OF src/Components/EntityHolders/MoveEffect.hpp /////////////

namespace pokesim {
// Contains the entity of what the primary or secondary effect of a move does.
struct MoveEffect {
  bool primary = true;
  types::entity val{};
};
}  // namespace pokesim

////////////// END OF src/Components/EntityHolders/MoveEffect.hpp //////////////

/////////////////// START OF src/Types/Enums/ActionOrder.hpp ///////////////////

#include <cstdint>
#include <limits>
#include <type_traits>

namespace pokesim {
enum class ActionOrder : std::uint8_t {
  NONE = std::numeric_limits<std::underlying_type_t<ActionOrder>>::max(),
  TEAM = 1,
  START = 2,
  BEFORE_TURN = 4,
  ITEM = BEFORE_TURN,

  SWITCH = 103,

  MOVE = 200,

  RESIDUAL = 254,
};
}

//////////////////// END OF src/Types/Enums/ActionOrder.hpp ////////////////////

////////////////// START OF src/Types/Enums/AddedTargets.hpp ///////////////////

#include <cstdint>

namespace pokesim {
enum class AddedTargetOptions : std::uint8_t {
  NONE = 0b00000000,
  TARGET_ALLY = 0b00000001,
  USER_ALLY = 0b00000010,
  TARGET_SIDE = 0b00000100,
  USER_SIDE = 0b00001000,
  FIELD = 0b00010000,
};
}

/////////////////// END OF src/Types/Enums/AddedTargets.hpp ////////////////////

////////////////// START OF src/Types/Enums/BattleFormat.hpp ///////////////////

#include <cstdint>

namespace pokesim {
// Denotes if the simulated battle is a single or double battle
enum class BattleFormat : std::uint8_t {
  SINGLES_BATTLE_FORMAT = 1,
  DOUBLES_BATTLE_FORMAT = 2,
};
}  // namespace pokesim

/////////////////// END OF src/Types/Enums/BattleFormat.hpp ////////////////////

///////////////// START OF src/Types/Enums/DamageRollKind.hpp //////////////////

#include <cstdint>

namespace pokesim {
enum class DamageRollKind : std::uint8_t {
  NONE = 0b00000000,
  AVERAGE_DAMAGE = 0b00000001,
  ALL_DAMAGE_ROLES = 0b00000010,
  P1_MAX_DAMAGE = 0b00000100,
  P2_MAX_DAMAGE = 0b00001000,
  P1_MIN_DAMAGE = 0b00010000,
  P2_MIN_DAMAGE = 0b00100000,

  MAX_DAMAGE = P1_MAX_DAMAGE | P2_MAX_DAMAGE,
  MIN_DAMAGE = P1_MIN_DAMAGE | P2_MIN_DAMAGE,
};
}  // namespace pokesim

////////////////// END OF src/Types/Enums/DamageRollKind.hpp ///////////////////

////////////////// START OF src/Types/Enums/GameMechanics.hpp //////////////////

#include <cstdint>

namespace pokesim {
// Game the simulator is imitating the mechanics of
enum class GameMechanics : std::uint8_t {
  NONE = 0,
  SWORD_SHIELD,
  BRILLIANT_DIAMOND_SHINING_PEARL,
  SCARLET_VIOLET,
};
}  // namespace pokesim

/////////////////// END OF src/Types/Enums/GameMechanics.hpp ///////////////////

////////////////// START OF src/Types/Enums/MoveCategory.hpp ///////////////////

#include <cstdint>

namespace pokesim::dex {

enum class MoveCategory : std::uint8_t {
  PHYSICAL,
  SPECIAL,
  STATUS,
};
}  // namespace pokesim::dex

/////////////////// END OF src/Types/Enums/MoveCategory.hpp ////////////////////

/////////////////// START OF src/Types/Enums/MoveTarget.hpp ////////////////////

#include <cstdint>

namespace pokesim {
enum class MoveTarget : std::uint8_t {
  NONE = 0,
  ANY_SINGLE_TARGET,  // Targets one Pokemon other than the move's user
  ANY_SINGLE_FOE,     // Targets one foe
  ANY_SINGLE_ALLY,    // Targets one ally
  ALLY_OR_SELF,       // Targets either one ally or the move's user
  SELF,               // Targets the move's user

  ALL_FOES,         // Targets every active foe
  ALLIES_AND_FOES,  // Targets every active foe and the move user's allies
  ALLIES_AND_SELF,  // Targets the move user's allies and the user

  FOE_SIDE,   // Targets the opponent's side
  ALLY_SIDE,  // Targets the move user's side
  FIELD,      // Targets the field or all Pokemon at once

  ALLY_TEAM,    // Targets all non-fainted Pokemon on the user's team
  RETALIATION,  // Targets the last Pokemon to damage the user during a turn
  RANDOM_FOE,   // Targets a random foe
};
}

//////////////////// END OF src/Types/Enums/MoveTarget.hpp /////////////////////

////////////////////// START OF src/Types/Enums/Stat.hpp ///////////////////////

#include <cstdint>

namespace pokesim::dex {
// Pokemon stat abbreviated name
enum class Stat : std::uint8_t {
  HP = 0b000001,
  ATK = 0b000010,
  DEF = 0b000100,
  SPA = 0b001000,
  SPD = 0b010000,
  SPE = 0b100000,
  STAT_TOTAL = 6
  // SPC = SPA | SPD,
};
}  // namespace pokesim::dex

/////////////////////// END OF src/Types/Enums/Stat.hpp ////////////////////////

////////////////////// START OF src/Types/Enums/Type.hpp ///////////////////////

#include <cstdint>

namespace pokesim::dex {
// Pokemon type name
enum class Type : std::uint8_t {
  // clang-format off
  NO_TYPE = 0, NORMAL_TYPE, FIGHTING_TYPE, FLYING_TYPE, POISON_TYPE, GROUND_TYPE, ROCK_TYPE, BUG_TYPE, GHOST_TYPE, STEEL_TYPE, FIRE_TYPE, WATER_TYPE, GRASS_TYPE, ELECTRIC_TYPE, PSYCHIC_TYPE, ICE_TYPE, DRAGON_TYPE, DARK_TYPE, FAIRY_TYPE, TYPE_TOTAL
  // clang-format on
};
}  // namespace pokesim::dex

/////////////////////// END OF src/Types/Enums/Type.hpp ////////////////////////

/////////////////////// START OF src/Pokedex/Pokedex.hpp ///////////////////////

namespace pokesim {

/**
 * @brief Holds the data of each species, item, and move in a simulation.
 *
 * @details It is designed so only one instance of this class is needed while simulating a particular battle as the
 * entirety of all the move, item, and species, information that might be needed for a battle is known when it starts.
 * In theory, all the information within the game mechanics chosen or information for multiple battles could be loaded
 * into one Pokedex instance and used repeatedly, but that is not recommended as this class is optimized for instances
 * holding a small amount of data.
 */
class Pokedex {
 private:
  types::registry dexRegistry{};

  entt::dense_map<dex::Species, types::entity> speciesMap{};
  entt::dense_map<dex::Item, types::entity> itemsMap{};
  entt::dense_map<dex::Move, types::entity> movesMap{};
  entt::dense_map<dex::Ability, types::entity> abilitiesMap{};

  template <typename Build, typename T>
  inline void load(entt::dense_map<T, types::entity>& map, const entt::dense_set<T>& list, Build build);

  inline types::entity buildSpecies(dex::Species species, types::registry& registry, bool forActiveMove) const;
  inline types::entity buildMove(dex::Move move, types::registry& registry, bool forActiveMove) const;
  inline types::entity buildItem(dex::Item item, types::registry& registry, bool forActiveMove) const;
  inline types::entity buildAbility(dex::Ability ability, types::registry& registry, bool forActiveMove) const;

 public:
  /**
   * @brief The data for the Pokedex will be based the this game's data.
   * For example, if this is set to DIAMOND_PEARL_GAME_MECHANICS, Clefable's data will list it as a Normal type, but if
   * it's set to BRILLIANT_DIAMOND_SHINING_PEARL_GAME_MECHANICS, Clefable will be listed as a Fairy type.
   */
  const GameMechanics mechanics;

  Pokedex(GameMechanics mechanics_) : mechanics(mechanics_) {}

  /**
   * @brief Calls the load functions for a set of species to add their data to a Pokedex's storage.
   *
   * @details The Pokedex class is designed to store the minimum amount of data needed to run the simulation a Pokedex
   * instance is assigned to, so if a battle is being simulated where both sides have 6 Pokemon each, then this function
   * should only be called with a set of those 12 Pokemon's species.
   *
   * @note Only call this once per species per Pokedex instance.
   */
  inline void loadSpecies(const entt::dense_set<dex::Species>& speciesSet);

  /**
   * @brief Calls the load functions for a set of items to add their data to a Pokedex's storage.
   *
   * @details The Pokedex class is designed to store the minimum amount of data needed to run the simulation a Pokedex
   * instance is assigned to, so if a battle is being simulated where both sides have 6 Pokemon each, then this function
   * should only be called with a set of those 12 Pokemon's items.
   *
   * @note Only call this once per item per Pokedex instance.
   */
  inline void loadItems(const entt::dense_set<dex::Item>& itemSet);

  /**
   * @brief Calls the load functions for a set of moves to add their data to a Pokedex's storage.
   *
   * @details The Pokedex class is designed to store the minimum amount of data needed to run the simulation a Pokedex
   * instance is assigned to, so if one turn of a single battle is being simulated and the opposing Pokemon have 4
   * moves, then this function should only be called with a set of those 8 moves.
   *
   * @note Only call this once per move per Pokedex instance.
   */
  inline void loadMoves(const entt::dense_set<dex::Move>& moveSet);

  /**
   * @brief Calls the load functions for a set of abilities to add their data to a Pokedex's storage.
   *
   * @details The Pokedex class is designed to store the minimum amount of data needed to run the simulation a Pokedex
   * instance is assigned to, so if a battle is being simulated where both sides have 6 Pokemon each, then this function
   * should only be called with a set of those 12 Pokemon's abilities.
   *
   * @note Only call this once per ability per Pokedex instance.
   */
  inline void loadAbilities(const entt::dense_set<dex::Ability>& abilitySet);

  /**
   * @brief Returns references to the given dex data components for a species
   *
   * @warning
   * Attempting to get a component from a species that doesn't own it or an unloaded species results in undefined
   * behavior.
   *
   * @tparam T Type of data to retrieve. Valid types: TODO(aed3): Add in list
   */
  template <typename... T>
  auto getSpeciesData(dex::Species species) const {
    // ENTT_ASSERT(registry.all_of<T...>(speciesMap.at(species)), "Species does not contain at least one of the
    // component types");
    return dexRegistry.get<T...>(speciesMap.at(species));
  }

  /**
   * @brief Returns references to the given dex data components for an item
   *
   * @warning
   * Attempting to get a component from an item that doesn't own it or an unloaded item results in undefined behavior.
   *
   * @tparam T Type of data to retrieve. Valid types: TODO(aed3): Add in list
   */
  template <typename... T>
  auto getItemData(dex::Item item) const {
    return dexRegistry.get<T...>(itemsMap.at(item));
  }

  /**
   * @brief Returns references to the given dex data components for a move
   *
   * @warning
   * Attempting to get a component from a move that doesn't own it or an unloaded item results in undefined behavior.
   *
   * @tparam T Type of data to retrieve. Valid types: TODO(aed3): Add in list
   */
  template <typename... T>
  auto getMoveData(dex::Move move) const {
    return dexRegistry.get<T...>(movesMap.at(move));
  }

  template <typename... T>
  auto getEffectData(MoveEffect effect) const {
    return dexRegistry.get<T...>(effect.val);
  }

  template <typename... T>
  bool speciesHas(dex::Species species) const {
    return dexRegistry.all_of<T...>(speciesMap.at(species));
  }

  template <typename... T>
  bool itemHas(dex::Item item) const {
    return dexRegistry.all_of<T...>(itemsMap.at(item));
  }

  template <typename... T>
  bool moveHas(dex::Move move) const {
    return dexRegistry.all_of<T...>(movesMap.at(move));
  }

  template <typename... T>
  bool effectHas(MoveEffect effect) const {
    return dexRegistry.all_of<T...>(effect.val);
  }

  inline types::entity buildActionMove(dex::Move move, types::registry& registry) const;
};
}  // namespace pokesim

//////////////////////// END OF src/Pokedex/Pokedex.hpp ////////////////////////

//////////////////// START OF src/Components/Selection.hpp /////////////////////

#include <cstdint>

// TODO (aed3): THIS WILL NOT BE OK WHEN RUNNING MULTIPLE SIMULATION OBJECTS ON DIFFERENT THREADS!
namespace pokesim {
struct SelectedForViewBattle {
  static inline std::uint8_t depth = 0;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
struct SelectedForViewSide {
  static inline std::uint8_t depth = 0;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
struct SelectedForViewPokemon {
  static inline std::uint8_t depth = 0;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
struct SelectedForViewMove {
  static inline std::uint8_t depth = 0;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
}  // namespace pokesim

///////////////////// END OF src/Components/Selection.hpp //////////////////////

//////////////// START OF src/Simulation/SimulationOptions.hpp /////////////////

#include <cstdint>
#include <optional>

namespace pokesim {
class Simulation;

namespace simulate_turn {
struct Options {
  DamageRollKind damageRollsConsidered = DamageRollKind::AVERAGE_DAMAGE;
  bool applyChangesToInputBattle = true;
  types::percentChance randomChanceUpperLimit = 90;  // NOLINT(readability-magic-numbers)
  types::percentChance randomChanceLowerLimit = 10;  // NOLINT(readability-magic-numbers)
  types::probability branchProbabilityLowerLimit = 0.0F;

  // For Monte Carlo method. If no number is given, the number of branches
  // is determined by the number of random chance events that happen in the turn.
  // To get just one random outcome (aka using the simulator to just run a game),
  // set the value to 1
  std::optional<std::uint32_t> numberOfSamples = std::nullopt;
  bool makeBranchesOnRandomEvents() const { return !numberOfSamples.has_value(); }

  entt::delegate<void(Simulation&)> decisionCallback{};
  entt::delegate<void(Simulation&)> faintCallback{};
};
}  // namespace simulate_turn

namespace calc_damage {
struct Options {
  DamageRollKind damageRollsReturned = DamageRollKind::ALL_DAMAGE_ROLES;
};
}  // namespace calc_damage

namespace analyze_effect {
struct Options {
  // Whether to consider the multiplier even if the effect is already active (i.e. Rain will return a 1x multiplier
  // instead of 1.5x multiplier for Surf if this option is true and it's already raining)
  bool reconsiderActiveEffects = false;
  bool returnMultipliedKoChance = false;

  DamageRollKind damageRollsReturned = DamageRollKind::NONE;
};
}  // namespace analyze_effect
}  // namespace pokesim

///////////////// END OF src/Simulation/SimulationOptions.hpp //////////////////

//////////////////////// START OF src/Types/Damage.hpp /////////////////////////

#include <array>
#include <cstdint>

namespace pokesim::types {
namespace internal {
const std::uint8_t MAX_DAMAGE_ROLL_COUNT = 16U;
}

using damage = std::uint16_t;
using damageRolls = std::array<damage, internal::MAX_DAMAGE_ROLL_COUNT>;
}  // namespace pokesim::types

///////////////////////// END OF src/Types/Damage.hpp //////////////////////////

/////////////////// START OF src/Utilities/RegistryLoop.hpp ////////////////////

#include <type_traits>

namespace pokesim {
class Simulation;

namespace internal {
template <auto Function, typename... Tags>
struct RegistryLoop {
 private:
  template <class Signature>
  struct RegistryLoopInternal;

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation&, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...).each([](types::entity, auto&&... args) {
        Function(args...);
      });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation&, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...).each([](types::entity, auto&&... args) {
        Function(args...);
      });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(types::handle, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation&, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...)
        .each([&registry](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, args...);
        });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation&, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...)
        .each([&registry](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, args...);
        });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(const Simulation&, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation& simulation, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...).each([&simulation](types::entity, auto&&... args) {
        Function(simulation, args...);
      });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation& simulation, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...).each([&simulation](types::entity, auto&&... args) {
        Function(simulation, args...);
      });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(types::handle, const Simulation&, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation& simulation, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...)
        .each([&registry, &simulation](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, simulation, args...);
        });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation& simulation, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...)
        .each([&registry, &simulation](types::entity entity, auto&&... args) {
          Function(types::handle{registry, entity}, simulation, args...);
        });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(types::registry&, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation&, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...).each([&registry](types::entity, auto&&... args) {
        Function(registry, args...);
      });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation&, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...).each([&registry](types::entity, auto&&... args) {
        Function(registry, args...);
      });
    }
  };

  template <class ReturnType, class... Args>
  struct RegistryLoopInternal<ReturnType (*)(types::registry&, const Simulation&, Args...)> {
    template <typename... ViewArgs>
    static void view(types::registry& registry, const Simulation& simulation, const ViewArgs&... viewArgs) {
      registry.view<Tags..., std::decay_t<Args>...>(viewArgs...)
        .each([&registry, &simulation](types::entity, auto&&... args) { Function(registry, simulation, args...); });
    }

    template <typename... GroupArgs>
    static void group(types::registry& registry, const Simulation& simulation, const GroupArgs&... groupArgs) {
      registry.group<Tags..., std::decay_t<Args>...>(groupArgs...)
        .each([&registry, &simulation](types::entity, auto&&... args) { Function(registry, simulation, args...); });
    }
  };

  using FunctionSig = std::decay_t<decltype(Function)>;

 public:
  template <typename... ViewArgs>
  static void view(types::registry& registry, const Simulation& simulation, const ViewArgs&... viewArgs) {
    RegistryLoopInternal<FunctionSig>::view(registry, simulation, viewArgs...);
  }

  template <typename... GroupArgs>
  static void group(types::registry& registry, const Simulation& simulation, const GroupArgs&... groupArgs) {
    RegistryLoopInternal<FunctionSig>::group(registry, simulation, groupArgs...);
  }
};
}  // namespace internal
}  // namespace pokesim

//////////////////// END OF src/Utilities/RegistryLoop.hpp /////////////////////

//////////////////// START OF src/Simulation/Simulation.hpp ////////////////////

#include <initializer_list>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>


namespace pokesim {
struct SideStateSetup;
struct PokemonStateSetup;
struct BattleStateSetup;
class Pokedex;

namespace simulate_turn {
struct Results;
}
namespace calc_damage {
struct Results;
}
namespace analyze_effect {
struct Results;
}

/**
 * @brief The entry point for creating and running simulations.
 *
 * @details Each `Simulation` instance will only simulate for either single or double battles. This class is optimized
 * for running multiple simulations of the same battle, where each battle state has completed the same number of turns.
 */
class Simulation {
 public:
  struct MoveCreationInfo {
    dex::Move name = dex::Move::NO_MOVE;
    types::pp pp = 1;
    types::pp maxPp = 1;
  };

  struct PokemonCreationInfo {
    std::optional<types::stateId> id = std::nullopt;
    dex::Species species = dex::Species::MISSING_NO;
    dex::Item item = dex::Item::NO_ITEM;
    dex::Ability ability = dex::Ability::NO_ABILITY;
    dex::Gender gender = dex::Gender::NO_GENDER;
    dex::Status status = dex::Status::NO_STATUS;
    types::level level = 1;

    dex::Nature nature = dex::Nature::NO_NATURE;
    Evs evs;
    Ivs ivs;
    struct {
      types::stat hp = 1;
      types::stat atk = 1;
      types::stat def = 1;
      types::stat spa = 1;
      types::stat spd = 1;
      types::stat spe = 1;
    } stats;

    std::vector<MoveCreationInfo> moves{};
  };

  struct SideCreationInfo {
    std::vector<PokemonCreationInfo> team;
  };

  struct TurnDecisionInfo {
    SideDecision p1;
    SideDecision p2;
  };

  struct CalcDamageInputInfo {
    Slot attackerSlot = Slot::NONE;
    Slot defenderSlot = Slot::NONE;
    dex::Move move = dex::Move::NO_MOVE;
  };

  struct AnalyzeEffectInputInfo {
    Slot attackerSlot = Slot::NONE;
    Slot defenderSlot = Slot::NONE;
    types::effectEnum effect;
  };

  struct BattleCreationInfo {
    bool runWithSimulateTurn = false;
    bool runWithCalculateDamage = false;
    bool runWithAnalyzeEffect = false;
    types::battleTurn turn = 0;
    std::optional<types::rngState> rngSeed = std::nullopt;
    types::probability probability = 1;

    SideCreationInfo p1;
    SideCreationInfo p2;

    std::vector<TurnDecisionInfo> decisionsToSimulate;
    std::vector<CalcDamageInputInfo> damageCalculations;
    std::vector<AnalyzeEffectInputInfo> effectsToAnalyze;
  };

 private:
  template <typename Selected, auto Function, typename... Tags, typename... ViewArgs>
  void viewForSelected(const ViewArgs&... viewArgs) {
    if (Selected::depth) {
      internal::RegistryLoop<Function, Selected, Tags...>::view(registry, *this, viewArgs...);
    }
    else {
      view<Function, Tags...>(viewArgs...);
    }
  }

  template <typename Selected, auto Function, typename... Tags, typename... GroupArgs>
  void groupForSelected(const GroupArgs&... groupArgs) {
    if (Selected::depth) {
      internal::RegistryLoop<Function, Selected, Tags...>::group(registry, *this, groupArgs...);
    }
    else {
      group<Function, Tags...>(groupArgs...);
    }
  }

 public:
  template <auto Function, typename... Tags, typename... ViewArgs>
  void viewForSelectedBattles(const ViewArgs&... viewArgs) {
    viewForSelected<SelectedForViewBattle, Function, Tags...>(viewArgs...);
  }

  template <auto Function, typename... Tags, typename... GroupArgs>
  void groupForSelectedBattles(const GroupArgs&... groupArgs) {
    groupForSelected<SelectedForViewBattle, Function, Tags...>(groupArgs...);
  }

  template <auto Function, typename... Tags, typename... ViewArgs>
  void viewForSelectedSides(const ViewArgs&... viewArgs) {
    viewForSelected<SelectedForViewSide, Function, Tags...>(viewArgs...);
  }

  template <auto Function, typename... Tags, typename... GroupArgs>
  void groupForSelectedSides(const GroupArgs&... groupArgs) {
    groupForSelected<SelectedForViewSide, Function, Tags...>(groupArgs...);
  }

  template <auto Function, typename... Tags, typename... ViewArgs>
  void viewForSelectedPokemon(const ViewArgs&... viewArgs) {
    viewForSelected<SelectedForViewPokemon, Function, Tags...>(viewArgs...);
  }

  template <auto Function, typename... Tags, typename... GroupArgs>
  void groupForSelectedPokemon(const GroupArgs&... groupArgs) {
    groupForSelected<SelectedForViewPokemon, Function, Tags...>(groupArgs...);
  }

  template <auto Function, typename... Tags, typename... ViewArgs>
  void viewForSelectedMoves(const ViewArgs&... viewArgs) {
    viewForSelected<SelectedForViewMove, Function, Tags...>(viewArgs...);
  }

  template <auto Function, typename... Tags, typename... GroupArgs>
  void groupForSelectedMoves(const GroupArgs&... groupArgs) {
    groupForSelected<SelectedForViewMove, Function, Tags...>(groupArgs...);
  }

  template <auto Function, typename... Tags, typename... ViewArgs>
  void view(const ViewArgs&... viewArgs) {
    internal::RegistryLoop<Function, Tags...>::view(registry, *this, viewArgs...);
  }

  template <auto Function, typename... Tags, typename... GroupArgs>
  void group(const GroupArgs&... groupArgs) {
    internal::RegistryLoop<Function, Tags...>::group(registry, *this, groupArgs...);
  }

  inline std::vector<types::entity> selectedBattleEntities();

 private:
  inline std::vector<types::entity> createInitialMoves(const std::vector<MoveCreationInfo>& moveDataList);
  inline PokemonStateSetup createInitialPokemon(const PokemonCreationInfo& pokemonData);
  inline void createInitialSide(SideStateSetup sideSetup, const SideCreationInfo& sideData);

  inline void createInitialTurnDecision(BattleStateSetup battleStateSetup, const TurnDecisionInfo& turnDecisionData);
  inline void createCalcDamageInput(BattleStateSetup battleStateSetup, const CalcDamageInputInfo& damageCalcInputData);
  inline void createAnalyzeEffectInput(
    BattleStateSetup battleStateSetup, const AnalyzeEffectInputInfo& analyzeEffectInputData);

  inline std::tuple<SideStateSetup, SideStateSetup> createInitialBattle(
    BattleStateSetup battleStateSetup, const BattleCreationInfo& battleData);

 public:
  const BattleFormat battleFormat = BattleFormat::SINGLES_BATTLE_FORMAT;
  const Pokedex& pokedex;
  types::registry registry{};

  simulate_turn::Options simulateTurnOptions;
  calc_damage::Options calculateDamageOptions;
  analyze_effect::Options analyzeEffectOptions;

  inline Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_);

  // Load information about any number of battle states into the simulation's registry.
  inline void createInitialStates(std::initializer_list<BattleCreationInfo> battleDataList);

  inline void run();

  inline simulate_turn::Results simulateTurn(std::optional<simulate_turn::Options> options = std::nullopt);
  inline calc_damage::Results calculateDamage(std::optional<calc_damage::Options> options = std::nullopt);
  inline analyze_effect::Results analyzeEffect(std::optional<analyze_effect::Options> options = std::nullopt);

  inline simulate_turn::Results simulateTurn(
    std::initializer_list<BattleCreationInfo> battleDataList,
    std::optional<simulate_turn::Options> options = std::nullopt);

  inline calc_damage::Results calculateDamage(
    std::initializer_list<BattleCreationInfo> battleDataList,
    std::optional<calc_damage::Options> options = std::nullopt);

  inline analyze_effect::Results analyzeEffect(
    std::initializer_list<BattleCreationInfo> battleDataList,
    std::optional<analyze_effect::Options> options = std::nullopt);

  inline void clearAllResults();
  inline void clearSimulateTurnResults();
  inline void clearCalculateDamageResults();
  inline void clearAnalyzeEffectResults();
};
}  // namespace pokesim

///////////////////// END OF src/Simulation/Simulation.hpp /////////////////////

///////////////// START OF src/Simulation/SimulationSetup.cpp //////////////////

#include <cstddef>
#include <initializer_list>
#include <utility>
#include <vector>


namespace pokesim {
Simulation::Simulation(const Pokedex& pokedex_, BattleFormat battleFormat_)
    : battleFormat(battleFormat_), pokedex(pokedex_) {}

std::vector<types::entity> Simulation::createInitialMoves(const std::vector<MoveCreationInfo>& moveDataList) {
  std::vector<types::entity> moveEntities{};
  moveEntities.reserve(moveDataList.size());

  for (const MoveCreationInfo& moveData : moveDataList) {
    MoveStateSetup moveSetup(registry);
    moveSetup.setName(moveData.name);
    moveSetup.setPP(moveData.pp);
    moveSetup.setMaxPP(moveData.maxPp);
    moveEntities.push_back(moveSetup.entity());
  }

  return moveEntities;
}

PokemonStateSetup Simulation::createInitialPokemon(const PokemonCreationInfo& pokemonData) {
  PokemonStateSetup pokemonSetup(registry);
  if (pokemonData.id.has_value()) {
    pokemonSetup.setID(pokemonData.id.value());
  }
  else {
    pokemonSetup.setAutoID();
  }

  pokemonSetup.setSpecies(pokemonData.species);
  pokemonSetup.setLevel(pokemonData.level);
  if (pokemonData.gender != dex::Gender::NO_GENDER) pokemonSetup.setGender(pokemonData.gender);
  if (pokemonData.ability != dex::Ability::NO_ABILITY) pokemonSetup.setAbility(pokemonData.ability);
  if (pokemonData.item != dex::Item::NO_ITEM) pokemonSetup.setItem(pokemonData.item);
  if (pokemonData.nature != dex::Nature::NO_NATURE) pokemonSetup.setNature(pokemonData.nature);
  if (pokemonData.status != dex::Status::NO_STATUS) pokemonSetup.setStatus(pokemonData.status);

  pokemonSetup.setEVs(pokemonData.evs);
  pokemonSetup.setIVs(pokemonData.ivs);
  pokemonSetup.setStat<stat::Hp>(pokemonData.stats.hp);
  pokemonSetup.setStat<stat::Atk>(pokemonData.stats.atk);
  pokemonSetup.setStat<stat::Def>(pokemonData.stats.def);
  pokemonSetup.setStat<stat::Spa>(pokemonData.stats.spa);
  pokemonSetup.setStat<stat::Spd>(pokemonData.stats.spd);
  pokemonSetup.setStat<stat::Spe>(pokemonData.stats.spe);
  pokemonSetup.setProperty<tags::SpeedUpdateRequired>();

  return pokemonSetup;
}

void Simulation::createInitialSide(SideStateSetup sideSetup, const SideCreationInfo& sideData) {
  std::vector<PokemonStateSetup> pokemonSetupList;
  pokemonSetupList.reserve(sideData.team.size());

  for (const PokemonCreationInfo& pokemonData : sideData.team) {
    PokemonStateSetup pokemonSetup = createInitialPokemon(pokemonData);
    std::vector<types::entity> moveEntities = createInitialMoves(pokemonData.moves);
    pokemonSetup.setMoves(moveEntities);

    pokemonSetupList.push_back(pokemonSetup);
  }

  sideSetup.setTeam(pokemonSetupList);
}

std::tuple<SideStateSetup, SideStateSetup> Simulation::createInitialBattle(
  BattleStateSetup battleStateSetup, const BattleCreationInfo& battleData) {
  battleStateSetup.setAutoID();
  battleStateSetup.setTurn(battleData.turn);
  battleStateSetup.setRNGSeed(battleData.rngSeed);
  battleStateSetup.setProbability(battleData.probability);

  if (battleData.runWithSimulateTurn) {
    battleStateSetup.setProperty<tags::SimulateTurn>();
  }
  if (battleData.runWithCalculateDamage) {
    battleStateSetup.setProperty<tags::CalculateDamage>();
  }
  if (battleData.runWithAnalyzeEffect) {
    battleStateSetup.setProperty<tags::AnalyzeEffect>();
  }

  SideStateSetup p1SideSetup(registry);
  SideStateSetup p2SideSetup(registry);

  types::entity battleEntity = battleStateSetup.entity();
  types::entity p1Entity = p1SideSetup.entity();
  types::entity p2Entity = p2SideSetup.entity();

  battleStateSetup.setSide(PlayerSideId::P1, p1Entity);
  battleStateSetup.setSide(PlayerSideId::P2, p2Entity);

  p1SideSetup.setOpponent(p2Entity);
  p2SideSetup.setOpponent(p1Entity);

  p1SideSetup.setBattle(battleEntity);
  p2SideSetup.setBattle(battleEntity);

  return {p1SideSetup, p2SideSetup};
}

void Simulation::createInitialTurnDecision(
  BattleStateSetup battleStateSetup, const TurnDecisionInfo& turnDecisionData) {
  types::handle battleHandle{registry, battleStateSetup.entity()};
  const Sides& sides = battleHandle.get<Sides>();

  registry.emplace<SideDecision>(sides.p1(), turnDecisionData.p1);
  registry.emplace<SideDecision>(sides.p2(), turnDecisionData.p2);
}

void Simulation::createCalcDamageInput(
  BattleStateSetup battleStateSetup, const CalcDamageInputInfo& damageCalcInputData) {
  ENTT_ASSERT(damageCalcInputData.attackerSlot != Slot::NONE, "A damage calculation must have a attacker");
  ENTT_ASSERT(damageCalcInputData.defenderSlot != Slot::NONE, "A damage calculation must have a defender");
  ENTT_ASSERT(damageCalcInputData.move != dex::Move::NO_MOVE, "A damage calculation must have a move");

  const Sides& sides = registry.get<Sides>(battleStateSetup.entity());
  types::entity attackerEntity = slotToPokemonEntity(registry, sides, damageCalcInputData.attackerSlot);
  types::entity defenderEntity = slotToPokemonEntity(registry, sides, damageCalcInputData.defenderSlot);

  calc_damage::InputSetup inputSetup(registry);
  inputSetup.setAttacker(attackerEntity);
  inputSetup.setDefender(defenderEntity);
  inputSetup.setMove(damageCalcInputData.move);
  inputSetup.setBattle(battleStateSetup.entity());
}

void Simulation::createAnalyzeEffectInput(
  BattleStateSetup battleStateSetup, const AnalyzeEffectInputInfo& analyzeEffectInputData) {
  ENTT_ASSERT(analyzeEffectInputData.attackerSlot != Slot::NONE, "An effect analysis must have a attacker");
  ENTT_ASSERT(analyzeEffectInputData.defenderSlot != Slot::NONE, "An effect analysis must have a defender");
  ENTT_ASSERT(!analyzeEffectInputData.effect.empty(), "An effect analysis must have an effect");

  const Sides& sides = registry.get<Sides>(battleStateSetup.entity());
  types::entity attackerEntity = slotToPokemonEntity(registry, sides, analyzeEffectInputData.attackerSlot);
  types::entity defenderEntity = slotToPokemonEntity(registry, sides, analyzeEffectInputData.defenderSlot);

  analyze_effect::InputSetup inputSetup(registry);
  inputSetup.setAttacker(attackerEntity);
  inputSetup.setDefender(defenderEntity);
  inputSetup.setEffect(analyzeEffectInputData.effect);
  inputSetup.setBattle(battleStateSetup.entity());
}

void Simulation::createInitialStates(std::initializer_list<BattleCreationInfo> battleDataList) {
  for (const BattleCreationInfo& battleData : battleDataList) {
    BattleStateSetup battleStateSetup(registry);
    auto [p1SideSetup, p2SideSetup] = createInitialBattle(battleStateSetup, battleData);

    createInitialSide(p1SideSetup, battleData.p1);
    createInitialSide(p2SideSetup, battleData.p2);

    if (!battleData.decisionsToSimulate.empty()) {
      ENTT_ASSERT(
        battleData.decisionsToSimulate.size() < std::numeric_limits<types::cloneIndex>::max(),
        "Cannot make more clones than their are entities");

      types::cloneIndex cloneCount = (types::cloneIndex)(battleData.decisionsToSimulate.size() - 1);
      if (cloneCount) {
        std::vector<BattleStateSetup> clones = battleStateSetup.clone(cloneCount);

        for (types::cloneIndex i = 0; i < cloneCount; i++) {
          createInitialTurnDecision(clones[i], battleData.decisionsToSimulate[i]);
          clones[i].setID(i);
        }
      }

      createInitialTurnDecision(battleStateSetup, *battleData.decisionsToSimulate.rbegin());
      battleStateSetup.setID(cloneCount);
    }

    for (const CalcDamageInputInfo& damageCalcInputData : battleData.damageCalculations) {
      createCalcDamageInput(battleStateSetup, damageCalcInputData);
    }

    for (const AnalyzeEffectInputInfo& analyzeEffectInputData : battleData.effectsToAnalyze) {
      createAnalyzeEffectInput(battleStateSetup, analyzeEffectInputData);
    }
  }
}
}  // namespace pokesim

////////////////// END OF src/Simulation/SimulationSetup.cpp ///////////////////

//////////////// START OF src/Simulation/SimulationResults.hpp /////////////////

#include <array>
#include <vector>

namespace pokesim {
class Simulation;

namespace simulate_turn {
struct TurnOutcomeBattles {
  std::vector<types::entity> turnOutcomeBattles;
};

struct Results {
  inline types::view<TurnOutcomeBattles> turnOutcomeBattlesResults() const;

  inline Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace simulate_turn

namespace calc_damage {
struct MaxDamage {
  types::damage maxDamage = 0;
};

struct MinUsesUntilKo {
  types::damage minUsesUntilKo = 0;
};

struct AttackerHpRecovered {
  types::stat hpRecovered = 0;
};

struct AttackerHpLost {
  types::stat hpLost = 0;
};

struct HitCount {
  types::moveHits hitCount = 1;
};

struct Results {
  inline types::view<MaxDamage> maxDamageResults() const;
  inline types::view<MinUsesUntilKo> minUsesUntilKoResults() const;
  inline types::view<AttackerHpRecovered> hpRecoveredResults() const;
  inline types::view<AttackerHpLost> hpLostResults() const;
  inline types::view<HitCount> hitCountResults() const;

  inline Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace calc_damage

namespace analyze_effect {
struct EffectMultiplier {
  float effectMultiplier = 1.0F;
};

struct MultipliedMaxDamage {
  types::damage multipliedMaxDamage;
};

struct MultipliedDamageRolls {
  types::damageRolls multipliedDamageRolls;
};

struct MultipliedKoChance {
  float multipliedKoChance = 1.0F;
};

struct Results {
  inline types::view<EffectMultiplier> effectMultiplierResults() const;
  inline types::view<MultipliedMaxDamage> multipliedMaxDamageResults() const;
  inline types::view<MultipliedDamageRolls> multipliedDamageRollsResults() const;
  inline types::view<MultipliedKoChance> multipliedKoChanceResults() const;

  inline Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace analyze_effect
}  // namespace pokesim

///////////////// END OF src/Simulation/SimulationResults.hpp //////////////////

//////////////// START OF src/Simulation/SimulationResults.cpp /////////////////

namespace pokesim {
namespace simulate_turn {
Results::Results(const Simulation& simulation_) : simulation(simulation_) {}

types::view<TurnOutcomeBattles> Results::turnOutcomeBattlesResults() const {
  return simulation.registry.view<TurnOutcomeBattles>();
}
}  // namespace simulate_turn

namespace calc_damage {
Results::Results(const Simulation& simulation_) : simulation(simulation_) {}

types::view<MaxDamage> Results::maxDamageResults() const {
  return simulation.registry.view<MaxDamage>();
}

types::view<MinUsesUntilKo> Results::minUsesUntilKoResults() const {
  return simulation.registry.view<MinUsesUntilKo>();
}

types::view<AttackerHpRecovered> Results::hpRecoveredResults() const {
  return simulation.registry.view<AttackerHpRecovered>();
}

types::view<AttackerHpLost> Results::hpLostResults() const {
  return simulation.registry.view<AttackerHpLost>();
}

types::view<HitCount> Results::hitCountResults() const {
  return simulation.registry.view<HitCount>();
}
}  // namespace calc_damage

namespace analyze_effect {
Results::Results(const Simulation& simulation_) : simulation(simulation_) {}

types::view<EffectMultiplier> Results::effectMultiplierResults() const {
  return simulation.registry.view<EffectMultiplier>();
}

types::view<MultipliedMaxDamage> Results::multipliedMaxDamageResults() const {
  return simulation.registry.view<MultipliedMaxDamage>();
}

types::view<MultipliedDamageRolls> Results::multipliedDamageRollsResults() const {
  return simulation.registry.view<MultipliedDamageRolls>();
}

types::view<MultipliedKoChance> Results::multipliedKoChanceResults() const {
  return simulation.registry.view<MultipliedKoChance>();
}
}  // namespace analyze_effect
}  // namespace pokesim

///////////////// END OF src/Simulation/SimulationResults.cpp //////////////////

///////////////// START OF src/AnalyzeEffect/AnalyzeEffect.hpp /////////////////

namespace pokesim {
class Simulation;

namespace analyze_effect {
inline void run(Simulation& simulation);

inline void assignAttackerDefender(Simulation& simulation);
inline void createAppliedEffectBattles(Simulation& simulation);
inline void removeConsideredEffect(Simulation& simulation);
inline void reapplyConsideredEffect(Simulation& simulation);
inline void postRunCleanup(Simulation& simulation);
}  // namespace analyze_effect
}  // namespace pokesim

////////////////// END OF src/AnalyzeEffect/AnalyzeEffect.hpp //////////////////

//////////////////// START OF src/CalcDamage/CalcDamage.hpp ////////////////////

namespace pokesim {
class Simulation;

namespace calc_damage {
inline void run(Simulation& simulation);

inline void modifyDamageWithTypes(Simulation& simulation);
inline void getDamageRole(Simulation& simulation);

inline void criticalHitRandomChance(Simulation& simulation);
inline void getCritMultiplier(Simulation& simulation);
inline void getDamage(Simulation& simulation);
}  // namespace calc_damage
}  // namespace pokesim

///////////////////// END OF src/CalcDamage/CalcDamage.hpp /////////////////////

////////////////// START OF src/SimulateTurn/SimulateTurn.hpp //////////////////

namespace pokesim {
class Simulation;
struct Battle;
struct CurrentActionTargets;

namespace simulate_turn {
inline void run(Simulation& simulation);
inline void runCurrentAction(Simulation& simulation);
inline void nextTurn(Simulation& simulation);

inline void runBeforeTurnAction(Simulation& simulation);
inline void runMoveAction(Simulation& simulation);
inline void runResidualAction(Simulation& simulation);

inline void addTargetAllyToTargets(types::registry& registry, const Battle& battle);
inline void addUserAllyToTargets(types::registry& registry, const Battle& battle);
inline void resolveMoveTargets(CurrentActionTargets&);
inline void getMoveTargets(Simulation& simulation);

inline void useMove(Simulation& simulation);
}  // namespace simulate_turn
}  // namespace pokesim

/////////////////// END OF src/SimulateTurn/SimulateTurn.hpp ///////////////////

//////////////////// START OF src/Simulation/Simulation.cpp ////////////////////

namespace pokesim {
void Simulation::clearAllResults() {
  clearSimulateTurnResults();
  clearCalculateDamageResults();
  clearAnalyzeEffectResults();
}

void Simulation::clearSimulateTurnResults() {
  registry.clear<simulate_turn::TurnOutcomeBattles>();
}

void Simulation::clearCalculateDamageResults() {
  registry.clear<calc_damage::MaxDamage>();
  registry.clear<calc_damage::MaxDamage>();
  registry.clear<calc_damage::MinUsesUntilKo>();
  registry.clear<calc_damage::AttackerHpRecovered>();
  registry.clear<calc_damage::AttackerHpLost>();
  registry.clear<calc_damage::HitCount>();
}

void Simulation::clearAnalyzeEffectResults() {
  registry.clear<analyze_effect::EffectMultiplier>();
  registry.clear<analyze_effect::MultipliedMaxDamage>();
  registry.clear<analyze_effect::MultipliedDamageRolls>();
  registry.clear<analyze_effect::MultipliedKoChance>();
}

simulate_turn::Results Simulation::simulateTurn(std::optional<simulate_turn::Options> options) {
  if (options.has_value()) {
    simulateTurnOptions = options.value();
  }

  simulate_turn::run(*this);

  return {*this};
}

calc_damage::Results Simulation::calculateDamage(std::optional<calc_damage::Options> options) {
  if (options.has_value()) {
    calculateDamageOptions = options.value();
  }

  calc_damage::run(*this);

  return {*this};
}

analyze_effect::Results Simulation::analyzeEffect(std::optional<analyze_effect::Options> options) {
  if (options.has_value()) {
    analyzeEffectOptions = options.value();
  }

  analyze_effect::run(*this);

  return {*this};
}

simulate_turn::Results Simulation::simulateTurn(
  std::initializer_list<BattleCreationInfo> battleDataList, std::optional<simulate_turn::Options> options) {
  createInitialStates(battleDataList);
  return simulateTurn(options);
}

calc_damage::Results Simulation::calculateDamage(
  std::initializer_list<BattleCreationInfo> battleDataList, std::optional<calc_damage::Options> options) {
  createInitialStates(battleDataList);
  return calculateDamage(options);
}

analyze_effect::Results Simulation::analyzeEffect(
  std::initializer_list<BattleCreationInfo> battleDataList, std::optional<analyze_effect::Options> options) {
  createInitialStates(battleDataList);
  return analyzeEffect(options);
}

void Simulation::run() {
  clearAllResults();
  simulateTurn();
  calculateDamage();
  analyzeEffect();
}

std::vector<types::entity> Simulation::selectedBattleEntities() {
  if (SelectedForViewBattle::depth) {
    auto view = registry.view<SelectedForViewBattle, Sides>();
    return {view.begin(), view.end()};
  }

  auto view = registry.view<Sides>();
  return {view.begin(), view.end()};
}
}  // namespace pokesim

///////////////////// END OF src/Simulation/Simulation.cpp /////////////////////

///////////////// START OF src/Components/Tags/StatusTags.hpp //////////////////

// TODO(aed3): Make this auto generated

namespace pokesim::status::tags {
struct Burn {};
struct Freeze {};
struct Paralysis {};
struct Poison {};
struct Sleep {};
struct Toxic {};

// Assigns a status' tag to a handle
inline void enumToTag(dex::Status status, types::handle& handle);
}  // namespace pokesim::status::tags

////////////////// END OF src/Components/Tags/StatusTags.hpp ///////////////////

////////////////// START OF src/Pokedex/Abilities/Static.hpp ///////////////////

#include <string_view>

namespace pokesim {
class Simulation;

namespace stat {
struct EffectiveSpeed;
}
}  // namespace pokesim

namespace pokesim::dex {
namespace internal {
struct StaticEvents {
  inline static void onDamagingHit(Simulation& simulation);
  inline static void onModifySpe(stat::EffectiveSpeed& effectiveSpeed);
};
}  // namespace internal

template <GameMechanics>
struct Static : internal::StaticEvents {
  static constexpr dex::Ability name = dex::Ability::STATIC;

  struct Strings {
    static constexpr std::string_view name = "Static";
    static constexpr std::string_view smogonId = "static";
  };
};

namespace latest {
using Static = dex::Static<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Abilities/Static.hpp ////////////////////

///////////////////// START OF src/Simulation/RunEvent.hpp /////////////////////

namespace pokesim {
class Simulation;

inline void runAccuracyEvent(Simulation& simulation);
inline void runModifyAccuracyEvent(Simulation& simulation);
inline void runModifyCritRatioEvent(Simulation& simulation);
inline void runBasePowerEvent(Simulation& simulation);
inline void runDamagingHitEvent(Simulation& simulation);

inline void runModifySpe(Simulation& simulation);
}  // namespace pokesim

////////////////////// END OF src/Simulation/RunEvent.hpp //////////////////////

///////////////////// START OF src/Simulation/RunEvent.cpp /////////////////////

namespace pokesim {
void runAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyCritRatioEvent(Simulation& /*simulation*/) {}

void runBasePowerEvent(Simulation& /*simulation*/) {}

void runDamagingHitEvent(Simulation& simulation) {
  dex::latest::Static::onDamagingHit(simulation);
}

void runModifySpe(Simulation& simulation) {
  // simulation.view<function, ...Tags>();

  simulation.viewForSelectedPokemon<dex::latest::Static::onModifySpe, status::tags::Paralysis>(
    /*entt::exclude_t<ability::tags::QuickFeet>{}*/);
}
}  // namespace pokesim

////////////////////// END OF src/Simulation/RunEvent.cpp //////////////////////

///////////////////// START OF src/Battle/Clone/Clone.hpp //////////////////////

#include <optional>

namespace pokesim {
struct CloneTo;

inline types::ClonedEntityMap clone(types::registry& registry, std::optional<types::cloneIndex> cloneCount);

namespace internal {
inline void cloneEntity(
  types::entity src, types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);

inline void cloneBattle(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);
inline void cloneSide(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);
inline void cloneActionQueue(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);
inline void clonePokemon(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);
inline void cloneMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);

inline void remapEntity(types::entity& entity, const CloneTo& cloneTo, const types::ClonedEntityMap& entityMap);
template <typename Component, typename GetEntity>
inline void remapEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap, GetEntity getEntity);
template <typename Component, typename GetEntityList>
inline void remapEntityListMembers(
  types::registry& registry, const types::ClonedEntityMap& entityMap, GetEntityList getEntityList);

inline void remapActionQueueEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
inline void remapBattleEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
inline void remapFoeSideEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
inline void remapMoveEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
inline void remapMoveEffectEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
inline void remapMoveSlotsEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
inline void remapPokemonEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
inline void remapSideEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
inline void remapSidesEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
inline void remapTeamEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
inline void remapActionTargetsEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
}  // namespace internal
}  // namespace pokesim

////////////////////// END OF src/Battle/Clone/Clone.hpp ///////////////////////

///////////////// START OF src/Components/CloneFromCloneTo.hpp /////////////////

namespace pokesim {
namespace tags {
struct CloneFrom {};
}  // namespace tags

struct CloneTo {
  types::cloneIndex val{};
};
}  // namespace pokesim

////////////////// END OF src/Components/CloneFromCloneTo.hpp //////////////////

///////////////////// START OF src/Components/RNGSeed.hpp //////////////////////

namespace pokesim {
struct RngSeed {
  types::rngState val = 1;
};
}  // namespace pokesim

////////////////////// END OF src/Components/RNGSeed.hpp ///////////////////////

///////////////// START OF src/Components/RandomEventCheck.hpp /////////////////

#include <cstdint>

namespace pokesim {
namespace internal {
// Moves such as Metronome and Psywave, the Forewarn ability, and damage rolls have more random options than 4, but
// those cases will be handled separately
const std::uint8_t MAX_TYPICAL_RANDOM_OPTIONS = 5U;
}  // namespace internal

template <std::uint8_t RANDOM_OPTIONS>
struct RandomEventCheck {
  internal::maxSizedVector<types::percentChance, RANDOM_OPTIONS> chances;
};

struct RandomBinaryEventCheckChance {
  types::percentChance val = 100;
};
}  // namespace pokesim

////////////////// END OF src/Components/RandomEventCheck.hpp //////////////////

////////////// START OF src/Components/Tags/RandomChanceTags.hpp ///////////////

namespace pokesim::tags {
struct RandomEventCheckPassed {};
struct RandomEventCheckFailed {};

struct RandomEvent1 {};
struct RandomEvent2 {};
struct RandomEvent3 {};
struct RandomEvent4 {};
struct RandomEvent5 {};
}  // namespace pokesim::tags

/////////////// END OF src/Components/Tags/RandomChanceTags.hpp ////////////////

/////////////////// START OF src/Simulation/RandomChance.hpp ///////////////////

namespace pokesim {
class Simulation;

inline void setRandomBinaryChoice(
  const Simulation& simulation, types::handle battleHandle, types::percentChance percentChance);
template <std::uint8_t POSSIBLE_EVENT_COUNT>
inline void randomChance(Simulation& simulation);
inline void randomBinaryChance(Simulation& simulation);
inline void clearRandomChanceResult(Simulation& simulation);

inline void sampleRandomChance(Simulation& simulation);
}  // namespace pokesim

//////////////////// END OF src/Simulation/RandomChance.hpp ////////////////////

//////////////////////// START OF src/Utilities/RNG.hpp ////////////////////////

// Adapted from https://github.com/imneme/pcg-c-basic/blob/master/pcg_basic.c
namespace pokesim::internal {

// Generate a uniformly distributed 32-bit random number
inline types::rngResult nextRandomValue(types::rngState& state) {
  // NOLINTBEGIN
  types::rngState oldState = state;
  state = oldState * 6364136223846793005ULL;
  types::rngResult xorShifted = ((oldState >> 18U) ^ oldState) >> 27U;
  types::rngResult rot = oldState >> 59U;
  return (xorShifted >> rot) | (xorShifted << ((-rot) & 31));
  // NOLINTEND
}

// Generate a uniformly distributed 32-bit random number
inline types::rngResult nextRandomValue(RngSeed& seed) {
  return nextRandomValue(seed.val);
}

// Generate a uniformly distributed number, r, where 0 <= r < bound
inline types::rngResult nextBoundedRandomValue(types::rngState& state, types::rngResult upperBound) {
  // To avoid bias, we need to make the range of the RNG a multiple of
  // bound, which we do by dropping output less than a threshold.
  // A naive scheme to calculate the threshold would be to do
  //
  //     uint32_t threshold = 0x100000000ull % bound;
  //
  // but 64-bit div/mod is slower than 32-bit div/mod (especially on
  // 32-bit platforms).  In essence, we do
  //
  //     uint32_t threshold = (0x100000000ull-bound) % bound;
  //
  // because this version will calculate the same modulus, but the LHS
  // value is less than 2^32.

  types::rngResult threshold = -upperBound % upperBound;

  // Uniformity guarantees that this loop will terminate.  In practice, it
  // should usually terminate quickly; on average (assuming all bounds are
  // equally likely), 82.25% of the time, we can expect it to require just
  // one iteration.  In the worst case, someone passes a bound of 2^31 + 1
  // (i.e., 2147483649), which invalidates almost 50% of the range.  In
  // practice, bounds are typically small and only a tiny amount of the range
  // is eliminated.
  for (;;) {
    types::rngResult value = nextRandomValue(state);
    if (value >= threshold) return value % upperBound;
  }
}

// Generate a uniformly distributed number, r, where 0 <= r < bound
inline types::rngResult nextBoundedRandomValue(RngSeed& seed, types::rngResult upperBound) {
  return nextBoundedRandomValue(seed.val, upperBound);
}
}  // namespace pokesim::internal

///////////////////////// END OF src/Utilities/RNG.hpp /////////////////////////

/////////////////// START OF src/Simulation/RandomChance.cpp ///////////////////

namespace pokesim {
void setRandomBinaryChoice(
  const Simulation& simulation, types::handle battleHandle, types::percentChance percentChance) {
  if (percentChance >= simulation.simulateTurnOptions.randomChanceUpperLimit) {
    battleHandle.emplace<tags::RandomEventCheckPassed>();
  }
  else if (percentChance <= simulation.simulateTurnOptions.randomChanceLowerLimit) {
    battleHandle.emplace<tags::RandomEventCheckFailed>();
  }
  else {
    battleHandle.emplace<RandomBinaryEventCheckChance>(percentChance);
  }
}

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void randomChance(Simulation& simulation) {
  static_assert(POSSIBLE_EVENT_COUNT > 2);
  static_assert(POSSIBLE_EVENT_COUNT <= internal::MAX_TYPICAL_RANDOM_OPTIONS);

  types::registry& registry = simulation.registry;
  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
    auto checkView = registry.view<RandomEventCheck<POSSIBLE_EVENT_COUNT>>();
    ENTT_ASSERT(
      checkView.empty() || POSSIBLE_EVENT_COUNT > 2U,
      "RandomEventCheck should only be used for events with more than two options. Use RandomBinaryEventCheckChance "
      "instead");
    registry.insert<tags::CloneFrom>(checkView.begin(), checkView.end());
    auto clonedEntityMap = clone(registry, POSSIBLE_EVENT_COUNT - 1);

    for (const auto [originalBattle, clonedBattles] : clonedEntityMap) {
      registry.emplace<tags::RandomEvent1>(originalBattle);
      registry.emplace<tags::RandomEvent2>(clonedBattles[0]);
      registry.emplace<tags::RandomEvent3>(clonedBattles[1]);

      if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
        registry.emplace<tags::RandomEvent4>(clonedBattles[2]);
      }
      if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
        registry.emplace<tags::RandomEvent4>(clonedBattles[3]);
      }
    }
  }
  else {
    registry.view<RandomEventCheck<POSSIBLE_EVENT_COUNT>, RngSeed>().each(
      [&registry](types::entity entity, const RandomEventCheck<POSSIBLE_EVENT_COUNT>& eventCheck, RngSeed& rngSeed) {
        types::percentChance rng = internal::nextBoundedRandomValue(rngSeed, 100);

        if (rng <= eventCheck.chances[0]) {
          registry.emplace<tags::RandomEvent1>(entity);
        }
        else if (rng <= eventCheck.chances[1]) {
          registry.emplace<tags::RandomEvent2>(entity);
        }
        else if (rng <= eventCheck.chances[2]) {
          registry.emplace<tags::RandomEvent3>(entity);
        }
        else if (POSSIBLE_EVENT_COUNT >= 4U && rng <= eventCheck.chances[3]) {
          registry.emplace<tags::RandomEvent4>(entity);
        }
        else if (POSSIBLE_EVENT_COUNT == 5U && rng <= eventCheck.chances[4]) {
          registry.emplace<tags::RandomEvent5>(entity);
        }
      });
  }

  registry.clear<RandomEventCheck<POSSIBLE_EVENT_COUNT>>();
}

void randomBinaryChance(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
    auto binaryCheckView = registry.view<RandomBinaryEventCheckChance>();
    registry.insert<tags::CloneFrom>(binaryCheckView.begin(), binaryCheckView.end());
    auto clonedEntityMap = clone(registry, 1);
    for (const auto [originalBattle, clonedBattle] : clonedEntityMap) {
      registry.emplace<tags::RandomEventCheckPassed>(originalBattle);
      registry.emplace<tags::RandomEventCheckFailed>(clonedBattle[0]);
    }
  }
  else {
    registry.view<RandomBinaryEventCheckChance, RngSeed>().each(
      [&registry](types::entity entity, const RandomBinaryEventCheckChance& eventCheck, RngSeed& rngSeed) {
        types::percentChance rng = internal::nextBoundedRandomValue(rngSeed, 100);

        if (rng <= eventCheck.val) {
          registry.emplace<tags::RandomEventCheckPassed>(entity);
        }
        else {
          registry.emplace<tags::RandomEventCheckFailed>(entity);
        }
      });
  }

  registry.clear<RandomBinaryEventCheckChance>();
}

void clearRandomChanceResult(Simulation& simulation) {
  simulation.registry.clear<tags::RandomEventCheckPassed>();
  simulation.registry.clear<tags::RandomEventCheckFailed>();
  simulation.registry.clear<tags::RandomEvent1>();
  simulation.registry.clear<tags::RandomEvent2>();
  simulation.registry.clear<tags::RandomEvent3>();
  simulation.registry.clear<tags::RandomEvent4>();
  simulation.registry.clear<tags::RandomEvent5>();
}

template <>
void randomChance<2U>(Simulation& simulation) {
  randomBinaryChance(simulation);
}
template void randomChance<3U>(Simulation& simulation);
template void randomChance<4U>(Simulation& simulation);
template void randomChance<5U>(Simulation& simulation);

void sampleRandomChance(Simulation& simulation) {}
}  // namespace pokesim

//////////////////// END OF src/Simulation/RandomChance.cpp ////////////////////

///////////////////// START OF src/Components/Accuracy.hpp /////////////////////

namespace pokesim {
struct Accuracy {
  types::percentChance val = 100;
};
}  // namespace pokesim

////////////////////// END OF src/Components/Accuracy.hpp //////////////////////

////////////// START OF src/Components/EntityHolders/Current.hpp ///////////////

namespace pokesim {
struct CurrentAction {
  types::entity val{};
};

struct NextAction {
  types::entity val{};
};

struct CurrentActionTargets {
  types::targets<types::entity> val{};
};

struct CurrentActionSource {
  types::entity val{};
};

struct CurrentActionMove {
  types::entity val{};
};

struct CurrentActionMoveSlot {
  types::entity val{};
};
}  // namespace pokesim

/////////////// END OF src/Components/EntityHolders/Current.hpp ////////////////

///////////////////// START OF src/Components/MultiHit.hpp /////////////////////

namespace pokesim {
// The minimum and maximum number of hits a multi-hit move can cause
struct MultiHit {
  types::moveHits minHits = 2;
  types::moveHits maxHits = 5;
};

struct HitCount {
  types::moveHits val = 1;
};
}  // namespace pokesim

////////////////////// END OF src/Components/MultiHit.hpp //////////////////////

/////////// START OF src/Components/SimulateTurn/MoveHitStepTags.hpp ///////////

namespace pokesim::tags::internal {
struct TargetCanBeHit {};
}  // namespace pokesim::tags::internal

//////////// END OF src/Components/SimulateTurn/MoveHitStepTags.hpp ////////////

///////////////// START OF src/Components/Tags/BattleTags.hpp //////////////////

namespace pokesim::tags {

// Current Action Tag: The move that is being processed by the simulator
struct CurrentActionMove {};
// Current Action Tag: The move slot the current action's move was chosen and will deduct PP from
struct CurrentActionMoveSlot {};
// Current Action Tag: The target of the active move
struct CurrentActionMoveTarget {};
// Current Action Tag: The user of the active move
struct CurrentActionMoveSource {};

// Battle Turn State Tag: When a battle is in the middle of a turn
struct BattleMidTurn {};
// Battle Turn State Tag: When a battle has ended
struct BattleEnded {};
}  // namespace pokesim::tags

////////////////// END OF src/Components/Tags/BattleTags.hpp ///////////////////

/////////////////// START OF src/Simulation/MoveHitSteps.hpp ///////////////////

namespace pokesim {
class Simulation;
struct CurrentActionTargets;
struct Accuracy;
struct Battle;

namespace internal {
inline void assignMoveAccuracyToTargets(types::handle targetHandle, const Battle& battle);
inline void removeAccuracyFromTargets(types::registry& registry, const CurrentActionTargets& targets);
inline void assignAccuracyToRandomEvent(
  types::registry& registry, const Simulation& simulation, const Battle& battle, const Accuracy& accuracy);

inline void assignHitTags(types::registry& registry, const CurrentActionTargets& targets);
inline void removeFailedHitTargets(types::registry& registry, CurrentActionTargets& targets);
}  // namespace internal

inline void setMoveHitCount(Simulation& simulation);

inline void trySetStatusFromEffect(Simulation& simulation);
inline void applyDamage(Simulation& simulation);
inline void runSecondaryMoveEffects(Simulation& simulation);
inline void accuracyCheck(Simulation& simulation);
inline void moveHitLoop(Simulation& simulation);

inline void runMoveHitChecks(Simulation& simulation);
}  // namespace pokesim

//////////////////// END OF src/Simulation/MoveHitSteps.hpp ////////////////////

/////////////////// START OF src/Simulation/MoveHitSteps.cpp ///////////////////

namespace pokesim {
void setMoveHitCount(Simulation& simulation) {
  sampleRandomChance(simulation);
}

void applyDamage(Simulation& /*simulation*/) {}

void trySetStatusFromEffect(Simulation& /*simulation*/) {}

void runSecondaryMoveEffects(Simulation& simulation) {
  // Set secondary effect of active move

  trySetStatusFromEffect(simulation);
}

void internal::assignAccuracyToRandomEvent(
  types::registry& registry, const Simulation& simulation, const Battle& battle, const Accuracy& accuracy) {
  setRandomBinaryChoice(simulation, {registry, battle.val}, accuracy.val);
}

void internal::assignMoveAccuracyToTargets(types::handle targetHandle, const Battle& battle) {
  types::registry& registry = *targetHandle.registry();
  const Accuracy& accuracy = registry.get<Accuracy>(registry.get<CurrentActionMove>(battle.val).val);
  targetHandle.emplace<Accuracy>(accuracy);
}

void internal::removeAccuracyFromTargets(types::registry& registry, const CurrentActionTargets& targets) {
  registry.remove<Accuracy>(targets.val.begin(), targets.val.end());
}

void accuracyCheck(Simulation& simulation) {
  simulation.view<internal::assignMoveAccuracyToTargets, tags::CurrentActionMoveTarget>();
  runModifyAccuracyEvent(simulation);
  runAccuracyEvent(simulation);

  simulation.view<internal::assignAccuracyToRandomEvent, tags::CurrentActionMoveTarget>();
  randomChance<2U>(simulation);

  auto missed = simulation.registry.view<tags::RandomEventCheckFailed, tags::CurrentActionMoveTarget>();
  simulation.registry.remove<tags::internal::TargetCanBeHit>(missed.begin(), missed.end());

  clearRandomChanceResult(simulation);

  simulation.view<internal::removeAccuracyFromTargets>();
  // clear accuracy from target
}

void moveHitLoop(Simulation& simulation) {
  setMoveHitCount(simulation);

  while (!simulation.registry.view<HitCount>().empty()) {
    calc_damage::run(simulation);

    // for simulate turn only
    applyDamage(simulation);
    runSecondaryMoveEffects(simulation);
    runDamagingHitEvent(simulation);

    // Update stats if needed
  }
}

void internal::assignHitTags(types::registry& registry, const CurrentActionTargets& targets) {
  registry.insert<tags::internal::TargetCanBeHit>(targets.val.begin(), targets.val.end());
}

void internal::removeFailedHitTargets(types::registry& registry, CurrentActionTargets& targets) {
  types::targets<types::entity> newTargets;
  for (types::entity target : targets.val) {
    if (!registry.all_of<tags::internal::TargetCanBeHit>(target)) {
      registry.remove<tags::CurrentActionMoveTarget>(target);
    }
    else {
      newTargets.push_back(target);
    }
  }
  targets.val = newTargets;
}

template <auto Function>
void runMoveHitCheck(Simulation& simulation) {
  simulation.view<internal::assignHitTags>();

  Function(simulation);
  // exit if there are no targets
  // assign a hit tag to each target
  // for inside each function, use the hitResult as what's selected and remove the tag as needed when something fails
  // if tag isn't there in the end, remove the target and its CurrentActionMoveTarget tag
  // clear the tags

  simulation.view<internal::removeFailedHitTargets>();
  simulation.registry.clear<tags::internal::TargetCanBeHit>();
}

void runMoveHitChecks(Simulation& simulation) {
  // invulnerabilityCheck
  // hitCheck
  // immunityCheck
  runMoveHitCheck<accuracyCheck>(simulation);
  // breakProtectCheck
  // stealBoostCheck
  runMoveHitCheck<moveHitLoop>(simulation);
}

template void runMoveHitCheck<accuracyCheck>(Simulation& simulation);
template void runMoveHitCheck<moveHitLoop>(Simulation& simulation);
}  // namespace pokesim

//////////////////// END OF src/Simulation/MoveHitSteps.cpp ////////////////////

////////////////// START OF src/Battle/ManageBattleState.hpp ///////////////////

namespace pokesim {
class Simulation;
class Pokedex;
struct Sides;
struct CurrentAction;
struct CurrentActionSource;

inline void setCurrentActionTarget(types::handle battleHandle, const Sides& sides, const CurrentAction& action);
inline void setCurrentActionSource(types::handle battleHandle, const Sides& sides, const CurrentAction& action);
inline void setCurrentActionMove(
  types::handle battleHandle, const Simulation& simulation, const CurrentActionSource& source,
  const CurrentAction& action);
inline void clearCurrentAction(Simulation& simulation);
}  // namespace pokesim

/////////////////// END OF src/Battle/ManageBattleState.hpp ////////////////////

////////////// START OF src/Battle/Pokemon/ManagePokemonState.hpp //////////////

namespace pokesim {
class Simulation;
struct CurrentActionSource;
struct CurrentActionMoveSlot;
struct Pp;
namespace stat {
struct Spe;
}  // namespace stat

inline void deductPp(Pp& pp);
inline void setLastMoveUsed(types::registry& registry, const CurrentActionSource& source, const CurrentActionMoveSlot& move);
inline void resetEffectiveSpeed(types::handle handle, stat::Spe spe);

inline void updateSpeed(Simulation& simulation);
}  // namespace pokesim

/////////////// END OF src/Battle/Pokemon/ManagePokemonState.hpp ///////////////

/////////////////// START OF src/Components/AddedTargets.hpp ///////////////////

namespace pokesim {
struct AddedTargets {
  AddedTargetOptions val = AddedTargetOptions::NONE;
};
}  // namespace pokesim

//////////////////// END OF src/Components/AddedTargets.hpp ////////////////////

/////////////// START OF src/Components/Names/SourceSlotName.hpp ///////////////

namespace pokesim {
/**
 * @brief Represents the source slot for a move in a Pokemon battle.
 */
struct SourceSlotName {
  Slot name = Slot::NONE;
};
}  // namespace pokesim

//////////////// END OF src/Components/Names/SourceSlotName.hpp ////////////////

/////////////// START OF src/Components/Names/TargetSlotName.hpp ///////////////

namespace pokesim {
/**
 * @brief Represents the target slot for a move in a Pokemon battle.
 *
 * In a single battle, only P1A and P2A are valid targets. In a double battle, P1B and P2B are also valid.
 */
struct TargetSlotName {
  Slot name = Slot::NONE;
};
}  // namespace pokesim

//////////////// END OF src/Components/Names/TargetSlotName.hpp ////////////////

//////////////////////// START OF src/Components/PP.hpp ////////////////////////

namespace pokesim {
struct Pp {
  types::pp val = 0;
};

struct MaxPp {
  types::pp val = 5;
};
}  // namespace pokesim

///////////////////////// END OF src/Components/PP.hpp /////////////////////////

///////////////// START OF src/Components/Names/ItemNames.hpp //////////////////

namespace pokesim {
struct ItemName {
  dex::Item name = dex::Item::NO_ITEM;
};
}  // namespace pokesim

////////////////// END OF src/Components/Names/ItemNames.hpp ///////////////////

///////////////// START OF src/Components/Names/MoveNames.hpp //////////////////

namespace pokesim {
struct MoveName {
  dex::Move name = dex::Move::NO_MOVE;
};
}  // namespace pokesim

////////////////// END OF src/Components/Names/MoveNames.hpp ///////////////////

///////////// START OF src/Components/SimulateTurn/ActionNames.hpp /////////////

namespace pokesim::action {
// Component to make an action one where a Pokemon uses a move. Contains the name of the move being used.
struct Move : MoveName {};
// Component to make an action one where a player uses an item. Contains the name of the item being used.
struct Item : ItemName {};
}  // namespace pokesim::action

////////////// END OF src/Components/SimulateTurn/ActionNames.hpp //////////////

///////////// START OF src/Components/SimulateTurn/ActionTags.hpp //////////////

namespace pokesim::action::tags {
struct BeforeTurn {};
struct Residual {};

struct Current {};

struct Switch {};

// Action Tag for Switching: When a Pokemon is about to be switched out
// Example: Pursuit activating
struct PreSwitchOut {};
// Action Tag for Switching: When a Pokemon is being switched out
struct SwitchOut {};
/**
 * @brief Action Tag for Switching: When a Pokemon is being switched in because a player chose to directly switch the
 * Pokemon in as their turn action
 */
struct PreTurnSwitchIn {};
// Action Tag for Switching: When a Pokemon is being switched in because a team member switches out mid-turn
// Examples: U-Turn, Baton Pash, Emergency Exit
struct MidTurnSwitchIn {};
// Action Tag for Switching: When a Pokemon is being switched in to replace a fainted team member
struct PostFoeFaintSwitchIn {};

// Action Tag: When Revival Blessing is being used on a fainted party member
struct RevivalBlessing {};
// Action Tag: When a mega evolution is activating
struct MegaEvolve {};
// Action Tag: When a primal reversion is activating
struct PrimalRevert {};
// Action Tag: When a Pokemon dynamaxing
struct Dynamax {};
// Action Tag: When a Pokemon is terastallizing
struct Terastallize {};
}  // namespace pokesim::action::tags

////////////// END OF src/Components/SimulateTurn/ActionTags.hpp ///////////////

//////////////////// START OF src/Components/SpeedSort.hpp /////////////////////

namespace pokesim {
// Data that determines the order actions take place
struct SpeedSort {
  // Order of the types of actions (lower first)
  ActionOrder order = ActionOrder::NONE;
  // Priority of the action (higher first)
  types::priority priority = 0;
  // Whether negative fractional priority is active for the action (false first)
  types::fractionalPriority fractionalPriority = false;
  // Speed of Pokemon using move (higher first if priority tie)
  types::stat speed = 1;
};
}  // namespace pokesim

///////////////////// END OF src/Components/SpeedSort.hpp //////////////////////

///////////////// START OF src/Components/Tags/TargetTags.hpp //////////////////

namespace pokesim::move {
namespace tags {
struct AnySingleTarget {};
struct AnySingleFoe {};
struct AnySingleAlly {};
struct AllyOrSelf {};
struct Self {};
struct AllFoes {};
struct AlliesAndFoes {};
struct AlliesAndSelf {};
struct FoeSide {};
struct AllySide {};
struct Field {};
struct AllyTeam {};
struct Retaliation {};
struct RandomFoe {};
}  // namespace tags

namespace added_targets::tags {
struct TargetAlly {};
struct UserAlly {};
struct TargetSide {};
struct UserSide {};
struct Field {};
}  // namespace added_targets::tags
}  // namespace pokesim::move

////////////////// END OF src/Components/Tags/TargetTags.hpp ///////////////////

/////////////////////// START OF src/Components/Turn.hpp ///////////////////////

namespace pokesim {
// The current turn of a battle
struct Turn {
  types::battleTurn val = 0;
};
}  // namespace pokesim

//////////////////////// END OF src/Components/Turn.hpp ////////////////////////

/////////////// START OF src/SimulateTurn/ManageActionQueue.hpp ////////////////

// Systems
namespace pokesim {
struct SideDecision;
struct ActionQueue;

namespace simulate_turn {
inline void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision);
inline void speedSort(types::handle handle, ActionQueue& actionQueue);

inline void addBeforeTurnAction(types::registry& registry, ActionQueue& actionQueue);
inline void addResidualAction(types::registry& registry, ActionQueue& actionQueue);
inline void setCurrentAction(types::handle battleHandle, ActionQueue& actionQueue);
}  // namespace simulate_turn
}  // namespace pokesim

//////////////// END OF src/SimulateTurn/ManageActionQueue.hpp /////////////////

//////////////// START OF src/Components/EntityHolders/Team.hpp ////////////////

namespace pokesim {
// Contains a list of entities pointing to the Pokemon on a team.
struct Team {
  types::teamPositions<types::entity> val{};
};
}  // namespace pokesim

///////////////// END OF src/Components/EntityHolders/Team.hpp /////////////////

//////////////// START OF src/Components/Names/SpeciesNames.hpp ////////////////

namespace pokesim {
struct SpeciesName {
  dex::Species name = dex::Species::MISSING_NO;
};
}  // namespace pokesim

///////////////// END OF src/Components/Names/SpeciesNames.hpp /////////////////

/////////////////// START OF src/Utilities/SelectForView.hpp ///////////////////

#include <cstdint>
#include <vector>

namespace pokesim::internal {
template <typename Selection, typename... ComponentsToSelect>
struct SelectForView {
  SelectForView(Simulation& simulation_) : simulation(simulation_), depth(&Selection::depth) {
    auto view = simulation.registry.view<Selection>();
    if (!view.empty()) {
      previouslySelected.insert(previouslySelected.begin(), view.begin(), view.end());
      simulation.registry.clear<Selection>();
    }

    for (types::entity entity : simulation.registry.view<ComponentsToSelect...>()) {
      simulation.registry.emplace<Selection>(entity);
    }

    ENTT_ASSERT(depth != nullptr, "Depth needs to be defined.");
    *depth = *depth + 1;
  }

  ~SelectForView() { deselect(); }

  void deselect() {
    simulation.registry.clear<Selection>();

    for (types::entity entity : previouslySelected) {
      if (simulation.registry.valid(entity)) {
        simulation.registry.emplace<Selection>(entity);
      }
    }
    previouslySelected.clear();

    if (depth) {
      ENTT_ASSERT(*depth, "Selection depth cannot go negative.");
      *depth = *depth - 1;
      depth = nullptr;
    }
  }

 private:
  Simulation& simulation;
  std::uint8_t* depth = nullptr;
  std::vector<types::entity> previouslySelected;
};

template <typename... ComponentsToSelect>
struct SelectForBattleView : SelectForView<SelectedForViewBattle, Sides, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForSideView : SelectForView<SelectedForViewSide, Team, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForPokemonView : SelectForView<SelectedForViewPokemon, SpeciesName, ComponentsToSelect...> {};
template <typename... ComponentsToSelect>
struct SelectForMoveView : SelectForView<SelectedForViewMove, MoveName, ComponentsToSelect...> {};
}  // namespace pokesim::internal

//////////////////// END OF src/Utilities/SelectForView.hpp ////////////////////

////////////////// START OF src/SimulateTurn/SimulateTurn.cpp //////////////////

namespace pokesim::simulate_turn {
void run(Simulation& simulation) {
  internal::SelectForBattleView<tags::SimulateTurn> selectedBattle{simulation};

  updateSpeed(simulation);
  simulation.view<resolveDecision>();
  simulation.registry.clear<SideDecision>();

  // simulation.viewForSelectedBattles<addBeforeTurnAction>(entt::exclude_t<tags::BattleMidTurn>{});
  simulation.viewForSelectedBattles<speedSort>();
  simulation.viewForSelectedBattles<addResidualAction>(entt::exclude_t<tags::BattleMidTurn>{});

  auto turnEntities = simulation.registry.view<Turn, tags::SimulateTurn>();
  simulation.registry.insert<tags::BattleMidTurn>(turnEntities.begin(), turnEntities.end());

  simulation.viewForSelectedBattles<setCurrentAction>();
  while (!simulation.registry.view<action::tags::Current>().empty()) {
    runCurrentAction(simulation);
    simulation.viewForSelectedBattles<setCurrentAction>();
  }

  nextTurn(simulation);
}

void runCurrentAction(Simulation& simulation) {
  // runBeforeTurnAction(simulation);
  runMoveAction(simulation);
  runResidualAction(simulation);

  clearCurrentAction(simulation);
  // faint pokemon
  // Update
  // Switch requests

  if (!simulation.registry.view<tags::SpeedUpdateRequired>().empty()) {
    updateSpeed(simulation);
    simulation.viewForSelectedBattles<speedSort>();  // Should only speed sort battles affected
  }
}

void runBeforeTurnAction(Simulation& /*simulation*/) {
  // Barely used, will find different way of handling it
}

void runMoveAction(Simulation& simulation) {
  internal::SelectForBattleView<action::Move> selectedBattle{simulation};

  simulation.viewForSelectedBattles<setCurrentActionTarget>();
  simulation.viewForSelectedBattles<setCurrentActionSource>();
  simulation.viewForSelectedBattles<setCurrentActionMove>();

  simulation.view<deductPp, tags::CurrentActionMoveSlot>();
  simulation.view<setLastMoveUsed>();

  useMove(simulation);
}

void runResidualAction(Simulation& simulation) {
  internal::SelectForBattleView<action::tags::Residual> selectedBattle{simulation};
}

void nextTurn(Simulation& /*simulation*/) {}

void addTargetAllyToTargets(types::registry& registry, const Battle& battle) {
  const Sides& sides = registry.get<Sides>(battle.val);
  const TargetSlotName& targetSlotName = registry.get<TargetSlotName>(registry.get<CurrentAction>(battle.val).val);

  types::entity allyEntity = slotToAllyPokemonEntity(registry, sides, targetSlotName.name);
  if (allyEntity == entt::null) {
    return;
  }

  CurrentActionTargets& targets = registry.get<CurrentActionTargets>(battle.val);
  targets.val.push_back(allyEntity);
}

void addUserAllyToTargets(types::registry& registry, const Battle& battle) {
  const Sides& sides = registry.get<Sides>(battle.val);
  const SourceSlotName& sourceSlotName = registry.get<SourceSlotName>(registry.get<CurrentAction>(battle.val).val);

  types::entity allyEntity = slotToAllyPokemonEntity(registry, sides, sourceSlotName.name);
  if (allyEntity == entt::null) {
    return;
  }

  CurrentActionTargets& targets = registry.get<CurrentActionTargets>(battle.val);
  targets.val.push_back(allyEntity);
}

void resolveMoveTargets(CurrentActionTargets&) {}

void getMoveTargets(Simulation& simulation) {
  if (simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT) {
    simulation.view<addTargetAllyToTargets, tags::CurrentActionMove, move::added_targets::tags::TargetAlly>();
    simulation.view<addUserAllyToTargets, tags::CurrentActionMove, move::added_targets::tags::UserAlly>();
  }
  simulation.view<resolveMoveTargets, tags::CurrentActionMove>(entt::exclude<AddedTargets>);
}

void useMove(Simulation& simulation) {
  // ModifyTarget
  // ModifyType
  // ModifyMove

  getMoveTargets(simulation);
  runMoveHitChecks(simulation);
}
}  // namespace pokesim::simulate_turn

/////////////////// END OF src/SimulateTurn/SimulateTurn.cpp ///////////////////

/////////// START OF src/Components/SimulateTurn/SpeedTieIndexes.hpp ///////////

#include <cstdint>
#include <vector>

namespace pokesim {
struct SpeedTieIndexes {
  struct Span {
    std::size_t start = 0;
    std::size_t length = 0;
  };

  std::vector<Span> spans{};
};
}  // namespace pokesim

//////////// END OF src/Components/SimulateTurn/SpeedTieIndexes.hpp ////////////

///////////// START OF src/Components/SimulateTurn/TeamAction.hpp //////////////

namespace pokesim::action {
// Action Tag: When team member order has been picked as part of team preview
struct Team {
  types::teamOrder val{};
};
}  // namespace pokesim::action

////////////// END OF src/Components/SimulateTurn/TeamAction.hpp ///////////////

/////////////// START OF src/SimulateTurn/ManageActionQueue.cpp ////////////////

#include <algorithm>
#include <vector>

namespace pokesim::simulate_turn {
void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision) {
  ENTT_ASSERT(sideDecision.sideId != PlayerSideId::NONE, "Decisions must be assigned to a player");
  ENTT_ASSERT(!sideDecision.decisions.valueless_by_exception(), "Decisions must be non-empty");
  types::registry& registry = *sideHandle.registry();

  ActionQueue& battleActionQueue = registry.get<ActionQueue>(sideHandle.get<Battle>().val);

  if (sideDecision.decisions.holds<types::slotDecisions>()) {
    const auto& decisions = sideDecision.decisions.get<types::slotDecisions>();

    for (const SlotDecision& decision : decisions) {
      ENTT_ASSERT(decision.sourceSlot != Slot::NONE, "Source slot must be assigned");
      if (sideDecision.sideId == PlayerSideId::P1) {
        ENTT_ASSERT(
          (decision.sourceSlot == Slot::P1A || decision.sourceSlot == Slot::P1B),
          "Source must be from a player 1 in battle slot");
      }
      else {
        ENTT_ASSERT(
          (decision.sourceSlot == Slot::P2A || decision.sourceSlot == Slot::P2B),
          "Source must be from a player 2 in battle slot");
      }

      ENTT_ASSERT(decision.targetSlot != Slot::NONE, "Target slot must be assigned");
      ENTT_ASSERT(
        !(decision.moveChoice.has_value() && decision.itemChoice.has_value()),
        "Decisions can't have a move and an item choice");

      types::handle actionHandle = {registry, registry.create()};
      actionHandle.emplace<SourceSlotName>(decision.sourceSlot);
      actionHandle.emplace<TargetSlotName>(decision.targetSlot);

      SpeedSort speedSort;
      types::entity sourceEntity = slotToPokemonEntity(registry, sideHandle.entity(), decision.sourceSlot);

      stat::EffectiveSpeed* effectiveSpeed = registry.try_get<stat::EffectiveSpeed>(sourceEntity);
      if (effectiveSpeed != nullptr) {
        speedSort.speed = effectiveSpeed->effectiveSpeed;
      }
      else {
        speedSort.speed = registry.get<stat::Spe>(sourceEntity).val;
      }

      if (decision.moveChoice.has_value()) {
        actionHandle.emplace<action::Move>(decision.moveChoice.value());

        speedSort.order = ActionOrder::MOVE;
        speedSort.priority = 0;                // TODO (aed3): Move priority + modify priority
        speedSort.fractionalPriority = false;  // TODO (aed3): get fractionalPriority
      }
      else if (decision.itemChoice.has_value()) {
        actionHandle.emplace<action::Item>(decision.itemChoice.value());
        speedSort.order = ActionOrder::ITEM;
      }
      else {
        actionHandle.emplace<action::tags::Switch>();
        speedSort.order = ActionOrder::SWITCH;
      }

      actionHandle.emplace<SpeedSort>(speedSort);

      battleActionQueue.val.push_back(actionHandle.entity());
    }
  }
  else if (sideDecision.decisions.holds<types::teamOrder>()) {
    const auto& teamOrder = sideDecision.decisions.get<types::teamOrder>();

    ENTT_ASSERT(
      sideHandle.get<Team>().val.size() == teamOrder.size(),
      "Must pick a placement for each Pokemon on the team");
    types::handle actionHandle = {registry, registry.create()};

    actionHandle.emplace<action::Team>(teamOrder);

    battleActionQueue.val.push_back(actionHandle.entity());
  }
}

void speedSort(types::handle handle, ActionQueue& actionQueue) {
  std::vector<types::entity>& entityList = actionQueue.val;

  if (entityList.size() == 1) return;
  const types::registry* registry = handle.registry();

  std::vector<std::pair<SpeedSort, types::entity>> speedSortList;
  speedSortList.reserve(entityList.size());

  for (types::entity entity : entityList) {
    speedSortList.push_back({registry->get<SpeedSort>(entity), entity});
  }

  // TODO (aed3): Test how different sorting algorithms effect speed
  std::sort(speedSortList.begin(), speedSortList.end(), [](const auto& pairA, const auto& pairB) {
    if (pairA.first.order != pairB.first.order) {
      return pairA.first.order < pairB.first.order;
    }

    if (pairA.first.priority != pairB.first.priority) {
      return pairB.first.priority < pairA.first.priority;
    }

    if (pairA.first.fractionalPriority != pairB.first.fractionalPriority) {
      return pairB.first.fractionalPriority;
    }

    if (pairA.first.speed != pairB.first.speed) {
      return pairB.first.speed < pairA.first.speed;
    }

    return false;
  });

  SpeedTieIndexes speedTies;
  std::size_t lastEqual = 0, tieCount = 1;

  auto speedSortEqual = [](const SpeedSort& speedSortA, const SpeedSort& speedSortB) {
    return speedSortA.order == speedSortB.order && speedSortA.priority == speedSortB.priority &&
           speedSortA.speed == speedSortB.speed && speedSortA.fractionalPriority == speedSortB.fractionalPriority;
  };

  for (std::size_t i = 0; i < speedSortList.size(); i++) {
    entityList[i] = speedSortList[i].second;

    if (i > 0 && speedSortEqual(speedSortList[i].first, speedSortList[i - 1].first)) {
      tieCount++;
    }
    else {
      if (tieCount > 1) {
        speedTies.spans.push_back({lastEqual, tieCount});
      }
      lastEqual = i;
      tieCount = 1;
    }
  }

  if (tieCount > 1) {
    speedTies.spans.push_back({lastEqual, tieCount});
  }

  if (!speedTies.spans.empty()) {
    handle.emplace<SpeedTieIndexes>(speedTies);
  }
}

void addBeforeTurnAction(types::registry& registry, ActionQueue& actionQueue) {
  types::handle actionHandle{registry, registry.create()};
  SpeedSort speedSort{ActionOrder::BEFORE_TURN};

  actionHandle.emplace<action::tags::BeforeTurn>();
  actionHandle.emplace<SpeedSort>(speedSort);
  actionQueue.val.push_back(actionHandle.entity());
}

void addResidualAction(types::registry& registry, ActionQueue& actionQueue) {
  types::handle actionHandle{registry, registry.create()};
  SpeedSort speedSort{ActionOrder::RESIDUAL};

  actionHandle.emplace<action::tags::Residual>();
  actionHandle.emplace<SpeedSort>(speedSort);
  actionQueue.val.push_back(actionHandle.entity());
}

void setCurrentAction(types::handle battleHandle, ActionQueue& actionQueue) {
  types::registry& registry = *battleHandle.registry();
  registry.clear<action::tags::Current>();

  if (actionQueue.val.empty()) return;

  types::entity newCurrentAction = actionQueue.val.front();
  registry.emplace<action::tags::Current>(newCurrentAction);

  action::Move* moveAction = registry.try_get<action::Move>(newCurrentAction);
  if (moveAction) {
    battleHandle.emplace<action::Move>(*moveAction);
  }
  else if (registry.all_of<action::tags::Residual>(newCurrentAction)) {
    battleHandle.emplace<action::tags::Residual>();
  }

  actionQueue.val.erase(actionQueue.val.begin());

  registry.clear<NextAction>();
  battleHandle.emplace<CurrentAction>(newCurrentAction);
  if (!actionQueue.val.empty()) {
    battleHandle.emplace<NextAction>(actionQueue.val[0]);
  }
}
}  // namespace pokesim::simulate_turn

//////////////// END OF src/SimulateTurn/ManageActionQueue.cpp /////////////////

//////////////// START OF src/Components/Pokedex/Abilities.hpp /////////////////

namespace pokesim {
// Contains one of the standard abilities a species can have.
struct PrimaryAbility {
  dex::Ability name = dex::Ability::NO_ABILITY;
};

// Contains one of the standard abilities a species can have if the species can have two standard abilities.
struct SecondaryAbility {
  dex::Ability name = dex::Ability::NO_ABILITY;
};

// Contains The hidden ability a species has.
struct HiddenAbility {
  dex::Ability name = dex::Ability::NO_ABILITY;
};
}  // namespace pokesim

///////////////// END OF src/Components/Pokedex/Abilities.hpp //////////////////

//////////////// START OF src/Components/Pokedex/BaseStats.hpp /////////////////

namespace pokesim {
// Contains all of the base stats of a species
struct BaseStats {
  types::baseStat hp = 1;
  types::baseStat atk = 1;
  types::baseStat def = 1;
  types::baseStat spa = 1;
  types::baseStat spd = 1;
  types::baseStat spe = 1;
};
}  // namespace pokesim

///////////////// END OF src/Components/Pokedex/BaseStats.hpp //////////////////

/////////////// START OF src/Components/Pokedex/SpeciesTypes.hpp ///////////////

#include <array>

namespace pokesim {
// Contains the types a species has
struct SpeciesTypes {
  std::array<dex::Type, 2U> val{dex::Type::NO_TYPE, dex::Type::NO_TYPE};

  dex::Type& type1() { return val[0]; };
  dex::Type& type2() { return val[1]; };
  const dex::Type& type1() const { return val[0]; };
  const dex::Type& type2() const { return val[1]; };
};
}  // namespace pokesim

//////////////// END OF src/Components/Pokedex/SpeciesTypes.hpp ////////////////

////////////////// START OF src/Pokedex/Setup/DexDataTags.hpp //////////////////

#include <type_traits>

namespace pokesim::dex::internal {
template <typename... T>
struct Tags {};
}  // namespace pokesim::dex::internal

/////////////////// END OF src/Pokedex/Setup/DexDataTags.hpp ///////////////////

///////////////// START OF src/Pokedex/Setup/DexDataSetup.hpp //////////////////

namespace pokesim::dex::internal {
struct DexDataSetup {
 protected:
  types::handle handle;

 public:
  DexDataSetup(types::registry& registry) : handle(registry, registry.create()) {}

  template <typename Tag>
  void setProperty() {
    handle.emplace<Tag>();
  }

  template <typename... T>
  void setProperties(Tags<T...>) {
    (handle.emplace<T>(), ...);
  }

  types::entity entity() { return handle.entity(); }
};
}  // namespace pokesim::dex::internal

////////////////// END OF src/Pokedex/Setup/DexDataSetup.hpp ///////////////////

////////////// START OF src/Pokedex/Setup/SpeciesDexDataSetup.hpp //////////////

namespace pokesim {
class Pokedex;
}

namespace pokesim::dex::internal {
struct SpeciesDexDataSetup : DexDataSetup {
  SpeciesDexDataSetup(types::registry& registry) : DexDataSetup(registry) {}

  inline void setName(Species species);
  inline void setType(Type type1, Type type2 = Type::NO_TYPE);
  inline void setBaseStats(
    types::baseStat hp, types::baseStat atk, types::baseStat def, types::baseStat spa, types::baseStat spd,
    types::baseStat spe);
  inline void setPrimaryAbility(Ability ability);
  inline void setSecondaryAbility(Ability ability);
  inline void setHiddenAbility(Ability ability);
};
}  // namespace pokesim::dex::internal

/////////////// END OF src/Pokedex/Setup/SpeciesDexDataSetup.hpp ///////////////

////////////// START OF src/Pokedex/Setup/SpeciesDexDataSetup.cpp //////////////

namespace pokesim::dex::internal {
void SpeciesDexDataSetup::setName(Species species) {
  handle.emplace<SpeciesName>(species);
}

void SpeciesDexDataSetup::setType(Type type1, Type type2) {
  handle.emplace<SpeciesTypes>(type1, type2);
}

void SpeciesDexDataSetup::setBaseStats(
  types::baseStat hp, types::baseStat atk, types::baseStat def, types::baseStat spa, types::baseStat spd,
  types::baseStat spe) {
  handle.emplace<BaseStats>(hp, atk, def, spa, spd, spe);
}

void SpeciesDexDataSetup::setPrimaryAbility(Ability ability) {
  handle.emplace<PrimaryAbility>(ability);
}

void SpeciesDexDataSetup::setSecondaryAbility(Ability ability) {
  handle.emplace<SecondaryAbility>(ability);
}

void SpeciesDexDataSetup::setHiddenAbility(Ability ability) {
  handle.emplace<HiddenAbility>(ability);
}
}  // namespace pokesim::dex::internal

/////////////// END OF src/Pokedex/Setup/SpeciesDexDataSetup.cpp ///////////////

//////////////////// START OF src/Components/BasePower.hpp /////////////////////

namespace pokesim {
struct BasePower {
  types::basePower val = 1;
};
}  // namespace pokesim

///////////////////// END OF src/Components/BasePower.hpp //////////////////////

////////////////////// START OF src/Components/Chance.hpp //////////////////////

namespace pokesim {
/**
 * @brief Chance of an effect occurring.
 *
 * Examples: Air Slash has a 60% chance to flinch, Harvest has a 50% chance to restore a used berry.
 */
struct Chance {
  types::baseEffectChance val = 100;
};
}  // namespace pokesim

/////////////////////// END OF src/Components/Chance.hpp ///////////////////////

///////////////// START OF src/Components/Names/TypeNames.hpp //////////////////

namespace pokesim {
struct TypeName {
  dex::Type name = dex::Type::NO_TYPE;
};
}  // namespace pokesim

////////////////// END OF src/Components/Names/TypeNames.hpp ///////////////////

///////////////////// START OF src/Components/Priority.hpp /////////////////////

namespace pokesim {
struct MovePriority {
  types::priority val = 0;
};
}  // namespace pokesim

////////////////////// END OF src/Components/Priority.hpp //////////////////////

////////////////// START OF src/Components/Tags/MoveTags.hpp ///////////////////

namespace pokesim::move::tags {
// Move Category Tag
struct Physical {};
// Move Category Tag
struct Special {};
// Move Category Tag
struct Status {};

// Move Property Tag: Makes contact
struct Contact {};
// Move Property Tag: Ignores a target's substitute
struct BypassSubstitute {};
// Move Property Tag: Power is multiplied by 1.2 when used by a Pokemon with the Ability Iron Fist.
struct Punch {};

namespace effect {
// Move Effect Participant Tag: Who the effect will affect
struct MoveTarget {};
// Move Effect Participant Tag: Who created the effect
struct MoveSource {};
}  // namespace effect
}  // namespace pokesim::move::tags

/////////////////// END OF src/Components/Tags/MoveTags.hpp ////////////////////

/////////////// START OF src/Pokedex/Setup/MoveDexDataSetup.hpp ////////////////

#include <type_traits>


namespace pokesim::dex::internal {
struct MoveDexDataSetup : DexDataSetup {
  MoveDexDataSetup(types::registry& registry) : DexDataSetup(registry) {}

  inline void setName(Move move);
  inline void setType(Type type);
  inline void setAccuracy(types::baseAccuracy accuracy);
  inline void setBasePower(types::basePower basePower);

  inline void setCategoryPhysical();
  inline void setCategorySpecial();
  inline void setCategoryStatus();

  inline void setBasePp(types::pp pp);
  inline void setPriority(types::priority priority);
  inline void setMultiHit(types::moveHits minHits, types::moveHits maxHits);

  inline void setPrimaryEffect(types::entity entity);
  inline void setSecondaryEffect(types::entity entity);

  inline void addAddedTargets(AddedTargetOptions addedTargets);
};

struct MoveEffectSetup : DexDataSetup {
  MoveEffectSetup(types::registry& registry) : DexDataSetup(registry) {}
  types::entity entity() const { return handle; }

  inline void setChance(types::baseEffectChance chance);
  inline void setEffectsSelf();
  inline void setEffectsTarget();

  template <typename BoostType>
  void setBoost(types::boost boost) {
    static_assert(
      std::is_same<AtkBoost, BoostType>() || std::is_same<DefBoost, BoostType>() ||
      std::is_same<SpaBoost, BoostType>() || std::is_same<SpdBoost, BoostType>() ||
      std::is_same<SpeBoost, BoostType>());
    handle.emplace<BoostType>(boost);
  }
};
}  // namespace pokesim::dex::internal

//////////////// END OF src/Pokedex/Setup/MoveDexDataSetup.hpp /////////////////

/////////////// START OF src/Pokedex/Setup/MoveDexDataSetup.cpp ////////////////

namespace pokesim::dex::internal {
void MoveDexDataSetup::setName(Move move) {
  handle.emplace<MoveName>(move);
}

void MoveDexDataSetup::setType(Type type) {
  handle.emplace<TypeName>(type);
}

void MoveDexDataSetup::addAddedTargets(AddedTargetOptions addedTargets) {
  AddedTargets& existingTargets = handle.get_or_emplace<AddedTargets>();
  existingTargets.val = static_cast<AddedTargetOptions>(
    static_cast<std::uint8_t>(existingTargets.val) | static_cast<std::uint8_t>(addedTargets));

  switch (addedTargets) {
    case AddedTargetOptions::TARGET_ALLY: {
      setProperty<move::added_targets::tags::TargetAlly>();
      break;
    }
    case AddedTargetOptions::USER_ALLY: {
      setProperty<move::added_targets::tags::UserAlly>();
      break;
    }
    case AddedTargetOptions::TARGET_SIDE: {
      setProperty<move::added_targets::tags::TargetSide>();
      break;
    }
    case AddedTargetOptions::USER_SIDE: {
      setProperty<move::added_targets::tags::UserSide>();
      break;
    }
    case AddedTargetOptions::FIELD: {
      setProperty<move::added_targets::tags::Field>();
      break;
    }
    default: break;
  }
}

void MoveDexDataSetup::setAccuracy(types::baseAccuracy accuracy) {
  handle.emplace<Accuracy>(accuracy);
}

void MoveDexDataSetup::setBasePower(types::basePower basePower) {
  handle.emplace<BasePower>(basePower);
}

void MoveDexDataSetup::setCategoryPhysical() {
  ENTT_ASSERT(!(handle.any_of<move::tags::Special, move::tags::Status>()), "A move can only have one category");
  setProperty<move::tags::Physical>();
}

void MoveDexDataSetup::setCategorySpecial() {
  ENTT_ASSERT(!(handle.any_of<move::tags::Physical, move::tags::Status>()), "A move can only have one category");
  setProperty<move::tags::Special>();
}

void MoveDexDataSetup::setCategoryStatus() {
  ENTT_ASSERT(!(handle.any_of<move::tags::Physical, move::tags::Special>()), "A move can only have one category");
  setProperty<move::tags::Status>();
}

void MoveDexDataSetup::setBasePp(types::pp pp) {
  handle.emplace<Pp>(pp);
}

void MoveDexDataSetup::setPriority(types::priority priority) {
  handle.emplace<MovePriority>(priority);
}

void MoveDexDataSetup::setMultiHit(types::moveHits minHits, types::moveHits maxHits) {
  handle.emplace<MultiHit>(minHits, maxHits);
}

void MoveDexDataSetup::setPrimaryEffect(types::entity entity) {
  handle.emplace<MoveEffect>(true, entity);
}

void MoveDexDataSetup::setSecondaryEffect(types::entity entity) {
  handle.emplace<MoveEffect>(false, entity);
}

void MoveEffectSetup::setChance(types::baseEffectChance chance) {
  handle.emplace<Chance>(chance);
}

void MoveEffectSetup::setEffectsSelf() {
  handle.emplace<move::tags::effect::MoveSource>();
}

void MoveEffectSetup::setEffectsTarget() {
  handle.emplace<move::tags::effect::MoveTarget>();
}
}  // namespace pokesim::dex::internal

//////////////// END OF src/Pokedex/Setup/MoveDexDataSetup.cpp /////////////////

/////////////// START OF src/Pokedex/Setup/ItemDexDataSetup.hpp ////////////////

namespace pokesim::dex::internal {
struct ItemDexDataSetup : DexDataSetup {
  ItemDexDataSetup(types::registry& registry) : DexDataSetup(registry) {}

  inline void setName(Item item);
};
}  // namespace pokesim::dex::internal

//////////////// END OF src/Pokedex/Setup/ItemDexDataSetup.hpp /////////////////

/////////////// START OF src/Pokedex/Setup/ItemDexDataSetup.cpp ////////////////

namespace pokesim::dex::internal {
void ItemDexDataSetup::setName(Item item) {
  handle.emplace<ItemName>(item);
}
}  // namespace pokesim::dex::internal

//////////////// END OF src/Pokedex/Setup/ItemDexDataSetup.cpp /////////////////

////////////////// START OF src/Pokedex/Species/Ampharos.hpp ///////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct Ampharos {
  static constexpr Species name = Species::AMPHAROS;
  static constexpr types::baseStat hp = 90, atk = 75, def = 85, spa = 115, spd = 90, spe = 55;

  static constexpr SpeciesTypes type = {Type::ELECTRIC_TYPE};

  static constexpr Ability primaryAbility = Ability::STATIC;
  static constexpr Ability hiddenAbility = Ability::PLUS;

  struct Strings {
    static constexpr std::string_view name = "Ampharos";
    static constexpr std::string_view smogonName = "Ampharos";
    static constexpr std::string_view smogonId = "ampharos";
  };
};

namespace latest {
using Ampharos = dex::Ampharos<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Species/Ampharos.hpp ////////////////////

////////////////// START OF src/Pokedex/Species/Dragapult.hpp //////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct Dragapult {
  static constexpr Species name = Species::DRAGAPULT;
  static constexpr types::baseStat hp = 88, atk = 120, def = 75, spa = 100, spd = 75, spe = 142;

  static constexpr SpeciesTypes type = {Type::DRAGON_TYPE, Type::GHOST_TYPE};

  static constexpr Ability primaryAbility = Ability::CLEAR_BODY;
  static constexpr Ability secondaryAbility = Ability::INFILTRATOR;
  static constexpr Ability hiddenAbility = Ability::CURSED_BODY;

  struct Strings {
    static constexpr std::string_view name = "Dragapult";
    static constexpr std::string_view smogonName = "Dragapult";
    static constexpr std::string_view smogonId = "dragapult";
  };
};

namespace latest {
using Dragapult = dex::Dragapult<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Species/Dragapult.hpp ///////////////////

////////////////// START OF src/Pokedex/Species/Empoleon.hpp ///////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct Empoleon {
  static constexpr Species name = Species::EMPOLEON;
  static constexpr types::baseStat hp = 84, atk = 86, def = 88, spa = 111, spd = 101, spe = 60;

  static constexpr SpeciesTypes type = {Type::WATER_TYPE, Type::STEEL_TYPE};

  static constexpr Ability primaryAbility = Ability::TORRENT;
  static constexpr Ability hiddenAbility = Ability::DEFIANT;

  struct Strings {
    static constexpr std::string_view name = "Empoleon";
    static constexpr std::string_view smogonName = "Empoleon";
    static constexpr std::string_view smogonId = "empoleon";
  };
};

template <>
struct Empoleon<GameMechanics::SCARLET_VIOLET> : Empoleon<GameMechanics::NONE> {
  static constexpr Ability hiddenAbility = Ability::COMPETITIVE;
};

namespace latest {
using Empoleon = dex::Empoleon<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Species/Empoleon.hpp ////////////////////

////////////////// START OF src/Pokedex/Species/Gardevoir.hpp //////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct Gardevoir {
  static constexpr Species name = Species::GARDEVOIR;
  static constexpr types::baseStat hp = 68, atk = 65, def = 65, spa = 125, spd = 115, spe = 80;

  static constexpr SpeciesTypes type = {Type::PSYCHIC_TYPE, Type::FAIRY_TYPE};

  static constexpr Ability primaryAbility = Ability::SYNCHRONIZE;
  static constexpr Ability secondaryAbility = Ability::TRACE;
  static constexpr Ability hiddenAbility = Ability::TELEPATHY;

  struct Strings {
    static constexpr std::string_view name = "Gardevoir";
    static constexpr std::string_view smogonName = "Gardevoir";
    static constexpr std::string_view smogonId = "gardevoir";
  };
};

namespace latest {
using Gardevoir = dex::Gardevoir<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Species/Gardevoir.hpp ///////////////////

/////////////////// START OF src/Pokedex/Species/Pangoro.hpp ///////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct Pangoro {
  static constexpr Species name = Species::PANGORO;
  static constexpr types::baseStat hp = 95, atk = 124, def = 78, spa = 69, spd = 71, spe = 58;

  static constexpr SpeciesTypes type = {Type::FIGHTING_TYPE, Type::DARK_TYPE};

  static constexpr Ability primaryAbility = Ability::IRON_FIST;
  static constexpr Ability secondaryAbility = Ability::MOLD_BREAKER;
  static constexpr Ability hiddenAbility = Ability::SCRAPPY;

  struct Strings {
    static constexpr std::string_view name = "Pangoro";
    static constexpr std::string_view smogonName = "Pangoro";
    static constexpr std::string_view smogonId = "pangoro";
  };
};

namespace latest {
using Pangoro = dex::Pangoro<GameMechanics::SWORD_SHIELD>;
}
}  // namespace pokesim::dex

//////////////////// END OF src/Pokedex/Species/Pangoro.hpp ////////////////////

////////////////// START OF src/Pokedex/Species/Ribombee.hpp ///////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct Ribombee {
  static constexpr Species name = Species::RIBOMBEE;
  static constexpr types::baseStat hp = 60, atk = 55, def = 60, spa = 95, spd = 70, spe = 124;

  static constexpr SpeciesTypes type = {Type::BUG_TYPE, Type::FAIRY_TYPE};

  static constexpr Ability primaryAbility = Ability::HONEY_GATHER;
  static constexpr Ability secondaryAbility = Ability::SHIELD_DUST;
  static constexpr Ability hiddenAbility = Ability::SWEET_VEIL;

  struct Strings {
    static constexpr std::string_view name = "Ribombee";
    static constexpr std::string_view smogonName = "Ribombee";
    static constexpr std::string_view smogonId = "Ribombee";
  };
};

namespace latest {
using Ribombee = dex::Ribombee<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Species/Ribombee.hpp ////////////////////

//////////////// START OF src/Pokedex/Setup/GetSpeciesBuild.cpp ////////////////

#include <type_traits>


// TODO(aed3): Make this and the individual species files auto generated

namespace pokesim {
namespace internal {
template <typename T>
struct BuildSpecies {
 private:
  enum class Optional {
    primaryAbility,
    secondaryAbility,
    hiddenAbility,
  };

  template <auto Member>
  using void_t = std::void_t<decltype(Member)>;

  template <Optional, typename = T, typename V = void>
  struct has : std::false_type {};
  template <typename Type>
  struct has<Optional::primaryAbility, Type, void_t<Type::primaryAbility>> : std::true_type {};
  template <typename Type>
  struct has<Optional::secondaryAbility, Type, void_t<Type::secondaryAbility>> : std::true_type {};
  template <typename Type>
  struct has<Optional::hiddenAbility, Type, void_t<Type::hiddenAbility>> : std::true_type {};

 public:
  static types::entity build(types::registry& registry, bool /*forActiveMove*/) {
    dex::internal::SpeciesDexDataSetup species(registry);

    species.setName(T::name);
    species.setBaseStats(T::hp, T::atk, T::def, T::spa, T::spd, T::spe);
    species.setType(T::type.type1(), T::type.type2());

    if constexpr (has<Optional::primaryAbility>::value) {
      species.setPrimaryAbility(T::primaryAbility);
    }
    if constexpr (has<Optional::secondaryAbility>::value) {
      species.setSecondaryAbility(T::secondaryAbility);
    }
    if constexpr (has<Optional::hiddenAbility>::value) {
      species.setHiddenAbility(T::hiddenAbility);
    }

    return species.entity();
  }
};

template <template <GameMechanics> class T>
auto buildSpeciesSV(types::registry& registry, bool forActiveMove) {
  return BuildSpecies<T<GameMechanics::SCARLET_VIOLET>>::build(registry, forActiveMove);
}
};  // namespace internal

types::entity Pokedex::buildSpecies(dex::Species species, types::registry& registry, bool forActiveMove) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (species) {
        case Species::AMPHAROS: return buildSpeciesSV<Ampharos>(registry, forActiveMove);
        case Species::GARDEVOIR: return buildSpeciesSV<Gardevoir>(registry, forActiveMove);
        case Species::EMPOLEON: return buildSpeciesSV<Empoleon>(registry, forActiveMove);
        case Species::PANGORO: return buildSpeciesSV<Pangoro>(registry, forActiveMove);
        case Species::RIBOMBEE: return buildSpeciesSV<Ribombee>(registry, forActiveMove);
        case Species::DRAGAPULT: return buildSpeciesSV<Dragapult>(registry, forActiveMove);
        default: break;
      }
      break;
    }
    default: break;
  }

  ENTT_FAIL("Building a species that does not exist");
  return types::entity{};
}
};  // namespace pokesim

///////////////// END OF src/Pokedex/Setup/GetSpeciesBuild.cpp /////////////////

////////////////// START OF src/Pokedex/Moves/FuryAttack.hpp ///////////////////

#include <string_view>


namespace pokesim::dex {
template <GameMechanics>
struct FuryAttack {
  static constexpr Move name = Move::FURY_ATTACK;
  static constexpr Type type = Type::NORMAL_TYPE;
  static constexpr MoveCategory category = MoveCategory::PHYSICAL;

  static constexpr types::baseAccuracy accuracy = 85;
  static constexpr types::basePower basePower = 15;
  static constexpr types::pp basePp = 20;
  static constexpr types::moveHits minHits = 2, maxHits = 5;

  static constexpr internal::Tags<move::tags::Contact> moveTags{};
  static constexpr MoveTarget target = MoveTarget::ANY_SINGLE_TARGET;

  struct Strings {
    static constexpr std::string_view name = "Fury Attack";
    static constexpr std::string_view smogonId = "furyattack";
  };
};

namespace latest {
using FuryAttack = dex::FuryAttack<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Moves/FuryAttack.hpp ////////////////////

/////////////////// START OF src/Pokedex/Moves/KnockOff.hpp ////////////////////

#include <string_view>


namespace pokesim::dex {
template <GameMechanics>
struct KnockOff {
  static constexpr Move name = Move::KNOCK_OFF;
  static constexpr Type type = Type::DARK_TYPE;
  static constexpr MoveCategory category = MoveCategory::PHYSICAL;

  static constexpr types::baseAccuracy accuracy = 100;
  static constexpr types::basePower basePower = 65;
  static constexpr types::pp basePp = 20;

  static constexpr internal::Tags<move::tags::Contact> moveTags{};
  static constexpr MoveTarget target = MoveTarget::ANY_SINGLE_TARGET;

  struct Strings {
    static constexpr std::string_view name = "Knock Off";
    static constexpr std::string_view smogonId = "knockoff";
  };
};

namespace latest {
using KnockOff = dex::KnockOff<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

//////////////////// END OF src/Pokedex/Moves/KnockOff.hpp /////////////////////

/////////////////// START OF src/Pokedex/Moves/Moonblast.hpp ///////////////////

#include <string_view>


namespace pokesim::dex {
template <GameMechanics>
struct Moonblast {
  static constexpr Move name = Move::MOONBLAST;
  static constexpr Type type = Type::FAIRY_TYPE;
  static constexpr MoveCategory category = MoveCategory::SPECIAL;

  static constexpr types::baseAccuracy accuracy = 100;
  static constexpr types::basePower basePower = 95;
  static constexpr types::pp basePp = 15;

  struct targetSecondaryEffect {
    static constexpr types::baseEffectChance chance = 30;
    static constexpr types::boost spaBoost = -1;

    static constexpr internal::Tags<> effectTags{};
  };

  static constexpr internal::Tags<> moveTags{};
  static constexpr MoveTarget target = MoveTarget::ANY_SINGLE_TARGET;

  struct Strings {
    static constexpr std::string_view name = "Moonblast";
    static constexpr std::string_view smogonId = "moonblast";
  };
};

namespace latest {
using Moonblast = dex::Moonblast<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

//////////////////// END OF src/Pokedex/Moves/Moonblast.hpp ////////////////////

////////////////// START OF src/Pokedex/Moves/QuiverDance.hpp //////////////////

#include <string_view>


namespace pokesim::dex {
template <GameMechanics>
struct QuiverDance {
  static constexpr Move name = Move::QUIVER_DANCE;
  static constexpr Type type = Type::BUG_TYPE;
  static constexpr MoveCategory category = MoveCategory::STATUS;

  static constexpr types::pp basePp = 20;

  struct sourcePrimaryEffect {
    static constexpr types::boost spaBoost = 1, spdBoost = 1, speBoost = 1;

    static constexpr internal::Tags<> effectTags{};
  };

  static constexpr internal::Tags<> moveTags{};
  static constexpr MoveTarget target = MoveTarget::SELF;

  struct Strings {
    static constexpr std::string_view name = "Quiver Dance";
    static constexpr std::string_view smogonId = "quiverdance";
  };
};

namespace latest {
using QuiverDance = dex::QuiverDance<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Moves/QuiverDance.hpp ///////////////////

////////////////// START OF src/Pokedex/Moves/Thunderbolt.hpp //////////////////

#include <string_view>


namespace pokesim::dex {
template <GameMechanics>
struct Thunderbolt {
  static constexpr Move name = Move::THUNDERBOLT;
  static constexpr Type type = Type::ELECTRIC_TYPE;
  static constexpr MoveCategory category = MoveCategory::SPECIAL;

  static constexpr types::baseAccuracy accuracy = 100;
  static constexpr types::basePower basePower = 90;
  static constexpr types::pp basePp = 15;

  struct targetSecondaryEffect {
    static constexpr types::baseEffectChance chance = 10;

    static constexpr internal::Tags<status::tags::Paralysis> effectTags{};
  };

  static constexpr internal::Tags<> moveTags{};
  static constexpr MoveTarget target = MoveTarget::ANY_SINGLE_TARGET;

  struct Strings {
    static constexpr std::string_view name = "Thunderbolt";
    static constexpr std::string_view smogonId = "thunderbolt";
  };
};

namespace latest {
using Thunderbolt = dex::Thunderbolt<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Moves/Thunderbolt.hpp ///////////////////

/////////////////// START OF src/Pokedex/Moves/WillOWisp.hpp ///////////////////

#include <string_view>


namespace pokesim::dex {
template <GameMechanics>
struct WillOWisp {
  static constexpr Move name = Move::WILL_O_WISP;
  static constexpr Type type = Type::FIRE_TYPE;
  static constexpr MoveCategory category = MoveCategory::STATUS;

  static constexpr types::baseAccuracy accuracy = 85;
  static constexpr types::pp basePp = 15;

  struct targetPrimaryEffect {
    static constexpr internal::Tags<status::tags::Burn> effectTags{};
  };

  static constexpr internal::Tags<> moveTags{};
  static constexpr MoveTarget target = MoveTarget::ANY_SINGLE_TARGET;

  struct Strings {
    static constexpr std::string_view name = "Will-O-Wisp";
    static constexpr std::string_view smogonId = "willowisp";
  };
};

namespace latest {
using WillOWisp = dex::WillOWisp<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex

//////////////////// END OF src/Pokedex/Moves/WillOWisp.hpp ////////////////////

///////////////// START OF src/Pokedex/Setup/GetMoveBuild.cpp //////////////////

#include <type_traits>


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

////////////////// END OF src/Pokedex/Setup/GetMoveBuild.cpp ///////////////////

////////////////// START OF src/Pokedex/Items/AssaultVest.hpp //////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct AssaultVest {
  static constexpr dex::Item name = dex::Item::ASSAULT_VEST;

  struct Strings {
    static constexpr std::string_view name = "Assault Vest";
    static constexpr std::string_view smogonId = "assaultvest";
  };
};
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Items/AssaultVest.hpp ///////////////////

///////////////// START OF src/Pokedex/Items/BrightPowder.hpp //////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct BrightPowder {
  static constexpr dex::Item name = dex::Item::BRIGHT_POWDER;

  struct Strings {
    static constexpr std::string_view name = "Bright Powder";
    static constexpr std::string_view smogonId = "brightpowder";
  };
};
}  // namespace pokesim::dex

////////////////// END OF src/Pokedex/Items/BrightPowder.hpp ///////////////////

////////////////// START OF src/Pokedex/Items/ChoiceScarf.hpp //////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct ChoiceScarf {
  static constexpr dex::Item name = dex::Item::CHOICE_SCARF;

  struct Strings {
    static constexpr std::string_view name = "Choice Scarf";
    static constexpr std::string_view smogonId = "choicescarf";
  };
};
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Items/ChoiceScarf.hpp ///////////////////

////////////////// START OF src/Pokedex/Items/ChoiceSpecs.hpp //////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct ChoiceSpecs {
  static constexpr dex::Item name = dex::Item::CHOICE_SPECS;

  struct Strings {
    static constexpr std::string_view name = "Choice Specs";
    static constexpr std::string_view smogonId = "choicespecs";
  };
};
}  // namespace pokesim::dex

/////////////////// END OF src/Pokedex/Items/ChoiceSpecs.hpp ///////////////////

/////////////////// START OF src/Pokedex/Items/FocusSash.hpp ///////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct FocusSash {
  static constexpr dex::Item name = dex::Item::FOCUS_SASH;

  struct Strings {
    static constexpr std::string_view name = "Focus Sash";
    static constexpr std::string_view smogonId = "focussash";
  };
};
}  // namespace pokesim::dex

//////////////////// END OF src/Pokedex/Items/FocusSash.hpp ////////////////////

//////////////////// START OF src/Pokedex/Items/LifeOrb.hpp ////////////////////

#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct LifeOrb {
  static constexpr dex::Item name = dex::Item::LIFE_ORB;

  struct Strings {
    static constexpr std::string_view name = "Life Orb";
    static constexpr std::string_view smogonId = "lifeorb";
  };
};
}  // namespace pokesim::dex

///////////////////// END OF src/Pokedex/Items/LifeOrb.hpp /////////////////////

///////////////// START OF src/Pokedex/Setup/GetItemBuild.cpp //////////////////

#include <type_traits>


// TODO(aed3): Make this and the individual item files auto generated

namespace pokesim {
namespace internal {
template <typename T>
struct BuildItem {
 private:
 public:
  static types::entity build(types::registry& registry, bool /*forActiveMove*/) {
    dex::internal::ItemDexDataSetup item(registry);

    item.setName(T::name);

    return item.entity();
  }
};

template <template <GameMechanics> class T>
auto buildItemSV(types::registry& registry, bool forActiveMove) {
  return BuildItem<T<GameMechanics::SCARLET_VIOLET>>::build(registry, forActiveMove);
}
};  // namespace internal

types::entity Pokedex::buildItem(dex::Item item, types::registry& registry, bool forActiveMove) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (item) {
        case Item::ASSAULT_VEST: return buildItemSV<AssaultVest>(registry, forActiveMove);
        case Item::BRIGHT_POWDER: return buildItemSV<BrightPowder>(registry, forActiveMove);
        case Item::CHOICE_SCARF: return buildItemSV<ChoiceScarf>(registry, forActiveMove);
        case Item::CHOICE_SPECS: return buildItemSV<ChoiceSpecs>(registry, forActiveMove);
        case Item::FOCUS_SASH: return buildItemSV<FocusSash>(registry, forActiveMove);
        case Item::LIFE_ORB: return buildItemSV<LifeOrb>(registry, forActiveMove);
        default: break;
      }
      break;
    }
    default: break;
  }

  ENTT_FAIL("Building an item that does not exist");
  return types::entity{};
}
};  // namespace pokesim

////////////////// END OF src/Pokedex/Setup/GetItemBuild.cpp ///////////////////

////////////// START OF src/Pokedex/Setup/AbilityDexDataSetup.hpp //////////////

namespace pokesim::dex::internal {
struct AbilityDexDataSetup : DexDataSetup {
  AbilityDexDataSetup(types::registry& registry) : DexDataSetup(registry) {}

  inline void setName(Ability ability);
};
}  // namespace pokesim::dex::internal

/////////////// END OF src/Pokedex/Setup/AbilityDexDataSetup.hpp ///////////////

//////////////// START OF src/Pokedex/Setup/GetAbilityBuild.cpp ////////////////

#include <type_traits>


// TODO(aed3): Make this and the individual ability files auto generated

namespace pokesim {
namespace internal {
template <typename T>
struct BuildAbility {
 private:
 public:
  static types::entity build(types::registry& registry, bool /*forActiveMove*/) {
    dex::internal::AbilityDexDataSetup ability(registry);

    ability.setName(T::name);

    return ability.entity();
  }
};

template <template <GameMechanics> class T>
auto buildAbilitySV(types::registry& registry, bool forActiveMove) {
  return BuildAbility<T<GameMechanics::SCARLET_VIOLET>>::build(registry, forActiveMove);
}
};  // namespace internal

types::entity Pokedex::buildAbility(dex::Ability ability, types::registry& registry, bool forActiveMove) const {
  // Tidy check ignored because "using namespace" is in function
  using namespace pokesim::dex;       // NOLINT(google-build-using-namespace)
  using namespace pokesim::internal;  // NOLINT(google-build-using-namespace)

  switch (mechanics) {
    case GameMechanics::SCARLET_VIOLET: {
      switch (ability) {
        case Ability::STATIC: return buildAbilitySV<Static>(registry, forActiveMove);
        default: break;
      }
      break;
    }
    default: break;
  }

  ENTT_FAIL("Building an ability that does not exist");
  return types::entity{};
}
};  // namespace pokesim

///////////////// END OF src/Pokedex/Setup/GetAbilityBuild.cpp /////////////////

//////////////// START OF src/Components/Names/AbilityNames.hpp ////////////////

namespace pokesim {
struct AbilityName {
  dex::Ability name = dex::Ability::NO_ABILITY;
};
}  // namespace pokesim

///////////////// END OF src/Components/Names/AbilityNames.hpp /////////////////

////////////// START OF src/Pokedex/Setup/AbilityDexDataSetup.cpp //////////////

namespace pokesim::dex::internal {
void AbilityDexDataSetup::setName(Ability ability) {
  handle.emplace<AbilityName>(ability);
}
}  // namespace pokesim::dex::internal

/////////////// END OF src/Pokedex/Setup/AbilityDexDataSetup.cpp ///////////////

//////////////// START OF external/entt/container/dense_set.hpp ////////////////

#ifndef ENTT_CONTAINER_DENSE_SET_HPP
#define ENTT_CONTAINER_DENSE_SET_HPP

#include <cmath>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace entt {

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

namespace internal {

template<typename It>
class dense_set_iterator final {
    template<typename>
    friend class dense_set_iterator;

public:
    using value_type = typename It::value_type::second_type;
    using pointer = const value_type *;
    using reference = const value_type &;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

    constexpr dense_set_iterator() noexcept
        : it{} {}

    constexpr dense_set_iterator(const It iter) noexcept
        : it{iter} {}

    template<typename Other, typename = std::enable_if_t<!std::is_same_v<It, Other> && std::is_constructible_v<It, Other>>>
    constexpr dense_set_iterator(const dense_set_iterator<Other> &other) noexcept
        : it{other.it} {}

    constexpr dense_set_iterator &operator++() noexcept {
        return ++it, *this;
    }

    constexpr dense_set_iterator operator++(int) noexcept {
        dense_set_iterator orig = *this;
        return ++(*this), orig;
    }

    constexpr dense_set_iterator &operator--() noexcept {
        return --it, *this;
    }

    constexpr dense_set_iterator operator--(int) noexcept {
        dense_set_iterator orig = *this;
        return operator--(), orig;
    }

    constexpr dense_set_iterator &operator+=(const difference_type value) noexcept {
        it += value;
        return *this;
    }

    constexpr dense_set_iterator operator+(const difference_type value) const noexcept {
        dense_set_iterator copy = *this;
        return (copy += value);
    }

    constexpr dense_set_iterator &operator-=(const difference_type value) noexcept {
        return (*this += -value);
    }

    constexpr dense_set_iterator operator-(const difference_type value) const noexcept {
        return (*this + -value);
    }

    [[nodiscard]] constexpr reference operator[](const difference_type value) const noexcept {
        return it[value].second;
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept {
        return std::addressof(it->second);
    }

    [[nodiscard]] constexpr reference operator*() const noexcept {
        return *operator->();
    }

    template<typename Lhs, typename Rhs>
    friend constexpr std::ptrdiff_t operator-(const dense_set_iterator<Lhs> &, const dense_set_iterator<Rhs> &) noexcept;

    template<typename Lhs, typename Rhs>
    friend constexpr bool operator==(const dense_set_iterator<Lhs> &, const dense_set_iterator<Rhs> &) noexcept;

    template<typename Lhs, typename Rhs>
    friend constexpr bool operator<(const dense_set_iterator<Lhs> &, const dense_set_iterator<Rhs> &) noexcept;

private:
    It it;
};

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr std::ptrdiff_t operator-(const dense_set_iterator<Lhs> &lhs, const dense_set_iterator<Rhs> &rhs) noexcept {
    return lhs.it - rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator==(const dense_set_iterator<Lhs> &lhs, const dense_set_iterator<Rhs> &rhs) noexcept {
    return lhs.it == rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator!=(const dense_set_iterator<Lhs> &lhs, const dense_set_iterator<Rhs> &rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator<(const dense_set_iterator<Lhs> &lhs, const dense_set_iterator<Rhs> &rhs) noexcept {
    return lhs.it < rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator>(const dense_set_iterator<Lhs> &lhs, const dense_set_iterator<Rhs> &rhs) noexcept {
    return rhs < lhs;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator<=(const dense_set_iterator<Lhs> &lhs, const dense_set_iterator<Rhs> &rhs) noexcept {
    return !(lhs > rhs);
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator>=(const dense_set_iterator<Lhs> &lhs, const dense_set_iterator<Rhs> &rhs) noexcept {
    return !(lhs < rhs);
}

template<typename It>
class dense_set_local_iterator final {
    template<typename>
    friend class dense_set_local_iterator;

public:
    using value_type = typename It::value_type::second_type;
    using pointer = const value_type *;
    using reference = const value_type &;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    constexpr dense_set_local_iterator() noexcept
        : it{},
          offset{} {}

    constexpr dense_set_local_iterator(It iter, const std::size_t pos) noexcept
        : it{iter},
          offset{pos} {}

    template<typename Other, typename = std::enable_if_t<!std::is_same_v<It, Other> && std::is_constructible_v<It, Other>>>
    constexpr dense_set_local_iterator(const dense_set_local_iterator<Other> &other) noexcept
        : it{other.it},
          offset{other.offset} {}

    constexpr dense_set_local_iterator &operator++() noexcept {
        return offset = it[offset].first, *this;
    }

    constexpr dense_set_local_iterator operator++(int) noexcept {
        dense_set_local_iterator orig = *this;
        return ++(*this), orig;
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept {
        return std::addressof(it[offset].second);
    }

    [[nodiscard]] constexpr reference operator*() const noexcept {
        return *operator->();
    }

    [[nodiscard]] constexpr std::size_t index() const noexcept {
        return offset;
    }

private:
    It it;
    std::size_t offset;
};

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator==(const dense_set_local_iterator<Lhs> &lhs, const dense_set_local_iterator<Rhs> &rhs) noexcept {
    return lhs.index() == rhs.index();
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator!=(const dense_set_local_iterator<Lhs> &lhs, const dense_set_local_iterator<Rhs> &rhs) noexcept {
    return !(lhs == rhs);
}

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Associative container for unique objects of a given type.
 *
 * Internally, elements are organized into buckets. Which bucket an element is
 * placed into depends entirely on its hash. Elements with the same hash code
 * appear in the same bucket.
 *
 * @tparam Type Value type of the associative container.
 * @tparam Hash Type of function to use to hash the values.
 * @tparam KeyEqual Type of function to use to compare the values for equality.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Type, typename Hash, typename KeyEqual, typename Allocator>
class dense_set {
    static constexpr float default_threshold = 0.875f;
    static constexpr std::size_t minimum_capacity = 8u;

    using node_type = std::pair<std::size_t, Type>;
    using alloc_traits = std::allocator_traits<Allocator>;
    static_assert(std::is_same_v<typename alloc_traits::value_type, Type>, "Invalid value type");
    using sparse_container_type = std::vector<std::size_t, typename alloc_traits::template rebind_alloc<std::size_t>>;
    using packed_container_type = std::vector<node_type, typename alloc_traits::template rebind_alloc<node_type>>;

    template<typename Other>
    [[nodiscard]] std::size_t value_to_bucket(const Other &value) const noexcept {
        return fast_mod(static_cast<size_type>(sparse.second()(value)), bucket_count());
    }

    template<typename Other>
    [[nodiscard]] auto constrained_find(const Other &value, std::size_t bucket) {
        for(auto it = begin(bucket), last = end(bucket); it != last; ++it) {
            if(packed.second()(*it, value)) {
                return begin() + static_cast<typename iterator::difference_type>(it.index());
            }
        }

        return end();
    }

    template<typename Other>
    [[nodiscard]] auto constrained_find(const Other &value, std::size_t bucket) const {
        for(auto it = cbegin(bucket), last = cend(bucket); it != last; ++it) {
            if(packed.second()(*it, value)) {
                return cbegin() + static_cast<typename iterator::difference_type>(it.index());
            }
        }

        return cend();
    }

    template<typename Other>
    [[nodiscard]] auto insert_or_do_nothing(Other &&value) {
        const auto index = value_to_bucket(value);

        if(auto it = constrained_find(value, index); it != end()) {
            return std::make_pair(it, false);
        }

        packed.first().emplace_back(sparse.first()[index], std::forward<Other>(value));
        sparse.first()[index] = packed.first().size() - 1u;
        rehash_if_required();

        return std::make_pair(--end(), true);
    }

    void move_and_pop(const std::size_t pos) {
        if(const auto last = size() - 1u; pos != last) {
            size_type *curr = sparse.first().data() + value_to_bucket(packed.first().back().second);
            packed.first()[pos] = std::move(packed.first().back());
            for(; *curr != last; curr = &packed.first()[*curr].first) {}
            *curr = pos;
        }

        packed.first().pop_back();
    }

    void rehash_if_required() {
        if(size() > (bucket_count() * max_load_factor())) {
            rehash(bucket_count() * 2u);
        }
    }

public:
    /*! @brief Key type of the container. */
    using key_type = Type;
    /*! @brief Value type of the container. */
    using value_type = Type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Type of function to use to hash the elements. */
    using hasher = Hash;
    /*! @brief Type of function to use to compare the elements for equality. */
    using key_equal = KeyEqual;
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Random access iterator type. */
    using iterator = internal::dense_set_iterator<typename packed_container_type::iterator>;
    /*! @brief Constant random access iterator type. */
    using const_iterator = internal::dense_set_iterator<typename packed_container_type::const_iterator>;
    /*! @brief Forward iterator type. */
    using local_iterator = internal::dense_set_local_iterator<typename packed_container_type::iterator>;
    /*! @brief Constant forward iterator type. */
    using const_local_iterator = internal::dense_set_local_iterator<typename packed_container_type::const_iterator>;

    /*! @brief Default constructor. */
    dense_set()
        : dense_set{minimum_capacity} {}

    /**
     * @brief Constructs an empty container with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit dense_set(const allocator_type &allocator)
        : dense_set{minimum_capacity, hasher{}, key_equal{}, allocator} {}

    /**
     * @brief Constructs an empty container with a given allocator and user
     * supplied minimal number of buckets.
     * @param cnt Minimal number of buckets.
     * @param allocator The allocator to use.
     */
    dense_set(const size_type cnt, const allocator_type &allocator)
        : dense_set{cnt, hasher{}, key_equal{}, allocator} {}

    /**
     * @brief Constructs an empty container with a given allocator, hash
     * function and user supplied minimal number of buckets.
     * @param cnt Minimal number of buckets.
     * @param hash Hash function to use.
     * @param allocator The allocator to use.
     */
    dense_set(const size_type cnt, const hasher &hash, const allocator_type &allocator)
        : dense_set{cnt, hash, key_equal{}, allocator} {}

    /**
     * @brief Constructs an empty container with a given allocator, hash
     * function, compare function and user supplied minimal number of buckets.
     * @param cnt Minimal number of buckets.
     * @param hash Hash function to use.
     * @param equal Compare function to use.
     * @param allocator The allocator to use.
     */
    explicit dense_set(const size_type cnt, const hasher &hash = hasher{}, const key_equal &equal = key_equal{}, const allocator_type &allocator = allocator_type{})
        : sparse{allocator, hash},
          packed{allocator, equal},
          threshold{default_threshold} {
        rehash(cnt);
    }

    /*! @brief Default copy constructor. */
    dense_set(const dense_set &) = default;

    /**
     * @brief Allocator-extended copy constructor.
     * @param other The instance to copy from.
     * @param allocator The allocator to use.
     */
    dense_set(const dense_set &other, const allocator_type &allocator)
        : sparse{std::piecewise_construct, std::forward_as_tuple(other.sparse.first(), allocator), std::forward_as_tuple(other.sparse.second())},
          packed{std::piecewise_construct, std::forward_as_tuple(other.packed.first(), allocator), std::forward_as_tuple(other.packed.second())},
          threshold{other.threshold} {}

    /*! @brief Default move constructor. */
    dense_set(dense_set &&) noexcept(std::is_nothrow_move_constructible_v<compressed_pair<sparse_container_type, hasher>> &&std::is_nothrow_move_constructible_v<compressed_pair<packed_container_type, key_equal>>) = default;

    /**
     * @brief Allocator-extended move constructor.
     * @param other The instance to move from.
     * @param allocator The allocator to use.
     */
    dense_set(dense_set &&other, const allocator_type &allocator)
        : sparse{std::piecewise_construct, std::forward_as_tuple(std::move(other.sparse.first()), allocator), std::forward_as_tuple(std::move(other.sparse.second()))},
          packed{std::piecewise_construct, std::forward_as_tuple(std::move(other.packed.first()), allocator), std::forward_as_tuple(std::move(other.packed.second()))},
          threshold{other.threshold} {}

    /**
     * @brief Default copy assignment operator.
     * @return This container.
     */
    dense_set &operator=(const dense_set &) = default;

    /**
     * @brief Default move assignment operator.
     * @return This container.
     */
    dense_set &operator=(dense_set &&) noexcept(std::is_nothrow_move_assignable_v<compressed_pair<sparse_container_type, hasher>> &&std::is_nothrow_move_assignable_v<compressed_pair<packed_container_type, key_equal>>) = default;

    /**
     * @brief Returns the associated allocator.
     * @return The associated allocator.
     */
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return sparse.first().get_allocator();
    }

    /**
     * @brief Returns an iterator to the beginning.
     *
     * If the array is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first instance of the internal array.
     */
    [[nodiscard]] const_iterator cbegin() const noexcept {
        return packed.first().begin();
    }

    /*! @copydoc cbegin */
    [[nodiscard]] const_iterator begin() const noexcept {
        return cbegin();
    }

    /*! @copydoc begin */
    [[nodiscard]] iterator begin() noexcept {
        return packed.first().begin();
    }

    /**
     * @brief Returns an iterator to the end.
     * @return An iterator to the element following the last instance of the
     * internal array.
     */
    [[nodiscard]] const_iterator cend() const noexcept {
        return packed.first().end();
    }

    /*! @copydoc cend */
    [[nodiscard]] const_iterator end() const noexcept {
        return cend();
    }

    /*! @copydoc end */
    [[nodiscard]] iterator end() noexcept {
        return packed.first().end();
    }

    /**
     * @brief Checks whether a container is empty.
     * @return True if the container is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return packed.first().empty();
    }

    /**
     * @brief Returns the number of elements in a container.
     * @return Number of elements in a container.
     */
    [[nodiscard]] size_type size() const noexcept {
        return packed.first().size();
    }

    /**
     * @brief Returns the maximum possible number of elements.
     * @return Maximum possible number of elements.
     */
    [[nodiscard]] size_type max_size() const noexcept {
        return packed.first().max_size();
    }

    /*! @brief Clears the container. */
    void clear() noexcept {
        sparse.first().clear();
        packed.first().clear();
        rehash(0u);
    }

    /**
     * @brief Inserts an element into the container, if it does not exist.
     * @param value An element to insert into the container.
     * @return A pair consisting of an iterator to the inserted element (or to
     * the element that prevented the insertion) and a bool denoting whether the
     * insertion took place.
     */
    std::pair<iterator, bool> insert(const value_type &value) {
        return insert_or_do_nothing(value);
    }

    /*! @copydoc insert */
    std::pair<iterator, bool> insert(value_type &&value) {
        return insert_or_do_nothing(std::move(value));
    }

    /**
     * @brief Inserts elements into the container, if they do not exist.
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of elements.
     * @param last An iterator past the last element of the range of elements.
     */
    template<typename It>
    void insert(It first, It last) {
        for(; first != last; ++first) {
            insert(*first);
        }
    }

    /**
     * @brief Constructs an element in-place, if it does not exist.
     *
     * The element is also constructed when the container already has the key,
     * in which case the newly constructed object is destroyed immediately.
     *
     * @tparam Args Types of arguments to forward to the constructor of the
     * element.
     * @param args Arguments to forward to the constructor of the element.
     * @return A pair consisting of an iterator to the inserted element (or to
     * the element that prevented the insertion) and a bool denoting whether the
     * insertion took place.
     */
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args &&...args) {
        if constexpr(((sizeof...(Args) == 1u) && ... && std::is_same_v<std::decay_t<Args>, value_type>)) {
            return insert_or_do_nothing(std::forward<Args>(args)...);
        } else {
            auto &node = packed.first().emplace_back(std::piecewise_construct, std::make_tuple(packed.first().size()), std::forward_as_tuple(std::forward<Args>(args)...));
            const auto index = value_to_bucket(node.second);

            if(auto it = constrained_find(node.second, index); it != end()) {
                packed.first().pop_back();
                return std::make_pair(it, false);
            }

            std::swap(node.first, sparse.first()[index]);
            rehash_if_required();

            return std::make_pair(--end(), true);
        }
    }

    /**
     * @brief Removes an element from a given position.
     * @param pos An iterator to the element to remove.
     * @return An iterator following the removed element.
     */
    iterator erase(const_iterator pos) {
        const auto diff = pos - cbegin();
        erase(*pos);
        return begin() + diff;
    }

    /**
     * @brief Removes the given elements from a container.
     * @param first An iterator to the first element of the range of elements.
     * @param last An iterator past the last element of the range of elements.
     * @return An iterator following the last removed element.
     */
    iterator erase(const_iterator first, const_iterator last) {
        const auto dist = first - cbegin();

        for(auto from = last - cbegin(); from != dist; --from) {
            erase(packed.first()[from - 1u].second);
        }

        return (begin() + dist);
    }

    /**
     * @brief Removes the element associated with a given value.
     * @param value Value of an element to remove.
     * @return Number of elements removed (either 0 or 1).
     */
    size_type erase(const value_type &value) {
        for(size_type *curr = sparse.first().data() + value_to_bucket(value); *curr != (std::numeric_limits<size_type>::max)(); curr = &packed.first()[*curr].first) {
            if(packed.second()(packed.first()[*curr].second, value)) {
                const auto index = *curr;
                *curr = packed.first()[*curr].first;
                move_and_pop(index);
                return 1u;
            }
        }

        return 0u;
    }

    /**
     * @brief Exchanges the contents with those of a given container.
     * @param other Container to exchange the content with.
     */
    void swap(dense_set &other) {
        using std::swap;
        swap(sparse, other.sparse);
        swap(packed, other.packed);
        swap(threshold, other.threshold);
    }

    /**
     * @brief Returns the number of elements matching a value (either 1 or 0).
     * @param key Key value of an element to search for.
     * @return Number of elements matching the key (either 1 or 0).
     */
    [[nodiscard]] size_type count(const value_type &key) const {
        return find(key) != end();
    }

    /**
     * @brief Returns the number of elements matching a key (either 1 or 0).
     * @tparam Other Type of the key value of an element to search for.
     * @param key Key value of an element to search for.
     * @return Number of elements matching the key (either 1 or 0).
     */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, size_type>>
    count(const Other &key) const {
        return find(key) != end();
    }

    /**
     * @brief Finds an element with a given value.
     * @param value Value of an element to search for.
     * @return An iterator to an element with the given value. If no such
     * element is found, a past-the-end iterator is returned.
     */
    [[nodiscard]] iterator find(const value_type &value) {
        return constrained_find(value, value_to_bucket(value));
    }

    /*! @copydoc find */
    [[nodiscard]] const_iterator find(const value_type &value) const {
        return constrained_find(value, value_to_bucket(value));
    }

    /**
     * @brief Finds an element that compares _equivalent_ to a given value.
     * @tparam Other Type of an element to search for.
     * @param value Value of an element to search for.
     * @return An iterator to an element with the given value. If no such
     * element is found, a past-the-end iterator is returned.
     */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, iterator>>
    find(const Other &value) {
        return constrained_find(value, value_to_bucket(value));
    }

    /*! @copydoc find */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, const_iterator>>
    find(const Other &value) const {
        return constrained_find(value, value_to_bucket(value));
    }

    /**
     * @brief Returns a range containing all elements with a given value.
     * @param value Value of an element to search for.
     * @return A pair of iterators pointing to the first element and past the
     * last element of the range.
     */
    [[nodiscard]] std::pair<iterator, iterator> equal_range(const value_type &value) {
        const auto it = find(value);
        return {it, it + !(it == end())};
    }

    /*! @copydoc equal_range */
    [[nodiscard]] std::pair<const_iterator, const_iterator> equal_range(const value_type &value) const {
        const auto it = find(value);
        return {it, it + !(it == cend())};
    }

    /**
     * @brief Returns a range containing all elements that compare _equivalent_
     * to a given value.
     * @tparam Other Type of an element to search for.
     * @param value Value of an element to search for.
     * @return A pair of iterators pointing to the first element and past the
     * last element of the range.
     */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, std::pair<iterator, iterator>>>
    equal_range(const Other &value) {
        const auto it = find(value);
        return {it, it + !(it == end())};
    }

    /*! @copydoc equal_range */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, std::pair<const_iterator, const_iterator>>>
    equal_range(const Other &value) const {
        const auto it = find(value);
        return {it, it + !(it == cend())};
    }

    /**
     * @brief Checks if the container contains an element with a given value.
     * @param value Value of an element to search for.
     * @return True if there is such an element, false otherwise.
     */
    [[nodiscard]] bool contains(const value_type &value) const {
        return (find(value) != cend());
    }

    /**
     * @brief Checks if the container contains an element that compares
     * _equivalent_ to a given value.
     * @tparam Other Type of an element to search for.
     * @param value Value of an element to search for.
     * @return True if there is such an element, false otherwise.
     */
    template<typename Other>
    [[nodiscard]] std::enable_if_t<is_transparent_v<hasher> && is_transparent_v<key_equal>, std::conditional_t<false, Other, bool>>
    contains(const Other &value) const {
        return (find(value) != cend());
    }

    /**
     * @brief Returns an iterator to the beginning of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the beginning of the given bucket.
     */
    [[nodiscard]] const_local_iterator cbegin(const size_type index) const {
        return {packed.first().begin(), sparse.first()[index]};
    }

    /**
     * @brief Returns an iterator to the beginning of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the beginning of the given bucket.
     */
    [[nodiscard]] const_local_iterator begin(const size_type index) const {
        return cbegin(index);
    }

    /**
     * @brief Returns an iterator to the beginning of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the beginning of the given bucket.
     */
    [[nodiscard]] local_iterator begin(const size_type index) {
        return {packed.first().begin(), sparse.first()[index]};
    }

    /**
     * @brief Returns an iterator to the end of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the end of the given bucket.
     */
    [[nodiscard]] const_local_iterator cend([[maybe_unused]] const size_type index) const {
        return {packed.first().begin(), (std::numeric_limits<size_type>::max)()};
    }

    /**
     * @brief Returns an iterator to the end of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the end of the given bucket.
     */
    [[nodiscard]] const_local_iterator end(const size_type index) const {
        return cend(index);
    }

    /**
     * @brief Returns an iterator to the end of a given bucket.
     * @param index An index of a bucket to access.
     * @return An iterator to the end of the given bucket.
     */
    [[nodiscard]] local_iterator end([[maybe_unused]] const size_type index) {
        return {packed.first().begin(), (std::numeric_limits<size_type>::max)()};
    }

    /**
     * @brief Returns the number of buckets.
     * @return The number of buckets.
     */
    [[nodiscard]] size_type bucket_count() const {
        return sparse.first().size();
    }

    /**
     * @brief Returns the maximum number of buckets.
     * @return The maximum number of buckets.
     */
    [[nodiscard]] size_type max_bucket_count() const {
        return sparse.first().max_size();
    }

    /**
     * @brief Returns the number of elements in a given bucket.
     * @param index The index of the bucket to examine.
     * @return The number of elements in the given bucket.
     */
    [[nodiscard]] size_type bucket_size(const size_type index) const {
        return static_cast<size_type>(std::distance(begin(index), end(index)));
    }

    /**
     * @brief Returns the bucket for a given element.
     * @param value The value of the element to examine.
     * @return The bucket for the given element.
     */
    [[nodiscard]] size_type bucket(const value_type &value) const {
        return value_to_bucket(value);
    }

    /**
     * @brief Returns the average number of elements per bucket.
     * @return The average number of elements per bucket.
     */
    [[nodiscard]] float load_factor() const {
        return size() / static_cast<float>(bucket_count());
    }

    /**
     * @brief Returns the maximum average number of elements per bucket.
     * @return The maximum average number of elements per bucket.
     */
    [[nodiscard]] float max_load_factor() const {
        return threshold;
    }

    /**
     * @brief Sets the desired maximum average number of elements per bucket.
     * @param value A desired maximum average number of elements per bucket.
     */
    void max_load_factor(const float value) {
        ENTT_ASSERT(value > 0.f, "Invalid load factor");
        threshold = value;
        rehash(0u);
    }

    /**
     * @brief Reserves at least the specified number of buckets and regenerates
     * the hash table.
     * @param cnt New number of buckets.
     */
    void rehash(const size_type cnt) {
        auto value = cnt > minimum_capacity ? cnt : minimum_capacity;
        const auto cap = static_cast<size_type>(size() / max_load_factor());
        value = value > cap ? value : cap;

        if(const auto sz = next_power_of_two(value); sz != bucket_count()) {
            sparse.first().resize(sz);

            for(auto &&elem: sparse.first()) {
                elem = std::numeric_limits<size_type>::max();
            }

            for(size_type pos{}, last = size(); pos < last; ++pos) {
                const auto index = value_to_bucket(packed.first()[pos].second);
                packed.first()[pos].first = std::exchange(sparse.first()[index], pos);
            }
        }
    }

    /**
     * @brief Reserves space for at least the specified number of elements and
     * regenerates the hash table.
     * @param cnt New number of elements.
     */
    void reserve(const size_type cnt) {
        packed.first().reserve(cnt);
        rehash(static_cast<size_type>(std::ceil(cnt / max_load_factor())));
    }

    /**
     * @brief Returns the function used to hash the elements.
     * @return The function used to hash the elements.
     */
    [[nodiscard]] hasher hash_function() const {
        return sparse.second();
    }

    /**
     * @brief Returns the function used to compare elements for equality.
     * @return The function used to compare elements for equality.
     */
    [[nodiscard]] key_equal key_eq() const {
        return packed.second();
    }

private:
    compressed_pair<sparse_container_type, hasher> sparse;
    compressed_pair<packed_container_type, key_equal> packed;
    float threshold;
};

} // namespace entt

#endif

///////////////// END OF external/entt/container/dense_set.hpp /////////////////

/////////////////////// START OF src/Pokedex/Pokedex.cpp ///////////////////////

namespace pokesim {
template <typename Build, typename T>
void Pokedex::load(entt::dense_map<T, types::entity>& map, const entt::dense_set<T>& list, Build build) {
  map.reserve(map.size() + list.size());
  for (T listItem : list) {
    ENTT_ASSERT(!map.contains(listItem), "Shouldn't build data entries twice");
    map[listItem] = build(listItem);
  }
}

void Pokedex::loadSpecies(const entt::dense_set<dex::Species>& speciesSet) {
  load(speciesMap, speciesSet, [this](dex::Species species) { return buildSpecies(species, dexRegistry, false); });
}

void Pokedex::loadItems(const entt::dense_set<dex::Item>& itemSet) {
  load(itemsMap, itemSet, [this](dex::Item item) { return buildItem(item, dexRegistry, false); });
}

void Pokedex::loadMoves(const entt::dense_set<dex::Move>& moveSet) {
  load(movesMap, moveSet, [this](dex::Move move) { return buildMove(move, dexRegistry, false); });
}

void Pokedex::loadAbilities(const entt::dense_set<dex::Ability>& abilitySet) {
  load(abilitiesMap, abilitySet, [this](dex::Ability ability) { return buildAbility(ability, dexRegistry, false); });
}

types::entity Pokedex::buildActionMove(dex::Move move, types::registry& registry) const {
  return buildMove(move, registry, true);
}
}  // namespace pokesim

//////////////////////// END OF src/Pokedex/Pokedex.cpp ////////////////////////

/////////////// START OF src/Pokedex/Abilities/AbilityEvents.cpp ///////////////

namespace pokesim::dex {
void internal::StaticEvents::onDamagingHit(Simulation& /*simulation*/) {}
void internal::StaticEvents::onModifySpe(stat::EffectiveSpeed& effectiveSpeed) {
  effectiveSpeed.effectiveSpeed /= 2;
}
}  // namespace pokesim::dex

//////////////// END OF src/Pokedex/Abilities/AbilityEvents.cpp ////////////////

///////////////// START OF src/Components/Tags/StatusTags.cpp //////////////////

// TODO(aed3): Make this auto generated

namespace pokesim::status::tags {
void enumToTag(dex::Status status, types::handle& handle) {
  switch (status) {
    case dex::Status::BRN: handle.emplace<Burn>(); return;
    case dex::Status::FRZ: handle.emplace<Freeze>(); return;
    case dex::Status::PAR: handle.emplace<Paralysis>(); return;
    case dex::Status::PSN: handle.emplace<Poison>(); return;
    case dex::Status::SLP: handle.emplace<Sleep>(); return;
    case dex::Status::TOX: handle.emplace<Toxic>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for status that does not exist");
    }
  }
}
}  // namespace pokesim::status::tags

////////////////// END OF src/Components/Tags/StatusTags.cpp ///////////////////

///////////////// START OF src/Components/Tags/NatureTags.hpp //////////////////

// TODO(aed3): Make this auto generated

namespace pokesim::nature::tags {
struct Adamant {};
struct Bashful {};
struct Bold {};
struct Brave {};
struct Calm {};
struct Careful {};
struct Docile {};
struct Gentle {};
struct Hardy {};
struct Hasty {};
struct Impish {};
struct Jolly {};
struct Lax {};
struct Lonely {};
struct Mild {};
struct Modest {};
struct Naive {};
struct Naughty {};
struct Quiet {};
struct Quirky {};
struct Rash {};
struct Relaxed {};
struct Sassy {};
struct Serious {};
struct Timid {};

// Assigns a nature's tag to a handle
inline void enumToTag(dex::Nature nature, types::handle handle);
}  // namespace pokesim::nature::tags

////////////////// END OF src/Components/Tags/NatureTags.hpp ///////////////////

///////////////// START OF src/Components/Tags/NatureTags.cpp //////////////////

// TODO(aed3): Make this auto generated

namespace pokesim::nature::tags {
void enumToTag(dex::Nature nature, types::handle handle) {
  switch (nature) {
    case dex::Nature::ADAMANT: handle.emplace<Adamant>(); return;
    case dex::Nature::BASHFUL: handle.emplace<Bashful>(); return;
    case dex::Nature::BOLD: handle.emplace<Bold>(); return;
    case dex::Nature::BRAVE: handle.emplace<Brave>(); return;
    case dex::Nature::CALM: handle.emplace<Calm>(); return;
    case dex::Nature::CAREFUL: handle.emplace<Careful>(); return;
    case dex::Nature::DOCILE: handle.emplace<Docile>(); return;
    case dex::Nature::GENTLE: handle.emplace<Gentle>(); return;
    case dex::Nature::HARDY: handle.emplace<Hardy>(); return;
    case dex::Nature::HASTY: handle.emplace<Hasty>(); return;
    case dex::Nature::IMPISH: handle.emplace<Impish>(); return;
    case dex::Nature::JOLLY: handle.emplace<Jolly>(); return;
    case dex::Nature::LAX: handle.emplace<Lax>(); return;
    case dex::Nature::LONELY: handle.emplace<Lonely>(); return;
    case dex::Nature::MILD: handle.emplace<Mild>(); return;
    case dex::Nature::MODEST: handle.emplace<Modest>(); return;
    case dex::Nature::NAIVE: handle.emplace<Naive>(); return;
    case dex::Nature::NAUGHTY: handle.emplace<Naughty>(); return;
    case dex::Nature::QUIET: handle.emplace<Quiet>(); return;
    case dex::Nature::QUIRKY: handle.emplace<Quirky>(); return;
    case dex::Nature::RASH: handle.emplace<Rash>(); return;
    case dex::Nature::RELAXED: handle.emplace<Relaxed>(); return;
    case dex::Nature::SASSY: handle.emplace<Sassy>(); return;
    case dex::Nature::SERIOUS: handle.emplace<Serious>(); return;
    case dex::Nature::TIMID: handle.emplace<Timid>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for nature that does not exist");
    }
  }
}
}  // namespace pokesim::nature::tags

////////////////// END OF src/Components/Tags/NatureTags.cpp ///////////////////

////////////////// START OF src/Components/Tags/ItemTags.hpp ///////////////////

// TODO(aed3): Make this auto generated

namespace pokesim::item::tags {
struct AssaultVest {};
struct BrightPowder {};
struct ChoiceScarf {};
struct ChoiceSpecs {};
struct FocusSash {};
struct LifeOrb {};

// Assigns an item's tag to a handle
inline void enumToTag(dex::Item item, types::handle handle);
}  // namespace pokesim::item::tags

/////////////////// END OF src/Components/Tags/ItemTags.hpp ////////////////////

////////////////// START OF src/Components/Tags/ItemTags.cpp ///////////////////

// TODO(aed3): Make this auto generated

namespace pokesim::item::tags {
void enumToTag(dex::Item item, types::handle handle) {
  switch (item) {
    case dex::Item::ASSAULT_VEST: handle.emplace<AssaultVest>(); return;
    case dex::Item::BRIGHT_POWDER: handle.emplace<BrightPowder>(); return;
    case dex::Item::CHOICE_SCARF: handle.emplace<ChoiceScarf>(); return;
    case dex::Item::CHOICE_SPECS: handle.emplace<ChoiceSpecs>(); return;
    case dex::Item::FOCUS_SASH: handle.emplace<FocusSash>(); return;
    case dex::Item::LIFE_ORB: handle.emplace<LifeOrb>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for item that does not exist");
    }
  }
}
}  // namespace pokesim::item::tags

/////////////////// END OF src/Components/Tags/ItemTags.cpp ////////////////////

///////////////// START OF src/Components/Tags/AbilityTags.hpp /////////////////

// TODO(aed3): Make this auto generated

namespace pokesim::ability::tags {
struct Defiant {};
struct Infiltrator {};
struct IronFist {};
struct Static {};
struct SweetVeil {};
struct Trace {};

// Assigns an ability's tag to a handle
inline void enumToTag(dex::Ability ability, types::handle handle);
}  // namespace pokesim::ability::tags

////////////////// END OF src/Components/Tags/AbilityTags.hpp //////////////////

///////////////// START OF src/Components/Tags/AbilityTags.cpp /////////////////

// TODO(aed3): Make this auto generated

namespace pokesim::ability::tags {
void enumToTag(dex::Ability ability, types::handle handle) {
  switch (ability) {
    case dex::Ability::DEFIANT: handle.emplace<Defiant>(); return;
    case dex::Ability::INFILTRATOR: handle.emplace<Infiltrator>(); return;
    case dex::Ability::IRON_FIST: handle.emplace<IronFist>(); return;
    case dex::Ability::STATIC: handle.emplace<Static>(); return;
    case dex::Ability::SWEET_VEIL: handle.emplace<SweetVeil>(); return;
    case dex::Ability::TRACE: handle.emplace<Trace>(); return;
    default: {
      ENTT_ASSERT(false, "Adding tag for ability that does not exist");
    }
  }
}
}  // namespace pokesim::ability::tags

////////////////// END OF src/Components/Tags/AbilityTags.cpp //////////////////

//////////// START OF src/CalcDamage/Setup/CalcDamageInputSetup.cpp ////////////

namespace pokesim::calc_damage {
void InputSetup::setAttacker(types::entity entity) {
  handle.emplace<Attacker>(entity);
}

void InputSetup::setDefender(types::entity entity) {
  handle.emplace<Defender>(entity);
}

void InputSetup::setMove(dex::Move move) {
  handle.emplace<MoveName>(move);
}

void InputSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
}
}  // namespace pokesim::calc_damage

///////////// END OF src/CalcDamage/Setup/CalcDamageInputSetup.cpp /////////////

//////////////////// START OF src/CalcDamage/CalcDamage.cpp ////////////////////

namespace pokesim::calc_damage {
void run(Simulation& simulation) {
  getDamage(simulation);
}

void criticalHitRandomChance(Simulation& simulation) {
  // Set critical hit chances as random chance variable

  randomChance<5U>(simulation);
}

void modifyDamageWithTypes(Simulation& /*simulation*/) {}

void getDamageRole(Simulation& simulation) {
  sampleRandomChance(simulation);
}

void getCritMultiplier(Simulation& simulation) {
  runModifyCritRatioEvent(simulation);
}

void getDamage(Simulation& simulation) {
  getCritMultiplier(simulation);
  criticalHitRandomChance(simulation);

  // Get base power, boosts, get atk/def stats
  runBasePowerEvent(simulation);

  getDamageRole(simulation);

  modifyDamageWithTypes(simulation);
}
}  // namespace pokesim::calc_damage

///////////////////// END OF src/CalcDamage/CalcDamage.cpp /////////////////////

////////////// START OF src/Components/EntityHolders/FoeSide.hpp ///////////////

namespace pokesim {
// Contains the entity of a side's opponent.
struct FoeSide {
  types::entity val{};
};
}  // namespace pokesim

/////////////// END OF src/Components/EntityHolders/FoeSide.hpp ////////////////

///////////////// START OF src/Battle/Setup/SideStateSetup.cpp /////////////////

namespace pokesim {
void SideStateSetup::initBlank() {
  handle.emplace<Battle>();
  handle.emplace<Team>();
  handle.emplace<FoeSide>();
}

void SideStateSetup::setTeam(std::vector<PokemonStateSetup>& team) {
  Team& teamEntities = handle.emplace<Team>();
  Battle battle = handle.get<Battle>();
  ENTT_ASSERT(
    team.size() <= teamEntities.val.max_size(),
    "Cannot add more Pokemon to a team than types::internal::MAX_TEAM_SIZE");

  for (std::size_t i = 0; i < team.size(); i++) {
    teamEntities.val.push_back(team[i].entity());
    team[i].setPostion((types::teamPositionIndex)(i + 1));
    team[i].setSide(entity());
    team[i].setBattle(battle.val);
  }
}

void SideStateSetup::setOpponent(types::entity entity) {
  handle.emplace<FoeSide>(entity);
}

void SideStateSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
}
}  // namespace pokesim

////////////////// END OF src/Battle/Setup/SideStateSetup.cpp //////////////////

///////////// START OF src/Components/EntityHolders/MoveSlots.hpp //////////////

namespace pokesim {
// Contains a list of entities of the moves a Pokemon known.
struct MoveSlots {
  types::moveSlots<types::entity> val{};
};
}  // namespace pokesim

////////////// END OF src/Components/EntityHolders/MoveSlots.hpp ///////////////

//////////////////////// START OF src/Components/ID.hpp ////////////////////////

namespace pokesim {
struct Id {
  types::stateId val = 1;
};
}  // namespace pokesim

///////////////////////// END OF src/Components/ID.hpp /////////////////////////

////////////////////// START OF src/Components/Level.hpp ///////////////////////

namespace pokesim {
// A Pokemon's level
struct Level {
  types::level val = 1;
};
}  // namespace pokesim

/////////////////////// END OF src/Components/Level.hpp ////////////////////////

//////////////// START OF src/Components/Names/GenderNames.hpp /////////////////

namespace pokesim {
struct GenderName {
  dex::Gender name = dex::Gender::NO_GENDER;
};
}  // namespace pokesim

///////////////// END OF src/Components/Names/GenderNames.hpp //////////////////

//////////////// START OF src/Components/Names/NatureNames.hpp /////////////////

namespace pokesim {
struct NatureName {
  dex::Nature name = dex::Nature::NO_NATURE;
};
}  // namespace pokesim

///////////////// END OF src/Components/Names/NatureNames.hpp //////////////////

//////////////// START OF src/Components/Names/StatusNames.hpp /////////////////

namespace pokesim {
struct StatusName {
  dex::Status name = dex::Status::NO_STATUS;
};
}  // namespace pokesim

///////////////// END OF src/Components/Names/StatusNames.hpp //////////////////

///////////////////// START OF src/Components/Position.hpp /////////////////////

namespace pokesim {
// The position of a Pokemon in its team's order (starts at 1)
struct Position {
  types::teamPositionIndex val = 1;
};
}  // namespace pokesim

////////////////////// END OF src/Components/Position.hpp //////////////////////

/////////////// START OF src/Battle/Setup/PokemonStateSetup.cpp ////////////////

namespace pokesim {
void PokemonStateSetup::initBlank() {
  handle.emplace<SpeciesName>();
  handle.emplace<Side>();
  handle.emplace<Battle>();
  setAutoID();
}

void PokemonStateSetup::setAutoID() {
  setID((uint16_t)handle.registry()->view<SpeciesName>().size() + 1);
}

void PokemonStateSetup::setID(types::stateId id) {
  handle.emplace<Id>(id);
}

void PokemonStateSetup::setSpecies(dex::Species speciesName) {
  handle.emplace<SpeciesName>(speciesName);
}

void PokemonStateSetup::setSide(types::entity entity) {
  handle.emplace<Side>(entity);
}

void PokemonStateSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
}

void PokemonStateSetup::setLevel(types::level level) {
  handle.emplace<Level>(level);
}

void PokemonStateSetup::setGender(dex::Gender gender) {
  handle.emplace<GenderName>(gender);
}

void PokemonStateSetup::setAbility(dex::Ability ability) {
  handle.emplace<AbilityName>(ability);
  ability::tags::enumToTag(ability, handle);
}

void PokemonStateSetup::setItem(dex::Item item) {
  handle.emplace<ItemName>(item);
  item::tags::enumToTag(item, handle);
}

void PokemonStateSetup::setMoves(const std::vector<types::entity>& moveSlots) {
  MoveSlots& moveEntities = handle.emplace<MoveSlots>();
  ENTT_ASSERT(
    moveSlots.size() <= moveEntities.val.max_size(),
    "Cannot add more moves to a Pokemon than types::internal::MAX_MOVE_SLOTS");
  for (types::entity moveSlot : moveSlots) {
    moveEntities.val.push_back(moveSlot);
  }
}

void PokemonStateSetup::setPostion(types::teamPositionIndex position) {
  handle.emplace<Position>(position);
}

void PokemonStateSetup::setStatus(dex::Status status) {
  handle.emplace<StatusName>(status);
  status::tags::enumToTag(status, handle);
}

void PokemonStateSetup::setNature(dex::Nature nature) {
  handle.emplace<NatureName>(nature);
  nature::tags::enumToTag(nature, handle);
}

void PokemonStateSetup::setEVs(
  types::ev hp, types::ev atk, types::ev def, types::ev spa, types::ev spd, types::ev spe) {
  handle.emplace<Evs>(hp, atk, def, spa, spd, spe);
}

void PokemonStateSetup::setEVs(const Evs& evs) {
  handle.emplace<Evs>(evs);
}

void PokemonStateSetup::setIVs(
  types::iv hp, types::iv atk, types::iv def, types::iv spa, types::iv spd, types::iv spe) {
  handle.emplace<Ivs>(hp, atk, def, spa, spd, spe);
}

void PokemonStateSetup::setIVs(const Ivs& ivs) {
  handle.emplace<Ivs>(ivs);
}
}  // namespace pokesim

//////////////// END OF src/Battle/Setup/PokemonStateSetup.cpp /////////////////

///////////////// START OF src/Battle/Setup/MoveStateSetup.cpp /////////////////

namespace pokesim {
void MoveStateSetup::initBlank() {
  handle.emplace<MoveName>();
  handle.emplace<Pp>();
  handle.emplace<MaxPp>();
}

void MoveStateSetup::setName(dex::Move moveName) {
  handle.emplace<MoveName>(moveName);
}

void MoveStateSetup::setPP(types::pp pp) {
  handle.emplace<Pp>(pp);
}

void MoveStateSetup::setMaxPP(types::pp maxPp) {
  handle.emplace<MaxPp>(maxPp);
}
}  // namespace pokesim

////////////////// END OF src/Battle/Setup/MoveStateSetup.cpp //////////////////

/////////////////// START OF src/Components/Probability.hpp ////////////////////

namespace pokesim {
/**
 * @brief The probability of all the previous actions in a battle's simulation occurring.
 *
 * Calculated by multiplying the various Accuracy and Chance numbers of a battle state's events.
 */
struct Probability {
  types::probability val = 1;
};
}  // namespace pokesim

//////////////////// END OF src/Components/Probability.hpp /////////////////////

//////////////// START OF src/Battle/Setup/BattleStateSetup.cpp ////////////////

#include <atomic>

namespace pokesim {
BattleStateSetup::BattleStateSetup(types::registry& registry, types::entity entity) : StateSetupBase(registry, entity) {
  if (!handle.any_of<ActionQueue>()) {
    handle.emplace<ActionQueue>();
  }
}

void BattleStateSetup::initBlank() {
  handle.emplace<Sides>();
  handle.emplace<ActionQueue>();
  setAutoID();
  setTurn(0);
  setProbability(1);
}

void BattleStateSetup::setAutoID() {
  setID((types::stateId)handle.registry()->view<Sides>().size());
}

void BattleStateSetup::setID(types::stateId id) {
  handle.emplace_or_replace<Id>(id);
}

void BattleStateSetup::setSide(PlayerSideId sideID, types::entity sideEntity) {
  auto& sides = handle.get_or_emplace<Sides>();
  switch (sideID) {
    case PlayerSideId::P1: sides.p1() = sideEntity; break;
    case PlayerSideId::P2: sides.p2() = sideEntity; break;
    default: ENTT_FAIL("sideID must be assigned P1 or P2"); break;
  }
}

void BattleStateSetup::setRNGSeed(std::optional<types::rngState> seed) {
  if (!seed.has_value()) {
    static std::atomic_uint64_t state = 1;
    seed = state;
    types::rngState newState = state;
    internal::nextRandomValue(newState);
    state = newState;
  }
  handle.emplace<RngSeed>(seed.value());
}

void BattleStateSetup::setActionQueue(const std::vector<types::entity>& queue) {
  handle.emplace<ActionQueue>(queue);
}

void BattleStateSetup::setTurn(types::battleTurn turn) {
  handle.emplace<Turn>(turn);
}

void BattleStateSetup::setActivePokemon(types::entity activePokemon) {
  handle.registry()->emplace<tags::ActivePokemon>(activePokemon);
}

void BattleStateSetup::setCurrentActionTarget(types::targets<types::entity> actionTargets) {
  handle.emplace<CurrentActionTargets>(actionTargets);
  for (types::entity entity : actionTargets) {
    handle.registry()->emplace<tags::CurrentActionMoveTarget>(entity);
  }
}

void BattleStateSetup::setCurrentActionSource(types::entity actionSource) {
  handle.emplace<CurrentActionSource>(actionSource);
  handle.registry()->emplace<tags::CurrentActionMoveSource>(actionSource);
}

void BattleStateSetup::setCurrentActionMove(types::entity actionMove) {
  handle.emplace<CurrentActionMove>(actionMove);
  handle.registry()->emplace<tags::CurrentActionMove>(actionMove);
}

void BattleStateSetup::setProbability(types::probability probability) {
  handle.emplace<Probability>(probability);
}

std::vector<BattleStateSetup> BattleStateSetup::clone(std::optional<types::cloneIndex> cloneCount) {
  types::registry& registry = *handle.registry();

  handle.emplace<tags::CloneFrom>();
  const types::ClonedEntityMap entityMap = pokesim::clone(registry, cloneCount);

  const auto& clonedBattles = entityMap.at(handle.entity());
  std::vector<BattleStateSetup> clonedSetups;
  clonedSetups.reserve(clonedBattles.size());

  for (types::entity entity : clonedBattles) {
    clonedSetups.emplace_back(registry, entity);
  }

  return clonedSetups;
}
}  // namespace pokesim

///////////////// END OF src/Battle/Setup/BattleStateSetup.cpp /////////////////

//////////// START OF src/Components/EntityHolders/LastUsedMove.hpp ////////////

namespace pokesim {
struct LastUsedMove {
  types::entity val{};
};
}  // namespace pokesim

///////////// END OF src/Components/EntityHolders/LastUsedMove.hpp /////////////

////////////// START OF src/Battle/Pokemon/ManagePokemonState.cpp //////////////

namespace pokesim {
void deductPp(Pp& pp) {
  if (pp.val) {
    pp.val -= 1;
  }
}

void setLastMoveUsed(types::registry& registry, const CurrentActionSource& source, const CurrentActionMoveSlot& move) {
  registry.emplace<LastUsedMove>(source.val, move.val);
}

void resetEffectiveSpeed(types::handle handle, stat::Spe spe) {
  handle.emplace_or_replace<stat::EffectiveSpeed>(spe.val);
}

void updateSpeed(Simulation& simulation) {
  internal::SelectForPokemonView<tags::SpeedUpdateRequired> selectedSpeedUpdateRequired{simulation};

  simulation.viewForSelectedPokemon<resetEffectiveSpeed>();

  // apply boosts
  runModifySpe(simulation);
  // trick room

  selectedSpeedUpdateRequired.deselect();
  simulation.registry.clear<tags::SpeedUpdateRequired>();
}
}  // namespace pokesim

/////////////// END OF src/Battle/Pokemon/ManagePokemonState.cpp ///////////////

////////////////// START OF src/Battle/ManageBattleState.cpp ///////////////////

namespace pokesim {
void setCurrentActionTarget(types::handle battleHandle, const Sides& sides, const CurrentAction& action) {
  types::registry& registry = *battleHandle.registry();
  const TargetSlotName& targetSlotName = registry.get<TargetSlotName>(action.val);
  types::entity targetEntity = slotToPokemonEntity(registry, sides, targetSlotName.name);

  if (!registry.any_of<tags::Fainted>(targetEntity)) {
    battleHandle.emplace<CurrentActionTargets>(std::initializer_list<types::entity>{targetEntity});
    registry.emplace<tags::CurrentActionMoveTarget>(targetEntity);
  }
  else {
    // Set tag to get random target after this function exits
  }
}

void setCurrentActionSource(types::handle battleHandle, const Sides& sides, const CurrentAction& action) {
  types::registry& registry = *battleHandle.registry();
  const SourceSlotName& sourceSlotName = registry.get<SourceSlotName>(action.val);
  types::entity sourceEntity = slotToPokemonEntity(registry, sides, sourceSlotName.name);

  battleHandle.emplace<CurrentActionSource>(sourceEntity);
  registry.emplace<tags::CurrentActionMoveSource>(sourceEntity);
}

void setCurrentActionMove(
  types::handle battleHandle, const Simulation& simulation, const CurrentActionSource& source,
  const CurrentAction& action) {
  types::registry& registry = *battleHandle.registry();
  const action::Move& move = registry.get<action::Move>(action.val);
  const MoveSlots& moveSlots = registry.get<MoveSlots>(source.val);

  types::entity moveSlotEntity = moveToEntity(registry, moveSlots, move.name);
  types::entity moveEntity = simulation.pokedex.buildActionMove(move.name, registry);

  battleHandle.emplace<CurrentActionMove>(moveEntity);
  battleHandle.emplace<CurrentActionMoveSlot>(moveSlotEntity);
  registry.emplace<Battle>(moveEntity, battleHandle.entity());
  registry.emplace<tags::CurrentActionMove>(moveEntity);
  registry.emplace<tags::CurrentActionMoveSlot>(moveSlotEntity);
}

void clearCurrentAction(Simulation& simulation) {
  types::registry& registry = simulation.registry;
  registry.clear<CurrentAction>();
  registry.clear<CurrentActionTargets>();
  registry.clear<CurrentActionSource>();
  registry.clear<CurrentActionMove>();
  registry.clear<CurrentActionMoveSlot>();

  registry.clear<tags::CurrentActionMoveTarget>();
  registry.clear<tags::CurrentActionMoveSource>();
  registry.clear<tags::CurrentActionMoveSlot>();

  auto actionMoves = registry.view<tags::CurrentActionMove>();
  registry.destroy(actionMoves.begin(), actionMoves.end());

  auto battles = simulation.selectedBattleEntities();
  registry.remove<
    action::Item,
    action::Move,
    action::tags::BeforeTurn,
    action::tags::Dynamax,
    action::tags::MegaEvolve,
    action::tags::MidTurnSwitchIn,
    action::tags::PostFoeFaintSwitchIn,
    action::tags::PreSwitchOut,
    action::tags::PreTurnSwitchIn,
    action::tags::PrimalRevert,
    action::tags::Residual,
    action::tags::RevivalBlessing,
    action::tags::Switch,
    action::tags::SwitchOut,
    action::tags::Terastallize>(battles.begin(), battles.end());
}
}  // namespace pokesim

/////////////////// END OF src/Battle/ManageBattleState.cpp ////////////////////

/////////////////// START OF src/Battle/Helpers/Helpers.cpp ////////////////////

#include <cstdint>

namespace pokesim {

types::entity slotToSideEntity(const Sides& sides, Slot targetSlot) {
  ENTT_ASSERT(targetSlot != Slot::NONE, "Can only get entity from valid target slot");
  types::entity sideEntity = sides.val[((std::uint8_t)targetSlot - 1) % 2];
  return sideEntity;
}

types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot targetSlot) {
  types::teamPositionIndex index = ((std::uint8_t)targetSlot - 1) / 2;

  const Team& team = registry.get<Team>(sideEntity);
  ENTT_ASSERT(team.val.size() > index, "Choosing a target slot for team member that does not exist");
  return team.val[index];
}

types::entity slotToPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot) {
  ENTT_ASSERT(targetSlot != Slot::NONE, "Can only get entity from valid target slot");
  return slotToPokemonEntity(registry, slotToSideEntity(sides, targetSlot), targetSlot);
}

types::entity slotToAllyPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot) {
  ENTT_ASSERT(targetSlot != Slot::NONE, "Can only get entity from valid target slot");
  Slot allySlot = Slot::NONE;
  types::teamPositionIndex index = 0;

  switch (targetSlot) {
    case Slot::P1A: {
      allySlot = Slot::P1B;
      index = 1;
      break;
    }
    case Slot::P1B:
    case Slot::P1C:
    case Slot::P1D:
    case Slot::P1E:
    case Slot::P1F: {
      allySlot = Slot::P1A;
      break;
    }
    case Slot::P2A: {
      allySlot = Slot::P2B;
      index = 1;
      break;
    }
    case Slot::P2B:
    case Slot::P2C:
    case Slot::P2D:
    case Slot::P2E:
    case Slot::P2F: {
      allySlot = Slot::P2A;
      break;
    }
    default: break;
  }

  types::entity sideEntity = slotToSideEntity(sides, allySlot);
  const Team& team = registry.get<Team>(sideEntity);
  if (team.val.size() <= index) {
    return entt::null;
  }

  types::entity allyEntity = team.val[index];
  if (registry.any_of<tags::Fainted>(allyEntity)) {
    return entt::null;
  }

  return allyEntity;
}

types::entity moveToEntity(const types::registry& registry, const MoveSlots& moveSlots, dex::Move move) {
  for (types::entity moveSlot : moveSlots.val) {
    if (registry.get<MoveName>(moveSlot).name == move) {
      return moveSlot;
    }
  }

  ENTT_FAIL("No move of entity found");
  return entt::null;
}
}  // namespace pokesim

//////////////////// END OF src/Battle/Helpers/Helpers.cpp /////////////////////

////////////// START OF src/Components/EntityHolders/Pokemon.hpp ///////////////

namespace pokesim {
// Contains the entity pointing to a Pokemon.
struct Pokemon {
  types::entity val{};
};
}  // namespace pokesim

/////////////// END OF src/Components/EntityHolders/Pokemon.hpp ////////////////

///////////////////// START OF src/Battle/Clone/Clone.cpp //////////////////////

#include <vector>

namespace pokesim {
types::ClonedEntityMap clone(types::registry& registry, std::optional<types::cloneIndex> cloneCount) {
  types::cloneIndex count = cloneCount.value_or(1);
  types::ClonedEntityMap entityMap;
  entt::dense_map<entt::id_type, std::vector<types::entity>> srcEntityStorages;

  internal::cloneBattle(registry, entityMap, srcEntityStorages, count);
  internal::cloneSide(registry, entityMap, srcEntityStorages, count);
  internal::cloneActionQueue(registry, entityMap, srcEntityStorages, count);
  internal::clonePokemon(registry, entityMap, srcEntityStorages, count);
  internal::cloneMove(registry, entityMap, srcEntityStorages, count);

  for (auto [id, storage] : registry.storage()) {
    if (srcEntityStorages.contains(id)) {
      const auto& sources = srcEntityStorages.at(id);
      storage.reserve(storage.size() + sources.size() * count);
      for (types::entity src : sources) {
        auto* value = storage.value(src);
        for (types::cloneIndex i = 0; i < count; i++) {
          storage.push(entityMap[src][i], value);
        }
      }
    }
  }

  auto& cloneToStorage = registry.storage<CloneTo>();
  cloneToStorage.reserve(entityMap.size() * count);

  for (const auto& [src, destinations] : entityMap) {
    cloneToStorage.insert(destinations.begin(), destinations.end());
  }

  for (const auto& [src, destinations] : entityMap) {
    for (types::cloneIndex i = 0; i < count; i++) {
      cloneToStorage.get(destinations[i]).val = i;
    }
  }

  internal::remapActionQueueEntityMembers(registry, entityMap);
  internal::remapBattleEntityMembers(registry, entityMap);
  internal::remapFoeSideEntityMembers(registry, entityMap);
  internal::remapMoveEntityMembers(registry, entityMap);
  internal::remapMoveEffectEntityMembers(registry, entityMap);
  internal::remapMoveSlotsEntityMembers(registry, entityMap);
  internal::remapPokemonEntityMembers(registry, entityMap);
  internal::remapSideEntityMembers(registry, entityMap);
  internal::remapSidesEntityMembers(registry, entityMap);
  internal::remapTeamEntityMembers(registry, entityMap);
  internal::remapActionTargetsEntityMembers(registry, entityMap);

  registry.clear<CloneTo, tags::CloneFrom>();

  return entityMap;
}

namespace internal {
void cloneEntity(
  types::entity src, types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (auto [id, storage] : registry.storage()) {
    if (storage.contains(src)) {
      srcEntityStorages[id].push_back(src);
    }
  }

  auto& destinations = entityMap[src] = std::vector<types::entity>{cloneCount};
  registry.create(destinations.begin(), destinations.end());
}

void cloneBattle(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (const auto [entity, sides, actionQueue] : registry.view<tags::CloneFrom, Sides, ActionQueue>().each()) {
    registry.emplace<tags::CloneFrom>(sides.p1());
    registry.emplace<tags::CloneFrom>(sides.p2());
    for (auto queueItem : actionQueue.val) {
      registry.emplace<tags::CloneFrom>(queueItem);
    }

    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void cloneSide(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (const auto [entity, team] : registry.view<tags::CloneFrom, Team>().each()) {
    for (auto pokemon : team.val) {
      registry.emplace<tags::CloneFrom>(pokemon);
    }

    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void cloneActionQueue(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (types::entity entity : registry.view<tags::CloneFrom, SpeedSort>()) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void clonePokemon(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (const auto [entity, species, moveSlots] : registry.view<tags::CloneFrom, SpeciesName, MoveSlots>().each()) {
    for (auto move : moveSlots.val) {
      registry.emplace<tags::CloneFrom>(move);
    }

    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void cloneMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (types::entity entity : registry.view<tags::CloneFrom, MoveName>()) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void remapEntity(types::entity& entity, const CloneTo& cloneTo, const types::ClonedEntityMap& entityMap) {
  ENTT_ASSERT(entityMap.contains(entity), "Source node was not loaded into the map");
  ENTT_ASSERT(entityMap.at(entity).size() > cloneTo.val, "More entities are trying to be copied to than were copied");
  entity = entityMap.at(entity)[cloneTo.val];
}

template <typename Component, typename GetEntity>
void remapEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap, GetEntity getEntity) {
  for (auto [clonedEntity, cloneTo, component] : registry.view<CloneTo, Component>().each()) {
    types::entity& entity = getEntity(component);
    remapEntity(entity, cloneTo, entityMap);
  }
}

template <typename Component, typename GetEntityList>
void remapEntityListMembers(
  types::registry& registry, const types::ClonedEntityMap& entityMap, GetEntityList getEntityList) {
  for (auto [clonedEntity, cloneTo, component] : registry.view<CloneTo, Component>().each()) {
    auto& entities = getEntityList(component);
    for (types::entity& entity : entities) {
      remapEntity(entity, cloneTo, entityMap);
    }
  }
}

void remapActionQueueEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntityList = [](ActionQueue & queue) -> auto& {
    return queue.val;
  };

  remapEntityListMembers<ActionQueue>(registry, entityMap, getEntityList);
}

void remapBattleEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](Battle & battle) -> auto& {
    return battle.val;
  };

  remapEntityMembers<Battle>(registry, entityMap, getEntity);
}

void remapFoeSideEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](FoeSide & foeSide) -> auto& {
    return foeSide.val;
  };

  remapEntityMembers<FoeSide>(registry, entityMap, getEntity);
}

void remapMoveEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](LastUsedMove & lastUsedMove) -> auto& {
    return lastUsedMove.val;
  };

  remapEntityMembers<LastUsedMove>(registry, entityMap, getEntity);
}

void remapMoveEffectEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](MoveEffect & moveEffect) -> auto& {
    return moveEffect.val;
  };

  remapEntityMembers<MoveEffect>(registry, entityMap, getEntity);
}

void remapMoveSlotsEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntityList = [](MoveSlots & moveSlots) -> auto& {
    return moveSlots.val;
  };

  remapEntityListMembers<MoveSlots>(registry, entityMap, getEntityList);
}

void remapPokemonEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](Pokemon & pokemon) -> auto& {
    return pokemon.val;
  };

  remapEntityMembers<Pokemon>(registry, entityMap, getEntity);
}

void remapSideEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](Side & side) -> auto& {
    return side.val;
  };

  remapEntityMembers<Side>(registry, entityMap, getEntity);
}

void remapSidesEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  for (auto [clonedEntity, cloneTo, sides] : registry.view<CloneTo, Sides>().each()) {
    remapEntity(sides.p1(), cloneTo, entityMap);
    remapEntity(sides.p2(), cloneTo, entityMap);
  }
}

void remapTeamEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntityList = [](Team & team) -> auto& {
    return team.val;
  };

  remapEntityListMembers<Team>(registry, entityMap, getEntityList);
}

void remapActionTargetsEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntityList = [](CurrentActionTargets & targets) -> auto& {
    return targets.val;
  };

  remapEntityListMembers<CurrentActionTargets>(registry, entityMap, getEntityList);
}
}  // namespace internal
}  // namespace pokesim

////////////////////// END OF src/Battle/Clone/Clone.cpp ///////////////////////

////////// START OF src/Components/AnalyzeEffect/AttackerDefender.hpp //////////

namespace pokesim::analyze_effect {
struct Attacker {
  types::entity val{};
};

struct Defender {
  types::entity val{};
};
}  // namespace pokesim::analyze_effect

/////////// END OF src/Components/AnalyzeEffect/AttackerDefender.hpp ///////////

///////////// START OF src/Components/Names/PseudoWeatherNames.hpp /////////////

namespace pokesim {
struct PseudoWeatherName {
  dex::PseudoWeather name = dex::PseudoWeather::NO_PSEUDO_WEATHER;
};
}  // namespace pokesim

////////////// END OF src/Components/Names/PseudoWeatherNames.hpp //////////////

///////////// START OF src/Components/Names/SideConditionNames.hpp /////////////

namespace pokesim {
struct SideConditionName {
  dex::SideCondition name = dex::SideCondition::NO_SIDE_CONDITION;
};
}  // namespace pokesim

////////////// END OF src/Components/Names/SideConditionNames.hpp //////////////

//////////////// START OF src/Components/Names/TerrainNames.hpp ////////////////

namespace pokesim {
struct TerrainName {
  dex::Terrain name = dex::Terrain::NO_TERRAIN;
};
}  // namespace pokesim

///////////////// END OF src/Components/Names/TerrainNames.hpp /////////////////

/////////////// START OF src/Components/Names/VolatileNames.hpp ////////////////

namespace pokesim {
struct VolatileName {
  dex::Volatile name = dex::Volatile::NO_VOLATILE;
};
}  // namespace pokesim

//////////////// END OF src/Components/Names/VolatileNames.hpp /////////////////

//////////////// START OF src/Components/Names/WeatherNames.hpp ////////////////

namespace pokesim {
struct WeatherName {
  dex::Weather name = dex::Weather::NO_WEATHER;
};
}  // namespace pokesim

///////////////// END OF src/Components/Names/WeatherNames.hpp /////////////////

///////// START OF src/AnalyzeEffect/Setup/AnalyzeEffectInputSetup.cpp /////////

namespace pokesim::analyze_effect {
void InputSetup::setAttacker(types::entity entity) {
  handle.emplace<Attacker>(entity);
}

void InputSetup::setDefender(types::entity entity) {
  handle.emplace<Defender>(entity);
}

void InputSetup::setEffect(types::effectEnum effect) {
  if (effect.holds<dex::PseudoWeather>()) {
    handle.emplace<PseudoWeatherName>(effect.get<dex::PseudoWeather>());
  }
  else if (effect.holds<dex::SideCondition>()) {
    handle.emplace<SideConditionName>(effect.get<dex::SideCondition>());
  }
  else if (effect.holds<dex::Status>()) {
    handle.emplace<StatusName>(effect.get<dex::Status>());
  }
  else if (effect.holds<dex::Terrain>()) {
    handle.emplace<TerrainName>(effect.get<dex::Terrain>());
  }
  else if (effect.holds<dex::Volatile>()) {
    handle.emplace<VolatileName>(effect.get<dex::Volatile>());
  }
  else if (effect.holds<dex::Weather>()) {
    handle.emplace<WeatherName>(effect.get<dex::Weather>());
  }
  else {
    ENTT_FAIL("Effect does not contain a valid enum");
  }
}

void InputSetup::setBattle(types::entity entity) {
  handle.emplace<Battle>(entity);
}
}  // namespace pokesim::analyze_effect

////////// END OF src/AnalyzeEffect/Setup/AnalyzeEffectInputSetup.cpp //////////

/////////// START OF src/Components/AnalyzeEffect/RemovedEffect.hpp ////////////

namespace pokesim::analyze_effect {
struct RemovedEffect {
  types::effectEnum val{};
};
}  // namespace pokesim::analyze_effect

//////////// END OF src/Components/AnalyzeEffect/RemovedEffect.hpp /////////////

///////////////// START OF src/AnalyzeEffect/AnalyzeEffect.cpp /////////////////

namespace pokesim::analyze_effect {
void run(Simulation& simulation) {
  assignAttackerDefender(simulation);

  if (simulation.analyzeEffectOptions.reconsiderActiveEffects) {
    removeConsideredEffect(simulation);
  }

  createAppliedEffectBattles(simulation);

  calc_damage::run(simulation);

  if (simulation.analyzeEffectOptions.reconsiderActiveEffects) {
    reapplyConsideredEffect(simulation);
  }

  postRunCleanup(simulation);
}

void assignAttackerDefender(Simulation& /*simulation*/) {}

void createAppliedEffectBattles(Simulation& /*simulation*/) {}

void removeConsideredEffect(Simulation& /*simulation*/) {}

void reapplyConsideredEffect(Simulation& /*simulation*/) {}

void postRunCleanup(Simulation& /*simulation*/) {
  // Remove duplicate states and attacker/defender components
}
}  // namespace pokesim::analyze_effect

////////////////// END OF src/AnalyzeEffect/AnalyzeEffect.cpp //////////////////

///////////////// START OF src/Components/Names/StatNames.hpp //////////////////

namespace pokesim {
struct StatName {
  dex::Stat name;
};
}  // namespace pokesim

////////////////// END OF src/Components/Names/StatNames.hpp ///////////////////

////////////////// START OF src/Components/Tags/TypeTags.hpp ///////////////////

// TODO(aed3): Make this auto generated

namespace pokesim::type::tags {
struct Normal {};
struct Fighting {};
struct Flying {};
struct Poison {};
struct Ground {};
struct Rock {};
struct Bug {};
struct Ghost {};
struct Steel {};
struct Fire {};
struct Water {};
struct Grass {};
struct Electric {};
struct Psychic {};
struct Ice {};
struct Dragon {};
struct Dark {};
struct Fairy {};
}  // namespace pokesim::type::tags

/////////////////// END OF src/Components/Tags/TypeTags.hpp ////////////////////

//////////////////////// START OF src/Pokedex/Names.hpp ////////////////////////

#include <string>

// TODO(aed3): Change the move names to be part of the build functions so they aren't all loaded all the time

namespace pokesim::dex {
// Returns the name of a species represented by its enum as a string.
inline std::string toString(Species speciesEnum);

// Returns the name of a move represented by its enum as a string.
inline std::string toString(Move moveEnum);

// Returns the name of an ability represented by its enum as a string.
inline std::string toString(Ability abilityEnum);

// Returns the name of an item represented by its enum as a string.
inline std::string toString(Item itemEnum);

// Returns the name of a nature represented by its enum as a string.
inline std::string toString(Nature natureEnum);

// Returns the name of a status condition represented by its enum as a string.
inline std::string toString(Status statusEnum);

// Returns the name of a gender represented by its enum as a string.
inline std::string toString(Gender genderEnum);

// Returns the name of a type represented by its enum as a string.
inline std::string toString(Type typeEnum);

// Returns the name of a stat represented by its enum as a string.
inline std::string toString(Stat statEnum);

// Returns the Smogon name of a species represented by its enum as a string.
inline std::string toSmogonString(Species speciesEnum);

// Returns the Smogon name of a move represented by its enum as a string.
inline std::string toSmogonString(Move moveEnum);

// Returns the Smogon name of an ability represented by its enum as a string.
inline std::string toSmogonString(Ability abilityEnum);

// Returns the Smogon name of an item represented by its enum as a string.
inline std::string toSmogonString(Item itemEnum);

// Returns the Smogon ID of a species represented by its enum as a string.
inline std::string toSmogonIdString(Species speciesEnum);

// Returns the Smogon ID of a move represented by its enum as a string.
inline std::string toSmogonIdString(Move moveEnum);

// Returns the Smogon ID of an ability represented by its enum as a string.
inline std::string toSmogonIdString(Ability abilityEnum);

// Returns the Smogon ID of an item represented by its enum as a string.
inline std::string toSmogonIdString(Item itemEnum);

/**
 * @brief Returns the enum of the species name string.
 * Returns `MISSING_NO` if the string does not represent a species.
 */
inline Species fromNameToSpeciesEnum(const std::string& name);

/**
 * @brief Returns the enum of the move name string.
 * Returns `NO_MOVE` if the string does not represent a move.
 */
inline Move fromNameToMoveEnum(const std::string& name);

/**
 * @brief Returns the enum of the ability name string.
 * Returns `NO_ABILITY` if the string does not represent an ability.
 */
inline Ability fromNameToAbilityEnum(const std::string& name);

/**
 * @brief Returns the enum of the item name string.
 * Returns `NO_ITEM` if the string does not represent an item.
 */
inline Item fromNameToItemEnum(const std::string& name);

/**
 * @brief Returns the enum of the nature name string.
 * Returns `NO_NATURE` if the string does not represent a nature.
 */
inline Nature fromNameToNatureEnum(const std::string& name);

/**
 * @brief Returns the enum of the status condition name string.
 * Returns `NO_STATUS` if the string does not represent a status condition.
 */
inline Status fromNameToStatusEnum(const std::string& name);

/**
 * @brief Returns the enum of the gender name string.
 * Returns `NO_GENDER` if the string does not represent a gender.
 */
inline Gender fromNameToGenderEnum(const std::string& name);

/**
 * @brief Returns the enum of the type name string.
 * Returns `NO_TYPE` if the string does not represent a type.
 */
inline Type fromNameToTypeEnum(const std::string& name);

/**
 * @brief Returns `NO_STAT` if the string does not represent a stat.
 */
inline Stat fromNameToStatEnum(const std::string& name);

/**
 * @brief Returns the enum of the species Smogon name string.
 * Returns `MISSING_NO` if the string does not represent a species.
 */
inline Species fromSmogonNameToSpeciesEnum(const std::string& name);

/**
 * @brief Returns the enum of the move Smogon name string.
 * Returns `NO_MOVE` if the string does not represent a move.
 */
inline Move fromSmogonNameToMoveEnum(const std::string& name);

/**
 * @brief Returns the enum of the ability Smogon name string.
 * Returns `NO_ABILITY` if the string does not represent an ability.
 */
inline Ability fromSmogonNameToAbilityEnum(const std::string& name);

/**
 * @brief Returns the enum of the item Smogon name string.
 * Returns `NO_ITEM` if the string does not represent an item.
 */
inline Item fromSmogonNameToItemEnum(const std::string& name);

/**
 * @brief Returns the enum of the species Smogon ID string.
 * Returns `MISSING_NO` if the string does not represent a species.
 */
inline Species fromSmogonIDToSpeciesEnum(const std::string& name);

/**
 * @brief Returns the enum of the move Smogon ID string.
 * Returns `NO_MOVE` if the string does not represent a move.
 */
inline Move fromSmogonIDToMoveEnum(const std::string& name);

/**
 * @brief Returns the enum of the ability Smogon ID string.
 * Returns `NO_ABILITY` if the string does not represent an ability.
 */
inline Ability fromSmogonIDToAbilityEnum(const std::string& name);

/**
 * @brief Returns the enum of the item Smogon ID string.
 * Returns `NO_ITEM` if the string does not represent an item.
 */
inline Item fromSmogonIDToItemEnum(const std::string& name);

// Converts a string to only have lowercase alphanumeric characters
inline std::string toID(const std::string& name);
}  // namespace pokesim::dex

///////////////////////// END OF src/Pokedex/Names.hpp /////////////////////////

/////////////////////////// START OF src/PokeSim.hpp ///////////////////////////


//////////////////////////// END OF src/PokeSim.hpp ////////////////////////////