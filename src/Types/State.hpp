#pragma once

#include <Utilities/NumberToType.hpp>
#include <array>
#include <optional>
#include <type_traits>

#include "Constants.hpp"
#include "Entity.hpp"
#include "FixedMemoryVector.hpp"
#include "Indexes.hpp"

namespace pokesim {
class Simulation;

namespace types {
using stateId = std::underlying_type_t<entity>;

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

template <typename T>
using teamPositions = fixedMemoryVector<T, Constants::TeamSize::MAX>;
using teamOrder = teamPositions<teamPositionIndex>;

template <typename T>
using moveSlots = fixedMemoryVector<T, Constants::MoveSlots::MAX>;

template <typename T>
using sideSlots = fixedMemoryVector<T, Constants::ActivePokemonSlotsPerSide::MAX>;

template <typename T>
using targets = fixedMemoryVector<T, Constants::Targets::MAX>;

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
