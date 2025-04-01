#include "CalcDamage.hpp"

#include <Battle/Helpers/IntegerModify.hpp>
#include <Battle/Pokemon/PokemonDataChecks.hpp>
#include <Components/BasePower.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CalcDamage/CriticalHit.hpp>
#include <Components/CalcDamage/DamageCalcVariables.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/HitCount.hpp>
#include <Components/Level.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/TypeNames.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/SimulationResults.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <SimulateTurn/CalcDamageSpecifics.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Simulation/SimulationOptions.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Enums/StabBoostKind.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Registry.hpp>
#include <Utilities/DebugChecks.hpp>
#include <Utilities/SelectForView.hpp>
#include <cmath>
#include <type_traits>

#include "Helpers.hpp"

namespace pokesim::calc_damage {
namespace internal {
struct DebugChecks {
#ifdef NDEBUG
  DebugChecks(const Simulation&) {}
#else
  DebugChecks(const Simulation& _simulation) : simulation(_simulation), registry(simulation.registry) { checkInputs(); }
  ~DebugChecks() { checkOutputs(); }

 private:
  const Simulation& simulation;
  const types::registry& registry;
  types::registry registryOnInput;
  entt::dense_map<types::entity, types::entity> originalToCopy;
  entt::dense_set<types::entity> specificallyChecked;

  std::size_t simulateTurnCount = 0;
  std::size_t calcDamageCount = 0;
  std::size_t analyzeEffectCount = 0;
  std::size_t entityCount = 0;

  void checkMoveInputs() {
    for (types::entity move : simulation.selectedMoveEntities()) {
      originalToCopy[move] = debug::createEntityCopy(move, registry, registryOnInput);

      assert(registry.all_of<Attacker>(move));
      assert(registry.all_of<tags::Attacker>(registry.get<Attacker>(move).val));
      assert(registry.all_of<Defenders>(move));
      assert(registry.all_of<tags::Defender>(registry.get<Defenders>(move).only()));
      assert(registry.all_of<Battle>(move));
      assert(registry.all_of<TypeName>(move));
      bool isPhysical = registry.all_of<move::tags::Physical>(move);
      bool isSpecial = registry.all_of<move::tags::Special>(move);
      bool isStatus = registry.all_of<move::tags::Status>(move);
      assert(isPhysical || isSpecial || isStatus);

      bool hasSimulateTurn = registry.all_of<pokesim::tags::SimulateTurn>(move);
      bool hasCalculateDamage = registry.all_of<pokesim::tags::CalculateDamage>(move);
      bool hasAnalyzeEffect = registry.all_of<pokesim::tags::AnalyzeEffect>(move);
      assert(hasSimulateTurn || hasCalculateDamage || hasAnalyzeEffect);
    }
  }

  std::vector<types::entity> getPokemonList(bool forAttacker) const {
    std::vector<types::entity> selectedPokemon = simulation.selectedPokemonEntities();
    auto end = std::remove_if(selectedPokemon.begin(), selectedPokemon.end(), [&](types::entity entity) {
      if (forAttacker) {
        return !this->registry.all_of<tags::Attacker>(entity);
      }

      return !this->registry.all_of<tags::Defender>(entity);
    });

    return {selectedPokemon.begin(), end};
  }

