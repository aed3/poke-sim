#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Pokedex Data Building: Species", "[Pokedex]") {
  Pokedex pokedex(GameMechanics::SCARLET_VIOLET);

  entt::dense_set<dex::Species> speciesList{};
  for (dex::Species species : {dex::Species::EMPOLEON, dex::Species::AMPHAROS, dex::Species::GARDEVOIR}) {
    speciesList.insert(species);
  }

  pokedex.loadSpecies(speciesList);

  {
    INFO("Empoleon");
    auto [species, types, baseStats, primaryAbility, hiddenAbility] =
      pokedex.getSpeciesData<SpeciesName, SpeciesTypes, BaseStats, PrimaryAbility, HiddenAbility>(
        dex::Species::EMPOLEON);

    REQUIRE(species.val == pokedex.getStaticValue<dex::Empoleon::name>());
    REQUIRE(types.type1() == pokedex.getStaticValue<dex::Empoleon::type>().type1());
    REQUIRE(types.type2() == pokedex.getStaticValue<dex::Empoleon::type>().type2());
    REQUIRE(baseStats.hp == pokedex.getStaticValue<dex::Empoleon::hp>());
    REQUIRE(baseStats.atk == pokedex.getStaticValue<dex::Empoleon::atk>());
    REQUIRE(baseStats.def == pokedex.getStaticValue<dex::Empoleon::def>());
    REQUIRE(baseStats.spa == pokedex.getStaticValue<dex::Empoleon::spa>());
    REQUIRE(baseStats.spd == pokedex.getStaticValue<dex::Empoleon::spd>());
    REQUIRE(baseStats.spe == pokedex.getStaticValue<dex::Empoleon::spe>());

    REQUIRE(primaryAbility.val == pokedex.getStaticValue<dex::Empoleon::primaryAbility>());
    REQUIRE(hiddenAbility.val == pokedex.getStaticValue<dex::Empoleon::hiddenAbility>());
    REQUIRE(hiddenAbility.val == dex::Empoleon::hiddenAbility(GameMechanics::SCARLET_VIOLET));

    REQUIRE_FALSE(pokedex.speciesHas<SecondaryAbility>(dex::Species::EMPOLEON));

    REQUIRE(dex::Empoleon::hiddenAbility(GameMechanics::BRILLIANT_DIAMOND_SHINING_PEARL) == dex::Ability::DEFIANT);
  }

  {
    INFO("Ampharos");
    auto [species, types, baseStats, primaryAbility, hiddenAbility] =
      pokedex.getSpeciesData<SpeciesName, SpeciesTypes, BaseStats, PrimaryAbility, HiddenAbility>(
        dex::Species::AMPHAROS);

    REQUIRE(species.val == pokedex.getStaticValue<dex::Ampharos::name>());
    REQUIRE(types.type1() == pokedex.getStaticValue<dex::Ampharos::type>().type1());
    REQUIRE(types.type2() == pokedex.getStaticValue<dex::Ampharos::type>().type2());
    REQUIRE(baseStats.hp == pokedex.getStaticValue<dex::Ampharos::hp>());
    REQUIRE(baseStats.atk == pokedex.getStaticValue<dex::Ampharos::atk>());
    REQUIRE(baseStats.def == pokedex.getStaticValue<dex::Ampharos::def>());
    REQUIRE(baseStats.spa == pokedex.getStaticValue<dex::Ampharos::spa>());
    REQUIRE(baseStats.spd == pokedex.getStaticValue<dex::Ampharos::spd>());
    REQUIRE(baseStats.spe == pokedex.getStaticValue<dex::Ampharos::spe>());

    REQUIRE(primaryAbility.val == pokedex.getStaticValue<dex::Ampharos::primaryAbility>());
    REQUIRE(hiddenAbility.val == pokedex.getStaticValue<dex::Ampharos::hiddenAbility>());

    REQUIRE_FALSE(pokedex.speciesHas<SecondaryAbility>(dex::Species::AMPHAROS));
  }

  {
    INFO("Gardevoir");
    auto [species, types, baseStats, primaryAbility, secondaryAbility, hiddenAbility] =
      pokedex.getSpeciesData<SpeciesName, SpeciesTypes, BaseStats, PrimaryAbility, SecondaryAbility, HiddenAbility>(
        dex::Species::GARDEVOIR);

    REQUIRE(species.val == pokedex.getStaticValue<dex::Gardevoir::name>());
    REQUIRE(types.type1() == pokedex.getStaticValue<dex::Gardevoir::type>().type1());
    REQUIRE(types.type2() == pokedex.getStaticValue<dex::Gardevoir::type>().type2());
    REQUIRE(baseStats.hp == pokedex.getStaticValue<dex::Gardevoir::hp>());
    REQUIRE(baseStats.atk == pokedex.getStaticValue<dex::Gardevoir::atk>());
    REQUIRE(baseStats.def == pokedex.getStaticValue<dex::Gardevoir::def>());
    REQUIRE(baseStats.spa == pokedex.getStaticValue<dex::Gardevoir::spa>());
    REQUIRE(baseStats.spd == pokedex.getStaticValue<dex::Gardevoir::spd>());
    REQUIRE(baseStats.spe == pokedex.getStaticValue<dex::Gardevoir::spe>());

    REQUIRE(primaryAbility.val == pokedex.getStaticValue<dex::Gardevoir::primaryAbility>());
    REQUIRE(secondaryAbility.val == pokedex.getStaticValue<dex::Gardevoir::secondaryAbility>());
    REQUIRE(hiddenAbility.val == pokedex.getStaticValue<dex::Gardevoir::hiddenAbility>());
  }
}

