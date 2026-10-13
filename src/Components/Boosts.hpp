#pragma once

#include <Types/Constants.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
struct AtkBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
  constexpr bool operator==(AtkBoost other) const { return val == other.val; }
};

struct DefBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
  constexpr bool operator==(DefBoost other) const { return val == other.val; }
};

struct SpaBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
  constexpr bool operator==(SpaBoost other) const { return val == other.val; }
};

struct SpdBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
  constexpr bool operator==(SpdBoost other) const { return val == other.val; }
};

struct SpeBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
  constexpr bool operator==(SpeBoost other) const { return val == other.val; }
};

struct AccBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
  constexpr bool operator==(AccBoost other) const { return val == other.val; }
};

struct EvaBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
  constexpr bool operator==(EvaBoost other) const { return val == other.val; }
};
}  // namespace pokesim
