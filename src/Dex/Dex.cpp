#include "Dex.hpp"
#include <entt/container/dense_map.hpp>

namespace pokesim {
namespace internal {
  template <typename NameEnum, typename StringArray>
  entt::dense_map<std::string, NameEnum> static createNameToEnumMap(const StringArray& array) {
    entt::dense_map<std::string, NameEnum> map;
    for (uint i = 0; i < array.size(); i++) {
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
}

Dex::Species Dex::fromNameToSpeciesEnum(const std::string& name) {
  static const entt::dense_map<std::string, Species> map = internal::createNameToEnumMap<Species>(SpeciesNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Move Dex::fromNameToMoveEnum(const std::string& name) {
  static const entt::dense_map<std::string, Move> map = internal::createNameToEnumMap<Move>(MoveNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Ability Dex::fromNameToAbilityEnum(const std::string& name) {
  static const entt::dense_map<std::string, Ability> map = internal::createNameToEnumMap<Ability>(AbilityNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Item Dex::fromNameToItemEnum(const std::string& name) {
  static const entt::dense_map<std::string, Item> map = internal::createNameToEnumMap<Item>(ItemNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Nature Dex::fromNameToNatureEnum(const std::string& name) {
  static const entt::dense_map<std::string, Nature> map = internal::createNameToEnumMap<Nature>(NatureNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Status Dex::fromNameToStatusEnum(const std::string& name) {
  static const entt::dense_map<std::string, Status> map = internal::createNameToEnumMap<Status>(StatusNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Gender Dex::fromNameToGenderEnum(const std::string& name) {
  static const entt::dense_map<std::string, Gender> map = internal::createNameToEnumMap<Gender>(GenderNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Type Dex::fromNameToTypeEnum(const std::string& name) {
  static const entt::dense_map<std::string, Type> map = internal::createNameToEnumMap<Type>(TypeNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Stat Dex::fromNameToStatEnum(const std::string& name) {
  static const entt::dense_map<std::string, Stat> map = internal::createNameToEnumMap<Stat>(StatNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Species Dex::fromSmogonNameToSpeciesEnum(const std::string& name) {
  static const entt::dense_map<std::string, Species> map = internal::createNameToEnumMap<Species>(SpeciesSmogonNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Move Dex::fromSmogonNameToMoveEnum(const std::string& name) {
  static const entt::dense_map<std::string, Move> map = internal::createNameToEnumMap<Move>(MoveSmogonNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Ability Dex::fromSmogonNameToAbilityEnum(const std::string& name) {
  static const entt::dense_map<std::string, Ability> map = internal::createNameToEnumMap<Ability>(AbilitySmogonNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Item Dex::fromSmogonNameToItemEnum(const std::string& name) {
  static const entt::dense_map<std::string, Item> map = internal::createNameToEnumMap<Item>(ItemSmogonNames);
  return internal::fromStringToEnum(map, name);
}

Dex::Species Dex::fromSmogonIDToSpeciesEnum(const std::string& name) {
  static const entt::dense_map<std::string, Species> map = internal::createNameToEnumMap<Species>(SpeciesSmogonIDs);
  return internal::fromStringToEnum(map, name);
}

Dex::Move Dex::fromSmogonIDToMoveEnum(const std::string& name) {
  static const entt::dense_map<std::string, Move> map = internal::createNameToEnumMap<Move>(MoveSmogonIDs);
  return internal::fromStringToEnum(map, name);
}

Dex::Ability Dex::fromSmogonIDToAbilityEnum(const std::string& name) {
  static const entt::dense_map<std::string, Ability> map = internal::createNameToEnumMap<Ability>(AbilitySmogonIDs);
  return internal::fromStringToEnum(map, name);
}

Dex::Item Dex::fromSmogonIDToItemEnum(const std::string& name) {
  static const entt::dense_map<std::string, Item> map = internal::createNameToEnumMap<Item>(ItemSmogonIDs);
  return internal::fromStringToEnum(map, name);
}

std::string Dex::toID(const std::string& name) {
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