  void checkPokemonInputs(bool forAttacker) {
    const std::vector<types::entity> pokemonList = getPokemonList(forAttacker);
    for (types::entity pokemon : pokemonList) {
      originalToCopy[pokemon] = debug::createEntityCopy(pokemon, registry, registryOnInput);

      assert(registry.all_of<UsedMoves>(pokemon));
      bool needsPhy = false;
      bool needsSpc = false;
      for (types::entity move : registry.get<UsedMoves>(pokemon).val) {
        assert(registry.all_of<tags::UsedMove>(move));
        needsPhy |= registry.all_of<move::tags::Physical>(move);
        needsSpc |= registry.all_of<move::tags::Special>(move);
      }
      assert(registry.all_of<Battle>(pokemon));
      assert(registry.all_of<Side>(pokemon));
      assert(registry.all_of<PlayerSide>(registry.get<Side>(pokemon).val));
      assert(registry.all_of<Level>(pokemon));
      assert(registry.all_of<SpeciesTypes>(pokemon));

      if (forAttacker) {
        if (needsPhy) {
          assert(registry.all_of<stat::EffectiveAtk>(pokemon));
        }
        if (needsSpc) {
          assert(registry.all_of<stat::EffectiveSpa>(pokemon));
        }
      }
      else {
        assert(registry.all_of<stat::CurrentHp>(pokemon));
        if (needsPhy) {
          assert(registry.all_of<stat::EffectiveDef>(pokemon));
        }
        if (needsSpc) {
          assert(registry.all_of<stat::EffectiveSpd>(pokemon));
        }
      }

      bool hasSimulateTurn = registry.all_of<pokesim::tags::SimulateTurn>(pokemon);
      bool hasCalculateDamage = registry.all_of<pokesim::tags::CalculateDamage>(pokemon);
      bool hasAnalyzeEffect = registry.all_of<pokesim::tags::AnalyzeEffect>(pokemon);
      assert(hasSimulateTurn || hasCalculateDamage || hasAnalyzeEffect);
    }
  }

  void checkBattleInputs() {
    for (types::entity battle : simulation.selectedBattleEntities()) {
      originalToCopy[battle] = debug::createEntityCopy(battle, registry, registryOnInput);
    }
  }

  DamageRollKind getDamageRollKind(types::entity move, DamageRollOptions damageRollOptions) const {
    const Defenders& defenders = registry.get<Defenders>(move);
    const Side& side = registry.get<Side>(defenders.only());
    PlayerSideId playerSide = registry.get<PlayerSide>(side.val).val;
    switch (playerSide) {
      default: {
        ENTT_FAIL("Player side wasn't set properly.");
        break;
      }
      case PlayerSideId::P1: {
        return damageRollOptions.p1;
        break;
      }
      case PlayerSideId::P2: {
        return damageRollOptions.p2;
        break;
      }
    }
  }

  void checkCalcDamageResultOutputs(types::entity move) const {
    assert(registry.all_of<DamageRolls>(move));

    DamageRollOptions damageRollOptions;
    bool noKoChanceCalculation;
    bool calculateUpToFoeHp;
    if (registry.all_of<pokesim::tags::CalculateDamage>(move)) {
      damageRollOptions = simulation.calculateDamageOptions.damageRollOptions;
      calculateUpToFoeHp = simulation.calculateDamageOptions.calculateUpToFoeHp;
      noKoChanceCalculation = simulation.calculateDamageOptions.noKoChanceCalculation;
    }
    else if (registry.all_of<pokesim::tags::AnalyzeEffect>(move)) {
      damageRollOptions = simulation.analyzeEffectOptions.damageRollOptions;
      calculateUpToFoeHp = simulation.analyzeEffectOptions.calculateUpToFoeHp;
      noKoChanceCalculation = simulation.analyzeEffectOptions.noKoChanceCalculation;
    }
    else {
      ENTT_FAIL("Where'd the simulation type go? It was checked in the inputs.");
    }

    types::damage lastDamage = std::numeric_limits<types::damage>::max();
    const auto& [damageRolls, defenders] = registry.get<DamageRolls, Defenders>(move);
    const stat::CurrentHp& defenderHp = registry.get<stat::CurrentHp>(defenders.only());

    assert(!damageRolls.val.empty());

    for (const Damage& damageRoll : damageRolls.val) {
      assert(lastDamage >= damageRoll.val);
      assert(damageRoll.val > 0);
      if (calculateUpToFoeHp) {
        assert(damageRoll.val <= defenderHp.val);
      }

      lastDamage = damageRoll.val;
    }

    DamageRollKind damageRollKind = getDamageRollKind(move, damageRollOptions);

    ENTT_ASSERT(
      damageRollKind != DamageRollKind::NONE,
      "Cannot calculate damage without knowing what rolls to consider.");

    std::size_t idealDamageRollCount = 0;
    if (damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS)) {
      idealDamageRollCount = MechanicConstants::MAX_DAMAGE_ROLL_COUNT;
    }
    else {
      if (damageKindsMatch(damageRollKind, DamageRollKind::MAX_DAMAGE)) {
        idealDamageRollCount++;
      }

      if (damageKindsMatch(damageRollKind, DamageRollKind::AVERAGE_DAMAGE)) {
        idealDamageRollCount++;
      }

      if (damageKindsMatch(damageRollKind, DamageRollKind::MIN_DAMAGE)) {
        idealDamageRollCount++;
      }
    }

