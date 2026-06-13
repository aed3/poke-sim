#include "Tests.hpp"

namespace pokesim {
namespace {
BattleCreationInfo createBaseBattleInfo(Pokedex& pokedex) {
  BattleCreationInfo battleCreationInfo;

  PokemonCreationInfo p1aPokemonInfo{};
  p1aPokemonInfo.species = dex::Species::EMPOLEON;
  p1aPokemonInfo.ability = dex::Ability::COMPETITIVE;
  p1aPokemonInfo.gender = dex::Gender::MALE;
  p1aPokemonInfo.level = 100U;
  p1aPokemonInfo.stats = {309U, 208U, 212U, 258U, 238U, 156U};
  p1aPokemonInfo.moves.push_back(createMove(pokedex, dex::Move::FURY_ATTACK));

  PokemonCreationInfo p2aPokemonInfo{};
  p2aPokemonInfo.species = dex::Species::AMPHAROS;
  p2aPokemonInfo.ability = dex::Ability::STATIC;
  p2aPokemonInfo.gender = dex::Gender::FEMALE;
  p2aPokemonInfo.level = 100U;
  p2aPokemonInfo.stats = {321U, 186U, 206U, 266U, 216U, 146U};
  p2aPokemonInfo.moves.push_back(createMove(pokedex, dex::Move::THUNDERBOLT));

  PokemonCreationInfo p1bPokemonInfo{};
  p1bPokemonInfo.species = dex::Species::GARDEVOIR;
  p1bPokemonInfo.ability = dex::Ability::TRACE;
  p1bPokemonInfo.gender = dex::Gender::FEMALE;
  p1bPokemonInfo.level = 100U;
  p1bPokemonInfo.stats = {277U, 166U, 166U, 286U, 266U, 196U};
  p1bPokemonInfo.moves.push_back(createMove(pokedex, dex::Move::MOONBLAST));

  PokemonCreationInfo p2bPokemonInfo{};
  p2bPokemonInfo.species = dex::Species::PANGORO;
  p2bPokemonInfo.ability = dex::Ability::IRON_FIST;
  p2bPokemonInfo.gender = dex::Gender::MALE;
  p2bPokemonInfo.level = 100U;
  p2bPokemonInfo.stats = {331U, 284U, 192U, 174U, 178U, 152U};
  p2bPokemonInfo.moves.push_back(createMove(pokedex, dex::Move::KNOCK_OFF));

  PokemonCreationInfo p1cPokemonInfo{};
  p1cPokemonInfo.species = dex::Species::DRAGAPULT;
  p1cPokemonInfo.ability = dex::Ability::INFILTRATOR;
  p1cPokemonInfo.gender = dex::Gender::FEMALE;
  p1cPokemonInfo.level = 100U;
  p1cPokemonInfo.stats = {217U, 276U, 186U, 236U, 186U, 320U};
  p1cPokemonInfo.moves.push_back(createMove(pokedex, dex::Move::WILL_O_WISP));

  PokemonCreationInfo p2cPokemonInfo{};
  p2cPokemonInfo.species = dex::Species::RIBOMBEE;
  p2cPokemonInfo.ability = dex::Ability::SWEET_VEIL;
  p2cPokemonInfo.gender = dex::Gender::MALE;
  p2cPokemonInfo.level = 100U;
  p2cPokemonInfo.stats = {261U, 146U, 156U, 226U, 176U, 284U};
  p2cPokemonInfo.moves.push_back(createMove(pokedex, dex::Move::QUIVER_DANCE));

  battleCreationInfo.sides = {
    {{p1aPokemonInfo, p1bPokemonInfo, p1cPokemonInfo}},
    {{p2aPokemonInfo, p1bPokemonInfo, p2cPokemonInfo}},
  };

  entt::dense_set<dex::Species> speciesSet{};
  speciesSet.insert(p1aPokemonInfo.species);
  speciesSet.insert(p1bPokemonInfo.species);
  speciesSet.insert(p1cPokemonInfo.species);
  speciesSet.insert(p2aPokemonInfo.species);
  speciesSet.insert(p2bPokemonInfo.species);
  speciesSet.insert(p2cPokemonInfo.species);
  pokedex.loadSpecies(speciesSet);
  battleCreationInfo.turn = 1;

  return battleCreationInfo;
}
}  // namespace

TEST_CASE("Simulation Setup: Simulate Turn", "[Simulation][SimulateTurn][Setup]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  BattleCreationInfo battleInfo = createBaseBattleInfo(pokedex);
  battleInfo.runWithSimulateTurn = true;

  SideDecision p1Decision{PlayerSideId::P1};
  SideDecision p2Decision{PlayerSideId::P2};
  MoveDecision p1MoveDecision{Slot::P1A, Slot::P2A, dex::Move::FURY_ATTACK};
  MoveDecision p2MoveDecision{Slot::P2A, Slot::P1A, dex::Move::THUNDERBOLT};

  p1Decision.decisions = types::slotDecisions{p1MoveDecision};
  p2Decision.decisions = types::slotDecisions{p2MoveDecision};

  auto check = [&]() {
    Simulation simulation(pokedex, BattleFormat::SINGLES);
    simulation.createInitialStates({battleInfo});
    updateAllStats(simulation);

    types::registry& registry = simulation.registry;
    auto group = registry.group<ActionQueue, Id, Sides>();
    REQUIRE(group.size() == battleInfo.decisionsToSimulate.size());

    for (std::size_t i = 0U; i < battleInfo.decisionsToSimulate.size(); i++) {
      const auto sides = registry.get<Sides>(group[i]);
      const types::handle p1Handle = {registry, sides.val.p1()};
      const types::handle p2Handle = {registry, sides.val.p2()};

      simulate_turn::resolveDecision(p1Handle, p1Handle.get<SideDecision>());
      simulate_turn::resolveDecision(p2Handle, p2Handle.get<SideDecision>());

      const auto& queue = registry.get<ActionQueue>(group[i]).val;
      const auto id = registry.get<Id>(group[i]).val;
      REQUIRE(queue.size() == 2U);

      auto p1DecisionItem = *std::find_if(queue.begin(), queue.end(), [&](const ActionQueueItem& item) {
        return item.decision.sourceSlot() == Slot::P1A;
      });

      auto p2DecisionItem = *std::find_if(queue.begin(), queue.end(), [&](const ActionQueueItem& item) {
        return item.decision.sourceSlot() == Slot::P2A;
      });

      auto checkDecision =
        [&](ActionQueueItem actionQueueItem, const types::slotDecision& decision, const PokemonCreationInfo& pokemon) {
          if (decision.holds<MoveDecision>()) {
            const auto& trueMoveDecision = actionQueueItem.decision.get<MoveDecision>();
            const auto& idealMoveDecision = decision.get<MoveDecision>();

            REQUIRE(trueMoveDecision == idealMoveDecision);

            REQUIRE(actionQueueItem.speed == pokemon.stats.spe);
            REQUIRE(actionQueueItem.order == ActionOrder::MOVE);
            REQUIRE(actionQueueItem.priority == 0);
            REQUIRE(actionQueueItem.fractionalPriority == false);
          }
          else if (decision.holds<SwitchDecision>()) {
            const auto& trueSwitchDecision = actionQueueItem.decision.get<SwitchDecision>();
            const auto& idealSwitchDecision = decision.get<SwitchDecision>();

            REQUIRE(trueSwitchDecision == idealSwitchDecision);
            REQUIRE(actionQueueItem.speed == pokemon.stats.spe);
            REQUIRE(actionQueueItem.order == ActionOrder::SWITCH);
            REQUIRE(actionQueueItem.priority == 0);
            REQUIRE(actionQueueItem.fractionalPriority == false);
          }
        };

      {
        INFO("P1 Action");
        const auto& decision = battleInfo.decisionsToSimulate[id].p1().decisions.get<types::slotDecisions>()[0];
        checkDecision(p1DecisionItem, decision, battleInfo.sides.p1().team[0]);
      }

      {
        INFO("P2 Action");
        const auto& decision = battleInfo.decisionsToSimulate[id].p2().decisions.get<types::slotDecisions>()[0];
        checkDecision(p2DecisionItem, decision, battleInfo.sides.p2().team[0]);
      }
    }
  };

  SECTION("One Decision") {
    battleInfo.decisionsToSimulate = {{p1Decision, p2Decision}};

    check();
  }

  SECTION("Multiple Decisions") {
    battleInfo.decisionsToSimulate = {{p1Decision, p2Decision}, {p1Decision, p2Decision}};

    check();
  }

  SECTION("Multiple Different Decisions") {
    for (std::uint8_t i = 0U; i < 3U; i++) {
      for (std::uint8_t j = 0U; j < 3U; j++) {
        auto newP1Decision = p1Decision;
        auto newP2Decision = p2Decision;
        auto& newP1SlotDecision = newP1Decision.decisions.get<types::slotDecisions>()[0];
        auto& newP2SlotDecision = newP2Decision.decisions.get<types::slotDecisions>()[0];

        switch (i) {
          case 0U: {
            newP1SlotDecision = MoveDecision{p1MoveDecision.sourceSlot, Slot::P2A, p1MoveDecision.move};
            break;
          }
          case 1U: {
            newP1SlotDecision = SwitchDecision{p1MoveDecision.sourceSlot, Slot::P1B};
            break;
          }
          case 2U: {
            newP1SlotDecision = SwitchDecision{p1MoveDecision.sourceSlot, Slot::P1C};
            break;
          }
          default: break;
        }

        switch (j) {
          case 0U: {
            newP2SlotDecision = MoveDecision{p2MoveDecision.sourceSlot, Slot::P2A, p2MoveDecision.move};
            break;
          }
          case 1U: {
            newP2SlotDecision = SwitchDecision{p2MoveDecision.sourceSlot, Slot::P2B};
            break;
          }
          case 2U: {
            newP2SlotDecision = SwitchDecision{p2MoveDecision.sourceSlot, Slot::P2C};
            break;
          }
          default: break;
        }

        battleInfo.decisionsToSimulate.push_back({newP1Decision, newP2Decision});
      }
    }

    check();
  }
}

TEST_CASE("Simulation Setup: Calc Damage", "[Simulation][CalculateDamage][Setup]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  BattleCreationInfo battleInfo = createBaseBattleInfo(pokedex);

  battleInfo.runWithCalculateDamage = true;
  battleInfo.damageCalculations = {
    {
      Slot::P1A,
      Slot::P1B,
      {dex::Move::FURY_ATTACK, dex::Move::KNOCK_OFF},
    },
    {
      Slot::P1B,
      Slot::P1A,
      {dex::Move::FURY_ATTACK},
    },
    {
      Slot::P1B,
      Slot::P1A,
      {dex::Move::THUNDERBOLT},
    },
    {
      Slot::P2B,
      Slot::P2C,
      {dex::Move::KNOCK_OFF},
    },
    {
      Slot::P2B,
      Slot::P1B,
      {dex::Move::MOONBLAST},
    },
  };

  Simulation simulation(pokedex, BattleFormat::SINGLES);
  simulation.createInitialStates({battleInfo});

  const types::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 1U);

