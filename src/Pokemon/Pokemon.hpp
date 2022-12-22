#pragma once

#include <Dex/Dex.hpp>
#include <Utils/Universal.hpp>

namespace pokesim {
class Pokemon {
  using Names = Dex::Names;

 public:
  Names::Species species;
  Names::Ability ability;
  Names::Item item;

  /**
   * @brief Checks if the Pokemon is one of the species given as a parameter.
   * Takes any number of arguments.
   *
   * @details
   * @code {.cpp}
   * eevee.isSpecies(Dex::Names::EEVEE); // Returns true
   * eevee.isSpecies(Dex::Names::VAPOREON, Dex::Names::JOLTEON, Dex::Names::FLAREON); // Returns false
   * eevee.isSpecies(Dex::Names::ESPEON, Dex::Names::EEVEE, Dex::Names::UMBREON); // Returns true
   * eevee.isSpecies(); // Returns false
   * @endcode
   *
   * @param ...speciesList A list of `Dex::Names::Species` arguments to check
   */
  template <typename... SpeciesList>
  bool isSpecies(SpeciesList... speciesList) const {
    return matchesFirst(species, speciesList...);
  }

  /**
   * @brief Checks if the Pokemon has one of the abilities given as a parameter.
   * Takes any number of arguments.
   *
   * @details
   * @code {.cpp}
   * shedinja.hasAbility(Dex::Names::WONDER_GUARD); // Returns true
   * shedinja.hasAbility(Dex::Names::BATTLE_BOND, Dex::Names::DISGUISE, Dex::ILLUSION); // Returns false
   * shedinja.hasAbility(Dex::Names::COMATOSE, Dex::Names::WONDER_GUARD); // Returns true
   * shedinja.hasAbility(); // Returns false
   * @endcode
   *
   * @param ...abilities A list of `Dex::Names::Ability` arguments to check
   */
  template <typename... AbilityList>
  bool hasAbility(AbilityList... abilities) const {
    return matchesFirst(ability, abilities...);
  }

  /**
   * @brief Checks if the Pokemon has one of the items given as a parameter.
   * Takes any number of arguments.
   *
   * @details
   * @code {.cpp}
   * mega_venusaur.hasItem(Dex::Names::VENUSAURITE); // Returns true
   * mega_venusaur.hasItem(Dex::Names::LIFE_ORB, Dex::Names::CHOICE_SPECS, Dex::LEFTOVERS); // Returns false
   * mega_venusaur.hasItem(Dex::Names::VENUSAURITE, Dex::Names::BLASTOISINITE); // Returns true
   * mega_venusaur.hasItem(); // Returns false
   * @endcode
   *
   * @param ...items A list of `Dex::Names::Item` arguments to check
   */
  template <typename... ItemList>
  bool hasItem(ItemList... items) const {
    return matchesFirst(item, items...);
  }
};
}  // namespace pokesim
