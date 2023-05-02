#pragma once

/**
 * FILE ORDER
 * external/entt/core/iterator.hpp
 * external/entt/config/macro.h
 * external/entt/config/version.h
 * external/entt/config/config.h
 * external/entt/core/fwd.hpp
 * external/entt/core/type_traits.hpp
 * external/entt/entity/fwd.hpp
 * external/entt/entity/entity.hpp
 * external/entt/entity/handle.hpp
 * src/Types/Ability.hpp
 * src/Components/Names/Ability.hpp
 * src/Components/DexData/Abilities.hpp
 * src/Components/DexData/BaseStats.hpp
 * src/Types/Type.hpp
 * src/Components/Names/Type.hpp
 * src/Components/DexData/SpeciesTypes.hpp
 * src/Types/Species.hpp
 * src/Components/Names/Species.hpp
 * external/entt/container/fwd.hpp
 * external/entt/core/compressed_pair.hpp
 * external/entt/core/memory.hpp
 * external/entt/container/dense_map.hpp
 * external/entt/core/utility.hpp
 * external/entt/core/algorithm.hpp
 * external/entt/core/attribute.h
 * external/entt/core/hashed_string.hpp
 * external/entt/core/type_info.hpp
 * external/entt/core/any.hpp
 * external/entt/entity/component.hpp
 * external/entt/entity/sparse_set.hpp
 * external/entt/signal/fwd.hpp
 * external/entt/signal/delegate.hpp
 * external/entt/signal/sigh.hpp
 * external/entt/entity/storage_mixin.hpp
 * external/entt/entity/storage.hpp
 * external/entt/entity/group.hpp
 * external/entt/entity/view.hpp
 * external/entt/entity/registry.hpp
 * src/Types/GameMechanic.hpp
 * src/Types/Gender.hpp
 * src/Types/Item.hpp
 * src/Types/Move.hpp
 * src/Types/Nature.hpp
 * src/Types/Stat.hpp
 * src/Types/Status.hpp
 * src/Dex/Dex.hpp
 * src/Dex/Setup/Setup.hpp
 * src/Dex/Setup/SpeciesSetup.hpp
 * src/Dex/Setup/SpeciesSetup.cpp
 * src/Components/Accuracy.hpp
 * src/Components/BasePower.hpp
 * src/Components/Boosts.hpp
 * src/Components/Chance.hpp
 * src/Components/EntityHolders/MoveEffect.hpp
 * src/Components/MultiHit.hpp
 * src/Components/Names/Move.hpp
 * src/Components/PP.hpp
 * src/Components/Priority.hpp
 * src/Components/Tags/Move.hpp
 * src/Dex/Setup/MoveSetup.hpp
 * src/Dex/Setup/MoveSetup.cpp
 * src/Components/Names/Item.hpp
 * src/Dex/Setup/ItemSetup.hpp
 * src/Dex/Setup/ItemSetup.cpp
 * src/Dex/Species/Ampharos.hpp
 * src/Dex/Species/Dragapult.hpp
 * src/Dex/Species/Empoleon.hpp
 * src/Dex/Species/Gardevoir.hpp
 * src/Dex/Species/Pangoro.hpp
 * src/Dex/Species/Ribombee.hpp
 * src/Dex/Setup/GetSpeciesBuild.cpp
 * src/Dex/Items/AssaultVest.hpp
 * src/Dex/Items/BrightPowder.hpp
 * src/Dex/Items/ChoiceScarf.hpp
 * src/Dex/Items/ChoiceSpecs.hpp
 * src/Dex/Items/FocusSash.hpp
 * src/Dex/Items/LifeOrb.hpp
 * src/Dex/Setup/GetItemBuild.cpp
 * src/Dex/Names.hpp
 * src/Dex/Names.cpp
 * external/entt/container/dense_set.hpp
 * src/Dex/Dex.cpp
 * src/Components/EVsIVs.hpp
 * src/Components/EntityHolders/ActionQueue.hpp
 * src/Components/EntityHolders/Battle.hpp
 * src/Components/EntityHolders/FoeSide.hpp
 * src/Components/EntityHolders/Move.hpp
 * src/Components/EntityHolders/MoveSlots.hpp
 * src/Components/EntityHolders/Pokemon.hpp
 * src/Components/EntityHolders/Side.hpp
 * src/Components/EntityHolders/Sides.hpp
 * src/Components/EntityHolders/Team.hpp
 * src/Components/ID.hpp
 * src/Components/Level.hpp
 * src/Components/Names/Gender.hpp
 * src/Components/Names/Nature.hpp
 * src/Components/Names/Stat.hpp
 * src/Components/Names/Status.hpp
 * src/Components/Position.hpp
 * src/Components/Probability.hpp
 * src/Components/RNGSeed.hpp
 * src/Components/SpeedSort.hpp
 * src/Components/Stats.hpp
 * src/Components/Tags/Ability.hpp
 * src/Components/Tags/Actions.hpp
 * src/Components/Tags/Battle.hpp
 * src/Components/Tags/Item.hpp
 * src/Components/Tags/Pokemon.hpp
 * src/Components/Tags/Status.hpp
 * src/Components/Tags/Type.hpp
 * src/Components/TargetSlot.hpp
 * src/Components/Turn.hpp
 * src/PokeSim.hpp
 */


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
#define ENTT_VERSION_MINOR 11
#define ENTT_VERSION_PATCH 1

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
 * @tparam The size of the type if `sizeof` accepts it, 0 otherwise.
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
struct type_list_contains<type_list<Type...>, Other>: std::disjunction<std::is_same<Type, Other>...> {};

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
    /*! @brief Searched value. */
    static constexpr auto value = Value;
};

/**
 * @brief Helper type.
 * @tparam Index Index of the value to return.
 * @tparam List Value list to search into.
 */
template<std::size_t Index, typename List>
inline constexpr auto value_list_element_v = value_list_element<Index, List>::value;

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
    : std::conjunction<std::is_empty<Type>, std::negation<std::is_final<Type>>> {};

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

#endif

////////////////// END OF external/entt/core/type_traits.hpp ///////////////////

//////////////////// START OF external/entt/entity/fwd.hpp /////////////////////

#ifndef ENTT_ENTITY_FWD_HPP
#define ENTT_ENTITY_FWD_HPP

#include <memory>
#include <type_traits>

namespace entt {

/*! @brief Default entity identifier. */
enum class entity : id_type {};

template<typename Entity = entity, typename = std::allocator<Entity>>
class basic_sparse_set;

template<typename Type, typename = entity, typename = std::allocator<Type>, typename = void>
class basic_storage;

template<typename Type>
class sigh_storage_mixin;

/**
 * @brief Provides a common way to define storage types.
 * @tparam Type Storage value type.
 * @tparam Entity A valid entity type (see entt_traits for more details).
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Type, typename Entity = entity, typename Allocator = std::allocator<Type>, typename = void>
struct storage_type {
    /*! @brief Type-to-storage conversion result. */
    using type = sigh_storage_mixin<basic_storage<Type, Entity, Allocator>>;
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
 * @tparam Entity A valid entity type (see entt_traits for more details).
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

template<typename>
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
using exclude_t = type_list<Type...>;

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
using get_t = type_list<Type...>;

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
using owned_t = type_list<Type...>;

/**
 * @brief Variable template for lists of owned components.
 * @tparam Type List of types.
 */
template<typename... Type>
inline constexpr owned_t<Type...> owned{};

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

template<typename, typename = void>
struct entt_traits;

template<typename Type>
struct entt_traits<Type, std::enable_if_t<std::is_enum_v<Type>>>
    : entt_traits<std::underlying_type_t<Type>> {};

template<typename Type>
struct entt_traits<Type, std::enable_if_t<std::is_class_v<Type>>>
    : entt_traits<typename Type::entity_type> {};

template<>
struct entt_traits<std::uint32_t> {
    using entity_type = std::uint32_t;
    using version_type = std::uint16_t;

    static constexpr entity_type entity_mask = 0xFFFFF;
    static constexpr entity_type version_mask = 0xFFF;
    static constexpr std::size_t entity_shift = 20u;
};

template<>
struct entt_traits<std::uint64_t> {
    using entity_type = std::uint64_t;
    using version_type = std::uint32_t;

    static constexpr entity_type entity_mask = 0xFFFFFFFF;
    static constexpr entity_type version_mask = 0xFFFFFFFF;
    static constexpr std::size_t entity_shift = 32u;
};

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Entity traits.
 * @tparam Type Type of identifier.
 */
template<typename Type>
class entt_traits: internal::entt_traits<Type> {
    using base_type = internal::entt_traits<Type>;

public:
    /*! @brief Value type. */
    using value_type = Type;
    /*! @brief Underlying entity type. */
    using entity_type = typename base_type::entity_type;
    /*! @brief Underlying version type. */
    using version_type = typename base_type::version_type;
    /*! @brief Reserved identifier. */
    static constexpr entity_type reserved = base_type::entity_mask | (base_type::version_mask << base_type::entity_shift);
    /*! @brief Page size, default is `ENTT_SPARSE_PAGE`. */
    static constexpr auto page_size = ENTT_SPARSE_PAGE;

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
        return (to_integral(value) & base_type::entity_mask);
    }

    /**
     * @brief Returns the version part once converted to the underlying type.
     * @param value The value to convert.
     * @return The integral representation of the version part.
     */
    [[nodiscard]] static constexpr version_type to_version(const value_type value) noexcept {
        return (to_integral(value) >> base_type::entity_shift);
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
        return value_type{(entity & base_type::entity_mask) | (static_cast<entity_type>(version) << base_type::entity_shift)};
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
        constexpr auto mask = (base_type::version_mask << base_type::entity_shift);
        return value_type{(lhs & base_type::entity_mask) | (rhs & mask)};
    }
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
        using entity_traits = entt_traits<Entity>;
        return entity_traits::combine(entity_traits::reserved, entity_traits::reserved);
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
        using entity_traits = entt_traits<Entity>;
        return entity_traits::to_entity(entity) == entity_traits::to_entity(*this);
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
        using entity_traits = entt_traits<Entity>;
        return entity_traits::combine(entity_traits::reserved, entity_traits::reserved);
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
        using entity_traits = entt_traits<Entity>;
        return entity_traits::to_version(entity) == entity_traits::to_version(*this);
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
        while(it != last && !it->second.contains(entt)) { ++it; }
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
     * @tparam Other A valid entity type (see entt_traits for more details).
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
        reg->destroy(entt);
    }

    /**
     * @brief Destroys the entity associated with a handle.
     * @param version A desired version upon destruction.
     */
    void destroy(const version_type version) {
        reg->destroy(entt, version);
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

//////////////////////// START OF src/Types/Ability.hpp ////////////////////////

#include <cstdint>

namespace pokesim::dex {
/** @brief Pokemon ability name */
enum Ability : std::uint16_t {
  // clang-format off
  NO_ABILITY = 0, ADAPTABILITY, AERILATE, AFTERMATH, AIR_LOCK, ANALYTIC, ANGER_POINT, ANGER_SHELL, ANTICIPATION, ARENA_TRAP, ARMOR_TAIL, AROMA_VEIL, AS_ONE, AURA_BREAK, BAD_DREAMS, BALL_FETCH, BATTERY, BATTLE_ARMOR, BATTLE_BOND, BEADS_OF_RUIN, BEAST_BOOST, BERSERK, BIG_PECKS, BLAZE, BULLETPROOF, CHEEK_POUCH, CHILLING_NEIGH, CHLOROPHYLL, CLEAR_BODY, CLOUD_NINE, COLOR_CHANGE, COMATOSE, COMMANDER, COMPETITIVE, COMPOUND_EYES, CONTRARY, CORROSION, COSTAR, COTTON_DOWN, CUD_CHEW, CURIOUS_MEDICINE, CURSED_BODY, CUTE_CHARM, DAMP, DANCER, DARK_AURA, DAUNTLESS_SHIELD, DAZZLING, DEFEATIST, DEFIANT, DELTA_STREAM, DESOLATE_LAND, DISGUISE, DOWNLOAD, DRAGONS_MAW, DRIZZLE, DROUGHT, DRY_SKIN, EARTH_EATER, EARLY_BIRD, EFFECT_SPORE, ELECTRIC_SURGE, ELECTROMORPHOSIS, EMERGENCY_EXIT, FAIRY_AURA, FILTER, FLAME_BODY, FLARE_BOOST, FLASH_FIRE, FLOWER_GIFT, FLOWER_VEIL, FLUFFY, FORECAST, FOREWARN, FRIEND_GUARD, FRISK, FULL_METAL_BODY, FUR_COAT, GALE_WINGS, GALVANIZE, GLUTTONY, GOOD_AS_GOLD, GOOEY, GORILLA_TACTICS, GRASS_PELT, GRASSY_SURGE, GRIM_NEIGH, GUARD_DOG, GULP_MISSILE, GUTS, HADRON_ENGINE, HARVEST, HEALER, HEATPROOF, HEAVY_METAL, HONEY_GATHER, HUGE_POWER, HUNGER_SWITCH, HUSTLE, HYDRATION, HYPER_CUTTER, ICE_BODY, ICE_FACE, ICE_SCALES, ILLUMINATE, ILLUSION, IMMUNITY, IMPOSTER, INFILTRATOR, INNARDS_OUT, INNER_FOCUS, INSOMNIA, INTIMIDATE, INTREPID_SWORD, IRON_BARBS, IRON_FIST, JUSTIFIED, KEEN_EYE, KLUTZ, LEAF_GUARD, LEVITATE, LIBERO, LIGHT_METAL, LIGHTNING_ROD, LINGERING_AROMA, LIMBER, LIQUID_OOZE, LIQUID_VOICE, LONG_REACH, MAGIC_BOUNCE, MAGIC_GUARD, MAGICIAN, MAGMA_ARMOR, MAGNET_PULL, MARVEL_SCALE, MEGA_LAUNCHER, MERCILESS, MIMICRY, MINUS, MIRROR_ARMOR, MISTY_SURGE, MOLD_BREAKER, MOODY, MOTOR_DRIVE, MOXIE, MULTISCALE, MULTITYPE, MUMMY, MYCELIUM_MIGHT, NATURAL_CURE, NEUROFORCE, NEUTRALIZING_GAS, NO_GUARD, NORMALIZE, OBLIVIOUS, OPPORTUNIST, ORICHALCUM_PULSE, OVERCOAT, OVERGROW, OWN_TEMPO, PARENTAL_BOND, PASTEL_VEIL, PERISH_BODY, PICKPOCKET, PICKUP, PIXILATE, PLUS, POISON_HEAL, POISON_POINT, POISON_TOUCH, POWER_CONSTRUCT, POWER_OF_ALCHEMY, POWER_SPOT, PRANKSTER, PRESSURE, PRIMORDIAL_SEA, PRISM_ARMOR, PROPELLER_TAIL, PROTEAN, PROTOSYNTHESIS, PSYCHIC_SURGE, PUNK_ROCK, PURE_POWER, PURIFYING_SALT, QUARK_DRIVE, QUEENLY_MAJESTY, QUICK_DRAW, QUICK_FEET, RAIN_DISH, RATTLED, RECEIVER, RECKLESS, REFRIGERATE, REGENERATOR, RIPEN, RIVALRY, RKS_SYSTEM, ROCK_HEAD, ROCKY_PAYLOAD, ROUGH_SKIN, RUN_AWAY, SAND_FORCE, SAND_RUSH, SAND_SPIT, SAND_STREAM, SAND_VEIL, SAP_SIPPER, SCHOOLING, SCRAPPY, SCREEN_CLEANER, SEED_SOWER, SERENE_GRACE, SHADOW_SHIELD, SHADOW_TAG, SHARPNESS, SHED_SKIN, SHEER_FORCE, SHELL_ARMOR, SHIELD_DUST, SHIELDS_DOWN, SIMPLE, SKILL_LINK, SLOW_START, SLUSH_RUSH, SNIPER, SNOW_CLOAK, SNOW_WARNING, SOLAR_POWER, SOLID_ROCK, SOUL_HEART, SOUNDPROOF, SPEED_BOOST, STAKEOUT, STALL, STALWART, STAMINA, STANCE_CHANGE, STATIC, STEADFAST, STEAM_ENGINE, STEELWORKER, STEELY_SPIRIT, STENCH, STICKY_HOLD, STORM_DRAIN, STRONG_JAW, STURDY, SUCTION_CUPS, SUPER_LUCK, SUPREME_OVERLORD, SURGE_SURFER, SWARM, SWEET_VEIL, SWIFT_SWIM, SYMBIOSIS, SYNCHRONIZE, SWORD_OF_RUIN, TABLETS_OF_RUIN, TANGLED_FEET, TANGLING_HAIR, TECHNICIAN, TELEPATHY, TERAVOLT, THERMAL_EXCHANGE, THICK_FAT, TINTED_LENS, TORRENT, TOUGH_CLAWS, TOXIC_BOOST, TOXIC_DEBRIS, TRACE, TRANSISTOR, TRIAGE, TRUANT, TURBOBLAZE, UNAWARE, UNBURDEN, UNNERVE, UNSEEN_FIST, VESSEL_OF_RUIN, VICTORY_STAR, VITAL_SPIRIT, VOLT_ABSORB, WANDERING_SPIRIT, WATER_ABSORB, WATER_BUBBLE, WATER_COMPACTION, WATER_VEIL, WEAK_ARMOR, WELL_BAKED_BODY, WHITE_SMOKE, WIMP_OUT, WIND_POWER, WIND_RIDER, WONDER_GUARD, WONDER_SKIN, ZEN_MODE, ZERO_TO_HERO, ABILITY_TOTAL,
  // clang-format on
};
}  // namespace pokesim::dex

///////////////////////// END OF src/Types/Ability.hpp /////////////////////////

////////////////// START OF src/Components/Names/Ability.hpp ///////////////////

namespace pokesim {
struct AbilityName {
  dex::Ability name = dex::NO_ABILITY;
};
}  // namespace pokesim

/////////////////// END OF src/Components/Names/Ability.hpp ////////////////////

//////////////// START OF src/Components/DexData/Abilities.hpp /////////////////

namespace pokesim {
struct PrimaryAbility {
  dex::Ability ability = dex::NO_ABILITY;
};

struct SecondaryAbility {
  dex::Ability ability = dex::NO_ABILITY;
};

struct HiddenAbility {
  dex::Ability ability = dex::NO_ABILITY;
};
}  // namespace pokesim

///////////////// END OF src/Components/DexData/Abilities.hpp //////////////////

//////////////// START OF src/Components/DexData/BaseStats.hpp /////////////////

#include <cstdint>

namespace pokesim {
struct BaseStats {
  std::uint8_t hp;
  std::uint8_t atk;
  std::uint8_t def;
  std::uint8_t spa;
  std::uint8_t spd;
  std::uint8_t spe;
};
}  // namespace pokesim

///////////////// END OF src/Components/DexData/BaseStats.hpp //////////////////

///////////////////////// START OF src/Types/Type.hpp //////////////////////////

#include <cstdint>

namespace pokesim::dex {
/** @brief Pokemon type name */
enum Type : std::uint8_t {
  // clang-format off
  NO_TYPE = 0, NORMAL_TYPE, FIGHTING_TYPE, FLYING_TYPE, POISON_TYPE, GROUND_TYPE, ROCK_TYPE, BUG_TYPE, GHOST_TYPE, STEEL_TYPE, FIRE_TYPE, WATER_TYPE, GRASS_TYPE, ELECTRIC_TYPE, PSYCHIC_TYPE, ICE_TYPE, DRAGON_TYPE, DARK_TYPE, FAIRY_TYPE, TYPE_TOTAL
  // clang-format on
};
}  // namespace pokesim::dex

////////////////////////// END OF src/Types/Type.hpp ///////////////////////////

//////////////////// START OF src/Components/Names/Type.hpp ////////////////////

namespace pokesim {
struct TypeName {
  dex::Type name = dex::NO_TYPE;
};
}  // namespace pokesim

///////////////////// END OF src/Components/Names/Type.hpp /////////////////////

/////////////// START OF src/Components/DexData/SpeciesTypes.hpp ///////////////

namespace pokesim {
struct SpeciesTypes {
  dex::Type t1 = dex::NO_TYPE;
  dex::Type t2 = dex::NO_TYPE;
};
}  // namespace pokesim

//////////////// END OF src/Components/DexData/SpeciesTypes.hpp ////////////////

//////////////////////// START OF src/Types/Species.hpp ////////////////////////

#include <cstdint>

namespace pokesim::dex {

/**
 * @brief Pokemon and Pokemon form name
 *
 * @details Pokemon that have multiple forms will have their base form and alternate forms listed here.
 * However, if none of a Pokemon's forms are cosmetic (i.e. change nothing expect appearance), the forms cannot be
 * changed during battle, and no true base form exists, then the Pokemon's species name without a form specifier is
 * omitted. For example:
 *  - `VENUSAUR`, `MEGA_VENUSAUR`, and `GIGANTAMAX_VENUSAUR` are all listed because Venusaur changes into the other
 * forms mid battle
 *  - `GASTRODON`, `WEST_SEA_GASTRODON`, and `EAST_SEA_GASTRODON` are all listed because although Gastrodon's forms
 * are permanent, their only difference is how they look
 *  - `PLANT_CLOAK_WORMADAM`, `SANDY_CLOAK_WORMADAM`, and `TRASH_CLOAK_WORMADAM` listed while `WORMADAM` is not
 * because the Wormadam forms have different types, stats, and moves; their forms are permanent; and there is no base
 * Wormadam
 */
enum Species : std::uint16_t {
  // clang-format off
  MISSING_NO = 0, BULBASAUR, IVYSAUR, VENUSAUR, MEGA_VENUSAUR, GIGANTAMAX_VENUSAUR, CHARMANDER, CHARMELEON, CHARIZARD, MEGA_CHARIZARD_X, MEGA_CHARIZARD_Y, GIGANTAMAX_CHARIZARD, SQUIRTLE, WARTORTLE, BLASTOISE, MEGA_BLASTOISE, GIGANTAMAX_BLASTOISE, CATERPIE, METAPOD, BUTTERFREE, GIGANTAMAX_BUTTERFREE, WEEDLE, KAKUNA, BEEDRILL, MEGA_BEEDRILL, PIDGEY, PIDGEOTTO, PIDGEOT, MEGA_PIDGEOT, RATTATA, ALOLAN_RATTATA, RATICATE, ALOLAN_RATICATE, TOTEM_ALOLAN_RATICATE, SPEAROW, FEAROW, EKANS, ARBOK, PIKACHU, COSPLAY_PIKACHU, PIKACHU_ROCK_STAR, PIKACHU_BELLE, PIKACHU_POP_STAR, PIKACHU_PHD, PIKACHU_LIBRE, ORIGINAL_CAP_PIKACHU, HOENN_CAP_PIKACHU, SINNOH_CAP_PIKACHU, UNOVA_CAP_PIKACHU, KALOS_CAP_PIKACHU, ALOLA_CAP_PIKACHU, PARTNER_CAP_PIKACHU, STARTER_PIKACHU, GIGANTAMAX_PIKACHU, WORLD_CAP_PIKACHU, RAICHU, ALOLAN_RAICHU, SANDSHREW, ALOLAN_SANDSHREW, SANDSLASH, ALOLAN_SANDSLASH, NIDORAN_FEMALE, NIDORINA, NIDOQUEEN, NIDORAN_MALE, NIDORINO, NIDOKING, CLEFAIRY, CLEFABLE, VULPIX, ALOLAN_VULPIX, NINETALES, ALOLAN_NINETALES, JIGGLYPUFF, WIGGLYTUFF, ZUBAT, GOLBAT, ODDISH, GLOOM, VILEPLUME, PARAS, PARASECT, VENONAT, VENOMOTH, DIGLETT, ALOLAN_DIGLETT, DUGTRIO, ALOLAN_DUGTRIO, MEOWTH, ALOLAN_MEOWTH, GALARIAN_MEOWTH, GIGANTAMAX_MEOWTH, PERSIAN, ALOLAN_PERSIAN, PSYDUCK, GOLDUCK, MANKEY, PRIMEAPE, HISUIAN_GROWLITHE, GROWLITHE, HISUIAN_ARCANINE, ARCANINE, POLIWAG, POLIWHIRL, POLIWRATH, ABRA, KADABRA, ALAKAZAM, MEGA_ALAKAZAM, MACHOP, MACHOKE, MACHAMP, GIGANTAMAX_MACHAMP, BELLSPROUT, WEEPINBELL, VICTREEBEL, TENTACOOL, TENTACRUEL, GEODUDE, ALOLAN_GEODUDE, GRAVELER, ALOLAN_GRAVELER, GOLEM, ALOLAN_GOLEM, PONYTA, GALARIAN_PONYTA, RAPIDASH, GALARIAN_RAPIDASH, SLOWPOKE, GALARIAN_SLOWPOKE, SLOWBRO, MEGA_SLOWBRO, GALARIAN_SLOWBRO, MAGNEMITE, MAGNETON, FARFETCH_D, GALARIAN_FARFETCH_D, DODUO, DODRIO, SEEL, DEWGONG, GRIMER, ALOLAN_GRIMER, MUK, ALOLAN_MUK, SHELLDER, CLOYSTER, GASTLY, HAUNTER, GENGAR, MEGA_GENGAR, GIGANTAMAX_GENGAR, ONIX, DROWZEE, HYPNO, KRABBY, KINGLER, GIGANTAMAX_KINGLER, VOLTORB, HISUIAN_VOLTORB, ELECTRODE, HISUIAN_ELECTRODE, EXEGGCUTE, EXEGGUTOR, ALOLAN_EXEGGUTOR, CUBONE, MAROWAK, ALOLAN_MAROWAK, TOTEM_ALOLAN_MAROWAK, HITMONLEE, HITMONCHAN, LICKITUNG, KOFFING, WEEZING, GALARIAN_WEEZING, RHYHORN, RHYDON, CHANSEY, TANGELA, KANGASKHAN, MEGA_KANGASKHAN, HORSEA, SEADRA, GOLDEEN, SEAKING, STARYU, STARMIE, MR_MIME, GALARIAN_MR_MIME, SCYTHER, JYNX, ELECTABUZZ, MAGMAR, PINSIR, MEGA_PINSIR, TAUROS, PALDEAN_TAUROS_COMBAT_BREAD, PALDEAN_TAUROS_BLAZE_BREAD, PALDEAN_TAUROS_AQUA_BREAD, MAGIKARP, GYARADOS, MEGA_GYARADOS, LAPRAS, GIGANTAMAX_LAPRAS, DITTO, EEVEE, STARTER_EEVEE, GIGANTAMAX_EEVEE, VAPOREON, JOLTEON, FLAREON, PORYGON, OMANYTE, OMASTAR, KABUTO, KABUTOPS, AERODACTYL, MEGA_AERODACTYL, SNORLAX, GIGANTAMAX_SNORLAX, ARTICUNO, GALARIAN_ARTICUNO, ZAPDOS, GALARIAN_ZAPDOS, MOLTRES, GALARIAN_MOLTRES, DRATINI, DRAGONAIR, DRAGONITE, MEWTWO, MEGA_MEWTWO_X, MEGA_MEWTWO_Y, MEW, CHIKORITA, BAYLEEF, MEGANIUM, CYNDAQUIL, QUILAVA, HISUIAN_TYPHLOSION, TYPHLOSION, TOTODILE, CROCONAW, FERALIGATR, SENTRET, FURRET, HOOTHOOT, NOCTOWL, LEDYBA, LEDIAN, SPINARAK, ARIADOS, CROBAT, CHINCHOU, LANTURN, PICHU, SPIKY_EARED_PICHU, CLEFFA, IGGLYBUFF, TOGEPI, TOGETIC, NATU, XATU, MAREEP, FLAAFFY, AMPHAROS, MEGA_AMPHAROS, BELLOSSOM, MARILL, AZUMARILL, SUDOWOODO, POLITOED, HOPPIP, SKIPLOOM, JUMPLUFF, AIPOM, SUNKERN, SUNFLORA, YANMA, WOOPER, PALDEAN_WOOPER, QUAGSIRE, ESPEON, UMBREON, MURKROW, SLOWKING, GALARIAN_SLOWKING, MISDREAVUS, UNOWN, UNOWN_A, UNOWN_B, UNOWN_C, UNOWN_D, UNOWN_E, UNOWN_F, UNOWN_G, UNOWN_H, UNOWN_I, UNOWN_J, UNOWN_K, UNOWN_L, UNOWN_M, UNOWN_N, UNOWN_O, UNOWN_P, UNOWN_Q, UNOWN_R, UNOWN_S, UNOWN_T, UNOWN_U, UNOWN_V, UNOWN_W, UNOWN_X, UNOWN_Y, UNOWN_Z, UNOWN_EXCLAMATION, UNOWN_QUESTION, WOBBUFFET, GIRAFARIG, PINECO, FORRETRESS, DUNSPARCE, GLIGAR, STEELIX, MEGA_STEELIX, SNUBBULL, GRANBULL, QWILFISH, HISUIAN_QWILFISH, SCIZOR, MEGA_SCIZOR, SHUCKLE, HERACROSS, MEGA_HERACROSS, HISUIAN_SNEASEL, SNEASEL, TEDDIURSA, URSARING, SLUGMA, MAGCARGO, SWINUB, PILOSWINE, CORSOLA, GALARIAN_CORSOLA, REMORAID, OCTILLERY, DELIBIRD, MANTINE, SKARMORY, HOUNDOUR, HOUNDOOM, MEGA_HOUNDOOM, KINGDRA, PHANPY, DONPHAN, PORYGON2, STANTLER, SMEARGLE, TYROGUE, HITMONTOP, SMOOCHUM, ELEKID, MAGBY, MILTANK, BLISSEY, RAIKOU, ENTEI, SUICUNE, LARVITAR, PUPITAR, TYRANITAR, MEGA_TYRANITAR, LUGIA, HO_OH, CELEBI, TREECKO, GROVYLE, SCEPTILE, MEGA_SCEPTILE, TORCHIC, COMBUSKEN, BLAZIKEN, MEGA_BLAZIKEN, MUDKIP, MARSHTOMP, SWAMPERT, MEGA_SWAMPERT, POOCHYENA, MIGHTYENA, ZIGZAGOON, GALARIAN_ZIGZAGOON, LINOONE, GALARIAN_LINOONE, WURMPLE, SILCOON, BEAUTIFLY, CASCOON, DUSTOX, LOTAD, LOMBRE, LUDICOLO, SEEDOT, NUZLEAF, SHIFTRY, TAILLOW, SWELLOW, WINGULL, PELIPPER, RALTS, KIRLIA, GARDEVOIR, MEGA_GARDEVOIR, SURSKIT, MASQUERAIN, SHROOMISH, BRELOOM, SLAKOTH, VIGOROTH, SLAKING, NINCADA, NINJASK, SHEDINJA, WHISMUR, LOUDRED, EXPLOUD, MAKUHITA, HARIYAMA, AZURILL, NOSEPASS, SKITTY, DELCATTY, SABLEYE, MEGA_SABLEYE, MAWILE, MEGA_MAWILE, ARON, LAIRON, AGGRON, MEGA_AGGRON, MEDITITE, MEDICHAM, MEGA_MEDICHAM, ELECTRIKE, MANECTRIC, MEGA_MANECTRIC, PLUSLE, MINUN, VOLBEAT, ILLUMISE, ROSELIA, GULPIN, SWALOT, CARVANHA, SHARPEDO, MEGA_SHARPEDO, WAILMER, WAILORD, NUMEL, CAMERUPT, MEGA_CAMERUPT, TORKOAL, SPOINK, GRUMPIG, SPINDA, TRAPINCH, VIBRAVA, FLYGON, CACNEA, CACTURNE, SWABLU, ALTARIA, MEGA_ALTARIA, ZANGOOSE, SEVIPER, LUNATONE, SOLROCK, BARBOACH, WHISCASH, CORPHISH, CRAWDAUNT, BALTOY, CLAYDOL, LILEEP, CRADILY, ANORITH, ARMALDO, FEEBAS, MILOTIC, CASTFORM, SUNNY_CASTFORM, RAINY_CASTFORM, SNOWY_CASTFORM, KECLEON, SHUPPET, BANETTE, MEGA_BANETTE, DUSKULL, DUSCLOPS, TROPIUS, CHIMECHO, ABSOL, MEGA_ABSOL, WYNAUT, SNORUNT, GLALIE, MEGA_GLALIE, SPHEAL, SEALEO, WALREIN, CLAMPERL, HUNTAIL, GOREBYSS, RELICANTH, LUVDISC, BAGON, SHELGON, SALAMENCE, MEGA_SALAMENCE, BELDUM, METANG, METAGROSS, MEGA_METAGROSS, REGIROCK, REGICE, REGISTEEL, LATIAS, MEGA_LATIAS, LATIOS, MEGA_LATIOS, KYOGRE, PRIMAL_KYOGRE, GROUDON, PRIMAL_GROUDON, RAYQUAZA, MEGA_RAYQUAZA, JIRACHI, DEOXYS, ATTACK_DEOXYS, DEFENSE_DEOXYS, SPEED_DEOXYS, TURTWIG, GROTLE, TORTERRA, CHIMCHAR, MONFERNO, INFERNAPE, PIPLUP, PRINPLUP, EMPOLEON, STARLY, STARAVIA, STARAPTOR, BIDOOF, BIBAREL, KRICKETOT, KRICKETUNE, SHINX, LUXIO, LUXRAY, BUDEW, ROSERADE, CRANIDOS, RAMPARDOS, SHIELDON, BASTIODON, BURMY, PLANT_CLOAK_BURMY, SANDY_CLOAK_BURMY, TRASH_CLOAK_BURMY, PLANT_CLOAK_WORMADAM, SANDY_CLOAK_WORMADAM, TRASH_CLOAK_WORMADAM, MOTHIM, COMBEE, VESPIQUEN, PACHIRISU, BUIZEL, FLOATZEL, CHERUBI, CHERRIM, CHERRIM_OVERCAST, CHERRIM_SUNSHINE, SHELLOS, WEST_SEA_SHELLOS, EAST_SEA_SHELLOS, GASTRODON, WEST_SEA_GASTRODON, EAST_SEA_GASTRODON, AMBIPOM, DRIFLOON, DRIFBLIM, BUNEARY, LOPUNNY, MEGA_LOPUNNY, MISMAGIUS, HONCHKROW, GLAMEOW, PURUGLY, CHINGLING, STUNKY, SKUNTANK, BRONZOR, BRONZONG, BONSLY, MIME_JR, HAPPINY, CHATOT, SPIRITOMB, GIBLE, GABITE, GARCHOMP, MEGA_GARCHOMP, MUNCHLAX, RIOLU, LUCARIO, MEGA_LUCARIO, HIPPOPOTAS, HIPPOWDON, SKORUPI, DRAPION, CROAGUNK, TOXICROAK, CARNIVINE, FINNEON, LUMINEON, MANTYKE, SNOVER, ABOMASNOW, MEGA_ABOMASNOW, WEAVILE, MAGNEZONE, LICKILICKY, RHYPERIOR, TANGROWTH, ELECTIVIRE, MAGMORTAR, TOGEKISS, YANMEGA, LEAFEON, GLACEON, GLISCOR, MAMOSWINE, PORYGON_Z, GALLADE, MEGA_GALLADE, PROBOPASS, DUSKNOIR, FROSLASS, ROTOM, HEAT_ROTOM, WASH_ROTOM, FROST_ROTOM, FAN_ROTOM, MOW_ROTOM, UXIE, MESPRIT, AZELF, DIALGA, DIALGA_ORIGIN, PALKIA, PALKIA_ORIGIN, HEATRAN, REGIGIGAS, GIRATINA_ALTERED, GIRATINA_ORIGIN, CRESSELIA, PHIONE, MANAPHY, DARKRAI, SHAYMIN, SHAYMIN_LAND, SHAYMIN_SKY, ARCEUS, ARCEUS_BUG, ARCEUS_DARK, ARCEUS_DRAGON, ARCEUS_ELECTRIC, ARCEUS_FAIRY, ARCEUS_FIGHTING, ARCEUS_FIRE, ARCEUS_FLYING, ARCEUS_GHOST, ARCEUS_GRASS, ARCEUS_GROUND, ARCEUS_ICE, ARCEUS_POISON, ARCEUS_PSYCHIC, ARCEUS_ROCK, ARCEUS_STEEL, ARCEUS_WATER, VICTINI, SNIVY, SERVINE, SERPERIOR, TEPIG, PIGNITE, EMBOAR, OSHAWOTT, DEWOTT, HISUIAN_SAMUROTT, SAMUROTT, PATRAT, WATCHOG, LILLIPUP, HERDIER, STOUTLAND, PURRLOIN, LIEPARD, PANSAGE, SIMISAGE, PANSEAR, SIMISEAR, PANPOUR, SIMIPOUR, MUNNA, MUSHARNA, PIDOVE, TRANQUILL, UNFEZANT, BLITZLE, ZEBSTRIKA, ROGGENROLA, BOLDORE, GIGALITH, WOOBAT, SWOOBAT, DRILBUR, EXCADRILL, AUDINO, MEGA_AUDINO, TIMBURR, GURDURR, CONKELDURR, TYMPOLE, PALPITOAD, SEISMITOAD, THROH, SAWK, SEWADDLE, SWADLOON, LEAVANNY, VENIPEDE, WHIRLIPEDE, SCOLIPEDE, COTTONEE, WHIMSICOTT, PETILIL, LILLIGANT, HISUIAN_LILLIGANT, RED_STRIPED_BASCULIN, BLUE_STRIPED_BASCULIN, WHITE_STRIPED_BASCULIN, SANDILE, KROKOROK, KROOKODILE, DARUMAKA, GALARIAN_DARUMAKA, DARMANITAN, ZEN_MODE_DARMANITAN, GALARIAN_DARMANITAN, GALARIAN_ZEN_MODE_DARMANITAN, MARACTUS, DWEBBLE, CRUSTLE, SCRAGGY, SCRAFTY, SIGILYPH, YAMASK, GALARIAN_YAMASK, COFAGRIGUS, TIRTOUGA, CARRACOSTA, ARCHEN, ARCHEOPS, TRUBBISH, GARBODOR, GIGANTAMAX_GARBODOR, ZORUA, HISUIAN_ZORUA, HISUIAN_ZOROARK, ZOROARK, MINCCINO, CINCCINO, GOTHITA, GOTHORITA, GOTHITELLE, SOLOSIS, DUOSION, REUNICLUS, DUCKLETT, SWANNA, VANILLITE, VANILLISH, VANILLUXE, DEERLING, DEERLING_SPRING, DEERLING_SUMMER, DEERLING_AUTUMN, DEERLING_WINTER, SAWSBUCK, SAWSBUCK_SPRING, SAWSBUCK_SUMMER, SAWSBUCK_AUTUMN, SAWSBUCK_WINTER, EMOLGA, KARRABLAST, ESCAVALIER, FOONGUS, AMOONGUSS, FRILLISH, JELLICENT, ALOMOMOLA, JOLTIK, GALVANTULA, FERROSEED, FERROTHORN, KLINK, KLANG, KLINKLANG, TYNAMO, EELEKTRIK, EELEKTROSS, ELGYEM, BEHEEYEM, LITWICK, LAMPENT, CHANDELURE, AXEW, FRAXURE, HAXORUS, CUBCHOO, BEARTIC, CRYOGONAL, SHELMET, ACCELGOR, STUNFISK, GALARIAN_STUNFISK, MIENFOO, MIENSHAO, DRUDDIGON, GOLETT, GOLURK, PAWNIARD, BISHARP, BOUFFALANT, RUFFLET, HISUIAN_BRAVIARY, BRAVIARY, VULLABY, MANDIBUZZ, HEATMOR, DURANT, DEINO, ZWEILOUS, HYDREIGON, LARVESTA, VOLCARONA, COBALION, TERRAKION, VIRIZION, INCARNATE_TORNADUS, TORNADUS_THERIAN, INCARNATE_THUNDURUS, THUNDURUS_THERIAN, RESHIRAM, ZEKROM, INCARNATE_LANDORUS, LANDORUS_THERIAN, KYUREM, BLACK_KYUREM, WHITE_KYUREM, KELDEO, RESOLUTE_KELDEO, ARIA_MELOETTA, PIROUETTE_MELOETTA, GENESECT, DOUSE_DRIVE_GENESECT, SHOCK_DRIVE_GENESECT, BURN_DRIVE_GENESECT, CHILL_DRIVE_GENESECT, CHESPIN, QUILLADIN, CHESNAUGHT, FENNEKIN, BRAIXEN, DELPHOX, FROAKIE, FROGADIER, GRENINJA, ASH_GRENINJA, BUNNELBY, DIGGERSBY, FLETCHLING, FLETCHINDER, TALONFLAME, SCATTERBUG, SPEWPA, VIVILLON, MEADOW_PATTERN_VIVILLON, ARCHIPELAGO_PATTERN_VIVILLON, CONTINENTAL_PATTERN_VIVILLON, ELEGANT_PATTERN_VIVILLON, GARDEN_PATTERN_VIVILLON, HIGH_PLAINS_PATTERN_VIVILLON, ICY_SNOW_PATTERN_VIVILLON, JUNGLE_PATTERN_VIVILLON, MARINE_PATTERN_VIVILLON, MODERN_PATTERN_VIVILLON, MONSOON_PATTERN_VIVILLON, OCEAN_PATTERN_VIVILLON, POLAR_PATTERN_VIVILLON, RIVER_PATTERN_VIVILLON, SANDSTORM_PATTERN_VIVILLON, SAVANNA_PATTERN_VIVILLON, SUN_PATTERN_VIVILLON, TUNDRA_PATTERN_VIVILLON, FANCY_PATTERN_VIVILLON, POKEBALL_PATTERN_VIVILLON, LITLEO, PYROAR, FLABEBE, RED_FLOWER_FLABEBE, BLUE_FLOWER_FLABEBE, ORANGE_FLOWER_FLABEBE, WHITE_FLOWER_FLABEBE, YELLOW_FLOWER_FLABEBE, FLOETTE, RED_FLOWER_FLOETTE, BLUE_FLOWER_FLOETTE, ORANGE_FLOWER_FLOETTE, WHITE_FLOWER_FLOETTE, YELLOW_FLOWER_FLOETTE, ETERNAL_FLOWER_FLOETTE, FLORGES, RED_FLOWER_FLORGES, BLUE_FLOWER_FLORGES, ORANGE_FLOWER_FLORGES, WHITE_FLOWER_FLORGES, YELLOW_FLOWER_FLORGES, SKIDDO, GOGOAT, PANCHAM, PANGORO, FURFROU, NATURAL_FURFROU, DANDY_TRIM_FURFROU, DEBUTANTE_TRIM_FURFROU, DIAMOND_TRIM_FURFROU, HEART_TRIM_FURFROU, KABUKI_TRIM_FURFROU, LA_REINE_TRIM_FURFROU, MATRON_TRIM_FURFROU, PHARAOH_TRIM_FURFROU, STAR_TRIM_FURFROU, ESPURR, MALE_MEOWSTIC, FEMALE_MEOWSTIC, HONEDGE, DOUBLADE, AEGISLASH, SHIELD_AEGISLASH, BLADE_AEGISLASH, SPRITZEE, AROMATISSE, SWIRLIX, SLURPUFF, INKAY, MALAMAR, BINACLE, BARBARACLE, SKRELP, DRAGALGE, CLAUNCHER, CLAWITZER, HELIOPTILE, HELIOLISK, TYRUNT, TYRANTRUM, AMAURA, AURORUS, SYLVEON, HAWLUCHA, DEDENNE, CARBINK, GOOMY, HISUIAN_SLIGGOO, SLIGGOO, HISUIAN_GOODRA, GOODRA, KLEFKI, PHANTUMP, TREVENANT, AVERAGE_SIZE_PUMPKABOO, SMALL_SIZE_PUMPKABOO, LARGE_SIZE_PUMPKABOO, SUPER_SIZE_PUMPKABOO, AVERAGE_SIZE_GOURGEIST, SMALL_SIZE_GOURGEIST, LARGE_SIZE_GOURGEIST, SUPER_SIZE_GOURGEIST, BERGMITE, AVALUGG, HISUIAN_AVALUGG, NOIBAT, NOIVERN, XERNEAS, YVELTAL, ZYGARDE_50, ZYGARDE_10, ZYGARDE_COMPLETE, DIANCIE, MEGA_DIANCIE, HOOPA_CONFINED, HOOPA_UNBOUND, VOLCANION, ROWLET, DARTRIX, HISUIAN_DECIDUEYE, DECIDUEYE, LITTEN, TORRACAT, INCINEROAR, POPPLIO, BRIONNE, PRIMARINA, PIKIPEK, TRUMBEAK, TOUCANNON, YUNGOOS, GUMSHOOS, TOTEM_GUMSHOOS, GRUBBIN, CHARJABUG, VIKAVOLT, TOTEM_VIKAVOLT, CRABRAWLER, CRABOMINABLE, BAILE_STYLE_ORICORIO, POM_POM_STYLE_ORICORIO, PA_U_STYLE_ORICORIO, SENSU_STYLE_ORICORIO, CUTIEFLY, RIBOMBEE, TOTEM_RIBOMBEE, ROCKRUFF, MIDDAY_ROCKRUFF, MIDDAY_LYCANROC, MIDNIGHT_LYCANROC, DUSK_LYCANROC, WISHIWASHI, SOLO_WISHIWASHI, SCHOOL_WISHIWASHI, MAREANIE, TOXAPEX, MUDBRAY, MUDSDALE, DEWPIDER, ARAQUANID, TOTEM_ARAQUANID, FOMANTIS, LURANTIS, TOTEM_LURANTIS, MORELULL, SHIINOTIC, SALANDIT, SALAZZLE, TOTEM_SALAZZLE, STUFFUL, BEWEAR, BOUNSWEET, STEENEE, TSAREENA, COMFEY, ORANGURU, PASSIMIAN, WIMPOD, GOLISOPOD, SANDYGAST, PALOSSAND, PYUKUMUKU, TYPE_NULL, SILVALLY, SILVALLY_BUG, SILVALLY_DARK, SILVALLY_DRAGON, SILVALLY_ELECTRIC, SILVALLY_FAIRY, SILVALLY_FIGHTING, SILVALLY_FIRE, SILVALLY_FLYING, SILVALLY_GHOST, SILVALLY_GRASS, SILVALLY_GROUND, SILVALLY_ICE, SILVALLY_POISON, SILVALLY_PSYCHIC, SILVALLY_ROCK, SILVALLY_STEEL, SILVALLY_WATER, MINIOR, CORE_MINIOR, RED_CORE_MINIOR, ORANGE_CORE_MINIOR, YELLOW_CORE_MINIOR, GREEN_CORE_MINIOR, BLUE_CORE_MINIOR, INDIGO_CORE_MINIOR, VIOLET_CORE_MINIOR, METEOR_MINIOR, KOMALA, TURTONATOR, TOGEDEMARU, TOTEM_TOGEDEMARU, MIMIKYU, MIMIKYU_BUSTED, TOTEM_MIMIKYU, BUSTED_TOTEM_MIMIKYU, BRUXISH, DRAMPA, DHELMISE, JANGMO_O, HAKAMO_O, KOMMO_O, TOTEM_KOMMO_O, TAPU_KOKO, TAPU_LELE, TAPU_BULU, TAPU_FINI, COSMOG, COSMOEM, SOLGALEO, LUNALA, NIHILEGO, BUZZWOLE, PHEROMOSA, XURKITREE, CELESTEELA, KARTANA, GUZZLORD, NECROZMA, DUSK_MANE_NECROZMA, DAWN_WINGS_NECROZMA, ULTRA_NECROZMA, MAGEARNA, ORIGINAL_COLOR_MAGEARNA, MARSHADOW, POIPOLE, NAGANADEL, STAKATAKA, BLACEPHALON, ZERAORA, MELTAN, MELMETAL, GIGANTAMAX_MELMETAL, GROOKEY, THWACKEY, RILLABOOM, GIGANTAMAX_RILLABOOM, SCORBUNNY, RABOOT, CINDERACE, GIGANTAMAX_CINDERACE, SOBBLE, DRIZZILE, INTELEON, GIGANTAMAX_INTELEON, SKWOVET, GREEDENT, ROOKIDEE, CORVISQUIRE, CORVIKNIGHT, GIGANTAMAX_CORVIKNIGHT, BLIPBUG, DOTTLER, ORBEETLE, GIGANTAMAX_ORBEETLE, NICKIT, THIEVUL, GOSSIFLEUR, ELDEGOSS, WOOLOO, DUBWOOL, CHEWTLE, DREDNAW, GIGANTAMAX_DREDNAW, YAMPER, BOLTUND, ROLYCOLY, CARKOL, COALOSSAL, GIGANTAMAX_COALOSSAL, APPLIN, FLAPPLE, GIGANTAMAX_FLAPPLE, APPLETUN, GIGANTAMAX_APPLETUN, SILICOBRA, SANDACONDA, GIGANTAMAX_SANDACONDA, CRAMORANT, CRAMORANT_GULPING, CRAMORANT_GORGING, ARROKUDA, BARRASKEWDA, TOXEL, TOXTRICITY_AMPED, TOXTRICITY_LOW_KEY, GIGANTAMAX_TOXTRICITY_AMPED, GIGANTAMAX_TOXTRICITY_LOW_KEY, SIZZLIPEDE, CENTISKORCH, GIGANTAMAX_CENTISKORCH, CLOBBOPUS, GRAPPLOCT, SINISTEA, ANTIQUE_SINISTEA, POLTEAGEIST, ANTIQUE_POLTEAGEIST, HATENNA, HATTREM, HATTERENE, GIGANTAMAX_HATTERENE, IMPIDIMP, MORGREM, GRIMMSNARL, GIGANTAMAX_GRIMMSNARL, OBSTAGOON, PERRSERKER, CURSOLA, SIRFETCH_D, MR_RIME, RUNERIGUS, MILCERY, ALCREMIE, VANILLA_CREAM_ALCREMIE, RUBY_CREAM_ALCREMIE, MATCHA_CREAM_ALCREMIE, MINT_CREAM_ALCREMIE, LEMON_CREAM_ALCREMIE, SALTED_CREAM_ALCREMIE, RUBY_SWIRL_ALCREMIE, CARAMEL_SWIRL_ALCREMIE, RAINBOW_SWIRL_ALCREMIE, GIGANTAMAX_ALCREMIE, FALINKS, PINCURCHIN, SNOM, FROSMOTH, STONJOURNER, EISCUE, ICE_FACE_EISCUE, NOICE_FACE_EISCUE, MALE_INDEEDEE, FEMALE_INDEEDEE, MORPEKO, FULL_BELLY_MODE_MORPEKO, HANGRY_MODE_MORPEKO, CUFANT, COPPERAJAH, GIGANTAMAX_COPPERAJAH, DRACOZOLT, ARCTOZOLT, DRACOVISH, ARCTOVISH, DURALUDON, GIGANTAMAX_DURALUDON, DREEPY, DRAKLOAK, DRAGAPULT, HERO_OF_MANY_BATTLES_ZACIAN, CROWNED_SWORD_ZACIAN, HERO_OF_MANY_BATTLES_ZAMAZENTA, CROWNED_SHIELD_ZAMAZENTA, ETERNATUS, ETERNAMAX_ETERNATUS, KUBFU, SINGLE_STRIKE_STYLE_URSHIFU, RAPID_STRIKE_STYLE_URSHIFU, GIGANTAMAX_SINGLE_STRIKE_STYLE_URSHIFU, GIGANTAMAX_RAPID_STRIKE_STYLE_URSHIFU, ZARUDE, DADA_ZARUDE, REGIELEKI, REGIDRAGO, GLASTRIER, SPECTRIER, CALYREX, ICE_RIDER_CALYREX, SHADOW_RIDER_CALYREX, WYRDEER, KLEAVOR, URSALUNA, MALE_BASCULEGION, FEMALE_BASCULEGION, SNEASLER, OVERQWIL, INCARNATE_ENAMORUS, ENAMORUS_THERIAN, SPRIGATITO, FLORAGATO, MEOWSCARADA, FUECOCO, CROCALOR, SKELEDIRGE, QUAXLY, QUAXWELL, QUAQUAVAL, LECHONK, MALE_OINKOLOGNE, FEMALE_OINKOLOGNE, TAROUNTULA, SPIDOPS, NYMBLE, LOKIX, PAWMI, PAWMO, PAWMOT, TANDEMAUS, MAUSHOLD, FAMILY_OF_THREE_MAUSHOLD, FAMILY_OF_FOUR_MAUSHOLD, FIDOUGH, DACHSBUN, SMOLIV, DOLLIV, ARBOLIVA, GREEN_PLUMAGE_SQUAWKABILLY, BLUE_PLUMAGE_SQUAWKABILLY, YELLOW_PLUMAGE_SQUAWKABILLY, WHITE_PLUMAGE_SQUAWKABILLY, NACLI, NACLSTACK, GARGANACL, CHARCADET, ARMAROUGE, CERULEDGE, TADBULB, BELLIBOLT, WATTREL, KILOWATTREL, MASCHIFF, MABOSSTIFF, SHROODLE, GRAFAIAI, BRAMBLIN, BRAMBLEGHAST, TOEDSCOOL, TOEDSCRUEL, KLAWF, CAPSAKID, SCOVILLAIN, RELLOR, RABSCA, FLITTLE, ESPATHRA, TINKATINK, TINKATUFF, TINKATON, WIGLETT, WUGTRIO, BOMBIRDIER, FINIZEN, ZERO_PALAFIN, HERO_PALAFIN, VAROOM, REVAVROOM, CYCLIZAR, ORTHWORM, GLIMMET, GLIMMORA, GREAVARD, HOUNDSTONE, FLAMIGO, CETODDLE, CETITAN, VELUZA, DONDOZO, TATSUGIRI, CURLY_TATSUGIRI, DROOPY_TATSUGIRI, STRETCHY_TATSUGIRI, ANNIHILAPE, CLODSIRE, FARIGIRAF, DUDUNSPARCE, TWO_SEGMENT_DUDUNSPARCE, THREE_SEGMENT_DUDUNSPARCE, KINGAMBIT, GREAT_TUSK, SCREAM_TAIL, BRUTE_BONNET, FLUTTER_MANE, SLITHER_WING, SANDY_SHOCKS, IRON_TREADS, IRON_BUNDLE, IRON_HANDS, IRON_JUGULIS, IRON_MOTH, IRON_THORNS, FRIGIBAX, ARCTIBAX, BAXCALIBUR, CHEST_GIMMIGHOUL, ROAMING_GIMMIGHOUL, GHOLDENGO, WO_CHIEN, CHIEN_PAO, TING_LU, CHI_YU, ROARING_MOON, IRON_VALIANT, KORAIDON, MIRAIDON, WALKING_WAKE, IRON_LEAVES, SPECIES_TOTAL
  // clang-format on
};
}  // namespace pokesim::dex

///////////////////////// END OF src/Types/Species.hpp /////////////////////////

////////////////// START OF src/Components/Names/Species.hpp ///////////////////

namespace pokesim {
struct SpeciesName {
  dex::Species name = dex::MISSING_NO;
};
}  // namespace pokesim

/////////////////// END OF src/Components/Names/Species.hpp ////////////////////

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
 * @tparam Args Types of arguments to use to construct the object.
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
        using alloc_traits = typename std::allocator_traits<Allocator>;
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

    template<typename ILhs, typename IRhs>
    friend constexpr std::ptrdiff_t operator-(const dense_map_iterator<ILhs> &, const dense_map_iterator<IRhs> &) noexcept;

    template<typename ILhs, typename IRhs>
    friend constexpr bool operator==(const dense_map_iterator<ILhs> &, const dense_map_iterator<IRhs> &) noexcept;

    template<typename ILhs, typename IRhs>
    friend constexpr bool operator<(const dense_map_iterator<ILhs> &, const dense_map_iterator<IRhs> &) noexcept;

private:
    It it;
};

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr std::ptrdiff_t operator-(const dense_map_iterator<ILhs> &lhs, const dense_map_iterator<IRhs> &rhs) noexcept {
    return lhs.it - rhs.it;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator==(const dense_map_iterator<ILhs> &lhs, const dense_map_iterator<IRhs> &rhs) noexcept {
    return lhs.it == rhs.it;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator!=(const dense_map_iterator<ILhs> &lhs, const dense_map_iterator<IRhs> &rhs) noexcept {
    return !(lhs == rhs);
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator<(const dense_map_iterator<ILhs> &lhs, const dense_map_iterator<IRhs> &rhs) noexcept {
    return lhs.it < rhs.it;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator>(const dense_map_iterator<ILhs> &lhs, const dense_map_iterator<IRhs> &rhs) noexcept {
    return rhs < lhs;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator<=(const dense_map_iterator<ILhs> &lhs, const dense_map_iterator<IRhs> &rhs) noexcept {
    return !(lhs > rhs);
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator>=(const dense_map_iterator<ILhs> &lhs, const dense_map_iterator<IRhs> &rhs) noexcept {
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

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator==(const dense_map_local_iterator<ILhs> &lhs, const dense_map_local_iterator<IRhs> &rhs) noexcept {
    return lhs.index() == rhs.index();
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator!=(const dense_map_local_iterator<ILhs> &lhs, const dense_map_local_iterator<IRhs> &rhs) noexcept {
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
    using alloc_traits = typename std::allocator_traits<Allocator>;
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
     * The returned iterator points to the first instance of the internal array.
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
     *
     * The returned iterator points to the element following the last instance
     * of the internal array. Attempting to dereference the returned iterator
     * results in undefined behavior.
     *
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
    template<class Other>
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
    template<class Type>
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
template<class... Func>
struct overloaded: Func... {
    using Func::operator()...;
};

/**
 * @brief Deduction guide.
 * @tparam Func Types of function objects.
 */
template<class... Func>
overloaded(Func...) -> overloaded<Func...>;

/**
 * @brief Basic implementation of a y-combinator.
 * @tparam Func Type of a potentially recursive function.
 */
template<class Func>
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
    template<class... Args>
    constexpr decltype(auto) operator()(Args &&...args) const noexcept(std::is_nothrow_invocable_v<Func, const y_combinator &, Args...>) {
        return func(*this, std::forward<Args>(args)...);
    }

    /*! @copydoc operator()() */
    template<class... Args>
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
            static constexpr auto mask = (1 << Bit) - 1;
            static constexpr auto buckets = 1 << Bit;
            static constexpr auto passes = N / Bit;

            using value_type = typename std::iterator_traits<It>::value_type;
            std::vector<value_type> aux(std::distance(first, last));

            auto part = [getter = std::move(getter)](auto from, auto to, auto out, auto start) {
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
    using hs_traits = internal::fnv1a_traits<id_type>;

    struct const_wrapper {
        // non-explicit constructor on purpose
        constexpr const_wrapper(const Char *str) noexcept
            : repr{str} {}

        const Char *repr;
    };

    // Fowler–Noll–Vo hash function v. 1a - the good
    [[nodiscard]] static constexpr auto helper(const Char *str) noexcept {
        base_type base{str, 0u, hs_traits::offset};

        for(; str[base.length]; ++base.length) {
            base.hash = (base.hash ^ static_cast<hs_traits::type>(str[base.length])) * hs_traits::prime;
        }

        return base;
    }

    // Fowler–Noll–Vo hash function v. 1a - the good
    [[nodiscard]] static constexpr auto helper(const Char *str, const std::size_t len) noexcept {
        base_type base{str, len, hs_traits::offset};

        for(size_type pos{}; pos < len; ++pos) {
            base.hash = (base.hash ^ static_cast<hs_traits::type>(str[pos])) * hs_traits::prime;
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
    static constexpr bool in_situ = Len && alignof(Type) <= Align && sizeof(Type) <= Len &&std::is_nothrow_move_constructible_v<Type>;

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
                static_assert(sizeof...(Args) == 1u && (std::is_lvalue_reference_v<Args> && ...), "Invalid arguments");
                mode = std::is_const_v<std::remove_reference_t<Type>> ? policy::cref : policy::ref;
                instance = (std::addressof(args), ...);
            } else if constexpr(in_situ<std::remove_cv_t<std::remove_reference_t<Type>>>) {
                if constexpr(sizeof...(Args) != 0u && std::is_aggregate_v<std::remove_cv_t<std::remove_reference_t<Type>>>) {
                    new(&storage) std::remove_cv_t<std::remove_reference_t<Type>>{std::forward<Args>(args)...};
                } else {
                    new(&storage) std::remove_cv_t<std::remove_reference_t<Type>>(std::forward<Args>(args)...);
                }
            } else {
                if constexpr(sizeof...(Args) != 0u && std::is_aggregate_v<std::remove_cv_t<std::remove_reference_t<Type>>>) {
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
Type any_cast(const basic_any<Len, Align> &data) noexcept {
    const auto *const instance = any_cast<std::remove_reference_t<Type>>(&data);
    ENTT_ASSERT(instance, "Invalid instance");
    return static_cast<Type>(*instance);
}

/*! @copydoc any_cast */
template<typename Type, std::size_t Len, std::size_t Align>
Type any_cast(basic_any<Len, Align> &data) noexcept {
    // forces const on non-reference types to make them work also with wrappers for const references
    auto *const instance = any_cast<std::remove_reference_t<const Type>>(&data);
    ENTT_ASSERT(instance, "Invalid instance");
    return static_cast<Type>(*instance);
}

/*! @copydoc any_cast */
template<typename Type, std::size_t Len, std::size_t Align>
Type any_cast(basic_any<Len, Align> &&data) noexcept {
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
const Type *any_cast(const basic_any<Len, Align> *data) noexcept {
    const auto &info = type_id<std::remove_cv_t<Type>>();
    return static_cast<const Type *>(data->data(info));
}

/*! @copydoc any_cast */
template<typename Type, std::size_t Len, std::size_t Align>
Type *any_cast(basic_any<Len, Align> *data) noexcept {
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
basic_any<Len, Align> make_any(Args &&...args) {
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
basic_any<Len, Align> forward_as_any(Type &&value) {
    return basic_any<Len, Align>{std::in_place_type<Type &&>, std::forward<Type>(value)};
}

} // namespace entt

#endif

////////////////////// END OF external/entt/core/any.hpp ///////////////////////

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

template<typename Type>
struct in_place_delete<Type, std::enable_if_t<Type::in_place_delete>>
    : std::true_type {};

template<typename Type, typename = void>
struct page_size: std::integral_constant<std::size_t, !std::is_empty_v<ENTT_ETO_TYPE(Type)> * ENTT_PACKED_PAGE> {};

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

/**
 * @brief Helper variable template.
 * @tparam Type Type of component.
 */
template<class Type>
inline constexpr bool ignore_as_empty_v = (std::is_void_v<Type> || component_traits<Type>::page_size == 0u);

} // namespace entt

#endif

////////////////// END OF external/entt/entity/component.hpp ///////////////////

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

    [[nodiscard]] constexpr difference_type index() const noexcept {
        return offset - 1;
    }

private:
    const Container *packed;
    difference_type offset;
};

template<typename Type, typename Other>
[[nodiscard]] constexpr std::ptrdiff_t operator-(const sparse_set_iterator<Type> &lhs, const sparse_set_iterator<Other> &rhs) noexcept {
    return rhs.index() - lhs.index();
}

template<typename Type, typename Other>
[[nodiscard]] constexpr bool operator==(const sparse_set_iterator<Type> &lhs, const sparse_set_iterator<Other> &rhs) noexcept {
    return lhs.index() == rhs.index();
}

template<typename Type, typename Other>
[[nodiscard]] constexpr bool operator!=(const sparse_set_iterator<Type> &lhs, const sparse_set_iterator<Other> &rhs) noexcept {
    return !(lhs == rhs);
}

template<typename Type, typename Other>
[[nodiscard]] constexpr bool operator<(const sparse_set_iterator<Type> &lhs, const sparse_set_iterator<Other> &rhs) noexcept {
    return lhs.index() > rhs.index();
}

template<typename Type, typename Other>
[[nodiscard]] constexpr bool operator>(const sparse_set_iterator<Type> &lhs, const sparse_set_iterator<Other> &rhs) noexcept {
    return lhs.index() < rhs.index();
}

template<typename Type, typename Other>
[[nodiscard]] constexpr bool operator<=(const sparse_set_iterator<Type> &lhs, const sparse_set_iterator<Other> &rhs) noexcept {
    return !(lhs > rhs);
}

template<typename Type, typename Other>
[[nodiscard]] constexpr bool operator>=(const sparse_set_iterator<Type> &lhs, const sparse_set_iterator<Other> &rhs) noexcept {
    return !(lhs < rhs);
}

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/*! @brief Sparse set deletion policy. */
enum class deletion_policy : std::uint8_t {
    /*! @brief Swap-and-pop deletion policy. */
    swap_and_pop = 0u,
    /*! @brief In-place deletion policy. */
    in_place = 1u
};

/**
 * @brief Basic sparse set implementation.
 *
 * Sparse set or packed array or whatever is the name users give it.<br/>
 * Two arrays: an _external_ one and an _internal_ one; a _sparse_ one and a
 * _packed_ one; one used for direct access through contiguous memory, the other
 * one used to get the data through an extra level of indirection.<br/>
 * This is largely used by the registry to offer users the fastest access ever
 * to the components. Views and groups in general are almost entirely designed
 * around sparse sets.
 *
 * This type of data structure is widely documented in the literature and on the
 * web. This is nothing more than a customized implementation suitable for the
 * purpose of the framework.
 *
 * @note
 * Internal data structures arrange elements to maximize performance. There are
 * no guarantees that entities are returned in the insertion order when iterate
 * a sparse set. Do not make assumption on the order in any case.
 *
 * @tparam Entity A valid entity type (see entt_traits for more details).
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Entity, typename Allocator>
class basic_sparse_set {
    using alloc_traits = std::allocator_traits<Allocator>;
    static_assert(std::is_same_v<typename alloc_traits::value_type, Entity>, "Invalid value type");
    using sparse_container_type = std::vector<typename alloc_traits::pointer, typename alloc_traits::template rebind_alloc<typename alloc_traits::pointer>>;
    using packed_container_type = std::vector<Entity, Allocator>;
    using entity_traits = entt_traits<Entity>;

    [[nodiscard]] auto sparse_ptr(const Entity entt) const {
        const auto pos = static_cast<size_type>(entity_traits::to_entity(entt));
        const auto page = pos / entity_traits::page_size;
        return (page < sparse.size() && sparse[page]) ? (sparse[page] + fast_mod(pos, entity_traits::page_size)) : nullptr;
    }

    [[nodiscard]] auto &sparse_ref(const Entity entt) const {
        ENTT_ASSERT(sparse_ptr(entt), "Invalid element");
        const auto pos = static_cast<size_type>(entity_traits::to_entity(entt));
        return sparse[pos / entity_traits::page_size][fast_mod(pos, entity_traits::page_size)];
    }

    [[nodiscard]] auto &assure_at_least(const Entity entt) {
        const auto pos = static_cast<size_type>(entity_traits::to_entity(entt));
        const auto page = pos / entity_traits::page_size;

        if(!(page < sparse.size())) {
            sparse.resize(page + 1u, nullptr);
        }

        if(!sparse[page]) {
            auto page_allocator{packed.get_allocator()};
            sparse[page] = alloc_traits::allocate(page_allocator, entity_traits::page_size);
            std::uninitialized_fill(sparse[page], sparse[page] + entity_traits::page_size, null);
        }

        auto &elem = sparse[page][fast_mod(pos, entity_traits::page_size)];
        ENTT_ASSERT(elem == null, "Slot not available");
        return elem;
    }

    void release_sparse_pages() {
        auto page_allocator{packed.get_allocator()};

        for(auto &&page: sparse) {
            if(page != nullptr) {
                std::destroy(page, page + entity_traits::page_size);
                alloc_traits::deallocate(page_allocator, page, entity_traits::page_size);
                page = nullptr;
            }
        }
    }

private:
    virtual const void *get_at(const std::size_t) const {
        return nullptr;
    }

    virtual void swap_at(const std::size_t, const std::size_t) {}
    virtual void move_element(const std::size_t, const std::size_t) {}

protected:
    /*! @brief Random access iterator type. */
    using basic_iterator = internal::sparse_set_iterator<packed_container_type>;

    /**
     * @brief Erases an entity from a sparse set.
     * @param it An iterator to the element to pop.
     */
    void swap_and_pop(const basic_iterator it) {
        ENTT_ASSERT(mode == deletion_policy::swap_and_pop, "Deletion policy mismatched");
        auto &self = sparse_ref(*it);
        const auto entt = entity_traits::to_entity(self);
        sparse_ref(packed.back()) = entity_traits::combine(entt, entity_traits::to_integral(packed.back()));
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
        const auto entt = entity_traits::to_entity(std::exchange(sparse_ref(*it), null));
        packed[static_cast<size_type>(entt)] = std::exchange(free_list, entity_traits::combine(entt, entity_traits::reserved));
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
            elem = entity_traits::combine(static_cast<typename entity_traits::entity_type>(packed.size() - 1u), entity_traits::to_integral(entt));
            return begin();
        } else {
            const auto pos = static_cast<size_type>(entity_traits::to_entity(free_list));
            elem = entity_traits::combine(entity_traits::to_integral(free_list), entity_traits::to_integral(entt));
            free_list = std::exchange(packed[pos], entt);
            return --(end() - pos);
        }
    }

public:
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Underlying entity identifier. */
    using entity_type = typename entity_traits::value_type;
    /*! @brief Underlying version type. */
    using version_type = typename entity_traits::version_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Pointer type to contained entities. */
    using pointer = typename packed_container_type::const_pointer;
    /*! @brief Random access iterator type. */
    using iterator = basic_iterator;
    /*! @brief Constant random access iterator type. */
    using const_iterator = iterator;
    /*! @brief Reverse iterator type. */
    using reverse_iterator = std::reverse_iterator<iterator>;
    /*! @brief Constant reverse iterator type. */
    using const_reverse_iterator = reverse_iterator;

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
     * @param value Returned value type, if any.
     * @param pol Type of deletion policy.
     * @param allocator The allocator to use (possibly default-constructed).
     */
    explicit basic_sparse_set(const type_info &value, deletion_policy pol = deletion_policy::swap_and_pop, const allocator_type &allocator = {})
        : sparse{allocator},
          packed{allocator},
          info{&value},
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
        return sparse.size() * entity_traits::page_size;
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
     * @brief Direct access to the internal packed array.
     * @return A pointer to the internal packed array.
     */
    [[nodiscard]] pointer data() const noexcept {
        return packed.data();
    }

    /**
     * @brief Returns an iterator to the beginning.
     *
     * The returned iterator points to the first entity of the internal packed
     * array. If the sparse set is empty, the returned iterator will be equal to
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
     *
     * The returned iterator points to the element following the last entity in
     * a sparse set. Attempting to dereference the returned iterator results in
     * undefined behavior.
     *
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
     * The returned iterator points to the first entity of the reversed internal
     * packed array. If the sparse set is empty, the returned iterator will be
     * equal to `rend()`.
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
     *
     * The returned iterator points to the element following the last entity in
     * the reversed sparse set. Attempting to dereference the returned iterator
     * results in undefined behavior.
     *
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
        return contains(entt) ? --(end() - index(entt)) : end();
    }

    /**
     * @brief Checks if a sparse set contains an entity.
     * @param entt A valid identifier.
     * @return True if the sparse set contains the entity, false otherwise.
     */
    [[nodiscard]] bool contains(const entity_type entt) const noexcept {
        const auto elem = sparse_ptr(entt);
        constexpr auto cap = entity_traits::to_entity(null);
        // testing versions permits to avoid accessing the packed array
        return elem && (((~cap & entity_traits::to_integral(entt)) ^ entity_traits::to_integral(*elem)) < cap);
    }

    /**
     * @brief Returns the contained version for an identifier.
     * @param entt A valid identifier.
     * @return The version for the given identifier if present, the tombstone
     * version otherwise.
     */
    [[nodiscard]] version_type current(const entity_type entt) const noexcept {
        const auto elem = sparse_ptr(entt);
        constexpr auto fallback = entity_traits::to_version(tombstone);
        return elem ? entity_traits::to_version(*elem) : fallback;
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
        return static_cast<size_type>(entity_traits::to_entity(sparse_ref(entt)));
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
    [[nodiscard]] const void *get(const entity_type entt) const noexcept {
        return get_at(index(entt));
    }

    /*! @copydoc get */
    [[nodiscard]] void *get(const entity_type entt) noexcept {
        return const_cast<void *>(std::as_const(*this).get(entt));
    }

    /**
     * @brief Assigns an entity to a sparse set.
     *
     * @warning
     * Attempting to assign an entity that already belongs to the sparse set
     * results in undefined behavior.
     *
     * @param entt A valid identifier.
     * @param value Optional opaque value to forward to mixins, if any.
     * @return Iterator pointing to the emplaced element in case of success, the
     * `end()` iterator otherwise.
     */
    iterator emplace(const entity_type entt, const void *value = nullptr) {
        return try_emplace(entt, false, value);
    }

    /**
     * @brief Bump the version number of an entity.
     *
     * @warning
     * Attempting to bump the version of an entity that doesn't belong to the
     * sparse set results in undefined behavior.
     *
     * @param entt A valid identifier.
     */
    void bump(const entity_type entt) {
        auto &entity = sparse_ref(entt);
        ENTT_ASSERT(entt != tombstone && entity != null, "Cannot set the required version");
        entity = entity_traits::combine(entity_traits::to_integral(entity), entity_traits::to_integral(entt));
        packed[static_cast<size_type>(entity_traits::to_entity(entity))] = entt;
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
    iterator insert(It first, It last) {
        for(auto it = first; it != last; ++it) {
            try_emplace(*it, true);
        }

        return first == last ? end() : find(*first);
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
        const auto it = --(end() - index(entt));
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

        for(; first != last; ++first) {
            count += remove(*first);
        }

        return count;
    }

    /*! @brief Removes all tombstones from the packed array of a sparse set. */
    void compact() {
        size_type from = packed.size();
        for(; from && packed[from - 1u] == tombstone; --from) {}

        for(auto *it = &free_list; *it != null && from; it = std::addressof(packed[entity_traits::to_entity(*it)])) {
            if(const size_type to = entity_traits::to_entity(*it); to < from) {
                --from;
                move_element(from, to);

                using std::swap;
                swap(packed[from], packed[to]);

                const auto entity = static_cast<typename entity_traits::entity_type>(to);
                sparse_ref(packed[to]) = entity_traits::combine(entity, entity_traits::to_integral(packed[to]));
                *it = entity_traits::combine(static_cast<typename entity_traits::entity_type>(from), entity_traits::reserved);
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
        ENTT_ASSERT(contains(lhs) && contains(rhs), "Set does not contain entities");

        auto &entt = sparse_ref(lhs);
        auto &other = sparse_ref(rhs);

        const auto from = entity_traits::to_entity(entt);
        const auto to = entity_traits::to_entity(other);

        // basic no-leak guarantee (with invalid state) if swapping throws
        swap_at(static_cast<size_type>(from), static_cast<size_type>(to));
        entt = entity_traits::combine(to, entity_traits::to_integral(packed[from]));
        other = entity_traits::combine(from, entity_traits::to_integral(packed[to]));

        using std::swap;
        swap(packed[from], packed[to]);
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

                swap_at(next, idx);
                const auto entity = static_cast<typename entity_traits::entity_type>(curr);
                sparse_ref(entt) = entity_traits::combine(entity, entity_traits::to_integral(packed[curr]));
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
     * according to the order they have in `other`. All the other entities goes
     * to the end of the list and there are no guarantees on their order.<br/>
     * In other terms, this function can be used to impose the same order on two
     * sets by using one of them as a master and the other one as a slave.
     *
     * Iterating the sparse set with a couple of iterators returns elements in
     * the expected order after a call to `respect`. See `begin` and `end` for
     * more details.
     *
     * @param other The sparse sets that imposes the order of the entities.
     */
    void respect(const basic_sparse_set &other) {
        compact();

        const auto to = other.end();
        auto from = other.begin();

        for(size_type pos = packed.size() - 1; pos && from != to; ++from) {
            if(contains(*from)) {
                if(*from != packed[pos]) {
                    // basic no-leak guarantee (with invalid state) if swapping throws
                    swap_elements(packed[pos], *from);
                }

                --pos;
            }
        }
    }

    /*! @brief Clears a sparse set. */
    void clear() {
        if(const auto last = end(); free_list == null) {
            pop(begin(), last);
        } else {
            for(auto &&entity: *this) {
                // tombstone filter on itself
                if(const auto it = find(entity); it != last) {
                    pop(it, it + 1u);
                }
            }
        }

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
            return static_cast<Ret>(std::invoke(Candidate, std::forward<type_list_element_t<Index, type_list<Args...>>>(std::get<Index>(arguments))...));
        };
    }

    template<auto Candidate, typename Type, std::size_t... Index>
    [[nodiscard]] auto wrap(Type &, std::index_sequence<Index...>) noexcept {
        return [](const void *payload, Args... args) -> Ret {
            [[maybe_unused]] const auto arguments = std::forward_as_tuple(std::forward<Args>(args)...);
            Type *curr = static_cast<Type *>(const_cast<constness_as_t<void, Type> *>(payload));
            return static_cast<Ret>(std::invoke(Candidate, *curr, std::forward<type_list_element_t<Index, type_list<Args...>>>(std::get<Index>(arguments))...));
        };
    }

    template<auto Candidate, typename Type, std::size_t... Index>
    [[nodiscard]] auto wrap(Type *, std::index_sequence<Index...>) noexcept {
        return [](const void *payload, Args... args) -> Ret {
            [[maybe_unused]] const auto arguments = std::forward_as_tuple(std::forward<Args>(args)...);
            Type *curr = static_cast<Type *>(const_cast<constness_as_t<void, Type> *>(payload));
            return static_cast<Ret>(std::invoke(Candidate, curr, std::forward<type_list_element_t<Index, type_list<Args...>>>(std::get<Index>(arguments))...));
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

#include <algorithm>
#include <functional>
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
    using container_type = std::vector<delegate<Ret(Args...)>, typename alloc_traits::template rebind_alloc<delegate<Ret(Args...)>>>;

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
        for(auto &&call: std::as_const(calls)) {
            call(args...);
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
        for(auto &&call: calls) {
            if constexpr(std::is_void_v<Ret>) {
                if constexpr(std::is_invocable_r_v<bool, Func>) {
                    call(args...);
                    if(func()) {
                        break;
                    }
                } else {
                    call(args...);
                    func();
                }
            } else {
                if constexpr(std::is_invocable_r_v<bool, Func, Ret>) {
                    if(func(call(args...))) {
                        break;
                    }
                } else {
                    func(call(args...));
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
    using difference_type = typename signal_type::container_type::difference_type;

    template<auto Candidate, typename Type>
    static void release(Type value_or_instance, void *signal) {
        sink{*static_cast<signal_type *>(signal)}.disconnect<Candidate>(value_or_instance);
    }

    template<auto Candidate>
    static void release(void *signal) {
        sink{*static_cast<signal_type *>(signal)}.disconnect<Candidate>();
    }

    auto before(delegate<Ret(Args...)> call) {
        const auto &calls = signal->calls;
        const auto it = std::find(calls.cbegin(), calls.cend(), std::move(call));

        sink other{*this};
        other.offset = calls.cend() - it;
        return other;
    }

public:
    /**
     * @brief Constructs a sink that is allowed to modify a given signal.
     * @param ref A valid reference to a signal object.
     */
    sink(sigh<Ret(Args...), Allocator> &ref) noexcept
        : offset{},
          signal{&ref} {}

    /**
     * @brief Returns false if at least a listener is connected to the sink.
     * @return True if the sink has no listeners connected, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return signal->calls.empty();
    }

    /**
     * @brief Returns a sink that connects before a given free function or an
     * unbound member.
     * @tparam Function A valid free function pointer.
     * @return A properly initialized sink object.
     */
    template<auto Function>
    [[nodiscard]] sink before() {
        delegate<Ret(Args...)> call{};
        call.template connect<Function>();
        return before(std::move(call));
    }

    /**
     * @brief Returns a sink that connects before a free function with payload
     * or a bound member.
     * @tparam Candidate Member or free function to look for.
     * @tparam Type Type of class or type of payload.
     * @param value_or_instance A valid object that fits the purpose.
     * @return A properly initialized sink object.
     */
    template<auto Candidate, typename Type>
    [[nodiscard]] sink before(Type &&value_or_instance) {
        delegate<Ret(Args...)> call{};
        call.template connect<Candidate>(value_or_instance);
        return before(std::move(call));
    }

    /**
     * @brief Returns a sink that connects before a given instance or specific
     * payload.
     * @tparam Type Type of class or type of payload.
     * @param value_or_instance A valid object that fits the purpose.
     * @return A properly initialized sink object.
     */
    template<typename Type, typename = std::enable_if_t<!std::is_same_v<std::decay_t<std::remove_pointer_t<Type>>, void>, sink>>
    [[nodiscard]] sink before(Type &value_or_instance) {
        return before(&value_or_instance);
    }

    /**
     * @brief Returns a sink that connects before a given instance or specific
     * payload.
     * @param value_or_instance A valid pointer that fits the purpose.
     * @return A properly initialized sink object.
     */
    [[nodiscard]] sink before(const void *value_or_instance) {
        sink other{*this};

        if(value_or_instance) {
            const auto &calls = signal->calls;
            const auto it = std::find_if(calls.cbegin(), calls.cend(), [value_or_instance](const auto &delegate) {
                return delegate.data() == value_or_instance;
            });

            other.offset = calls.cend() - it;
        }

        return other;
    }

    /**
     * @brief Returns a sink that connects before anything else.
     * @return A properly initialized sink object.
     */
    [[nodiscard]] sink before() {
        sink other{*this};
        other.offset = signal->calls.size();
        return other;
    }

    /**
     * @brief Connects a free function (with or without payload), a bound or an
     * unbound member to a signal.
     *
     * The signal isn't responsible for the connected object or the payload, if
     * any. Users must guarantee that the lifetime of the instance overcomes the
     * one of the signal. On the other side, the signal handler performs
     * checks to avoid multiple connections for the same function.<br/>
     * When used to connect a free function with payload, its signature must be
     * such that the instance is the first argument before the ones used to
     * define the signal itself.
     *
     * @tparam Candidate Function or member to connect to the signal.
     * @tparam Type Type of class or type of payload, if any.
     * @param value_or_instance A valid object that fits the purpose, if any.
     * @return A properly initialized connection object.
     */
    template<auto Candidate, typename... Type>
    connection connect(Type &&...value_or_instance) {
        disconnect<Candidate>(value_or_instance...);

        delegate<Ret(Args...)> call{};
        call.template connect<Candidate>(value_or_instance...);
        signal->calls.insert(signal->calls.end() - offset, std::move(call));

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
        auto &calls = signal->calls;
        delegate<Ret(Args...)> call{};
        call.template connect<Candidate>(value_or_instance...);
        calls.erase(std::remove(calls.begin(), calls.end(), std::move(call)), calls.end());
    }

    /**
     * @brief Disconnects free functions with payload or bound members from a
     * signal.
     * @tparam Type Type of class or type of payload.
     * @param value_or_instance A valid object that fits the purpose.
     */
    template<typename Type, typename = std::enable_if_t<!std::is_same_v<std::decay_t<std::remove_pointer_t<Type>>, void>>>
    void disconnect(Type &value_or_instance) {
        disconnect(&value_or_instance);
    }

    /**
     * @brief Disconnects free functions with payload or bound members from a
     * signal.
     * @param value_or_instance A valid object that fits the purpose.
     */
    void disconnect(const void *value_or_instance) {
        if(value_or_instance) {
            auto &calls = signal->calls;
            auto predicate = [value_or_instance](const auto &delegate) { return delegate.data() == value_or_instance; };
            calls.erase(std::remove_if(calls.begin(), calls.end(), std::move(predicate)), calls.end());
        }
    }

    /*! @brief Disconnects all the listeners from a signal. */
    void disconnect() {
        signal->calls.clear();
    }

private:
    difference_type offset;
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

/////////////// START OF external/entt/entity/storage_mixin.hpp ////////////////

#ifndef ENTT_ENTITY_SIGH_STORAGE_MIXIN_HPP
#define ENTT_ENTITY_SIGH_STORAGE_MIXIN_HPP

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
class sigh_storage_mixin final: public Type {
    using basic_registry_type = basic_registry<typename Type::entity_type, typename Type::base_type::allocator_type>;
    using sigh_type = sigh<void(basic_registry_type &, const typename Type::entity_type), typename Type::allocator_type>;
    using basic_iterator = typename Type::basic_iterator;

    void pop(basic_iterator first, basic_iterator last) override {
        ENTT_ASSERT(owner != nullptr, "Invalid pointer to registry");

        for(; first != last; ++first) {
            const auto entt = *first;
            destruction.publish(*owner, entt);
            const auto it = Type::find(entt);
            Type::pop(it, it + 1u);
        }
    }

    basic_iterator try_emplace(const typename basic_registry_type::entity_type entt, const bool force_back, const void *value) final {
        ENTT_ASSERT(owner != nullptr, "Invalid pointer to registry");
        Type::try_emplace(entt, force_back, value);
        construction.publish(*owner, entt);
        return Type::find(entt);
    }

public:
    /*! @brief Allocator type. */
    using allocator_type = typename Type::allocator_type;
    /*! @brief Underlying entity identifier. */
    using entity_type = typename Type::entity_type;
    /*! @brief Expected registry type. */
    using registry_type = basic_registry_type;

    /*! @brief Default constructor. */
    sigh_storage_mixin()
        : sigh_storage_mixin{allocator_type{}} {}

    /**
     * @brief Constructs an empty storage with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit sigh_storage_mixin(const allocator_type &allocator)
        : Type{allocator},
          owner{},
          construction{allocator},
          destruction{allocator},
          update{allocator} {}

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    sigh_storage_mixin(sigh_storage_mixin &&other) noexcept
        : Type{std::move(other)},
          owner{other.owner},
          construction{std::move(other.construction)},
          destruction{std::move(other.destruction)},
          update{std::move(other.update)} {}

    /**
     * @brief Allocator-extended move constructor.
     * @param other The instance to move from.
     * @param allocator The allocator to use.
     */
    sigh_storage_mixin(sigh_storage_mixin &&other, const allocator_type &allocator) noexcept
        : Type{std::move(other), allocator},
          owner{other.owner},
          construction{std::move(other.construction), allocator},
          destruction{std::move(other.destruction), allocator},
          update{std::move(other.update), allocator} {}

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This storage.
     */
    sigh_storage_mixin &operator=(sigh_storage_mixin &&other) noexcept {
        Type::operator=(std::move(other));
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
    void swap(sigh_storage_mixin &other) {
        using std::swap;
        Type::swap(other);
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
     * @brief Assigns entities to a storage.
     * @tparam Args Types of arguments to use to construct the object.
     * @param entt A valid identifier.
     * @param args Parameters to use to initialize the object.
     * @return A reference to the newly created object.
     */
    template<typename... Args>
    decltype(auto) emplace(const entity_type entt, Args &&...args) {
        ENTT_ASSERT(owner != nullptr, "Invalid pointer to registry");
        Type::emplace(entt, std::forward<Args>(args)...);
        construction.publish(*owner, entt);
        return this->get(entt);
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
        ENTT_ASSERT(owner != nullptr, "Invalid pointer to registry");
        Type::patch(entt, std::forward<Func>(func)...);
        update.publish(*owner, entt);
        return this->get(entt);
    }

    /**
     * @brief Assigns entities to a storage.
     * @tparam It Type of input iterator.
     * @tparam Args Types of arguments to use to construct the objects assigned
     * to the entities.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     * @param args Parameters to use to initialize the objects assigned to the
     * entities.
     */
    template<typename It, typename... Args>
    void insert(It first, It last, Args &&...args) {
        ENTT_ASSERT(owner != nullptr, "Invalid pointer to registry");
        Type::insert(first, last, std::forward<Args>(args)...);

        for(auto it = construction.empty() ? last : first; it != last; ++it) {
            construction.publish(*owner, *it);
        }
    }

    /**
     * @brief Forwards variables to derived classes, if any.
     * @param value A variable wrapped in an opaque container.
     */
    void bind(any value) noexcept final {
        auto *reg = any_cast<basic_registry_type>(&value);
        owner = reg ? reg : owner;
        Type::bind(std::move(value));
    }

private:
    basic_registry_type *owner;
    sigh_type construction;
    sigh_type destruction;
    sigh_type update;
};

} // namespace entt

#endif

//////////////// END OF external/entt/entity/storage_mixin.hpp /////////////////

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

template<typename Container>
class storage_iterator final {
    friend storage_iterator<const Container>;

    using container_type = std::remove_const_t<Container>;
    using alloc_traits = std::allocator_traits<typename container_type::allocator_type>;
    using comp_traits = component_traits<std::remove_pointer_t<typename container_type::value_type>>;

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
        : packed{ref},
          offset{idx} {}

    template<bool Const = std::is_const_v<Container>, typename = std::enable_if_t<Const>>
    constexpr storage_iterator(const storage_iterator<std::remove_const_t<Container>> &other) noexcept
        : storage_iterator{other.packed, other.offset} {}

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
        return (*packed)[pos / comp_traits::page_size][fast_mod(pos, comp_traits::page_size)];
    }

    [[nodiscard]] constexpr pointer operator->() const noexcept {
        const auto pos = index();
        return (*packed)[pos / comp_traits::page_size] + fast_mod(pos, comp_traits::page_size);
    }

    [[nodiscard]] constexpr reference operator*() const noexcept {
        return *operator->();
    }

    [[nodiscard]] constexpr difference_type index() const noexcept {
        return offset - 1;
    }

private:
    Container *packed;
    difference_type offset;
};

template<typename CLhs, typename CRhs>
[[nodiscard]] constexpr std::ptrdiff_t operator-(const storage_iterator<CLhs> &lhs, const storage_iterator<CRhs> &rhs) noexcept {
    return rhs.index() - lhs.index();
}

template<typename CLhs, typename CRhs>
[[nodiscard]] constexpr bool operator==(const storage_iterator<CLhs> &lhs, const storage_iterator<CRhs> &rhs) noexcept {
    return lhs.index() == rhs.index();
}

template<typename CLhs, typename CRhs>
[[nodiscard]] constexpr bool operator!=(const storage_iterator<CLhs> &lhs, const storage_iterator<CRhs> &rhs) noexcept {
    return !(lhs == rhs);
}

template<typename CLhs, typename CRhs>
[[nodiscard]] constexpr bool operator<(const storage_iterator<CLhs> &lhs, const storage_iterator<CRhs> &rhs) noexcept {
    return lhs.index() > rhs.index();
}

template<typename CLhs, typename CRhs>
[[nodiscard]] constexpr bool operator>(const storage_iterator<CLhs> &lhs, const storage_iterator<CRhs> &rhs) noexcept {
    return lhs.index() < rhs.index();
}

template<typename CLhs, typename CRhs>
[[nodiscard]] constexpr bool operator<=(const storage_iterator<CLhs> &lhs, const storage_iterator<CRhs> &rhs) noexcept {
    return !(lhs > rhs);
}

template<typename CLhs, typename CRhs>
[[nodiscard]] constexpr bool operator>=(const storage_iterator<CLhs> &lhs, const storage_iterator<CRhs> &rhs) noexcept {
    return !(lhs < rhs);
}

template<typename It, typename... Other>
class extended_storage_iterator final {
    template<typename Iter, typename... Args>
    friend class extended_storage_iterator;

public:
    using value_type = decltype(std::tuple_cat(std::make_tuple(*std::declval<It>()), std::forward_as_tuple(*std::declval<Other>()...)));
    using pointer = input_iterator_pointer<value_type>;
    using reference = value_type;
    using difference_type = std::ptrdiff_t;
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

    template<typename... CLhs, typename... CRhs>
    friend constexpr bool operator==(const extended_storage_iterator<CLhs...> &, const extended_storage_iterator<CRhs...> &) noexcept;

private:
    std::tuple<It, Other...> it;
};

template<typename... CLhs, typename... CRhs>
[[nodiscard]] constexpr bool operator==(const extended_storage_iterator<CLhs...> &lhs, const extended_storage_iterator<CRhs...> &rhs) noexcept {
    return std::get<0>(lhs.it) == std::get<0>(rhs.it);
}

template<typename... CLhs, typename... CRhs>
[[nodiscard]] constexpr bool operator!=(const extended_storage_iterator<CLhs...> &lhs, const extended_storage_iterator<CRhs...> &rhs) noexcept {
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
 * @tparam Entity A valid entity type (see entt_traits for more details).
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Type, typename Entity, typename Allocator, typename>
class basic_storage: public basic_sparse_set<Entity, typename std::allocator_traits<Allocator>::template rebind_alloc<Entity>> {
    using alloc_traits = std::allocator_traits<Allocator>;
    static_assert(std::is_same_v<typename alloc_traits::value_type, Type>, "Invalid value type");
    using underlying_type = basic_sparse_set<Entity, typename alloc_traits::template rebind_alloc<Entity>>;
    using container_type = std::vector<typename alloc_traits::pointer, typename alloc_traits::template rebind_alloc<typename alloc_traits::pointer>>;
    using comp_traits = component_traits<Type>;

    static constexpr bool is_pinned_type_v = !(std::is_move_constructible_v<Type> && std::is_move_assignable_v<Type>);

    [[nodiscard]] auto &element_at(const std::size_t pos) const {
        return packed.first()[pos / comp_traits::page_size][fast_mod(pos, comp_traits::page_size)];
    }

    auto assure_at_least(const std::size_t pos) {
        auto &&container = packed.first();
        const auto idx = pos / comp_traits::page_size;

        if(!(idx < container.size())) {
            auto curr = container.size();
            container.resize(idx + 1u, nullptr);

            ENTT_TRY {
                for(const auto last = container.size(); curr < last; ++curr) {
                    container[curr] = alloc_traits::allocate(packed.second(), comp_traits::page_size);
                }
            }
            ENTT_CATCH {
                container.resize(curr);
                ENTT_THROW;
            }
        }

        return container[idx] + fast_mod(pos, comp_traits::page_size);
    }

    template<typename... Args>
    auto emplace_element(const Entity entt, const bool force_back, Args &&...args) {
        const auto it = base_type::try_emplace(entt, force_back);

        ENTT_TRY {
            auto elem = assure_at_least(static_cast<size_type>(it.index()));
            entt::uninitialized_construct_using_allocator(to_address(elem), packed.second(), std::forward<Args>(args)...);
        }
        ENTT_CATCH {
            base_type::pop(it, it + 1u);
            ENTT_THROW;
        }

        return it;
    }

    void shrink_to_size(const std::size_t sz) {
        for(auto pos = sz, length = base_type::size(); pos < length; ++pos) {
            if constexpr(comp_traits::in_place_delete) {
                if(base_type::at(pos) != tombstone) {
                    std::destroy_at(std::addressof(element_at(pos)));
                }
            } else {
                std::destroy_at(std::addressof(element_at(pos)));
            }
        }

        auto &&container = packed.first();
        auto page_allocator{packed.second()};
        const auto from = (sz + comp_traits::page_size - 1u) / comp_traits::page_size;

        for(auto pos = from, last = container.size(); pos < last; ++pos) {
            alloc_traits::deallocate(page_allocator, container[pos], comp_traits::page_size);
        }

        container.resize(from);
    }

private:
    const void *get_at(const std::size_t pos) const final {
        return std::addressof(element_at(pos));
    }

    void swap_at([[maybe_unused]] const std::size_t lhs, [[maybe_unused]] const std::size_t rhs) final {
        // use a runtime value to avoid compile-time suppression that drives the code coverage tool crazy
        ENTT_ASSERT((lhs + 1u) && !is_pinned_type_v, "Pinned type");

        if constexpr(!is_pinned_type_v) {
            using std::swap;
            swap(element_at(lhs), element_at(rhs));
        }
    }

    void move_element([[maybe_unused]] const std::size_t from, [[maybe_unused]] const std::size_t to) final {
        // use a runtime value to avoid compile-time suppression that drives the code coverage tool crazy
        ENTT_ASSERT((from + 1u) && !is_pinned_type_v, "Pinned type");

        if constexpr(!is_pinned_type_v) {
            auto &elem = element_at(from);
            entt::uninitialized_construct_using_allocator(to_address(assure_at_least(to)), packed.second(), std::move(elem));
            std::destroy_at(std::addressof(elem));
        }
    }

protected:
    /*! @brief Random access iterator type. */
    using basic_iterator = typename underlying_type::basic_iterator;

    /**
     * @brief Erases entities from a sparse set.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     */
    void pop(basic_iterator first, basic_iterator last) override {
        for(; first != last; ++first) {
            // cannot use first.index() because it would break with cross iterators
            auto &elem = element_at(base_type::index(*first));

            if constexpr(comp_traits::in_place_delete) {
                base_type::in_place_pop(first);
                std::destroy_at(std::addressof(elem));
            } else {
                auto &other = element_at(base_type::size() - 1u);
                // destroying on exit allows reentrant destructors
                [[maybe_unused]] auto unused = std::exchange(elem, std::move(other));
                std::destroy_at(std::addressof(other));
                base_type::swap_and_pop(first);
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
    basic_iterator try_emplace([[maybe_unused]] const Entity entt, [[maybe_unused]] const bool force_back, const void *value) override {
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
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Type of the objects assigned to entities. */
    using value_type = Type;
    /*! @brief Underlying entity identifier. */
    using entity_type = Entity;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Pointer type to contained elements. */
    using pointer = typename container_type::pointer;
    /*! @brief Constant pointer type to contained elements. */
    using const_pointer = typename alloc_traits::template rebind_traits<typename alloc_traits::const_pointer>::const_pointer;
    /*! @brief Random access iterator type. */
    using iterator = internal::storage_iterator<container_type>;
    /*! @brief Constant random access iterator type. */
    using const_iterator = internal::storage_iterator<const container_type>;
    /*! @brief Reverse iterator type. */
    using reverse_iterator = std::reverse_iterator<iterator>;
    /*! @brief Constant reverse iterator type. */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    /*! @brief Extended iterable storage proxy. */
    using iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::iterator, iterator>>;
    /*! @brief Constant extended iterable storage proxy. */
    using const_iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::const_iterator, const_iterator>>;

    /*! @brief Default constructor. */
    basic_storage()
        : basic_storage{allocator_type{}} {}

    /**
     * @brief Constructs an empty storage with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit basic_storage(const allocator_type &allocator)
        : base_type{type_id<value_type>(), deletion_policy{comp_traits::in_place_delete}, allocator},
          packed{container_type{allocator}, allocator} {}

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    basic_storage(basic_storage &&other) noexcept
        : base_type{std::move(other)},
          packed{std::move(other.packed)} {}

    /**
     * @brief Allocator-extended move constructor.
     * @param other The instance to move from.
     * @param allocator The allocator to use.
     */
    basic_storage(basic_storage &&other, const allocator_type &allocator) noexcept
        : base_type{std::move(other), allocator},
          packed{container_type{std::move(other.packed.first()), allocator}, allocator} {
        ENTT_ASSERT(alloc_traits::is_always_equal::value || packed.second() == other.packed.second(), "Copying a storage is not allowed");
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
        ENTT_ASSERT(alloc_traits::is_always_equal::value || packed.second() == other.packed.second(), "Copying a storage is not allowed");

        shrink_to_size(0u);
        base_type::operator=(std::move(other));
        packed.first() = std::move(other.packed.first());
        propagate_on_container_move_assignment(packed.second(), other.packed.second());
        return *this;
    }

    /**
     * @brief Exchanges the contents with those of a given storage.
     * @param other Storage to exchange the content with.
     */
    void swap(basic_storage &other) {
        using std::swap;
        underlying_type::swap(other);
        propagate_on_container_swap(packed.second(), other.packed.second());
        swap(packed.first(), other.packed.first());
    }

    /**
     * @brief Returns the associated allocator.
     * @return The associated allocator.
     */
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return allocator_type{packed.second()};
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
        return packed.first().size() * comp_traits::page_size;
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
        return packed.first().data();
    }

    /*! @copydoc raw */
    [[nodiscard]] pointer raw() noexcept {
        return packed.first().data();
    }

    /**
     * @brief Returns an iterator to the beginning.
     *
     * The returned iterator points to the first instance of the internal array.
     * If the storage is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first instance of the internal array.
     */
    [[nodiscard]] const_iterator cbegin() const noexcept {
        const auto pos = static_cast<typename iterator::difference_type>(base_type::size());
        return const_iterator{&packed.first(), pos};
    }

    /*! @copydoc cbegin */
    [[nodiscard]] const_iterator begin() const noexcept {
        return cbegin();
    }

    /*! @copydoc begin */
    [[nodiscard]] iterator begin() noexcept {
        const auto pos = static_cast<typename iterator::difference_type>(base_type::size());
        return iterator{&packed.first(), pos};
    }

    /**
     * @brief Returns an iterator to the end.
     *
     * The returned iterator points to the element following the last instance
     * of the internal array. Attempting to dereference the returned iterator
     * results in undefined behavior.
     *
     * @return An iterator to the element following the last instance of the
     * internal array.
     */
    [[nodiscard]] const_iterator cend() const noexcept {
        return const_iterator{&packed.first(), {}};
    }

    /*! @copydoc cend */
    [[nodiscard]] const_iterator end() const noexcept {
        return cend();
    }

    /*! @copydoc end */
    [[nodiscard]] iterator end() noexcept {
        return iterator{&packed.first(), {}};
    }

    /**
     * @brief Returns a reverse iterator to the beginning.
     *
     * The returned iterator points to the first instance of the reversed
     * internal array. If the storage is empty, the returned iterator will be
     * equal to `rend()`.
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
     *
     * The returned iterator points to the element following the last instance
     * of the reversed internal array. Attempting to dereference the returned
     * iterator results in undefined behavior.
     *
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
        if constexpr(std::is_aggregate_v<value_type>) {
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
     */
    template<typename It>
    void insert(It first, It last, const value_type &value = {}) {
        for(; first != last; ++first) {
            emplace_element(*first, true, value);
        }
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
     */
    template<typename EIt, typename CIt, typename = std::enable_if_t<std::is_same_v<typename std::iterator_traits<CIt>::value_type, value_type>>>
    void insert(EIt first, EIt last, CIt from) {
        for(; first != last; ++first, ++from) {
            emplace_element(*first, true, *from);
        }
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

private:
    compressed_pair<container_type, allocator_type> packed;
};

/*! @copydoc basic_storage */
template<typename Type, typename Entity, typename Allocator>
class basic_storage<Type, Entity, Allocator, std::enable_if_t<ignore_as_empty_v<Type>>>
    : public basic_sparse_set<Entity, typename std::allocator_traits<Allocator>::template rebind_alloc<Entity>> {
    using alloc_traits = std::allocator_traits<Allocator>;
    static_assert(std::is_same_v<typename alloc_traits::value_type, Type>, "Invalid value type");
    using underlying_type = basic_sparse_set<Entity, typename alloc_traits::template rebind_alloc<Entity>>;
    using comp_traits = component_traits<Type>;

public:
    /*! @brief Base type. */
    using base_type = underlying_type;
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Type of the objects assigned to entities. */
    using value_type = Type;
    /*! @brief Underlying entity identifier. */
    using entity_type = Entity;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Extended iterable storage proxy. */
    using iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::iterator>>;
    /*! @brief Constant extended iterable storage proxy. */
    using const_iterable = iterable_adaptor<internal::extended_storage_iterator<typename base_type::const_iterator>>;

    /*! @brief Default constructor. */
    basic_storage()
        : basic_storage{allocator_type{}} {}

    /**
     * @brief Constructs an empty container with a given allocator.
     * @param allocator The allocator to use.
     */
    explicit basic_storage(const allocator_type &allocator)
        : base_type{type_id<value_type>(), deletion_policy{comp_traits::in_place_delete}, allocator} {}

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
        if constexpr(ignore_as_empty_v<typename Type::value_type>) {
            return std::make_tuple();
        } else {
            return std::forward_as_tuple(cpool.rbegin()[it.index()]);
        }
    }

public:
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
 * invalidates all the iterators and using them results in undefined behavior.
 *
 * @tparam Get Types of storage _observed_ by the group.
 * @tparam Exclude Types of storage used to filter the group.
 */
template<typename... Get, typename... Exclude>
class basic_group<owned_t<>, get_t<Get...>, exclude_t<Exclude...>> {
    using underlying_type = std::common_type_t<typename Get::entity_type..., typename Exclude::entity_type...>;
    using basic_common_type = std::common_type_t<typename Get::base_type..., typename Exclude::base_type...>;

    template<typename Type>
    static constexpr std::size_t index_of = type_list_index_v<std::remove_const_t<Type>, type_list<typename Get::value_type...>>;

public:
    /*! @brief Underlying entity identifier. */
    using entity_type = underlying_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Common type among all storage types. */
    using base_type = basic_common_type;
    /*! @brief Random access iterator type. */
    using iterator = typename base_type::iterator;
    /*! @brief Reversed iterator type. */
    using reverse_iterator = typename base_type::reverse_iterator;
    /*! @brief Iterable group type. */
    using iterable = iterable_adaptor<internal::extended_group_iterator<iterator, owned_t<>, get_t<Get...>>>;

    /*! @brief Default constructor to use to create empty, invalid groups. */
    basic_group() noexcept
        : handler{} {}

    /**
     * @brief Constructs a group from a set of storage classes.
     * @param ref The actual entities to iterate.
     * @param gpool Storage types to iterate _observed_ by the group.
     */
    basic_group(basic_common_type &ref, Get &...gpool) noexcept
        : handler{&ref},
          pools{&gpool...} {}

    /**
     * @brief Returns a const reference to the underlying handler.
     * @return A const reference to the underlying handler.
     */
    [[nodiscard]] const base_type &handle() const noexcept {
        return *handler;
    }

    /**
     * @brief Returns the storage for a given component type.
     * @tparam Type Type of component of which to return the storage.
     * @return The storage for the given component type.
     */
    template<typename Type>
    [[nodiscard]] decltype(auto) storage() const noexcept {
        return storage<index_of<Type>>();
    }

    /**
     * @brief Returns the storage for a given index.
     * @tparam Index Index of the storage to return.
     * @return The storage for the given index.
     */
    template<std::size_t Index>
    [[nodiscard]] decltype(auto) storage() const noexcept {
        return *std::get<Index>(pools);
    }

    /**
     * @brief Returns the number of entities that are part of the group.
     * @return Number of entities that are part of the group.
     */
    [[nodiscard]] size_type size() const noexcept {
        return *this ? handler->size() : size_type{};
    }

    /**
     * @brief Returns the number of elements that a group has currently
     * allocated space for.
     * @return Capacity of the group.
     */
    [[nodiscard]] size_type capacity() const noexcept {
        return *this ? handler->capacity() : size_type{};
    }

    /*! @brief Requests the removal of unused capacity. */
    void shrink_to_fit() {
        if(*this) {
            handler->shrink_to_fit();
        }
    }

    /**
     * @brief Checks whether a group is empty.
     * @return True if the group is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return !*this || handler->empty();
    }

    /**
     * @brief Returns an iterator to the first entity of the group.
     *
     * The returned iterator points to the first entity of the group. If the
     * group is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first entity of the group.
     */
    [[nodiscard]] iterator begin() const noexcept {
        return *this ? handler->begin() : iterator{};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the group.
     *
     * The returned iterator points to the entity following the last entity of
     * the group. Attempting to dereference the returned iterator results in
     * undefined behavior.
     *
     * @return An iterator to the entity following the last entity of the
     * group.
     */
    [[nodiscard]] iterator end() const noexcept {
        return *this ? handler->end() : iterator{};
    }

    /**
     * @brief Returns an iterator to the first entity of the reversed group.
     *
     * The returned iterator points to the first entity of the reversed group.
     * If the group is empty, the returned iterator will be equal to `rend()`.
     *
     * @return An iterator to the first entity of the reversed group.
     */
    [[nodiscard]] reverse_iterator rbegin() const noexcept {
        return *this ? handler->rbegin() : reverse_iterator{};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the reversed
     * group.
     *
     * The returned iterator points to the entity following the last entity of
     * the reversed group. Attempting to dereference the returned iterator
     * results in undefined behavior.
     *
     * @return An iterator to the entity following the last entity of the
     * reversed group.
     */
    [[nodiscard]] reverse_iterator rend() const noexcept {
        return *this ? handler->rend() : reverse_iterator{};
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
        const auto it = *this ? handler->find(entt) : iterator{};
        return it != end() && *it == entt ? it : end();
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
        return handler != nullptr;
    }

    /**
     * @brief Checks if a group contains an entity.
     * @param entt A valid identifier.
     * @return True if the group contains the given entity, false otherwise.
     */
    [[nodiscard]] bool contains(const entity_type entt) const noexcept {
        return *this && handler->contains(entt);
    }

    /**
     * @brief Returns the components assigned to the given entity.
     *
     * Prefer this function instead of `registry::get` during iterations. It has
     * far better performance than its counterpart.
     *
     * @warning
     * Attempting to use an invalid component type results in a compilation
     * error. Attempting to use an entity that doesn't belong to the group
     * results in undefined behavior.
     *
     * @tparam Type Types of components to get.
     * @param entt A valid identifier.
     * @return The components assigned to the entity.
     */
    template<typename... Type>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        if constexpr(sizeof...(Type) == 0) {
            return std::apply([entt](auto *...curr) { return std::tuple_cat(curr->get_as_tuple(entt)...); }, pools);
        } else if constexpr(sizeof...(Type) == 1) {
            return (std::get<index_of<Type>>(pools)->get(entt), ...);
        } else {
            return std::tuple_cat(std::get<index_of<Type>>(pools)->get_as_tuple(entt)...);
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
        return iterable{{begin(), pools}, {end(), pools}};
    }

    /**
     * @brief Sort a group according to the given comparison function.
     *
     * Sort the group so that iterating it with a couple of iterators returns
     * entities and components in the expected order. See `begin` and `end` for
     * more details.
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
     * @tparam Type Optional types of components to compare.
     * @tparam Compare Type of comparison function object.
     * @tparam Sort Type of sort function object.
     * @tparam Args Types of arguments to forward to the sort function object.
     * @param compare A valid comparison function object.
     * @param algo A valid sort function object.
     * @param args Arguments to forward to the sort function object, if any.
     */
    template<typename... Type, typename Compare, typename Sort = std_sort, typename... Args>
    void sort(Compare compare, Sort algo = Sort{}, Args &&...args) {
        if(*this) {
            if constexpr(sizeof...(Type) == 0) {
                static_assert(std::is_invocable_v<Compare, const entity_type, const entity_type>, "Invalid comparison function");
                handler->sort(std::move(compare), std::move(algo), std::forward<Args>(args)...);
            } else {
                auto comp = [this, &compare](const entity_type lhs, const entity_type rhs) {
                    if constexpr(sizeof...(Type) == 1) {
                        return compare((std::get<index_of<Type>>(pools)->get(lhs), ...), (std::get<index_of<Type>>(pools)->get(rhs), ...));
                    } else {
                        return compare(std::forward_as_tuple(std::get<index_of<Type>>(pools)->get(lhs)...), std::forward_as_tuple(std::get<index_of<Type>>(pools)->get(rhs)...));
                    }
                };

                handler->sort(std::move(comp), std::move(algo), std::forward<Args>(args)...);
            }
        }
    }

    /**
     * @brief Sort the shared pool of entities according to the given component.
     *
     * Non-owning groups of the same type share with the registry a pool of
     * entities with its own order that doesn't depend on the order of any pool
     * of components. Users can order the underlying data structure so that it
     * respects the order of the pool of the given component.
     *
     * @note
     * The shared pool of entities and thus its order is affected by the changes
     * to each and every pool that it tracks. Therefore changes to those pools
     * can quickly ruin the order imposed to the pool of entities shared between
     * the non-owning groups.
     *
     * @tparam Type Type of component to use to impose the order.
     */
    template<typename Type>
    void sort() const {
        if(*this) {
            handler->respect(*std::get<index_of<Type>>(pools));
        }
    }

private:
    base_type *const handler;
    const std::tuple<Get *...> pools;
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
 * invalidates all the iterators and using them results in undefined behavior.
 *
 * @tparam Owned Types of storage _owned_ by the group.
 * @tparam Get Types of storage _observed_ by the group.
 * @tparam Exclude Types of storage used to filter the group.
 */
template<typename... Owned, typename... Get, typename... Exclude>
class basic_group<owned_t<Owned...>, get_t<Get...>, exclude_t<Exclude...>> {
    using underlying_type = std::common_type_t<typename Owned::entity_type..., typename Get::entity_type..., typename Exclude::entity_type...>;
    using basic_common_type = std::common_type_t<typename Owned::base_type..., typename Get::base_type..., typename Exclude::base_type...>;

    template<typename Type>
    static constexpr std::size_t index_of = type_list_index_v<std::remove_const_t<Type>, type_list<typename Owned::value_type..., typename Get::value_type...>>;

public:
    /*! @brief Underlying entity identifier. */
    using entity_type = underlying_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Common type among all storage types. */
    using base_type = basic_common_type;
    /*! @brief Random access iterator type. */
    using iterator = typename base_type::iterator;
    /*! @brief Reversed iterator type. */
    using reverse_iterator = typename base_type::reverse_iterator;
    /*! @brief Iterable group type. */
    using iterable = iterable_adaptor<internal::extended_group_iterator<iterator, owned_t<Owned...>, get_t<Get...>>>;

    /*! @brief Default constructor to use to create empty, invalid groups. */
    basic_group() noexcept
        : length{} {}

    /**
     * @brief Constructs a group from a set of storage classes.
     * @param extent The actual number of entities to iterate.
     * @param opool Storage types to iterate _owned_ by the group.
     * @param gpool Storage types to iterate _observed_ by the group.
     */
    basic_group(const std::size_t &extent, Owned &...opool, Get &...gpool) noexcept
        : pools{&opool..., &gpool...},
          length{&extent} {}

    /**
     * @brief Returns the storage for a given component type.
     * @tparam Type Type of component of which to return the storage.
     * @return The storage for the given component type.
     */
    template<typename Type>
    [[nodiscard]] decltype(auto) storage() const noexcept {
        return storage<index_of<Type>>();
    }

    /**
     * @brief Returns the storage for a given index.
     * @tparam Index Index of the storage to return.
     * @return The storage for the given index.
     */
    template<std::size_t Index>
    [[nodiscard]] decltype(auto) storage() const noexcept {
        return *std::get<Index>(pools);
    }

    /**
     * @brief Returns the number of entities that that are part of the group.
     * @return Number of entities that that are part of the group.
     */
    [[nodiscard]] size_type size() const noexcept {
        return *this ? *length : size_type{};
    }

    /**
     * @brief Checks whether a group is empty.
     * @return True if the group is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return !*this || !*length;
    }

    /**
     * @brief Returns an iterator to the first entity of the group.
     *
     * The returned iterator points to the first entity of the group. If the
     * group is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first entity of the group.
     */
    [[nodiscard]] iterator begin() const noexcept {
        return *this ? (std::get<0>(pools)->base_type::end() - *length) : iterator{};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the group.
     *
     * The returned iterator points to the entity following the last entity of
     * the group. Attempting to dereference the returned iterator results in
     * undefined behavior.
     *
     * @return An iterator to the entity following the last entity of the
     * group.
     */
    [[nodiscard]] iterator end() const noexcept {
        return *this ? std::get<0>(pools)->base_type::end() : iterator{};
    }

    /**
     * @brief Returns an iterator to the first entity of the reversed group.
     *
     * The returned iterator points to the first entity of the reversed group.
     * If the group is empty, the returned iterator will be equal to `rend()`.
     *
     * @return An iterator to the first entity of the reversed group.
     */
    [[nodiscard]] reverse_iterator rbegin() const noexcept {
        return *this ? std::get<0>(pools)->base_type::rbegin() : reverse_iterator{};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the reversed
     * group.
     *
     * The returned iterator points to the entity following the last entity of
     * the reversed group. Attempting to dereference the returned iterator
     * results in undefined behavior.
     *
     * @return An iterator to the entity following the last entity of the
     * reversed group.
     */
    [[nodiscard]] reverse_iterator rend() const noexcept {
        return *this ? (std::get<0>(pools)->base_type::rbegin() + *length) : reverse_iterator{};
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
        const auto it = *this ? std::get<0>(pools)->find(entt) : iterator{};
        return it != end() && it >= begin() && *it == entt ? it : end();
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
        return length != nullptr;
    }

    /**
     * @brief Checks if a group contains an entity.
     * @param entt A valid identifier.
     * @return True if the group contains the given entity, false otherwise.
     */
    [[nodiscard]] bool contains(const entity_type entt) const noexcept {
        return *this && std::get<0>(pools)->contains(entt) && (std::get<0>(pools)->index(entt) < (*length));
    }

    /**
     * @brief Returns the components assigned to the given entity.
     *
     * Prefer this function instead of `registry::get` during iterations. It has
     * far better performance than its counterpart.
     *
     * @warning
     * Attempting to use an invalid component type results in a compilation
     * error. Attempting to use an entity that doesn't belong to the group
     * results in undefined behavior.
     *
     * @tparam Type Types of components to get.
     * @param entt A valid identifier.
     * @return The components assigned to the entity.
     */
    template<typename... Type>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        if constexpr(sizeof...(Type) == 0) {
            return std::apply([entt](auto *...curr) { return std::tuple_cat(curr->get_as_tuple(entt)...); }, pools);
        } else if constexpr(sizeof...(Type) == 1) {
            return (std::get<index_of<Type>>(pools)->get(entt), ...);
        } else {
            return std::tuple_cat(std::get<index_of<Type>>(pools)->get_as_tuple(entt)...);
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
        return {{begin(), pools}, {end(), pools}};
    }

    /**
     * @brief Sort a group according to the given comparison function.
     *
     * Sort the group so that iterating it with a couple of iterators returns
     * entities and components in the expected order. See `begin` and `end` for
     * more details.
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
     * @tparam Type Optional types of components to compare.
     * @tparam Compare Type of comparison function object.
     * @tparam Sort Type of sort function object.
     * @tparam Args Types of arguments to forward to the sort function object.
     * @param compare A valid comparison function object.
     * @param algo A valid sort function object.
     * @param args Arguments to forward to the sort function object, if any.
     */
    template<typename... Type, typename Compare, typename Sort = std_sort, typename... Args>
    void sort(Compare compare, Sort algo = Sort{}, Args &&...args) const {
        if constexpr(sizeof...(Type) == 0) {
            static_assert(std::is_invocable_v<Compare, const entity_type, const entity_type>, "Invalid comparison function");
            std::get<0>(pools)->sort_n(*length, std::move(compare), std::move(algo), std::forward<Args>(args)...);
        } else {
            auto comp = [this, &compare](const entity_type lhs, const entity_type rhs) {
                if constexpr(sizeof...(Type) == 1) {
                    return compare((std::get<index_of<Type>>(pools)->get(lhs), ...), (std::get<index_of<Type>>(pools)->get(rhs), ...));
                } else {
                    return compare(std::forward_as_tuple(std::get<index_of<Type>>(pools)->get(lhs)...), std::forward_as_tuple(std::get<index_of<Type>>(pools)->get(rhs)...));
                }
            };

            std::get<0>(pools)->sort_n(*length, std::move(comp), std::move(algo), std::forward<Args>(args)...);
        }

        std::apply([this](auto *head, auto *...other) {
            for(auto next = *length; next; --next) {
                const auto pos = next - 1;
                [[maybe_unused]] const auto entt = head->data()[pos];
                (other->swap_elements(other->data()[pos], entt), ...);
            }
        },
                   pools);
    }

private:
    const std::tuple<Owned *..., Get *...> pools;
    const size_type *const length;
};

} // namespace entt

#endif

//////////////////// END OF external/entt/entity/group.hpp /////////////////////

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

template<typename Type, std::size_t Get, std::size_t Exclude>
class view_iterator final {
    using iterator_type = typename Type::const_iterator;

    [[nodiscard]] bool valid() const noexcept {
        return ((Get != 0u) || (*it != tombstone))
               && std::apply([entt = *it](const auto *...curr) { return (curr->contains(entt) && ...); }, pools)
               && std::apply([entt = *it](const auto *...curr) { return (!curr->contains(entt) && ...); }, filter);
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

    view_iterator(iterator_type curr, iterator_type to, std::array<const Type *, Get> all_of, std::array<const Type *, Exclude> none_of) noexcept
        : it{curr},
          last{to},
          pools{all_of},
          filter{none_of} {
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
    using difference_type = std::ptrdiff_t;
    using value_type = decltype(std::tuple_cat(std::make_tuple(*std::declval<It>()), std::declval<Type>().get_as_tuple({})...));
    using pointer = input_iterator_pointer<value_type>;
    using reference = value_type;
    using iterator_category = std::input_iterator_tag;

    constexpr extended_view_iterator()
        : it{},
          pools{} {}

    extended_view_iterator(It from, std::tuple<Type *...> storage)
        : it{from},
          pools{storage} {}

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
 * In all other cases, modifying the pools iterated by the view in any way
 * invalidates all the iterators and using them results in undefined behavior.
 *
 * @tparam Get Types of storage iterated by the view.
 * @tparam Exclude Types of storage used to filter the view.
 */
template<typename... Get, typename... Exclude>
class basic_view<get_t<Get...>, exclude_t<Exclude...>> {
    using underlying_type = std::common_type_t<typename Get::entity_type..., typename Exclude::entity_type...>;
    using basic_common_type = std::common_type_t<typename Get::base_type..., typename Exclude::base_type...>;

    template<typename, typename, typename>
    friend class basic_view;

    template<typename Type>
    static constexpr std::size_t index_of = type_list_index_v<std::remove_const_t<Type>, type_list<typename Get::value_type...>>;

    [[nodiscard]] auto opaque_check_set() const noexcept {
        std::array<const base_type *, sizeof...(Get) - 1u> other{};
        std::apply([&other, pos = 0u, view = view](const auto *...curr) mutable { ((curr == view ? void() : void(other[pos++] = curr)), ...); }, pools);
        return other;
    }

    [[nodiscard]] auto filter_as_array() const noexcept {
        return std::apply([](const auto *...curr) { return std::array<const base_type *, sizeof...(Exclude)>{curr...}; }, filter);
    }

    template<std::size_t Curr, std::size_t Other, typename... Args>
    [[nodiscard]] auto dispatch_get(const std::tuple<underlying_type, Args...> &curr) const {
        if constexpr(Curr == Other) {
            return std::forward_as_tuple(std::get<Args>(curr)...);
        } else {
            return storage<Other>().get_as_tuple(std::get<0>(curr));
        }
    }

    [[nodiscard]] auto reject(const underlying_type entt) const noexcept {
        return std::apply([entt](const auto *...curr) { return (curr->contains(entt) || ...); }, filter);
    }

    template<std::size_t Curr, typename Func, std::size_t... Index>
    void each(Func &func, std::index_sequence<Index...>) const {
        for(const auto curr: storage<Curr>().each()) {
            if(const auto entt = std::get<0>(curr); ((sizeof...(Get) != 1u) || (entt != tombstone)) && ((Curr == Index || storage<Index>().contains(entt)) && ...) && !reject(entt)) {
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
        ((&storage<Index>() == view ? each<Index>(func, seq) : void()), ...);
    }

public:
    /*! @brief Underlying entity identifier. */
    using entity_type = underlying_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Common type among all storage types. */
    using base_type = basic_common_type;
    /*! @brief Bidirectional iterator type. */
    using iterator = internal::view_iterator<base_type, sizeof...(Get) - 1u, sizeof...(Exclude)>;
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
     * @param exclude The storage for the types used to filter the view.
     */
    basic_view(Get &...value, Exclude &...exclude) noexcept
        : pools{&value...},
          filter{&exclude...},
          view{[](const base_type *first, const auto *...other) { ((first = other->size() < first->size() ? other : first), ...); return first; }(&value...)} {}

    /**
     * @brief Constructs a multi-type view from a set of storage classes.
     * @param value The storage for the types to iterate.
     * @param excl The storage for the types used to filter the view.
     */
    basic_view(std::tuple<Get &...> value, std::tuple<Exclude &...> excl = {}) noexcept
        : pools{std::apply([](auto &...curr) { return std::make_tuple(&curr...); }, value)},
          filter{std::apply([](auto &...curr) { return std::make_tuple(&curr...); }, excl)},
          view{std::apply([](const base_type *first, const auto *...other) { ((first = other->size() < first->size() ? other : first), ...); return first; }, pools)} {}

    /**
     * @brief Creates a new view driven by a given component in its iterations.
     * @tparam Type Type of component used to drive the iteration.
     * @return A new view driven by the given component in its iterations.
     */
    template<typename Type>
    [[nodiscard]] basic_view use() const noexcept {
        return use<index_of<Type>>();
    }

    /**
     * @brief Creates a new view driven by a given component in its iterations.
     * @tparam Index Index of the component used to drive the iteration.
     * @return A new view driven by the given component in its iterations.
     */
    template<std::size_t Index>
    [[nodiscard]] basic_view use() const noexcept {
        basic_view other{*this};
        other.view = &storage<Index>();
        return other;
    }

    /**
     * @brief Updates the internal leading view if required.
     * @return A newly created and internally optimized view.
     */
    [[nodiscard]] basic_view refresh() const noexcept {
        return std::apply([](auto *...elem) { return basic_view{*elem...}; }, std::tuple_cat(pools, filter));
    }

    /**
     * @brief Returns the leading storage of a view.
     * @return The leading storage of the view.
     */
    [[nodiscard]] const base_type &handle() const noexcept {
        return *view;
    }

    /**
     * @brief Returns the storage for a given component type.
     * @tparam Comp Type of component of which to return the storage.
     * @return The storage for the given component type.
     */
    template<typename Type>
    [[nodiscard]] decltype(auto) storage() const noexcept {
        return storage<index_of<Type>>();
    }

    /**
     * @brief Returns the storage for a given index.
     * @tparam Index Index of the storage to return.
     * @return The storage for the given index.
     */
    template<std::size_t Index>
    [[nodiscard]] decltype(auto) storage() const noexcept {
        return *std::get<Index>(pools);
    }

    /**
     * @brief Estimates the number of entities iterated by the view.
     * @return Estimated number of entities iterated by the view.
     */
    [[nodiscard]] size_type size_hint() const noexcept {
        return view->size();
    }

    /**
     * @brief Returns an iterator to the first entity of the view.
     *
     * The returned iterator points to the first entity of the view. If the view
     * is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first entity of the view.
     */
    [[nodiscard]] iterator begin() const noexcept {
        return iterator{view->begin(), view->end(), opaque_check_set(), filter_as_array()};
    }

    /**
     * @brief Returns an iterator that is past the last entity of the view.
     *
     * The returned iterator points to the entity following the last entity of
     * the view. Attempting to dereference the returned iterator results in
     * undefined behavior.
     *
     * @return An iterator to the entity following the last entity of the view.
     */
    [[nodiscard]] iterator end() const noexcept {
        return iterator{view->end(), view->end(), opaque_check_set(), filter_as_array()};
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
        auto it = view->rbegin();
        for(const auto last = view->rend(); it != last && !contains(*it); ++it) {}
        return it == view->rend() ? null : *it;
    }

    /**
     * @brief Finds an entity.
     * @param entt A valid identifier.
     * @return An iterator to the given entity if it's found, past the end
     * iterator otherwise.
     */
    [[nodiscard]] iterator find(const entity_type entt) const noexcept {
        return contains(entt) ? iterator{view->find(entt), view->end(), opaque_check_set(), filter_as_array()} : end();
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
     * @brief Checks if a view is properly initialized.
     * @return True if the view is properly initialized, false otherwise.
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return view != nullptr;
    }

    /**
     * @brief Checks if a view contains an entity.
     * @param entt A valid identifier.
     * @return True if the view contains the given entity, false otherwise.
     */
    [[nodiscard]] bool contains(const entity_type entt) const noexcept {
        return std::apply([entt](const auto *...curr) { return (curr->contains(entt) && ...); }, pools)
               && std::apply([entt](const auto *...curr) { return (!curr->contains(entt) && ...); }, filter);
    }

    /**
     * @brief Returns the components assigned to the given entity.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the view results in
     * undefined behavior.
     *
     * @tparam Type Types of components to get.
     * @param entt A valid identifier.
     * @return The components assigned to the entity.
     */
    template<typename... Type>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        if constexpr(sizeof...(Type) == 0) {
            return std::apply([entt](auto *...curr) { return std::tuple_cat(curr->get_as_tuple(entt)...); }, pools);
        } else if constexpr(sizeof...(Type) == 1) {
            return (storage<index_of<Type>>().get(entt), ...);
        } else {
            return std::tuple_cat(storage<index_of<Type>>().get_as_tuple(entt)...);
        }
    }

    /**
     * @brief Returns the components assigned to the given entity.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the view results in
     * undefined behavior.
     *
     * @tparam First Index of a component to get.
     * @tparam Other Indexes of other components to get.
     * @param entt A valid identifier.
     * @return The components assigned to the entity.
     */
    template<std::size_t First, std::size_t... Other>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        if constexpr(sizeof...(Other) == 0) {
            return storage<First>().get(entt);
        } else {
            return std::tuple_cat(storage<First>().get_as_tuple(entt), storage<Other>().get_as_tuple(entt)...);
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
        pick_and_each(func, std::index_sequence_for<Get...>{});
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
        return std::make_from_tuple<basic_view<get_t<Get..., OGet...>, exclude_t<Exclude..., OExclude...>>>(
            std::apply([](auto *...curr) { return std::forward_as_tuple(*curr...); }, std::tuple_cat(pools, other.pools, filter, other.filter)));
    }

private:
    std::tuple<Get *...> pools;
    std::tuple<Exclude *...> filter;
    const base_type *view;
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
 * In all other cases, modifying the pool iterated by the view in any way
 * invalidates all the iterators and using them results in undefined behavior.
 *
 * @tparam Get Type of storage iterated by the view.
 */
template<typename Get>
class basic_view<get_t<Get>, exclude_t<>, std::void_t<std::enable_if_t<!component_traits<typename Get::value_type>::in_place_delete>>> {
    template<typename, typename, typename>
    friend class basic_view;

public:
    /*! @brief Underlying entity identifier. */
    using entity_type = typename Get::entity_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Common type among all storage types. */
    using base_type = typename Get::base_type;
    /*! @brief Random access iterator type. */
    using iterator = typename base_type::iterator;
    /*! @brief Reversed iterator type. */
    using reverse_iterator = typename base_type::reverse_iterator;
    /*! @brief Iterable view type. */
    using iterable = decltype(std::declval<Get>().each());

    /*! @brief Default constructor to use to create empty, invalid views. */
    basic_view() noexcept
        : pools{},
          filter{} {}

    /**
     * @brief Constructs a single-type view from a storage class.
     * @param ref The storage for the type to iterate.
     */
    basic_view(Get &ref) noexcept
        : pools{&ref},
          filter{} {}

    /**
     * @brief Constructs a single-type view from a storage class.
     * @param ref The storage for the type to iterate.
     */
    basic_view(std::tuple<Get &> ref, std::tuple<> = {}) noexcept
        : pools{&std::get<0>(ref)},
          filter{} {}

    /**
     * @brief Returns the leading storage of a view.
     * @return The leading storage of the view.
     */
    [[nodiscard]] const base_type &handle() const noexcept {
        return storage();
    }

    /**
     * @brief Returns the storage for a given component type.
     * @tparam Type Type of component of which to return the storage.
     * @return The storage for the given component type.
     */
    template<typename Type = typename Get::value_type>
    [[nodiscard]] decltype(auto) storage() const noexcept {
        static_assert(std::is_same_v<std::remove_const_t<Type>, typename Get::value_type>, "Invalid component type");
        return storage<0>();
    }

    /**
     * @brief Returns the storage for a given index.
     * @tparam Index Index of the storage to return.
     * @return The storage for the given index.
     */
    template<std::size_t Index>
    [[nodiscard]] decltype(auto) storage() const noexcept {
        return *std::get<Index>(pools);
    }

    /**
     * @brief Returns the number of entities that have the given component.
     * @return Number of entities that have the given component.
     */
    [[nodiscard]] size_type size() const noexcept {
        return handle().size();
    }

    /**
     * @brief Checks whether a view is empty.
     * @return True if the view is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const noexcept {
        return handle().empty();
    }

    /**
     * @brief Returns an iterator to the first entity of the view.
     *
     * The returned iterator points to the first entity of the view. If the view
     * is empty, the returned iterator will be equal to `end()`.
     *
     * @return An iterator to the first entity of the view.
     */
    [[nodiscard]] iterator begin() const noexcept {
        return handle().begin();
    }

    /**
     * @brief Returns an iterator that is past the last entity of the view.
     *
     * The returned iterator points to the entity following the last entity of
     * the view. Attempting to dereference the returned iterator results in
     * undefined behavior.
     *
     * @return An iterator to the entity following the last entity of the view.
     */
    [[nodiscard]] iterator end() const noexcept {
        return handle().end();
    }

    /**
     * @brief Returns an iterator to the first entity of the reversed view.
     *
     * The returned iterator points to the first entity of the reversed view. If
     * the view is empty, the returned iterator will be equal to `rend()`.
     *
     * @return An iterator to the first entity of the reversed view.
     */
    [[nodiscard]] reverse_iterator rbegin() const noexcept {
        return handle().rbegin();
    }

    /**
     * @brief Returns an iterator that is past the last entity of the reversed
     * view.
     *
     * The returned iterator points to the entity following the last entity of
     * the reversed view. Attempting to dereference the returned iterator
     * results in undefined behavior.
     *
     * @return An iterator to the entity following the last entity of the
     * reversed view.
     */
    [[nodiscard]] reverse_iterator rend() const noexcept {
        return handle().rend();
    }

    /**
     * @brief Returns the first entity of the view, if any.
     * @return The first entity of the view if one exists, the null entity
     * otherwise.
     */
    [[nodiscard]] entity_type front() const noexcept {
        return empty() ? null : *begin();
    }

    /**
     * @brief Returns the last entity of the view, if any.
     * @return The last entity of the view if one exists, the null entity
     * otherwise.
     */
    [[nodiscard]] entity_type back() const noexcept {
        return empty() ? null : *rbegin();
    }

    /**
     * @brief Finds an entity.
     * @param entt A valid identifier.
     * @return An iterator to the given entity if it's found, past the end
     * iterator otherwise.
     */
    [[nodiscard]] iterator find(const entity_type entt) const noexcept {
        return contains(entt) ? handle().find(entt) : end();
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
        return storage().get(entt);
    }

    /**
     * @brief Checks if a view is properly initialized.
     * @return True if the view is properly initialized, false otherwise.
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return std::get<0>(pools) != nullptr;
    }

    /**
     * @brief Checks if a view contains an entity.
     * @param entt A valid identifier.
     * @return True if the view contains the given entity, false otherwise.
     */
    [[nodiscard]] bool contains(const entity_type entt) const noexcept {
        return handle().contains(entt);
    }

    /**
     * @brief Returns the component assigned to the given entity.
     *
     * @warning
     * Attempting to use an entity that doesn't belong to the view results in
     * undefined behavior.
     *
     * @tparam Type Type or index of the component to get.
     * @param entt A valid identifier.
     * @return The component assigned to the entity.
     */
    template<typename... Type>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        if constexpr(sizeof...(Type) == 0) {
            return storage().get_as_tuple(entt);
        } else {
            static_assert((std::is_same_v<std::remove_const_t<Type>, typename Get::value_type> && ...), "Invalid component type");
            return storage().get(entt);
        }
    }

    /*! @copydoc get */
    template<std::size_t Index>
    [[nodiscard]] decltype(auto) get(const entity_type entt) const {
        return storage().get(entt);
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
        if constexpr(is_applicable_v<Func, decltype(*each().begin())>) {
            for(const auto pack: each()) {
                std::apply(func, pack);
            }
        } else if constexpr(ignore_as_empty_v<typename Get::value_type>) {
            for(size_type pos{}, last = size(); pos < last; ++pos) {
                func();
            }
        } else {
            for(auto &&component: storage()) {
                func(component);
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
        return storage().each();
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
        return std::make_from_tuple<basic_view<get_t<Get, OGet...>, exclude_t<OExclude...>>>(
            std::apply([](auto *...curr) { return std::forward_as_tuple(*curr...); }, std::tuple_cat(pools, other.pools, other.filter)));
    }

private:
    std::tuple<Get *> pools;
    std::tuple<> filter;
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

    template<typename ILhs, typename IRhs>
    friend constexpr std::ptrdiff_t operator-(const registry_storage_iterator<ILhs> &, const registry_storage_iterator<IRhs> &) noexcept;

    template<typename ILhs, typename IRhs>
    friend constexpr bool operator==(const registry_storage_iterator<ILhs> &, const registry_storage_iterator<IRhs> &) noexcept;

    template<typename ILhs, typename IRhs>
    friend constexpr bool operator<(const registry_storage_iterator<ILhs> &, const registry_storage_iterator<IRhs> &) noexcept;

private:
    It it;
};

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr std::ptrdiff_t operator-(const registry_storage_iterator<ILhs> &lhs, const registry_storage_iterator<IRhs> &rhs) noexcept {
    return lhs.it - rhs.it;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator==(const registry_storage_iterator<ILhs> &lhs, const registry_storage_iterator<IRhs> &rhs) noexcept {
    return lhs.it == rhs.it;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator!=(const registry_storage_iterator<ILhs> &lhs, const registry_storage_iterator<IRhs> &rhs) noexcept {
    return !(lhs == rhs);
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator<(const registry_storage_iterator<ILhs> &lhs, const registry_storage_iterator<IRhs> &rhs) noexcept {
    return lhs.it < rhs.it;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator>(const registry_storage_iterator<ILhs> &lhs, const registry_storage_iterator<IRhs> &rhs) noexcept {
    return rhs < lhs;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator<=(const registry_storage_iterator<ILhs> &lhs, const registry_storage_iterator<IRhs> &rhs) noexcept {
    return !(lhs > rhs);
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator>=(const registry_storage_iterator<ILhs> &lhs, const registry_storage_iterator<IRhs> &rhs) noexcept {
    return !(lhs < rhs);
}

class registry_context {
    using key_type = id_type;
    using mapped_type = basic_any<0u>;
    using container_type = dense_map<key_type, mapped_type, identity>;

public:
    template<typename Type, typename... Args>
    [[deprecated("Use ::emplace_as instead")]] Type &emplace_hint(const id_type id, Args &&...args) {
        return emplace_as<Type>(id, std::forward<Args>(args)...);
    }

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
    [[deprecated("Use ::get instead")]] [[nodiscard]] const Type &at(const id_type id = type_id<Type>().hash()) const {
        return get<Type>(id);
    }

    template<typename Type>
    [[deprecated("Use ::get instead")]] [[nodiscard]] Type &at(const id_type id = type_id<Type>().hash()) {
        return get<Type>(id);
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
    container_type ctx;
};

} // namespace internal

/**
 * Internal details not to be documented.
 * @endcond
 */

/**
 * @brief Fast and reliable entity-component system.
 * @tparam Entity A valid entity type (see entt_traits for more details).
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Entity, typename Allocator>
class basic_registry {
    using alloc_traits = typename std::allocator_traits<Allocator>;
    static_assert(std::is_same_v<typename alloc_traits::value_type, Entity>, "Invalid value type");
    using basic_common_type = basic_sparse_set<Entity, Allocator>;
    using entity_traits = entt_traits<Entity>;

    template<typename Type>
    using storage_for_type = typename storage_for<Type, Entity, typename alloc_traits::template rebind_alloc<std::remove_const_t<Type>>>::type;

    template<typename...>
    struct group_handler;

    template<typename... Exclude, typename... Get, typename... Owned>
    struct group_handler<exclude_t<Exclude...>, get_t<Get...>, Owned...> {
        // nasty workaround for an issue with the toolset v141 that doesn't accept a fold expression here
        static_assert(!std::disjunction_v<std::bool_constant<component_traits<Owned>::in_place_delete>...>, "Groups do not support in-place delete");
        using value_type = std::conditional_t<sizeof...(Owned) == 0, basic_common_type, std::size_t>;
        value_type current{};

        template<typename... Args>
        group_handler(Args &&...args)
            : current{std::forward<Args>(args)...} {}

        template<typename Type>
        void maybe_valid_if(basic_registry &owner, const Entity entt) {
            [[maybe_unused]] const auto cpools = std::forward_as_tuple(owner.assure<Owned>()...);

            const auto is_valid = ((std::is_same_v<Type, Owned> || std::get<storage_for_type<Owned> &>(cpools).contains(entt)) && ...)
                                  && ((std::is_same_v<Type, Get> || owner.assure<Get>().contains(entt)) && ...)
                                  && ((std::is_same_v<Type, Exclude> || !owner.assure<Exclude>().contains(entt)) && ...);

            if constexpr(sizeof...(Owned) == 0) {
                if(is_valid && !current.contains(entt)) {
                    current.emplace(entt);
                }
            } else {
                if(is_valid && !(std::get<0>(cpools).index(entt) < current)) {
                    const auto pos = current++;
                    (std::get<storage_for_type<Owned> &>(cpools).swap_elements(std::get<storage_for_type<Owned> &>(cpools).data()[pos], entt), ...);
                }
            }
        }

        void discard_if([[maybe_unused]] basic_registry &owner, const Entity entt) {
            if constexpr(sizeof...(Owned) == 0) {
                current.remove(entt);
            } else {
                if(const auto cpools = std::forward_as_tuple(owner.assure<Owned>()...); std::get<0>(cpools).contains(entt) && (std::get<0>(cpools).index(entt) < current)) {
                    const auto pos = --current;
                    (std::get<storage_for_type<Owned> &>(cpools).swap_elements(std::get<storage_for_type<Owned> &>(cpools).data()[pos], entt), ...);
                }
            }
        }
    };

    struct group_data {
        std::size_t size;
        std::shared_ptr<void> group;
        bool (*owned)(const id_type) noexcept;
        bool (*get)(const id_type) noexcept;
        bool (*exclude)(const id_type) noexcept;
    };

    template<typename Type>
    [[nodiscard]] auto &assure(const id_type id = type_hash<Type>::value()) {
        static_assert(std::is_same_v<Type, std::decay_t<Type>>, "Non-decayed types not allowed");
        auto &cpool = pools[id];

        if(!cpool) {
            cpool = std::allocate_shared<storage_for_type<std::remove_const_t<Type>>>(get_allocator(), get_allocator());
            cpool->bind(forward_as_any(*this));
        }

        ENTT_ASSERT(cpool->type() == type_id<Type>(), "Unexpected type");
        return static_cast<storage_for_type<Type> &>(*cpool);
    }

    template<typename Type>
    [[nodiscard]] const auto &assure(const id_type id = type_hash<Type>::value()) const {
        static_assert(std::is_same_v<Type, std::decay_t<Type>>, "Non-decayed types not allowed");

        if(const auto it = pools.find(id); it != pools.cend()) {
            ENTT_ASSERT(it->second->type() == type_id<Type>(), "Unexpected type");
            return static_cast<const storage_for_type<Type> &>(*it->second);
        }

        static storage_for_type<Type> placeholder{};
        return placeholder;
    }

    auto generate_identifier(const std::size_t pos) noexcept {
        ENTT_ASSERT(pos < entity_traits::to_entity(null), "No entities available");
        return entity_traits::combine(static_cast<typename entity_traits::entity_type>(pos), {});
    }

    auto recycle_identifier() noexcept {
        ENTT_ASSERT(free_list != null, "No entities available");
        const auto curr = entity_traits::to_entity(free_list);
        free_list = entity_traits::combine(entity_traits::to_integral(epool[curr]), tombstone);
        return (epool[curr] = entity_traits::combine(curr, entity_traits::to_integral(epool[curr])));
    }

    auto release_entity(const Entity entt, const typename entity_traits::version_type version) {
        const typename entity_traits::version_type vers = version + (version == entity_traits::to_version(tombstone));
        epool[entity_traits::to_entity(entt)] = entity_traits::construct(entity_traits::to_integral(free_list), vers);
        free_list = entity_traits::combine(entity_traits::to_integral(entt), tombstone);
        return vers;
    }

    void rebind() {
        for(auto &&curr: pools) {
            curr.second->bind(forward_as_any(*this));
        }
    }

public:
    /*! @brief Allocator type. */
    using allocator_type = Allocator;
    /*! @brief Underlying entity identifier. */
    using entity_type = Entity;
    /*! @brief Underlying version type. */
    using version_type = typename entity_traits::version_type;
    /*! @brief Unsigned integer type. */
    using size_type = std::size_t;
    /*! @brief Common type among all storage types. */
    using base_type = basic_common_type;
    /*! @brief Context type. */
    using context = internal::registry_context;

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
        : vars{},
          free_list{tombstone},
          epool{allocator},
          pools{allocator},
          groups{allocator} {
        pools.reserve(count);
    }

    /**
     * @brief Move constructor.
     * @param other The instance to move from.
     */
    basic_registry(basic_registry &&other) noexcept
        : vars{std::move(other.vars)},
          free_list{std::move(other.free_list)},
          epool{std::move(other.epool)},
          pools{std::move(other.pools)},
          groups{std::move(other.groups)} {
        rebind();
    }

    /**
     * @brief Move assignment operator.
     * @param other The instance to move from.
     * @return This registry.
     */
    basic_registry &operator=(basic_registry &&other) noexcept {
        vars = std::move(other.vars);
        free_list = std::move(other.free_list);
        epool = std::move(other.epool);
        pools = std::move(other.pools);
        groups = std::move(other.groups);

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
        swap(free_list, other.free_list);
        swap(epool, other.epool);
        swap(pools, other.pools);
        swap(groups, other.groups);

        rebind();
        other.rebind();
    }

    /**
     * @brief Returns the associated allocator.
     * @return The associated allocator.
     */
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return epool.get_allocator();
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
    [[nodiscard]] base_type *storage(const id_type id) {
        return const_cast<base_type *>(std::as_const(*this).storage(id));
    }

    /**
     * @brief Finds the storage associated with a given name, if any.
     * @param id Name used to map the storage within the registry.
     * @return A pointer to the storage if it exists, a null pointer otherwise.
     */
    [[nodiscard]] const base_type *storage(const id_type id) const {
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
    decltype(auto) storage(const id_type id = type_hash<Type>::value()) {
        return assure<Type>(id);
    }

    /**
     * @brief Returns the storage for a given component type.
     *
     * @warning
     * If a storage for the given component doesn't exist yet, a temporary
     * placeholder is returned instead.
     *
     * @tparam Type Type of component of which to return the storage.
     * @param id Optional name used to map the storage within the registry.
     * @return The storage for the given component type.
     */
    template<typename Type>
    decltype(auto) storage(const id_type id = type_hash<Type>::value()) const {
        return assure<Type>(id);
    }

    /**
     * @brief Returns the number of entities created so far.
     * @return Number of entities created so far.
     */
    [[nodiscard]] size_type size() const noexcept {
        return epool.size();
    }

    /**
     * @brief Returns the number of entities still in use.
     * @return Number of entities still in use.
     */
    [[nodiscard]] size_type alive() const {
        auto sz = epool.size();

        for(auto curr = free_list; curr != null; --sz) {
            curr = epool[entity_traits::to_entity(curr)];
        }

        return sz;
    }

    /**
     * @brief Increases the capacity (number of entities) of the registry.
     * @param cap Desired capacity.
     */
    void reserve(const size_type cap) {
        epool.reserve(cap);
    }

    /**
     * @brief Returns the number of entities that a registry has currently
     * allocated space for.
     * @return Capacity of the registry.
     */
    [[nodiscard]] size_type capacity() const noexcept {
        return epool.capacity();
    }

    /**
     * @brief Checks whether the registry is empty (no entities still in use).
     * @return True if the registry is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const {
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
    [[nodiscard]] const entity_type *data() const noexcept {
        return epool.data();
    }

    /**
     * @brief Returns the head of the list of released entities.
     *
     * This function is intended for use in conjunction with `assign`.<br/>
     * The returned entity has an invalid identifier in all cases.
     *
     * @return The head of the list of released entities.
     */
    [[nodiscard]] entity_type released() const noexcept {
        return free_list;
    }

    /**
     * @brief Checks if an identifier refers to a valid entity.
     * @param entt An identifier, either valid or not.
     * @return True if the identifier is valid, false otherwise.
     */
    [[nodiscard]] bool valid(const entity_type entt) const {
        const auto pos = size_type(entity_traits::to_entity(entt));
        return (pos < epool.size() && epool[pos] == entt);
    }

    /**
     * @brief Returns the actual version for an identifier.
     * @param entt A valid identifier.
     * @return The version for the given identifier if valid, the tombstone
     * version otherwise.
     */
    [[nodiscard]] version_type current(const entity_type entt) const {
        const auto pos = size_type(entity_traits::to_entity(entt));
        return entity_traits::to_version(pos < epool.size() ? epool[pos] : tombstone);
    }

    /**
     * @brief Creates a new entity or recycles a destroyed one.
     * @return A valid identifier.
     */
    [[nodiscard]] entity_type create() {
        return (free_list == null) ? epool.emplace_back(generate_identifier(epool.size())) : recycle_identifier();
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
        const auto length = epool.size();

        if(hint == null || hint == tombstone) {
            return create();
        } else if(const auto req = entity_traits::to_entity(hint); !(req < length)) {
            epool.resize(size_type(req) + 1u, null);

            for(auto pos = length; pos < req; ++pos) {
                release_entity(generate_identifier(pos), {});
            }

            return (epool[req] = hint);
        } else if(const auto curr = entity_traits::to_entity(epool[req]); req == curr) {
            return create();
        } else {
            auto *it = &free_list;
            for(; entity_traits::to_entity(*it) != req; it = &epool[entity_traits::to_entity(*it)]) {}
            *it = entity_traits::combine(curr, entity_traits::to_integral(*it));
            return (epool[req] = hint);
        }
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
        for(; free_list != null && first != last; ++first) {
            *first = recycle_identifier();
        }

        const auto length = epool.size();
        epool.resize(length + std::distance(first, last), null);

        for(auto pos = length; first != last; ++first, ++pos) {
            *first = epool[pos] = generate_identifier(pos);
        }
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
     * @param destroyed The head of the list of destroyed entities.
     */
    template<typename It>
    void assign(It first, It last, const entity_type destroyed) {
        ENTT_ASSERT(!alive(), "Entities still alive");
        epool.assign(first, last);
        free_list = destroyed;
    }

    /**
     * @brief Releases an identifier.
     *
     * The version is updated and the identifier can be recycled at any time.
     *
     * @warning
     * Attempting to use an invalid entity results in undefined behavior.
     *
     * @param entt A valid identifier.
     * @return The version of the recycled entity.
     */
    version_type release(const entity_type entt) {
        return release(entt, static_cast<version_type>(entity_traits::to_version(entt) + 1u));
    }

    /**
     * @brief Releases an identifier.
     *
     * The suggested version or the valid version closest to the suggested one
     * is used instead of the implicitly generated version.
     *
     * @sa release
     *
     * @param entt A valid identifier.
     * @param version A desired version upon destruction.
     * @return The version actually assigned to the entity.
     */
    version_type release(const entity_type entt, const version_type version) {
        ENTT_ASSERT(valid(entt), "Invalid identifier");
        ENTT_ASSERT(std::all_of(pools.cbegin(), pools.cend(), [entt](auto &&curr) { return (curr.second->current(entt) == entity_traits::to_version(tombstone)); }), "Non-orphan entity");
        return release_entity(entt, version);
    }

    /**
     * @brief Releases all identifiers in a range.
     *
     * @sa release
     *
     * @tparam It Type of input iterator.
     * @param first An iterator to the first element of the range of entities.
     * @param last An iterator past the last element of the range of entities.
     */
    template<typename It>
    void release(It first, It last) {
        for(; first != last; ++first) {
            release(*first);
        }
    }

    /**
     * @brief Destroys an entity and releases its identifier.
     *
     * @sa release
     *
     * @warning
     * Adding or removing components to an entity that is being destroyed can
     * result in undefined behavior. Attempting to use an invalid entity results
     * in undefined behavior.
     *
     * @param entt A valid identifier.
     * @return The version of the recycled entity.
     */
    version_type destroy(const entity_type entt) {
        return destroy(entt, static_cast<version_type>(entity_traits::to_version(entt) + 1u));
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
        for(size_type pos = pools.size(); pos; --pos) {
            pools.begin()[pos - 1u].second->remove(entt);
        }

        return release(entt, version);
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
        for(; first != last; ++first) {
            destroy(*first);
        }
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
        assure<Type>().insert(first, last, value);
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
     * @note
     * Empty types aren't explicitly instantiated and therefore they are never
     * returned. However, this function can be used to trigger an update signal
     * for them.
     *
     * @warning
     * Attempting to to patch a component of an entity that doesn't own it
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
     *
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
        if constexpr(sizeof...(Other) == 0u) {
            return assure<Type>().remove(std::move(first), std::move(last));
        } else {
            size_type count{};

            for(auto cpools = std::forward_as_tuple(assure<Type>(), assure<Other>()...); first != last; ++first) {
                count += std::apply([entt = *first](auto &...curr) { return (curr.remove(entt) + ... + 0u); }, cpools);
            }

            return count;
        }
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
        if constexpr(sizeof...(Other) == 0u) {
            assure<Type>().erase(std::move(first), std::move(last));
        } else {
            for(auto cpools = std::forward_as_tuple(assure<Type>(), assure<Other>()...); first != last; ++first) {
                std::apply([entt = *first](auto &...curr) { (curr.erase(entt), ...); }, cpools);
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
        if constexpr(sizeof...(Type) == 0) {
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
        return (assure<std::remove_const_t<Type>>().contains(entt) && ...);
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
        return (assure<std::remove_const_t<Type>>().contains(entt) || ...);
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
            return (assure<std::remove_const_t<Type>>().get(entt), ...);
        } else {
            return std::forward_as_tuple(get<Type>(entt)...);
        }
    }

    /*! @copydoc get */
    template<typename... Type>
    [[nodiscard]] decltype(auto) get([[maybe_unused]] const entity_type entt) {
        if constexpr(sizeof...(Type) == 1u) {
            return (const_cast<Type &>(std::as_const(*this).template get<Type>(entt)), ...);
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
        if constexpr(sizeof...(Type) == 1) {
            const auto &cpool = assure<std::remove_const_t<Type>...>();
            return cpool.contains(entt) ? std::addressof(cpool.get(entt)) : nullptr;
        } else {
            return std::make_tuple(try_get<Type>(entt)...);
        }
    }

    /*! @copydoc try_get */
    template<typename... Type>
    [[nodiscard]] auto try_get([[maybe_unused]] const entity_type entt) {
        if constexpr(sizeof...(Type) == 1) {
            return (const_cast<Type *>(std::as_const(*this).template try_get<Type>(entt)), ...);
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
        if constexpr(sizeof...(Type) == 0) {
            for(auto &&curr: pools) {
                curr.second->clear();
            }

            each([this](const auto entity) { this->release(entity); });
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
    void each(Func func) const {
        if(free_list == null) {
            for(auto pos = epool.size(); pos; --pos) {
                func(epool[pos - 1]);
            }
        } else {
            for(auto pos = epool.size(); pos; --pos) {
                if(const auto entity = epool[pos - 1]; entity_traits::to_entity(entity) == (pos - 1)) {
                    func(entity);
                }
            }
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
     * @return A temporary sink object.
     */
    template<typename Type>
    [[nodiscard]] auto on_construct() {
        return assure<Type>().on_construct();
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
     * @return A temporary sink object.
     */
    template<typename Type>
    [[nodiscard]] auto on_update() {
        return assure<Type>().on_update();
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
     * @return A temporary sink object.
     */
    template<typename Type>
    [[nodiscard]] auto on_destroy() {
        return assure<Type>().on_destroy();
    }

    /**
     * @brief Returns a view for the given components.
     *
     * Views are created on the fly and share with the registry its internal
     * data structures. Feel free to discard them after the use.<br/>
     * Creating and destroying a view is an incredibly cheap operation. As a
     * rule of thumb, storing a view should never be an option.
     *
     * @tparam Type Type of component used to construct the view.
     * @tparam Other Other types of components used to construct the view.
     * @tparam Exclude Types of components used to filter the view.
     * @return A newly created view.
     */
    template<typename Type, typename... Other, typename... Exclude>
    [[nodiscard]] basic_view<get_t<storage_for_type<const Type>, storage_for_type<const Other>...>, exclude_t<storage_for_type<const Exclude>...>>
    view(exclude_t<Exclude...> = {}) const {
        return {assure<std::remove_const_t<Type>>(), assure<std::remove_const_t<Other>>()..., assure<std::remove_const_t<Exclude>>()...};
    }

    /*! @copydoc view */
    template<typename Type, typename... Other, typename... Exclude>
    [[nodiscard]] basic_view<get_t<storage_for_type<Type>, storage_for_type<Other>...>, exclude_t<storage_for_type<Exclude>...>>
    view(exclude_t<Exclude...> = {}) {
        return {assure<std::remove_const_t<Type>>(), assure<std::remove_const_t<Other>>()..., assure<std::remove_const_t<Exclude>>()...};
    }

    /**
     * @brief Returns a group for the given components.
     *
     * Groups are created on the fly and share with the registry its internal
     * data structures. Feel free to discard them after the use.<br/>
     * Creating and destroying a group is an incredibly cheap operation. As a
     * rule of thumb, storing a group should never be an option.
     *
     * Groups support exclusion lists and can own types of components. The more
     * types are owned by a group, the faster it is to iterate entities and
     * components.<br/>
     * However, groups also affect some features of the registry such as the
     * creation and destruction of components.
     *
     * @note
     * Pools of components that are owned by a group cannot be sorted anymore.
     * The group takes the ownership of the pools and arrange components so as
     * to iterate them as fast as possible.
     *
     * @tparam Owned Type of storage _owned_ by the group.
     * @tparam Get Type of storage _observed_ by the group.
     * @tparam Exclude Type of storage used to filter the group.
     * @return A newly created group.
     */
    template<typename... Owned, typename... Get, typename... Exclude>
    [[nodiscard]] basic_group<owned_t<storage_for_type<Owned>...>, get_t<storage_for_type<Get>...>, exclude_t<storage_for_type<Exclude>...>>
    group(get_t<Get...> = {}, exclude_t<Exclude...> = {}) {
        static_assert(sizeof...(Owned) + sizeof...(Get) > 0, "Exclusion-only groups are not supported");
        static_assert(sizeof...(Owned) + sizeof...(Get) + sizeof...(Exclude) > 1, "Single component groups are not allowed");

        using handler_type = group_handler<exclude_t<std::remove_const_t<Exclude>...>, get_t<std::remove_const_t<Get>...>, std::remove_const_t<Owned>...>;

        const auto cpools = std::forward_as_tuple(assure<std::remove_const_t<Owned>>()..., assure<std::remove_const_t<Get>>()...);
        constexpr auto size = sizeof...(Owned) + sizeof...(Get) + sizeof...(Exclude);
        handler_type *handler = nullptr;

        auto it = std::find_if(groups.cbegin(), groups.cend(), [size](const auto &gdata) {
            return gdata.size == size
                   && (gdata.owned(type_hash<std::remove_const_t<Owned>>::value()) && ...)
                   && (gdata.get(type_hash<std::remove_const_t<Get>>::value()) && ...)
                   && (gdata.exclude(type_hash<std::remove_const_t<Exclude>>::value()) && ...);
        });

        if(it != groups.cend()) {
            handler = static_cast<handler_type *>(it->group.get());
        } else {
            group_data candidate = {
                size,
                std::apply([this](auto &&...args) { return std::allocate_shared<handler_type>(get_allocator(), std::forward<decltype(args)>(args)...); }, entt::uses_allocator_construction_args<typename handler_type::value_type>(get_allocator())),
                []([[maybe_unused]] const id_type ctype) noexcept { return ((ctype == type_hash<std::remove_const_t<Owned>>::value()) || ...); },
                []([[maybe_unused]] const id_type ctype) noexcept { return ((ctype == type_hash<std::remove_const_t<Get>>::value()) || ...); },
                []([[maybe_unused]] const id_type ctype) noexcept { return ((ctype == type_hash<std::remove_const_t<Exclude>>::value()) || ...); },
            };

            handler = static_cast<handler_type *>(candidate.group.get());

            const void *maybe_valid_if = nullptr;
            const void *discard_if = nullptr;

            if constexpr(sizeof...(Owned) == 0) {
                groups.push_back(std::move(candidate));
            } else {
                [[maybe_unused]] auto has_conflict = [size](const auto &gdata) {
                    const auto overlapping = (0u + ... + gdata.owned(type_hash<std::remove_const_t<Owned>>::value()));
                    const auto sz = overlapping + (0u + ... + gdata.get(type_hash<std::remove_const_t<Get>>::value())) + (0u + ... + gdata.exclude(type_hash<std::remove_const_t<Exclude>>::value()));
                    return !overlapping || ((sz == size) || (sz == gdata.size));
                };

                ENTT_ASSERT(std::all_of(groups.cbegin(), groups.cend(), std::move(has_conflict)), "Conflicting groups");

                const auto next = std::find_if_not(groups.cbegin(), groups.cend(), [size](const auto &gdata) {
                    return !(0u + ... + gdata.owned(type_hash<std::remove_const_t<Owned>>::value())) || (size > gdata.size);
                });

                const auto prev = std::find_if(std::make_reverse_iterator(next), groups.crend(), [](const auto &gdata) {
                    return (0u + ... + gdata.owned(type_hash<std::remove_const_t<Owned>>::value()));
                });

                maybe_valid_if = (next == groups.cend() ? maybe_valid_if : next->group.get());
                discard_if = (prev == groups.crend() ? discard_if : prev->group.get());
                groups.insert(next, std::move(candidate));
            }

            (on_construct<std::remove_const_t<Owned>>().before(maybe_valid_if).template connect<&handler_type::template maybe_valid_if<std::remove_const_t<Owned>>>(*handler), ...);
            (on_construct<std::remove_const_t<Get>>().before(maybe_valid_if).template connect<&handler_type::template maybe_valid_if<std::remove_const_t<Get>>>(*handler), ...);
            (on_destroy<std::remove_const_t<Exclude>>().before(maybe_valid_if).template connect<&handler_type::template maybe_valid_if<std::remove_const_t<Exclude>>>(*handler), ...);

            (on_destroy<std::remove_const_t<Owned>>().before(discard_if).template connect<&handler_type::discard_if>(*handler), ...);
            (on_destroy<std::remove_const_t<Get>>().before(discard_if).template connect<&handler_type::discard_if>(*handler), ...);
            (on_construct<std::remove_const_t<Exclude>>().before(discard_if).template connect<&handler_type::discard_if>(*handler), ...);

            if constexpr(sizeof...(Owned) == 0) {
                for(const auto entity: view<Owned..., Get...>(exclude<Exclude...>)) {
                    handler->current.emplace(entity);
                }
            } else {
                // we cannot iterate backwards because we want to leave behind valid entities in case of owned types
                for(auto *first = std::get<0>(cpools).data(), *last = first + std::get<0>(cpools).size(); first != last; ++first) {
                    handler->template maybe_valid_if<type_list_element_t<0, type_list<std::remove_const_t<Owned>...>>>(*this, *first);
                }
            }
        }

        return {handler->current, std::get<storage_for_type<std::remove_const_t<Owned>> &>(cpools)..., std::get<storage_for_type<std::remove_const_t<Get>> &>(cpools)...};
    }

    /*! @copydoc group */
    template<typename... Owned, typename... Get, typename... Exclude>
    [[nodiscard]] basic_group<owned_t<storage_for_type<const Owned>...>, get_t<storage_for_type<const Get>...>, exclude_t<storage_for_type<const Exclude>...>>
    group_if_exists(get_t<Get...> = {}, exclude_t<Exclude...> = {}) const {
        auto it = std::find_if(groups.cbegin(), groups.cend(), [](const auto &gdata) {
            return gdata.size == (sizeof...(Owned) + sizeof...(Get) + sizeof...(Exclude))
                   && (gdata.owned(type_hash<std::remove_const_t<Owned>>::value()) && ...)
                   && (gdata.get(type_hash<std::remove_const_t<Get>>::value()) && ...)
                   && (gdata.exclude(type_hash<std::remove_const_t<Exclude>>::value()) && ...);
        });

        if(it == groups.cend()) {
            return {};
        } else {
            using handler_type = group_handler<exclude_t<std::remove_const_t<Exclude>...>, get_t<std::remove_const_t<Get>...>, std::remove_const_t<Owned>...>;
            return {static_cast<handler_type *>(it->group.get())->current, assure<std::remove_const_t<Owned>>()..., assure<std::remove_const_t<Get>>()...};
        }
    }

    /**
     * @brief Checks whether the given components belong to any group.
     * @tparam Component Types of components in which one is interested.
     * @return True if the pools of the given components are _free_, false
     * otherwise.
     */
    template<typename... Type>
    [[nodiscard]] bool owned() const {
        return std::any_of(groups.cbegin(), groups.cend(), [](auto &&gdata) { return (gdata.owned(type_hash<std::remove_const_t<Type>>::value()) || ...); });
    }

    /**
     * @brief Checks whether a group can be sorted.
     * @tparam Owned Type of storage _owned_ by the group.
     * @tparam Get Type of storage _observed_ by the group.
     * @tparam Exclude Type of storage used to filter the group.
     * @return True if the group can be sorted, false otherwise.
     */
    template<typename... Owned, typename... Get, typename... Exclude>
    [[nodiscard]] bool sortable(const basic_group<owned_t<Owned...>, get_t<Get...>, exclude_t<Exclude...>> &) noexcept {
        constexpr auto size = sizeof...(Owned) + sizeof...(Get) + sizeof...(Exclude);
        auto pred = [size](const auto &gdata) { return (0u + ... + gdata.owned(type_hash<typename Owned::value_type>::value())) && (size < gdata.size); };
        return std::find_if(groups.cbegin(), groups.cend(), std::move(pred)) == groups.cend();
    }

    /**
     * @brief Sorts the elements of a given component.
     *
     * The order remains valid until a component of the given type is assigned
     * to or removed from an entity.<br/>
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
     * Being `To` and `From` the two sets, after invoking this function an
     * iterator for `To` returns elements according to the following rules:
     *
     * * All entities in `To` that are also in `From` are returned first
     *   according to the order they have in `From`.
     * * All entities in `To` that are not in `From` are returned in no
     *   particular order after all the other entities.
     *
     * Any subsequent change to `From` won't affect the order in `To`.
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
        assure<To>().respect(assure<From>());
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
    entity_type free_list;
    std::vector<entity_type, allocator_type> epool;
    // std::shared_ptr because of its type erased allocator which is useful here
    dense_map<id_type, std::shared_ptr<base_type>, identity, std::equal_to<id_type>, typename alloc_traits::template rebind_alloc<std::pair<const id_type, std::shared_ptr<base_type>>>> pools;
    std::vector<group_data, typename alloc_traits::template rebind_alloc<group_data>> groups;
};

} // namespace entt

#endif

/////////////////// END OF external/entt/entity/registry.hpp ///////////////////

///////////////////// START OF src/Types/GameMechanic.hpp //////////////////////

#include <cstdint>

namespace pokesim {
enum GameMechanic : std::uint8_t {
  SCARLET_VIOLET,
};
}

////////////////////// END OF src/Types/GameMechanic.hpp ///////////////////////

//////////////////////// START OF src/Types/Gender.hpp /////////////////////////

#include <cstdint>

namespace pokesim::dex {
/** @brief Pokemon gender name */
enum Gender : std::uint8_t { NO_GENDER = 0, FEMALE, MALE, GENDER_TOTAL };
}  // namespace pokesim::dex

///////////////////////// END OF src/Types/Gender.hpp //////////////////////////

///////////////////////// START OF src/Types/Item.hpp //////////////////////////

#include <cstdint>

namespace pokesim::dex {
/** @brief Name of items in Pokemon games */
enum Item : std::uint16_t {
  // clang-format off
  NO_ITEM = 0, ABILITY_CAPSULE, ABILITY_PATCH, ABILITY_SHIELD, ABOMASITE, ABRA_CANDY, ABSOLITE, ABSORB_BULB, ACRO_BIKE, ADAMANT_CRYSTAL, ADAMANT_MINT, ADAMANT_ORB, ADRENALINE_ORB, ADVENTURE_GUIDE, AERODACTYLITE, AERODACTYL_CANDY, AGGRONITE, AGUAV_BERRY, AIR_BALLOON, AIR_MAIL, ALAKAZITE, ALORAICHIUM_Z, ALTARIANITE, AMAZE_MULCH, AMPHAROSITE, AMULET_COIN, ANTIDOTE, APICOT_BERRY, APRICORN, APRICORN_BOX, AQUA_SUIT, ARMOR_FOSSIL, ARMOR_PASS, ARMORITE_ORE, ARTICUNO_CANDY, ASPEAR_BERRY, ASSAULT_VEST, AUDINITE, AURORATICKET, AUSPICIOUS_ARMOR, AUTOGRAPH, AUX_EVASION, AUX_GUARD, AUX_POWER, AUX_POWERGUARD, AWAKENING, AZELFS_FANG, AZURE_FLUTE, BABIRI_BERRY, BALL_OF_MUD, BALM_MUSHROOM, BAND_AUTOGRAPH, BANETTITE, BASCULEGION_FOOD, BASEMENT_KEY, BEACH_GLASS, BEAD_MAIL, BEAN_CAKE, BEAST_BALL, BEEDRILLITE, BELLSPROUT_CANDY, BELUE_BERRY, BERRY, BERRY_JUICE, BERRY_POTS, BERRY_POUCH, BERRY_SWEET, BERSERK_GENE, BICYCLE, BIG_BAMBOO_SHOOT, BIG_MALASADA, BIG_MUSHROOM, BIG_NUGGET, BIG_PEARL, BIG_ROOT, BIKE_VOUCHER, BINDING_BAND, BITTER_BERRY, BLACK_APRICORN, BLACK_AUGURITE, BLACK_BELT, BLACK_FLUTE, BLACK_GLASSES, BLACK_MANE_HAIR, BLACK_SLUDGE, BLACK_TUMBLESTONE, BLANK_PLATE, BLASTOISINITE, BLAZIKENITE, BLOOM_MAIL, BLUE_APRICORN, BLUE_CARD, BLUE_FLUTE, BLUE_ORB, BLUE_PETAL, BLUE_SCARF, BLUE_SHARD, BLUESKY_MAIL, BLU_ID_BADGE, BLUK_BERRY, BLUNDER_POLICY, BOLD_MINT, BONSLY_CARD, BONSLY_PHOTO, BOOST_MULCH, BOOSTER_ENERGY, BOTTLE_CAP, BRAVE_MINT, BRICK_MAIL, BRICK_PIECE, BRIDGE_MAIL_D, BRIDGE_MAIL_M, BRIDGE_MAIL_S, BRIDGE_MAIL_T, BRIDGE_MAIL_V, BRIGHT_POWDER, BUBBLE_MAIL, BUG_GEM, BUG_MEMORY, BUG_TERA_SHARD, BUGINIUM_Z, BUGWORT, BULBASAUR_CANDY, BURN_DRIVE, BURN_HEAL, BURNT_BERRY, CAKE_LURE_BASE, CALCIUM, CALM_MINT, CAMERUPTITE, CAMPING_GEAR, CANDY_TRUFFLE, CARBOS, CARD_KEY, CAREFUL_MINT, CARROT_SEEDS, CASTELIACONE, CASTER_FERN, CATCHING_CHARM, CATERPIE_CANDY, CELESTICA_FLUTE, CELL_BATTERY, CHALKY_STONE, CHANSEY_CANDY, CHARCOAL, CHARIZARDITE_X, CHARIZARDITE_Y, CHARMANDER_CANDY, CHARTI_BERRY, CHERI_BERRY, CHERISH_BALL, CHESTO_BERRY, CHILAN_BERRY, CHILL_DRIVE, CHIPPED_POT, CHOICE_BAND, CHOICE_DUMPLING, CHOICE_SCARF, CHOICE_SPECS, CHOPLE_BERRY, CLAW_FOSSIL, CLEANSE_TAG, CLEAR_AMULET, CLEAR_BELL, CLEFAIRY_CANDY, CLEVER_FEATHER, CLOVER_SWEET, COBA_BERRY, COIN_CASE, COLBUR_BERRY, COLOGNE_CASE, COLRESS_MACHINE, COMET_SHARD, COMMON_STONE, CONTEST_COSTUME, CONTEST_PASS, CORNN_BERRY, COUPON_1, COUPON_2, COUPON_3, COURAGE_CANDY, COURAGE_CANDY_L, COURAGE_CANDY_XL, COVER_FOSSIL, COVERT_CLOAK, CRACKED_POT, CRAFTING_KIT, CROWN_PASS, CRUNCHY_SALT, CUBONE_CANDY, CRY_ANALYZER, CUSTAP_BERRY, DAMP_MULCH, DAMP_ROCK, DARK_GEM, DARK_MEMORY, DARK_STONE, DARK_TERA_SHARD, DARKINIUM_Z, DATA_CARDS, DATA_ROM, DAWN_STONE, DAZZLING_HONEY, D_DISK, DECIDIUM_Z, DEEP_SEA_SCALE, DEEP_SEA_TOOTH, DESTINY_KNOT, DEVON_PARTS, DEVON_SCOPE, DEVON_SCUBA_GEAR, DIANCITE, DIGGER_DRILL, DIGLETT_CANDY, DIRE_HIT, DIRESHROOM, DISC_CASE, DISCOUNT_COUPON, DISCOVERY_SLATE, DISTORTION_SLATE, DITTO_CANDY, DIVE_BALL, DNA_SAMPLE, DNA_SPLICERS, DODUO_CANDY, DOME_FOSSIL, DOPPEL_BONNETS, DOUSE_DRIVE, DOWN_ST_KEY, DOWSING_MACHINE, DRACO_PLATE, DRAGON_FANG, DRAGON_GEM, DRAGON_MEMORY, DRAGON_SCALE, DRAGON_SKULL, DRAGON_TERA_SHARD, DRAGONIUM_Z, DRASH_BERRY, DRATINI_CANDY, DREAD_PLATE, DREAM_BALL, DREAM_MAIL, DROPPED_ITEM, DROWZEE_CANDY, DS_SOUNDS, DUBIOUS_DISC, DURIN_BERRY, DUSK_BALL, DUSK_STONE, DYNAMAX_BAND, DYNAMAX_CANDY, DYNAMAX_CRYSTALS, DYNITE_ORE, EARTH_PLATE, EEVEE_CANDY, EEVIUM_Z, EGG_TICKET, EGGANT_BERRY, EIN_FILE_C, EIN_FILE_F, EIN_FILE_H, EIN_FILE_P, EIN_FILE_S, EJECT_BUTTON, EJECT_PACK, EKANS_CANDY, ELECTABUZZ_CANDY, ELECTIRIZER, ELECTRIC_GEM, ELECTRIC_MEMORY, ELECTRIC_SEED, ELECTRIC_TERA_SHARD, ELECTRIUM_Z, ELEVATOR_KEY, ELIXIR, ENDORSEMENT, ENERGY_POWDER, ENERGY_ROOT, ENIGMA_BERRY, ENIGMA_STONE, ENIGMATIC_CARD, EON_FLUTE, EON_MAIL, EON_TICKET, ESCAPE_ROPE, ETERNAL_ICE, ETHER, EVERSTONE, EVIOLITE, EXCITE_SCENT, EXEGGCUTE_CANDY, EXP_CANDY_L, EXP_CANDY_M, EXP_CANDY_S, EXP_CANDY_XL, EXP_CANDY_XS, EXP_CHARM, EXP_SHARE, EXPERT_BELT, EXPLORER_KIT, FAB_MAIL, FAIRIUM_Z, FAIRY_GEM, FAIRY_MEMORY, FAIRY_TERA_SHARD, FAME_CHECKER, FARFETCHD_CANDY, FASHION_CASE, FAST_BALL, FAVORED_MAIL, F_DISK, FEATHER_BALL, FESTIVAL_TICKET, FIGHTING_GEM, FIGHTING_MEMORY, FIGHTING_TERA_SHARD, FIGHTINIUM_Z, FIGY_BERRY, FINE_REMEDY, FIRE_GEM, FIRE_MEMORY, FIRE_STONE, FIRE_TERA_SHARD, FIRIUM_Z, FISHING_ROD, FIST_PLATE, FLAME_MAIL, FLAME_ORB, FLAME_PLATE, FLOAT_STONE, FLOWER_MAIL, FLOWER_SWEET, FLUFFY_TAIL, FLYING_GEM, FLYING_MEMORY, FLYING_TERA_SHARD, FLYINIUM_Z, FOCUS_BAND, FOCUS_SASH, FORAGE_BAG, FOREST_BALM, FOSSILIZED_BIRD, FOSSILIZED_DINO, FOSSILIZED_DRAKE, FOSSILIZED_FISH, FRESH_WATER, FRIEND_BALL, FULL_HEAL, FULL_INCENSE, FULL_RESTORE, GALACTIC_KEY, GALARICA_CUFF, GALARICA_TWIG, GALARICA_WREATH, GALLADITE, GANLON_BERRY, GARCHOMPITE, GARDEVOIRITE, GASTLY_CANDY, GB_SOUNDS, GEAR, GENGARITE, GENIUS_FEATHER, GENOME_SLATE, GENTLE_MINT, GEODUDE_CANDY, GHOST_GEM, GHOST_MEMORY, GHOST_TERA_SHARD, GHOSTIUM_Z, GIGATON_BALL, GINEMA_BERRY, GLALITITE, GLITTER_MAIL, GO_GOGGLES, GOD_STONE, GOLD_BERRY, GOLD_BOTTLE_CAP, GOLD_LEAF, GOLD_TEETH, GOLDEEN_CANDY, GONZAPS_KEY, GOLDEN_NANAB_BERRY, GOLDEN_PINAP_BERRY, GOLDEN_RAZZ_BERRY, GOOD_ROD, GOOEY_MULCH, GORGEOUS_BOX, GRACIDEA, GRAIN_CAKE, GRAM_1, GRAM_2, GRAM_3, GRASS_GEM, GRASS_MEMORY, GRASS_TERA_SHARD, GRASSIUM_Z, GRASS_MAIL, GRASSY_SEED, GREAT_BALL, GREEN_APRICORN, GREEN_PETAL, GREEN_SCARF, GREEN_SHARD, GREET_MAIL, GREPA_BERRY, GRIMER_CANDY, GRIP_CLAW, GRIT_DUST, GRIT_GRAVEL, GRIT_PEBBLE, GRIT_ROCK, GRISEOUS_CORE, GRISEOUS_ORB, GRN_ID_BADGE, GROUND_GEM, GROUND_MEMORY, GROUND_TERA_SHARD, GROUNDIUM_Z, GROWLITHE_CANDY, GROWTH_MULCH, GRUBBY_HANKY, GS_BALL, GUARD_SPEC, GUIDEBOOK, GYARADOSITE, HABAN_BERRY, HARBOR_MAIL, HARD_STONE, HASTY_MINT, HEAL_BALL, HEAL_POWDER, HEALTH_CANDY, HEALTH_CANDY_L, HEALTH_CANDY_XL, HEALTH_FEATHER, HEART_MAIL, HEART_SCALE, HEARTY_GRAINS, HEAT_ROCK, HEAVY_BALL, HEAVY_DUTY_BOOTS, HELIX_FOSSIL, HERACRONITE, HI_TECH_EARBUDS, HITMONCHAN_CANDY, HITMONLEE_CANDY, HM01, HM02, HM03, HM04, HM05, HM06, HM07, HM08, HOLO_CASTER, HOMETOWN_MUFFIN, HONDEW_BERRY, HONEY, HONEY_CAKE, HONOR_OF_KALOS, HOPO_BERRY, HORSEA_CANDY, HOUNDOOMINITE, HP_UP, HYPER_POTION, IAPAPA_BERRY, ICE_BERRY, ICE_GEM, ICE_HEAL, ICE_MEMORY, ICE_STONE, ICE_TERA_SHARD, ICEROOT_CARROT, ICICLE_PLATE, ICIUM_Z, ICY_ROCK, ID_CARD, ILIMAS_NORMALIUM_Z, IMPISH_MINT, INCINIUM_Z, INQUIRY_MAIL, INSECT_PLATE, INTRIGUING_STONE, IRON, IRON_BALL, IRON_BARKTONGUE, IRON_CHUNK, IRON_PLATE, JABOCA_BERRY, JADE_ORB, JAIL_KEY, JAW_FOSSIL, JET_BALL, JIGGLYPUFF_CANDY, JOHTO_SLATE, JOLLY_MINT, JOY_SCENT, JUBILIFE_MUFFIN, JYNX_CANDY, KABUTO_CANDY, KANGASKHAN_CANDY, KANGASKHANITE, KANTO_SLATE, KASIB_BERRY, KEBIA_BERRY, KEE_BERRY, KELPSY_BERRY, KEY_STONE, KEY_TO_ROOM_1, KEY_TO_ROOM_2, KEY_TO_ROOM_4, KEY_TO_ROOM_6, KINGS_LEAF, KINGS_ROCK, KOFFING_CANDY, KOFUS_BOOK, KOMMONIUM_Z, KORAIDONS_POKE_BALL, KRABBY_CANDY, KRANE_MEMO_1, KRANE_MEMO_2, KRANE_MEMO_3, KRANE_MEMO_4, KRANE_MEMO_5, KUO_BERRY, LAGGING_TAIL, LANSAT_BERRY, LAPRAS_CANDY, LATIASITE, LATIOSITE, LAVA_COOKIE, LAX_INCENSE, LAX_MINT, L_DISK, LEADEN_BALL, LEADERS_CREST, LEAF_LETTER, LEAF_STONE, LEEK, LEFT_POKE_BALL, LEFTOVERS, LEGEND_PLATE, LEGENDARY_CLUE_1, LEGENDARY_CLUE_2, LEGENDARY_CLUE_3, LEGENDARY_CLUE, LEMONADE, LENS_CASE, LEPPA_BERRY, LETTER, LEVEL_BALL, LIBERTY_PASS, LICKITUNG_CANDY, LIECHI_BERRY, LIFE_ORB, LIFT_KEY, LIGHT_BALL, LIGHT_CLAY, LIGHT_STONE, LIKE_MAIL, LINKING_CORD, LITEBLUEMAIL, LOADED_DICE, LOCK_CAPSULE, LONE_EARRING, LONELY_MINT, LOOKER_TICKET, LOOT_SACK, LOPUNNITE, LOST_ITEM, LOST_SATCHEL, LOVE_BALL, LOVE_SWEET, LOVELY_MAIL, LUCARIONITE, LUCK_INCENSE, LUCKY_EGG, LUCKY_PUNCH, LUM_BERRY, LUMINOUS_MOSS, LUMIOSE_GALETTE, LUNALIUM_Z, LUNAR_FEATHER, LURE, LURE_BALL, LUSTROUS_GLOBE, LUSTROUS_ORB, LUXURY_BALL, LYCANIUM_Z, MACH_BIKE, MACHINE_PART, MACHO_BRACE, MACHOP_CANDY, MAGIKARP_CANDY, MAGMA_EMBLEM, MAGMA_STONE, MAGMA_SUIT, MAGMAR_CANDY, MAGMARIZER, MAGNEMITE_CANDY, MAGNET, MAGO_BERRY, MAGOST_BERRY, MAINGATE_KEY, MAKEUP_BAG, MALICIOUS_ARMOR, MANECTITE, MANKEY_CANDY, MARANGA_BERRY, MARBLE, MARK_CHARM, MARSHADIUM_Z, MARSH_BALM, MASTER_BALL, MAWILITE, MAX_ELIXIR, MAX_ETHER, MAX_LURE, MAX_HONEY, MAX_MUSHROOMS, MAX_POTION, MAX_REPEL, MAX_REVIVE, MAYORS_NOTE, MEADOW_PLATE, MECH_MAIL, MECHANICAL_BOX, MECHANICAL_CABINET, MECHANICAL_CIRCULAR_SAW, MECHANICAL_PINWHEEL, MECHANICAL_TUB, MEDAL_BOX, MEDICHAMITE, MEDICINAL_LEEK, MEGA_BRACELET, MEGA_RING, MELTAN_CANDY, MEMBER_CARD, MENTAL_HERB, MEOWTH_CANDY, MESPRITS_PLUME, METAGROSSITE, METAL_COAT, METAL_POWDER, METEORITE, METEORITE_SHARD, METRONOME_ITEM, MEW_CANDY, MEWNIUM_Z, MEWTWO_CANDY, MEWTWONITE_X, MEWTWONITE_Y, MICLE_BERRY, MIGHTY_CANDY, MIGHTY_CANDY_L, MIGHTY_CANDY_XL, MILD_MINT, MIMIKIUM_Z, MIND_PLATE, MINT_BERRY, MIRACLEBERRY, MIRACLE_SEED, MIRAGE_MAIL, MIRAIDONS_POKE_BALL, MIROR_RADAR, MIRROR_HERB, MISTY_SEED, MODEST_MINT, MOLTRES_CANDY, MOOMOO_MILK, MOON_BALL, MOON_FLUTE, MOON_SHARD, MOON_STONE, MORPH_MAIL, MOSAIC_MAIL, MOUNTAIN_BALM, MR_MIME_CANDY, MUSCLE_BAND, MUSCLE_FEATHER, MUSHROOM_CAKE, MUSIC_DISC, MUSIC_MAIL, MYSTERIOUS_BALM, MYSTERIOUS_SHARD_S, MYSTERIOUS_SHARD_L, MYSTERYBERRY, MYSTERY_EGG, MYSTIC_WATER, MYSTICTICKET, NAIVE_MINT, NANAB_BERRY, NAUGHTY_MINT, NEST_BALL, NET_BALL, NEVER_MELT_ICE, NIDORAN_MALE_CANDY, NIDORAN_FEMALE_CANDY, NINIKU_BERRY, N_LUNARIZER, NOMEL_BERRY, NORMAL_BOX, NORMAL_GEM, NORMAL_TERA_SHARD, NORMALIUM_Z, N_SOLARIZER, NUGGET, NUTPEA_BERRY, OAKS_LETTER, OAKS_PARCEL, OCCA_BERRY, OCEANIC_SLATE, ODD_INCENSE, ODD_KEYSTONE, ODDISH_CANDY, OLD_AMBER, OLD_CHARM, OLD_GATEAU, OLD_JOURNAL, OLD_LETTER, OLD_ROD, OLD_SEA_MAP, OLD_VERSES, OMANYTE_CANDY, ONIX_CANDY, ORAN_BERRY, ORANGE_MAIL, ORANGE_PETAL, ORIGIN_BALL, ORIGIN_ORE, OVAL_CHARM, OVAL_STONE, PAIR_OF_TICKETS, PAL_PAD, PAMTRE_BERRY, PARALYZE_HEAL, PARAS_CANDY, PARCEL, PARK_BALL, PASS, PASS_ORB, PASSHO_BERRY, PAYAPA_BERRY, PEARL, PEARL_STRING, PEAT_BLOCK, PECHA_BERRY, PEP_UP_PLANT, PERMIT, PERSIM_BERRY, PETAYA_BERRY, PEWTER_CRUNCHIES, PICNIC_SET, PIDGEOTITE, PIDGEY_CANDY, PIKACHU_CANDY, PIKANIUM_Z, PIKASHUNIUM_Z, PINAP_BERRY, PINK_APRICORN, PINK_BOW, PINK_NECTAR, PINK_PETAL, PINK_SCARF, PINSIR_CANDY, PINSIRITE, PIXIE_PLATE, PLASMA_CARD, PLUME_FOSSIL, PLUMP_BEANS, POFFIN_CASE, POINT_CARD, POISON_BARB, POISON_GEM, POISON_MEMORY, POISON_TERA_SHARD, POISONIUM_Z, POKE_BALL, POKE_DOLL, POKE_FLUTE, POKE_RADAR, POKE_SNACK, POKE_TOY, POKEBLOCK_CASE, POKEBLOCK_KIT, POKEDEX, POKEMON_BOX_LINK, POKESHI_DOLL, POLKADOT_BOW, POLISHED_MUD_BALL, POLIWAG_CANDY, POMEG_BERRY, PONYTA_CANDY, POP_POD, PORTRAITMAIL, PORYGON_CANDY, POTION, POWER_ANKLET, POWER_BAND, POWER_BELT, POWER_BRACER, POWER_HERB, POWER_LENS, POWER_PLANT_PASS, POWER_WEIGHT, POWERUP_PART, POWDER_JAR, PP_MAX, PP_UP, PREMIER_BALL, PRETTY_FEATHER, PRIMARIUM_Z, PRISM_SCALE, PRISON_BOTTLE, PROFS_LETTER, PROFESSORS_MASK, PROP_CASE, PROTECTIVE_PADS, PROTECTOR, PROTEIN, PRZCUREBERRY, PSNCUREBERRY, PSYCHIC_GEM, PSYCHIC_MEMORY, PSYCHIC_SEED, PSYCHIC_TERA_SHARD, PSYCHIUM_Z, PSYDUCK_CANDY, PUMKIN_BERRY, PUNCHING_GLOVE, PURE_INCENSE, PURPLE_NECTAR, PURPLE_PETAL, QUALOT_BERRY, QUICK_BALL, QUICK_CANDY, QUICK_CANDY_L, QUICK_CANDY_XL, QUICK_CLAW, QUICK_POWDER, QUIET_MINT, RABUTA_BERRY, RADIANT_PETAL, RAGECANDYBAR, RAINBOW_FLOWER, RAINBOW_PASS, RAINBOW_SLATE, RAINBOW_WING, RARE_BONE, RARE_CANDY, RASH_MINT, RATTATA_CANDY, RAWST_BERRY, RAZOR_CLAW, RAZOR_FANG, RAZZ_BERRY, R_DISK, REAPER_CLOTH, RECIPES, RED_APRICORN, RED_CARD, RED_CHAIN, RED_FLUTE, RED_ID_BADGE, RED_NECTAR, RED_ORB, RED_PETAL, RED_SCALE, RED_SCARF, RED_SHARD, REINS_OF_UNITY, RELAXED_MINT, RELIC_BAND, RELIC_COPPER, RELIC_CROWN, RELIC_GOLD, RELIC_SILVER, RELIC_STATUE, RELIC_VASE, REMEDY, REPEAT_BALL, REPEL, REPLY_MAIL, RESIST_FEATHER, RETRO_MAIL, REVEAL_GLASS, REVIVAL_HERB, REVIVE, RHYHORN_CANDY, RIBBON_SWEET, RICH_MULCH, RIDE_PAGER, RINDO_BERRY, RING_TARGET, ROCK_GEM, ROCK_INCENSE, ROCK_MEMORY, ROCK_TERA_SHARD, ROCKIUM_Z, ROCKY_HELMET, ROLLER_SKATES, ROOM_SERVICE, ROOT_FOSSIL, ROSE_INCENSE, ROSELI_BERRY, ROTO_BARGAIN, ROTO_BOOST, ROTO_CATCH, ROTO_ENCOUNTER, ROTO_EXP_POINTS, ROTO_FRIENDSHIP, ROTO_HATCH, ROTO_HP_RESTORE, ROTO_PP_RESTORE, ROTO_PRIZE_MONEY, ROTO_STEALTH, ROTOM_BIKE, ROTOM_CATALOG, ROTOM_PHONE, ROWAP_BERRY, RSVP_MAIL, RUBY, RUNNING_SHOES, RUSTED_SHIELD, RUSTED_SWORD, S_S_TICKET, SABLENITE, SACHET, SACRED_ASH, SAFARI_BALL, SAFETY_GOGGLES, SAIL_FOSSIL, SALAC_BERRY, SALAMENCITE, SALT_CAKE, SAND_RADISH, SANDSHREW_CANDY, SANDWICH, SAPPHIRE, SASSY_MINT, SCANNER, SCARLET_BOOK, SCATTER_BANG, SCEPTILITE, SCIZORITE, SCOPE_LENS, SCROLL_OF_DARKNESS, SCROLL_OF_WATERS, SCYTHER_CANDY, SEA_INCENSE, SEAL_CASE, SECRET_KEY, SECRET_MEDICINE, SEED_OF_MASTERY, SEEL_CANDY, SERIOUS_MINT, SHADOW_MAIL, SHADEROOT_CARROT, SHALOUR_SABLE, SHARP_BEAK, SHARPEDONITE, SHED_SHELL, SHELL_BELL, SHELLDER_CANDY, SHINY_CHARM, SHINY_LEAF, SHINY_STONE, SHOAL_SALT, SHOAL_SHELL, SHOCK_DRIVE, SHUCA_BERRY, SILK_SCARF, SILPH_SCOPE, SILVER_LEAF, SILVER_NANAB_BERRY, SILVER_PINAP_BERRY, SILVER_POWDER, SILVER_RAZZ_BERRY, SILVER_WING, SITRUS_BERRY, SKULL_FOSSIL, SKY_PLATE, SKY_TUMBLESTONE, SLOWBRONITE, SLOWPOKE_CANDY, SLOWPOKE_TAIL, SMALL_BOUQUET, SMALL_TABLET, SMART_CANDY, SMART_CANDY_L, SMART_CANDY_XL, SMOKE_BALL, SMOKE_BOMB, SMOOTH_ROCK, SNORLAX_CANDY, SNORLIUM_Z, SNOWBALL, SNOW_BALM, SNOW_MAIL, SODA_POP, SOFT_SAND, SOLGANIUM_Z, SONIAS_BOOK, SOOT_SACK, SOOTFOOT_ROOT, SOOTHE_BELL, SOUL_DEW, SOUL_SLATE, SPACE_BALM, SPACE_MAIL, SPARKLING_STONE, SPEAROW_CANDY, SPELL_TAG, SPELON_BERRY, SPLASH_PLATE, SPOILED_APRICORN, SPOOKY_PLATE, SPORT_BALL, SPRAYDUCK, SPRINGY_MUSHROOM, SPRINKLOTAD, SQUALL_SLATE, SQUIRT_BOTTLE, SQUIRTLE_CANDY, STABLE_MULCH, STAR_PIECE, STAR_SWEET, STARDUST, STARF_BERRY, STARYU_CANDY, STEALTH_SPRAY, STEEL_GEM, STEEL_MAIL, STEEL_MEMORY, STEEL_TEETH, STEEL_TERA_SHARD, STEELIUM_Z, STEELIXITE, STICKY_BARB, STICKY_GLOB, STONE_PLATE, STORAGE_KEY, STRANGE_BALL, STRANGE_SOUVENIR, STRATOSPHERIC_SLATE, STRAWBERRY_SWEET, STRETCHY_SPRING, STRIB_BERRY, STYLE_CARD, SUBWAY_KEY, SUITE_KEY, SUN_FLUTE, SUN_SHARD, SUN_STONE, SUPER_LURE, SUPER_POTION, SUPER_REPEL, SUPER_ROD, SUPERB_REMEDY, SURFBOARD, SURF_MAIL, SURGE_BADGE, SURPRISE_MULCH, SURVIVAL_CHARM_B, SURVIVAL_CHARM_P, SURVIVAL_CHARM_R, SURVIVAL_CHARM_T, SURVIVAL_CHARM_Y, SWAMPERTITE, SWAP_SNACK, SWEET_APPLE, SWEET_HEART, SWIFT_FEATHER, SWORDCAP, SYSTEM_LEVER, TAMATO_BERRY, TANGA_BERRY, TANGELA_CANDY, TAPUNIUM_Z, TART_APPLE, TAUROS_CANDY, TEA, TEACHY_TV, TECTONIC_SLATE, TEMPTING_CHARM_B, TEMPTING_CHARM_P, TEMPTING_CHARM_R, TEMPTING_CHARM_T, TEMPTING_CHARM_Y, TENTACOOL_CANDY, TERA_ORB, TERRAIN_EXTENDER, TERU_SAMA, THANKS_MAIL, THICK_CLUB, THROAT_SPRAY, THUNDER_STONE, TIDAL_BELL, TIME_BALM, TIME_FLUTE, TIMER_BALL, TIMID_MINT, TINY_BAMBOO_SHOOT, TINY_MUSHROOM, TM01, TM02, TM03, TM04, TM05, TM06, TM07, TM08, TM09, TM10, TM11, TM12, TM13, TM14, TM15, TM16, TM17, TM18, TM19, TM20, TM21, TM22, TM23, TM24, TM25, TM26, TM27, TM28, TM29, TM30, TM31, TM32, TM33, TM34, TM35, TM36, TM37, TM38, TM39, TM40, TM41, TM42, TM43, TM44, TM45, TM46, TM47, TM48, TM49, TM50, TM51, TM52, TM53, TM54, TM55, TM56, TM57, TM58, TM59, TM60, TM61, TM62, TM63, TM64, TM65, TM66, TM67, TM68, TM69, TM70, TM71, TM72, TM73, TM74, TM75, TM76, TM77, TM78, TM79, TM80, TM81, TM82, TM83, TM84, TM85, TM86, TM87, TM88, TM89, TM90, TM91, TM92, TM93, TM94, TM95, TM96, TM97, TM98, TM99, TM_CASE, TM_MATERIALS, TR01, TR02, TR03, TR04, TR05, TR06, TR07, TR08, TR09, TR10, TR11, TR12, TR13, TR14, TR15, TR16, TR17, TR18, TR19, TR20, TR21, TR22, TR23, TR24, TR25, TR26, TR27, TR28, TR29, TR30, TR31, TR32, TR33, TR34, TR35, TR36, TR37, TR38, TR39, TR40, TR41, TR42, TR43, TR44, TR45, TR46, TR47, TR48, TR49, TR50, TR51, TR52, TR53, TR54, TR55, TR56, TR57, TR58, TR59, TR60, TR61, TR62, TR63, TR64, TR65, TR66, TR67, TR68, TR69, TR70, TR71, TR72, TR73, TR74, TR75, TR76, TR77, TR78, TR79, TR80, TR81, TR82, TR83, TR84, TR85, TR86, TR87, TR88, TR89, TR90, TR91, TR92, TR93, TR94, TR95, TR96, TR97, TR98, TR99, TMV_PASS, TOPO_BERRY, TORN_JOURNAL, TOUGA_BERRY, TOUGH_CANDY, TOUGH_CANDY_L, TOUGH_CANDY_XL, TOWN_MAP, TOXIC_ORB, TOXIC_PLATE, TRAVEL_TRUNK, TRI_PASS, TROPIC_MAIL, TROPICAL_SHELL, TUMBLESTONE, TUNNEL_MAIL, TWICE_SPICED_RADISH, TWISTED_SPOON, TYRANITARITE, U_DISK, ULTRA_BALL, ULTRANECROZIUM_Z, UNOWN_REPORT, UNUSUAL_SHOES, UPGRADE, UTILITY_UMBRELLA, UXIES_CLAW, VENONAT_CANDY, VENUSAURITE, VIOLET_BOOK, VIVICHOKE, VIVID_SCENT, VOICE_CASE_1, VOICE_CASE_2, VOICE_CASE_3, VOICE_CASE_4, VOICE_CASE_5, VOLCANO_BALM, VOLTORB_CANDY, VS_RECORDER, VS_SEEKER, VULPIX_CANDY, WACAN_BERRY, WAILMER_PAIL, WALL_FRAGMENT, WARDING_CHARM_B, WARDING_CHARM_P, WARDING_CHARM_R, WARDING_CHARM_T, WARDING_CHARM_Y, WATER_GEM, WATER_MEMORY, WATER_STONE, WATER_TERA_SHARD, WATERIUM_Z, WATMEL_BERRY, WAVE_INCENSE, WAVE_MAIL, WEAKNESS_POLICY, WEEDLE_CANDY, WEPEAR_BERRY, WHIPPED_DREAM, WHITE_APRICORN, WHITE_FLUTE, WHITE_HERB, WHITE_MANE_HAIR, WIDE_LENS, WIKI_BERRY, WING_BALL, WISE_GLASSES, WISHING_CHIP, WISHING_PIECE, WISHING_STAR, WOOD, WOOD_MAIL, WOODEN_CROWN, WORKS_KEY, X_ACCURACY, X_ATTACK, X_DEFENSE, X_SP_ATK, X_SP_DEF, X_SPEED, XTRANSCEIVER, YACHE_BERRY, YAGO_BERRY, YELLOW_APRICORN, YELLOW_FLUTE, YELLOW_NECTAR, YELLOW_PETAL, YELLOW_SCARF, YELLOW_SHARD, YLW_ID_BADGE, ZAP_PLATE, ZAPDOS_CANDY, ZINC, ZOOM_LENS, Z_POWER_RING, Z_RING, ZUBAT_CANDY, ZYGARDE_CUBE, ITEM_TOTAL
  // clang-format on
};
}  // namespace pokesim::dex

////////////////////////// END OF src/Types/Item.hpp ///////////////////////////

///////////////////////// START OF src/Types/Move.hpp //////////////////////////

#include <cstdint>

namespace pokesim::dex {
/** @brief Pokemon move name */
enum Move : std::uint16_t {
  // clang-format off
  NO_MOVE = 0, ONE_MILLION_VOLT_THUNDERBOLT, ABSORB, ACCELEROCK, ACID, ACID_ARMOR, ACID_DOWNPOUR, ACID_SPRAY, ACROBATICS, ACUPRESSURE, AERIAL_ACE, AEROBLAST, AFTER_YOU, AGILITY, AIR_CUTTER, AIR_SLASH, ALL_OUT_PUMMELING, ALLY_SWITCH, AMNESIA, ANCHOR_SHOT, ANCIENT_POWER, APPLE_ACID, AQUA_CUTTER, AQUA_JET, AQUA_RING, AQUA_STEP, AQUA_TAIL, ARMOR_CANNON, ARM_THRUST, AROMATHERAPY, AROMATIC_MIST, ASSIST, ASSURANCE, ASTONISH, ASTRAL_BARRAGE, ATTACK_ORDER, ATTRACT, AURA_SPHERE, AURA_WHEEL, AURORA_BEAM, AURORA_VEIL, AUTOTOMIZE, AVALANCHE, AXE_KICK, BABY_DOLL_EYES, BADDY_BAD, BANEFUL_BUNKER, BARB_BARRAGE, BARRAGE, BARRIER, BATON_PASS, BEAK_BLAST, BEAT_UP, BEHEMOTH_BASH, BEHEMOTH_BLADE, BELCH, BELLY_DRUM, BESTOW, BIDE, BIND, BITE, BITTER_BLADE, BITTER_MALICE, BLACK_HOLE_ECLIPSE, BLAST_BURN, BLAZE_KICK, BLAZING_TORQUE, BLEAKWIND_STORM, BLIZZARD, BLOCK, BLOOM_DOOM, BLUE_FLARE, BODY_PRESS, BODY_SLAM, BOLT_BEAK, BOLT_STRIKE, BONE_CLUB, BONEMERANG, BONE_RUSH, BOOMBURST, BOUNCE, BOUNCY_BUBBLE, BRANCH_POKE, BRAVE_BIRD, BREAKING_SWIPE, BREAKNECK_BLITZ, BRICK_BREAK, BRINE, BRUTAL_SWING, BUBBLE, BUBBLE_BEAM, BUG_BITE, BUG_BUZZ, BULK_UP, BULLDOZE, BULLET_PUNCH, BULLET_SEED, BURNING_JEALOUSY, BURN_UP, BUZZY_BUZZ, CALM_MIND, CAMOUFLAGE, CAPTIVATE, CATASTROPIKA, CEASELESS_EDGE, CELEBRATE, CHARGE, CHARGE_BEAM, CHARM, CHATTER, CHILLING_WATER, CHILLY_RECEPTION, CHIP_AWAY, CHLOROBLAST, CIRCLE_THROW, CLAMP, CLANGING_SCALES, CLANGOROUS_SOUL, CLANGOROUS_SOULBLAZE, CLEAR_SMOG, CLOSE_COMBAT, COACHING, COIL, COLLISION_COURSE, COMBAT_TORQUE, COMET_PUNCH, COMEUPPANCE, CONFIDE, CONFUSE_RAY, CONFUSION, CONSTRICT, CONTINENTAL_CRUSH, CONVERSION, CONVERSION_2, COPYCAT, CORE_ENFORCER, CORKSCREW_CRASH, CORROSIVE_GAS, COSMIC_POWER, COTTON_GUARD, COTTON_SPORE, COUNTER, COURT_CHANGE, COVET, CRABHAMMER, CRAFTY_SHIELD, CROSS_CHOP, CROSS_POISON, CRUNCH, CRUSH_CLAW, CRUSH_GRIP, CURSE, CUT, DARKEST_LARIAT, DARK_PULSE, DARK_VOID, DAZZLING_GLEAM, DECORATE, DEFEND_ORDER, DEFENSE_CURL, DEFOG, DESTINY_BOND, DETECT, DEVASTATING_DRAKE, DIAMOND_STORM, DIG, DISABLE, DISARMING_VOICE, DISCHARGE, DIRE_CLAW, DIVE, DIZZY_PUNCH, DOODLE, DOOM_DESIRE, DOUBLE_EDGE, DOUBLE_HIT, DOUBLE_IRON_BASH, DOUBLE_KICK, DOUBLE_SHOCK, DOUBLE_SLAP, DOUBLE_TEAM, DRACO_METEOR, DRAGON_ASCENT, DRAGON_BREATH, DRAGON_CLAW, DRAGON_DANCE, DRAGON_DARTS, DRAGON_ENERGY, DRAGON_HAMMER, DRAGON_PULSE, DRAGON_RAGE, DRAGON_RUSH, DRAGON_TAIL, DRAINING_KISS, DRAIN_PUNCH, DREAM_EATER, DRILL_PECK, DRILL_RUN, DRUM_BEATING, DUAL_CHOP, DUAL_WINGBEAT, DYNAMAX_CANNON, DYNAMIC_PUNCH, EARTH_POWER, EARTHQUAKE, ECHOED_VOICE, EERIE_IMPULSE, EERIE_SPELL, EGG_BOMB, ELECTRIC_TERRAIN, ELECTRIFY, ELECTRO_BALL, ELECTRO_DRIFT, ELECTROWEB, EMBARGO, EMBER, ENCORE, ENDEAVOR, ENDURE, ENERGY_BALL, ENTRAINMENT, ERUPTION, ESPER_WING, ETERNABEAM, EXPANDING_FORCE, EXPLOSION, EXTRASENSORY, EXTREME_EVOBOOST, EXTREME_SPEED, FACADE, FAIRY_LOCK, FAIRY_WIND, FAKE_OUT, FAKE_TEARS, FALSE_SURRENDER, FALSE_SWIPE, FEATHER_DANCE, FEINT, FEINT_ATTACK, FELL_STINGER, FIERY_DANCE, FIERY_WRATH, FILLET_AWAY, FINAL_GAMBIT, FIRE_BLAST, FIRE_FANG, FIRE_LASH, FIRE_PLEDGE, FIRE_PUNCH, FIRE_SPIN, FIRST_IMPRESSION, FISHIOUS_REND, FISSURE, FLAIL, FLAME_BURST, FLAME_CHARGE, FLAME_WHEEL, FLAMETHROWER, FLARE_BLITZ, FLASH, FLASH_CANNON, FLATTER, FLEUR_CANNON, FLING, FLIP_TURN, FLOATY_FALL, FLORAL_HEALING, FLOWER_SHIELD, FLOWER_TRICK, FLY, FLYING_PRESS, FOCUS_BLAST, FOCUS_ENERGY, FOCUS_PUNCH, FOLLOW_ME, FORCE_PALM, FORESIGHT, FORESTS_CURSE, FOUL_PLAY, FREEZE_DRY, FREEZE_SHOCK, FREEZING_GLARE, FREEZY_FROST, FRENZY_PLANT, FROST_BREATH, FRUSTRATION, FURY_ATTACK, FURY_CUTTER, FURY_SWIPES, FUSION_BOLT, FUSION_FLARE, FUTURE_SIGHT, GASTRO_ACID, GEAR_GRIND, GEAR_UP, GENESIS_SUPERNOVA, GEOMANCY, GIGA_DRAIN, GIGA_IMPACT, GIGATON_HAMMER, GIGAVOLT_HAVOC, GLACIAL_LANCE, GLACIATE, GLAIVE_RUSH, GLARE, GLITZY_GLOW, G_MAX_BEFUDDLE, G_MAX_CANNONADE, G_MAX_CENTIFERNO, G_MAX_CHI_STRIKE, G_MAX_CUDDLE, G_MAX_DEPLETION, G_MAX_DRUM_SOLO, G_MAX_FINALE, G_MAX_FIREBALL, G_MAX_FOAM_BURST, G_MAX_GOLD_RUSH, G_MAX_GRAVITAS, G_MAX_HYDROSNIPE, G_MAX_MALODOR, G_MAX_MELTDOWN, G_MAX_ONE_BLOW, G_MAX_RAPID_FLOW, G_MAX_REPLENISH, G_MAX_RESONANCE, G_MAX_SANDBLAST, G_MAX_SMITE, G_MAX_SNOOZE, G_MAX_STEELSURGE, G_MAX_STONESURGE, G_MAX_STUN_SHOCK, G_MAX_SWEETNESS, G_MAX_TARTNESS, G_MAX_TERROR, G_MAX_VINE_LASH, G_MAX_VOLCALITH, G_MAX_VOLT_CRASH, G_MAX_WILDFIRE, G_MAX_WIND_RAGE, GRASS_KNOT, GRASS_PLEDGE, GRASS_WHISTLE, GRASSY_GLIDE, GRASSY_TERRAIN, GRAV_APPLE, GRAVITY, GROWL, GROWTH, GRUDGE, GUARDIAN_OF_ALOLA, GUARD_SPLIT, GUARD_SWAP, GUILLOTINE, GUNK_SHOT, GUST, GYRO_BALL, HAIL, HAMMER_ARM, HAPPY_HOUR, HARDEN, HAZE, HEADBUTT, HEAD_CHARGE, HEADLONG_RUSH, HEAD_SMASH, HEAL_BELL, HEAL_BLOCK, HEALING_WISH, HEAL_ORDER, HEAL_PULSE, HEART_STAMP, HEART_SWAP, HEAT_CRASH, HEAT_WAVE, HEAVY_SLAM, HELPING_HAND, HEX, HIDDEN_POWER, HIDDEN_POWER_BUG, HIDDEN_POWER_DARK, HIDDEN_POWER_DRAGON, HIDDEN_POWER_ELECTRIC, HIDDEN_POWER_FIGHTING, HIDDEN_POWER_FIRE, HIDDEN_POWER_FLYING, HIDDEN_POWER_GHOST, HIDDEN_POWER_GRASS, HIDDEN_POWER_GROUND, HIDDEN_POWER_ICE, HIDDEN_POWER_POISON, HIDDEN_POWER_PSYCHIC, HIDDEN_POWER_ROCK, HIDDEN_POWER_STEEL, HIDDEN_POWER_WATER, HIGH_HORSEPOWER, HIGH_JUMP_KICK, HOLD_BACK, HOLD_HANDS, HONE_CLAWS, HORN_ATTACK, HORN_DRILL, HORN_LEECH, HOWL, HURRICANE, HYDRO_CANNON, HYDRO_PUMP, HYDRO_STEAM, HYDRO_VORTEX, HYPER_BEAM, HYPER_DRILL, HYPER_FANG, HYPERSPACE_FURY, HYPERSPACE_HOLE, HYPER_VOICE, HYPNOSIS, ICE_BALL, ICE_BEAM, ICE_BURN, ICE_FANG, ICE_HAMMER, ICE_PUNCH, ICE_SHARD, ICE_SPINNER, ICICLE_CRASH, ICICLE_SPEAR, ICY_WIND, IMPRISON, INCINERATE, INFERNAL_PARADE, INFERNO, INFERNO_OVERDRIVE, INFESTATION, INGRAIN, INSTRUCT, ION_DELUGE, IRON_DEFENSE, IRON_HEAD, IRON_TAIL, JAW_LOCK, JET_PUNCH, JUDGMENT, JUMP_KICK, JUNGLE_HEALING, KARATE_CHOP, KINESIS, KINGS_SHIELD, KNOCK_OFF, KOWTOW_CLEAVE, LANDS_WRATH, LASER_FOCUS, LASH_OUT, LAST_RESORT, LAST_RESPECTS, LAVA_PLUME, LEAFAGE, LEAF_BLADE, LEAF_STORM, LEAF_TORNADO, LEECH_LIFE, LEECH_SEED, LEER, LETS_SNUGGLE_FOREVER, LICK, LIFE_DEW, LIGHT_OF_RUIN, LIGHT_SCREEN, LIGHT_THAT_BURNS_THE_SKY, LIQUIDATION, LOCK_ON, LOVELY_KISS, LOW_KICK, LOW_SWEEP, LUCKY_CHANT, LUMINA_CRASH, LUNAR_BLESSING, LUNAR_DANCE, LUNGE, LUSTER_PURGE, MACH_PUNCH, MAGICAL_LEAF, MAGICAL_TORQUE, MAGIC_COAT, MAGIC_POWDER, MAGIC_ROOM, MAGMA_STORM, MAGNET_BOMB, MAGNETIC_FLUX, MAGNET_RISE, MAGNITUDE, MAKE_IT_RAIN, MALICIOUS_MOONSAULT, MAT_BLOCK, MAX_AIRSTREAM, MAX_DARKNESS, MAX_FLARE, MAX_FLUTTERBY, MAX_GEYSER, MAX_GUARD, MAX_HAILSTORM, MAX_KNUCKLE, MAX_LIGHTNING, MAX_MINDSTORM, MAX_OOZE, MAX_OVERGROWTH, MAX_PHANTASM, MAX_QUAKE, MAX_ROCKFALL, MAX_STARFALL, MAX_STEELSPIKE, MAX_STRIKE, MAX_WYRMWIND, MEAN_LOOK, MEDITATE, ME_FIRST, MEGA_DRAIN, MEGAHORN, MEGA_KICK, MEGA_PUNCH, MEMENTO, MENACING_MOONRAZE_MAELSTROM, METAL_BURST, METAL_CLAW, METAL_SOUND, METEOR_ASSAULT, METEOR_BEAM, METEOR_MASH, METRONOME, MILK_DRINK, MIMIC, MIND_BLOWN, MIND_READER, MINIMIZE, MIRACLE_EYE, MIRROR_COAT, MIRROR_MOVE, MIRROR_SHOT, MIST, MIST_BALL, MISTY_EXPLOSION, MISTY_TERRAIN, MOONBLAST, MOONGEIST_BEAM, MOONLIGHT, MORNING_SUN, MORTAL_SPIN, MOUNTAIN_GALE, MUD_BOMB, MUD_SHOT, MUD_SLAP, MUD_SPORT, MUDDY_WATER, MULTI_ATTACK, MYSTICAL_FIRE, MYSTICAL_POWER, NASTY_PLOT, NATURAL_GIFT, NATURE_POWER, NATURES_MADNESS, NEEDLE_ARM, NEVER_ENDING_NIGHTMARE, NIGHT_DAZE, NIGHTMARE, NIGHT_SHADE, NIGHT_SLASH, NOBLE_ROAR, NO_RETREAT, NOXIOUS_TORQUE, NUZZLE, OBLIVION_WING, OBSTRUCT, OCEANIC_OPERETTA, OCTAZOOKA, OCTOLOCK, ODOR_SLEUTH, OMINOUS_WIND, ORDER_UP, ORIGIN_PULSE, OUTRAGE, OVERDRIVE, OVERHEAT, PAIN_SPLIT, PARABOLIC_CHARGE, PARTING_SHOT, PAYBACK, PAY_DAY, PECK, PERISH_SONG, PETAL_BLIZZARD, PETAL_DANCE, PHANTOM_FORCE, PHOTON_GEYSER, PIKA_PAPOW, PIN_MISSILE, PLASMA_FISTS, PLAY_NICE, PLAY_ROUGH, PLUCK, POISON_FANG, POISON_GAS, POISON_JAB, POISON_POWDER, POISON_STING, POISON_TAIL, POLLEN_PUFF, POLTERGEIST, POPULATION_BOMB, POUNCE, POUND, POWDER, POWDER_SNOW, POWER_GEM, POWER_SHIFT, POWER_SPLIT, POWER_SWAP, POWER_TRICK, POWER_TRIP, POWER_UP_PUNCH, POWER_WHIP, PRECIPICE_BLADES, PRESENT, PRISMATIC_LASER, PROTECT, PSYBEAM, PSYBLADE, PSYCH_UP, PSYCHIC, PSYCHIC_FANGS, PSYCHIC_TERRAIN, PSYCHO_BOOST, PSYCHO_CUT, PSYCHO_SHIFT, PSYSHIELD_BASH, PSYSHOCK, PSYSTRIKE, PSYWAVE, PULVERIZING_PANCAKE, PUNISHMENT, PURIFY, PURSUIT, PYRO_BALL, QUASH, QUICK_ATTACK, QUICK_GUARD, QUIVER_DANCE, RAGE, RAGE_FIST, RAGE_POWDER, RAGING_BULL, RAGING_FURY, RAIN_DANCE, RAPID_SPIN, RAZOR_LEAF, RAZOR_SHELL, RAZOR_WIND, RECOVER, RECYCLE, REFLECT, REFLECT_TYPE, REFRESH, RELIC_SONG, REST, RETALIATE, RETURN, REVELATION_DANCE, REVENGE, REVERSAL, REVIVAL_BLESSING, RISING_VOLTAGE, ROAR, ROAR_OF_TIME, ROCK_BLAST, ROCK_CLIMB, ROCK_POLISH, ROCK_SLIDE, ROCK_SMASH, ROCK_THROW, ROCK_TOMB, ROCK_WRECKER, ROLE_PLAY, ROLLING_KICK, ROLLOUT, ROOST, ROTOTILLER, ROUND, RUINATION, SACRED_FIRE, SACRED_SWORD, SAFEGUARD, SALT_CURE, SAND_ATTACK, SANDSEAR_STORM, SANDSTORM, SAND_TOMB, SAPPY_SEED, SAVAGE_SPIN_OUT, SCALD, SCALE_SHOT, SCARY_FACE, SCORCHING_SANDS, SCRATCH, SCREECH, SEARING_SHOT, SEARING_SUNRAZE_SMASH, SECRET_POWER, SECRET_SWORD, SEED_BOMB, SEED_FLARE, SEISMIC_TOSS, SELF_DESTRUCT, SHADOW_BALL, SHADOW_BONE, SHADOW_CLAW, SHADOW_FORCE, SHADOW_PUNCH, SHADOW_SNEAK, SHARPEN, SHATTERED_PSYCHE, SHED_TAIL, SHEER_COLD, SHELL_SIDE_ARM, SHELL_SMASH, SHELL_TRAP, SHELTER, SHIFT_GEAR, SHOCK_WAVE, SHORE_UP, SIGNAL_BEAM, SILK_TRAP, SILVER_WIND, SIMPLE_BEAM, SING_MOVE /*Many math libraries define SING as a macro*/, SINISTER_ARROW_RAID, SIZZLY_SLIDE, SKETCH, SKILL_SWAP, SKITTER_SMACK, SKULL_BASH, SKY_ATTACK, SKY_DROP, SKY_UPPERCUT, SLACK_OFF, SLAM, SLASH, SLEEP_POWDER, SLEEP_TALK, SLUDGE, SLUDGE_BOMB, SLUDGE_WAVE, SMACK_DOWN, SMART_STRIKE, SMELLING_SALTS, SMOG, SMOKESCREEN, SNAP_TRAP, SNARL, SNATCH, SNIPE_SHOT, SNORE, SNOWSCAPE, SOAK, SOFT_BOILED, SOLAR_BEAM, SOLAR_BLADE, SONIC_BOOM, SOUL_STEALING_7_STAR_STRIKE, SPACIAL_REND, SPARK, SPARKLING_ARIA, SPARKLY_SWIRL, SPECTRAL_THIEF, SPEED_SWAP, SPICY_EXTRACT, SPIDER_WEB, SPIKE_CANNON, SPIKES, SPIKY_SHIELD, SPIN_OUT, SPIRIT_BREAK, SPIRIT_SHACKLE, SPIT_UP, SPITE, SPLASH, SPLINTERED_STORMSHARDS, SPLISHY_SPLASH, SPORE, SPOTLIGHT, SPRINGTIDE_STORM, STEALTH_ROCK, STEAM_ERUPTION, STEAMROLLER, STEEL_BEAM, STEEL_ROLLER, STEEL_WING, STICKY_WEB, STOCKPILE, STOKED_SPARKSURFER, STOMP, STOMPING_TANTRUM, STONE_AXE, STONE_EDGE, STORED_POWER, STORM_THROW, STRANGE_STEAM, STRENGTH, STRENGTH_SAP, STRING_SHOT, STRUGGLE, STRUGGLE_BUG, STUFF_CHEEKS, STUN_SPORE, SUBMISSION, SUBSTITUTE, SUBZERO_SLAMMER, SUCKER_PUNCH, SUNNY_DAY, SUNSTEEL_STRIKE, SUPER_FANG, SUPERPOWER, SUPERSONIC, SUPERSONIC_SKYSTRIKE, SURF, SURGING_STRIKES, SWAGGER, SWALLOW, SWEET_KISS, SWEET_SCENT, SWIFT, SWITCHEROO, SWORDS_DANCE, SYNCHRONOISE, SYNTHESIS, TACKLE, TAIL_GLOW, TAIL_SLAP, TAIL_WHIP, TAILWIND, TAKE_DOWN, TAKE_HEART, TAR_SHOT, TAUNT, TEARFUL_LOOK, TEATIME, TECHNO_BLAST, TECTONIC_RAGE, TEETER_DANCE, TELEKINESIS, TELEPORT, TERA_BLAST, TERRAIN_PULSE, THIEF, THOUSAND_ARROWS, THOUSAND_WAVES, THRASH, THROAT_CHOP, THUNDER, THUNDERBOLT, THUNDER_CAGE, THUNDER_FANG, THUNDEROUS_KICK, THUNDER_PUNCH, THUNDER_SHOCK, THUNDER_WAVE, TICKLE, TIDY_UP, TOPSY_TURVY, TORCH_SONG, TORMENT, TOXIC, TOXIC_SPIKES, TOXIC_THREAD, TRAILBLAZE, TRANSFORM, TRI_ATTACK, TRICK, TRICK_OR_TREAT, TRICK_ROOM, TRIPLE_ARROWS, TRIPLE_AXEL, TRIPLE_DIVE, TRIPLE_KICK, TROP_KICK, TRUMP_CARD, TWIN_BEAM, TWINEEDLE, TWINKLE_TACKLE, TWISTER, U_TURN, UPROAR, VACUUM_WAVE, V_CREATE, VEEVEE_VOLLEY, VENOM_DRENCH, VENOSHOCK, VICTORY_DANCE, VINE_WHIP, VISE_GRIP, VITAL_THROW, VOLT_SWITCH, VOLT_TACKLE, WAKE_UP_SLAP, WATERFALL, WATER_GUN, WATER_PLEDGE, WATER_PULSE, WATER_SHURIKEN, WATER_SPORT, WATER_SPOUT, WAVE_CRASH, WEATHER_BALL, WHIRLPOOL, WHIRLWIND, WICKED_BLOW, WICKED_TORQUE, WIDE_GUARD, WILDBOLT_STORM, WILD_CHARGE, WILL_O_WISP, WING_ATTACK, WISH, WITHDRAW, WONDER_ROOM, WOOD_HAMMER, WORK_UP, WORRY_SEED, WRAP, WRING_OUT, X_SCISSOR, YAWN, ZAP_CANNON, ZEN_HEADBUTT, ZING_ZAP, ZIPPY_ZAP, MOVE_TOTAL,
  // clang-format on
};
}  // namespace pokesim::dex

////////////////////////// END OF src/Types/Move.hpp ///////////////////////////

//////////////////////// START OF src/Types/Nature.hpp /////////////////////////

#include <cstdint>

namespace pokesim::dex {
/** @brief Pokemon nature name */
enum Nature : std::uint8_t {
  // clang-format off
  NO_NATURE = 0, ADAMANT, BASHFUL, BOLD, BRAVE, CALM, CAREFUL, DOCILE, GENTLE, HARDY, HASTY, IMPISH, JOLLY, LAX, LONELY, MILD, MODEST, NAIVE, NAUGHTY, QUIET, QUIRKY, RASH, RELAXED, SASSY, SERIOUS, TIMID, NATURE_TOTAL
  // clang-format on
};
}  // namespace pokesim::dex

///////////////////////// END OF src/Types/Nature.hpp //////////////////////////

///////////////////////// START OF src/Types/Stat.hpp //////////////////////////

#include <cstdint>

namespace pokesim::dex {
/** @brief Pokemon stat abbreviated name */
enum Stat : std::uint8_t {
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

////////////////////////// END OF src/Types/Stat.hpp ///////////////////////////

//////////////////////// START OF src/Types/Status.hpp /////////////////////////

#include <cstdint>

namespace pokesim::dex {
/** @brief Pokemon status condition name */
enum Status : std::uint8_t { NO_STATUS = 0, BRN, FRZ, PAR, PSN, SLP, TOX, /*, FRB, DRO, */ STATUS_TOTAL };
}  // namespace pokesim::dex

///////////////////////// END OF src/Types/Status.hpp //////////////////////////

/////////////////////////// START OF src/Dex/Dex.hpp ///////////////////////////

namespace pokesim {
class Dex {
 private:
  entt::registry registry{};

  entt::dense_map<dex::Species, entt::entity> speciesMap{};
  entt::dense_map<dex::Item, entt::entity> itemsMap{};
  entt::dense_map<dex::Move, entt::entity> movesMap{};

  inline static entt::entity (*getSpeciesBuild(dex::Species species))(Dex&);
  inline static entt::entity (*getMoveBuild(dex::Move move))(Dex&);
  inline static entt::entity (*getItemBuild(dex::Item item))(Dex&);

 public:
  const GameMechanic mechanics;

  Dex(GameMechanic mechanics_ = SCARLET_VIOLET) : mechanics(mechanics_) {}

  inline entt::handle createEntry();

  inline void loadSpecies(const entt::dense_set<dex::Species>& speciesList);

  template <typename... T>
  auto getSpeciesData(dex::Species species) const {
    return registry.get<T...>(speciesMap.at(species));
  }
};
}  // namespace pokesim

//////////////////////////// END OF src/Dex/Dex.hpp ////////////////////////////

/////////////////////// START OF src/Dex/Setup/Setup.hpp ///////////////////////

namespace pokesim::dex::internal {
struct DexDataSetup {
 protected:
  entt::handle handle;

 public:
  DexDataSetup(Dex& pokedex) : handle(pokedex.createEntry()) {}

  template <typename Tag>
  void setProperty() {
    handle.emplace<Tag>();
  }

  entt::entity entity() { return handle; }
};
}  // namespace pokesim::dex::internal

//////////////////////// END OF src/Dex/Setup/Setup.hpp ////////////////////////

/////////////////// START OF src/Dex/Setup/SpeciesSetup.hpp ////////////////////

namespace pokesim::dex::internal {
struct SpeciesSetup : DexDataSetup {
  SpeciesSetup(Dex& pokedex) : DexDataSetup(pokedex) {}

  inline void setName(Species species);
  inline void setType(Type type1, Type type2 = NO_TYPE);
  inline void setBaseStats(
    std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe);
};
}  // namespace pokesim::dex::internal

//////////////////// END OF src/Dex/Setup/SpeciesSetup.hpp /////////////////////

/////////////////// START OF src/Dex/Setup/SpeciesSetup.cpp ////////////////////

namespace pokesim::dex::internal {
void SpeciesSetup::setName(Species species) {
  handle.emplace<SpeciesName>(species);
}

void SpeciesSetup::setType(Type type1, Type type2) {
  handle.emplace<SpeciesTypes>(type1, type2);
}

void SpeciesSetup::setBaseStats(
  std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe) {
  handle.emplace<BaseStats>(hp, atk, def, spa, spd, spe);
}
}  // namespace pokesim::dex::internal

//////////////////// END OF src/Dex/Setup/SpeciesSetup.cpp /////////////////////

///////////////////// START OF src/Components/Accuracy.hpp /////////////////////

#include <cstdint>

namespace pokesim {
struct Accuracy {
  std::uint8_t accuracy;
};
}  // namespace pokesim

////////////////////// END OF src/Components/Accuracy.hpp //////////////////////

//////////////////// START OF src/Components/BasePower.hpp /////////////////////

#include <cstdint>

namespace pokesim {
struct BasePower {
  std::uint8_t basePower;
};
}  // namespace pokesim

///////////////////// END OF src/Components/BasePower.hpp //////////////////////

////////////////////// START OF src/Components/Boosts.hpp //////////////////////

#include <cstdint>

namespace pokesim {
struct AtkBoost {
  std::int8_t boost;
};

struct DefBoost {
  std::int8_t boost;
};

struct SpaBoost {
  std::int8_t boost;
};

struct SpdBoost {
  std::int8_t boost;
};

struct SpeBoost {
  std::int8_t boost;
};
}  // namespace pokesim

/////////////////////// END OF src/Components/Boosts.hpp ///////////////////////

////////////////////// START OF src/Components/Chance.hpp //////////////////////

#include <cstdint>

namespace pokesim {
struct Chance {
  std::uint8_t chance;
};
}  // namespace pokesim

/////////////////////// END OF src/Components/Chance.hpp ///////////////////////

///////////// START OF src/Components/EntityHolders/MoveEffect.hpp /////////////

namespace pokesim {
struct MoveEffect {
  bool primary = true;
  entt::entity moveEffect{};
};
}  // namespace pokesim

////////////// END OF src/Components/EntityHolders/MoveEffect.hpp //////////////

///////////////////// START OF src/Components/MultiHit.hpp /////////////////////

#include <cstdint>

namespace pokesim {
struct MultiHit {
  std::uint8_t minHits;
  std::uint8_t maxHits;
};
}  // namespace pokesim

////////////////////// END OF src/Components/MultiHit.hpp //////////////////////

//////////////////// START OF src/Components/Names/Move.hpp ////////////////////

namespace pokesim {
struct MoveName {
  dex::Move name = dex::NO_MOVE;
};
}  // namespace pokesim

///////////////////// END OF src/Components/Names/Move.hpp /////////////////////

//////////////////////// START OF src/Components/PP.hpp ////////////////////////

#include <cstdint>

namespace pokesim {
struct PP {
  std::uint8_t pp;
};

struct MaxPP {
  std::uint8_t maxPP;
};
}  // namespace pokesim

///////////////////////// END OF src/Components/PP.hpp /////////////////////////

///////////////////// START OF src/Components/Priority.hpp /////////////////////

#include <cstdint>

namespace pokesim {
struct Priority {
  std::uint8_t priority;
};
}  // namespace pokesim

////////////////////// END OF src/Components/Priority.hpp //////////////////////

//////////////////// START OF src/Components/Tags/Move.hpp /////////////////////

namespace pokesim::move {
// Categories

struct Physical {};
struct Special {};
struct Status {};

// Properties

struct Contact {};
struct BypassSubstitute {};
struct Punch {};

// Targets

struct AnySingleTarget {};
struct Self {};

namespace effect {
// Targets

struct MoveTarget {};
struct MoveSource {};
}  // namespace effect
}  // namespace pokesim::move

///////////////////// END OF src/Components/Tags/Move.hpp //////////////////////

///////////////////// START OF src/Dex/Setup/MoveSetup.hpp /////////////////////

namespace pokesim::dex::internal {
struct MoveSetup : DexDataSetup {
  MoveSetup(Dex& pokedex) : DexDataSetup(pokedex) {}

  inline void setName(Move move);
  inline void setType(Type type);
  inline void setAccuracy(std::uint8_t accuracy);
  inline void setBasePower(std::uint8_t basePower);

  inline void setCategoryPhysical();
  inline void setCategorySpecial();
  inline void setCategoryStatus();

  inline void setBasePP(std::uint8_t pp);
  inline void setPriority(std::uint8_t priority);
  inline void setMultiHit(std::uint8_t minHits, std::uint8_t maxHits);

  inline void setPrimaryEffect(entt::entity entity);
  inline void setSecondaryEffect(entt::entity entity);
};

struct MoveEffectSetup : DexDataSetup {
  MoveEffectSetup(Dex& pokedex) : DexDataSetup(pokedex) {}
  entt::entity entity() const { return handle; }

  inline void setChance(std::uint8_t chance);
  inline void setEffectsSelf();
  inline void setEffectsTarget();

  template <typename BoostType>
  inline void setBoost(std::int8_t boost);
};
}  // namespace pokesim::dex::internal

////////////////////// END OF src/Dex/Setup/MoveSetup.hpp //////////////////////

///////////////////// START OF src/Dex/Setup/MoveSetup.cpp /////////////////////

namespace pokesim::dex::internal {
void MoveSetup::setName(Move move) {
  handle.emplace<MoveName>(move);
}

void MoveSetup::setType(Type type) {
  handle.emplace<TypeName>(type);
}

void MoveSetup::setAccuracy(std::uint8_t accuracy) {
  handle.emplace<Accuracy>(accuracy);
}

void MoveSetup::setBasePower(std::uint8_t basePower) {
  handle.emplace<BasePower>(basePower);
}

void MoveSetup::setCategoryPhysical() {
  ENTT_ASSERT(!(handle.any_of<move::Special, move::Status>()), "A move can only have one category");
  handle.emplace<move::Physical>();
}

void MoveSetup::setCategorySpecial() {
  ENTT_ASSERT(!(handle.any_of<move::Physical, move::Status>()), "A move can only have one category");
  handle.emplace<move::Special>();
}

void MoveSetup::setCategoryStatus() {
  ENTT_ASSERT(!(handle.any_of<move::Physical, move::Special>()), "A move can only have one category");
  handle.emplace<move::Status>();
}

void MoveSetup::setBasePP(std::uint8_t pp) {
  handle.emplace<PP>(pp);
}

void MoveSetup::setPriority(std::uint8_t priority) {
  handle.emplace<Priority>(priority);
}

void MoveSetup::setMultiHit(std::uint8_t minHits, std::uint8_t maxHits) {
  handle.emplace<MultiHit>(minHits, maxHits);
}

void MoveSetup::setPrimaryEffect(entt::entity entity) {
  handle.emplace<MoveEffect>(true, entity);
}

void MoveSetup::setSecondaryEffect(entt::entity entity) {
  handle.emplace<MoveEffect>(false, entity);
}

void MoveEffectSetup::setChance(std::uint8_t chance) {
  handle.emplace<Chance>(chance);
}

void MoveEffectSetup::setEffectsSelf() {
  handle.emplace<move::effect::MoveSource>();
}

void MoveEffectSetup::setEffectsTarget() {
  handle.emplace<move::effect::MoveTarget>();
}

template <typename BoostType>
void MoveEffectSetup::setBoost(std::int8_t boost) {
  static_assert(
    std::is_same<AtkBoost, BoostType>() || std::is_same<DefBoost, BoostType>() || std::is_same<SpaBoost, BoostType>() ||
    std::is_same<SpdBoost, BoostType>() || std::is_same<SpeBoost, BoostType>());
  handle.emplace<BoostType>(boost);
}
}  // namespace pokesim::dex::internal

////////////////////// END OF src/Dex/Setup/MoveSetup.cpp //////////////////////

//////////////////// START OF src/Components/Names/Item.hpp ////////////////////

namespace pokesim {
struct ItemName {
  dex::Item name = dex::NO_ITEM;
};
}  // namespace pokesim

///////////////////// END OF src/Components/Names/Item.hpp /////////////////////

///////////////////// START OF src/Dex/Setup/ItemSetup.hpp /////////////////////

namespace pokesim::dex::internal {
struct ItemSetup : DexDataSetup {
  ItemSetup(Dex& pokedex) : DexDataSetup(pokedex) {}

  inline void setName(Item item);
};
}  // namespace pokesim::dex::internal

////////////////////// END OF src/Dex/Setup/ItemSetup.hpp //////////////////////

///////////////////// START OF src/Dex/Setup/ItemSetup.cpp /////////////////////

namespace pokesim::dex::internal {
void ItemSetup::setName(Item item) {
  handle.emplace<ItemName>(item);
}
}  // namespace pokesim::dex::internal

////////////////////// END OF src/Dex/Setup/ItemSetup.cpp //////////////////////

//////////////////// START OF src/Dex/Species/Ampharos.hpp /////////////////////

namespace pokesim::dex::build {
struct Ampharos {
  static const dex::Species name = dex::AMPHAROS;
  static const std::uint8_t hp = 90, atk = 75, def = 85, spa = 115, spd = 90, spe = 55;

  static entt::entity build(Dex& pokedex) {
    internal::SpeciesSetup species(pokedex);
    species.setName(name);
    species.setType(dex::ELECTRIC_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build

///////////////////// END OF src/Dex/Species/Ampharos.hpp //////////////////////

//////////////////// START OF src/Dex/Species/Dragapult.hpp ////////////////////

namespace pokesim::dex::build {
struct Dragapult {
  static const dex::Species name = dex::DRAGAPULT;
  static const std::uint8_t hp = 88, atk = 120, def = 75, spa = 100, spd = 75, spe = 142;

  static entt::entity build(Dex& pokedex) {
    internal::SpeciesSetup species(pokedex);
    species.setName(name);
    species.setType(dex::DRAGON_TYPE, dex::GHOST_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build

///////////////////// END OF src/Dex/Species/Dragapult.hpp /////////////////////

//////////////////// START OF src/Dex/Species/Empoleon.hpp /////////////////////

namespace pokesim::dex::build {
struct Empoleon {
  static const dex::Species name = dex::EMPOLEON;
  static const std::uint8_t hp = 84, atk = 86, def = 88, spa = 111, spd = 101, spe = 60;

  static entt::entity build(Dex& pokedex) {
    internal::SpeciesSetup species(pokedex);
    species.setName(name);
    species.setType(dex::WATER_TYPE, dex::STEEL_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build

///////////////////// END OF src/Dex/Species/Empoleon.hpp //////////////////////

//////////////////// START OF src/Dex/Species/Gardevoir.hpp ////////////////////

namespace pokesim::dex::build {
struct Gardevoir {
  static const dex::Species name = dex::GARDEVOIR;
  static const std::uint8_t hp = 68, atk = 65, def = 65, spa = 125, spd = 115, spe = 80;

  static entt::entity build(Dex& pokedex) {
    internal::SpeciesSetup species(pokedex);
    species.setName(name);
    species.setType(dex::PSYCHIC_TYPE, dex::FAIRY_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build

///////////////////// END OF src/Dex/Species/Gardevoir.hpp /////////////////////

///////////////////// START OF src/Dex/Species/Pangoro.hpp /////////////////////

namespace pokesim::dex::build {
struct Pangoro {
  static const dex::Species name = dex::PANGORO;
  static const std::uint8_t hp = 95, atk = 124, def = 78, spa = 69, spd = 71, spe = 58;

  static entt::entity build(Dex& pokedex) {
    internal::SpeciesSetup species(pokedex);
    species.setName(name);
    species.setType(dex::FIGHTING_TYPE, dex::DARK_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build

////////////////////// END OF src/Dex/Species/Pangoro.hpp //////////////////////

//////////////////// START OF src/Dex/Species/Ribombee.hpp /////////////////////

namespace pokesim::dex::build {
struct Ribombee {
  static const dex::Species name = dex::RIBOMBEE;
  static const std::uint8_t hp = 60, atk = 55, def = 60, spa = 95, spd = 70, spe = 124;

  static entt::entity build(Dex& pokedex) {
    internal::SpeciesSetup species(pokedex);
    species.setName(name);
    species.setType(dex::BUG_TYPE, dex::FAIRY_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build

///////////////////// END OF src/Dex/Species/Ribombee.hpp //////////////////////

////////////////// START OF src/Dex/Setup/GetSpeciesBuild.cpp //////////////////

// TODO(aed3): Make this and the individual species files auto generated

namespace pokesim {
entt::entity (*Dex::getSpeciesBuild(dex::Species species))(Dex&) {
  // Tidy check ignored because "using namespace" is in function
  using namespace dex::build;  // NOLINT(google-build-using-namespace)

  switch (species) {
    case Ampharos::name: return Ampharos::build;
    case Gardevoir::name: return Gardevoir::build;
    case Empoleon::name: return Empoleon::build;
    case Pangoro::name: return Pangoro::build;
    case Ribombee::name: return Ribombee::build;
    case Dragapult::name: return Dragapult::build;
    default: {
      ENTT_ASSERT(false, "Building a species that does not exist");
      return nullptr;
    }
  }
}
};  // namespace pokesim

/////////////////// END OF src/Dex/Setup/GetSpeciesBuild.cpp ///////////////////

//////////////////// START OF src/Dex/Items/AssaultVest.hpp ////////////////////

namespace pokesim::dex::build {
struct AssaultVest {
  static const dex::Item name = dex::ASSAULT_VEST;

  static entt::entity build(Dex& pokedex) {
    internal::ItemSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build

///////////////////// END OF src/Dex/Items/AssaultVest.hpp /////////////////////

/////////////////// START OF src/Dex/Items/BrightPowder.hpp ////////////////////

namespace pokesim::dex::build {
struct BrightPowder {
  static const dex::Item name = dex::BRIGHT_POWDER;

  static entt::entity build(Dex& pokedex) {
    internal::ItemSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build

//////////////////// END OF src/Dex/Items/BrightPowder.hpp /////////////////////

//////////////////// START OF src/Dex/Items/ChoiceScarf.hpp ////////////////////

namespace pokesim::dex::build {
struct ChoiceScarf {
  static const dex::Item name = dex::CHOICE_SCARF;

  static entt::entity build(Dex& pokedex) {
    internal::ItemSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build

///////////////////// END OF src/Dex/Items/ChoiceScarf.hpp /////////////////////

//////////////////// START OF src/Dex/Items/ChoiceSpecs.hpp ////////////////////

namespace pokesim::dex::build {
struct ChoiceSpecs {
  static const dex::Item name = dex::CHOICE_SPECS;

  static entt::entity build(Dex& pokedex) {
    internal::ItemSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build

///////////////////// END OF src/Dex/Items/ChoiceSpecs.hpp /////////////////////

///////////////////// START OF src/Dex/Items/FocusSash.hpp /////////////////////

namespace pokesim::dex::build {
struct FocusSash {
  static const dex::Item name = dex::FOCUS_SASH;

  static entt::entity build(Dex& pokedex) {
    internal::ItemSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build

////////////////////// END OF src/Dex/Items/FocusSash.hpp //////////////////////

////////////////////// START OF src/Dex/Items/LifeOrb.hpp //////////////////////

namespace pokesim::dex::build {
struct LifeOrb {
  static const dex::Item name = dex::LIFE_ORB;

  static entt::entity build(Dex& pokedex) {
    internal::ItemSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build

/////////////////////// END OF src/Dex/Items/LifeOrb.hpp ///////////////////////

/////////////////// START OF src/Dex/Setup/GetItemBuild.cpp ////////////////////

// TODO(aed3): Make this and the individual species files auto generated

namespace pokesim {
entt::entity (*Dex::getItemBuild(dex::Item item))(Dex&) {
  // Tidy check ignored because "using namespace" is in function
  using namespace dex::build;  // NOLINT(google-build-using-namespace)

  switch (item) {
    case BrightPowder::name: return BrightPowder::build;
    case LifeOrb::name: return LifeOrb::build;
    case FocusSash::name: return FocusSash::build;
    case ChoiceScarf::name: return ChoiceScarf::build;
    case ChoiceSpecs::name: return ChoiceSpecs::build;
    case AssaultVest::name: return AssaultVest::build;
    default: {
      ENTT_ASSERT(false, "Building a item that does not exist");
      return nullptr;
    }
  }
}
};  // namespace pokesim

//////////////////// END OF src/Dex/Setup/GetItemBuild.cpp /////////////////////

////////////////////////// START OF src/Dex/Names.hpp //////////////////////////

#include <array>
#include <cstdint>
#include <string>

namespace pokesim::dex {
/** @brief Returns the name of a species represented by its enum as a string. */
inline std::string toString(Species speciesEnum);

/** @brief Returns the name of a move represented by its enum as a string. */
inline std::string toString(Move moveEnum);

/** @brief Returns the name of an ability represented by its enum as a string. */
inline std::string toString(Ability abilityEnum);

/** @brief Returns the name of an item represented by its enum as a string. */
inline std::string toString(Item itemEnum);

/** @brief Returns the name of a nature represented by its enum as a string. */
inline std::string toString(Nature natureEnum);

/** @brief Returns the name of a status condition represented by its enum as a string. */
inline std::string toString(Status statusEnum);

/** @brief Returns the name of a gender represented by its enum as a string. */
inline std::string toString(Gender genderEnum);

/** @brief Returns the name of a type represented by its enum as a string. */
inline std::string toString(Type typeEnum);

/** @brief Returns the name of a stat represented by its enum as a string. */
inline std::string toString(Stat statEnum);

/** @brief Returns the Smogon name of a species represented by its enum as a string. */
inline std::string toSmogonString(Species speciesEnum);

/** @brief Returns the Smogon name of a move represented by its enum as a string. */
inline std::string toSmogonString(Move moveEnum);

/** @brief Returns the Smogon name of an ability represented by its enum as a string. */
inline std::string toSmogonString(Ability abilityEnum);

/** @brief Returns the Smogon name of an item represented by its enum as a string. */
inline std::string toSmogonString(Item itemEnum);

/** @brief Returns the Smogon ID of a species represented by its enum as a string. */
inline std::string toSmogonIdString(Species speciesEnum);

/** @brief Returns the Smogon ID of a move represented by its enum as a string. */
inline std::string toSmogonIdString(Move moveEnum);

/** @brief Returns the Smogon ID of an ability represented by its enum as a string. */
inline std::string toSmogonIdString(Ability abilityEnum);

/** @brief Returns the Smogon ID of an item represented by its enum as a string. */
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
 * @brief Returns the enum of the status condition name a string.
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

/** @brief Converts a string to only have lowercase alphanumeric characters */
inline std::string toID(const std::string& name);
}  // namespace pokesim::dex

/////////////////////////// END OF src/Dex/Names.hpp ///////////////////////////

////////////////////////// START OF src/Dex/Names.cpp //////////////////////////

namespace pokesim::dex {
namespace internal {
template <typename NameEnum, typename StringArray>
entt::dense_map<std::string, NameEnum> static createNameToEnumMap(const StringArray& array) {
  entt::dense_map<std::string, NameEnum> map;
  for (std::size_t i = 0; i < array.size(); i++) {
    map[array[i]] = static_cast<NameEnum>(i);
  }
  return map;
}

template <typename NameEnum>
NameEnum static fromStringToEnum(const entt::dense_map<std::string, NameEnum>& nameToEnum, const std::string& name) {
  auto foundPair = nameToEnum.find(name);
  if (foundPair == nameToEnum.end()) return static_cast<NameEnum>(0);
  return foundPair->second;
}

// clang-format off
/** @brief List where `internal::SpeciesNames[SPECIES]` contains the name of the species as a string */
static inline const std::array<std::string, SPECIES_TOTAL> SpeciesNames = {"", "Bulbasaur", "Ivysaur", "Venusaur", "Mega Venusaur", "Gigantamax Venusaur", "Charmander", "Charmeleon", "Charizard", "Mega Charizard X", "Mega Charizard Y", "Gigantamax Charizard", "Squirtle", "Wartortle", "Blastoise", "Mega Blastoise", "Gigantamax Blastoise", "Caterpie", "Metapod", "Butterfree", "Gigantamax Butterfree", "Weedle", "Kakuna", "Beedrill", "Mega Beedrill", "Pidgey", "Pidgeotto", "Pidgeot", "Mega Pidgeot", "Rattata", "Alolan Rattata", "Raticate", "Alolan Raticate", "Totem Alolan Raticate", "Spearow", "Fearow", "Ekans", "Arbok", "Pikachu", "Cosplay Pikachu", "Pikachu Rock-Star", "Pikachu Belle", "Pikachu Pop-Star", "Pikachu Ph.D", "Pikachu Libre", "Original Cap Pikachu", "Hoenn Cap Pikachu", "Sinnoh Cap Pikachu", "Unova Cap Pikachu", "Kalos Cap Pikachu", "Alola Cap Pikachu", "Partner Cap Pikachu", "Starter Pikachu", "Gigantamax Pikachu", "World Cap Pikachu", "Raichu", "Alolan Raichu", "Sandshrew", "Alolan Sandshrew", "Sandslash", "Alolan Sandslash", "Nidoran Female", "Nidorina", "Nidoqueen", "Nidoran Male", "Nidorino", "Nidoking", "Clefairy", "Clefable", "Vulpix", "Alolan Vulpix", "Ninetales", "Alolan Ninetales", "Jigglypuff", "Wigglytuff", "Zubat", "Golbat", "Oddish", "Gloom", "Vileplume", "Paras", "Parasect", "Venonat", "Venomoth", "Diglett", "Alolan Diglett", "Dugtrio", "Alolan Dugtrio", "Meowth", "Alolan Meowth", "Galarian Meowth", "Gigantamax Meowth", "Persian", "Alolan Persian", "Psyduck", "Golduck", "Mankey", "Primeape", "Hisuian Growlithe", "Growlithe", "Hisuian Arcanine", "Arcanine", "Poliwag", "Poliwhirl", "Poliwrath", "Abra", "Kadabra", "Alakazam", "Mega Alakazam", "Machop", "Machoke", "Machamp", "Gigantamax Machamp", "Bellsprout", "Weepinbell", "Victreebel", "Tentacool", "Tentacruel", "Geodude", "Alolan Geodude", "Graveler", "Alolan Graveler", "Golem", "Alolan Golem", "Ponyta", "Galarian Ponyta", "Rapidash", "Galarian Rapidash", "Slowpoke", "Galarian Slowpoke", "Slowbro", "Mega Slowbro", "Galarian Slowbro", "Magnemite", "Magneton", "Farfetch'd", "Galarian Farfetch'd", "Doduo", "Dodrio", "Seel", "Dewgong", "Grimer", "Alolan Grimer", "Muk", "Alolan Muk", "Shellder", "Cloyster", "Gastly", "Haunter", "Gengar", "Mega Gengar", "Gigantamax Gengar", "Onix", "Drowzee", "Hypno", "Krabby", "Kingler", "Gigantamax Kingler", "Voltorb", "Hisuian Voltorb", "Electrode", "Hisuian Electrode", "Exeggcute", "Exeggutor", "Alolan Exeggutor", "Cubone", "Marowak", "Alolan Marowak", "Totem Alolan Marowak", "Hitmonlee", "Hitmonchan", "Lickitung", "Koffing", "Weezing", "Galarian Weezing", "Rhyhorn", "Rhydon", "Chansey", "Tangela", "Kangaskhan", "Mega Kangaskhan", "Horsea", "Seadra", "Goldeen", "Seaking", "Staryu", "Starmie", "Mr. Mime", "Galarian Mr. Mime", "Scyther", "Jynx", "Electabuzz", "Magmar", "Pinsir", "Mega Pinsir", "Tauros", "Paldean Tauros Combat Bread", "Paldean Tauros Blaze Bread", "Paldean Tauros Aqua Bread", "Magikarp", "Gyarados", "Mega Gyarados", "Lapras", "Gigantamax Lapras", "Ditto", "Eevee", "Starter Eevee", "Gigantamax Eevee", "Vaporeon", "Jolteon", "Flareon", "Porygon", "Omanyte", "Omastar", "Kabuto", "Kabutops", "Aerodactyl", "Mega Aerodactyl", "Snorlax", "Gigantamax Snorlax", "Articuno", "Galarian Articuno", "Zapdos", "Galarian Zapdos", "Moltres", "Galarian Moltres", "Dratini", "Dragonair", "Dragonite", "Mewtwo", "Mega Mewtwo X", "Mega Mewtwo Y", "Mew", "Chikorita", "Bayleef", "Meganium", "Cyndaquil", "Quilava", "Hisuian Typhlosion", "Typhlosion", "Totodile", "Croconaw", "Feraligatr", "Sentret", "Furret", "Hoothoot", "Noctowl", "Ledyba", "Ledian", "Spinarak", "Ariados", "Crobat", "Chinchou", "Lanturn", "Pichu", "Spiky-eared Pichu", "Cleffa", "Igglybuff", "Togepi", "Togetic", "Natu", "Xatu", "Mareep", "Flaaffy", "Ampharos", "Mega Ampharos", "Bellossom", "Marill", "Azumarill", "Sudowoodo", "Politoed", "Hoppip", "Skiploom", "Jumpluff", "Aipom", "Sunkern", "Sunflora", "Yanma", "Wooper", "Paldean Wooper", "Quagsire", "Espeon", "Umbreon", "Murkrow", "Slowking", "Galarian Slowking", "Misdreavus", "Unown", "Unown A", "Unown B", "Unown C", "Unown D", "Unown E", "Unown F", "Unown G", "Unown H", "Unown I", "Unown J", "Unown K", "Unown L", "Unown M", "Unown N", "Unown O", "Unown P", "Unown Q", "Unown R", "Unown S", "Unown T", "Unown U", "Unown V", "Unown W", "Unown X", "Unown Y", "Unown Z", "Unown Exclamation", "Unown Question", "Wobbuffet", "Girafarig", "Pineco", "Forretress", "Dunsparce", "Gligar", "Steelix", "Mega Steelix", "Snubbull", "Granbull", "Qwilfish", "Hisuian Qwilfish", "Scizor", "Mega Scizor", "Shuckle", "Heracross", "Mega Heracross", "Hisuian Sneasel", "Sneasel", "Teddiursa", "Ursaring", "Slugma", "Magcargo", "Swinub", "Piloswine", "Corsola", "Galarian Corsola", "Remoraid", "Octillery", "Delibird", "Mantine", "Skarmory", "Houndour", "Houndoom", "Mega Houndoom", "Kingdra", "Phanpy", "Donphan", "Porygon2", "Stantler", "Smeargle", "Tyrogue", "Hitmontop", "Smoochum", "Elekid", "Magby", "Miltank", "Blissey", "Raikou", "Entei", "Suicune", "Larvitar", "Pupitar", "Tyranitar", "Mega Tyranitar", "Lugia", "Ho-Oh", "Celebi", "Treecko", "Grovyle", "Sceptile", "Mega Sceptile", "Torchic", "Combusken", "Blaziken", "Mega Blaziken", "Mudkip", "Marshtomp", "Swampert", "Mega Swampert", "Poochyena", "Mightyena", "Zigzagoon", "Galarian Zigzagoon", "Linoone", "Galarian Linoone", "Wurmple", "Silcoon", "Beautifly", "Cascoon", "Dustox", "Lotad", "Lombre", "Ludicolo", "Seedot", "Nuzleaf", "Shiftry", "Taillow", "Swellow", "Wingull", "Pelipper", "Ralts", "Kirlia", "Gardevoir", "Mega Gardevoir", "Surskit", "Masquerain", "Shroomish", "Breloom", "Slakoth", "Vigoroth", "Slaking", "Nincada", "Ninjask", "Shedinja", "Whismur", "Loudred", "Exploud", "Makuhita", "Hariyama", "Azurill", "Nosepass", "Skitty", "Delcatty", "Sableye", "Mega Sableye", "Mawile", "Mega Mawile", "Aron", "Lairon", "Aggron", "Mega Aggron", "Meditite", "Medicham", "Mega Medicham", "Electrike", "Manectric", "Mega Manectric", "Plusle", "Minun", "Volbeat", "Illumise", "Roselia", "Gulpin", "Swalot", "Carvanha", "Sharpedo", "Mega Sharpedo", "Wailmer", "Wailord", "Numel", "Camerupt", "Mega Camerupt", "Torkoal", "Spoink", "Grumpig", "Spinda", "Trapinch", "Vibrava", "Flygon", "Cacnea", "Cacturne", "Swablu", "Altaria", "Mega Altaria", "Zangoose", "Seviper", "Lunatone", "Solrock", "Barboach", "Whiscash", "Corphish", "Crawdaunt", "Baltoy", "Claydol", "Lileep", "Cradily", "Anorith", "Armaldo", "Feebas", "Milotic", "Castform", "Sunny Castform", "Rainy Castform", "Snowy Castform", "Kecleon", "Shuppet", "Banette", "Mega Banette", "Duskull", "Dusclops", "Tropius", "Chimecho", "Absol", "Mega Absol", "Wynaut", "Snorunt", "Glalie", "Mega Glalie", "Spheal", "Sealeo", "Walrein", "Clamperl", "Huntail", "Gorebyss", "Relicanth", "Luvdisc", "Bagon", "Shelgon", "Salamence", "Mega Salamence", "Beldum", "Metang", "Metagross", "Mega Metagross", "Regirock", "Regice", "Registeel", "Latias", "Mega Latias", "Latios", "Mega Latios", "Kyogre", "Primal Kyogre", "Groudon", "Primal Groudon", "Rayquaza", "Mega Rayquaza", "Jirachi", "Deoxys", "Attack Deoxys", "Defense Deoxys", "Speed Deoxys", "Turtwig", "Grotle", "Torterra", "Chimchar", "Monferno", "Infernape", "Piplup", "Prinplup", "Empoleon", "Starly", "Staravia", "Staraptor", "Bidoof", "Bibarel", "Kricketot", "Kricketune", "Shinx", "Luxio", "Luxray", "Budew", "Roserade", "Cranidos", "Rampardos", "Shieldon", "Bastiodon", "Burmy", "Plant Cloak Burmy", "Sandy Cloak Burmy", "Trash Cloak Burmy", "Plant Cloak Wormadam", "Sandy Cloak Wormadam", "Trash Cloak Wormadam", "Mothim", "Combee", "Vespiquen", "Pachirisu", "Buizel", "Floatzel", "Cherubi", "Cherrim", "Cherrim Overcast", "Cherrim Sunshine", "Shellos", "West Sea Shellos", "East Sea Shellos", "Gastrodon", "West Sea Gastrodon", "East Sea Gastrodon", "Ambipom", "Drifloon", "Drifblim", "Buneary", "Lopunny", "Mega Lopunny", "Mismagius", "Honchkrow", "Glameow", "Purugly", "Chingling", "Stunky", "Skuntank", "Bronzor", "Bronzong", "Bonsly", "Mime Jr.", "Happiny", "Chatot", "Spiritomb", "Gible", "Gabite", "Garchomp", "Mega Garchomp", "Munchlax", "Riolu", "Lucario", "Mega Lucario", "Hippopotas", "Hippowdon", "Skorupi", "Drapion", "Croagunk", "Toxicroak", "Carnivine", "Finneon", "Lumineon", "Mantyke", "Snover", "Abomasnow", "Mega Abomasnow", "Weavile", "Magnezone", "Lickilicky", "Rhyperior", "Tangrowth", "Electivire", "Magmortar", "Togekiss", "Yanmega", "Leafeon", "Glaceon", "Gliscor", "Mamoswine", "Porygon-Z", "Gallade", "Mega Gallade", "Probopass", "Dusknoir", "Froslass", "Rotom", "Heat Rotom", "Wash Rotom", "Frost Rotom", "Fan Rotom", "Mow Rotom", "Uxie", "Mesprit", "Azelf", "Dialga", "Dialga Origin", "Palkia", "Palkia Origin", "Heatran", "Regigigas", "Giratina Altered", "Giratina Origin", "Cresselia", "Phione", "Manaphy", "Darkrai", "Shaymin", "Shaymin Land", "Shaymin Sky", "Arceus", "Arceus Bug", "Arceus Dark", "Arceus Dragon", "Arceus Electric", "Arceus Fairy", "Arceus Fighting", "Arceus Fire", "Arceus Flying", "Arceus Ghost", "Arceus Grass", "Arceus Ground", "Arceus Ice", "Arceus Poison", "Arceus Psychic", "Arceus Rock", "Arceus Steel", "Arceus Water", "Victini", "Snivy", "Servine", "Serperior", "Tepig", "Pignite", "Emboar", "Oshawott", "Dewott", "Hisuian Samurott", "Samurott", "Patrat", "Watchog", "Lillipup", "Herdier", "Stoutland", "Purrloin", "Liepard", "Pansage", "Simisage", "Pansear", "Simisear", "Panpour", "Simipour", "Munna", "Musharna", "Pidove", "Tranquill", "Unfezant", "Blitzle", "Zebstrika", "Roggenrola", "Boldore", "Gigalith", "Woobat", "Swoobat", "Drilbur", "Excadrill", "Audino", "Mega Audino", "Timburr", "Gurdurr", "Conkeldurr", "Tympole", "Palpitoad", "Seismitoad", "Throh", "Sawk", "Sewaddle", "Swadloon", "Leavanny", "Venipede", "Whirlipede", "Scolipede", "Cottonee", "Whimsicott", "Petilil", "Lilligant", "Hisuian Lilligant", "Red-Striped Basculin", "Blue-Striped Basculin", "White-Striped Basculin", "Sandile", "Krokorok", "Krookodile", "Darumaka", "Galarian Darumaka", "Darmanitan", "Zen Mode Darmanitan", "Galarian Darmanitan", "Galarian Zen Mode Darmanitan", "Maractus", "Dwebble", "Crustle", "Scraggy", "Scrafty", "Sigilyph", "Yamask", "Galarian Yamask", "Cofagrigus", "Tirtouga", "Carracosta", "Archen", "Archeops", "Trubbish", "Garbodor", "Gigantamax Garbodor", "Zorua", "Hisuian Zorua", "Hisuian Zoroark", "Zoroark", "Minccino", "Cinccino", "Gothita", "Gothorita", "Gothitelle", "Solosis", "Duosion", "Reuniclus", "Ducklett", "Swanna", "Vanillite", "Vanillish", "Vanilluxe", "Deerling", "Deerling Spring", "Deerling Summer", "Deerling Autumn", "Deerling Winter", "Sawsbuck", "Sawsbuck Spring", "Sawsbuck Summer", "Sawsbuck Autumn", "Sawsbuck Winter", "Emolga", "Karrablast", "Escavalier", "Foongus", "Amoonguss", "Frillish", "Jellicent", "Alomomola", "Joltik", "Galvantula", "Ferroseed", "Ferrothorn", "Klink", "Klang", "Klinklang", "Tynamo", "Eelektrik", "Eelektross", "Elgyem", "Beheeyem", "Litwick", "Lampent", "Chandelure", "Axew", "Fraxure", "Haxorus", "Cubchoo", "Beartic", "Cryogonal", "Shelmet", "Accelgor", "Stunfisk", "Galarian Stunfisk", "Mienfoo", "Mienshao", "Druddigon", "Golett", "Golurk", "Pawniard", "Bisharp", "Bouffalant", "Rufflet", "Hisuian Braviary", "Braviary", "Vullaby", "Mandibuzz", "Heatmor", "Durant", "Deino", "Zweilous", "Hydreigon", "Larvesta", "Volcarona", "Cobalion", "Terrakion", "Virizion", "Incarnate Tornadus", "Tornadus Therian", "Incarnate Thundurus", "Thundurus Therian", "Reshiram", "Zekrom", "Incarnate Landorus", "Landorus Therian", "Kyurem", "Black Kyurem", "White Kyurem", "Keldeo", "Resolute Keldeo", "Aria Meloetta", "Pirouette Meloetta", "Genesect", "Douse Drive Genesect", "Shock Drive Genesect", "Burn Drive Genesect", "Chill Drive Genesect", "Chespin", "Quilladin", "Chesnaught", "Fennekin", "Braixen", "Delphox", "Froakie", "Frogadier", "Greninja", "Ash Greninja", "Bunnelby", "Diggersby", "Fletchling", "Fletchinder", "Talonflame", "Scatterbug", "Spewpa", "Vivillon", "Meadow Pattern Vivillon", "Archipelago Pattern Vivillon", "Continental Pattern Vivillon", "Elegant Pattern Vivillon", "Garden Pattern Vivillon", "High Plains Pattern Vivillon", "Icy Snow Pattern Vivillon", "Jungle Pattern Vivillon", "Marine Pattern Vivillon", "Modern Pattern Vivillon", "Monsoon Pattern Vivillon", "Ocean Pattern Vivillon", "Polar Pattern Vivillon", "River Pattern Vivillon", "Sandstorm Pattern Vivillon", "Savanna Pattern Vivillon", "Sun Pattern Vivillon", "Tundra Pattern Vivillon", "Fancy Pattern Vivillon", "Pokeball Pattern Vivillon", "Litleo", "Pyroar", "Flabébé", "Red Flower Flabébé", "Blue Flower Flabébé", "Orange Flower Flabébé", "White Flower Flabébé", "Yellow Flower Flabébé", "Floette", "Red Flower Floette", "Blue Flower Floette", "Orange Flower Floette", "White Flower Floette", "Yellow Flower Floette", "Eternal Flower Floette", "Florges", "Red Flower Florges", "Blue Flower Florges", "Orange Flower Florges", "White Flower Florges", "Yellow Flower Florges", "Skiddo", "Gogoat", "Pancham", "Pangoro", "Furfrou", "Natural Furfrou", "Dandy Trim Furfrou", "Debutante Trim Furfrou", "Diamond Trim Furfrou", "Heart Trim Furfrou", "Kabuki Trim Furfrou", "La Reine Trim Furfrou", "Matron Trim Furfrou", "Pharaoh Trim Furfrou", "Star Trim Furfrou", "Espurr", "Male Meowstic", "Female Meowstic", "Honedge", "Doublade", "Aegislash", "Shield Aegislash", "Blade Aegislash", "Spritzee", "Aromatisse", "Swirlix", "Slurpuff", "Inkay", "Malamar", "Binacle", "Barbaracle", "Skrelp", "Dragalge", "Clauncher", "Clawitzer", "Helioptile", "Heliolisk", "Tyrunt", "Tyrantrum", "Amaura", "Aurorus", "Sylveon", "Hawlucha", "Dedenne", "Carbink", "Goomy", "Hisuian Sliggoo", "Sliggoo", "Hisuian Goodra", "Goodra", "Klefki", "Phantump", "Trevenant", "Average Size Pumpkaboo", "Small Size Pumpkaboo", "Large Size Pumpkaboo", "Super Size Pumpkaboo", "Average Size Gourgeist", "Small Size Gourgeist", "Large Size Gourgeist", "Super Size Gourgeist", "Bergmite", "Avalugg", "Hisuian Avalugg", "Noibat", "Noivern", "Xerneas", "Yveltal", "Zygarde 50%", "Zygarde 10%", "Zygarde Complete", "Diancie", "Mega Diancie", "Hoopa Confined", "Hoopa Unbound", "Volcanion", "Rowlet", "Dartrix", "Hisuian Decidueye", "Decidueye", "Litten", "Torracat", "Incineroar", "Popplio", "Brionne", "Primarina", "Pikipek", "Trumbeak", "Toucannon", "Yungoos", "Gumshoos", "Totem Gumshoos", "Grubbin", "Charjabug", "Vikavolt", "Totem Vikavolt", "Crabrawler", "Crabominable", "Baile Style Oricorio", "Pom-Pom Style Oricorio", "Pa'u Style Oricorio", "Sensu Style Oricorio", "Cutiefly", "Ribombee", "Totem Ribombee", "Rockruff", "Midday Rockruff", "Midday Lycanroc", "Midnight Lycanroc", "Dusk Lycanroc", "Wishiwashi", "Solo Wishiwashi", "School Wishiwashi", "Mareanie", "Toxapex", "Mudbray", "Mudsdale", "Dewpider", "Araquanid", "Totem Araquanid", "Fomantis", "Lurantis", "Totem Lurantis", "Morelull", "Shiinotic", "Salandit", "Salazzle", "Totem Salazzle", "Stufful", "Bewear", "Bounsweet", "Steenee", "Tsareena", "Comfey", "Oranguru", "Passimian", "Wimpod", "Golisopod", "Sandygast", "Palossand", "Pyukumuku", "Type: Null", "Silvally", "Silvally Bug", "Silvally Dark", "Silvally Dragon", "Silvally Electric", "Silvally Fairy", "Silvally Fighting", "Silvally Fire", "Silvally Flying", "Silvally Ghost", "Silvally Grass", "Silvally Ground", "Silvally Ice", "Silvally Poison", "Silvally Psychic", "Silvally Rock", "Silvally Steel", "Silvally Water", "Minior", "Core Minior", "Red Core Minior", "Orange Core Minior", "Yellow Core Minior", "Green Core Minior", "Blue Core Minior", "Indigo Core Minior", "Violet Core Minior", "Meteor Minior", "Komala", "Turtonator", "Togedemaru", "Totem Togedemaru", "Mimikyu", "Mimikyu Busted", "Totem Mimikyu", "Busted Totem Mimikyu", "Bruxish", "Drampa", "Dhelmise", "Jangmo-o", "Hakamo-o", "Kommo-o", "Totem Kommo-o", "Tapu Koko", "Tapu Lele", "Tapu Bulu", "Tapu Fini", "Cosmog", "Cosmoem", "Solgaleo", "Lunala", "Nihilego", "Buzzwole", "Pheromosa", "Xurkitree", "Celesteela", "Kartana", "Guzzlord", "Necrozma", "Dusk-Mane Necrozma", "Dawn-Wings Necrozma", "Ultra Necrozma", "Magearna", "Original Color Magearna", "Marshadow", "Poipole", "Naganadel", "Stakataka", "Blacephalon", "Zeraora", "Meltan", "Melmetal", "Gigantamax Melmetal", "Grookey", "Thwackey", "Rillaboom", "Gigantamax Rillaboom", "Scorbunny", "Raboot", "Cinderace", "Gigantamax Cinderace", "Sobble", "Drizzile", "Inteleon", "Gigantamax Inteleon", "Skwovet", "Greedent", "Rookidee", "Corvisquire", "Corviknight", "Gigantamax Corviknight", "Blipbug", "Dottler", "Orbeetle", "Gigantamax Orbeetle", "Nickit", "Thievul", "Gossifleur", "Eldegoss", "Wooloo", "Dubwool", "Chewtle", "Drednaw", "Gigantamax Drednaw", "Yamper", "Boltund", "Rolycoly", "Carkol", "Coalossal", "Gigantamax Coalossal", "Applin", "Flapple", "Gigantamax Flapple", "Appletun", "Gigantamax Appletun", "Silicobra", "Sandaconda", "Gigantamax Sandaconda", "Cramorant", "Cramorant Gulping", "Cramorant Gorging", "Arrokuda", "Barraskewda", "Toxel", "Toxtricity Amped", "Toxtricity Low-Key", "Gigantamax Toxtricity Amped", "Gigantamax Toxtricity Low-Key", "Sizzlipede", "Centiskorch", "Gigantamax Centiskorch", "Clobbopus", "Grapploct", "Sinistea", "Antique Sinistea", "Polteageist", "Antique Polteageist", "Hatenna", "Hattrem", "Hatterene", "Gigantamax Hatterene", "Impidimp", "Morgrem", "Grimmsnarl", "Gigantamax Grimmsnarl", "Obstagoon", "Perrserker", "Cursola", "Sirfetch'd", "Mr. Rime", "Runerigus", "Milcery", "Alcremie", "Vanilla Cream Alcremie", "Ruby Cream Alcremie", "Matcha Cream Alcremie", "Mint Cream Alcremie", "Lemon Cream Alcremie", "Salted Cream Alcremie", "Ruby Swirl Alcremie", "Caramel Swirl Alcremie", "Rainbow Swirl Alcremie", "Gigantamax Alcremie", "Falinks", "Pincurchin", "Snom", "Frosmoth", "Stonjourner", "Eiscue", "Ice Face Eiscue", "Noice Face Eiscue", "Male Indeedee", "Female Indeedee", "Morpeko", "Full Belly Mode Morpeko", "Hangry Mode Morpeko", "Cufant", "Copperajah", "Gigantamax Copperajah", "Dracozolt", "Arctozolt", "Dracovish", "Arctovish", "Duraludon", "Gigantamax Duraludon", "Dreepy", "Drakloak", "Dragapult", "Hero of Many Battles Zacian", "Crowned Sword Zacian", "Hero of Many Battles Zamazenta", "Crowned Shield Zamazenta", "Eternatus", "Eternamax Eternatus", "Kubfu", "Single Strike Style Urshifu", "Rapid Strike Style Urshifu", "Gigantamax Single Strike Urshifu", "Gigantamax Rapid Strike Style Urshifu", "Zarude", "Dada Zarude", "Regieleki", "Regidrago", "Glastrier", "Spectrier", "Calyrex", "Ice Rider Calyrex", "Shadow Rider Calyrex", "Wyrdeer", "Kleavor", "Ursaluna", "Male Basculegion", "Female Basculegion", "Sneasler", "Overqwil", "Incarnate Enamorus", "Enamorus Therian", "Sprigatito", "Floragato", "Meowscarada", "Fuecoco", "Crocalor", "Skeledirge", "Quaxly", "Quaxwell", "Quaquaval", "Lechonk", "Male Oinkologne", "Female Oinkologne", "Tarountula", "Spidops", "Nymble", "Lokix", "Pawmi", "Pawmo", "Pawmot", "Tandemaus", "Maushold", "Family of Three Maushold", "Family of Four Maushold", "Fidough", "Dachsbun", "Smoliv", "Dolliv", "Arboliva", "Green Plumage Squawkabilly", "Blue Plumage Squawkabilly", "Yellow Plumage Squawkabilly", "White Plumage Squawkabilly", "Nacli", "Naclstack", "Garganacl", "Charcadet", "Armarouge", "Ceruledge", "Tadbulb", "Bellibolt", "Wattrel", "Kilowattrel", "Maschiff", "Mabosstiff", "Shroodle", "Grafaiai", "Bramblin", "Brambleghast", "Toedscool", "Toedscruel", "Klawf", "Capsakid", "Scovillain", "Rellor", "Rabsca", "Flittle", "Espathra", "Tinkatink", "Tinkatuff", "Tinkaton", "Wiglett", "Wugtrio", "Bombirdier", "Finizen", "Zero Palafin", "Hero Palafin", "Varoom", "Revavroom", "Cyclizar", "Orthworm", "Glimmet", "Glimmora", "Greavard", "Houndstone", "Flamigo", "Cetoddle", "Cetitan", "Veluza", "Dondozo", "Tatsugiri", "Curly Tatsugiri", "Droopy Tatsugiri", "Stretchy Tatsugiri", "Annihilape", "Clodsire", "Farigiraf", "Dudunsparce", "Two-Segment Dudunsparce", "Three-Segment Dudunsparce", "Kingambit", "Great Tusk", "Scream Tail", "Brute Bonnet", "Flutter Mane", "Slither Wing", "Sandy Shocks", "Iron Treads", "Iron Bundle", "Iron Hands", "Iron Jugulis", "Iron Moth", "Iron Thorns", "Frigibax", "Arctibax", "Baxcalibur", "Chest Gimmighoul", "Roaming Gimmighoul", "Gholdengo", "Wo-Chien", "Chien-Pao", "Ting-Lu", "Chi-Yu", "Roaring Moon", "Iron Valiant", "Koraidon", "Miraidon", "Walking Wake", "Iron Leaves"};

/** @brief List where `internal::MoveNames[MOVE]` contains the name of the move as a string */
static inline const std::array<std::string, MOVE_TOTAL> MoveNames = {"", "10,000,000 Volt Thunderbolt", "Absorb", "Accelerock", "Acid", "Acid Armor", "Acid Downpour", "Acid Spray", "Acrobatics", "Acupressure", "Aerial Ace", "Aeroblast", "After You", "Agility", "Air Cutter", "Air Slash", "All-Out Pummeling", "Ally Switch", "Amnesia", "Anchor Shot", "Ancient Power", "Apple Acid", "Aqua Cutter", "Aqua Jet", "Aqua Ring", "Aqua Step", "Aqua Tail", "Armor Cannon", "Arm Thrust", "Aromatherapy", "Aromatic Mist", "Assist", "Assurance", "Astonish", "Astral Barrage", "Attack Order", "Attract", "Aura Sphere", "Aura Wheel", "Aurora Beam", "Aurora Veil", "Autotomize", "Avalanche", "Axe Kick", "Baby-Doll Eyes", "Baddy Bad", "Baneful Bunker", "Barb Barrage", "Barrage", "Barrier", "Baton Pass", "Beak Blast", "Beat Up", "Behemoth Bash", "Behemoth Blade", "Belch", "Belly Drum", "Bestow", "Bide", "Bind", "Bite", "Bitter Blade", "Bitter Malice", "Black Hole Eclipse", "Blast Burn", "Blaze Kick", "Blazing Torque", "Bleakwind Storm", "Blizzard", "Block", "Bloom Doom", "Blue Flare", "Body Press", "Body Slam", "Bolt Beak", "Bolt Strike", "Bone Club", "Bonemerang", "Bone Rush", "Boomburst", "Bounce", "Bouncy Bubble", "Branch Poke", "Brave Bird", "Breaking Swipe", "Breakneck Blitz", "Brick Break", "Brine", "Brutal Swing", "Bubble", "Bubble Beam", "Bug Bite", "Bug Buzz", "Bulk Up", "Bulldoze", "Bullet Punch", "Bullet Seed", "Burning Jealousy", "Burn Up", "Buzzy Buzz", "Calm Mind", "Camouflage", "Captivate", "Catastropika", "Ceaseless Edge", "Celebrate", "Charge", "Charge Beam", "Charm", "Chatter", "Chilling Water", "Chilly Reception", "Chip Away", "Chloroblast", "Circle Throw", "Clamp", "Clanging Scales", "Clangorous Soul", "Clangorous Soulblaze", "Clear Smog", "Close Combat", "Coaching", "Coil", "Collision Course", "Combat Torque", "Comet Punch", "Comeuppance", "Confide", "Confuse Ray", "Confusion", "Constrict", "Continental Crush", "Conversion", "Conversion 2", "Copycat", "Core Enforcer", "Corkscrew Crash", "Corrosive Gas", "Cosmic Power", "Cotton Guard", "Cotton Spore", "Counter", "Court Change", "Covet", "Crabhammer", "Crafty Shield", "Cross Chop", "Cross Poison", "Crunch", "Crush Claw", "Crush Grip", "Curse", "Cut", "Darkest Lariat", "Dark Pulse", "Dark Void", "Dazzling Gleam", "Decorate", "Defend Order", "Defense Curl", "Defog", "Destiny Bond", "Detect", "Devastating Drake", "Diamond Storm", "Dig", "Disable", "Disarming Voice", "Discharge", "Dire Claw", "Dive", "Dizzy Punch", "Doodle", "Doom Desire", "Double-Edge", "Double Hit", "Double Iron Bash", "Double Kick", "Double Shock", "Double Slap", "Double Team", "Draco Meteor", "Dragon Ascent", "Dragon Breath", "Dragon Claw", "Dragon Dance", "Dragon Darts", "Dragon Energy", "Dragon Hammer", "Dragon Pulse", "Dragon Rage", "Dragon Rush", "Dragon Tail", "Draining Kiss", "Drain Punch", "Dream Eater", "Drill Peck", "Drill Run", "Drum Beating", "Dual Chop", "Dual Wingbeat", "Dynamax Cannon", "Dynamic Punch", "Earth Power", "Earthquake", "Echoed Voice", "Eerie Impulse", "Eerie Spell", "Egg Bomb", "Electric Terrain", "Electrify", "Electro Ball", "Electro Drift", "Electroweb", "Embargo", "Ember", "Encore", "Endeavor", "Endure", "Energy Ball", "Entrainment", "Eruption", "Esper Wing", "Eternabeam", "Expanding Force", "Explosion", "Extrasensory", "Extreme Evoboost", "Extreme Speed", "Facade", "Fairy Lock", "Fairy Wind", "Fake Out", "Fake Tears", "False Surrender", "False Swipe", "Feather Dance", "Feint", "Feint Attack", "Fell Stinger", "Fiery Dance", "Fiery Wrath", "Fillet Away", "Final Gambit", "Fire Blast", "Fire Fang", "Fire Lash", "Fire Pledge", "Fire Punch", "Fire Spin", "First Impression", "Fishious Rend", "Fissure", "Flail", "Flame Burst", "Flame Charge", "Flame Wheel", "Flamethrower", "Flare Blitz", "Flash", "Flash Cannon", "Flatter", "Fleur Cannon", "Fling", "Flip Turn", "Floaty Fall", "Floral Healing", "Flower Shield", "Flower Trick", "Fly", "Flying Press", "Focus Blast", "Focus Energy", "Focus Punch", "Follow Me", "Force Palm", "Foresight", "Forest's Curse", "Foul Play", "Freeze-Dry", "Freeze Shock", "Freezing Glare", "Freezy Frost", "Frenzy Plant", "Frost Breath", "Frustration", "Fury Attack", "Fury Cutter", "Fury Swipes", "Fusion Bolt", "Fusion Flare", "Future Sight", "Gastro Acid", "Gear Grind", "Gear Up", "Genesis Supernova", "Geomancy", "Giga Drain", "Giga Impact", "Gigaton Hammer", "Gigavolt Havoc", "Glacial Lance", "Glaciate", "Glaive Rush", "Glare", "Glitzy Glow", "G-Max Befuddle", "G-Max Cannonade", "G-Max Centiferno", "G-Max Chi Strike", "G-Max Cuddle", "G-Max Depletion", "G-Max Drum Solo", "G-Max Finale", "G-Max Fireball", "G-Max Foam Burst", "G-Max Gold Rush", "G-Max Gravitas", "G-Max Hydrosnipe", "G-Max Malodor", "G-Max Meltdown", "G-Max One Blow", "G-Max Rapid Flow", "G-Max Replenish", "G-Max Resonance", "G-Max Sandblast", "G-Max Smite", "G-Max Snooze", "G-Max Steelsurge", "G-Max Stonesurge", "G-Max Stun Shock", "G-Max Sweetness", "G-Max Tartness", "G-Max Terror", "G-Max Vine Lash", "G-Max Volcalith", "G-Max Volt Crash", "G-Max Wildfire", "G-Max Wind Rage", "Grass Knot", "Grass Pledge", "Grass Whistle", "Grassy Glide", "Grassy Terrain", "Grav Apple", "Gravity", "Growl", "Growth", "Grudge", "Guardian of Alola", "Guard Split", "Guard Swap", "Guillotine", "Gunk Shot", "Gust", "Gyro Ball", "Hail", "Hammer Arm", "Happy Hour", "Harden", "Haze", "Headbutt", "Head Charge", "Headlong Rush", "Head Smash", "Heal Bell", "Heal Block", "Healing Wish", "Heal Order", "Heal Pulse", "Heart Stamp", "Heart Swap", "Heat Crash", "Heat Wave", "Heavy Slam", "Helping Hand", "Hex", "Hidden Power", "Hidden Power Bug", "Hidden Power Dark", "Hidden Power Dragon", "Hidden Power Electric", "Hidden Power Fighting", "Hidden Power Fire", "Hidden Power Flying", "Hidden Power Ghost", "Hidden Power Grass", "Hidden Power Ground", "Hidden Power Ice", "Hidden Power Poison", "Hidden Power Psychic", "Hidden Power Rock", "Hidden Power Steel", "Hidden Power Water", "High Horsepower", "High Jump Kick", "Hold Back", "Hold Hands", "Hone Claws", "Horn Attack", "Horn Drill", "Horn Leech", "Howl", "Hurricane", "Hydro Cannon", "Hydro Pump", "Hydro Steam", "Hydro Vortex", "Hyper Beam", "Hyper Drill", "Hyper Fang", "Hyperspace Fury", "Hyperspace Hole", "Hyper Voice", "Hypnosis", "Ice Ball", "Ice Beam", "Ice Burn", "Ice Fang", "Ice Hammer", "Ice Punch", "Ice Shard", "Ice Spinner", "Icicle Crash", "Icicle Spear", "Icy Wind", "Imprison", "Incinerate", "Infernal Parade", "Inferno", "Inferno Overdrive", "Infestation", "Ingrain", "Instruct", "Ion Deluge", "Iron Defense", "Iron Head", "Iron Tail", "Jaw Lock", "Jet Punch", "Judgment", "Jump Kick", "Jungle Healing", "Karate Chop", "Kinesis", "King's Shield", "Knock Off", "Kowtow Cleave", "Land's Wrath", "Laser Focus", "Lash Out", "Last Resort", "Last Respects", "Lava Plume", "Leafage", "Leaf Blade", "Leaf Storm", "Leaf Tornado", "Leech Life", "Leech Seed", "Leer", "Let's Snuggle Forever", "Lick", "Life Dew", "Light of Ruin", "Light Screen", "Light That Burns the Sky", "Liquidation", "Lock-On", "Lovely Kiss", "Low Kick", "Low Sweep", "Lucky Chant", "Lumina Crash", "Lunar Blessing", "Lunar Dance", "Lunge", "Luster Purge", "Mach Punch", "Magical Leaf", "Magical Torque", "Magic Coat", "Magic Powder", "Magic Room", "Magma Storm", "Magnet Bomb", "Magnetic Flux", "Magnet Rise", "Magnitude", "Make It Rain", "Malicious Moonsault", "Mat Block", "Max Airstream", "Max Darkness", "Max Flare", "Max Flutterby", "Max Geyser", "Max Guard", "Max Hailstorm", "Max Knuckle", "Max Lightning", "Max Mindstorm", "Max Ooze", "Max Overgrowth", "Max Phantasm", "Max Quake", "Max Rockfall", "Max Starfall", "Max Steelspike", "Max Strike", "Max Wyrmwind", "Mean Look", "Meditate", "Me First", "Mega Drain", "Megahorn", "Mega Kick", "Mega Punch", "Memento", "Menacing Moonraze Maelstrom", "Metal Burst", "Metal Claw", "Metal Sound", "Meteor Assault", "Meteor Beam", "Meteor Mash", "Metronome", "Milk Drink", "Mimic", "Mind Blown", "Mind Reader", "Minimize", "Miracle Eye", "Mirror Coat", "Mirror Move", "Mirror Shot", "Mist", "Mist Ball", "Misty Explosion", "Misty Terrain", "Moonblast", "Moongeist Beam", "Moonlight", "Morning Sun", "Mortal Spin", "Mountain Gale", "Mud Bomb", "Mud Shot", "Mud-Slap", "Mud Sport", "Muddy Water", "Multi-Attack", "Mystical Fire", "Mystical Power", "Nasty Plot", "Natural Gift", "Nature Power", "Nature's Madness", "Needle Arm", "Never-Ending Nightmare", "Night Daze", "Nightmare", "Night Shade", "Night Slash", "Noble Roar", "No Retreat", "Noxious Torque", "Nuzzle", "Oblivion Wing", "Obstruct", "Oceanic Operetta", "Octazooka", "Octolock", "Odor Sleuth", "Ominous Wind", "Order Up", "Origin Pulse", "Outrage", "Overdrive", "Overheat", "Pain Split", "Parabolic Charge", "Parting Shot", "Payback", "Pay Day", "Peck", "Perish Song", "Petal Blizzard", "Petal Dance", "Phantom Force", "Photon Geyser", "Pika Papow", "Pin Missile", "Plasma Fists", "Play Nice", "Play Rough", "Pluck", "Poison Fang", "Poison Gas", "Poison Jab", "Poison Powder", "Poison Sting", "Poison Tail", "Pollen Puff", "Poltergeist", "Population Bomb", "Pounce", "Pound", "Powder", "Powder Snow", "Power Gem", "Power Shift", "Power Split", "Power Swap", "Power Trick", "Power Trip", "Power-Up Punch", "Power Whip", "Precipice Blades", "Present", "Prismatic Laser", "Protect", "Psybeam", "Psyblade", "Psych Up", "Psychic", "Psychic Fangs", "Psychic Terrain", "Psycho Boost", "Psycho Cut", "Psycho Shift", "Psyshield Bash", "Psyshock", "Psystrike", "Psywave", "Pulverizing Pancake", "Punishment", "Purify", "Pursuit", "Pyro Ball", "Quash", "Quick Attack", "Quick Guard", "Quiver Dance", "Rage", "Rage Fist", "Rage Powder", "Raging Bull", "Raging Fury", "Rain Dance", "Rapid Spin", "Razor Leaf", "Razor Shell", "Razor Wind", "Recover", "Recycle", "Reflect", "Reflect Type", "Refresh", "Relic Song", "Rest", "Retaliate", "Return", "Revelation Dance", "Revenge", "Reversal", "Revival Blessing", "Rising Voltage", "Roar", "Roar of Time", "Rock Blast", "Rock Climb", "Rock Polish", "Rock Slide", "Rock Smash", "Rock Throw", "Rock Tomb", "Rock Wrecker", "Role Play", "Rolling Kick", "Rollout", "Roost", "Rototiller", "Round", "Ruination", "Sacred Fire", "Sacred Sword", "Safeguard", "Salt Cure", "Sand Attack", "Sandsear Storm", "Sandstorm", "Sand Tomb", "Sappy Seed", "Savage Spin-Out", "Scald", "Scale Shot", "Scary Face", "Scorching Sands", "Scratch", "Screech", "Searing Shot", "Searing Sunraze Smash", "Secret Power", "Secret Sword", "Seed Bomb", "Seed Flare", "Seismic Toss", "Self-Destruct", "Shadow Ball", "Shadow Bone", "Shadow Claw", "Shadow Force", "Shadow Punch", "Shadow Sneak", "Sharpen", "Shattered Psyche", "Shed Tail", "Sheer Cold", "Shell Side Arm", "Shell Smash", "Shell Trap", "Shelter", "Shift Gear", "Shock Wave", "Shore Up", "Signal Beam", "Silk Trap", "Silver Wind", "Simple Beam", "Sing", "Sinister Arrow Raid", "Sizzly Slide", "Sketch", "Skill Swap", "Skitter Smack", "Skull Bash", "Sky Attack", "Sky Drop", "Sky Uppercut", "Slack Off", "Slam", "Slash", "Sleep Powder", "Sleep Talk", "Sludge", "Sludge Bomb", "Sludge Wave", "Smack Down", "Smart Strike", "Smelling Salts", "Smog", "Smokescreen", "Snap Trap", "Snarl", "Snatch", "Snipe Shot", "Snore", "Snowscape", "Soak", "Soft-Boiled", "Solar Beam", "Solar Blade", "Sonic Boom", "Soul-Stealing 7-Star Strike", "Spacial Rend", "Spark", "Sparkling Aria", "Sparkly Swirl", "Spectral Thief", "Speed Swap", "Spicy Extract", "Spider Web", "Spike Cannon", "Spikes", "Spiky Shield", "Spin Out", "Spirit Break", "Spirit Shackle", "Spit Up", "Spite", "Splash", "Splintered Stormshards", "Splishy Splash", "Spore", "Spotlight", "Springtide Storm", "Stealth Rock", "Steam Eruption", "Steamroller", "Steel Beam", "Steel Roller", "Steel Wing", "Sticky Web", "Stockpile", "Stoked Sparksurfer", "Stomp", "Stomping Tantrum", "Stone Axe", "Stone Edge", "Stored Power", "Storm Throw", "Strange Steam", "Strength", "Strength Sap", "String Shot", "Struggle", "Struggle Bug", "Stuff Cheeks", "Stun Spore", "Submission", "Substitute", "Subzero Slammer", "Sucker Punch", "Sunny Day", "Sunsteel Strike", "Super Fang", "Superpower", "Supersonic", "Supersonic Skystrike", "Surf", "Surging Strikes", "Swagger", "Swallow", "Sweet Kiss", "Sweet Scent", "Swift", "Switcheroo", "Swords Dance", "Synchronoise", "Synthesis", "Tackle", "Tail Glow", "Tail Slap", "Tail Whip", "Tailwind", "Take Down", "Take Heart", "Tar Shot", "Taunt", "Tearful Look", "Teatime", "Techno Blast", "Tectonic Rage", "Teeter Dance", "Telekinesis", "Teleport", "Tera Blast", "Terrain Pulse", "Thief", "Thousand Arrows", "Thousand Waves", "Thrash", "Throat Chop", "Thunder", "Thunderbolt", "Thunder Cage", "Thunder Fang", "Thunderous Kick", "Thunder Punch", "Thunder Shock", "Thunder Wave", "Tickle", "Tidy Up", "Topsy-Turvy", "Torch Song", "Torment", "Toxic", "Toxic Spikes", "Toxic Thread", "Trailblaze", "Transform", "Tri Attack", "Trick", "Trick-or-Treat", "Trick Room", "Triple Arrows", "Triple Axel", "Triple Dive", "Triple Kick", "Trop Kick", "Trump Card", "Twin Beam", "Twineedle", "Twinkle Tackle", "Twister", "U-turn", "Uproar", "Vacuum Wave", "V-create", "Veevee Volley", "Venom Drench", "Venoshock", "Victory Dance", "Vine Whip", "Vise Grip", "Vital Throw", "Volt Switch", "Volt Tackle", "Wake-Up Slap", "Waterfall", "Water Gun", "Water Pledge", "Water Pulse", "Water Shuriken", "Water Sport", "Water Spout", "Wave Crash", "Weather Ball", "Whirlpool", "Whirlwind", "Wicked Blow", "Wicked Torque", "Wide Guard", "Wildbolt Storm", "Wild Charge", "Will-O-Wisp", "Wing Attack", "Wish", "Withdraw", "Wonder Room", "Wood Hammer", "Work Up", "Worry Seed", "Wrap", "Wring Out", "X-Scissor", "Yawn", "Zap Cannon", "Zen Headbutt", "Zing Zap", "Zippy Zap"};

/** @brief List where `internal::AbilityNames[ABILITY]` contains the name of the ability as a string */
static inline const std::array<std::string, ABILITY_TOTAL> AbilityNames = {"", "Adaptability", "Aerilate", "Aftermath", "Air Lock", "Analytic", "Anger Point", "Anger Shell", "Anticipation", "Arena Trap", "Armor Tail", "Aroma Veil", "As One", "Aura Break", "Bad Dreams", "Ball Fetch", "Battery", "Battle Armor", "Battle Bond", "Beads of Ruin", "Beast Boost", "Berserk", "Big Pecks", "Blaze", "Bulletproof", "Cheek Pouch", "Chilling Neigh", "Chlorophyll", "Clear Body", "Cloud Nine", "Color Change", "Comatose", "Commander", "Competitive", "Compound Eyes", "Contrary", "Corrosion", "Costar", "Cotton Down", "Cud Chew", "Curious Medicine", "Cursed Body", "Cute Charm", "Damp", "Dancer", "Dark Aura", "Dauntless Shield", "Dazzling", "Defeatist", "Defiant", "Delta Stream", "Desolate Land", "Disguise", "Download", "Dragon's Maw", "Drizzle", "Drought", "Dry Skin", "Earth Eater", "Early Bird", "Effect Spore", "Electric Surge", "Electromorphosis", "Emergency Exit", "Fairy Aura", "Filter", "Flame Body", "Flare Boost", "Flash Fire", "Flower Gift", "Flower Veil", "Fluffy", "Forecast", "Forewarn", "Friend Guard", "Frisk", "Full Metal Body", "Fur Coat", "Gale Wings", "Galvanize", "Gluttony", "Good as Gold", "Gooey", "Gorilla Tactics", "Grass Pelt", "Grassy Surge", "Grim Neigh", "Guard Dog", "Gulp Missile", "Guts", "Hadron Engine", "Harvest", "Healer", "Heatproof", "Heavy Metal", "Honey Gather", "Huge Power", "Hunger Switch", "Hustle", "Hydration", "Hyper Cutter", "Ice Body", "Ice Face", "Ice Scales", "Illuminate", "Illusion", "Immunity", "Imposter", "Infiltrator", "Innards Out", "Inner Focus", "Insomnia", "Intimidate", "Intrepid Sword", "Iron Barbs", "Iron Fist", "Justified", "Keen Eye", "Klutz", "Leaf Guard", "Levitate", "Libero", "Light Metal", "Lightning Rod", "Lingering Aroma", "Limber", "Liquid Ooze", "Liquid Voice", "Long Reach", "Magic Bounce", "Magic Guard", "Magician", "Magma Armor", "Magnet Pull", "Marvel Scale", "Mega Launcher", "Merciless", "Mimicry", "Minus", "Mirror Armor", "Misty Surge", "Mold Breaker", "Moody", "Motor Drive", "Moxie", "Multiscale", "Multitype", "Mummy", "Mycelium Might", "Natural Cure", "Neuroforce", "Neutralizing Gas", "No Guard", "Normalize", "Oblivious", "Opportunist", "Orichalcum Pulse", "Overcoat", "Overgrow", "Own Tempo", "Parental Bond", "Pastel Veil", "Perish Body", "Pickpocket", "Pickup", "Pixilate", "Plus", "Poison Heal", "Poison Point", "Poison Touch", "Power Construct", "Power of Alchemy", "Power Spot", "Prankster", "Pressure", "Primordial Sea", "Prism Armor", "Propeller Tail", "Protean", "Protosynthesis", "Psychic Surge", "Punk Rock", "Pure Power", "Purifying Salt", "Quark Drive", "Queenly Majesty", "Quick Draw", "Quick Feet", "Rain Dish", "Rattled", "Receiver", "Reckless", "Refrigerate", "Regenerator", "Ripen", "Rivalry", "RKS System", "Rock Head", "Rocky Payload", "Rough Skin", "Run Away", "Sand Force", "Sand Rush", "Sand Spit", "Sand Stream", "Sand Veil", "Sap Sipper", "Schooling", "Scrappy", "Screen Cleaner", "Seed Sower", "Serene Grace", "Shadow Shield", "Shadow Tag", "Sharpness", "Shed Skin", "Sheer Force", "Shell Armor", "Shield Dust", "Shields Down", "Simple", "Skill Link", "Slow Start", "Slush Rush", "Sniper", "Snow Cloak", "Snow Warning", "Solar Power", "Solid Rock", "Soul-Heart", "Soundproof", "Speed Boost", "Stakeout", "Stall", "Stalwart", "Stamina", "Stance Change", "Static", "Steadfast", "Steam Engine", "Steelworker", "Steely Spirit", "Stench", "Sticky Hold", "Storm Drain", "Strong Jaw", "Sturdy", "Suction Cups", "Super Luck", "Supreme Overlord", "Surge Surfer", "Swarm", "Sweet Veil", "Swift Swim", "Symbiosis", "Synchronize", "Sword of Ruin", "Tablets of Ruin", "Tangled Feet", "Tangling Hair", "Technician", "Telepathy", "Teravolt", "Thermal Exchange", "Thick Fat", "Tinted Lens", "Torrent", "Tough Claws", "Toxic Boost", "Toxic Debris", "Trace", "Transistor", "Triage", "Truant", "Turboblaze", "Unaware", "Unburden", "Unnerve", "Unseen Fist", "Vessel of Ruin", "Victory Star", "Vital Spirit", "Volt Absorb", "Wandering Spirit", "Water Absorb", "Water Bubble", "Water Compaction", "Water Veil", "Weak Armor", "Well-Baked Body", "White Smoke", "Wimp Out", "Wind Power", "Wind Rider", "Wonder Guard", "Wonder Skin", "Zen Mode", "Zero to Hero"};

/** @brief List where `internal::ItemNames[ITEM]` contains the name of the item as a string */
static inline const std::array<std::string, ITEM_TOTAL> ItemNames = {"", "Ability Capsule", "Ability Patch", "Ability Shield", "Abomasite", "Abra Candy", "Absolite", "Absorb Bulb", "Acro Bike", "Adamant Crystal", "Adamant Mint", "Adamant Orb", "Adrenaline Orb", "Adventure Guide", "Aerodactylite", "Aerodactyl Candy", "Aggronite", "Aguav Berry", "Air Balloon", "Air Mail", "Alakazite", "Aloraichium Z", "Altarianite", "Amaze Mulch", "Ampharosite", "Amulet Coin", "Antidote", "Apicot Berry", "Apricorn", "Apricorn Box", "Aqua Suit", "Armor Fossil", "Armor Pass", "Armorite Ore", "Articuno Candy", "Aspear Berry", "Assault Vest", "Audinite", "AuroraTicket", "Auspicious Armor", "Autograph", "Aux Evasion", "Aux Guard", "Aux Power", "Aux Powerguard", "Awakening", "Azelf's Fang", "Azure Flute", "Babiri Berry", "Ball of Mud", "Balm Mushroom", "Band Autograph", "Banettite", "Basculegion Food", "Basement Key", "Beach Glass", "Bead Mail", "Bean Cake", "Beast Ball", "Beedrillite", "Bellsprout Candy", "Belue Berry", "Berry", "Berry Juice", "Berry Pots", "Berry Pouch", "Berry Sweet", "Berserk Gene", "Bicycle", "Big Bamboo Shoot", "Big Malasada", "Big Mushroom", "Big Nugget", "Big Pearl", "Big Root", "Bike Voucher", "Binding Band", "Bitter Berry", "Black Apricorn", "Black Augurite", "Black Belt", "Black Flute", "Black Glasses", "Black Mane Hair", "Black Sludge", "Black Tumblestone", "Blank Plate", "Blastoisinite", "Blazikenite", "Bloom Mail", "Blue Apricorn", "Blue Card", "Blue Flute", "Blue Orb", "Blue Petal", "Blue Scarf", "Blue Shard", "Bluesky Mail", "Blu ID Badge", "Bluk Berry", "Blunder Policy", "Bold Mint", "Bonsly Card", "Bonsly Photo", "Boost Mulch", "Booster Energy", "Bottle Cap", "Brave Mint", "Brick Mail", "Brick Piece", "Bridge Mail D", "Bridge Mail M", "Bridge Mail S", "Bridge Mail T", "Bridge Mail V", "Bright Powder", "Bubble Mail", "Bug Gem", "Bug Memory", "Bug Tera Shard", "Buginium Z", "Bugwort", "Bulbasaur Candy", "Burn Drive", "Burn Heal", "Burnt Berry", "Cake-Lure Base", "Calcium", "Calm Mint", "Cameruptite", "Camping Gear", "Candy Truffle", "Carbos", "Card Key", "Careful Mint", "Carrot Seeds", "Casteliacone", "Caster Fern", "Catching Charm", "Caterpie Candy", "Celestica Flute", "Cell Battery", "Chalky Stone", "Chansey Candy", "Charcoal", "Charizardite X", "Charizardite Y", "Charmander Candy", "Charti Berry", "Cheri Berry", "Cherish Ball", "Chesto Berry", "Chilan Berry", "Chill Drive", "Chipped Pot", "Choice Band", "Choice Dumpling", "Choice Scarf", "Choice Specs", "Chople Berry", "Claw Fossil", "Cleanse Tag", "Clear Amulet", "Clear Bell", "Clefairy Candy", "Clever Feather", "Clover Sweet", "Coba Berry", "Coin Case", "Colbur Berry", "Cologne Case", "Colress Machine", "Comet Shard", "Common Stone", "Contest Costume", "Contest Pass", "Cornn Berry", "Coupon 1", "Coupon 2", "Coupon 3", "Courage Candy", "Courage Candy L", "Courage Candy XL", "Cover Fossil", "Covert Cloak", "Cracked Pot", "Crafting Kit", "Crown Pass", "Crunchy Salt", "Cubone Candy", "Cry Analyzer", "Custap Berry", "Damp Mulch", "Damp Rock", "Dark Gem", "Dark Memory", "Dark Stone", "Dark Tera Shard", "Darkinium Z", "Data Cards", "Data ROM", "Dawn Stone", "Dazzling Honey", "D-Disk", "Decidium Z", "Deep Sea Scale", "Deep Sea Tooth", "Destiny Knot", "Devon Parts", "Devon Scope", "Devon Scuba Gear", "Diancite", "Digger Drill", "Diglett Candy", "Dire Hit", "Direshroom", "Disc Case", "Discount Coupon", "Discovery Slate", "Distortion Slate", "Ditto Candy", "Dive Ball", "DNA Sample", "DNA Splicers", "Doduo Candy", "Dome Fossil", "Doppel Bonnets", "Douse Drive", "Down St. Key", "Dowsing Machine", "Draco Plate", "Dragon Fang", "Dragon Gem", "Dragon Memory", "Dragon Scale", "Dragon Skull", "Dragon Tera Shard", "Dragonium Z", "Drash Berry", "Dratini Candy", "Dread Plate", "Dream Ball", "Dream Mail", "Dropped Item", "Drowzee Candy", "DS Sounds", "Dubious Disc", "Durin Berry", "Dusk Ball", "Dusk Stone", "Dynamax Band", "Dynamax Candy", "Dynamax Crystals", "Dynite Ore", "Earth Plate", "Eevee Candy", "Eevium Z", "Egg Ticket", "Eggant Berry", "Ein File C", "Ein File F", "Ein File H", "Ein File P", "Ein File S", "Eject Button", "Eject Pack", "Ekans Candy", "Electabuzz Candy", "Electirizer", "Electric Gem", "Electric Memory", "Electric Seed", "Electric Tera Shard", "Electrium Z", "Elevator Key", "Elixir", "Endorsement", "Energy Powder", "Energy Root", "Enigma Berry", "Enigma Stone", "Enigmatic Card", "Eon Flute", "Eon Mail", "Eon Ticket", "Escape Rope", "Eternal Ice", "Ether", "Everstone", "Eviolite", "Excite Scent", "Exeggcute Candy", "Exp. Candy L", "Exp. Candy M", "Exp. Candy S", "Exp. Candy XL", "Exp. Candy XS", "Exp. Charm", "Exp. Share", "Expert Belt", "Explorer Kit", "Fab Mail", "Fairium Z", "Fairy Gem", "Fairy Memory", "Fairy Tera Shard", "Fame Checker", "Farfetch'd Candy", "Fashion Case", "Fast Ball", "Favored Mail", "F-Disk", "Feather Ball", "Festival Ticket", "Fighting Gem", "Fighting Memory", "Fighting Tera Shard", "Fightinium Z", "Figy Berry", "Fine Remedy", "Fire Gem", "Fire Memory", "Fire Stone", "Fire Tera Shard", "Firium Z", "Fishing Rod", "Fist Plate", "Flame Mail", "Flame Orb", "Flame Plate", "Float Stone", "Flower Mail", "Flower Sweet", "Fluffy Tail", "Flying Gem", "Flying Memory", "Flying Tera Shard", "Flyinium Z", "Focus Band", "Focus Sash", "Forage Bag", "Forest Balm", "Fossilized Bird", "Fossilized Dino", "Fossilized Drake", "Fossilized Fish", "Fresh Water", "Friend Ball", "Full Heal", "Full Incense", "Full Restore", "Galactic Key", "Galarica Cuff", "Galarica Twig", "Galarica Wreath", "Galladite", "Ganlon Berry", "Garchompite", "Gardevoirite", "Gastly Candy", "GB Sounds", "Gear", "Gengarite", "Genius Feather", "Genome Slate", "Gentle Mint", "Geodude Candy", "Ghost Gem", "Ghost Memory", "Ghost Tera Shard", "Ghostium Z", "Gigaton Ball", "Ginema Berry", "Glalitite", "Glitter Mail", "Go-Goggles", "God Stone", "Gold Berry", "Gold Bottle Cap", "Gold Leaf", "Gold Teeth", "Goldeen Candy", "Gonzap's Key", "Golden Nanab Berry", "Golden Pinap Berry", "Golden Razz Berry", "Good Rod", "Gooey Mulch", "Gorgeous Box", "Gracidea", "Grain Cake", "Gram 1", "Gram 2", "Gram 3", "Grass Gem", "Grass Memory", "Grass Tera Shard", "Grassium Z", "Grass Mail", "Grassy Seed", "Great Ball", "Green Apricorn", "Green Petal", "Green Scarf", "Green Shard", "Greet Mail", "Grepa Berry", "Grimer Candy", "Grip Claw", "Grit Dust", "Grit Gravel", "Grit Pebble", "Grit Rock", "Griseous Core", "Griseous Orb", "Grn ID Badge", "Ground Gem", "Ground Memory", "Ground Tera Shard", "Groundium Z", "Growlithe Candy", "Growth Mulch", "Grubby Hanky", "GS Ball", "Guard Spec.", "Guidebook", "Gyaradosite", "Haban Berry", "Harbor Mail", "Hard Stone", "Hasty Mint", "Heal Ball", "Heal Powder", "Health Candy", "Health Candy L", "Health Candy XL", "Health Feather", "Heart Mail", "Heart Scale", "Hearty Grains", "Heat Rock", "Heavy Ball", "Heavy-Duty Boots", "Helix Fossil", "Heracronite", "Hi-tech Earbuds", "Hitmonchan Candy", "Hitmonlee Candy", "HM01", "HM02", "HM03", "HM04", "HM05", "HM06", "HM07", "HM08", "Holo Caster", "Hometown Muffin", "Hondew Berry", "Honey", "Honey Cake", "Honor of Kalos", "Hopo Berry", "Horsea Candy", "Houndoominite", "HP Up", "Hyper Potion", "Iapapa Berry", "Ice Berry", "Ice Gem", "Ice Heal", "Ice Memory", "Ice Stone", "Ice Tera Shard", "Iceroot Carrot", "Icicle Plate", "Icium Z", "Icy Rock", "ID Card", "Ilima's Normalium Z", "Impish Mint", "Incinium Z", "Inquiry Mail", "Insect Plate", "Intriguing Stone", "Iron", "Iron Ball", "Iron Barktongue", "Iron Chunk", "Iron Plate", "Jaboca Berry", "Jade Orb", "Jail Key", "Jaw Fossil", "Jet Ball", "Jigglypuff Candy", "Johto Slate", "Jolly Mint", "Joy Scent", "Jubilife Muffin", "Jynx Candy", "Kabuto Candy", "Kangaskhan Candy", "Kangaskhanite", "Kanto Slate", "Kasib Berry", "Kebia Berry", "Kee Berry", "Kelpsy Berry", "Key Stone", "Key to Room 1", "Key to Room 2", "Key to Room 4", "Key to Room 6", "King's Leaf", "King's Rock", "Koffing Candy", "Kofu's Book", "Kommonium Z", "Koraidon's Poké Ball", "Krabby Candy", "Krane Memo 1", "Krane Memo 2", "Krane Memo 3", "Krane Memo 4", "Krane Memo 5", "Kuo Berry", "Lagging Tail", "Lansat Berry", "Lapras Candy", "Latiasite", "Latiosite", "Lava Cookie", "Lax Incense", "Lax Mint", "L-Disk", "Leaden Ball", "Leader's Crest", "Leaf Letter", "Leaf Stone", "Leek", "Left Poké Ball", "Leftovers", "Legend Plate", "Legendary Clue 1", "Legendary Clue 2", "Legendary Clue 3", "Legendary Clue?", "Lemonade", "Lens Case", "Leppa Berry", "Letter", "Level Ball", "Liberty Pass", "Lickitung Candy", "Liechi Berry", "Life Orb", "Lift Key", "Light Ball", "Light Clay", "Light Stone", "Like Mail", "Linking Cord", "Litebluemail", "Loaded Dice", "Lock Capsule", "Lone Earring", "Lonely Mint", "Looker Ticket", "Loot Sack", "Lopunnite", "Lost Item", "Lost Satchel", "Love Ball", "Love Sweet", "Lovely Mail", "Lucarionite", "Luck Incense", "Lucky Egg", "Lucky Punch", "Lum Berry", "Luminous Moss", "Lumiose Galette", "Lunalium Z", "Lunar Feather", "Lure", "Lure Ball", "Lustrous Globe", "Lustrous Orb", "Luxury Ball", "Lycanium Z", "Mach Bike", "Machine Part", "Macho Brace", "Machop Candy", "Magikarp Candy", "Magma Emblem", "Magma Stone", "Magma Suit", "Magmar Candy", "Magmarizer", "Magnemite Candy", "Magnet", "Mago Berry", "Magost Berry", "Maingate Key", "Makeup Bag", "Malicious Armor", "Manectite", "Mankey Candy", "Maranga Berry", "Marble", "Mark Charm", "Marshadium Z", "Marsh Balm", "Master Ball", "Mawilite", "Max Elixir", "Max Ether", "Max Lure", "Max Honey", "Max Mushrooms", "Max Potion", "Max Repel", "Max Revive", "Mayor's Note", "Meadow Plate", "Mech Mail", "Mechanical Box", "Mechanical Cabinet", "Mechanical Circular Saw", "Mechanical Pinwheel", "Mechanical Tub", "Medal Box", "Medichamite", "Medicinal Leek", "Mega Bracelet", "Mega Ring", "Meltan Candy", "Member Card", "Mental Herb", "Meowth Candy", "Mesprit's Plume", "Metagrossite", "Metal Coat", "Metal Powder", "Meteorite", "Meteorite Shard", "Metronome", "Mew Candy", "Mewnium Z", "Mewtwo Candy", "Mewtwonite X", "Mewtwonite Y", "Micle Berry", "Mighty Candy", "Mighty Candy L", "Mighty Candy XL", "Mild Mint", "Mimikium Z", "Mind Plate", "Mint Berry", "MiracleBerry", "Miracle Seed", "Mirage Mail", "Miraidon's Poké Ball", "Miror Radar", "Mirror Herb", "Misty Seed", "Modest Mint", "Moltres Candy", "Moomoo Milk", "Moon Ball", "Moon Flute", "Moon Shard", "Moon Stone", "Morph Mail", "Mosaic Mail", "Mountain Balm", "Mr. Mime Candy", "Muscle Band", "Muscle Feather", "Mushroom Cake", "Music Disc", "Music Mail", "Mysterious Balm", "Mysterious Shard S", "Mysterious Shard L", "MysteryBerry", "Mystery Egg", "Mystic Water", "MysticTicket", "Naive Mint", "Nanab Berry", "Naughty Mint", "Nest Ball", "Net Ball", "Never-Melt Ice", "Nidoran-Male Candy", "Nidoran-Female Candy", "Niniku Berry", "N-Lunarizer", "Nomel Berry", "Normal Box", "Normal Gem", "Normal Tera Shard", "Normalium Z", "N-Solarizer", "Nugget", "Nutpea Berry", "Oak's Letter", "Oak's Parcel", "Occa Berry", "Oceanic Slate", "Odd Incense", "Odd Keystone", "Oddish Candy", "Old Amber", "Old Charm", "Old Gateau", "Old Journal", "Old Letter", "Old Rod", "Old Sea Map", "Old Verses", "Omanyte Candy", "Onix Candy", "Oran Berry", "Orange Mail", "Orange Petal", "Origin Ball", "Origin Ore", "Oval Charm", "Oval Stone", "Pair of Tickets", "Pal Pad", "Pamtre Berry", "Paralyze Heal", "Paras Candy", "Parcel", "Park Ball", "Pass", "Pass Orb", "Passho Berry", "Payapa Berry", "Pearl", "Pearl String", "Peat Block", "Pecha Berry", "Pep-Up Plant", "Permit", "Persim Berry", "Petaya Berry", "Pewter Crunchies", "Picnic Set", "Pidgeotite", "Pidgey Candy", "Pikachu Candy", "Pikanium Z", "Pikashunium Z", "Pinap Berry", "Pink Apricorn", "Pink Bow", "Pink Nectar", "Pink Petal", "Pink Scarf", "Pinsir Candy", "Pinsirite", "Pixie Plate", "Plasma Card", "Plume Fossil", "Plump Beans", "Poffin Case", "Point Card", "Poison Barb", "Poison Gem", "Poison Memory", "Poison Tera Shard", "Poisonium Z", "Poké Ball", "Poké Doll", "Poké Flute", "Poké Radar", "Poké Snack", "Poké Toy", "Pokéblock Case", "Pokéblock Kit", "Pokédex", "Pokémon Box Link", "Pokéshi Doll", "Polkadot Bow", "Polished Mud Ball", "Poliwag Candy", "Pomeg Berry", "Ponyta Candy", "Pop Pod", "Portraitmail", "Porygon Candy", "Potion", "Power Anklet", "Power Band", "Power Belt", "Power Bracer", "Power Herb", "Power Lens", "Power Plant Pass", "Power Weight", "Powerup Part", "Powder Jar", "PP Max", "PP Up", "Premier Ball", "Pretty Feather", "Primarium Z", "Prism Scale", "Prison Bottle", "Prof's Letter", "Professor's Mask", "Prop Case", "Protective Pads", "Protector", "Protein", "PRZCureBerry", "PSNCureBerry", "Psychic Gem", "Psychic Memory", "Psychic Seed", "Psychic Tera Shard", "Psychium Z", "Psyduck Candy", "Pumkin Berry", "Punching Glove", "Pure Incense", "Purple Nectar", "Purple Petal", "Qualot Berry", "Quick Ball", "Quick Candy", "Quick Candy L", "Quick Candy XL", "Quick Claw", "Quick Powder", "Quiet Mint", "Rabuta Berry", "Radiant Petal", "RageCandyBar", "Rainbow Flower", "Rainbow Pass", "Rainbow Slate", "Rainbow Wing", "Rare Bone", "Rare Candy", "Rash Mint", "Rattata Candy", "Rawst Berry", "Razor Claw", "Razor Fang", "Razz Berry", "R-Disk", "Reaper Cloth", "Recipes", "Red Apricorn", "Red Card", "Red Chain", "Red Flute", "Red ID Badge", "Red Nectar", "Red Orb", "Red Petal", "Red Scale", "Red Scarf", "Red Shard", "Reins of Unity", "Relaxed Mint", "Relic Band", "Relic Copper", "Relic Crown", "Relic Gold", "Relic Silver", "Relic Statue", "Relic Vase", "Remedy", "Repeat Ball", "Repel", "Reply Mail", "Resist Feather", "Retro Mail", "Reveal Glass", "Revival Herb", "Revive", "Rhyhorn Candy", "Ribbon Sweet", "Rich Mulch", "Ride Pager", "Rindo Berry", "Ring Target", "Rock Gem", "Rock Incense", "Rock Memory", "Rock Tera Shard", "Rockium Z", "Rocky Helmet", "Roller Skates", "Room Service", "Root Fossil", "Rose Incense", "Roseli Berry", "Roto Bargain", "Roto Boost", "Roto Catch", "Roto Encounter", "Roto Exp. Points", "Roto Friendship", "Roto Hatch", "Roto HP Restore", "Roto PP Restore", "Roto Prize Money", "Roto Stealth", "Rotom Bike", "Rotom Catalog", "Rotom Phone", "Rowap Berry", "RSVP Mail", "Ruby", "Running Shoes*", "Rusted Shield", "Rusted Sword", "S.S. Ticket", "Sablenite", "Sachet", "Sacred Ash", "Safari Ball", "Safety Goggles", "Sail Fossil", "Salac Berry", "Salamencite", "Salt Cake", "Sand Radish", "Sandshrew Candy", "Sandwich", "Sapphire", "Sassy Mint", "Scanner", "Scarlet Book", "Scatter Bang", "Sceptilite", "Scizorite", "Scope Lens", "Scroll of Darkness", "Scroll of Waters", "Scyther Candy", "Sea Incense", "Seal Case", "Secret Key", "Secret Medicine", "Seed of Mastery", "Seel Candy", "Serious Mint", "Shadow Mail", "Shaderoot Carrot", "Shalour Sable", "Sharp Beak", "Sharpedonite", "Shed Shell", "Shell Bell", "Shellder Candy", "Shiny Charm", "Shiny Leaf", "Shiny Stone", "Shoal Salt", "Shoal Shell", "Shock Drive", "Shuca Berry", "Silk Scarf", "Silph Scope", "Silver Leaf", "Silver Nanab Berry", "Silver Pinap Berry", "Silver Powder", "Silver Razz Berry", "Silver Wing", "Sitrus Berry", "Skull Fossil", "Sky Plate", "Sky Tumblestone", "Slowbronite", "Slowpoke Candy", "Slowpoke Tail", "Small Bouquet", "Small Tablet", "Smart Candy", "Smart Candy L", "Smart Candy XL", "Smoke Ball", "Smoke Bomb", "Smooth Rock", "Snorlax Candy", "Snorlium Z", "Snowball", "Snow Balm", "Snow Mail", "Soda Pop", "Soft Sand", "Solganium Z", "Sonia's Book", "Soot Sack", "Sootfoot Root", "Soothe Bell", "Soul Dew", "Soul Slate", "Space Balm", "Space Mail", "Sparkling Stone", "Spearow Candy", "Spell Tag", "Spelon Berry", "Splash Plate", "Spoiled Apricorn", "Spooky Plate", "Sport Ball", "Sprayduck", "Springy Mushroom", "Sprinklotad", "Squall Slate", "Squirt Bottle", "Squirtle Candy", "Stable Mulch", "Star Piece", "Star Sweet", "Stardust", "Starf Berry", "Staryu Candy", "Stealth Spray", "Steel Gem", "Steel Mail", "Steel Memory", "Steel Teeth", "Steel Tera Shard", "Steelium Z", "Steelixite", "Sticky Barb", "Sticky Glob", "Stone Plate", "Storage Key", "Strange Ball", "Strange Souvenir", "Stratospheric Slate", "Strawberry Sweet", "Stretchy Spring", "Strib Berry", "Style Card", "Subway Key", "Suite Key", "Sun Flute", "Sun Shard", "Sun Stone", "Super Lure", "Super Potion", "Super Repel", "Super Rod", "Superb Remedy", "Surfboard", "Surf Mail", "Surge Badge", "Surprise Mulch", "Survival Charm B", "Survival Charm P", "Survival Charm R", "Survival Charm T", "Survival Charm Y", "Swampertite", "Swap Snack", "Sweet Apple", "Sweet Heart", "Swift Feather", "Swordcap", "System Lever", "Tamato Berry", "Tanga Berry", "Tangela Candy", "Tapunium Z", "Tart Apple", "Tauros Candy", "Tea", "Teachy TV", "Tectonic Slate", "Tempting Charm B", "Tempting Charm P", "Tempting Charm R", "Tempting Charm T", "Tempting Charm Y", "Tentacool Candy", "Tera Orb", "Terrain Extender", "Teru-sama", "Thanks Mail", "Thick Club", "Throat Spray", "Thunder Stone", "Tidal Bell", "Time Balm", "Time Flute", "Timer Ball", "Timid Mint", "Tiny Bamboo Shoot", "Tiny Mushroom", "TM01", "TM02", "TM03", "TM04", "TM05", "TM06", "TM07", "TM08", "TM09", "TM10", "TM11", "TM12", "TM13", "TM14", "TM15", "TM16", "TM17", "TM18", "TM19", "TM20", "TM21", "TM22", "TM23", "TM24", "TM25", "TM26", "TM27", "TM28", "TM29", "TM30", "TM31", "TM32", "TM33", "TM34", "TM35", "TM36", "TM37", "TM38", "TM39", "TM40", "TM41", "TM42", "TM43", "TM44", "TM45", "TM46", "TM47", "TM48", "TM49", "TM50", "TM51", "TM52", "TM53", "TM54", "TM55", "TM56", "TM57", "TM58", "TM59", "TM60", "TM61", "TM62", "TM63", "TM64", "TM65", "TM66", "TM67", "TM68", "TM69", "TM70", "TM71", "TM72", "TM73", "TM74", "TM75", "TM76", "TM77", "TM78", "TM79", "TM80", "TM81", "TM82", "TM83", "TM84", "TM85", "TM86", "TM87", "TM88", "TM89", "TM90", "TM91", "TM92", "TM93", "TM94", "TM95", "TM96", "TM97", "TM98", "TM99", "TM Case", "TM Materials", "TR01", "TR02", "TR03", "TR04", "TR05", "TR06", "TR07", "TR08", "TR09", "TR10", "TR11", "TR12", "TR13", "TR14", "TR15", "TR16", "TR17", "TR18", "TR19", "TR20", "TR21", "TR22", "TR23", "TR24", "TR25", "TR26", "TR27", "TR28", "TR29", "TR30", "TR31", "TR32", "TR33", "TR34", "TR35", "TR36", "TR37", "TR38", "TR39", "TR40", "TR41", "TR42", "TR43", "TR44", "TR45", "TR46", "TR47", "TR48", "TR49", "TR50", "TR51", "TR52", "TR53", "TR54", "TR55", "TR56", "TR57", "TR58", "TR59", "TR60", "TR61", "TR62", "TR63", "TR64", "TR65", "TR66", "TR67", "TR68", "TR69", "TR70", "TR71", "TR72", "TR73", "TR74", "TR75", "TR76", "TR77", "TR78", "TR79", "TR80", "TR81", "TR82", "TR83", "TR84", "TR85", "TR86", "TR87", "TR88", "TR89", "TR90", "TR91", "TR92", "TR93", "TR94", "TR95", "TR96", "TR97", "TR98", "TR99", "TMV Pass", "Topo Berry", "Torn Journal", "Touga Berry", "Tough Candy", "Tough Candy L", "Tough Candy XL", "Town Map", "Toxic Orb", "Toxic Plate", "Travel Trunk", "Tri-Pass", "Tropic Mail", "Tropical Shell", "Tumblestone", "Tunnel Mail", "Twice-Spiced Radish", "Twisted Spoon", "Tyranitarite", "U-Disk", "Ultra Ball", "Ultranecrozium Z", "Unown Report", "Unusual Shoes", "Upgrade", "Utility Umbrella", "Uxie's Claw", "Venonat Candy", "Venusaurite", "Violet Book", "Vivichoke", "Vivid Scent", "Voice Case 1", "Voice Case 2", "Voice Case 3", "Voice Case 4", "Voice Case 5", "Volcano Balm", "Voltorb Candy", "Vs. Recorder", "Vs. Seeker", "Vulpix Candy", "Wacan Berry", "Wailmer Pail", "Wall Fragment", "Warding Charm B", "Warding Charm P", "Warding Charm R", "Warding Charm T", "Warding Charm Y", "Water Gem", "Water Memory", "Water Stone", "Water Tera Shard", "Waterium Z", "Watmel Berry", "Wave Incense", "Wave Mail", "Weakness Policy", "Weedle Candy", "Wepear Berry", "Whipped Dream", "White Apricorn", "White Flute", "White Herb", "White Mane Hair", "Wide Lens", "Wiki Berry", "Wing Ball", "Wise Glasses", "Wishing Chip", "Wishing Piece", "Wishing Star", "Wood", "Wood Mail", "Wooden Crown", "Works Key", "X Accuracy", "X Attack", "X Defense", "X Sp. Atk", "X Sp. Def", "X Speed", "Xtransceiver", "Yache Berry", "Yago Berry", "Yellow Apricorn", "Yellow Flute", "Yellow Nectar", "Yellow Petal", "Yellow Scarf", "Yellow Shard", "Ylw ID Badge", "Zap Plate", "Zapdos Candy", "Zinc", "Zoom Lens", "Z-Power Ring", "Z-Ring", "Zubat Candy", "Zygarde Cube"};

/** @brief List where `internal::NatureNames[NATURE]` contains the name of the nature as a string */
static inline const std::array<std::string, NATURE_TOTAL> NatureNames = {"", "Adamant", "Bashful", "Bold", "Brave", "Calm", "Careful", "Docile", "Gentle", "Hardy", "Hasty", "Impish", "Jolly", "Lax", "Lonely", "Mild", "Modest", "Naive", "Naughty", "Quiet", "Quirky", "Rash", "Relaxed", "Sassy", "Serious", "Timid"};

/** @brief List where `internal::StatusNames[STATUS]` contains the name of the status condition as a string */
static inline const std::array<std::string, STATUS_TOTAL> StatusNames = {"", "brn", "frz", "par", "psn", "slp", "tox", /* "frb", "dro" */};

/** @brief List where `internal::GenderNames[GENDER]` contains the name of the gender as a string */
static inline const std::array<std::string, GENDER_TOTAL> GenderNames = {"", "F", "M"};

/** @brief List where `internal::TypeNames[TYPE]` contains the name of the type as a string */
static inline const std::array<std::string, TYPE_TOTAL> TypeNames = {"", "Normal", "Fighting", "Flying", "Poison", "Ground", "Rock", "Bug", "Ghost", "Steel", "Fire", "Water", "Grass", "Electric", "Psychic", "Ice", "Dragon", "Dark", "Fairy"};

/** @brief List where `internal::StatNames[STAT]` contains the name of the stat as a string */
static inline const std::array<std::string, STAT_TOTAL> StatNames = {"HP", "ATK", "DEF", "SPA", "SPD", "SPE", /* "SPC" */};


/** @brief List where `internal::SpeciesSmogonNames[SPECIES]` contains the Smogon-style name of the species as a string */
static inline const std::array<std::string, SPECIES_TOTAL> SpeciesSmogonNames = {"", "Bulbasaur", "Ivysaur", "Venusaur", "Venusaur-Mega", "Venusaur-Gmax", "Charmander", "Charmeleon", "Charizard", "Charizard-Mega-X", "Charizard-Mega-Y", "Charizard-Gmax", "Squirtle", "Wartortle", "Blastoise", "Blastoise-Mega", "Blastoise-Gmax", "Caterpie", "Metapod", "Butterfree", "Butterfree-Gmax", "Weedle", "Kakuna", "Beedrill", "Beedrill-Mega", "Pidgey", "Pidgeotto", "Pidgeot", "Pidgeot-Mega", "Rattata", "Rattata-Alola", "Raticate", "Raticate-Alola", "Raticate-Alola-Totem", "Spearow", "Fearow", "Ekans", "Arbok", "Pikachu", "Pikachu-Cosplay", "Pikachu-Rock-Star", "Pikachu-Belle", "Pikachu-Pop-Star", "Pikachu-PhD", "Pikachu-Libre", "Pikachu-Original", "Pikachu-Hoenn", "Pikachu-Sinnoh", "Pikachu-Unova", "Pikachu-Kalos", "Pikachu-Alola", "Pikachu-Partner", "Pikachu-Starter", "Pikachu-Gmax", "Pikachu-World", "Raichu", "Raichu-Alola", "Sandshrew", "Sandshrew-Alola", "Sandslash", "Sandslash-Alola", "Nidoran-F", "Nidorina", "Nidoqueen", "Nidoran-M", "Nidorino", "Nidoking", "Clefairy", "Clefable", "Vulpix", "Vulpix-Alola", "Ninetales", "Ninetales-Alola", "Jigglypuff", "Wigglytuff", "Zubat", "Golbat", "Oddish", "Gloom", "Vileplume", "Paras", "Parasect", "Venonat", "Venomoth", "Diglett", "Diglett-Alola", "Dugtrio", "Dugtrio-Alola", "Meowth", "Meowth-Alola", "Meowth-Galar", "Meowth-Gmax", "Persian", "Persian-Alola", "Psyduck", "Golduck", "Mankey", "Primeape", "Growlithe-Hisui", "Growlithe", "Arcanine-Hisui", "Arcanine", "Poliwag", "Poliwhirl", "Poliwrath", "Abra", "Kadabra", "Alakazam", "Alakazam-Mega", "Machop", "Machoke", "Machamp", "Machamp-Gmax", "Bellsprout", "Weepinbell", "Victreebel", "Tentacool", "Tentacruel", "Geodude", "Geodude-Alola", "Graveler", "Graveler-Alola", "Golem", "Golem-Alola", "Ponyta", "Ponyta-Galar", "Rapidash", "Rapidash-Galar", "Slowpoke", "Slowpoke-Galar", "Slowbro", "Slowbro-Mega", "Slowbro-Galar", "Magnemite", "Magneton", "Farfetch’d", "Farfetch’d-Galar", "Doduo", "Dodrio", "Seel", "Dewgong", "Grimer", "Grimer-Alola", "Muk", "Muk-Alola", "Shellder", "Cloyster", "Gastly", "Haunter", "Gengar", "Gengar-Mega", "Gengar-Gmax", "Onix", "Drowzee", "Hypno", "Krabby", "Kingler", "Kingler-Gmax", "Voltorb", "Voltorb-Hisui", "Electrode", "Electrode-Hisui", "Exeggcute", "Exeggutor", "Exeggutor-Alola", "Cubone", "Marowak", "Marowak-Alola", "Marowak-Alola-Totem", "Hitmonlee", "Hitmonchan", "Lickitung", "Koffing", "Weezing", "Weezing-Galar", "Rhyhorn", "Rhydon", "Chansey", "Tangela", "Kangaskhan", "Kangaskhan-Mega", "Horsea", "Seadra", "Goldeen", "Seaking", "Staryu", "Starmie", "Mr. Mime", "Mr. Mime-Galar", "Scyther", "Jynx", "Electabuzz", "Magmar", "Pinsir", "Pinsir-Mega", "Tauros", "Tauros-Paldea", "Tauros-Paldea-Fire", "Tauros-Paldea-Water", "Magikarp", "Gyarados", "Gyarados-Mega", "Lapras", "Lapras-Gmax", "Ditto", "Eevee", "Eevee-Starter", "Eevee-Gmax", "Vaporeon", "Jolteon", "Flareon", "Porygon", "Omanyte", "Omastar", "Kabuto", "Kabutops", "Aerodactyl", "Aerodactyl-Mega", "Snorlax", "Snorlax-Gmax", "Articuno", "Articuno-Galar", "Zapdos", "Zapdos-Galar", "Moltres", "Moltres-Galar", "Dratini", "Dragonair", "Dragonite", "Mewtwo", "Mewtwo-Mega-X", "Mewtwo-Mega-Y", "Mew", "Chikorita", "Bayleef", "Meganium", "Cyndaquil", "Quilava", "Typhlosion-Hisui", "Typhlosion", "Totodile", "Croconaw", "Feraligatr", "Sentret", "Furret", "Hoothoot", "Noctowl", "Ledyba", "Ledian", "Spinarak", "Ariados", "Crobat", "Chinchou", "Lanturn", "Pichu", "Pichu-Spiky-eared", "Cleffa", "Igglybuff", "Togepi", "Togetic", "Natu", "Xatu", "Mareep", "Flaaffy", "Ampharos", "Ampharos-Mega", "Bellossom", "Marill", "Azumarill", "Sudowoodo", "Politoed", "Hoppip", "Skiploom", "Jumpluff", "Aipom", "Sunkern", "Sunflora", "Yanma", "Wooper", "Wooper-Paldea", "Quagsire", "Espeon", "Umbreon", "Murkrow", "Slowking", "Slowking-Galar", "Misdreavus", "Unown", "Unown", "Unown-B", "Unown-C", "Unown-D", "Unown-E", "Unown-F", "Unown-G", "Unown-H", "Unown-I", "Unown-J", "Unown-K", "Unown-L", "Unown-M", "Unown-N", "Unown-O", "Unown-P", "Unown-Q", "Unown-R", "Unown-S", "Unown-T", "Unown-U", "Unown-V", "Unown-W", "Unown-X", "Unown-Y", "Unown-Z", "Unown-Exclamation", "Unown-Question", "Wobbuffet", "Girafarig", "Pineco", "Forretress", "Dunsparce", "Gligar", "Steelix", "Steelix-Mega", "Snubbull", "Granbull", "Qwilfish", "Qwilfish-Hisui", "Scizor", "Scizor-Mega", "Shuckle", "Heracross", "Heracross-Mega", "Sneasel-Hisui", "Sneasel", "Teddiursa", "Ursaring", "Slugma", "Magcargo", "Swinub", "Piloswine", "Corsola", "Corsola-Galar", "Remoraid", "Octillery", "Delibird", "Mantine", "Skarmory", "Houndour", "Houndoom", "Houndoom-Mega", "Kingdra", "Phanpy", "Donphan", "Porygon2", "Stantler", "Smeargle", "Tyrogue", "Hitmontop", "Smoochum", "Elekid", "Magby", "Miltank", "Blissey", "Raikou", "Entei", "Suicune", "Larvitar", "Pupitar", "Tyranitar", "Tyranitar-Mega", "Lugia", "Ho-Oh", "Celebi", "Treecko", "Grovyle", "Sceptile", "Sceptile-Mega", "Torchic", "Combusken", "Blaziken", "Blaziken-Mega", "Mudkip", "Marshtomp", "Swampert", "Swampert-Mega", "Poochyena", "Mightyena", "Zigzagoon", "Zigzagoon-Galar", "Linoone", "Linoone-Galar", "Wurmple", "Silcoon", "Beautifly", "Cascoon", "Dustox", "Lotad", "Lombre", "Ludicolo", "Seedot", "Nuzleaf", "Shiftry", "Taillow", "Swellow", "Wingull", "Pelipper", "Ralts", "Kirlia", "Gardevoir", "Gardevoir-Mega", "Surskit", "Masquerain", "Shroomish", "Breloom", "Slakoth", "Vigoroth", "Slaking", "Nincada", "Ninjask", "Shedinja", "Whismur", "Loudred", "Exploud", "Makuhita", "Hariyama", "Azurill", "Nosepass", "Skitty", "Delcatty", "Sableye", "Sableye-Mega", "Mawile", "Mawile-Mega", "Aron", "Lairon", "Aggron", "Aggron-Mega", "Meditite", "Medicham", "Medicham-Mega", "Electrike", "Manectric", "Manectric-Mega", "Plusle", "Minun", "Volbeat", "Illumise", "Roselia", "Gulpin", "Swalot", "Carvanha", "Sharpedo", "Sharpedo-Mega", "Wailmer", "Wailord", "Numel", "Camerupt", "Camerupt-Mega", "Torkoal", "Spoink", "Grumpig", "Spinda", "Trapinch", "Vibrava", "Flygon", "Cacnea", "Cacturne", "Swablu", "Altaria", "Altaria-Mega", "Zangoose", "Seviper", "Lunatone", "Solrock", "Barboach", "Whiscash", "Corphish", "Crawdaunt", "Baltoy", "Claydol", "Lileep", "Cradily", "Anorith", "Armaldo", "Feebas", "Milotic", "Castform", "Castform-Sunny", "Castform-Rainy", "Castform-Snowy", "Kecleon", "Shuppet", "Banette", "Banette-Mega", "Duskull", "Dusclops", "Tropius", "Chimecho", "Absol", "Absol-Mega", "Wynaut", "Snorunt", "Glalie", "Glalie-Mega", "Spheal", "Sealeo", "Walrein", "Clamperl", "Huntail", "Gorebyss", "Relicanth", "Luvdisc", "Bagon", "Shelgon", "Salamence", "Salamence-Mega", "Beldum", "Metang", "Metagross", "Metagross-Mega", "Regirock", "Regice", "Registeel", "Latias", "Latias-Mega", "Latios", "Latios-Mega", "Kyogre", "Kyogre-Primal", "Groudon", "Groudon-Primal", "Rayquaza", "Rayquaza-Mega", "Jirachi", "Deoxys", "Deoxys-Attack", "Deoxys-Defense", "Deoxys-Speed", "Turtwig", "Grotle", "Torterra", "Chimchar", "Monferno", "Infernape", "Piplup", "Prinplup", "Empoleon", "Starly", "Staravia", "Staraptor", "Bidoof", "Bibarel", "Kricketot", "Kricketune", "Shinx", "Luxio", "Luxray", "Budew", "Roserade", "Cranidos", "Rampardos", "Shieldon", "Bastiodon", "Burmy", "Burmy", "Burmy-Sandy", "Burmy-Trash", "Wormadam", "Wormadam-Sandy", "Wormadam-Trash", "Mothim", "Combee", "Vespiquen", "Pachirisu", "Buizel", "Floatzel", "Cherubi", "Cherrim", "Cherrim", "Cherrim-Sunshine", "Shellos", "Shellos", "Shellos-East", "Gastrodon", "Gastrodon", "Gastrodon-East", "Ambipom", "Drifloon", "Drifblim", "Buneary", "Lopunny", "Lopunny-Mega", "Mismagius", "Honchkrow", "Glameow", "Purugly", "Chingling", "Stunky", "Skuntank", "Bronzor", "Bronzong", "Bonsly", "Mime Jr.", "Happiny", "Chatot", "Spiritomb", "Gible", "Gabite", "Garchomp", "Garchomp-Mega", "Munchlax", "Riolu", "Lucario", "Lucario-Mega", "Hippopotas", "Hippowdon", "Skorupi", "Drapion", "Croagunk", "Toxicroak", "Carnivine", "Finneon", "Lumineon", "Mantyke", "Snover", "Abomasnow", "Abomasnow-Mega", "Weavile", "Magnezone", "Lickilicky", "Rhyperior", "Tangrowth", "Electivire", "Magmortar", "Togekiss", "Yanmega", "Leafeon", "Glaceon", "Gliscor", "Mamoswine", "Porygon-Z", "Gallade", "Gallade-Mega", "Probopass", "Dusknoir", "Froslass", "Rotom", "Rotom-Heat", "Rotom-Wash", "Rotom-Frost", "Rotom-Fan", "Rotom-Mow", "Uxie", "Mesprit", "Azelf", "Dialga", "Dialga-Origin", "Palkia", "Palkia-Origin", "Heatran", "Regigigas", "Giratina", "Giratina-Origin", "Cresselia", "Phione", "Manaphy", "Darkrai", "Shaymin", "Shaymin", "Shaymin-Sky", "Arceus", "Arceus-Bug", "Arceus-Dark", "Arceus-Dragon", "Arceus-Electric", "Arceus-Fairy", "Arceus-Fighting", "Arceus-Fire", "Arceus-Flying", "Arceus-Ghost", "Arceus-Grass", "Arceus-Ground", "Arceus-Ice", "Arceus-Poison", "Arceus-Psychic", "Arceus-Rock", "Arceus-Steel", "Arceus-Water", "Victini", "Snivy", "Servine", "Serperior", "Tepig", "Pignite", "Emboar", "Oshawott", "Dewott", "Samurott-Hisui", "Samurott", "Patrat", "Watchog", "Lillipup", "Herdier", "Stoutland", "Purrloin", "Liepard", "Pansage", "Simisage", "Pansear", "Simisear", "Panpour", "Simipour", "Munna", "Musharna", "Pidove", "Tranquill", "Unfezant", "Blitzle", "Zebstrika", "Roggenrola", "Boldore", "Gigalith", "Woobat", "Swoobat", "Drilbur", "Excadrill", "Audino", "Audino-Mega", "Timburr", "Gurdurr", "Conkeldurr", "Tympole", "Palpitoad", "Seismitoad", "Throh", "Sawk", "Sewaddle", "Swadloon", "Leavanny", "Venipede", "Whirlipede", "Scolipede", "Cottonee", "Whimsicott", "Petilil", "Lilligant", "Lilligant-Hisui", "Basculin", "Basculin-Blue-Striped", "Basculin-White-Striped", "Sandile", "Krokorok", "Krookodile", "Darumaka", "Darumaka-Galar", "Darmanitan", "Darmanitan-Zen", "Darmanitan-Galar", "Darmanitan-Galar-Zen", "Maractus", "Dwebble", "Crustle", "Scraggy", "Scrafty", "Sigilyph", "Yamask", "Yamask-Galar", "Cofagrigus", "Tirtouga", "Carracosta", "Archen", "Archeops", "Trubbish", "Garbodor", "Garbodor-Gmax", "Zorua", "Zorua-Hisui", "Zoroark-Hisui", "Zoroark", "Minccino", "Cinccino", "Gothita", "Gothorita", "Gothitelle", "Solosis", "Duosion", "Reuniclus", "Ducklett", "Swanna", "Vanillite", "Vanillish", "Vanilluxe", "Deerling", "Deerling", "Deerling-Summer", "Deerling-Autumn", "Deerling-Winter", "Sawsbuck", "Sawsbuck", "Sawsbuck-Summer", "Sawsbuck-Autumn", "Sawsbuck-Winter", "Emolga", "Karrablast", "Escavalier", "Foongus", "Amoonguss", "Frillish", "Jellicent", "Alomomola", "Joltik", "Galvantula", "Ferroseed", "Ferrothorn", "Klink", "Klang", "Klinklang", "Tynamo", "Eelektrik", "Eelektross", "Elgyem", "Beheeyem", "Litwick", "Lampent", "Chandelure", "Axew", "Fraxure", "Haxorus", "Cubchoo", "Beartic", "Cryogonal", "Shelmet", "Accelgor", "Stunfisk", "Stunfisk-Galar", "Mienfoo", "Mienshao", "Druddigon", "Golett", "Golurk", "Pawniard", "Bisharp", "Bouffalant", "Rufflet", "Braviary-Hisui", "Braviary", "Vullaby", "Mandibuzz", "Heatmor", "Durant", "Deino", "Zweilous", "Hydreigon", "Larvesta", "Volcarona", "Cobalion", "Terrakion", "Virizion", "Tornadus", "Tornadus-Therian", "Thundurus", "Thundurus-Therian", "Reshiram", "Zekrom", "Landorus", "Landorus-Therian", "Kyurem", "Kyurem-Black", "Kyurem-White", "Keldeo", "Keldeo-Resolute", "Meloetta", "Meloetta-Pirouette", "Genesect", "Genesect-Douse", "Genesect-Shock", "Genesect-Burn", "Genesect-Chill", "Chespin", "Quilladin", "Chesnaught", "Fennekin", "Braixen", "Delphox", "Froakie", "Frogadier", "Greninja", "Greninja-Ash", "Bunnelby", "Diggersby", "Fletchling", "Fletchinder", "Talonflame", "Scatterbug", "Spewpa", "Vivillon", "Vivillon", "Vivillon-Archipelago", "Vivillon-Continental", "Vivillon-Elegant", "Vivillon-Garden", "Vivillon-High Plains", "Vivillon-Icy Snow", "Vivillon-Jungle", "Vivillon-Marine", "Vivillon-Modern", "Vivillon-Monsoon", "Vivillon-Ocean", "Vivillon-Polar", "Vivillon-River", "Vivillon-Sandstorm", "Vivillon-Savanna", "Vivillon-Sun", "Vivillon-Tundra", "Vivillon-Fancy", "Vivillon-Pokeball", "Litleo", "Pyroar", "Flabébé", "Flabébé", "Flabébé-Blue", "Flabébé-Orange", "Flabébé-White", "Flabébé-Yellow", "Floette", "Floette", "Floette-Blue", "Floette-Orange", "Floette-White", "Floette-Yellow", "Floette-Eternal", "Florges", "Florges", "Florges-Blue", "Florges-Orange", "Florges-White", "Florges-Yellow", "Skiddo", "Gogoat", "Pancham", "Pangoro", "Furfrou", "Furfrou", "Furfrou-Dandy", "Furfrou-Debutante", "Furfrou-Diamond", "Furfrou-Heart", "Furfrou-Kabuki", "Furfrou-La Reine", "Furfrou-Matron", "Furfrou-Pharaoh", "Furfrou-Star", "Espurr", "Meowstic", "Meowstic-F", "Honedge", "Doublade", "Aegislash", "Aegislash", "Aegislash-Blade", "Spritzee", "Aromatisse", "Swirlix", "Slurpuff", "Inkay", "Malamar", "Binacle", "Barbaracle", "Skrelp", "Dragalge", "Clauncher", "Clawitzer", "Helioptile", "Heliolisk", "Tyrunt", "Tyrantrum", "Amaura", "Aurorus", "Sylveon", "Hawlucha", "Dedenne", "Carbink", "Goomy", "Sliggoo-Hisui", "Sliggoo", "Goodra-Hisui", "Goodra", "Klefki", "Phantump", "Trevenant", "Pumpkaboo", "Pumpkaboo-Small", "Pumpkaboo-Large", "Pumpkaboo-Super", "Gourgeist", "Gourgeist-Small", "Gourgeist-Large", "Gourgeist-Super", "Bergmite", "Avalugg", "Avalugg-Hisui", "Noibat", "Noivern", "Xerneas", "Yveltal", "Zygarde", "Zygarde-10%", "Zygarde-Complete", "Diancie", "Diancie-Mega", "Hoopa", "Hoopa-Unbound", "Volcanion", "Rowlet", "Dartrix", "Decidueye-Hisui", "Decidueye", "Litten", "Torracat", "Incineroar", "Popplio", "Brionne", "Primarina", "Pikipek", "Trumbeak", "Toucannon", "Yungoos", "Gumshoos", "Gumshoos-Totem", "Grubbin", "Charjabug", "Vikavolt", "Vikavolt-Totem", "Crabrawler", "Crabominable", "Oricorio", "Oricorio-Pom-Pom", "Oricorio-Pa'u", "Oricorio-Sensu", "Cutiefly", "Ribombee", "Ribombee-Totem", "Rockruff", "Rockruff", "Lycanroc", "Lycanroc-Midnight", "Lycanroc-Dusk", "Wishiwashi", "Wishiwashi", "Wishiwashi-School", "Mareanie", "Toxapex", "Mudbray", "Mudsdale", "Dewpider", "Araquanid", "Araquanid-Totem", "Fomantis", "Lurantis", "Lurantis-Totem", "Morelull", "Shiinotic", "Salandit", "Salazzle", "Salazzle-Totem", "Stufful", "Bewear", "Bounsweet", "Steenee", "Tsareena", "Comfey", "Oranguru", "Passimian", "Wimpod", "Golisopod", "Sandygast", "Palossand", "Pyukumuku", "Type: Null", "Silvally", "Silvally-Bug", "Silvally-Dark", "Silvally-Dragon", "Silvally-Electric", "Silvally-Fairy", "Silvally-Fighting", "Silvally-Fire", "Silvally-Flying", "Silvally-Ghost", "Silvally-Grass", "Silvally-Ground", "Silvally-Ice", "Silvally-Poison", "Silvally-Psychic", "Silvally-Rock", "Silvally-Steel", "Silvally-Water", "Minior", "Minior", "Minior", "Minior-Orange", "Minior-Yellow", "Minior-Green", "Minior-Blue", "Minior-Indigo", "Minior-Violet", "Minior-Meteor", "Komala", "Turtonator", "Togedemaru", "Togedemaru-Totem", "Mimikyu", "Mimikyu-Busted", "Mimikyu-Totem", "Mimikyu-Busted-Totem", "Bruxish", "Drampa", "Dhelmise", "Jangmo-o", "Hakamo-o", "Kommo-o", "Kommo-o-Totem", "Tapu Koko", "Tapu Lele", "Tapu Bulu", "Tapu Fini", "Cosmog", "Cosmoem", "Solgaleo", "Lunala", "Nihilego", "Buzzwole", "Pheromosa", "Xurkitree", "Celesteela", "Kartana", "Guzzlord", "Necrozma", "Necrozma-Dusk-Mane", "Necrozma-Dawn-Wings", "Necrozma-Ultra", "Magearna", "Magearna-Original", "Marshadow", "Poipole", "Naganadel", "Stakataka", "Blacephalon", "Zeraora", "Meltan", "Melmetal", "Melmetal-Gmax", "Grookey", "Thwackey", "Rillaboom", "Rillaboom-Gmax", "Scorbunny", "Raboot", "Cinderace", "Cinderace-Gmax", "Sobble", "Drizzile", "Inteleon", "Inteleon-Gmax", "Skwovet", "Greedent", "Rookidee", "Corvisquire", "Corviknight", "Corviknight-Gmax", "Blipbug", "Dottler", "Orbeetle", "Orbeetle-Gmax", "Nickit", "Thievul", "Gossifleur", "Eldegoss", "Wooloo", "Dubwool", "Chewtle", "Drednaw", "Drednaw-Gmax", "Yamper", "Boltund", "Rolycoly", "Carkol", "Coalossal", "Coalossal-Gmax", "Applin", "Flapple", "Flapple-Gmax", "Appletun", "Appletun-Gmax", "Silicobra", "Sandaconda", "Sandaconda-Gmax", "Cramorant", "Cramorant-Gulping", "Cramorant-Gorging", "Arrokuda", "Barraskewda", "Toxel", "Toxtricity", "Toxtricity-Low-Key", "Toxtricity-Gmax", "Toxtricity-Low-Key-Gmax", "Sizzlipede", "Centiskorch", "Centiskorch-Gmax", "Clobbopus", "Grapploct", "Sinistea", "Sinistea-Antique", "Polteageist", "Polteageist-Antique", "Hatenna", "Hattrem", "Hatterene", "Hatterene-Gmax", "Impidimp", "Morgrem", "Grimmsnarl", "Grimmsnarl-Gmax", "Obstagoon", "Perrserker", "Cursola", "Sirfetch’d", "Mr. Rime", "Runerigus", "Milcery", "Alcremie", "Alcremie", "Alcremie-Ruby-Cream", "Alcremie-Matcha-Cream", "Alcremie-Mint-Cream", "Alcremie-Lemon-Cream", "Alcremie-Salted-Cream", "Alcremie-Ruby-Swirl", "Alcremie-Caramel-Swirl", "Alcremie-Rainbow-Swirl", "Alcremie-Gmax", "Falinks", "Pincurchin", "Snom", "Frosmoth", "Stonjourner", "Eiscue", "Eiscue", "Eiscue-Noice", "Indeedee", "Indeedee-F", "Morpeko", "Morpeko", "Morpeko-Hangry", "Cufant", "Copperajah", "Copperajah-Gmax", "Dracozolt", "Arctozolt", "Dracovish", "Arctovish", "Duraludon", "Duraludon-Gmax", "Dreepy", "Drakloak", "Dragapult", "Zacian", "Zacian-Crowned", "Zamazenta", "Zamazenta-Crowned", "Eternatus", "Eternatus-Eternamax", "Kubfu", "Urshifu", "Urshifu-Rapid-Strike", "Urshifu-Gmax", "Urshifu-Rapid-Strike-Gmax", "Zarude", "Zarude-Dada", "Regieleki", "Regidrago", "Glastrier", "Spectrier", "Calyrex", "Calyrex-Ice", "Calyrex-Shadow", "Wyrdeer", "Kleavor", "Ursaluna", "Basculegion", "Basculegion-F", "Sneasler", "Overqwil", "Enamorus", "Enamorus-Therian", "Sprigatito", "Floragato", "Meowscarada", "Fuecoco", "Crocalor", "Skeledirge", "Quaxly", "Quaxwell", "Quaquaval", "Lechonk", "Oinkologne", "Oinkologne-F", "Tarountula", "Spidops", "Nymble", "Lokix", "Pawmi", "Pawmo", "Pawmot", "Tandemaus", "Maushold", "Maushold-Three", "Maushold-Four", "Fidough", "Dachsbun", "Smoliv", "Dolliv", "Arboliva", "Squawkabilly", "Squawkabilly-Blue", "Squawkabilly-Yellow", "Squawkabilly-White", "Nacli", "Naclstack", "Garganacl", "Charcadet", "Armarouge", "Ceruledge", "Tadbulb", "Bellibolt", "Wattrel", "Kilowattrel", "Maschiff", "Mabosstiff", "Shroodle", "Grafaiai", "Bramblin", "Brambleghast", "Toedscool", "Toedscruel", "Klawf", "Capsakid", "Scovillain", "Rellor", "Rabsca", "Flittle", "Espathra", "Tinkatink", "Tinkatuff", "Tinkaton", "Wiglett", "Wugtrio", "Bombirdier", "Finizen", "Palafin", "Palafin-Hero", "Varoom", "Revavroom", "Cyclizar", "Orthworm", "Glimmet", "Glimmora", "Greavard", "Houndstone", "Flamigo", "Cetoddle", "Cetitan", "Veluza", "Dondozo", "Tatsugiri", "Tatsugiri-Curly", "Tatsugiri-Droopy", "Tatsugiri-Stretchy", "Annihilape", "Clodsire", "Farigiraf", "Dudunsparce", "Dudunsparce-Two-Segment", "Dudunsparce-Three-Segment", "Kingambit", "Great Tusk", "Scream Tail", "Brute Bonnet", "Flutter Mane", "Slither Wing", "Sandy Shocks", "Iron Treads", "Iron Bundle", "Iron Hands", "Iron Jugulis", "Iron Moth", "Iron Thorns", "Frigibax", "Arctibax", "Baxcalibur", "Gimmighoul", "Gimmighoul-Roaming", "Gholdengo", "Wo-Chien", "Chien-Pao", "Ting-Lu", "Chi-Yu", "Roaring Moon", "Iron Valiant", "Koraidon", "Miraidon", "Walking Wake", "Iron Leaves"};

/** @brief List where `internal::MoveSmogonNames[MOVE]` contains the Smogon-style name of the move as a string */
static inline const std::array<std::string, MOVE_TOTAL> MoveSmogonNames = {"", "10,000,000 Volt Thunderbolt", "Absorb", "Accelerock", "Acid", "Acid Armor", "Acid Downpour", "Acid Spray", "Acrobatics", "Acupressure", "Aerial Ace", "Aeroblast", "After You", "Agility", "Air Cutter", "Air Slash", "All-Out Pummeling", "Ally Switch", "Amnesia", "Anchor Shot", "Ancient Power", "Apple Acid", "Aqua Cutter", "Aqua Jet", "Aqua Ring", "Aqua Step", "Aqua Tail", "Armor Cannon", "Arm Thrust", "Aromatherapy", "Aromatic Mist", "Assist", "Assurance", "Astonish", "Astral Barrage", "Attack Order", "Attract", "Aura Sphere", "Aura Wheel", "Aurora Beam", "Aurora Veil", "Autotomize", "Avalanche", "Axe Kick", "Baby-Doll Eyes", "Baddy Bad", "Baneful Bunker", "Barb Barrage", "Barrage", "Barrier", "Baton Pass", "Beak Blast", "Beat Up", "Behemoth Bash", "Behemoth Blade", "Belch", "Belly Drum", "Bestow", "Bide", "Bind", "Bite", "Bitter Blade", "Bitter Malice", "Black Hole Eclipse", "Blast Burn", "Blaze Kick", "Blazing Torque", "Bleakwind Storm", "Blizzard", "Block", "Bloom Doom", "Blue Flare", "Body Press", "Body Slam", "Bolt Beak", "Bolt Strike", "Bone Club", "Bonemerang", "Bone Rush", "Boomburst", "Bounce", "Bouncy Bubble", "Branch Poke", "Brave Bird", "Breaking Swipe", "Breakneck Blitz", "Brick Break", "Brine", "Brutal Swing", "Bubble", "Bubble Beam", "Bug Bite", "Bug Buzz", "Bulk Up", "Bulldoze", "Bullet Punch", "Bullet Seed", "Burning Jealousy", "Burn Up", "Buzzy Buzz", "Calm Mind", "Camouflage", "Captivate", "Catastropika", "Ceaseless Edge", "Celebrate", "Charge", "Charge Beam", "Charm", "Chatter", "Chilling Water", "Chilly Reception", "Chip Away", "Chloroblast", "Circle Throw", "Clamp", "Clanging Scales", "Clangorous Soul", "Clangorous Soulblaze", "Clear Smog", "Close Combat", "Coaching", "Coil", "Collision Course", "Combat Torque", "Comet Punch", "Comeuppance", "Confide", "Confuse Ray", "Confusion", "Constrict", "Continental Crush", "Conversion", "Conversion 2", "Copycat", "Core Enforcer", "Corkscrew Crash", "Corrosive Gas", "Cosmic Power", "Cotton Guard", "Cotton Spore", "Counter", "Court Change", "Covet", "Crabhammer", "Crafty Shield", "Cross Chop", "Cross Poison", "Crunch", "Crush Claw", "Crush Grip", "Curse", "Cut", "Darkest Lariat", "Dark Pulse", "Dark Void", "Dazzling Gleam", "Decorate", "Defend Order", "Defense Curl", "Defog", "Destiny Bond", "Detect", "Devastating Drake", "Diamond Storm", "Dig", "Disable", "Disarming Voice", "Discharge", "Dire Claw", "Dive", "Dizzy Punch", "Doodle", "Doom Desire", "Double-Edge", "Double Hit", "Double Iron Bash", "Double Kick", "Double Shock", "Double Slap", "Double Team", "Draco Meteor", "Dragon Ascent", "Dragon Breath", "Dragon Claw", "Dragon Dance", "Dragon Darts", "Dragon Energy", "Dragon Hammer", "Dragon Pulse", "Dragon Rage", "Dragon Rush", "Dragon Tail", "Draining Kiss", "Drain Punch", "Dream Eater", "Drill Peck", "Drill Run", "Drum Beating", "Dual Chop", "Dual Wingbeat", "Dynamax Cannon", "Dynamic Punch", "Earth Power", "Earthquake", "Echoed Voice", "Eerie Impulse", "Eerie Spell", "Egg Bomb", "Electric Terrain", "Electrify", "Electro Ball", "Electro Drift", "Electroweb", "Embargo", "Ember", "Encore", "Endeavor", "Endure", "Energy Ball", "Entrainment", "Eruption", "Esper Wing", "Eternabeam", "Expanding Force", "Explosion", "Extrasensory", "Extreme Evoboost", "Extreme Speed", "Facade", "Fairy Lock", "Fairy Wind", "Fake Out", "Fake Tears", "False Surrender", "False Swipe", "Feather Dance", "Feint", "Feint Attack", "Fell Stinger", "Fiery Dance", "Fiery Wrath", "Fillet Away", "Final Gambit", "Fire Blast", "Fire Fang", "Fire Lash", "Fire Pledge", "Fire Punch", "Fire Spin", "First Impression", "Fishious Rend", "Fissure", "Flail", "Flame Burst", "Flame Charge", "Flame Wheel", "Flamethrower", "Flare Blitz", "Flash", "Flash Cannon", "Flatter", "Fleur Cannon", "Fling", "Flip Turn", "Floaty Fall", "Floral Healing", "Flower Shield", "Flower Trick", "Fly", "Flying Press", "Focus Blast", "Focus Energy", "Focus Punch", "Follow Me", "Force Palm", "Foresight", "Forest's Curse", "Foul Play", "Freeze-Dry", "Freeze Shock", "Freezing Glare", "Freezy Frost", "Frenzy Plant", "Frost Breath", "Frustration", "Fury Attack", "Fury Cutter", "Fury Swipes", "Fusion Bolt", "Fusion Flare", "Future Sight", "Gastro Acid", "Gear Grind", "Gear Up", "Genesis Supernova", "Geomancy", "Giga Drain", "Giga Impact", "Gigaton Hammer", "Gigavolt Havoc", "Glacial Lance", "Glaciate", "Glaive Rush", "Glare", "Glitzy Glow", "G-Max Befuddle", "G-Max Cannonade", "G-Max Centiferno", "G-Max Chi Strike", "G-Max Cuddle", "G-Max Depletion", "G-Max Drum Solo", "G-Max Finale", "G-Max Fireball", "G-Max Foam Burst", "G-Max Gold Rush", "G-Max Gravitas", "G-Max Hydrosnipe", "G-Max Malodor", "G-Max Meltdown", "G-Max One Blow", "G-Max Rapid Flow", "G-Max Replenish", "G-Max Resonance", "G-Max Sandblast", "G-Max Smite", "G-Max Snooze", "G-Max Steelsurge", "G-Max Stonesurge", "G-Max Stun Shock", "G-Max Sweetness", "G-Max Tartness", "G-Max Terror", "G-Max Vine Lash", "G-Max Volcalith", "G-Max Volt Crash", "G-Max Wildfire", "G-Max Wind Rage", "Grass Knot", "Grass Pledge", "Grass Whistle", "Grassy Glide", "Grassy Terrain", "Grav Apple", "Gravity", "Growl", "Growth", "Grudge", "Guardian of Alola", "Guard Split", "Guard Swap", "Guillotine", "Gunk Shot", "Gust", "Gyro Ball", "Hail", "Hammer Arm", "Happy Hour", "Harden", "Haze", "Headbutt", "Head Charge", "Headlong Rush", "Head Smash", "Heal Bell", "Heal Block", "Healing Wish", "Heal Order", "Heal Pulse", "Heart Stamp", "Heart Swap", "Heat Crash", "Heat Wave", "Heavy Slam", "Helping Hand", "Hex", "Hidden Power", "Hidden Power Bug", "Hidden Power Dark", "Hidden Power Dragon", "Hidden Power Electric", "Hidden Power Fighting", "Hidden Power Fire", "Hidden Power Flying", "Hidden Power Ghost", "Hidden Power Grass", "Hidden Power Ground", "Hidden Power Ice", "Hidden Power Poison", "Hidden Power Psychic", "Hidden Power Rock", "Hidden Power Steel", "Hidden Power Water", "High Horsepower", "High Jump Kick", "Hold Back", "Hold Hands", "Hone Claws", "Horn Attack", "Horn Drill", "Horn Leech", "Howl", "Hurricane", "Hydro Cannon", "Hydro Pump", "Hydro Steam", "Hydro Vortex", "Hyper Beam", "Hyper Drill", "Hyper Fang", "Hyperspace Fury", "Hyperspace Hole", "Hyper Voice", "Hypnosis", "Ice Ball", "Ice Beam", "Ice Burn", "Ice Fang", "Ice Hammer", "Ice Punch", "Ice Shard", "Ice Spinner", "Icicle Crash", "Icicle Spear", "Icy Wind", "Imprison", "Incinerate", "Infernal Parade", "Inferno", "Inferno Overdrive", "Infestation", "Ingrain", "Instruct", "Ion Deluge", "Iron Defense", "Iron Head", "Iron Tail", "Jaw Lock", "Jet Punch", "Judgment", "Jump Kick", "Jungle Healing", "Karate Chop", "Kinesis", "King's Shield", "Knock Off", "Kowtow Cleave", "Land's Wrath", "Laser Focus", "Lash Out", "Last Resort", "Last Respects", "Lava Plume", "Leafage", "Leaf Blade", "Leaf Storm", "Leaf Tornado", "Leech Life", "Leech Seed", "Leer", "Let's Snuggle Forever", "Lick", "Life Dew", "Light of Ruin", "Light Screen", "Light That Burns the Sky", "Liquidation", "Lock-On", "Lovely Kiss", "Low Kick", "Low Sweep", "Lucky Chant", "Lumina Crash", "Lunar Blessing", "Lunar Dance", "Lunge", "Luster Purge", "Mach Punch", "Magical Leaf", "Magical Torque", "Magic Coat", "Magic Powder", "Magic Room", "Magma Storm", "Magnet Bomb", "Magnetic Flux", "Magnet Rise", "Magnitude", "Make It Rain", "Malicious Moonsault", "Mat Block", "Max Airstream", "Max Darkness", "Max Flare", "Max Flutterby", "Max Geyser", "Max Guard", "Max Hailstorm", "Max Knuckle", "Max Lightning", "Max Mindstorm", "Max Ooze", "Max Overgrowth", "Max Phantasm", "Max Quake", "Max Rockfall", "Max Starfall", "Max Steelspike", "Max Strike", "Max Wyrmwind", "Mean Look", "Meditate", "Me First", "Mega Drain", "Megahorn", "Mega Kick", "Mega Punch", "Memento", "Menacing Moonraze Maelstrom", "Metal Burst", "Metal Claw", "Metal Sound", "Meteor Assault", "Meteor Beam", "Meteor Mash", "Metronome", "Milk Drink", "Mimic", "Mind Blown", "Mind Reader", "Minimize", "Miracle Eye", "Mirror Coat", "Mirror Move", "Mirror Shot", "Mist", "Mist Ball", "Misty Explosion", "Misty Terrain", "Moonblast", "Moongeist Beam", "Moonlight", "Morning Sun", "Mortal Spin", "Mountain Gale", "Mud Bomb", "Mud Shot", "Mud-Slap", "Mud Sport", "Muddy Water", "Multi-Attack", "Mystical Fire", "Mystical Power", "Nasty Plot", "Natural Gift", "Nature Power", "Nature's Madness", "Needle Arm", "Never-Ending Nightmare", "Night Daze", "Nightmare", "Night Shade", "Night Slash", "Noble Roar", "No Retreat", "Noxious Torque", "Nuzzle", "Oblivion Wing", "Obstruct", "Oceanic Operetta", "Octazooka", "Octolock", "Odor Sleuth", "Ominous Wind", "Order Up", "Origin Pulse", "Outrage", "Overdrive", "Overheat", "Pain Split", "Parabolic Charge", "Parting Shot", "Payback", "Pay Day", "Peck", "Perish Song", "Petal Blizzard", "Petal Dance", "Phantom Force", "Photon Geyser", "Pika Papow", "Pin Missile", "Plasma Fists", "Play Nice", "Play Rough", "Pluck", "Poison Fang", "Poison Gas", "Poison Jab", "Poison Powder", "Poison Sting", "Poison Tail", "Pollen Puff", "Poltergeist", "Population Bomb", "Pounce", "Pound", "Powder", "Powder Snow", "Power Gem", "Power Shift", "Power Split", "Power Swap", "Power Trick", "Power Trip", "Power-Up Punch", "Power Whip", "Precipice Blades", "Present", "Prismatic Laser", "Protect", "Psybeam", "Psyblade", "Psych Up", "Psychic", "Psychic Fangs", "Psychic Terrain", "Psycho Boost", "Psycho Cut", "Psycho Shift", "Psyshield Bash", "Psyshock", "Psystrike", "Psywave", "Pulverizing Pancake", "Punishment", "Purify", "Pursuit", "Pyro Ball", "Quash", "Quick Attack", "Quick Guard", "Quiver Dance", "Rage", "Rage Fist", "Rage Powder", "Raging Bull", "Raging Fury", "Rain Dance", "Rapid Spin", "Razor Leaf", "Razor Shell", "Razor Wind", "Recover", "Recycle", "Reflect", "Reflect Type", "Refresh", "Relic Song", "Rest", "Retaliate", "Return", "Revelation Dance", "Revenge", "Reversal", "Revival Blessing", "Rising Voltage", "Roar", "Roar of Time", "Rock Blast", "Rock Climb", "Rock Polish", "Rock Slide", "Rock Smash", "Rock Throw", "Rock Tomb", "Rock Wrecker", "Role Play", "Rolling Kick", "Rollout", "Roost", "Rototiller", "Round", "Ruination", "Sacred Fire", "Sacred Sword", "Safeguard", "Salt Cure", "Sand Attack", "Sandsear Storm", "Sandstorm", "Sand Tomb", "Sappy Seed", "Savage Spin-Out", "Scald", "Scale Shot", "Scary Face", "Scorching Sands", "Scratch", "Screech", "Searing Shot", "Searing Sunraze Smash", "Secret Power", "Secret Sword", "Seed Bomb", "Seed Flare", "Seismic Toss", "Self-Destruct", "Shadow Ball", "Shadow Bone", "Shadow Claw", "Shadow Force", "Shadow Punch", "Shadow Sneak", "Sharpen", "Shattered Psyche", "Shed Tail", "Sheer Cold", "Shell Side Arm", "Shell Smash", "Shell Trap", "Shelter", "Shift Gear", "Shock Wave", "Shore Up", "Signal Beam", "Silk Trap", "Silver Wind", "Simple Beam", "Sing", "Sinister Arrow Raid", "Sizzly Slide", "Sketch", "Skill Swap", "Skitter Smack", "Skull Bash", "Sky Attack", "Sky Drop", "Sky Uppercut", "Slack Off", "Slam", "Slash", "Sleep Powder", "Sleep Talk", "Sludge", "Sludge Bomb", "Sludge Wave", "Smack Down", "Smart Strike", "Smelling Salts", "Smog", "Smokescreen", "Snap Trap", "Snarl", "Snatch", "Snipe Shot", "Snore", "Snowscape", "Soak", "Soft-Boiled", "Solar Beam", "Solar Blade", "Sonic Boom", "Soul-Stealing 7-Star Strike", "Spacial Rend", "Spark", "Sparkling Aria", "Sparkly Swirl", "Spectral Thief", "Speed Swap", "Spicy Extract", "Spider Web", "Spike Cannon", "Spikes", "Spiky Shield", "Spin Out", "Spirit Break", "Spirit Shackle", "Spit Up", "Spite", "Splash", "Splintered Stormshards", "Splishy Splash", "Spore", "Spotlight", "Springtide Storm", "Stealth Rock", "Steam Eruption", "Steamroller", "Steel Beam", "Steel Roller", "Steel Wing", "Sticky Web", "Stockpile", "Stoked Sparksurfer", "Stomp", "Stomping Tantrum", "Stone Axe", "Stone Edge", "Stored Power", "Storm Throw", "Strange Steam", "Strength", "Strength Sap", "String Shot", "Struggle", "Struggle Bug", "Stuff Cheeks", "Stun Spore", "Submission", "Substitute", "Subzero Slammer", "Sucker Punch", "Sunny Day", "Sunsteel Strike", "Super Fang", "Superpower", "Supersonic", "Supersonic Skystrike", "Surf", "Surging Strikes", "Swagger", "Swallow", "Sweet Kiss", "Sweet Scent", "Swift", "Switcheroo", "Swords Dance", "Synchronoise", "Synthesis", "Tackle", "Tail Glow", "Tail Slap", "Tail Whip", "Tailwind", "Take Down", "Take Heart", "Tar Shot", "Taunt", "Tearful Look", "Teatime", "Techno Blast", "Tectonic Rage", "Teeter Dance", "Telekinesis", "Teleport", "Tera Blast", "Terrain Pulse", "Thief", "Thousand Arrows", "Thousand Waves", "Thrash", "Throat Chop", "Thunder", "Thunderbolt", "Thunder Cage", "Thunder Fang", "Thunderous Kick", "Thunder Punch", "Thunder Shock", "Thunder Wave", "Tickle", "Tidy Up", "Topsy-Turvy", "Torch Song", "Torment", "Toxic", "Toxic Spikes", "Toxic Thread", "Trailblaze", "Transform", "Tri Attack", "Trick", "Trick-or-Treat", "Trick Room", "Triple Arrows", "Triple Axel", "Triple Dive", "Triple Kick", "Trop Kick", "Trump Card", "Twin Beam", "Twineedle", "Twinkle Tackle", "Twister", "U-turn", "Uproar", "Vacuum Wave", "V-create", "Veevee Volley", "Venom Drench", "Venoshock", "Victory Dance", "Vine Whip", "Vise Grip", "Vital Throw", "Volt Switch", "Volt Tackle", "Wake-Up Slap", "Waterfall", "Water Gun", "Water Pledge", "Water Pulse", "Water Shuriken", "Water Sport", "Water Spout", "Wave Crash", "Weather Ball", "Whirlpool", "Whirlwind", "Wicked Blow", "Wicked Torque", "Wide Guard", "Wildbolt Storm", "Wild Charge", "Will-O-Wisp", "Wing Attack", "Wish", "Withdraw", "Wonder Room", "Wood Hammer", "Work Up", "Worry Seed", "Wrap", "Wring Out", "X-Scissor", "Yawn", "Zap Cannon", "Zen Headbutt", "Zing Zap", "Zippy Zap"};

/** @brief List where `internal::AbilitySmogonNames[ABILITY]` contains the Smogon-style name of the ability as a string */
static inline const std::array<std::string, ABILITY_TOTAL> AbilitySmogonNames = {"", "Adaptability", "Aerilate", "Aftermath", "Air Lock", "Analytic", "Anger Point", "Anger Shell", "Anticipation", "Arena Trap", "Armor Tail", "Aroma Veil", "As One (Glastrier)", "Aura Break", "Bad Dreams", "Ball Fetch", "Battery", "Battle Armor", "Battle Bond", "Beads of Ruin", "Beast Boost", "Berserk", "Big Pecks", "Blaze", "Bulletproof", "Cheek Pouch", "Chilling Neigh", "Chlorophyll", "Clear Body", "Cloud Nine", "Color Change", "Comatose", "Commander", "Competitive", "Compound Eyes", "Contrary", "Corrosion", "Costar", "Cotton Down", "Cud Chew", "Curious Medicine", "Cursed Body", "Cute Charm", "Damp", "Dancer", "Dark Aura", "Dauntless Shield", "Dazzling", "Defeatist", "Defiant", "Delta Stream", "Desolate Land", "Disguise", "Download", "Dragon's Maw", "Drizzle", "Drought", "Dry Skin", "Earth Eater", "Early Bird", "Effect Spore", "Electric Surge", "Electromorphosis", "Emergency Exit", "Fairy Aura", "Filter", "Flame Body", "Flare Boost", "Flash Fire", "Flower Gift", "Flower Veil", "Fluffy", "Forecast", "Forewarn", "Friend Guard", "Frisk", "Full Metal Body", "Fur Coat", "Gale Wings", "Galvanize", "Gluttony", "Good as Gold", "Gooey", "Gorilla Tactics", "Grass Pelt", "Grassy Surge", "Grim Neigh", "Guard Dog", "Gulp Missile", "Guts", "Hadron Engine", "Harvest", "Healer", "Heatproof", "Heavy Metal", "Honey Gather", "Huge Power", "Hunger Switch", "Hustle", "Hydration", "Hyper Cutter", "Ice Body", "Ice Face", "Ice Scales", "Illuminate", "Illusion", "Immunity", "Imposter", "Infiltrator", "Innards Out", "Inner Focus", "Insomnia", "Intimidate", "Intrepid Sword", "Iron Barbs", "Iron Fist", "Justified", "Keen Eye", "Klutz", "Leaf Guard", "Levitate", "Libero", "Light Metal", "Lightning Rod", "Lingering Aroma", "Limber", "Liquid Ooze", "Liquid Voice", "Long Reach", "Magic Bounce", "Magic Guard", "Magician", "Magma Armor", "Magnet Pull", "Marvel Scale", "Mega Launcher", "Merciless", "Mimicry", "Minus", "Mirror Armor", "Misty Surge", "Mold Breaker", "Moody", "Motor Drive", "Moxie", "Multiscale", "Multitype", "Mummy", "Mycelium Might", "Natural Cure", "Neuroforce", "Neutralizing Gas", "No Guard", "Normalize", "Oblivious", "Opportunist", "Orichalcum Pulse", "Overcoat", "Overgrow", "Own Tempo", "Parental Bond", "Pastel Veil", "Perish Body", "Pickpocket", "Pickup", "Pixilate", "Plus", "Poison Heal", "Poison Point", "Poison Touch", "Power Construct", "Power of Alchemy", "Power Spot", "Prankster", "Pressure", "Primordial Sea", "Prism Armor", "Propeller Tail", "Protean", "Protosynthesis", "Psychic Surge", "Punk Rock", "Pure Power", "Purifying Salt", "Quark Drive", "Queenly Majesty", "Quick Draw", "Quick Feet", "Rain Dish", "Rattled", "Receiver", "Reckless", "Refrigerate", "Regenerator", "Ripen", "Rivalry", "RKS System", "Rock Head", "Rocky Payload", "Rough Skin", "Run Away", "Sand Force", "Sand Rush", "Sand Spit", "Sand Stream", "Sand Veil", "Sap Sipper", "Schooling", "Scrappy", "Screen Cleaner", "Seed Sower", "Serene Grace", "Shadow Shield", "Shadow Tag", "Sharpness", "Shed Skin", "Sheer Force", "Shell Armor", "Shield Dust", "Shields Down", "Simple", "Skill Link", "Slow Start", "Slush Rush", "Sniper", "Snow Cloak", "Snow Warning", "Solar Power", "Solid Rock", "Soul-Heart", "Soundproof", "Speed Boost", "Stakeout", "Stall", "Stalwart", "Stamina", "Stance Change", "Static", "Steadfast", "Steam Engine", "Steelworker", "Steely Spirit", "Stench", "Sticky Hold", "Storm Drain", "Strong Jaw", "Sturdy", "Suction Cups", "Super Luck", "Supreme Overlord", "Surge Surfer", "Swarm", "Sweet Veil", "Swift Swim", "Symbiosis", "Synchronize", "Sword of Ruin", "Tablets of Ruin", "Tangled Feet", "Tangling Hair", "Technician", "Telepathy", "Teravolt", "Thermal Exchange", "Thick Fat", "Tinted Lens", "Torrent", "Tough Claws", "Toxic Boost", "Toxic Debris", "Trace", "Transistor", "Triage", "Truant", "Turboblaze", "Unaware", "Unburden", "Unnerve", "Unseen Fist", "Vessel of Ruin", "Victory Star", "Vital Spirit", "Volt Absorb", "Wandering Spirit", "Water Absorb", "Water Bubble", "Water Compaction", "Water Veil", "Weak Armor", "Well-Baked Body", "White Smoke", "Wimp Out", "Wind Power", "Wind Rider", "Wonder Guard", "Wonder Skin", "Zen Mode", "Zero to Hero"};

/**
 * @brief List where `internal::ItemSmogonNames[ITEM]` contains the Smogon-style name of the item as a string
 *
 * @details Many items are not listed in Smogon as they have no effect in battle as held items.
 * A blank string is listed for that item's name in those cases.
 */
static inline const std::array<std::string, ITEM_TOTAL> ItemSmogonNames = {"", "", "", "Ability Shield", "Abomasite", "", "Absolite", "Absorb Bulb", "", "", "", "Adamant Orb", "Adrenaline Orb", "", "Aerodactylite", "", "Aggronite", "Aguav Berry", "Air Balloon", "Mail", "Alakazite", "Aloraichium Z", "Altarianite", "", "Ampharosite", "", "", "Apicot Berry", "", "", "", "Armor Fossil", "", "", "", "Aspear Berry", "Assault Vest", "Audinite", "", "Auspicious Armor", "", "", "", "", "", "", "", "", "Babiri Berry", "", "", "", "Banettite", "", "", "", "", "", "Beast Ball", "Beedrillite", "", "Belue Berry", "Berry", "Berry Juice", "", "", "Berry Sweet", "Berserk Gene", "", "", "", "", "", "", "Big Root", "", "Binding Band", "Bitter Berry", "", "", "Black Belt", "", "Black Glasses", "", "Black Sludge", "", "", "Blastoisinite", "Blazikenite", "", "", "", "", "Blue Orb", "", "", "", "", "", "Bluk Berry", "Blunder Policy", "", "", "", "", "Booster Energy", "Bottle Cap", "", "", "", "", "", "", "", "", "Bright Powder", "", "Bug Gem", "Bug Memory", "", "Buginium Z", "", "", "Burn Drive", "", "Burnt Berry", "", "", "", "Cameruptite", "", "", "", "", "", "", "", "", "", "", "", "Cell Battery", "", "", "Charcoal", "Charizardite X", "Charizardite Y", "", "Charti Berry", "Cheri Berry", "Cherish Ball", "Chesto Berry", "Chilan Berry", "Chill Drive", "Chipped Pot", "Choice Band", "", "Choice Scarf", "Choice Specs", "Chople Berry", "Claw Fossil", "", "Clear Amulet", "", "", "", "Clover Sweet", "Coba Berry", "", "Colbur Berry", "", "", "", "", "", "", "Cornn Berry", "", "", "", "", "", "", "Cover Fossil", "Covert Cloak", "Cracked Pot", "", "", "", "", "", "Custap Berry", "", "Damp Rock", "Dark Gem", "Dark Memory", "", "", "Darkinium Z", "", "", "Dawn Stone", "", "", "Decidium Z", "Deep Sea Scale", "Deep Sea Tooth", "Destiny Knot", "", "", "", "Diancite", "", "", "", "", "", "", "", "", "", "Dive Ball", "", "", "", "Dome Fossil", "", "Douse Drive", "", "", "Draco Plate", "Dragon Fang", "Dragon Gem", "Dragon Memory", "Dragon Scale", "", "", "Dragonium Z", "", "", "Dread Plate", "Dream Ball", "", "", "", "", "Dubious Disc", "Durin Berry", "Dusk Ball", "Dusk Stone", "", "", "", "", "Earth Plate", "", "Eevium Z", "", "", "", "", "", "", "", "Eject Button", "Eject Pack", "", "", "Electirizer", "Electric Gem", "Electric Memory", "Electric Seed", "", "Electrium Z", "", "", "", "Energy Powder", "", "Enigma Berry", "", "", "", "", "", "", "", "", "", "Eviolite", "", "", "", "", "", "", "", "", "", "Expert Belt", "", "", "Fairium Z", "Fairy Gem", "Fairy Memory", "", "", "", "", "Fast Ball", "", "", "", "", "Fighting Gem", "Fighting Memory", "", "Fightinium Z", "Figy Berry", "", "Fire Gem", "Fire Memory", "Fire Stone", "", "Firium Z", "", "Fist Plate", "", "Flame Orb", "Flame Plate", "Float Stone", "", "Flower Sweet", "", "Flying Gem", "Flying Memory", "", "Flyinium Z", "Focus Band", "Focus Sash", "", "", "Fossilized Bird", "Fossilized Dino", "Fossilized Drake", "Fossilized Fish", "", "Friend Ball", "", "Full Incense", "", "", "Galarica Cuff", "", "Galarica Wreath", "Galladite", "Ganlon Berry", "Garchompite", "Gardevoirite", "", "", "", "Gengarite", "", "", "", "", "Ghost Gem", "Ghost Memory", "", "Ghostium Z", "", "", "Glalitite", "", "", "", "Gold Berry", "Gold Bottle Cap", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "Grass Gem", "Grass Memory", "", "Grassium Z", "", "Grassy Seed", "Great Ball", "", "", "", "", "", "Grepa Berry", "", "Grip Claw", "", "", "", "", "", "Griseous Orb", "", "Ground Gem", "Ground Memory", "", "Groundium Z", "", "", "", "", "", "", "Gyaradosite", "Haban Berry", "", "Hard Stone", "", "Heal Ball", "", "", "", "", "", "", "", "", "Heat Rock", "Heavy Ball", "Heavy-Duty Boots", "Helix Fossil", "Heracronite", "", "", "", "", "", "", "", "", "", "", "", "", "", "Hondew Berry", "", "", "", "", "", "Houndoominite", "", "", "Iapapa Berry", "Ice Berry", "Ice Gem", "", "Ice Memory", "Ice Stone", "", "", "Icicle Plate", "Icium Z", "Icy Rock", "", "", "", "Incinium Z", "", "Insect Plate", "", "", "Iron Ball", "", "", "Iron Plate", "Jaboca Berry", "", "", "Jaw Fossil", "", "", "", "", "", "", "", "", "", "Kangaskhanite", "", "Kasib Berry", "Kebia Berry", "Kee Berry", "Kelpsy Berry", "", "", "", "", "", "", "King's Rock", "", "", "Kommonium Z", "", "", "", "", "", "", "", "", "Lagging Tail", "Lansat Berry", "", "Latiasite", "Latiosite", "", "Lax Incense", "", "", "", "", "", "Leaf Stone", "Leek", "", "Leftovers", "", "", "", "", "", "", "", "Leppa Berry", "", "Level Ball", "", "", "Liechi Berry", "Life Orb", "", "Light Ball", "Light Clay", "", "", "", "", "Loaded Dice", "", "", "", "", "", "Lopunnite", "", "", "Love Ball", "Love Sweet", "", "Lucarionite", "", "", "Lucky Punch", "Lum Berry", "Luminous Moss", "", "Lunalium Z", "", "", "Lure Ball", "", "Lustrous Orb", "Luxury Ball", "Lycanium Z", "", "", "Macho Brace", "", "", "", "", "", "", "Magmarizer", "", "Magnet", "Mago Berry", "Magost Berry", "", "", "Malicious Armor", "Manectite", "", "Maranga Berry", "", "", "Marshadium Z", "", "Master Ball", "Mawilite", "", "", "", "", "", "", "", "", "", "Meadow Plate", "", "", "", "", "", "", "", "Medichamite", "", "", "", "", "", "Mental Herb", "", "", "Metagrossite", "Metal Coat", "Metal Powder", "", "", "Metronome", "", "Mewnium Z", "", "Mewtwonite X", "Mewtwonite Y", "Micle Berry", "", "", "", "", "Mimikium Z", "Mind Plate", "Mint Berry", "Miracle Berry", "Miracle Seed", "", "", "", "Mirror Herb", "Misty Seed", "", "", "", "Moon Ball", "", "", "Moon Stone", "", "", "", "", "Muscle Band", "", "", "", "", "", "", "", "Mystery Berry", "", "Mystic Water", "", "", "Nanab Berry", "", "Nest Ball", "Net Ball", "Never-Melt Ice", "", "", "", "", "Nomel Berry", "", "Normal Gem", "", "Normalium Z", "", "", "", "", "", "Occa Berry", "", "Odd Incense", "", "", "Old Amber", "", "", "", "", "", "", "", "", "", "Oran Berry", "", "", "", "", "", "Oval Stone", "", "", "Pamtre Berry", "", "", "", "Park Ball", "", "", "Passho Berry", "Payapa Berry", "", "", "", "Pecha Berry", "", "", "Persim Berry", "Petaya Berry", "", "", "Pidgeotite", "", "", "Pikanium Z", "Pikashunium Z", "Pinap Berry", "", "Pink Bow", "", "", "", "", "Pinsirite", "Pixie Plate", "", "Plume Fossil", "", "", "", "Poison Barb", "Poison Gem", "Poison Memory", "", "Poisonium Z", "Poke Ball", "", "", "", "", "", "", "", "", "", "", "Polkadot Bow", "", "", "Pomeg Berry", "", "", "", "", "", "Power Anklet", "Power Band", "Power Belt", "Power Bracer", "Power Herb", "Power Lens", "", "Power Weight", "", "", "", "", "Premier Ball", "", "Primarium Z", "Prism Scale", "", "", "", "", "Protective Pads", "Protector", "", "PRZ Cure Berry", "PSN Cure Berry", "Psychic Gem", "Psychic Memory", "Psychic Seed", "", "Psychium Z", "", "", "Punching Glove", "", "", "", "Qualot Berry", "Quick Ball", "", "", "", "Quick Claw", "Quick Powder", "", "Rabuta Berry", "", "", "", "", "", "", "Rare Bone", "", "", "", "Rawst Berry", "Razor Claw", "Razor Fang", "Razz Berry", "", "Reaper Cloth", "", "", "Red Card", "", "", "", "", "Red Orb", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "Repeat Ball", "", "", "", "", "", "", "", "", "Ribbon Sweet", "", "", "Rindo Berry", "Ring Target", "Rock Gem", "Rock Incense", "Rock Memory", "", "Rockium Z", "Rocky Helmet", "", "Room Service", "Root Fossil", "Rose Incense", "Roseli Berry", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "Rowap Berry", "", "", "", "Rusted Shield", "Rusted Sword", "", "Sablenite", "Sachet", "", "Safari Ball", "Safety Goggles", "Sail Fossil", "Salac Berry", "Salamencite", "", "", "", "", "", "", "", "", "", "Sceptilite", "Scizorite", "Scope Lens", "", "", "", "Sea Incense", "", "", "", "", "", "", "", "", "", "Sharp Beak", "Sharpedonite", "Shed Shell", "Shell Bell", "", "", "", "Shiny Stone", "", "", "Shock Drive", "Shuca Berry", "Silk Scarf", "", "", "", "", "Silver Powder", "", "", "Sitrus Berry", "Skull Fossil", "Sky Plate", "", "Slowbronite", "", "", "", "", "", "", "", "", "", "Smooth Rock", "", "Snorlium Z", "Snowball", "", "", "", "Soft Sand", "Solganium Z", "", "", "", "", "Soul Dew", "", "", "", "", "", "Spell Tag", "Spelon Berry", "Splash Plate", "", "Spooky Plate", "Sport Ball", "", "", "", "", "", "", "", "", "Star Sweet", "", "Starf Berry", "", "", "Steel Gem", "", "Steel Memory", "", "", "Steelium Z", "Steelixite", "Sticky Barb", "", "Stone Plate", "", "", "", "", "Strawberry Sweet", "", "", "", "", "", "", "", "Sun Stone", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "Swampertite", "", "Sweet Apple", "", "", "", "", "Tamato Berry", "Tanga Berry", "", "Tapunium Z", "Tart Apple", "", "", "", "", "", "", "", "", "", "", "", "Terrain Extender", "", "", "Thick Club", "Throat Spray", "Thunder Stone", "", "", "", "Timer Ball", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "TR01", "TR02", "TR03", "TR04", "TR05", "TR06", "TR07", "TR08", "TR09", "TR10", "TR11", "TR12", "TR13", "TR14", "TR15", "TR16", "TR17", "TR18", "TR19", "TR20", "TR21", "TR22", "TR23", "TR24", "TR25", "TR26", "TR27", "TR28", "TR29", "TR30", "TR31", "TR32", "TR33", "TR34", "TR35", "TR36", "TR37", "TR38", "TR39", "TR40", "TR41", "TR42", "TR43", "TR44", "TR45", "TR46", "TR47", "TR48", "TR49", "TR50", "TR51", "TR52", "TR53", "TR54", "TR55", "TR56", "TR57", "TR58", "TR59", "TR60", "TR61", "TR62", "TR63", "TR64", "TR65", "TR66", "TR67", "TR68", "TR69", "TR70", "TR71", "TR72", "TR73", "TR74", "TR75", "TR76", "TR77", "TR78", "TR79", "TR80", "TR81", "TR82", "TR83", "TR84", "TR85", "TR86", "TR87", "TR88", "TR89", "TR90", "TR91", "TR92", "TR93", "TR94", "TR95", "TR96", "TR97", "TR98", "TR99", "", "", "", "", "", "", "", "", "Toxic Orb", "Toxic Plate", "", "", "", "", "", "", "", "Twisted Spoon", "Tyranitarite", "", "Ultra Ball", "Ultranecrozium Z", "", "", "Up-Grade", "Utility Umbrella", "", "", "Venusaurite", "", "", "", "", "", "", "", "", "", "", "", "", "", "Wacan Berry", "", "", "", "", "", "", "", "Water Gem", "Water Memory", "Water Stone", "", "Waterium Z", "Watmel Berry", "Wave Incense", "", "Weakness Policy", "", "Wepear Berry", "Whipped Dream", "", "", "White Herb", "", "Wide Lens", "Wiki Berry", "", "Wise Glasses", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "Yache Berry", "", "", "", "", "", "", "", "", "Zap Plate", "", "", "Zoom Lens", "", "", "", ""};

/** @brief List where `SpeciesSmogonIDs[SPECIES]` contains the Smogon-style id of the species as a string */
static inline const std::array<std::string, SPECIES_TOTAL> SpeciesSmogonIDs = {"", "bulbasaur", "ivysaur", "venusaur", "venusaurmega", "venusaurgmax", "charmander", "charmeleon", "charizard", "charizardmegax", "charizardmegay", "charizardgmax", "squirtle", "wartortle", "blastoise", "blastoisemega", "blastoisegmax", "caterpie", "metapod", "butterfree", "butterfreegmax", "weedle", "kakuna", "beedrill", "beedrillmega", "pidgey", "pidgeotto", "pidgeot", "pidgeotmega", "rattata", "rattataalola", "raticate", "raticatealola", "raticatealolatotem", "spearow", "fearow", "ekans", "arbok", "pikachu", "pikachucosplay", "pikachurockstar", "pikachubelle", "pikachupopstar", "pikachuphd", "pikachulibre", "pikachuoriginal", "pikachuhoenn", "pikachusinnoh", "pikachuunova", "pikachukalos", "pikachualola", "pikachupartner", "pikachustarter", "pikachugmax", "pikachuworld", "raichu", "raichualola", "sandshrew", "sandshrewalola", "sandslash", "sandslashalola", "nidoranf", "nidorina", "nidoqueen", "nidoranm", "nidorino", "nidoking", "clefairy", "clefable", "vulpix", "vulpixalola", "ninetales", "ninetalesalola", "jigglypuff", "wigglytuff", "zubat", "golbat", "oddish", "gloom", "vileplume", "paras", "parasect", "venonat", "venomoth", "diglett", "diglettalola", "dugtrio", "dugtrioalola", "meowth", "meowthalola", "meowthgalar", "meowthgmax", "persian", "persianalola", "psyduck", "golduck", "mankey", "primeape", "growlithehisui", "growlithe", "arcaninehisui", "arcanine", "poliwag", "poliwhirl", "poliwrath", "abra", "kadabra", "alakazam", "alakazammega", "machop", "machoke", "machamp", "machampgmax", "bellsprout", "weepinbell", "victreebel", "tentacool", "tentacruel", "geodude", "geodudealola", "graveler", "graveleralola", "golem", "golemalola", "ponyta", "ponytagalar", "rapidash", "rapidashgalar", "slowpoke", "slowpokegalar", "slowbro", "slowbromega", "slowbrogalar", "magnemite", "magneton", "farfetchd", "farfetchdgalar", "doduo", "dodrio", "seel", "dewgong", "grimer", "grimeralola", "muk", "mukalola", "shellder", "cloyster", "gastly", "haunter", "gengar", "gengarmega", "gengargmax", "onix", "drowzee", "hypno", "krabby", "kingler", "kinglergmax", "voltorb", "voltorbhisui", "electrode", "electrodehisui", "exeggcute", "exeggutor", "exeggutoralola", "cubone", "marowak", "marowakalola", "marowakalolatotem", "hitmonlee", "hitmonchan", "lickitung", "koffing", "weezing", "weezinggalar", "rhyhorn", "rhydon", "chansey", "tangela", "kangaskhan", "kangaskhanmega", "horsea", "seadra", "goldeen", "seaking", "staryu", "starmie", "mrmime", "mrmimegalar", "scyther", "jynx", "electabuzz", "magmar", "pinsir", "pinsirmega", "tauros", "taurospaldea", "taurospaldeafire", "taurospaldeawater", "magikarp", "gyarados", "gyaradosmega", "lapras", "laprasgmax", "ditto", "eevee", "eeveestarter", "eeveegmax", "vaporeon", "jolteon", "flareon", "porygon", "omanyte", "omastar", "kabuto", "kabutops", "aerodactyl", "aerodactylmega", "snorlax", "snorlaxgmax", "articuno", "articunogalar", "zapdos", "zapdosgalar", "moltres", "moltresgalar", "dratini", "dragonair", "dragonite", "mewtwo", "mewtwomegax", "mewtwomegay", "mew", "chikorita", "bayleef", "meganium", "cyndaquil", "quilava", "typhlosionhisui", "typhlosion", "totodile", "croconaw", "feraligatr", "sentret", "furret", "hoothoot", "noctowl", "ledyba", "ledian", "spinarak", "ariados", "crobat", "chinchou", "lanturn", "pichu", "pichuspikyeared", "cleffa", "igglybuff", "togepi", "togetic", "natu", "xatu", "mareep", "flaaffy", "ampharos", "ampharosmega", "bellossom", "marill", "azumarill", "sudowoodo", "politoed", "hoppip", "skiploom", "jumpluff", "aipom", "sunkern", "sunflora", "yanma", "wooper", "wooperpaldea", "quagsire", "espeon", "umbreon", "murkrow", "slowking", "slowkinggalar", "misdreavus", "unown", "unown", "unownb", "unownc", "unownd", "unowne", "unownf", "unowng", "unownh", "unowni", "unownj", "unownk", "unownl", "unownm", "unownn", "unowno", "unownp", "unownq", "unownr", "unowns", "unownt", "unownu", "unownv", "unownw", "unownx", "unowny", "unownz", "unownexclamation", "unownquestion", "wobbuffet", "girafarig", "pineco", "forretress", "dunsparce", "gligar", "steelix", "steelixmega", "snubbull", "granbull", "qwilfish", "qwilfishhisui", "scizor", "scizormega", "shuckle", "heracross", "heracrossmega", "sneaselhisui", "sneasel", "teddiursa", "ursaring", "slugma", "magcargo", "swinub", "piloswine", "corsola", "corsolagalar", "remoraid", "octillery", "delibird", "mantine", "skarmory", "houndour", "houndoom", "houndoommega", "kingdra", "phanpy", "donphan", "porygon2", "stantler", "smeargle", "tyrogue", "hitmontop", "smoochum", "elekid", "magby", "miltank", "blissey", "raikou", "entei", "suicune", "larvitar", "pupitar", "tyranitar", "tyranitarmega", "lugia", "hooh", "celebi", "treecko", "grovyle", "sceptile", "sceptilemega", "torchic", "combusken", "blaziken", "blazikenmega", "mudkip", "marshtomp", "swampert", "swampertmega", "poochyena", "mightyena", "zigzagoon", "zigzagoongalar", "linoone", "linoonegalar", "wurmple", "silcoon", "beautifly", "cascoon", "dustox", "lotad", "lombre", "ludicolo", "seedot", "nuzleaf", "shiftry", "taillow", "swellow", "wingull", "pelipper", "ralts", "kirlia", "gardevoir", "gardevoirmega", "surskit", "masquerain", "shroomish", "breloom", "slakoth", "vigoroth", "slaking", "nincada", "ninjask", "shedinja", "whismur", "loudred", "exploud", "makuhita", "hariyama", "azurill", "nosepass", "skitty", "delcatty", "sableye", "sableyemega", "mawile", "mawilemega", "aron", "lairon", "aggron", "aggronmega", "meditite", "medicham", "medichammega", "electrike", "manectric", "manectricmega", "plusle", "minun", "volbeat", "illumise", "roselia", "gulpin", "swalot", "carvanha", "sharpedo", "sharpedomega", "wailmer", "wailord", "numel", "camerupt", "cameruptmega", "torkoal", "spoink", "grumpig", "spinda", "trapinch", "vibrava", "flygon", "cacnea", "cacturne", "swablu", "altaria", "altariamega", "zangoose", "seviper", "lunatone", "solrock", "barboach", "whiscash", "corphish", "crawdaunt", "baltoy", "claydol", "lileep", "cradily", "anorith", "armaldo", "feebas", "milotic", "castform", "castformsunny", "castformrainy", "castformsnowy", "kecleon", "shuppet", "banette", "banettemega", "duskull", "dusclops", "tropius", "chimecho", "absol", "absolmega", "wynaut", "snorunt", "glalie", "glaliemega", "spheal", "sealeo", "walrein", "clamperl", "huntail", "gorebyss", "relicanth", "luvdisc", "bagon", "shelgon", "salamence", "salamencemega", "beldum", "metang", "metagross", "metagrossmega", "regirock", "regice", "registeel", "latias", "latiasmega", "latios", "latiosmega", "kyogre", "kyogreprimal", "groudon", "groudonprimal", "rayquaza", "rayquazamega", "jirachi", "deoxys", "deoxysattack", "deoxysdefense", "deoxysspeed", "turtwig", "grotle", "torterra", "chimchar", "monferno", "infernape", "piplup", "prinplup", "empoleon", "starly", "staravia", "staraptor", "bidoof", "bibarel", "kricketot", "kricketune", "shinx", "luxio", "luxray", "budew", "roserade", "cranidos", "rampardos", "shieldon", "bastiodon", "burmy", "burmy", "burmysandy", "burmytrash", "wormadam", "wormadamsandy", "wormadamtrash", "mothim", "combee", "vespiquen", "pachirisu", "buizel", "floatzel", "cherubi", "cherrim", "cherrim", "cherrimsunshine", "shellos", "shellos", "shelloseast", "gastrodon", "gastrodon", "gastrodoneast", "ambipom", "drifloon", "drifblim", "buneary", "lopunny", "lopunnymega", "mismagius", "honchkrow", "glameow", "purugly", "chingling", "stunky", "skuntank", "bronzor", "bronzong", "bonsly", "mimejr", "happiny", "chatot", "spiritomb", "gible", "gabite", "garchomp", "garchompmega", "munchlax", "riolu", "lucario", "lucariomega", "hippopotas", "hippowdon", "skorupi", "drapion", "croagunk", "toxicroak", "carnivine", "finneon", "lumineon", "mantyke", "snover", "abomasnow", "abomasnowmega", "weavile", "magnezone", "lickilicky", "rhyperior", "tangrowth", "electivire", "magmortar", "togekiss", "yanmega", "leafeon", "glaceon", "gliscor", "mamoswine", "porygonz", "gallade", "gallademega", "probopass", "dusknoir", "froslass", "rotom", "rotomheat", "rotomwash", "rotomfrost", "rotomfan", "rotommow", "uxie", "mesprit", "azelf", "dialga", "dialgaorigin", "palkia", "palkiaorigin", "heatran", "regigigas", "giratina", "giratinaorigin", "cresselia", "phione", "manaphy", "darkrai", "shaymin", "shaymin", "shayminsky", "arceus", "arceusbug", "arceusdark", "arceusdragon", "arceuselectric", "arceusfairy", "arceusfighting", "arceusfire", "arceusflying", "arceusghost", "arceusgrass", "arceusground", "arceusice", "arceuspoison", "arceuspsychic", "arceusrock", "arceussteel", "arceuswater", "victini", "snivy", "servine", "serperior", "tepig", "pignite", "emboar", "oshawott", "dewott", "samurotthisui", "samurott", "patrat", "watchog", "lillipup", "herdier", "stoutland", "purrloin", "liepard", "pansage", "simisage", "pansear", "simisear", "panpour", "simipour", "munna", "musharna", "pidove", "tranquill", "unfezant", "blitzle", "zebstrika", "roggenrola", "boldore", "gigalith", "woobat", "swoobat", "drilbur", "excadrill", "audino", "audinomega", "timburr", "gurdurr", "conkeldurr", "tympole", "palpitoad", "seismitoad", "throh", "sawk", "sewaddle", "swadloon", "leavanny", "venipede", "whirlipede", "scolipede", "cottonee", "whimsicott", "petilil", "lilligant", "lilliganthisui", "basculin", "basculinbluestriped", "basculinwhitestriped", "sandile", "krokorok", "krookodile", "darumaka", "darumakagalar", "darmanitan", "darmanitanzen", "darmanitangalar", "darmanitangalarzen", "maractus", "dwebble", "crustle", "scraggy", "scrafty", "sigilyph", "yamask", "yamaskgalar", "cofagrigus", "tirtouga", "carracosta", "archen", "archeops", "trubbish", "garbodor", "garbodorgmax", "zorua", "zoruahisui", "zoroarkhisui", "zoroark", "minccino", "cinccino", "gothita", "gothorita", "gothitelle", "solosis", "duosion", "reuniclus", "ducklett", "swanna", "vanillite", "vanillish", "vanilluxe", "deerling", "deerling", "deerlingsummer", "deerlingautumn", "deerlingwinter", "sawsbuck", "sawsbuck", "sawsbucksummer", "sawsbuckautumn", "sawsbuckwinter", "emolga", "karrablast", "escavalier", "foongus", "amoonguss", "frillish", "jellicent", "alomomola", "joltik", "galvantula", "ferroseed", "ferrothorn", "klink", "klang", "klinklang", "tynamo", "eelektrik", "eelektross", "elgyem", "beheeyem", "litwick", "lampent", "chandelure", "axew", "fraxure", "haxorus", "cubchoo", "beartic", "cryogonal", "shelmet", "accelgor", "stunfisk", "stunfiskgalar", "mienfoo", "mienshao", "druddigon", "golett", "golurk", "pawniard", "bisharp", "bouffalant", "rufflet", "braviaryhisui", "braviary", "vullaby", "mandibuzz", "heatmor", "durant", "deino", "zweilous", "hydreigon", "larvesta", "volcarona", "cobalion", "terrakion", "virizion", "tornadus", "tornadustherian", "thundurus", "thundurustherian", "reshiram", "zekrom", "landorus", "landorustherian", "kyurem", "kyuremblack", "kyuremwhite", "keldeo", "keldeoresolute", "meloetta", "meloettapirouette", "genesect", "genesectdouse", "genesectshock", "genesectburn", "genesectchill", "chespin", "quilladin", "chesnaught", "fennekin", "braixen", "delphox", "froakie", "frogadier", "greninja", "greninjaash", "bunnelby", "diggersby", "fletchling", "fletchinder", "talonflame", "scatterbug", "spewpa", "vivillon", "vivillon", "vivillonarchipelago", "vivilloncontinental", "vivillonelegant", "vivillongarden", "vivillonhighplains", "vivillonicysnow", "vivillonjungle", "vivillonmarine", "vivillonmodern", "vivillonmonsoon", "vivillonocean", "vivillonpolar", "vivillonriver", "vivillonsandstorm", "vivillonsavanna", "vivillonsun", "vivillontundra", "vivillonfancy", "vivillonpokeball", "litleo", "pyroar", "flabebe", "flabebe", "flabebeblue", "flabebeorange", "flabebewhite", "flabebeyellow", "floette", "floette", "floetteblue", "floetteorange", "floettewhite", "floetteyellow", "floetteeternal", "florges", "florges", "florgesblue", "florgesorange", "florgeswhite", "florgesyellow", "skiddo", "gogoat", "pancham", "pangoro", "furfrou", "furfrou", "furfroudandy", "furfroudebutante", "furfroudiamond", "furfrouheart", "furfroukabuki", "furfroulareine", "furfroumatron", "furfroupharaoh", "furfroustar", "espurr", "meowstic", "meowsticf", "honedge", "doublade", "aegislash", "aegislash", "aegislashblade", "spritzee", "aromatisse", "swirlix", "slurpuff", "inkay", "malamar", "binacle", "barbaracle", "skrelp", "dragalge", "clauncher", "clawitzer", "helioptile", "heliolisk", "tyrunt", "tyrantrum", "amaura", "aurorus", "sylveon", "hawlucha", "dedenne", "carbink", "goomy", "sliggoohisui", "sliggoo", "goodrahisui", "goodra", "klefki", "phantump", "trevenant", "pumpkaboo", "pumpkaboosmall", "pumpkaboolarge", "pumpkaboosuper", "gourgeist", "gourgeistsmall", "gourgeistlarge", "gourgeistsuper", "bergmite", "avalugg", "avalugghisui", "noibat", "noivern", "xerneas", "yveltal", "zygarde", "zygarde10", "zygardecomplete", "diancie", "dianciemega", "hoopa", "hoopaunbound", "volcanion", "rowlet", "dartrix", "decidueyehisui", "decidueye", "litten", "torracat", "incineroar", "popplio", "brionne", "primarina", "pikipek", "trumbeak", "toucannon", "yungoos", "gumshoos", "gumshoostotem", "grubbin", "charjabug", "vikavolt", "vikavolttotem", "crabrawler", "crabominable", "oricorio", "oricoriopompom", "oricoriopau", "oricoriosensu", "cutiefly", "ribombee", "ribombeetotem", "rockruff", "rockruff", "lycanroc", "lycanrocmidnight", "lycanrocdusk", "wishiwashi", "wishiwashi", "wishiwashischool", "mareanie", "toxapex", "mudbray", "mudsdale", "dewpider", "araquanid", "araquanidtotem", "fomantis", "lurantis", "lurantistotem", "morelull", "shiinotic", "salandit", "salazzle", "salazzletotem", "stufful", "bewear", "bounsweet", "steenee", "tsareena", "comfey", "oranguru", "passimian", "wimpod", "golisopod", "sandygast", "palossand", "pyukumuku", "typenull", "silvally", "silvallybug", "silvallydark", "silvallydragon", "silvallyelectric", "silvallyfairy", "silvallyfighting", "silvallyfire", "silvallyflying", "silvallyghost", "silvallygrass", "silvallyground", "silvallyice", "silvallypoison", "silvallypsychic", "silvallyrock", "silvallysteel", "silvallywater", "minior", "minior", "minior", "miniororange", "minioryellow", "miniorgreen", "miniorblue", "miniorindigo", "miniorviolet", "miniormeteor", "komala", "turtonator", "togedemaru", "togedemarutotem", "mimikyu", "mimikyubusted", "mimikyutotem", "mimikyubustedtotem", "bruxish", "drampa", "dhelmise", "jangmoo", "hakamoo", "kommoo", "kommoototem", "tapukoko", "tapulele", "tapubulu", "tapufini", "cosmog", "cosmoem", "solgaleo", "lunala", "nihilego", "buzzwole", "pheromosa", "xurkitree", "celesteela", "kartana", "guzzlord", "necrozma", "necrozmaduskmane", "necrozmadawnwings", "necrozmaultra", "magearna", "magearnaoriginal", "marshadow", "poipole", "naganadel", "stakataka", "blacephalon", "zeraora", "meltan", "melmetal", "melmetalgmax", "grookey", "thwackey", "rillaboom", "rillaboomgmax", "scorbunny", "raboot", "cinderace", "cinderacegmax", "sobble", "drizzile", "inteleon", "inteleongmax", "skwovet", "greedent", "rookidee", "corvisquire", "corviknight", "corviknightgmax", "blipbug", "dottler", "orbeetle", "orbeetlegmax", "nickit", "thievul", "gossifleur", "eldegoss", "wooloo", "dubwool", "chewtle", "drednaw", "drednawgmax", "yamper", "boltund", "rolycoly", "carkol", "coalossal", "coalossalgmax", "applin", "flapple", "flapplegmax", "appletun", "appletungmax", "silicobra", "sandaconda", "sandacondagmax", "cramorant", "cramorantgulping", "cramorantgorging", "arrokuda", "barraskewda", "toxel", "toxtricity", "toxtricitylowkey", "toxtricitygmax", "toxtricitylowkeygmax", "sizzlipede", "centiskorch", "centiskorchgmax", "clobbopus", "grapploct", "sinistea", "sinisteaantique", "polteageist", "polteageistantique", "hatenna", "hattrem", "hatterene", "hatterenegmax", "impidimp", "morgrem", "grimmsnarl", "grimmsnarlgmax", "obstagoon", "perrserker", "cursola", "sirfetchd", "mrrime", "runerigus", "milcery", "alcremie", "alcremie", "alcremierubycream", "alcremiematchacream", "alcremiemintcream", "alcremielemoncream", "alcremiesaltedcream", "alcremierubyswirl", "alcremiecaramelswirl", "alcremierainbowswirl", "alcremiegmax", "falinks", "pincurchin", "snom", "frosmoth", "stonjourner", "eiscue", "eiscue", "eiscuenoice", "indeedee", "indeedeef", "morpeko", "morpeko", "morpekohangry", "cufant", "copperajah", "copperajahgmax", "dracozolt", "arctozolt", "dracovish", "arctovish", "duraludon", "duraludongmax", "dreepy", "drakloak", "dragapult", "zacian", "zaciancrowned", "zamazenta", "zamazentacrowned", "eternatus", "eternatuseternamax", "kubfu", "urshifu", "urshifurapidstrike", "urshifugmax", "urshifurapidstrikegmax", "zarude", "zarudedada", "regieleki", "regidrago", "glastrier", "spectrier", "calyrex", "calyrexice", "calyrexshadow", "wyrdeer", "kleavor", "ursaluna", "basculegion", "basculegionf", "sneasler", "overqwil", "enamorus", "enamorustherian", "sprigatito", "floragato", "meowscarada", "fuecoco", "crocalor", "skeledirge", "quaxly", "quaxwell", "quaquaval", "lechonk", "oinkologne", "oinkolognef", "tarountula", "spidops", "nymble", "lokix", "pawmi", "pawmo", "pawmot", "tandemaus", "maushold", "maushold", "mausholdfour", "fidough", "dachsbun", "smoliv", "dolliv", "arboliva", "squawkabilly", "squawkabillyblue", "squawkabillyyellow", "squawkabillywhite", "nacli", "naclstack", "garganacl", "charcadet", "armarouge", "ceruledge", "tadbulb", "bellibolt", "wattrel", "kilowattrel", "maschiff", "mabosstiff", "shroodle", "grafaiai", "bramblin", "brambleghast", "toedscool", "toedscruel", "klawf", "capsakid", "scovillain", "rellor", "rabsca", "flittle", "espathra", "tinkatink", "tinkatuff", "tinkaton", "wiglett", "wugtrio", "bombirdier", "finizen", "palafin", "palafinhero", "varoom", "revavroom", "cyclizar", "orthworm", "glimmet", "glimmora", "greavard", "houndstone", "flamigo", "cetoddle", "cetitan", "veluza", "dondozo", "tatsugiri", "tatsugiri", "tatsugiridroopy", "tatsugiristretchy", "annihilape", "clodsire", "farigiraf", "dudunsparce", "dudunsparce", "dudunsparcethreesegment", "kingambit", "greattusk", "screamtail", "brutebonnet", "fluttermane", "slitherwing", "sandyshocks", "irontreads", "ironbundle", "ironhands", "ironjugulis", "ironmoth", "ironthorns", "frigibax", "arctibax", "baxcalibur", "gimmighoul", "gimmighoulroaming", "gholdengo", "wochien", "chienpao", "tinglu", "chiyu", "roaringmoon", "ironvaliant", "koraidon", "miraidon", "walkingwake", "ironleaves"};

/** @brief List where `MoveSmogonIDs[MOVE]` contains the Smogon-style id of the move as a string */
static inline const std::array<std::string, MOVE_TOTAL> MoveSmogonIDs =  {"", "10000000voltthunderbolt", "absorb", "accelerock", "acid", "acidarmor", "aciddownpour", "acidspray", "acrobatics", "acupressure", "aerialace", "aeroblast", "afteryou", "agility", "aircutter", "airslash", "alloutpummeling", "allyswitch", "amnesia", "anchorshot", "ancientpower", "appleacid", "aquacutter", "aquajet", "aquaring", "aquastep", "aquatail", "armorcannon", "armthrust", "aromatherapy", "aromaticmist", "assist", "assurance", "astonish", "astralbarrage", "attackorder", "attract", "aurasphere", "aurawheel", "aurorabeam", "auroraveil", "autotomize", "avalanche", "axekick", "babydolleyes", "baddybad", "banefulbunker", "barbbarrage", "barrage", "barrier", "batonpass", "beakblast", "beatup", "behemothbash", "behemothblade", "belch", "bellydrum", "bestow", "bide", "bind", "bite", "bitterblade", "bittermalice", "blackholeeclipse", "blastburn", "blazekick", "blazingtorque", "bleakwindstorm", "blizzard", "block", "bloomdoom", "blueflare", "bodypress", "bodyslam", "boltbeak", "boltstrike", "boneclub", "bonemerang", "bonerush", "boomburst", "bounce", "bouncybubble", "branchpoke", "bravebird", "breakingswipe", "breakneckblitz", "brickbreak", "brine", "brutalswing", "bubble", "bubblebeam", "bugbite", "bugbuzz", "bulkup", "bulldoze", "bulletpunch", "bulletseed", "burningjealousy", "burnup", "buzzybuzz", "calmmind", "camouflage", "captivate", "catastropika", "ceaselessedge", "celebrate", "charge", "chargebeam", "charm", "chatter", "chillingwater", "chillyreception", "chipaway", "chloroblast", "circlethrow", "clamp", "clangingscales", "clangoroussoul", "clangoroussoulblaze", "clearsmog", "closecombat", "coaching", "coil", "collisioncourse", "combattorque", "cometpunch", "comeuppance", "confide", "confuseray", "confusion", "constrict", "continentalcrush", "conversion", "conversion2", "copycat", "coreenforcer", "corkscrewcrash", "corrosivegas", "cosmicpower", "cottonguard", "cottonspore", "counter", "courtchange", "covet", "crabhammer", "craftyshield", "crosschop", "crosspoison", "crunch", "crushclaw", "crushgrip", "curse", "cut", "darkestlariat", "darkpulse", "darkvoid", "dazzlinggleam", "decorate", "defendorder", "defensecurl", "defog", "destinybond", "detect", "devastatingdrake", "diamondstorm", "dig", "disable", "disarmingvoice", "discharge", "direclaw", "dive", "dizzypunch", "doodle", "doomdesire", "doubleedge", "doublehit", "doubleironbash", "doublekick", "doubleshock", "doubleslap", "doubleteam", "dracometeor", "dragonascent", "dragonbreath", "dragonclaw", "dragondance", "dragondarts", "dragonenergy", "dragonhammer", "dragonpulse", "dragonrage", "dragonrush", "dragontail", "drainingkiss", "drainpunch", "dreameater", "drillpeck", "drillrun", "drumbeating", "dualchop", "dualwingbeat", "dynamaxcannon", "dynamicpunch", "earthpower", "earthquake", "echoedvoice", "eerieimpulse", "eeriespell", "eggbomb", "electricterrain", "electrify", "electroball", "electrodrift", "electroweb", "embargo", "ember", "encore", "endeavor", "endure", "energyball", "entrainment", "eruption", "esperwing", "eternabeam", "expandingforce", "explosion", "extrasensory", "extremeevoboost", "extremespeed", "facade", "fairylock", "fairywind", "fakeout", "faketears", "falsesurrender", "falseswipe", "featherdance", "feint", "feintattack", "fellstinger", "fierydance", "fierywrath", "filletaway", "finalgambit", "fireblast", "firefang", "firelash", "firepledge", "firepunch", "firespin", "firstimpression", "fishiousrend", "fissure", "flail", "flameburst", "flamecharge", "flamewheel", "flamethrower", "flareblitz", "flash", "flashcannon", "flatter", "fleurcannon", "fling", "flipturn", "floatyfall", "floralhealing", "flowershield", "flowertrick", "fly", "flyingpress", "focusblast", "focusenergy", "focuspunch", "followme", "forcepalm", "foresight", "forestscurse", "foulplay", "freezedry", "freezeshock", "freezingglare", "freezyfrost", "frenzyplant", "frostbreath", "frustration", "furyattack", "furycutter", "furyswipes", "fusionbolt", "fusionflare", "futuresight", "gastroacid", "geargrind", "gearup", "genesissupernova", "geomancy", "gigadrain", "gigaimpact", "gigatonhammer", "gigavolthavoc", "glaciallance", "glaciate", "glaiverush", "glare", "glitzyglow", "gmaxbefuddle", "gmaxcannonade", "gmaxcentiferno", "gmaxchistrike", "gmaxcuddle", "gmaxdepletion", "gmaxdrumsolo", "gmaxfinale", "gmaxfireball", "gmaxfoamburst", "gmaxgoldrush", "gmaxgravitas", "gmaxhydrosnipe", "gmaxmalodor", "gmaxmeltdown", "gmaxoneblow", "gmaxrapidflow", "gmaxreplenish", "gmaxresonance", "gmaxsandblast", "gmaxsmite", "gmaxsnooze", "gmaxsteelsurge", "gmaxstonesurge", "gmaxstunshock", "gmaxsweetness", "gmaxtartness", "gmaxterror", "gmaxvinelash", "gmaxvolcalith", "gmaxvoltcrash", "gmaxwildfire", "gmaxwindrage", "grassknot", "grasspledge", "grasswhistle", "grassyglide", "grassyterrain", "gravapple", "gravity", "growl", "growth", "grudge", "guardianofalola", "guardsplit", "guardswap", "guillotine", "gunkshot", "gust", "gyroball", "hail", "hammerarm", "happyhour", "harden", "haze", "headbutt", "headcharge", "headlongrush", "headsmash", "healbell", "healblock", "healingwish", "healorder", "healpulse", "heartstamp", "heartswap", "heatcrash", "heatwave", "heavyslam", "helpinghand", "hex", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "hiddenpower", "highhorsepower", "highjumpkick", "holdback", "holdhands", "honeclaws", "hornattack", "horndrill", "hornleech", "howl", "hurricane", "hydrocannon", "hydropump", "hydrosteam", "hydrovortex", "hyperbeam", "hyperdrill", "hyperfang", "hyperspacefury", "hyperspacehole", "hypervoice", "hypnosis", "iceball", "icebeam", "iceburn", "icefang", "icehammer", "icepunch", "iceshard", "icespinner", "iciclecrash", "iciclespear", "icywind", "imprison", "incinerate", "infernalparade", "inferno", "infernooverdrive", "infestation", "ingrain", "instruct", "iondeluge", "irondefense", "ironhead", "irontail", "jawlock", "jetpunch", "judgment", "jumpkick", "junglehealing", "karatechop", "kinesis", "kingsshield", "knockoff", "kowtowcleave", "landswrath", "laserfocus", "lashout", "lastresort", "lastrespects", "lavaplume", "leafage", "leafblade", "leafstorm", "leaftornado", "leechlife", "leechseed", "leer", "letssnuggleforever", "lick", "lifedew", "lightofruin", "lightscreen", "lightthatburnsthesky", "liquidation", "lockon", "lovelykiss", "lowkick", "lowsweep", "luckychant", "luminacrash", "lunarblessing", "lunardance", "lunge", "lusterpurge", "machpunch", "magicalleaf", "magicaltorque", "magiccoat", "magicpowder", "magicroom", "magmastorm", "magnetbomb", "magneticflux", "magnetrise", "magnitude", "makeitrain", "maliciousmoonsault", "matblock", "maxairstream", "maxdarkness", "maxflare", "maxflutterby", "maxgeyser", "maxguard", "maxhailstorm", "maxknuckle", "maxlightning", "maxmindstorm", "maxooze", "maxovergrowth", "maxphantasm", "maxquake", "maxrockfall", "maxstarfall", "maxsteelspike", "maxstrike", "maxwyrmwind", "meanlook", "meditate", "mefirst", "megadrain", "megahorn", "megakick", "megapunch", "memento", "menacingmoonrazemaelstrom", "metalburst", "metalclaw", "metalsound", "meteorassault", "meteorbeam", "meteormash", "metronome", "milkdrink", "mimic", "mindblown", "mindreader", "minimize", "miracleeye", "mirrorcoat", "mirrormove", "mirrorshot", "mist", "mistball", "mistyexplosion", "mistyterrain", "moonblast", "moongeistbeam", "moonlight", "morningsun", "mortalspin", "mountaingale", "mudbomb", "mudshot", "mudslap", "mudsport", "muddywater", "multiattack", "mysticalfire", "mysticalpower", "nastyplot", "naturalgift", "naturepower", "naturesmadness", "needlearm", "neverendingnightmare", "nightdaze", "nightmare", "nightshade", "nightslash", "nobleroar", "noretreat", "noxioustorque", "nuzzle", "oblivionwing", "obstruct", "oceanicoperetta", "octazooka", "octolock", "odorsleuth", "ominouswind", "orderup", "originpulse", "outrage", "overdrive", "overheat", "painsplit", "paraboliccharge", "partingshot", "payback", "payday", "peck", "perishsong", "petalblizzard", "petaldance", "phantomforce", "photongeyser", "pikapapow", "pinmissile", "plasmafists", "playnice", "playrough", "pluck", "poisonfang", "poisongas", "poisonjab", "poisonpowder", "poisonsting", "poisontail", "pollenpuff", "poltergeist", "populationbomb", "pounce", "pound", "powder", "powdersnow", "powergem", "powershift", "powersplit", "powerswap", "powertrick", "powertrip", "poweruppunch", "powerwhip", "precipiceblades", "present", "prismaticlaser", "protect", "psybeam", "psyblade", "psychup", "psychic", "psychicfangs", "psychicterrain", "psychoboost", "psychocut", "psychoshift", "psyshieldbash", "psyshock", "psystrike", "psywave", "pulverizingpancake", "punishment", "purify", "pursuit", "pyroball", "quash", "quickattack", "quickguard", "quiverdance", "rage", "ragefist", "ragepowder", "ragingbull", "ragingfury", "raindance", "rapidspin", "razorleaf", "razorshell", "razorwind", "recover", "recycle", "reflect", "reflecttype", "refresh", "relicsong", "rest", "retaliate", "return", "revelationdance", "revenge", "reversal", "revivalblessing", "risingvoltage", "roar", "roaroftime", "rockblast", "rockclimb", "rockpolish", "rockslide", "rocksmash", "rockthrow", "rocktomb", "rockwrecker", "roleplay", "rollingkick", "rollout", "roost", "rototiller", "round", "ruination", "sacredfire", "sacredsword", "safeguard", "saltcure", "sandattack", "sandsearstorm", "sandstorm", "sandtomb", "sappyseed", "savagespinout", "scald", "scaleshot", "scaryface", "scorchingsands", "scratch", "screech", "searingshot", "searingsunrazesmash", "secretpower", "secretsword", "seedbomb", "seedflare", "seismictoss", "selfdestruct", "shadowball", "shadowbone", "shadowclaw", "shadowforce", "shadowpunch", "shadowsneak", "sharpen", "shatteredpsyche", "shedtail", "sheercold", "shellsidearm", "shellsmash", "shelltrap", "shelter", "shiftgear", "shockwave", "shoreup", "signalbeam", "silktrap", "silverwind", "simplebeam", "sing", "sinisterarrowraid", "sizzlyslide", "sketch", "skillswap", "skittersmack", "skullbash", "skyattack", "skydrop", "skyuppercut", "slackoff", "slam", "slash", "sleeppowder", "sleeptalk", "sludge", "sludgebomb", "sludgewave", "smackdown", "smartstrike", "smellingsalts", "smog", "smokescreen", "snaptrap", "snarl", "snatch", "snipeshot", "snore", "snowscape", "soak", "softboiled", "solarbeam", "solarblade", "sonicboom", "soulstealing7starstrike", "spacialrend", "spark", "sparklingaria", "sparklyswirl", "spectralthief", "speedswap", "spicyextract", "spiderweb", "spikecannon", "spikes", "spikyshield", "spinout", "spiritbreak", "spiritshackle", "spitup", "spite", "splash", "splinteredstormshards", "splishysplash", "spore", "spotlight", "springtidestorm", "stealthrock", "steameruption", "steamroller", "steelbeam", "steelroller", "steelwing", "stickyweb", "stockpile", "stokedsparksurfer", "stomp", "stompingtantrum", "stoneaxe", "stoneedge", "storedpower", "stormthrow", "strangesteam", "strength", "strengthsap", "stringshot", "struggle", "strugglebug", "stuffcheeks", "stunspore", "submission", "substitute", "subzeroslammer", "suckerpunch", "sunnyday", "sunsteelstrike", "superfang", "superpower", "supersonic", "supersonicskystrike", "surf", "surgingstrikes", "swagger", "swallow", "sweetkiss", "sweetscent", "swift", "switcheroo", "swordsdance", "synchronoise", "synthesis", "tackle", "tailglow", "tailslap", "tailwhip", "tailwind", "takedown", "takeheart", "tarshot", "taunt", "tearfullook", "teatime", "technoblast", "tectonicrage", "teeterdance", "telekinesis", "teleport", "terablast", "terrainpulse", "thief", "thousandarrows", "thousandwaves", "thrash", "throatchop", "thunder", "thunderbolt", "thundercage", "thunderfang", "thunderouskick", "thunderpunch", "thundershock", "thunderwave", "tickle", "tidyup", "topsyturvy", "torchsong", "torment", "toxic", "toxicspikes", "toxicthread", "trailblaze", "transform", "triattack", "trick", "trickortreat", "trickroom", "triplearrows", "tripleaxel", "tripledive", "triplekick", "tropkick", "trumpcard", "twinbeam", "twineedle", "twinkletackle", "twister", "uturn", "uproar", "vacuumwave", "vcreate", "veeveevolley", "venomdrench", "venoshock", "victorydance", "vinewhip", "visegrip", "vitalthrow", "voltswitch", "volttackle", "wakeupslap", "waterfall", "watergun", "waterpledge", "waterpulse", "watershuriken", "watersport", "waterspout", "wavecrash", "weatherball", "whirlpool", "whirlwind", "wickedblow", "wickedtorque", "wideguard", "wildboltstorm", "wildcharge", "willowisp", "wingattack", "wish", "withdraw", "wonderroom", "woodhammer", "workup", "worryseed", "wrap", "wringout", "xscissor", "yawn", "zapcannon", "zenheadbutt", "zingzap", "zippyzap"};

/** @brief List where `AbilitySmogonIDs[ABILITY]` contains the Smogon-style id of the ability as a string */
static inline const std::array<std::string, ABILITY_TOTAL> AbilitySmogonIDs = {"", "adaptability", "aerilate", "aftermath", "airlock", "analytic", "angerpoint", "angershell", "anticipation", "arenatrap", "armortail", "aromaveil", "asoneglastrier", "aurabreak", "baddreams", "ballfetch", "battery", "battlearmor", "battlebond", "beadsofruin", "beastboost", "berserk", "bigpecks", "blaze", "bulletproof", "cheekpouch", "chillingneigh", "chlorophyll", "clearbody", "cloudnine", "colorchange", "comatose", "commander", "competitive", "compoundeyes", "contrary", "corrosion", "costar", "cottondown", "cudchew", "curiousmedicine", "cursedbody", "cutecharm", "damp", "dancer", "darkaura", "dauntlessshield", "dazzling", "defeatist", "defiant", "deltastream", "desolateland", "disguise", "download", "dragonsmaw", "drizzle", "drought", "dryskin", "eartheater", "earlybird", "effectspore", "electricsurge", "electromorphosis", "emergencyexit", "fairyaura", "filter", "flamebody", "flareboost", "flashfire", "flowergift", "flowerveil", "fluffy", "forecast", "forewarn", "friendguard", "frisk", "fullmetalbody", "furcoat", "galewings", "galvanize", "gluttony", "goodasgold", "gooey", "gorillatactics", "grasspelt", "grassysurge", "grimneigh", "guarddog", "gulpmissile", "guts", "hadronengine", "harvest", "healer", "heatproof", "heavymetal", "honeygather", "hugepower", "hungerswitch", "hustle", "hydration", "hypercutter", "icebody", "iceface", "icescales", "illuminate", "illusion", "immunity", "imposter", "infiltrator", "innardsout", "innerfocus", "insomnia", "intimidate", "intrepidsword", "ironbarbs", "ironfist", "justified", "keeneye", "klutz", "leafguard", "levitate", "libero", "lightmetal", "lightningrod", "lingeringaroma", "limber", "liquidooze", "liquidvoice", "longreach", "magicbounce", "magicguard", "magician", "magmaarmor", "magnetpull", "marvelscale", "megalauncher", "merciless", "mimicry", "minus", "mirrorarmor", "mistysurge", "moldbreaker", "moody", "motordrive", "moxie", "multiscale", "multitype", "mummy", "myceliummight", "naturalcure", "neuroforce", "neutralizinggas", "noguard", "normalize", "oblivious", "opportunist", "orichalcumpulse", "overcoat", "overgrow", "owntempo", "parentalbond", "pastelveil", "perishbody", "pickpocket", "pickup", "pixilate", "plus", "poisonheal", "poisonpoint", "poisontouch", "powerconstruct", "powerofalchemy", "powerspot", "prankster", "pressure", "primordialsea", "prismarmor", "propellertail", "protean", "protosynthesis", "psychicsurge", "punkrock", "purepower", "purifyingsalt", "quarkdrive", "queenlymajesty", "quickdraw", "quickfeet", "raindish", "rattled", "receiver", "reckless", "refrigerate", "regenerator", "ripen", "rivalry", "rkssystem", "rockhead", "rockypayload", "roughskin", "runaway", "sandforce", "sandrush", "sandspit", "sandstream", "sandveil", "sapsipper", "schooling", "scrappy", "screencleaner", "seedsower", "serenegrace", "shadowshield", "shadowtag", "sharpness", "shedskin", "sheerforce", "shellarmor", "shielddust", "shieldsdown", "simple", "skilllink", "slowstart", "slushrush", "sniper", "snowcloak", "snowwarning", "solarpower", "solidrock", "soulheart", "soundproof", "speedboost", "stakeout", "stall", "stalwart", "stamina", "stancechange", "static", "steadfast", "steamengine", "steelworker", "steelyspirit", "stench", "stickyhold", "stormdrain", "strongjaw", "sturdy", "suctioncups", "superluck", "supremeoverlord", "surgesurfer", "swarm", "sweetveil", "swiftswim", "symbiosis", "synchronize", "swordofruin", "tabletsofruin", "tangledfeet", "tanglinghair", "technician", "telepathy", "teravolt", "thermalexchange", "thickfat", "tintedlens", "torrent", "toughclaws", "toxicboost", "toxicdebris", "trace", "transistor", "triage", "truant", "turboblaze", "unaware", "unburden", "unnerve", "unseenfist", "vesselofruin", "victorystar", "vitalspirit", "voltabsorb", "wanderingspirit", "waterabsorb", "waterbubble", "watercompaction", "waterveil", "weakarmor", "wellbakedbody", "whitesmoke", "wimpout", "windpower", "windrider", "wonderguard", "wonderskin", "zenmode", "zerotohero"};

/**
 * @brief List where `ItemSmogonIDs[ITEM]` contains the Smogon-style id of the item as a string
 *
 * @details Many items are not listed in Smogon as they have no effect in battle as held items.
 * A blank string is listed for that item's name in those cases.
 */
static inline const std::array<std::string, ITEM_TOTAL> ItemSmogonIDs = {"", "", "", "abilityshield", "abomasite", "", "absolite", "absorbbulb", "", "", "", "adamantorb", "adrenalineorb", "", "aerodactylite", "", "aggronite", "aguavberry", "airballoon", "mail", "alakazite", "aloraichiumz", "altarianite", "", "ampharosite", "", "", "apicotberry", "", "", "", "armorfossil", "", "", "", "aspearberry", "assaultvest", "audinite", "", "auspiciousarmor", "", "", "", "", "", "", "", "", "babiriberry", "", "", "", "banettite", "", "", "", "", "", "beastball", "beedrillite", "", "belueberry", "berry", "berryjuice", "", "", "berrysweet", "berserkgene", "", "", "", "", "", "", "bigroot", "", "bindingband", "bitterberry", "", "", "blackbelt", "", "blackglasses", "", "blacksludge", "", "", "blastoisinite", "blazikenite", "", "", "", "", "blueorb", "", "", "", "", "", "blukberry", "blunderpolicy", "", "", "", "", "boosterenergy", "bottlecap", "", "", "", "", "", "", "", "", "brightpowder", "", "buggem", "bugmemory", "", "buginiumz", "", "", "burndrive", "", "burntberry", "", "", "", "cameruptite", "", "", "", "", "", "", "", "", "", "", "", "cellbattery", "", "", "charcoal", "charizarditex", "charizarditey", "", "chartiberry", "cheriberry", "cherishball", "chestoberry", "chilanberry", "chilldrive", "chippedpot", "choiceband", "", "choicescarf", "choicespecs", "chopleberry", "clawfossil", "", "clearamulet", "", "", "", "cloversweet", "cobaberry", "", "colburberry", "", "", "", "", "", "", "cornnberry", "", "", "", "", "", "", "coverfossil", "covertcloak", "crackedpot", "", "", "", "", "", "custapberry", "", "damprock", "darkgem", "darkmemory", "", "", "darkiniumz", "", "", "dawnstone", "", "", "decidiumz", "deepseascale", "deepseatooth", "destinyknot", "", "", "", "diancite", "", "", "", "", "", "", "", "", "", "diveball", "", "", "", "domefossil", "", "dousedrive", "", "", "dracoplate", "dragonfang", "dragongem", "dragonmemory", "dragonscale", "", "", "dragoniumz", "", "", "dreadplate", "dreamball", "", "", "", "", "dubiousdisc", "durinberry", "duskball", "duskstone", "", "", "", "", "earthplate", "", "eeviumz", "", "", "", "", "", "", "", "ejectbutton", "ejectpack", "", "", "electirizer", "electricgem", "electricmemory", "electricseed", "", "electriumz", "", "", "", "energypowder", "", "enigmaberry", "", "", "", "", "", "", "", "", "", "eviolite", "", "", "", "", "", "", "", "", "", "expertbelt", "", "", "fairiumz", "fairygem", "fairymemory", "", "", "", "", "fastball", "", "", "", "", "fightinggem", "fightingmemory", "", "fightiniumz", "figyberry", "", "firegem", "firememory", "firestone", "", "firiumz", "", "fistplate", "", "flameorb", "flameplate", "floatstone", "", "flowersweet", "", "flyinggem", "flyingmemory", "", "flyiniumz", "focusband", "focussash", "", "", "fossilizedbird", "fossilizeddino", "fossilizeddrake", "fossilizedfish", "", "friendball", "", "fullincense", "", "", "galaricacuff", "", "galaricawreath", "galladite", "ganlonberry", "garchompite", "gardevoirite", "", "", "", "gengarite", "", "", "", "", "ghostgem", "ghostmemory", "", "ghostiumz", "", "", "glalitite", "", "", "", "goldberry", "goldbottlecap", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "grassgem", "grassmemory", "", "grassiumz", "", "grassyseed", "greatball", "", "", "", "", "", "grepaberry", "", "gripclaw", "", "", "", "", "", "griseousorb", "", "groundgem", "groundmemory", "", "groundiumz", "", "", "", "", "", "", "gyaradosite", "habanberry", "", "hardstone", "", "healball", "", "", "", "", "", "", "", "", "heatrock", "heavyball", "heavydutyboots", "helixfossil", "heracronite", "", "", "", "", "", "", "", "", "", "", "", "", "", "hondewberry", "", "", "", "", "", "houndoominite", "", "", "iapapaberry", "iceberry", "icegem", "", "icememory", "icestone", "", "", "icicleplate", "iciumz", "icyrock", "", "", "", "inciniumz", "", "insectplate", "", "", "ironball", "", "", "ironplate", "jabocaberry", "", "", "jawfossil", "", "", "", "", "", "", "", "", "", "kangaskhanite", "", "kasibberry", "kebiaberry", "keeberry", "kelpsyberry", "", "", "", "", "", "", "kingsrock", "", "", "kommoniumz", "", "", "", "", "", "", "", "", "laggingtail", "lansatberry", "", "latiasite", "latiosite", "", "laxincense", "", "", "", "", "", "leafstone", "leek", "", "leftovers", "", "", "", "", "", "", "", "leppaberry", "", "levelball", "", "", "liechiberry", "lifeorb", "", "lightball", "lightclay", "", "", "", "", "loadeddice", "", "", "", "", "", "lopunnite", "", "", "loveball", "lovesweet", "", "lucarionite", "", "", "luckypunch", "lumberry", "luminousmoss", "", "lunaliumz", "", "", "lureball", "", "lustrousorb", "luxuryball", "lycaniumz", "", "", "machobrace", "", "", "", "", "", "", "magmarizer", "", "magnet", "magoberry", "magostberry", "", "", "maliciousarmor", "manectite", "", "marangaberry", "", "", "marshadiumz", "", "masterball", "mawilite", "", "", "", "", "", "", "", "", "", "meadowplate", "", "", "", "", "", "", "", "medichamite", "", "", "", "", "", "mentalherb", "", "", "metagrossite", "metalcoat", "metalpowder", "", "", "metronome", "", "mewniumz", "", "mewtwonitex", "mewtwonitey", "micleberry", "", "", "", "", "mimikiumz", "mindplate", "mintberry", "miracleberry", "miracleseed", "", "", "", "mirrorherb", "mistyseed", "", "", "", "moonball", "", "", "moonstone", "", "", "", "", "muscleband", "", "", "", "", "", "", "", "mysteryberry", "", "mysticwater", "", "", "nanabberry", "", "nestball", "netball", "nevermeltice", "", "", "", "", "nomelberry", "", "normalgem", "", "normaliumz", "", "", "", "", "", "occaberry", "", "oddincense", "", "", "oldamber", "", "", "", "", "", "", "", "", "", "oranberry", "", "", "", "", "", "ovalstone", "", "", "pamtreberry", "", "", "", "parkball", "", "", "passhoberry", "payapaberry", "", "", "", "pechaberry", "", "", "persimberry", "petayaberry", "", "", "pidgeotite", "", "", "pikaniumz", "pikashuniumz", "pinapberry", "", "pinkbow", "", "", "", "", "pinsirite", "pixieplate", "", "plumefossil", "", "", "", "poisonbarb", "poisongem", "poisonmemory", "", "poisoniumz", "pokeball", "", "", "", "", "", "", "", "", "", "", "polkadotbow", "", "", "pomegberry", "", "", "", "", "", "poweranklet", "powerband", "powerbelt", "powerbracer", "powerherb", "powerlens", "", "powerweight", "", "", "", "", "premierball", "", "primariumz", "prismscale", "", "", "", "", "protectivepads", "protector", "", "przcureberry", "psncureberry", "psychicgem", "psychicmemory", "psychicseed", "", "psychiumz", "", "", "punchingglove", "", "", "", "qualotberry", "quickball", "", "", "", "quickclaw", "quickpowder", "", "rabutaberry", "", "", "", "", "", "", "rarebone", "", "", "", "rawstberry", "razorclaw", "razorfang", "razzberry", "", "reapercloth", "", "", "redcard", "", "", "", "", "redorb", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "repeatball", "", "", "", "", "", "", "", "", "ribbonsweet", "", "", "rindoberry", "ringtarget", "rockgem", "rockincense", "rockmemory", "", "rockiumz", "rockyhelmet", "", "roomservice", "rootfossil", "roseincense", "roseliberry", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "rowapberry", "", "", "", "rustedshield", "rustedsword", "", "sablenite", "sachet", "", "safariball", "safetygoggles", "sailfossil", "salacberry", "salamencite", "", "", "", "", "", "", "", "", "", "sceptilite", "scizorite", "scopelens", "", "", "", "seaincense", "", "", "", "", "", "", "", "", "", "sharpbeak", "sharpedonite", "shedshell", "shellbell", "", "", "", "shinystone", "", "", "shockdrive", "shucaberry", "silkscarf", "", "", "", "", "silverpowder", "", "", "sitrusberry", "skullfossil", "skyplate", "", "slowbronite", "", "", "", "", "", "", "", "", "", "smoothrock", "", "snorliumz", "snowball", "", "", "", "softsand", "solganiumz", "", "", "", "", "souldew", "", "", "", "", "", "spelltag", "spelonberry", "splashplate", "", "spookyplate", "sportball", "", "", "", "", "", "", "", "", "starsweet", "", "starfberry", "", "", "steelgem", "", "steelmemory", "", "", "steeliumz", "steelixite", "stickybarb", "", "stoneplate", "", "", "", "", "strawberrysweet", "", "", "", "", "", "", "", "sunstone", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "swampertite", "", "sweetapple", "", "", "", "", "tamatoberry", "tangaberry", "", "tapuniumz", "tartapple", "", "", "", "", "", "", "", "", "", "", "", "terrainextender", "", "", "thickclub", "throatspray", "thunderstone", "", "", "", "timerball", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "tr01", "tr02", "tr03", "tr04", "tr05", "tr06", "tr07", "tr08", "tr09", "tr10", "tr11", "tr12", "tr13", "tr14", "tr15", "tr16", "tr17", "tr18", "tr19", "tr20", "tr21", "tr22", "tr23", "tr24", "tr25", "tr26", "tr27", "tr28", "tr29", "tr30", "tr31", "tr32", "tr33", "tr34", "tr35", "tr36", "tr37", "tr38", "tr39", "tr40", "tr41", "tr42", "tr43", "tr44", "tr45", "tr46", "tr47", "tr48", "tr49", "tr50", "tr51", "tr52", "tr53", "tr54", "tr55", "tr56", "tr57", "tr58", "tr59", "tr60", "tr61", "tr62", "tr63", "tr64", "tr65", "tr66", "tr67", "tr68", "tr69", "tr70", "tr71", "tr72", "tr73", "tr74", "tr75", "tr76", "tr77", "tr78", "tr79", "tr80", "tr81", "tr82", "tr83", "tr84", "tr85", "tr86", "tr87", "tr88", "tr89", "tr90", "tr91", "tr92", "tr93", "tr94", "tr95", "tr96", "tr97", "tr98", "tr99", "", "", "", "", "", "", "", "", "toxicorb", "toxicplate", "", "", "", "", "", "", "", "twistedspoon", "tyranitarite", "", "ultraball", "ultranecroziumz", "", "", "upgrade", "utilityumbrella", "", "", "venusaurite", "", "", "", "", "", "", "", "", "", "", "", "", "", "wacanberry", "", "", "", "", "", "", "", "watergem", "watermemory", "waterstone", "", "wateriumz", "watmelberry", "waveincense", "", "weaknesspolicy", "", "wepearberry", "whippeddream", "", "", "whiteherb", "", "widelens", "wikiberry", "", "wiseglasses", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "yacheberry", "", "", "", "", "", "", "", "", "zapplate", "", "", "zoomlens", "", "", "", ""};
// clang-format on
}  // namespace internal

std::string toString(Species speciesEnum) {
  return internal::SpeciesNames[speciesEnum];
}

std::string toString(Move moveEnum) {
  return internal::MoveNames[moveEnum];
}

std::string toString(Ability abilityEnum) {
  return internal::AbilityNames[abilityEnum];
}

std::string toString(Item itemEnum) {
  return internal::ItemNames[itemEnum];
}

std::string toString(Nature natureEnum) {
  return internal::NatureNames[natureEnum];
}

std::string toString(Status statusEnum) {
  return internal::StatusNames[statusEnum];
}

std::string toString(Gender genderEnum) {
  return internal::GenderNames[genderEnum];
}

std::string toString(Type typeEnum) {
  return internal::TypeNames[typeEnum];
}

std::string toString(Stat statEnum) {
  return internal::StatNames[statEnum];
}

std::string toSmogonString(Species speciesEnum) {
  return internal::SpeciesSmogonNames[speciesEnum];
}

std::string toSmogonString(Move moveEnum) {
  return internal::MoveSmogonNames[moveEnum];
}

std::string toSmogonString(Ability abilityEnum) {
  return internal::AbilitySmogonNames[abilityEnum];
}

std::string toSmogonString(Item itemEnum) {
  return internal::ItemSmogonNames[itemEnum];
}

std::string toSmogonIdString(Species speciesEnum) {
  return internal::SpeciesSmogonIDs[speciesEnum];
}

std::string toSmogonIdString(Move moveEnum) {
  return internal::MoveSmogonIDs[moveEnum];
}

std::string toSmogonIdString(Ability abilityEnum) {
  return internal::AbilitySmogonIDs[abilityEnum];
}

std::string toSmogonIdString(Item itemEnum) {
  return internal::ItemSmogonIDs[itemEnum];
}

Species fromNameToSpeciesEnum(const std::string& name) {
  static const entt::dense_map<std::string, Species> map =
    internal::createNameToEnumMap<Species>(internal::SpeciesNames);
  return internal::fromStringToEnum(map, name);
}

Move fromNameToMoveEnum(const std::string& name) {
  static const entt::dense_map<std::string, Move> map = internal::createNameToEnumMap<Move>(internal::MoveNames);
  return internal::fromStringToEnum(map, name);
}

Ability fromNameToAbilityEnum(const std::string& name) {
  static const entt::dense_map<std::string, Ability> map =
    internal::createNameToEnumMap<Ability>(internal::AbilityNames);
  return internal::fromStringToEnum(map, name);
}

Item fromNameToItemEnum(const std::string& name) {
  static const entt::dense_map<std::string, Item> map = internal::createNameToEnumMap<Item>(internal::ItemNames);
  return internal::fromStringToEnum(map, name);
}

Nature fromNameToNatureEnum(const std::string& name) {
  static const entt::dense_map<std::string, Nature> map = internal::createNameToEnumMap<Nature>(internal::NatureNames);
  return internal::fromStringToEnum(map, name);
}

Status fromNameToStatusEnum(const std::string& name) {
  static const entt::dense_map<std::string, Status> map = internal::createNameToEnumMap<Status>(internal::StatusNames);
  return internal::fromStringToEnum(map, name);
}

Gender fromNameToGenderEnum(const std::string& name) {
  static const entt::dense_map<std::string, Gender> map = internal::createNameToEnumMap<Gender>(internal::GenderNames);
  return internal::fromStringToEnum(map, name);
}

Type fromNameToTypeEnum(const std::string& name) {
  static const entt::dense_map<std::string, Type> map = internal::createNameToEnumMap<Type>(internal::TypeNames);
  return internal::fromStringToEnum(map, name);
}

Stat fromNameToStatEnum(const std::string& name) {
  static const entt::dense_map<std::string, Stat> map = internal::createNameToEnumMap<Stat>(internal::StatNames);
  return internal::fromStringToEnum(map, name);
}

Species fromSmogonNameToSpeciesEnum(const std::string& name) {
  static const entt::dense_map<std::string, Species> map =
    internal::createNameToEnumMap<Species>(internal::SpeciesSmogonNames);
  return internal::fromStringToEnum(map, name);
}

Move fromSmogonNameToMoveEnum(const std::string& name) {
  static const entt::dense_map<std::string, Move> map = internal::createNameToEnumMap<Move>(internal::MoveSmogonNames);
  return internal::fromStringToEnum(map, name);
}

Ability fromSmogonNameToAbilityEnum(const std::string& name) {
  static const entt::dense_map<std::string, Ability> map =
    internal::createNameToEnumMap<Ability>(internal::AbilitySmogonNames);
  return internal::fromStringToEnum(map, name);
}

Item fromSmogonNameToItemEnum(const std::string& name) {
  static const entt::dense_map<std::string, Item> map = internal::createNameToEnumMap<Item>(internal::ItemSmogonNames);
  return internal::fromStringToEnum(map, name);
}

Species fromSmogonIDToSpeciesEnum(const std::string& name) {
  static const entt::dense_map<std::string, Species> map =
    internal::createNameToEnumMap<Species>(internal::SpeciesSmogonIDs);
  return internal::fromStringToEnum(map, name);
}

Move fromSmogonIDToMoveEnum(const std::string& name) {
  static const entt::dense_map<std::string, Move> map = internal::createNameToEnumMap<Move>(internal::MoveSmogonIDs);
  return internal::fromStringToEnum(map, name);
}

Ability fromSmogonIDToAbilityEnum(const std::string& name) {
  static const entt::dense_map<std::string, Ability> map =
    internal::createNameToEnumMap<Ability>(internal::AbilitySmogonIDs);
  return internal::fromStringToEnum(map, name);
}

Item fromSmogonIDToItemEnum(const std::string& name) {
  static const entt::dense_map<std::string, Item> map = internal::createNameToEnumMap<Item>(internal::ItemSmogonIDs);
  return internal::fromStringToEnum(map, name);
}

std::string toID(const std::string& name) {
  std::string idName;
  for (auto character : name) {
    if (character <= 'Z' && character >= 'A') {
      idName += (char)(character + ' ');
    }
    else if ((character <= '9' && character >= '0') || (character <= 'z' && character >= 'a')) {
      idName += character;
    }
  }

  return idName;
}
}  // namespace pokesim::dex

/////////////////////////// END OF src/Dex/Names.cpp ///////////////////////////

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

    template<typename ILhs, typename IRhs>
    friend constexpr std::ptrdiff_t operator-(const dense_set_iterator<ILhs> &, const dense_set_iterator<IRhs> &) noexcept;

    template<typename ILhs, typename IRhs>
    friend constexpr bool operator==(const dense_set_iterator<ILhs> &, const dense_set_iterator<IRhs> &) noexcept;

    template<typename ILhs, typename IRhs>
    friend constexpr bool operator<(const dense_set_iterator<ILhs> &, const dense_set_iterator<IRhs> &) noexcept;

private:
    It it;
};

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr std::ptrdiff_t operator-(const dense_set_iterator<ILhs> &lhs, const dense_set_iterator<IRhs> &rhs) noexcept {
    return lhs.it - rhs.it;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator==(const dense_set_iterator<ILhs> &lhs, const dense_set_iterator<IRhs> &rhs) noexcept {
    return lhs.it == rhs.it;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator!=(const dense_set_iterator<ILhs> &lhs, const dense_set_iterator<IRhs> &rhs) noexcept {
    return !(lhs == rhs);
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator<(const dense_set_iterator<ILhs> &lhs, const dense_set_iterator<IRhs> &rhs) noexcept {
    return lhs.it < rhs.it;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator>(const dense_set_iterator<ILhs> &lhs, const dense_set_iterator<IRhs> &rhs) noexcept {
    return rhs < lhs;
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator<=(const dense_set_iterator<ILhs> &lhs, const dense_set_iterator<IRhs> &rhs) noexcept {
    return !(lhs > rhs);
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator>=(const dense_set_iterator<ILhs> &lhs, const dense_set_iterator<IRhs> &rhs) noexcept {
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

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator==(const dense_set_local_iterator<ILhs> &lhs, const dense_set_local_iterator<IRhs> &rhs) noexcept {
    return lhs.index() == rhs.index();
}

template<typename ILhs, typename IRhs>
[[nodiscard]] constexpr bool operator!=(const dense_set_local_iterator<ILhs> &lhs, const dense_set_local_iterator<IRhs> &rhs) noexcept {
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
     * The returned iterator points to the first instance of the internal array.
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
     *
     * The returned iterator points to the element following the last instance
     * of the internal array. Attempting to dereference the returned iterator
     * results in undefined behavior.
     *
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
    template<class Other>
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

/////////////////////////// START OF src/Dex/Dex.cpp ///////////////////////////

namespace pokesim {
entt::handle Dex::createEntry() {
  return {registry, registry.create()};
}

void Dex::loadSpecies(const entt::dense_set<dex::Species>& speciesList) {
  speciesMap.reserve(speciesList.size());
  for (dex::Species species : speciesList) {
    ENTT_ASSERT(!speciesMap.contains(species), "Shouldn't build data entries twice");
    speciesMap[species] = getSpeciesBuild(species)(*this);
  }
}
}  // namespace pokesim

//////////////////////////// END OF src/Dex/Dex.cpp ////////////////////////////

////////////////////// START OF src/Components/EVsIVs.hpp //////////////////////

#include <cstdint>

namespace pokesim {
struct EVs {
  std::uint8_t hp;
  std::uint8_t atk;
  std::uint8_t def;
  std::uint8_t spa;
  std::uint8_t spd;
  std::uint8_t spe;
};

struct IVs {
  std::uint8_t hp;
  std::uint8_t atk;
  std::uint8_t def;
  std::uint8_t spa;
  std::uint8_t spd;
  std::uint8_t spe;
};
}  // namespace pokesim

/////////////////////// END OF src/Components/EVsIVs.hpp ///////////////////////

//////////// START OF src/Components/EntityHolders/ActionQueue.hpp /////////////

#include <vector>

namespace pokesim {
struct ActionQueue {
  std::vector<entt::entity> actionQueue{};
};
}  // namespace pokesim

///////////// END OF src/Components/EntityHolders/ActionQueue.hpp //////////////

/////////////// START OF src/Components/EntityHolders/Battle.hpp ///////////////

namespace pokesim {
struct Battle {
  entt::entity battle;
};
}  // namespace pokesim

//////////////// END OF src/Components/EntityHolders/Battle.hpp ////////////////

////////////// START OF src/Components/EntityHolders/FoeSide.hpp ///////////////

namespace pokesim {
struct FoeSide {
  entt::entity foeSide;
};
}  // namespace pokesim

/////////////// END OF src/Components/EntityHolders/FoeSide.hpp ////////////////

//////////////// START OF src/Components/EntityHolders/Move.hpp ////////////////

namespace pokesim {
struct Move {
  entt::entity move;
};
}  // namespace pokesim

///////////////// END OF src/Components/EntityHolders/Move.hpp /////////////////

///////////// START OF src/Components/EntityHolders/MoveSlots.hpp //////////////

#include <vector>

namespace pokesim {
struct MoveSlots {
  std::vector<entt::entity> moveSlots{};
};
}  // namespace pokesim

////////////// END OF src/Components/EntityHolders/MoveSlots.hpp ///////////////

////////////// START OF src/Components/EntityHolders/Pokemon.hpp ///////////////

namespace pokesim {
struct Pokemon {
  entt::entity pokemon;
};
}  // namespace pokesim

/////////////// END OF src/Components/EntityHolders/Pokemon.hpp ////////////////

//////////////// START OF src/Components/EntityHolders/Side.hpp ////////////////

namespace pokesim {
struct Side {
  entt::entity side;
};
}  // namespace pokesim

///////////////// END OF src/Components/EntityHolders/Side.hpp /////////////////

/////////////// START OF src/Components/EntityHolders/Sides.hpp ////////////////

namespace pokesim {
struct Sides {
  entt::entity p1;
  entt::entity p2;
};
}  // namespace pokesim

//////////////// END OF src/Components/EntityHolders/Sides.hpp /////////////////

//////////////// START OF src/Components/EntityHolders/Team.hpp ////////////////

#include <vector>

namespace pokesim {
struct Team {
  std::vector<entt::entity> team{};
};
}  // namespace pokesim

///////////////// END OF src/Components/EntityHolders/Team.hpp /////////////////

//////////////////////// START OF src/Components/ID.hpp ////////////////////////

#include <cstdint>

namespace pokesim {
struct ID {
  std::uint16_t id;
};
}  // namespace pokesim

///////////////////////// END OF src/Components/ID.hpp /////////////////////////

////////////////////// START OF src/Components/Level.hpp ///////////////////////

#include <cstdint>

namespace pokesim {
struct Level {
  std::uint8_t level;
};
}  // namespace pokesim

/////////////////////// END OF src/Components/Level.hpp ////////////////////////

/////////////////// START OF src/Components/Names/Gender.hpp ///////////////////

namespace pokesim {
struct GenderName {
  dex::Gender name = dex::NO_GENDER;
};
}  // namespace pokesim

//////////////////// END OF src/Components/Names/Gender.hpp ////////////////////

/////////////////// START OF src/Components/Names/Nature.hpp ///////////////////

namespace pokesim {
struct NatureName {
  dex::Nature name = dex::NO_NATURE;
};
}  // namespace pokesim

//////////////////// END OF src/Components/Names/Nature.hpp ////////////////////

//////////////////// START OF src/Components/Names/Stat.hpp ////////////////////

namespace pokesim {
struct StatName {
  dex::Stat name;
};
}  // namespace pokesim

///////////////////// END OF src/Components/Names/Stat.hpp /////////////////////

/////////////////// START OF src/Components/Names/Status.hpp ///////////////////

namespace pokesim {
struct StatusName {
  dex::Status name = dex::NO_STATUS;
};
}  // namespace pokesim

//////////////////// END OF src/Components/Names/Status.hpp ////////////////////

///////////////////// START OF src/Components/Position.hpp /////////////////////

#include <cstdint>

namespace pokesim {
struct Position {
  std::uint8_t position;
};
}  // namespace pokesim

////////////////////// END OF src/Components/Position.hpp //////////////////////

/////////////////// START OF src/Components/Probability.hpp ////////////////////

namespace pokesim {
struct Probability {
  float probability;
};
}  // namespace pokesim

//////////////////// END OF src/Components/Probability.hpp /////////////////////

///////////////////// START OF src/Components/RNGSeed.hpp //////////////////////

#include <cstdint>

namespace pokesim {
struct RNGSeed {
  std::uint32_t seed;
};
}  // namespace pokesim

////////////////////// END OF src/Components/RNGSeed.hpp ///////////////////////

//////////////////// START OF src/Components/SpeedSort.hpp /////////////////////

#include <cstdint>

namespace pokesim {
struct SpeedSort {
  std::uint8_t order;
  std::int8_t priority;
  std::int8_t fractionalPriority;
  std::uint16_t speed;
};
}  // namespace pokesim

///////////////////// END OF src/Components/SpeedSort.hpp //////////////////////

////////////////////// START OF src/Components/Stats.hpp ///////////////////////

#include <cstdint>

namespace pokesim::stat {
struct HP {
  std::uint16_t hp;
};

struct Atk {
  std::uint16_t atk;
};

struct Def {
  std::uint16_t def;
};

struct Spa {
  std::uint16_t spa;
};

struct Spd {
  std::uint16_t spd;
};

struct Spe {
  std::uint16_t spe;
};
}  // namespace pokesim::stat

/////////////////////// END OF src/Components/Stats.hpp ////////////////////////

/////////////////// START OF src/Components/Tags/Ability.hpp ///////////////////

namespace pokesim::ability {
struct Defiant {};
struct Infiltrator {};
struct IronFist {};
struct Static {};
struct SweetVeil {};
struct Trace {};
}  // namespace pokesim::ability

//////////////////// END OF src/Components/Tags/Ability.hpp ////////////////////

/////////////////// START OF src/Components/Tags/Actions.hpp ///////////////////

namespace pokesim::action {
struct Move {};

struct SwitchOut {};
struct PreTurn {};
struct MidTurn {};
struct PostFoeFaint {};

struct Item {};
struct Team {};
struct RevivalBlessing {};
struct SwitchIn {};
struct PreSwitch {};
struct MegaEvolve {};
struct Primal {};
struct Dynamax {};
struct Terastallize {};
}  // namespace pokesim::action

//////////////////// END OF src/Components/Tags/Actions.hpp ////////////////////

/////////////////// START OF src/Components/Tags/Battle.hpp ////////////////////

namespace pokesim {

// Current Action Tags

struct ActiveMove {};
struct ActiveMoveTarget {};
struct ActiveMoveSource {};

// Turn State

struct MidTurn {};
struct Started {};
struct Ended {};
}  // namespace pokesim

//////////////////// END OF src/Components/Tags/Battle.hpp /////////////////////

//////////////////// START OF src/Components/Tags/Item.hpp /////////////////////

namespace pokesim::item {
struct AssaultVest {};
struct BrightPowder {};
struct ChoiceScarf {};
struct ChoiceSpecs {};
struct FocusSash {};
struct LifeOrb {};
}  // namespace pokesim::item

///////////////////// END OF src/Components/Tags/Item.hpp //////////////////////

/////////////////// START OF src/Components/Tags/Pokemon.hpp ///////////////////

namespace pokesim {
struct Active {};
}  // namespace pokesim

//////////////////// END OF src/Components/Tags/Pokemon.hpp ////////////////////

/////////////////// START OF src/Components/Tags/Status.hpp ////////////////////

namespace pokesim::status {
struct Burn {};
struct Freeze {};
struct Paralysis {};
struct Poison {};
struct Sleep {};
struct Toxic {};
}  // namespace pokesim::status

//////////////////// END OF src/Components/Tags/Status.hpp /////////////////////

//////////////////// START OF src/Components/Tags/Type.hpp /////////////////////

namespace pokesim::type {
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
}  // namespace pokesim::type

///////////////////// END OF src/Components/Tags/Type.hpp //////////////////////

//////////////////// START OF src/Components/TargetSlot.hpp ////////////////////

#include <cstdint>

namespace pokesim {
struct TargetSlot {
  enum : uint8_t {
    P1A,
    P1B,
    P2A,
    P2B,
  } targetSlot;
};
}  // namespace pokesim

///////////////////// END OF src/Components/TargetSlot.hpp /////////////////////

/////////////////////// START OF src/Components/Turn.hpp ///////////////////////

#include <cstdint>

namespace pokesim {
struct Turn {
  std::uint16_t turn;
};
}  // namespace pokesim

//////////////////////// END OF src/Components/Turn.hpp ////////////////////////

/////////////////////////// START OF src/PokeSim.hpp ///////////////////////////


//////////////////////////// END OF src/PokeSim.hpp ////////////////////////////