    assert(idealDamageRollCount);
    assert(damageRolls.val.size() == idealDamageRollCount);

    if (noKoChanceCalculation) {
      assert(!registry.all_of<UsesUntilKo>(move));
    }
    else if (damageKindsMatch(DamageRollKind::ALL_DAMAGE_ROLLS, getDamageRollKind(move, damageRollOptions))) {
      assert(registry.all_of<UsesUntilKo>(move));

      const UsesUntilKo& usesUntilKo = registry.get<UsesUntilKo>(move);
      assert(!usesUntilKo.val.empty());
      assert(usesUntilKo.val.front() == usesUntilKo.minHits());
      assert(usesUntilKo.val.back() == usesUntilKo.maxHits());
      assert(usesUntilKo.val.size() <= MechanicConstants::MAX_DAMAGE_ROLL_COUNT);
      assert(usesUntilKo.val.size() <= damageRolls.val.size());

      types::moveHits lastUses = 0;
      types::useUntilKoChance totalChance = 0;
      for (const auto& useUntilKo : usesUntilKo.val) {
        assert(lastUses < useUntilKo.uses);
        totalChance += useUntilKo.chance;
        lastUses = useUntilKo.uses;
      }

      assert(totalChance == 1.0F);
    }
  }

  void checkAttackHpResults(types::entity move) const {
    const auto [attackerHpRecovered, attackerHpLost] = registry.try_get<AttackerHpRecovered, AttackerHpLost>(move);
    const DamageRolls& damageRolls = registry.get<DamageRolls>(move);
    if (attackerHpRecovered != nullptr) {
      assert(attackerHpRecovered->val.size() == damageRolls.val.size());

      for (std::size_t i = 0; i < damageRolls.val.size(); i++) {
        assert(attackerHpRecovered->val[i].val <= damageRolls.val[i].val);
      }
    }

    if (attackerHpLost != nullptr) {
      assert(attackerHpLost->val.size() == damageRolls.val.size());
    }
  }

  void checkMoveOutputs() const {
    for (types::entity move : simulation.selectedMoveEntities()) {
      debug::TypesToIgnore typesToIgnore{};
      if (registry.all_of<pokesim::tags::AnalyzeEffect>(move)) {
        typesToIgnore.add<Damage>();
        assert(registry.all_of<Damage>(move));
      }

      if (registry.all_of<pokesim::tags::SimulateTurn>(move)) {
        typesToIgnore.add<Damage>();
        assert(registry.all_of<Damage>(move));
        assert(registry.get<Damage>(move).val > 0);
        assert(!registry.all_of<DamageRolls>(move));
        assert(!registry.all_of<UsesUntilKo>(move));
      }
      else {
        typesToIgnore.add<DamageRolls, UsesUntilKo>();
        checkCalcDamageResultOutputs(move);

        if (registry.all_of<pokesim::tags::CalculateDamage>(move)) {
          typesToIgnore.add<AttackerHpRecovered, AttackerHpLost>();
          checkAttackHpResults(move);
        }
      }

      types::entity originalMove = debug::findCopyParent(originalToCopy, registry, move);
      debug::areEntitiesEqual(registry, move, registryOnInput, originalToCopy.at(originalMove), typesToIgnore);
    }
  }

  void checkPokemonOutputs(bool forAttacker) const {
    const std::vector<types::entity> pokemonList = getPokemonList(forAttacker);
    for (types::entity pokemon : pokemonList) {
      types::entity originalPokemon = debug::findCopyParent(originalToCopy, registry, pokemon);
      debug::areEntitiesEqual(registry, pokemon, registryOnInput, originalToCopy.at(originalPokemon));
    }
  }

