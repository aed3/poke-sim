# Move Primary & Secondary Effects

## Problem
The main issue with how move effects are applied is how layered the process is. Although there are only 8 effect types a move can activate, there are dozens of ways those effects are calculated which not only makes the code more complex but also hinders this project's ability to run as much similar code as possible. Another issue is Showdown handles effects from status moves differently than effects from attacking moves or effects added from items. This separation has other benefits (i.e. makes handling Sheer Force easier), but there exists a less intrusive way to distinguish primary from secondary effects.

## Solution Description
- All move effects, whether they be primary or secondary, will be stored the same way within a move's data along with whether it's primary or secondary. ([More details below](#overview-of-move-data-representation))
- Because move effects may have a random chance of occurring, creating the various battle states for the occurrences will happen before any effect is processed. A strategy for only making branches for one move effect per battle state at a time must be used.
- What Showdown calls `selfDrops` will be handled alongside secondary effects. Right now, I do not see this causing any out-of-order issues that change the end result of the turn, but if there are any, separating the two concepts may have to happen.
- Primary and secondary effects will be processed together. As with above, if it is an issue, they will be handled one after the other as Showdown currently does.

## Example Implementation of `runMoveEffect` for Status Conditions
```cpp
void runMoveEffect(entt::registry registry) {

  // Boost and heal secondaries handled here

  for (auto [entity, target, source, status] : registry.view<ActiveMoveTarget, ActiveMoveSource, Status /*ActiveMove*/>(entt::exclude<Ability /*ActiveMove*/>).each()) {
    registry.emplace<TryStatus>(target.entity, status.value);
    registry.emplace<TryStatusSource>(target.entity, source.entity);
    registry.emplace<TryStatusEffectSource>(target.entity, entity);
  }

  for (auto [entity, target, source, status, ability] : registry.view<ActiveMoveTarget, ActiveMoveSource, Status /*ActiveMove*/, Ability /*ActiveMove*/>().each()) {
    registry.emplace<TryStatus>(target.entity, status.value);
    registry.emplace<TryStatusSource>(target.entity, source.entity);
    registry.emplace<TryStatusEffectSource>(target.entity, ability.entity);
  }

  trySetStatus(registry, true /*deleteTryStatusComponents*/);
}

template<typename StatusSet>
void setEntityStatus(entt::registry registry, entt::entity) {
  registry.clear<BRN, FRZ, PAR, PSN, SLP, TOX>(entity);
  registry.emplace<StatusSet>(entity);
}

void trySetStatus(entt::registry registry, bool deleteTryStatusComponents = true) {
  for (auto [entity, tryStatus] : registry.view<TryStatus>.each()) {
    auto currentStatus = registry.try_get<Status>(entity);
    if (!currentStatus || currentStatus->value == tryStatus.value) {
      registry.remove<TryStatus>(entity);
      continue;
    }

    auto hp = registry.try_get<CurrentHP>(entity);
    if (hp && hp->value == 0) {
      registry.remove<TryStatus>(entity);
      continue;
    }

    switch (tryStatus.value) {
      case BRN: registry.emplace<TryBRN>(entity); break;
      case FRZ: registry.emplace<TryFRZ>(entity); break;
      ...
      case PSN: {
        registry.emplace<TryPSN>(entity);
        registry.emplace<TryPSN_TOX>(entity);
        break;
      }
      case TOX: {
        registry.emplace<TryTOX>(entity);
        registry.emplace<TryPSN_TOX>(entity);
        break;
      }
    }

  }

  // Forces status if corrosion is active
  for (auto [entity, tryStatusSource] : registry.view<TryStatusSource, TryPSN_TOX>.view(entt::exclude<IgnoreImmunities>).each()) {
    if (registry.all_of<Corrosion>(tryStatusSource.entity))
      registry.emplace<IgnoreImmunities>(entity);
  }

  // TryStatus is the component runStatusImmunity will use to determine the entity to look at and the component to delete if immune
  // This ignores any entities with `IgnoreImmunities` in it
  Event::runStatusImmunity<TryStatus>(registry);

  // Potentially removes any of the Try"StatusEnum"
  Event::SetStatus(registry);

  for (auto entity : registry.view<TryBRN>()) setEntityStatus<BRN>(registry, entity);
  for (auto entity : registry.view<TryFRZ>()) setEntityStatus<FRZ>(registry, entity);
  ...

  // Handle:
  //  - Random sleep duration
  //  - Toxic stage setting
  //  - Frozen form change for Sky Forme Shaymin

  for (auto [entity, tryStatusSource] : registry.view<TryStatusSource>.view()) {
    registry.emplace<StatusSource>(tryStatusSource.entity);
  }

  Event::AfterSetStatus(registry);

  if (deleteTryStatusComponents) {
    registry.clear<TryStatus, TryStatusSource, TryStatusSourceEffect, IgnoreImmunities, TryBRN, TryFRZ, ...>();
  }
}
```

## Overview of Move Data Representation
Move data, along with item and ability data, will be stored in a separate registry from battle data. This registry will be initialized before a battle simulation begins with either any amount of IMA data the user wants and will not be allowed to change by the simulation. Each IMA will be assigned to an entity that contains various properties of it. Each move effect will be stored as its own entity, and if a move has that effect, the move will contain a component that stores the entity to the effect and whether it's primary or secondary. For the 4 moves which have multiple effects (the three fang moves and Triple Arrows), a different, TBD storage method for effects will be used.