#include "Dex.hpp"

#include <entt/container/dense_map.hpp>

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
  static const entt::dense_map<std::string, Species> map = internal::createNameToEnumMap<Species>(internal::SpeciesNames);
  return internal::fromStringToEnum(map, name);
}

Move fromNameToMoveEnum(const std::string& name) {
  static const entt::dense_map<std::string, Move> map = internal::createNameToEnumMap<Move>(internal::MoveNames);
  return internal::fromStringToEnum(map, name);
}

Ability fromNameToAbilityEnum(const std::string& name) {
  static const entt::dense_map<std::string, Ability> map = internal::createNameToEnumMap<Ability>(internal::AbilityNames);
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
  static const entt::dense_map<std::string, Species> map = internal::createNameToEnumMap<Species>(internal::SpeciesSmogonNames);
  return internal::fromStringToEnum(map, name);
}

Move fromSmogonNameToMoveEnum(const std::string& name) {
  static const entt::dense_map<std::string, Move> map = internal::createNameToEnumMap<Move>(internal::MoveSmogonNames);
  return internal::fromStringToEnum(map, name);
}

Ability fromSmogonNameToAbilityEnum(const std::string& name) {
  static const entt::dense_map<std::string, Ability> map = internal::createNameToEnumMap<Ability>(internal::AbilitySmogonNames);
  return internal::fromStringToEnum(map, name);
}

Item fromSmogonNameToItemEnum(const std::string& name) {
  static const entt::dense_map<std::string, Item> map = internal::createNameToEnumMap<Item>(internal::ItemSmogonNames);
  return internal::fromStringToEnum(map, name);
}

Species fromSmogonIDToSpeciesEnum(const std::string& name) {
  static const entt::dense_map<std::string, Species> map = internal::createNameToEnumMap<Species>(internal::SpeciesSmogonIDs);
  return internal::fromStringToEnum(map, name);
}

Move fromSmogonIDToMoveEnum(const std::string& name) {
  static const entt::dense_map<std::string, Move> map = internal::createNameToEnumMap<Move>(internal::MoveSmogonIDs);
  return internal::fromStringToEnum(map, name);
}

Ability fromSmogonIDToAbilityEnum(const std::string& name) {
  static const entt::dense_map<std::string, Ability> map = internal::createNameToEnumMap<Ability>(internal::AbilitySmogonIDs);
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
}  // namespace pokesim