  void checkBattleOutputs() const {
    for (types::entity battle : simulation.selectedBattleEntities()) {
      debug::TypesToIgnore typesToIgnore;
      if (registry.all_of<pokesim::tags::SimulateTurn>(battle)) {
        typesToIgnore.add<Probability, RngSeed, ParentBattle>();
      }
      types::entity originalBattle = debug::findCopyParent(originalToCopy, registry, battle);
      debug::areEntitiesEqual(registry, battle, registryOnInput, originalToCopy.at(originalBattle), typesToIgnore);
    }
  }

  void checkRemainingOutputs() const {
    for (auto [original, copy] : originalToCopy) {
      if (!specificallyChecked.contains(original)) {
        debug::areEntitiesEqual(registry, original, registryOnInput, copy);
      }
    }
  }

  void checkInputs() {
    checkMoveInputs();
    checkPokemonInputs(true);
    checkPokemonInputs(false);
    checkBattleInputs();

    for (types::entity entity : registry.view<types::entity>()) {
      if (!registry.orphan(entity)) {
        entityCount++;
        if (originalToCopy.contains(entity)) {
          specificallyChecked.emplace(entity);
        }
        else {
          originalToCopy[entity] = debug::createEntityCopy(entity, registry, registryOnInput);
        }
      }
    }

    simulateTurnCount = registry.view<pokesim::tags::SimulateTurn>().size();
    calcDamageCount = registry.view<pokesim::tags::CalculateDamage>().size();
    analyzeEffectCount = registry.view<pokesim::tags::AnalyzeEffect>().size();
  }

