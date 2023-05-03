#pragma once

#include <Types/headers.hpp>
#include <array>
#include <cstdint>
#include <string>

namespace pokesim::dex {
/** @brief Returns the name of a species represented by its enum as a string. */
/*_inline_*/ std::string toString(Species speciesEnum);

/** @brief Returns the name of a move represented by its enum as a string. */
/*_inline_*/ std::string toString(Move moveEnum);

/** @brief Returns the name of an ability represented by its enum as a string. */
/*_inline_*/ std::string toString(Ability abilityEnum);

/** @brief Returns the name of an item represented by its enum as a string. */
/*_inline_*/ std::string toString(Item itemEnum);

/** @brief Returns the name of a nature represented by its enum as a string. */
/*_inline_*/ std::string toString(Nature natureEnum);

/** @brief Returns the name of a status condition represented by its enum as a string. */
/*_inline_*/ std::string toString(Status statusEnum);

/** @brief Returns the name of a gender represented by its enum as a string. */
/*_inline_*/ std::string toString(Gender genderEnum);

/** @brief Returns the name of a type represented by its enum as a string. */
/*_inline_*/ std::string toString(Type typeEnum);

/** @brief Returns the name of a stat represented by its enum as a string. */
/*_inline_*/ std::string toString(Stat statEnum);

/** @brief Returns the Smogon name of a species represented by its enum as a string. */
/*_inline_*/ std::string toSmogonString(Species speciesEnum);

/** @brief Returns the Smogon name of a move represented by its enum as a string. */
/*_inline_*/ std::string toSmogonString(Move moveEnum);

/** @brief Returns the Smogon name of an ability represented by its enum as a string. */
/*_inline_*/ std::string toSmogonString(Ability abilityEnum);

/** @brief Returns the Smogon name of an item represented by its enum as a string. */
/*_inline_*/ std::string toSmogonString(Item itemEnum);

/** @brief Returns the Smogon ID of a species represented by its enum as a string. */
/*_inline_*/ std::string toSmogonIdString(Species speciesEnum);

/** @brief Returns the Smogon ID of a move represented by its enum as a string. */
/*_inline_*/ std::string toSmogonIdString(Move moveEnum);

/** @brief Returns the Smogon ID of an ability represented by its enum as a string. */
/*_inline_*/ std::string toSmogonIdString(Ability abilityEnum);

/** @brief Returns the Smogon ID of an item represented by its enum as a string. */
/*_inline_*/ std::string toSmogonIdString(Item itemEnum);

/**
 * @brief Returns the enum of the species name string.
 * Returns `MISSING_NO` if the string does not represent a species.
 */
/*_inline_*/ Species fromNameToSpeciesEnum(const std::string& name);

/**
 * @brief Returns the enum of the move name string.
 * Returns `NO_MOVE` if the string does not represent a move.
 */
/*_inline_*/ Move fromNameToMoveEnum(const std::string& name);

/**
 * @brief Returns the enum of the ability name string.
 * Returns `NO_ABILITY` if the string does not represent an ability.
 */
/*_inline_*/ Ability fromNameToAbilityEnum(const std::string& name);

/**
 * @brief Returns the enum of the item name string.
 * Returns `NO_ITEM` if the string does not represent an item.
 */
/*_inline_*/ Item fromNameToItemEnum(const std::string& name);

/**
 * @brief Returns the enum of the nature name string.
 * Returns `NO_NATURE` if the string does not represent a nature.
 */
/*_inline_*/ Nature fromNameToNatureEnum(const std::string& name);

/**
 * @brief Returns the enum of the status condition name a string.
 * Returns `NO_STATUS` if the string does not represent a status condition.
 */
/*_inline_*/ Status fromNameToStatusEnum(const std::string& name);

/**
 * @brief Returns the enum of the gender name string.
 * Returns `NO_GENDER` if the string does not represent a gender.
 */
/*_inline_*/ Gender fromNameToGenderEnum(const std::string& name);

/**
 * @brief Returns the enum of the type name string.
 * Returns `NO_TYPE` if the string does not represent a type.
 */
/*_inline_*/ Type fromNameToTypeEnum(const std::string& name);

/**
 * @brief Returns `NO_STAT` if the string does not represent a stat.
 */
/*_inline_*/ Stat fromNameToStatEnum(const std::string& name);

/**
 * @brief Returns the enum of the species Smogon name string.
 * Returns `MISSING_NO` if the string does not represent a species.
 */
/*_inline_*/ Species fromSmogonNameToSpeciesEnum(const std::string& name);

/**
 * @brief Returns the enum of the move Smogon name string.
 * Returns `NO_MOVE` if the string does not represent a move.
 */
/*_inline_*/ Move fromSmogonNameToMoveEnum(const std::string& name);

/**
 * @brief Returns the enum of the ability Smogon name string.
 * Returns `NO_ABILITY` if the string does not represent an ability.
 */
/*_inline_*/ Ability fromSmogonNameToAbilityEnum(const std::string& name);

/**
 * @brief Returns the enum of the item Smogon name string.
 * Returns `NO_ITEM` if the string does not represent an item.
 */
/*_inline_*/ Item fromSmogonNameToItemEnum(const std::string& name);

/**
 * @brief Returns the enum of the species Smogon ID string.
 * Returns `MISSING_NO` if the string does not represent a species.
 */
/*_inline_*/ Species fromSmogonIDToSpeciesEnum(const std::string& name);

/**
 * @brief Returns the enum of the move Smogon ID string.
 * Returns `NO_MOVE` if the string does not represent a move.
 */
/*_inline_*/ Move fromSmogonIDToMoveEnum(const std::string& name);

/**
 * @brief Returns the enum of the ability Smogon ID string.
 * Returns `NO_ABILITY` if the string does not represent an ability.
 */
/*_inline_*/ Ability fromSmogonIDToAbilityEnum(const std::string& name);

/**
 * @brief Returns the enum of the item Smogon ID string.
 * Returns `NO_ITEM` if the string does not represent an item.
 */
/*_inline_*/ Item fromSmogonIDToItemEnum(const std::string& name);

/** @brief Converts a string to only have lowercase alphanumeric characters */
/*_inline_*/ std::string toID(const std::string& name);
}  // namespace pokesim::dex