TEST_CASE("Pokedex Data Building: Item", "[Pokedex]") {
  Pokedex pokedex(GameMechanics::SCARLET_VIOLET);

  entt::dense_set<dex::Item> itemList{};
  for (dex::Item item : {dex::Item::CHOICE_SCARF, dex::Item::CHOICE_SPECS, dex::Item::ASSAULT_VEST}) {
    itemList.insert(item);
  }
  pokedex.loadItems(itemList);

  {
    auto item = pokedex.getItemData<ItemName>(dex::Item::CHOICE_SCARF);
    REQUIRE(item.val == dex::Item::CHOICE_SCARF);
    REQUIRE(pokedex.itemHas<item::tags::Choice>(dex::Item::CHOICE_SCARF));
  }

  {
    auto item = pokedex.getItemData<ItemName>(dex::Item::CHOICE_SPECS);
    REQUIRE(item.val == dex::Item::CHOICE_SPECS);
    REQUIRE(pokedex.itemHas<item::tags::Choice>(dex::Item::CHOICE_SPECS));
  }

  {
    auto item = pokedex.getItemData<ItemName>(dex::Item::ASSAULT_VEST);
    REQUIRE(item.val == dex::Item::ASSAULT_VEST);
    REQUIRE_FALSE(pokedex.itemHas<item::tags::Choice>(dex::Item::ASSAULT_VEST));
  }
}