  void checkOutputs() const {
    checkMoveOutputs();
    checkPokemonOutputs(true);
    checkPokemonOutputs(false);
    checkBattleOutputs();
    checkRemainingOutputs();

    std::size_t finalEntityCount = 0;
    for (types::entity entity : registry.view<types::entity>()) {
      if (!registry.orphan(entity)) {
        finalEntityCount++;
      }
    }

    if (!simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
      std::size_t finalSimulationTurnCount = registry.view<pokesim::tags::SimulateTurn>().size();
      assert(simulateTurnCount == finalSimulationTurnCount);
      assert(entityCount == finalEntityCount);
    }
    else if (simulateTurnCount == 0) {
      assert(entityCount == finalEntityCount);
    }

    std::size_t finalCalcDamageCount = registry.view<pokesim::tags::CalculateDamage>().size();
    assert(calcDamageCount == finalCalcDamageCount);
    std::size_t finalAnalyzeEffectCount = registry.view<pokesim::tags::AnalyzeEffect>().size();
    assert(analyzeEffectCount == finalAnalyzeEffectCount);
  }
#endif
};

void clearRunVariables(Simulation& simulation) {
  simulation.registry.clear<tags::Crit, AttackingLevel, AttackingStat, DefendingStat, DamageRollModifiers>();
  simulation.removeFromEntities<Damage, pokesim::tags::CalculateDamage>();
}

void checkForAndApplyStab(
  types::handle moveHandle, const Attacker& attacker, const TypeName& type, DamageRollModifiers& modifier) {
  const SpeciesTypes& attackerTypes = moveHandle.registry()->get<SpeciesTypes>(attacker.val);

  if (attackerTypes.has(type.name)) {
    modifier.stab = StabBoostKind::STANDARD;
  }
}

void checkForAndApplyTypeEffectiveness(
  types::handle moveHandle, const Attacker& attacker, const Defenders& defenders, const TypeName& type,
  DamageRollModifiers& modifier) {
  const SpeciesTypes& defenderTypes = moveHandle.registry()->get<SpeciesTypes>(defenders.only());

  modifier.typeEffectiveness = getAttackEffectiveness(defenderTypes, type.name);
}

void applyBurnModifier(types::registry& registry, const CurrentActionMoves& moves) {
  for (types::entity move : moves.val) {
    if (!registry.all_of<move::tags::Physical>(move) /*entt::exclude<ignores burn (i.e. Facade) tag>*/) {
      return;
    }

    registry.get<DamageRollModifiers>(move).burn = true;
  }
}

void applyCritDamageIncrease(Damage& damage) {
  damage.val = (types::damage)(damage.val * MechanicConstants::CRIT_MULTIPLIER);
}

void setDamageToAtLeastOne(Damage& damage) {
  damage.val = std::max(damage.val, (types::damage)1U);
}

void setDefendingSide(types::handle moveHandle, const Defenders& defenders) {
  types::registry& registry = *moveHandle.registry();
  PlayerSideId playerSide = registry.get<PlayerSide>(registry.get<Side>(defenders.only()).val).val;
  switch (playerSide) {
    case PlayerSideId::NONE: {
      ENTT_FAIL("Player side wasn't set properly.");
      break;
    }
    case PlayerSideId::P1: {
      moveHandle.emplace<tags::P1Defending>();
      break;
    }
    case PlayerSideId::P2: {
      moveHandle.emplace<tags::P2Defending>();
      break;
    }
  }
}

void modifyDamage(Damage& damage, const DamageRollModifiers& modifiers) {
  damage.val = fixedPointMultiply(damage.val, ((std::uint8_t)modifiers.stab) / 100.0F);

  types::eventModifier typeEffectivenessModifier = MechanicConstants::FIXED_POINT_SCALE;
  if (modifiers.typeEffectiveness < 0) {
    typeEffectivenessModifier = typeEffectivenessModifier >> -modifiers.typeEffectiveness;
  }
  else {
    typeEffectivenessModifier = typeEffectivenessModifier << modifiers.typeEffectiveness;
  }

  applyChainedModifier(damage.val, typeEffectivenessModifier);
  applyChainedModifier(damage.val, modifiers.modifyDamageEvent);

  if (modifiers.burn) {
    damage.val = fixedPointMultiply(damage.val, 0.5F);
  }

  setDamageToAtLeastOne(damage);
}

void calculateAllDamageRolls(DamageRolls& damageRolls, const Damage& damage, const DamageRollModifiers& modifier) {
  damageRolls.val.reserve(MechanicConstants::MAX_DAMAGE_ROLL_COUNT);
  for (std::uint8_t i = 0; i < MechanicConstants::MAX_DAMAGE_ROLL_COUNT; i++) {
    Damage& damageRoll = damageRolls.val.emplace_back(damage);
    applyDamageRoll(damageRoll, i);
    modifyDamage(damageRoll, modifier);
  }
}

void applyAverageDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  applyAverageDamageRoll(damage);
  modifyDamage(damage, modifier);
  damageRolls.val.emplace_back(damage);
}

void applyMinDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  applyMinDamageRoll(damage);
  modifyDamage(damage, modifier);
  damageRolls.val.emplace_back(damage);
}

void applyDamageRollModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifiers& modifier) {
  modifyDamage(damage, modifier);
  damageRolls.val.emplace_back(damage);
}

void reduceDamageRollsToDefenderHp(
  types::handle moveHandle, DamageRolls& damageRolls, Damage& damage, const DamageRollModifiers& modifier,
  const Defenders& defenders) {
  const stat::CurrentHp& defenderHp = moveHandle.registry()->get<stat::CurrentHp>(defenders.only());
  for (auto& damageRoll : damageRolls.val) {
    damageRoll.val = std::min(defenderHp.val, damageRoll.val);
  }
  damage.val = std::min(defenderHp.val, damage.val);
}

