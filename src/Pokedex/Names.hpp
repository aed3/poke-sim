#pragma once

#include <Types/Enums/headers.hpp>
#include <string>

// TODO(aed3): Change the move names to be part of the build functions so they aren't all loaded all the time

namespace pokesim::dex {
// Returns the name of a species represented by its enum as a string.
std::string toString(Species speciesEnum);

// Returns the name of a move represented by its enum as a string.
std::string toString(Move moveEnum);

// Returns the name of an ability represented by its enum as a string.
std::string toString(Ability abilityEnum);

// Returns the name of an item represented by its enum as a string.
std::string toString(Item itemEnum);

// Returns the name of a nature represented by its enum as a string.
std::string toString(Nature natureEnum);

// Returns the name of a status condition represented by its enum as a string.
std::string toString(Status statusEnum);

// Returns the name of a gender represented by its enum as a string.
std::string toString(Gender genderEnum);

// Returns the name of a type represented by its enum as a string.
std::string toString(Type typeEnum);

// Returns the name of a stat represented by its enum as a string.
std::string toString(Stat statEnum);

// Returns the Smogon name of a species represented by its enum as a string.
std::string toSmogonString(Species speciesEnum);

// Returns the Smogon name of a move represented by its enum as a string.
std::string toSmogonString(Move moveEnum);

// Returns the Smogon name of an ability represented by its enum as a string.
std::string toSmogonString(Ability abilityEnum);

// Returns the Smogon name of an item represented by its enum as a string.
std::string toSmogonString(Item itemEnum);

// Returns the Smogon ID of a species represented by its enum as a string.
std::string toSmogonIdString(Species speciesEnum);

// Returns the Smogon ID of a move represented by its enum as a string.
std::string toSmogonIdString(Move moveEnum);

// Returns the Smogon ID of an ability represented by its enum as a string.
std::string toSmogonIdString(Ability abilityEnum);

// Returns the Smogon ID of an item represented by its enum as a string.
std::string toSmogonIdString(Item itemEnum);

/**
 * @brief Returns the enum of the species name string.
 * Returns `MISSING_NO` if the string does not represent a species.
 */
Species fromNameToSpeciesEnum(const std::string& name);

/**
 * @brief Returns the enum of the move name string.
 * Returns `NO_MOVE` if the string does not represent a move.
 */
Move fromNameToMoveEnum(const std::string& name);

/**
 * @brief Returns the enum of the ability name string.
 * Returns `NO_ABILITY` if the string does not represent an ability.
 */
Ability fromNameToAbilityEnum(const std::string& name);

/**
 * @brief Returns the enum of the item name string.
 * Returns `NO_ITEM` if the string does not represent an item.
 */
Item fromNameToItemEnum(const std::string& name);

/**
 * @brief Returns the enum of the nature name string.
 * Returns `NO_NATURE` if the string does not represent a nature.
 */
Nature fromNameToNatureEnum(const std::string& name);

/**
 * @brief Returns the enum of the status condition name string.
 * Returns `NO_STATUS` if the string does not represent a status condition.
 */
Status fromNameToStatusEnum(const std::string& name);

/**
 * @brief Returns the enum of the gender name string.
 * Returns `NO_GENDER` if the string does not represent a gender.
 */
Gender fromNameToGenderEnum(const std::string& name);

/**
 * @brief Returns the enum of the type name string.
 * Returns `NO_TYPE` if the string does not represent a type.
 */
Type fromNameToTypeEnum(const std::string& name);

/**
 * @brief Returns `NO_STAT` if the string does not represent a stat.
 */
Stat fromNameToStatEnum(const std::string& name);

/**
 * @brief Returns the enum of the species Smogon name string.
 * Returns `MISSING_NO` if the string does not represent a species.
 */
Species fromSmogonNameToSpeciesEnum(const std::string& name);

/**
 * @brief Returns the enum of the move Smogon name string.
 * Returns `NO_MOVE` if the string does not represent a move.
 */
Move fromSmogonNameToMoveEnum(const std::string& name);

/**
 * @brief Returns the enum of the ability Smogon name string.
 * Returns `NO_ABILITY` if the string does not represent an ability.
 */
Ability fromSmogonNameToAbilityEnum(const std::string& name);

/**
 * @brief Returns the enum of the item Smogon name string.
 * Returns `NO_ITEM` if the string does not represent an item.
 */
Item fromSmogonNameToItemEnum(const std::string& name);

/**
 * @brief Returns the enum of the species Smogon ID string.
 * Returns `MISSING_NO` if the string does not represent a species.
 */
Species fromSmogonIDToSpeciesEnum(const std::string& name);

/**
 * @brief Returns the enum of the move Smogon ID string.
 * Returns `NO_MOVE` if the string does not represent a move.
 */
Move fromSmogonIDToMoveEnum(const std::string& name);

/**
 * @brief Returns the enum of the ability Smogon ID string.
 * Returns `NO_ABILITY` if the string does not represent an ability.
 */
Ability fromSmogonIDToAbilityEnum(const std::string& name);

/**
 * @brief Returns the enum of the item Smogon ID string.
 * Returns `NO_ITEM` if the string does not represent an item.
 */
Item fromSmogonIDToItemEnum(const std::string& name);

// Converts a string to only have lowercase alphanumeric characters
std::string toID(const std::string& name);
}  // namespace pokesim::dex