  std::size_t calcDamageTagCount = registry.view<tags::CalculateDamage>().size();
  std::size_t idealCalcDamageTagCount = 1U +  // Battle
                                        2U +  // Sides
                                        6U +  // Pokemon
                                        6U;   // Damage calculation move inputs

  REQUIRE(calcDamageTagCount == idealCalcDamageTagCount);
}

TEST_CASE("Simulation Setup: Analyze Effect", "[Simulation][AnalyzeEffect][Setup]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  BattleCreationInfo battleInfo = createBaseBattleInfo(pokedex);

  battleInfo.runWithAnalyzeEffect = true;
  battleInfo.effectsToAnalyze = {
    {Slot::P1A, Slot::P1B, Slot::P1A, {dex::Move::FURY_ATTACK, dex::Move::KNOCK_OFF}, dex::Status::BRN},
    {Slot::P1B, Slot::P1A, Slot::P1A, {dex::Move::FURY_ATTACK}, std::nullopt, {{dex::Stat::DEF, 2}}},
    {
      Slot::P1B,
      Slot::P1A,
      Slot::P1B,
      {dex::Move::THUNDERBOLT},
      std::nullopt,
      {{dex::Stat::SPA, -3}},
    },
    {Slot::P2B, Slot::P2C, Slot::P2B, {dex::Move::KNOCK_OFF}, dex::Status::BRN, {{dex::Stat::ATK, 2}}},
    {Slot::P2B, Slot::P1B, Slot::P2B, {dex::Move::MOONBLAST}, dex::Status::TOX, {{dex::Stat::SPE, 6}}},
  };

  Simulation simulation(pokedex, BattleFormat::SINGLES);
  simulation.createInitialStates({battleInfo});

  const types::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 1U);

  auto view = registry.view<analyze_effect::tags::Input>();
  REQUIRE(view.size() == 6U);

  std::size_t analyzeEffectTagCount = registry.view<tags::AnalyzeEffect>().size();
  std::size_t idealAnalyzeEffectTagCount = 1U +  // Battle
                                           2U +  // Sides
                                           6U;   // Pokemon
  REQUIRE(analyzeEffectTagCount == idealAnalyzeEffectTagCount);
}