void applyDamageRollsAndModifiers(Simulation& simulation, DamageRollKind damageRollKind) {
  ENTT_ASSERT(
    damageRollKind != DamageRollKind::NONE,
    "Cannot calculate damage without knowing what rolls to consider.");
  ENTT_ASSERT(
    damageRollKind != DamageRollKind::GUARANTEED_CRIT_CHANCE,
    "Must pick a damage roll kind to go along with crits.");

  simulation.addToEntities<DamageRolls, DamageRollModifiers, pokesim::tags::SelectedForViewMove>();
  if (damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS)) {
    simulation.viewForSelectedMoves<calculateAllDamageRolls>();
  }
  else {
    if (damageKindsMatch(damageRollKind, DamageRollKind::MAX_DAMAGE)) {
      simulation.viewForSelectedMoves<applyDamageRollModifier>();
    }

    if (damageKindsMatch(damageRollKind, DamageRollKind::AVERAGE_DAMAGE)) {
      simulation.viewForSelectedMoves<applyAverageDamageRollModifier>();
    }

    if (damageKindsMatch(damageRollKind, DamageRollKind::MIN_DAMAGE)) {
      simulation.viewForSelectedMoves<applyMinDamageRollModifier>();
    }
  }
}

void assignCritChanceDivisor(types::handle moveHandle, const CritBoost& critBoost) {
  std::size_t index = std::min((std::size_t)critBoost.val, pokesim::MechanicConstants::CRIT_CHANCE_DIVISORS.size() - 1);
  moveHandle.emplace<CritChanceDivisor>(pokesim::MechanicConstants::CRIT_CHANCE_DIVISORS[index]);
}

void setSourceLevel(types::handle moveHandle, const Attacker& attacker) {
  moveHandle.emplace<AttackingLevel>(moveHandle.registry()->get<Level>(attacker.val).val);
}

template <typename Category>
void setUsedAttackStat(types::handle moveHandle, const Attacker& attacker) {
  types::stat attackingStat = 1;
  if constexpr (std::is_same_v<Category, move::tags::Physical>) {
    attackingStat = moveHandle.registry()->get<stat::EffectiveAtk>(attacker.val).val;
  }
  else {
    attackingStat = moveHandle.registry()->get<stat::EffectiveSpa>(attacker.val).val;
  }
  moveHandle.emplace<AttackingStat>(attackingStat);
}

template <typename Category>
void setUsedDefenseStat(types::handle moveHandle, const Defenders& defenders) {
  types::stat defendingStat = 1;
  if constexpr (std::is_same_v<Category, move::tags::Physical>) {
    defendingStat = moveHandle.registry()->get<stat::EffectiveDef>(defenders.only()).val;
  }
  else {
    defendingStat = moveHandle.registry()->get<stat::EffectiveSpd>(defenders.only()).val;
  }
  moveHandle.emplace<DefendingStat>(defendingStat);
}

void calculateBaseDamage(
  types::handle moveHandle, const BasePower& basePower, const AttackingLevel& level, const AttackingStat& attack,
  const DefendingStat& defense) {
  // NOLINTNEXTLINE(readability-magic-numbers)
  types::damage damage = (((2U * level.val / 5U + 2U) * basePower.val * attack.val) / defense.val) / 50U + 2;
  moveHandle.emplace<Damage>(damage);
}

void applyUsesUntilKo(types::handle moveHandle, const DamageRolls& damageRolls, const Defenders& defender) {
  const stat::CurrentHp& defenderHp = moveHandle.registry()->get<stat::CurrentHp>(defender.only());
  UsesUntilKo usesUntilKo;
  ENTT_ASSERT(
    damageRolls.val.size() == MechanicConstants::MAX_DAMAGE_ROLL_COUNT,
    "All the damage rolls are needed to calculate this correctly.");

  for (const Damage& damageRoll : damageRolls.val) {
    types::moveHits uses = (types::moveHits)std::ceil(defenderHp.val / (types::useUntilKoChance)damageRoll.val);
    if (usesUntilKo.val.empty() || usesUntilKo.val.back().uses != uses) {
      usesUntilKo.val.push_back({uses, 0.0F});
    }

    usesUntilKo.val.back().chance += (1.0 / MechanicConstants::MAX_DAMAGE_ROLL_COUNT);
  }
  moveHandle.emplace<UsesUntilKo>(usesUntilKo);
}

