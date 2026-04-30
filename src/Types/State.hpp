#pragma once

#include <Utilities/FixedMemoryVector.hpp>
#include <array>
#include <optional>
#include <type_traits>

#include "Entity.hpp"
#include "MechanicConstants.hpp"
#include "NumberToType.hpp"

namespace pokesim {
class Simulation;

namespace types {
using stateId = std::underlying_type_t<entity>;

using battleTurn = pokesim::internal::unsignedIntType<MechanicConstants::TurnCount::MAX>;

using sideIndex = pokesim::internal::unsignedIntType<MechanicConstants::SIDE_COUNT>;
template <typename T>
struct sides : public std::array<T, MechanicConstants::SIDE_COUNT> {
  constexpr T& p1() { return this->at(0); };
  constexpr T& p2() { return this->at(1); };
  constexpr const T& p1() const { return this->at(0); };
  constexpr const T& p2() const { return this->at(1); };

  template <std::size_t N>
  decltype(auto) get() const {
    return this->at(N);
  }

  sides() : std::array<T, MechanicConstants::SIDE_COUNT>() {}
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

  constexpr sideIndex size() const noexcept { return MechanicConstants::SIDE_COUNT; }
};

using entityIndex = std::underlying_type_t<entity>;

using teamPositionIndex = pokesim::internal::unsignedIntType<MechanicConstants::TeamSize::MAX>;
using moveSlotIndex = pokesim::internal::unsignedIntType<MechanicConstants::MoveSlots::MAX>;
using activePokemonIndex = pokesim::internal::unsignedIntType<MechanicConstants::ActivePokemon::MAX>;

template <typename T>
using teamPositions = pokesim::internal::fixedMemoryVector<T, MechanicConstants::TeamSize::MAX>;
using teamOrder = types::teamPositions<types::teamPositionIndex>;

template <typename T>
using moveSlots = pokesim::internal::fixedMemoryVector<T, MechanicConstants::MoveSlots::MAX>;

template <typename T>
using sideSlots = pokesim::internal::fixedMemoryVector<T, MechanicConstants::ActivePokemonSlotsPerSide::MAX>;

template <typename T>
using targets = pokesim::internal::fixedMemoryVector<T, MechanicConstants::Targets::MAX>;

using actionQueueIndex = pokesim::internal::unsignedIntType<MechanicConstants::ActionQueueLength::MAX>;

using callback = void (*)(Simulation&);
using optionalCallback = std::optional<callback>;
}  // namespace types
}  // namespace pokesim

template <typename T>
struct std::tuple_size<pokesim::types::sides<T>>
    : std::integral_constant<std::size_t, pokesim::MechanicConstants::SIDE_COUNT> {};

template <std::size_t N, typename T>
struct std::tuple_element<N, pokesim::types::sides<T>> {
  using type = decltype(std::declval<pokesim::types::sides<T>>().template get<N>());
};
