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
    using Empoleon = dex::Empoleon<GameMechanics::SCARLET_VIOLET>;

    auto [species, types, baseStats, primaryAbility, hiddenAbility] =
      pokedex.getSpeciesData<SpeciesName, SpeciesTypes, BaseStats, PrimaryAbility, HiddenAbility>(
        dex::Species::EMPOLEON);

    REQUIRE(species.name == Empoleon::name);
    REQUIRE(types.type1() == Empoleon::type.type1());
    REQUIRE(types.type2() == Empoleon::type.type2());
    REQUIRE(baseStats.hp == Empoleon::hp);
    REQUIRE(baseStats.atk == Empoleon::atk);
    REQUIRE(baseStats.def == Empoleon::def);
    REQUIRE(baseStats.spa == Empoleon::spa);
    REQUIRE(baseStats.spd == Empoleon::spd);
    REQUIRE(baseStats.spe == Empoleon::spe);

    REQUIRE(primaryAbility.name == Empoleon::primaryAbility);
    REQUIRE(hiddenAbility.name == Empoleon::hiddenAbility);

    REQUIRE_FALSE(pokedex.speciesHas<SecondaryAbility>(dex::Species::EMPOLEON));

    REQUIRE(dex::Empoleon<GameMechanics::BRILLIANT_DIAMOND_SHINING_PEARL>::hiddenAbility == dex::Ability::DEFIANT);
  }

  {
    INFO("Ampharos");
    using Ampharos = dex::Ampharos<GameMechanics::SCARLET_VIOLET>;

    auto [species, types, baseStats, primaryAbility, hiddenAbility] =
      pokedex.getSpeciesData<SpeciesName, SpeciesTypes, BaseStats, PrimaryAbility, HiddenAbility>(
        dex::Species::AMPHAROS);

    REQUIRE(species.name == Ampharos::name);
    REQUIRE(types.type1() == Ampharos::type.type1());
    REQUIRE(types.type2() == Ampharos::type.type2());
    REQUIRE(baseStats.hp == Ampharos::hp);
    REQUIRE(baseStats.atk == Ampharos::atk);
    REQUIRE(baseStats.def == Ampharos::def);
    REQUIRE(baseStats.spa == Ampharos::spa);
    REQUIRE(baseStats.spd == Ampharos::spd);
    REQUIRE(baseStats.spe == Ampharos::spe);

    REQUIRE(primaryAbility.name == Ampharos::primaryAbility);
    REQUIRE(hiddenAbility.name == Ampharos::hiddenAbility);

    REQUIRE_FALSE(pokedex.speciesHas<SecondaryAbility>(dex::Species::AMPHAROS));
  }

  {
    INFO("Gardevoir");
    using Gardevoir = dex::Gardevoir<GameMechanics::SCARLET_VIOLET>;

    auto [species, types, baseStats, primaryAbility, secondaryAbility, hiddenAbility] =
      pokedex.getSpeciesData<SpeciesName, SpeciesTypes, BaseStats, PrimaryAbility, SecondaryAbility, HiddenAbility>(
        dex::Species::GARDEVOIR);

    REQUIRE(species.name == Gardevoir::name);
    REQUIRE(types.type1() == Gardevoir::type.type1());
    REQUIRE(types.type2() == Gardevoir::type.type2());
    REQUIRE(baseStats.hp == Gardevoir::hp);
    REQUIRE(baseStats.atk == Gardevoir::atk);
    REQUIRE(baseStats.def == Gardevoir::def);
    REQUIRE(baseStats.spa == Gardevoir::spa);
    REQUIRE(baseStats.spd == Gardevoir::spd);
    REQUIRE(baseStats.spe == Gardevoir::spe);

    REQUIRE(primaryAbility.name == Gardevoir::primaryAbility);
    REQUIRE(secondaryAbility.name == Gardevoir::secondaryAbility);
    REQUIRE(hiddenAbility.name == Gardevoir::hiddenAbility);
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
    REQUIRE(item.name == dex::Item::CHOICE_SCARF);
  }

  {
    auto item = pokedex.getItemData<ItemName>(dex::Item::CHOICE_SPECS);
    REQUIRE(item.name == dex::Item::CHOICE_SPECS);
  }

  {
    auto item = pokedex.getItemData<ItemName>(dex::Item::ASSAULT_VEST);
    REQUIRE(item.name == dex::Item::ASSAULT_VEST);
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
    using FuryAttack = dex::FuryAttack<GameMechanics::SCARLET_VIOLET>;
    dex::Move moveEnum = dex::Move::FURY_ATTACK;

    auto [move, type, accuracy, power, pp, multiHit] =
      pokedex.getMoveData<MoveName, TypeName, Accuracy, BasePower, Pp, MultiHit>(moveEnum);

    REQUIRE(move.name == FuryAttack::name);
    REQUIRE(type.name == FuryAttack::type);
    REQUIRE(pokedex.moveHas<move::tags::Physical>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Special>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Status>(moveEnum));
    REQUIRE(accuracy.val == FuryAttack::accuracy);
    REQUIRE(power.val == FuryAttack::basePower);
    REQUIRE(pp.val == FuryAttack::basePp);
    REQUIRE(multiHit.minHits == FuryAttack::minHits);
    REQUIRE(multiHit.maxHits == FuryAttack::maxHits);

    REQUIRE(pokedex.moveHas<move::tags::AnySingleTarget>(moveEnum));
    REQUIRE(pokedex.moveHas<move::tags::Contact>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<AddedTargets>(moveEnum));
  }

  {
    INFO("Thunderbolt");
    using Thunderbolt = dex::Thunderbolt<GameMechanics::SCARLET_VIOLET>;
    dex::Move moveEnum = dex::Move::THUNDERBOLT;

    auto [move, type, accuracy, power, pp, secondaryEffect] =
      pokedex.getMoveData<MoveName, TypeName, Accuracy, BasePower, Pp, MoveEffect>(moveEnum);

    REQUIRE(move.name == Thunderbolt::name);
    REQUIRE(type.name == Thunderbolt::type);
    REQUIRE(pokedex.moveHas<move::tags::Special>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Physical>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Status>(moveEnum));
    REQUIRE(accuracy.val == Thunderbolt::accuracy);
    REQUIRE(power.val == Thunderbolt::basePower);
    REQUIRE(pp.val == Thunderbolt::basePp);

    REQUIRE_FALSE(secondaryEffect.primary);

    auto chance = pokedex.getEffectData<Chance>(secondaryEffect);
    REQUIRE(chance.val == Thunderbolt::targetSecondaryEffect::chance);
    REQUIRE(pokedex.effectHas<move::tags::effect::MoveTarget>(secondaryEffect));
    REQUIRE(pokedex.effectHas<status::tags::Paralysis>(secondaryEffect));
    REQUIRE_FALSE(pokedex.effectHas<move::tags::effect::MoveSource>(secondaryEffect));

    REQUIRE(pokedex.moveHas<move::tags::AnySingleTarget>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<AddedTargets>(moveEnum));
  }

  {
    INFO("Moonblast");
    using Moonblast = dex::Moonblast<GameMechanics::SCARLET_VIOLET>;
    dex::Move moveEnum = dex::Move::MOONBLAST;

    auto [move, type, accuracy, power, pp, secondaryEffect] =
      pokedex.getMoveData<MoveName, TypeName, Accuracy, BasePower, Pp, MoveEffect>(moveEnum);

    REQUIRE(move.name == Moonblast::name);
    REQUIRE(type.name == Moonblast::type);
    REQUIRE(pokedex.moveHas<move::tags::Special>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Physical>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Status>(moveEnum));
    REQUIRE(accuracy.val == Moonblast::accuracy);
    REQUIRE(power.val == Moonblast::basePower);
    REQUIRE(pp.val == Moonblast::basePp);

    REQUIRE_FALSE(secondaryEffect.primary);

    auto [chance, spaBoost] = pokedex.getEffectData<Chance, SpaBoost>(secondaryEffect);
    REQUIRE(chance.val == Moonblast::targetSecondaryEffect::chance);
    REQUIRE(spaBoost.val == Moonblast::targetSecondaryEffect::spaBoost);
    REQUIRE(pokedex.effectHas<move::tags::effect::MoveTarget>(secondaryEffect));
    REQUIRE_FALSE(pokedex.effectHas<move::tags::effect::MoveSource>(secondaryEffect));

    REQUIRE(pokedex.moveHas<move::tags::AnySingleTarget>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<AddedTargets>(moveEnum));
  }

  {
    INFO("Will-O-Wisp");
    using WillOWisp = dex::WillOWisp<GameMechanics::SCARLET_VIOLET>;
    dex::Move moveEnum = dex::Move::WILL_O_WISP;

    auto [move, type, accuracy, pp, primaryEffect] =
      pokedex.getMoveData<MoveName, TypeName, Accuracy, Pp, MoveEffect>(moveEnum);

    REQUIRE(move.name == WillOWisp::name);
    REQUIRE(type.name == WillOWisp::type);
    REQUIRE(pokedex.moveHas<move::tags::Status>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Physical>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Special>(moveEnum));
    REQUIRE(accuracy.val == WillOWisp::accuracy);
    REQUIRE(pp.val == WillOWisp::basePp);

    REQUIRE_FALSE(pokedex.moveHas<BasePower>(moveEnum));

    REQUIRE(primaryEffect.primary);

    REQUIRE(pokedex.effectHas<move::tags::effect::MoveTarget>(primaryEffect));
    REQUIRE(pokedex.effectHas<status::tags::Burn>(primaryEffect));
    REQUIRE_FALSE(pokedex.effectHas<move::tags::effect::MoveSource>(primaryEffect));

    REQUIRE(pokedex.moveHas<move::tags::AnySingleTarget>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<AddedTargets>(moveEnum));
  }

  {
    INFO("Quiver Dance");
    using QuiverDance = dex::QuiverDance<GameMechanics::SCARLET_VIOLET>;
    dex::Move moveEnum = dex::Move::QUIVER_DANCE;

    auto [move, type, pp, primaryEffect] = pokedex.getMoveData<MoveName, TypeName, Pp, MoveEffect>(moveEnum);

    REQUIRE(move.name == QuiverDance::name);
    REQUIRE(type.name == QuiverDance::type);
    REQUIRE(pokedex.moveHas<move::tags::Status>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Physical>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<move::tags::Special>(moveEnum));
    REQUIRE(pp.val == QuiverDance::basePp);

    REQUIRE_FALSE(pokedex.moveHas<BasePower>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<Accuracy>(moveEnum));

    REQUIRE(primaryEffect.primary);

    auto [spaBoost, spdBoost, speBoost] = pokedex.getEffectData<SpaBoost, SpdBoost, SpeBoost>(primaryEffect);
    REQUIRE(spaBoost.val == QuiverDance::sourcePrimaryEffect::spaBoost);
    REQUIRE(spdBoost.val == QuiverDance::sourcePrimaryEffect::spdBoost);
    REQUIRE(speBoost.val == QuiverDance::sourcePrimaryEffect::speBoost);

    REQUIRE(pokedex.effectHas<move::tags::effect::MoveSource>(primaryEffect));
    REQUIRE_FALSE(pokedex.effectHas<move::tags::effect::MoveTarget>(primaryEffect));

    REQUIRE(pokedex.moveHas<move::tags::Self>(moveEnum));
    REQUIRE_FALSE(pokedex.moveHas<AddedTargets>(moveEnum));
  }
}
}  // namespace pokesim