template <typename SimulationTag, auto ApplyDamageRollKind>
void applySideDamageRollOptions(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<SimulationTag> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  static constexpr bool isSimulateTurn = std::is_same_v<pokesim::tags::SimulateTurn, SimulationTag>;
  static constexpr bool onlyPassDamageRoll =
    std::is_same_v<std::decay_t<void(Simulation&, DamageRollKind)>, decltype(ApplyDamageRollKind)>;

  DamageRollOptions damageRollOptions;
  bool noKoChanceCalculation = false;
  bool calculateUpToFoeHp = false;
  if constexpr (isSimulateTurn) {
    damageRollOptions = simulation.simulateTurnOptions.damageRollsConsidered;
    calculateUpToFoeHp = true;
  }
  else if constexpr (std::is_same_v<pokesim::tags::CalculateDamage, SimulationTag>) {
    damageRollOptions = simulation.calculateDamageOptions.damageRollOptions;
    calculateUpToFoeHp = simulation.calculateDamageOptions.calculateUpToFoeHp;
    noKoChanceCalculation = simulation.calculateDamageOptions.noKoChanceCalculation;
  }
  else if constexpr (std::is_same_v<pokesim::tags::AnalyzeEffect, SimulationTag>) {
    damageRollOptions = simulation.analyzeEffectOptions.damageRollOptions;
    calculateUpToFoeHp = simulation.analyzeEffectOptions.calculateUpToFoeHp;
    noKoChanceCalculation = simulation.calculateDamageOptions.noKoChanceCalculation;
  }

  if (damageRollOptions.sidesMatch()) {
    if constexpr (onlyPassDamageRoll) {
      ApplyDamageRollKind(simulation, damageRollOptions.p1);
    }
    else {
      ApplyDamageRollKind(simulation, damageRollOptions.p1, calculateUpToFoeHp, noKoChanceCalculation);
    }
  }
  else {
    simulation.viewForSelectedMoves<internal::setDefendingSide>();
    pokesim::internal::SelectForCurrentActionMoveView<tags::P1Defending> p1DefendingMoves{simulation};
    if (!p1DefendingMoves.hasNoneSelected()) {
      if constexpr (onlyPassDamageRoll) {
        ApplyDamageRollKind(simulation, damageRollOptions.p1);
      }
      else {
        ApplyDamageRollKind(simulation, damageRollOptions.p1, calculateUpToFoeHp, noKoChanceCalculation);
      }
    }
    p1DefendingMoves.deselect();

    pokesim::internal::SelectForCurrentActionMoveView<tags::P2Defending> p2DefendingMoves{simulation};
    if (!p2DefendingMoves.hasNoneSelected()) {
      if constexpr (onlyPassDamageRoll) {
        ApplyDamageRollKind(simulation, damageRollOptions.p2);
      }
      else {
        ApplyDamageRollKind(simulation, damageRollOptions.p2, calculateUpToFoeHp, noKoChanceCalculation);
      }
    }

    simulation.registry.clear<tags::P1Defending, tags::P2Defending>();
  }
}
}  // namespace internal

template <typename SimulationTag>
void applyDamageRollsAndModifiers(
  Simulation& simulation, DamageRollKind damageRollKind, bool calculateUpToFoeHp, bool noKoChanceCalculation) {
  internal::applyDamageRollsAndModifiers(simulation, damageRollKind);

  if constexpr (std::is_same_v<pokesim::tags::SimulateTurn, SimulationTag>) {
    if (calculateUpToFoeHp) {
      simulation.viewForSelectedMoves<internal::reduceDamageRollsToDefenderHp>();
    }
    simulate_turn::cloneFromDamageRolls(simulation, damageRollKind);
  }
  else {
    simulation.viewForSelectedMoves<internal::modifyDamage>();
    if (calculateUpToFoeHp) {
      simulation.viewForSelectedMoves<internal::reduceDamageRollsToDefenderHp>();
    }
    if (!noKoChanceCalculation && damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS)) {
      simulation.viewForSelectedMoves<internal::applyUsesUntilKo, Tags<SimulationTag>>();
    }
  }
}

