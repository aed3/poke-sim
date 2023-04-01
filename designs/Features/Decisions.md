# Making Requests to the Turn Simulator

## Problem

`Battle.makeRequests` and `Side.requestState` are how Showdown decides which actions each side is allowed to take and adds valid actions to the action queue. Because Showdown is set up for user input rather than a computer that should know whether what options are available, keeping this functionality is in question. Some questions/problems that must be resolved while porting the request code to PokeSim:
- How to support callbacks for handling mid-turn choices so the simulation can finish the turn if a Pokemon must be switched midway
- How to communicate if a given choice is valid given what the battle state is expecting
- What format/data structure is most conducive for PokeSim to receive choices

## Solution Description

### Mid-Turn Callbacks
- As part of the turn simulation constructor, a function can be passed in that's called when a mid-turn switch choice is needed
- If the mid-turn choice isn't made (either because there is no mid-turn switch function or the function doesn't make a choice), then the simulation for that battle state does not continue and is returned from the simulator at that point

### Valid Request Checking
- Similar to how Showdown handles input choices, a boolean for whether the choice was valid and registered is returned.
- These checks will only run in debug or test builds and the function will always return `true` in other

### Receiving Choices
- Two choice input methods:
  - One battle, many choices
    - The same starting battle state will be simulated with different player choices. Choices will be input in groups of two for singles or groups of four for doubles (a choice for each Pokemon slot)
    - Those choice groups will be input as a vector and the return will be a vector that contains a boolean for each choice group
    - A function that takes in a list of choices for each slot and creates all the combinations of those choices would be nice to have
  - One battle, one choice
    - Same as above, but one choice is given

#### Example
```cpp
  auto manyChoices = SimulateTurn(battleState, options);
  manyChoices.inputChoice({
    {p1Choice[0], p2Choice[0]},
    {p1Choice[0], p2Choice[1]},
    {p1Choice[1], p2Choice[0]},
    {p1Choice[1], p2Choice[1]},
  });

  // one choice
  auto oneChoice = SimulateTurn(battleState, p1Choice[0], p2Choice[0], options);
```

### Choice Format
- Choices are input into the simulation in a struct similar to [`ChoiceAction`](https://github.com/smogon/pokemon-showdown/blob/332fc773efd5bcd59dc006e0130871a1084ec3d2/sim/side.ts#L29) so any kind of choice can be input, and that choice is transformed into a proper action using a function similar to [`BattleQueue.resolveAction`](https://github.com/smogon/pokemon-showdown/blob/332fc773efd5bcd59dc006e0130871a1084ec3d2/sim/battle-queue.ts#L161)
- The values of the resolved actions will be stored as separate components in an "action entity"
- Action entities are stored in a queue that can be sorted akin to Showdown's implementation
  - If the number of entities during a simulations starts to hit the upper limit allowed per registry (~2^20), then actions could be stored in their own registry


#### Action Components
Components that the various action entities will use (based on the [Showdown's action types](https://github.com/smogon/pokemon-showdown/blob/332fc773efd5bcd59dc006e0130871a1084ec3d2/sim/battle-queue.ts#L19))
- Type Flags
  - `MoveAction`
  - `SwitchOutAction`
    - `PreTurnAction`
    - `MidTurnAction`
    - `PostFoeFaintAction`
  - `ItemAction`
  - `TeamAction`
  - `RevivalBlessingAction`
  - `SwitchInAction` (aka runSwitch)
  - `PreSwitchAction`
  - `MegaEvolveAction`
  - `PrimalAction`
  - `DynamaxAction`
  - `TerastallizeAction`
- `SpeedSort` Properties
  - `order`
  - `priority`
  - `fractionalPriority`
  - `speed`
- `TargetSlot`
- `Pokemon`
- `Move`