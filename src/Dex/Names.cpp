#include "Names.hpp"

namespace pokesim::internal {
Names::Species Names::fromNameToSpeciesEnum(const std::string& name) {
  return fromStringToEnum(NameStringsToSpecies, name);
}

Names::Move Names::fromNameToMoveEnum(const std::string& name) {
  return fromStringToEnum(NameStringsToMove, name);
}

Names::Ability Names::fromNameToAbilityEnum(const std::string& name) {
  return fromStringToEnum(NameStringsToAbility, name);
}

Names::Item Names::fromNameToItemEnum(const std::string& name) {
  return fromStringToEnum(NameStringsToItem, name);
}

Names::Nature Names::fromNameToNatureEnum(const std::string& name) {
  return fromStringToEnum(NameStringsToNature, name);
}

Names::Status Names::fromNameToStatusEnum(const std::string& name) {
  return fromStringToEnum(NameStringsToStatus, name);
}

Names::Gender Names::fromNameToGenderEnum(const std::string& name) {
  return fromStringToEnum(NameStringsToGender, name);
}

Names::Type Names::fromNameToTypeEnum(const std::string& name) {
  return fromStringToEnum(NameStringsToType, name);
}

Names::Stat Names::fromNameToStatEnum(const std::string& name) {
  return fromStringToEnum(NameStringsToStat, name);
}

Names::Species Names::fromSmogonNameToSpeciesEnum(const std::string& name) {
  return fromStringToEnum(SmogonStringsToSpecies, name);
}

Names::Move Names::fromSmogonNameToMoveEnum(const std::string& name) {
  return fromStringToEnum(SmogonStringsToMove, name);
}

Names::Ability Names::fromSmogonNameToAbilityEnum(const std::string& name) {
  return fromStringToEnum(SmogonStringsToAbility, name);
}

Names::Item Names::fromSmogonNameToItemEnum(const std::string& name) {
  return fromStringToEnum(SmogonStringsToItem, name);
}

Names::Species Names::fromSmogonIDToSpeciesEnum(const std::string& name) {
  return fromStringToEnum(SmogonIDsToSpecies, name);
}

Names::Move Names::fromSmogonIDToMoveEnum(const std::string& name) {
  return fromStringToEnum(SmogonIDsToMove, name);
}

Names::Ability Names::fromSmogonIDToAbilityEnum(const std::string& name) {
  return fromStringToEnum(SmogonIDsToAbility, name);
}

Names::Item Names::fromSmogonIDToItemEnum(const std::string& name) {
  return fromStringToEnum(SmogonIDsToItem, name);
}
}  // namespace pokesim::internal
