# Event Handling

## Problem
When taking a CPU profile of my AI, the `Battle.runEvent`, `Battle.singleEvent`, and `Battle.findEventHandlers` functions use the most time overall are called the most, and take up the most time per call. The main culprit of the slowdown is the search of every active effect source to see if they have callbacks for the current event. Avoiding constant searching and collecting of callbacks will go a long way to improve the design.

Because of how core these functions are to any Pokemon simulator, changing it may require an entire reworking of the system. So, making sure its design works with every goal of the project to start with is imperative to prevent large rewrites.

## Solution Description
Instead of having a single function that each event calls into to find various event handlers, each event type will have its own function with its handlers called from there.

- Pokemon, Fields, Sides, etc will be marked to run various event handlers by having tags relating to the Effect Source the handler is for. Example:
```cpp
bool EmpoleonIsBurned = registry.all_of<Pokemon, Burned>(EmpoleonEntity);

bool P2SideHasSpikes = registry.all_of<Side, Spikes>(P2Entity);
bool P2SideHas2SpikeLayers = registry.get<Spikes>(P2Entity).layers == 2;
```

- An event's function will use a view/group for each handler to determine which data the handler should run on. Example for the `OnTrapPokemon` event:
```cpp
// Priority 0 handlers
for (auto [entity, active, pokemon, activeFoes] : registry.view<Active, Pokemon, ActiveFoes, ArenaTrap>().each()) {
  activeFoes.each([&registry](auto foeEntity) {
    ArenaTrap::OnFoeTrapPokemon(foeEntity, registry);
  });
}

...

// Priority -10 handlers
for (auto [entity, active, pokemon] : registry.view<Active, Pokemon, ShedShell>().each()) {
  ShedShell::OnTrapPokemon(pokemon, registry);
}
```
> `activeFoes.each` is just for the example and probably won't exist

### Static Priority Handlers
A handler has a static priority if the order in which the handler calls the Pokemon it applies does not affect the outcome. This can be a handler that's called on one Pokemon at a time (onEntryHazard, onHitField, handlers for single-target moves, etc.) or an event that's called on all affected Pokemon at once (onDisableMove, onTrapPokemon, etc.). Technically, events like dynamaxing or recovering HP from Leftovers do not result in a different outcome at the end of the turn, but for now, handlers where the order only makes a "cosmetic" difference will be treated as if those differences matter.

These types of events will have a straightforward form, similar to the example above.

### Dynamic Priority Handlers
A handler has a dynamic priority if any call to it might affect multiple Pokemon at once and the order in which they are affected matters. This applies to most multi-target moves and residual effects while being far more prevalent in double battles.

- Within their event function, each dynamic priority handler will have at most 4 calls (1 call per Pokemon it can affect)
- Those 4 calls will distinguish themselves by having one of an `Order_#` tag (where "#" is replaced with 1-4) that's assigned to each active Pokemon whenever the speed order may change:
  - Speed stat/boost of any change[^1]
  - Switches
  - Tailwind, Trick Room, Rain w/ a Swift Swimmer active, etc starts or ends
    - Speed sorting happens at the end of each action[^2], so the plan is either to assign a tag to the `Battle` entity that an update is needed or use entt's event dispatcher.
- The exact implementation of this will be determined later. The only guaranteed working idea right now is to write out the 2-4 duplicates of the handler code, but if there is a way to automatically convert a handler to an order-based one through template functions without any runtime performance loss, that is the way to go to prevent tons of code duplication.

## Improvement
The largest benefit to this approach is removing the need to search, collect, and sort handlers every time they're used. That alone makes this worth it, but the added benefit of using views/groups to determine what handlers to run allows for running events across multiple scenarios at once. The alternative approaches of using a large switch statement or virtual functions would also work, but the code jumps would lead to this being less performant. 

[^1]: Maybe rules to ignore the fastest getting faster and the slowest getting slower
[^2]: Most of the time for the new generations