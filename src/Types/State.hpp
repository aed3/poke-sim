#pragma once

#include <Utilities/FixedMemoryVector.hpp>
#include <Utilities/NumberToType.hpp>
#include <array>
#include <optional>
#include <type_traits>

#include "Constants.hpp"
#include "Entity.hpp"

namespace pokesim {
class Simulation;

namespace types {
using stateId = std::underlying_type_t<entity>;

using battleTurn = pokesim::internal::unsignedIntType<Constants::TurnCount::MAX>;

using sideIndex = pokesim::internal::unsignedIntType<Constants::SIDE_COUNT>;

template <typename T>
struct sides : public std::array<T, Constants::SIDE_COUNT> {
  constexpr T& p1() { return this->at(0); };
  constexpr T& p2() { return this->at(1); };
  constexpr const T& p1() const { return this->at(0); };
  constexpr const T& p2() const { return this->at(1); };

  template <std::size_t N>
  decltype(auto) get() const {
    static_assert(N < Constants::SIDE_COUNT);
    return this->at(N);
  }

  sides() : std::array<T, Constants::SIDE_COUNT>() {}
  sides(std::initializer_list<T> list) : sides() {
    sideIndex side = 0U;
    for (const T& value : list) {
      this->at(side) = value;
      side++;
      if (side == size()) {
        break;
      }
    }
  }

  constexpr sideIndex size() const noexcept { return Constants::SIDE_COUNT; }
};

using entityIndex = std::underlying_type_t<entity>;

using teamPositionIndex = pokesim::internal::unsignedIntType<Constants::TeamSize::MAX>;
using moveSlotIndex = pokesim::internal::unsignedIntType<Constants::MoveSlots::MAX>;
using activePokemonIndex = pokesim::internal::unsignedIntType<Constants::ActivePokemon::MAX>;

template <typename T>
using teamPositions = pokesim::internal::fixedMemoryVector<T, Constants::TeamSize::MAX>;
using teamOrder = types::teamPositions<types::teamPositionIndex>;

template <typename T>
using moveSlots = pokesim::internal::fixedMemoryVector<T, Constants::MoveSlots::MAX>;

template <typename T>
using sideSlots = pokesim::internal::fixedMemoryVector<T, Constants::ActivePokemonSlotsPerSide::MAX>;

template <typename T>
using targets = pokesim::internal::fixedMemoryVector<T, Constants::Targets::MAX>;

using actionQueueIndex = pokesim::internal::unsignedIntType<Constants::ActionQueueLength::MAX>;

using callback = void (*)(Simulation&);
using optionalCallback = std::optional<callback>;
}  // namespace types
}  // namespace pokesim

template <typename T>
struct std::tuple_size<pokesim::types::sides<T>> : std::integral_constant<std::size_t, pokesim::Constants::SIDE_COUNT> {
};

template <std::size_t N, typename T>
struct std::tuple_element<N, pokesim::types::sides<T>> {
  using type = decltype(std::declval<pokesim::types::sides<T>>().template get<N>());
};