void setDamageRollModifiers(Simulation& simulation) {
  simulation.viewForSelectedMoves<internal::checkForAndApplyStab>();
  simulation.viewForSelectedMoves<internal::checkForAndApplyTypeEffectiveness>();
  simulation.viewForSelectedPokemon<
    internal::applyBurnModifier,
    Tags<status::tags::Burn, tags::Attacker> /*, entt::exclude<ability::tags::Guts> */>();
}

void applyDamageRoll(Damage& damage, types::damageRoll damageRoll) {
  damage.val = (types::damage)(damage.val * ((100U - damageRoll) / 100.0F));
}

void applyAverageDamageRoll(Damage& damage) {
  damage.val = (types::damage)(damage.val * (100U - (MechanicConstants::MAX_DAMAGE_ROLL_COUNT - 1U) / 2.0F) / 100.0F);
}

void applyMinDamageRoll(Damage& damage) {
  applyDamageRoll(damage, MechanicConstants::MAX_DAMAGE_ROLL_COUNT - 1U);
}

template <typename SimulationTag>
void setIfMoveCrits(Simulation& simulation, DamageRollKind damageRollKind) {
  if (damageKindsMatch(damageRollKind, DamageRollKind::GUARANTEED_CRIT_CHANCE)) {
    simulation.addToEntities<tags::Crit, pokesim::tags::SelectedForViewMove>();
    return;
  }

  if constexpr (std::is_same_v<SimulationTag, pokesim::tags::SimulateTurn>) {
    simulation.addToEntities<calc_damage::CritBoost, pokesim::tags::SelectedForViewMove, pokesim::tags::SimulateTurn>();
    runModifyCritBoostEvent(simulation);
    simulation.viewForSelectedMoves<internal::assignCritChanceDivisor>();
    simulation.registry.clear<CritBoost>();

    simulate_turn::setIfMoveCrits(simulation);
  }
}

void getDamage(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<> selectedMoves{simulation, entt::exclude<move::tags::Status>};
  if (selectedMoves.hasNoneSelected()) return;

  using SimulateTurn = pokesim::tags::SimulateTurn;
  using CalculateDamage = pokesim::tags::CalculateDamage;
  using AnalyzeEffect = pokesim::tags::AnalyzeEffect;

  internal::applySideDamageRollOptions<SimulateTurn, setIfMoveCrits<SimulateTurn>>(simulation);
  internal::applySideDamageRollOptions<CalculateDamage, setIfMoveCrits<CalculateDamage>>(simulation);
  internal::applySideDamageRollOptions<AnalyzeEffect, setIfMoveCrits<AnalyzeEffect>>(simulation);

  // Get base power, boosts, get atk/def stats
  runBasePowerEvent(simulation);
  simulation.viewForSelectedMoves<internal::setSourceLevel>();

  simulation.viewForSelectedMoves<internal::setUsedAttackStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<internal::setUsedAttackStat<move::tags::Special>, Tags<move::tags::Special>>();
  simulation.viewForSelectedMoves<internal::setUsedDefenseStat<move::tags::Physical>, Tags<move::tags::Physical>>();
  simulation.viewForSelectedMoves<internal::setUsedDefenseStat<move::tags::Special>, Tags<move::tags::Special>>();

  simulation.viewForSelectedMoves<internal::calculateBaseDamage>();

  simulation.viewForSelectedMoves<internal::applyCritDamageIncrease, Tags<tags::Crit>>();

  simulation.addToEntities<DamageRollModifiers, pokesim::tags::SelectedForViewMove>();
  setDamageRollModifiers(simulation);

  internal::applySideDamageRollOptions<SimulateTurn, applyDamageRollsAndModifiers<SimulateTurn>>(simulation);
  internal::applySideDamageRollOptions<CalculateDamage, applyDamageRollsAndModifiers<CalculateDamage>>(simulation);
  internal::applySideDamageRollOptions<AnalyzeEffect, applyDamageRollsAndModifiers<AnalyzeEffect>>(simulation);
}

void run(Simulation& simulation) {
  internal::DebugChecks debugChecks(simulation);

  getDamage(simulation);

  internal::clearRunVariables(simulation);
}
}  // namespace pokesim::calc_damage