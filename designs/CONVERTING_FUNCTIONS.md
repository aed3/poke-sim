# Converting Showdown functions to PokeSim

## To ECS
To make the most out of the ECS system from a performance standpoint, functions should be made as "flat" as possible so components only run on code they use and that code is run all at once for each component that uses it.

Example:
This is part of `Battle.nextTurn` function which contains some internal loops, if statements, and calls to other functions.
> Some of this code would not actually be replicated PokeSim as it's Showdown specific
```Typescript
for (const pokemon of side.active) {
  if (!pokemon) continue;
  pokemon.moveThisTurn = '';
  pokemon.newlySwitched = false;
  pokemon.moveLastTurnResult = pokemon.moveThisTurnResult;
  pokemon.moveThisTurnResult = undefined;

  if (this.turn !== 1) {
    pokemon.usedItemThisTurn = false;
    pokemon.statsRaisedThisTurn = false;
    pokemon.statsLoweredThisTurn = false;
    // It shouldn't be possible in a normal battle for a Pokemon to be damaged before turn 1's move selection
    // However, this could be potentially relevant in certain OMs
    pokemon.hurtThisTurn = null;
  }

  pokemon.maybeDisabled = false;
  for (const moveSlot of pokemon.moveSlots) {
    moveSlot.disabled = false;
    moveSlot.disabledSource = '';
  }
  this.runEvent('DisableMove', pokemon);
  for (const moveSlot of pokemon.moveSlots) {
    this.singleEvent('DisableMove', this.dex.getActiveMove(moveSlot.id), null, pokemon);
  }

 ...

  if (pokemon.fainted) continue;

...

  pokemon.activeTurns++;
}
```

```cpp
for (auto [entity, moveThisTurnResult] : registry.view<MoveThisTurnResult>().each()) {
  registry.emplace_or_replace<MoveLastTurnResult>(entity, moveThisTurnResult);
}

registry.clear<MoveThisTurn, MoveThisTurnResult, NewlySwitched, MaybeDisabled>();


registry.clear<UsedItemThisTurn, StatsRaisedThisTurn, StatsLoweredThisTurn, HurtThisTurn>();


registry.clear<MoveSlotDisabled>();

// Loops over all ActivePokemon
DisableMove(registry);

// Loops over move slots of ActivePokemon that have disable functions
DisableSingleMove(registry);

for (auto [entity, activeTurns] : registry.view<ActiveTurns>(entt::exclude<Fainted>).each()) {
  activeTurns++;
}
```