TEST_CASE("Pokedex Data Building: Move", "[Pokedex]") {
  Pokedex pokedex(GameMechanics::SCARLET_VIOLET);

  entt::dense_set<dex::Move> moveList{};
  for (dex::Move move :
       {dex::Move::FURY_ATTACK,
        dex::Move::THUNDERBOLT,
        dex::Move::MOONBLAST,
        dex::Move::WILL_O_WISP,
        dex::Move::QUIVER_DANCE}) {
    moveList.insert(move);
  }
  pokedex.loadMoves(moveList);

  {
    INFO("Fury Attack");
    dex::Move moveEnum = dex::Move::FURY_ATTACK;

    auto [move, type, accuracy, power, pp] = pokedex.getMoveData<MoveName, TypeName, Accuracy, BasePower, Pp>(moveEnum);

    REQUIRE(move.val == pokedex.getStaticValue<dex::FuryAttack::name>());
    REQUIRE(type.val == pokedex.getStaticValue<dex::FuryAttack::type>());
    REQUIRE(pokedex.moveHas<move::tags::Physical>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Special>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Status>(moveEnum));
    REQUIRE(accuracy.val == pokedex.getStaticValue<dex::FuryAttack::accuracy>());
    REQUIRE(power.val == pokedex.getStaticValue<dex::FuryAttack::basePower>());
    REQUIRE(pp.val == pokedex.getStaticValue<dex::FuryAttack::basePp>());

    REQUIRE(pokedex.moveHas<move::tags::AnySingleTarget>(moveEnum));
    REQUIRE(pokedex.moveHas<move::tags::Contact>(moveEnum));
    REQUIRE(pokedex.moveHas<move::tags::VariableHitCount>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<AddedTargets>(moveEnum));
  }

  {
    INFO("Thunderbolt");
    dex::Move moveEnum = dex::Move::THUNDERBOLT;

    auto [move, type, accuracy, power, pp] = pokedex.getMoveData<MoveName, TypeName, Accuracy, BasePower, Pp>(moveEnum);

    REQUIRE(move.val == pokedex.getStaticValue<dex::Thunderbolt::name>());
    REQUIRE(type.val == pokedex.getStaticValue<dex::Thunderbolt::type>());
    REQUIRE(pokedex.moveHas<move::tags::Special>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Physical>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Status>(moveEnum));
    REQUIRE(accuracy.val == pokedex.getStaticValue<dex::Thunderbolt::accuracy>());
    REQUIRE(power.val == pokedex.getStaticValue<dex::Thunderbolt::basePower>());
    REQUIRE(pp.val == pokedex.getStaticValue<dex::Thunderbolt::basePp>());

    REQUIRE(pokedex.moveHas<move::effect::tags::Secondary>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::effect::tags::Primary>(moveEnum));
    REQUIRE(pokedex.moveHas<move::effect::tags::MoveTarget>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::effect::tags::MoveSource>(moveEnum));

    REQUIRE(pokedex.moveHas<status::tags::Paralysis>(moveEnum));
    REQUIRE(
      pokedex.getMoveData<BaseEffectChance>(moveEnum).val ==
      pokedex.getStaticValue<dex::Thunderbolt::targetSecondaryEffect::chance>());

    REQUIRE(pokedex.moveHas<move::tags::AnySingleTarget>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<AddedTargets>(moveEnum));
  }

  {
    INFO("Moonblast");
    dex::Move moveEnum = dex::Move::MOONBLAST;

    auto [move, type, accuracy, power, pp] = pokedex.getMoveData<MoveName, TypeName, Accuracy, BasePower, Pp>(moveEnum);

    REQUIRE(move.val == pokedex.getStaticValue<dex::Moonblast::name>());
    REQUIRE(type.val == pokedex.getStaticValue<dex::Moonblast::type>());
    REQUIRE(pokedex.moveHas<move::tags::Special>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Physical>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Status>(moveEnum));
    REQUIRE(accuracy.val == pokedex.getStaticValue<dex::Moonblast::accuracy>());
    REQUIRE(power.val == pokedex.getStaticValue<dex::Moonblast::basePower>());
    REQUIRE(pp.val == pokedex.getStaticValue<dex::Moonblast::basePp>());

    REQUIRE(pokedex.moveHas<move::effect::tags::Secondary>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::effect::tags::Primary>(moveEnum));
    REQUIRE(pokedex.moveHas<move::effect::tags::MoveTarget>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::effect::tags::MoveSource>(moveEnum));

    auto [chance, spaBoost] = pokedex.getMoveData<BaseEffectChance, SpaBoost>(moveEnum);
    REQUIRE(chance.val == pokedex.getStaticValue<dex::Moonblast::targetSecondaryEffect::chance>());
    REQUIRE(spaBoost.val == pokedex.getStaticValue<dex::Moonblast::targetSecondaryEffect::spaBoost>());

    REQUIRE(pokedex.moveHas<move::tags::AnySingleTarget>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<AddedTargets>(moveEnum));
  }

  {
    INFO("Will-O-Wisp");
    dex::Move moveEnum = dex::Move::WILL_O_WISP;

    auto [move, type, accuracy, pp] = pokedex.getMoveData<MoveName, TypeName, Accuracy, Pp>(moveEnum);

    REQUIRE(move.val == pokedex.getStaticValue<dex::WillOWisp::name>());
    REQUIRE(type.val == pokedex.getStaticValue<dex::WillOWisp::type>());
    REQUIRE(pokedex.moveHas<move::tags::Status>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Physical>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Special>(moveEnum));
    REQUIRE(accuracy.val == pokedex.getStaticValue<dex::WillOWisp::accuracy>());
    REQUIRE(pp.val == pokedex.getStaticValue<dex::WillOWisp::basePp>());

    REQUIRE_FALSE(pokedex.moveHas<BasePower>(moveEnum));

    REQUIRE(pokedex.moveHas<move::effect::tags::Primary>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::effect::tags::Secondary>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<BaseEffectChance>(moveEnum));
    REQUIRE(pokedex.moveHas<move::effect::tags::MoveTarget>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::effect::tags::MoveSource>(moveEnum));

    REQUIRE(pokedex.moveHas<status::tags::Burn>(moveEnum));

    REQUIRE(pokedex.moveHas<move::tags::AnySingleTarget>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<AddedTargets>(moveEnum));
  }

  {
    INFO("Quiver Dance");
    dex::Move moveEnum = dex::Move::QUIVER_DANCE;

    auto [move, type, pp] = pokedex.getMoveData<MoveName, TypeName, Pp>(moveEnum);

    REQUIRE(move.val == pokedex.getStaticValue<dex::QuiverDance::name>());
    REQUIRE(type.val == pokedex.getStaticValue<dex::QuiverDance::type>());
    REQUIRE(pokedex.moveHas<move::tags::Status>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Physical>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Special>(moveEnum));
    REQUIRE(pp.val == pokedex.getStaticValue<dex::QuiverDance::basePp>());

    REQUIRE_FALSE(pokedex.moveHas<BasePower>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<Accuracy>(moveEnum));

    REQUIRE(pokedex.moveHas<move::effect::tags::Primary>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::effect::tags::Secondary>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<BaseEffectChance>(moveEnum));
    REQUIRE(pokedex.moveHas<move::effect::tags::MoveTarget>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::effect::tags::MoveSource>(moveEnum));

    auto [spaBoost, spdBoost, speBoost] = pokedex.getMoveData<SpaBoost, SpdBoost, SpeBoost>(moveEnum);
    REQUIRE(spaBoost.val == pokedex.getStaticValue<dex::QuiverDance::targetPrimaryEffect::spaBoost>());
    REQUIRE(spdBoost.val == pokedex.getStaticValue<dex::QuiverDance::targetPrimaryEffect::spdBoost>());
    REQUIRE(speBoost.val == pokedex.getStaticValue<dex::QuiverDance::targetPrimaryEffect::speBoost>());

    REQUIRE(pokedex.moveHas<move::tags::Self>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<AddedTargets>(moveEnum));
  }
}
}  // namespace pokesim