TEST_CASE("Simulation Setup: Ignored Pokemon Options", "[Simulation][Setup]") {
  Pokedex pokedex{GameMechanics::SCARLET_VIOLET};
  std::vector<BattleCreationInfo> battleInfoList = {{}};
  PokemonCreationInfo p1AInfo{dex::Species::EMPOLEON}, p1BInfo{dex::Species::GARDEVOIR},
    p1CInfo{dex::Species::DRAGAPULT};
  PokemonCreationInfo p2AInfo{dex::Species::AMPHAROS}, p2BInfo{dex::Species::PANGORO}, p2CInfo{dex::Species::RIBOMBEE};
  Simulation simulation{pokedex, BattleFormat::SINGLES};
  const types::registry& registry = simulation.registry;

  using pokemonHandles = std::array<const types::handle, 6U>;
  auto setAndLoad = [&]() {
    p1AInfo.moves = {{dex::Move::FURY_ATTACK}};
    p1BInfo.moves = {{dex::Move::MOONBLAST}};
    p1CInfo.moves = {{dex::Move::WILL_O_WISP}};
    p2AInfo.moves = {{dex::Move::THUNDERBOLT}};
    p2BInfo.moves = {{dex::Move::KNOCK_OFF}};
    p2CInfo.moves = {{dex::Move::QUIVER_DANCE}};

    battleInfoList[0].sides.p1().team = {p1AInfo, p1BInfo, p1CInfo};
    battleInfoList[0].sides.p2().team = {p2AInfo, p2BInfo, p2CInfo};
    pokedex.loadForBattleInfo(battleInfoList);
    simulation.createInitialStates(battleInfoList);

    const Sides& sides = registry.get<Sides>(registry.view<Sides>()[0]);
    const Team& p1Team = registry.get<Team>(sides.val.p1());
    const Team& p2Team = registry.get<Team>(sides.val.p2());

    return pokemonHandles{{
      {simulation.registry, p1Team.val[0]},
      {simulation.registry, p1Team.val[1]},
      {simulation.registry, p1Team.val[2]},
      {simulation.registry, p2Team.val[0]},
      {simulation.registry, p2Team.val[1]},
      {simulation.registry, p2Team.val[2]},
    }};
  };

  auto checkForAllHandles = [&](const pokemonHandles& handles, void (*check)(const types::handle&)) {
    for (const types::handle& handle : handles) {
      INFO((int)handle.get<SpeciesName>().val);
      check(handle);
    }
  };

  auto checkDefaultLevels = [&](const pokemonHandles& handles) {
    checkForAllHandles(handles, [](const types::handle& handle) {
      REQUIRE(handle.get<Level>().val == Constants::PokemonLevel::DEFAULT);
    });
  };

  auto checkDefaultNatures = [&](const pokemonHandles& handles) {
    checkForAllHandles(handles, [](const types::handle& handle) { REQUIRE(!handle.all_of<NatureName>()); });
  };

  auto checkDefaultEvs = [&](const pokemonHandles& handles) {
    checkForAllHandles(handles, [](const types::handle& handle) {
      const Evs& evs = handle.get<Evs>();
      REQUIRE(evs.hp == Constants::PokemonEv::DEFAULT);
      REQUIRE(evs.atk == Constants::PokemonEv::DEFAULT);
      REQUIRE(evs.def == Constants::PokemonEv::DEFAULT);
      REQUIRE(evs.spa == Constants::PokemonEv::DEFAULT);
      REQUIRE(evs.spd == Constants::PokemonEv::DEFAULT);
      REQUIRE(evs.spe == Constants::PokemonEv::DEFAULT);
    });
  };

  auto checkDefaultIvs = [&](const pokemonHandles& handles) {
    checkForAllHandles(handles, [](const types::handle& handle) {
      const Ivs& ivs = handle.get<Ivs>();
      REQUIRE(ivs.hp == Constants::PokemonIv::DEFAULT);
      REQUIRE(ivs.atk == Constants::PokemonIv::DEFAULT);
      REQUIRE(ivs.def == Constants::PokemonIv::DEFAULT);
      REQUIRE(ivs.spa == Constants::PokemonIv::DEFAULT);
      REQUIRE(ivs.spd == Constants::PokemonIv::DEFAULT);
      REQUIRE(ivs.spe == Constants::PokemonIv::DEFAULT);
    });
  };

  auto checkDefaultAbilities = [&](const pokemonHandles& handles) {
    auto [p1A, p1B, p1C, p2A, p2B, p2C] = handles;
    REQUIRE(p1A.all_of<ability::tags::Torrent>());
    REQUIRE(p1B.all_of<ability::tags::Synchronize>());
    REQUIRE(p1C.all_of<ability::tags::ClearBody>());
    REQUIRE(p2A.all_of<ability::tags::Static>());
    REQUIRE(p2B.all_of<ability::tags::IronFist>());
    REQUIRE(p2C.all_of<ability::tags::HoneyGather>());
  };

  auto checkStats = [&](
                      const types::handle& handle,
                      types::stat hp,
                      types::stat atk,
                      types::stat def,
                      types::stat spa,
                      types::stat spd,
                      types::stat spe) {
    INFO((int)handle.get<SpeciesName>().val);

    REQUIRE(handle.get<stat::Hp>().val == hp);
    REQUIRE(handle.get<stat::Atk>().val == atk);
    REQUIRE(handle.get<stat::Def>().val == def);
    REQUIRE(handle.get<stat::Spa>().val == spa);
    REQUIRE(handle.get<stat::Spd>().val == spd);
    REQUIRE(handle.get<stat::Spe>().val == spe);
  };

  SECTION("All Optionals Empty") {
    auto handles = setAndLoad();
    checkDefaultLevels(handles);
    checkDefaultNatures(handles);
    checkDefaultAbilities(handles);
    checkDefaultEvs(handles);
    checkDefaultIvs(handles);

    auto [p1A, p1B, p1C, p2A, p2B, p2C] = handles;
    checkStats(p1A, 278U, 177U, 181U, 227U, 207U, 125U);
    checkStats(p1B, 246U, 135U, 135U, 255U, 235U, 165U);
    checkStats(p1C, 286U, 245U, 155U, 205U, 155U, 289U);
    checkStats(p2A, 290U, 155U, 175U, 235U, 185U, 115U);
    checkStats(p2B, 300U, 253U, 161U, 143U, 147U, 121U);
    checkStats(p2C, 230U, 115U, 125U, 195U, 145U, 253U);
  }

  SECTION("Set Level") {
    p1AInfo.level = 40U;
    p1BInfo.level = 50U;
    p1CInfo.level = 60U;
    p2AInfo.level = 70U;
    p2BInfo.level = 80U;
    p2CInfo.level = 90U;
    auto handles = setAndLoad();
    checkDefaultNatures(handles);
    checkDefaultAbilities(handles);
    checkDefaultEvs(handles);
    checkDefaultIvs(handles);

    auto [p1A, p1B, p1C, p2A, p2B, p2C] = handles;
    checkStats(p1A, 117U, 73U, 75U, 93U, 85U, 53U);
    checkStats(p1B, 128U, 70U, 70U, 130U, 120U, 85U);
    checkStats(p1C, 175U, 149U, 95U, 125U, 95U, 175U);
    checkStats(p2A, 206U, 110U, 124U, 166U, 131U, 82U);
    checkStats(p2B, 242U, 203U, 129U, 115U, 118U, 97U);
    checkStats(p2C, 208U, 104U, 113U, 176U, 131U, 228U);
  }

  SECTION("Set Nature") {
    p1AInfo.nature = dex::Nature::TIMID;
    p1BInfo.nature = dex::Nature::GENTLE;
    p1CInfo.nature = dex::Nature::LAX;
    p2AInfo.nature = dex::Nature::BRAVE;
    p2BInfo.nature = dex::Nature::MILD;
    p2CInfo.nature = dex::Nature::IMPISH;
    auto handles = setAndLoad();
    checkDefaultLevels(handles);
    checkDefaultAbilities(handles);
    checkDefaultEvs(handles);
    checkDefaultIvs(handles);

    auto [p1A, p1B, p1C, p2A, p2B, p2C] = handles;
    checkStats(p1A, 278U, 159U, 181U, 227U, 207U, 137U);
    checkStats(p1B, 246U, 135U, 121U, 255U, 258U, 165U);
    checkStats(p1C, 286U, 245U, 170U, 205U, 139U, 289U);
    checkStats(p2A, 290U, 170U, 175U, 235U, 185U, 103U);
    checkStats(p2B, 300U, 253U, 144U, 157U, 147U, 121U);
    checkStats(p2C, 230U, 115U, 137U, 175U, 145U, 253U);
  }

  SECTION("Set Evs") {
    p1AInfo.evs.hp = 1U;
    p1BInfo.evs.atk = 51U;
    p1CInfo.evs.def = 101U;
    p2AInfo.evs.spa = 151U;
    p2BInfo.evs.spd = 201U;
    p2CInfo.evs.spe = 251U;
    auto handles = setAndLoad();
    checkDefaultLevels(handles);
    checkDefaultNatures(handles);
    checkDefaultAbilities(handles);
    checkDefaultIvs(handles);

    auto [p1A, p1B, p1C, p2A, p2B, p2C] = handles;
    checkStats(p1A, 278U, 177U, 181U, 227U, 207U, 125U);
    checkStats(p1B, 246U, 147U, 135U, 255U, 235U, 165U);
    checkStats(p1C, 286U, 245U, 180U, 205U, 155U, 289U);
    checkStats(p2A, 290U, 155U, 175U, 272U, 185U, 115U);
    checkStats(p2B, 300U, 253U, 161U, 143U, 197U, 121U);
    checkStats(p2C, 230U, 115U, 125U, 195U, 145U, 315U);
  }

  SECTION("Set Ivs") {
    p1AInfo.ivs.spe = 5U;
    p1BInfo.ivs.spd = 10U;
    p1CInfo.ivs.spa = 15U;
    p2AInfo.ivs.def = 20U;
    p2BInfo.ivs.atk = 25U;
    p2CInfo.ivs.hp = 30U;
    auto handles = setAndLoad();
    checkDefaultLevels(handles);
    checkDefaultNatures(handles);
    checkDefaultAbilities(handles);
    checkDefaultEvs(handles);

    auto [p1A, p1B, p1C, p2A, p2B, p2C] = handles;
    checkStats(p1A, 278U, 177U, 181U, 227U, 207U, 130U);
    checkStats(p1B, 246U, 135U, 135U, 255U, 245U, 165U);
    checkStats(p1C, 286U, 245U, 155U, 220U, 155U, 289U);
    checkStats(p2A, 290U, 155U, 195U, 235U, 185U, 115U);
    checkStats(p2B, 300U, 278U, 161U, 143U, 147U, 121U);
    checkStats(p2C, 260U, 115U, 125U, 195U, 145U, 253U);
  }
}
}  // namespace pokesim
