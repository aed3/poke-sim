# Random Chance

## Problem
How random chance works must change. Instead of picking one outcome, this project requires creating multiple battles based on all outcomes and seeing them all through. This means efficiently making copies of the current battle is important as the current solution of creating and parsing JSON is slow; after the event handler, that task takes up the most time for FSAI. Showdown is also not built to pick up a simulation from any point a random event may occur, so the code to save and load previous random results for correct branching during re-simulation is messy. To this day, I'm not sure if that code fully works.

## Solution Description
> This is by no means an amazing or permanent solution. The goal here is to make a passible attempt that can be substituted with better solutions without changing the entire code base.

The `random` function will:
1. Duplicate applicable battle states
2. Assign battle states to branches
3. Group battle states by branch
4. Return the groups for custom callee use

The two modes that change how `random` works:
- All Paths Mode: Creating battle states for every* outcome
- Fixed Paths Mode: Assigning battle states to outcomes based on probability (useful for a [Monte Carlo method](https://en.wikipedia.org/wiki/Monte_Carlo_method) approach)

For how this relates to Showdown's implementation, random events will behave as if they are all calls to a version of `PRNG.sample` which takes in the probability of each item it could pick from rather than the items themselves.

### Function Ins & Outs
- Inputs
  - A list of probabilities that the resulting groups will act on. They are represented in this document as decimals, but will likely be 16-bit intergers where `% = value / 2^16`
  - A list of battle state entities encountering the random event
    - A battle should appear in this list **only once** per call to `random` as including a battle multiple times will cause incorrect calculations. This is mainly a problem for multi-target moves with random secondary effects (i.e., Rock Slide) and asserts should run in test/debug mode that ensures this rule.
- Outputs
  - A list for each probability containing the battle state entities. If an entity is in a probability's list, the state will simulate the probability's event.

Example:
```
Scald_Target_Entities
random(Scald_Target_Entities, [0.3, 0.7])
→ [ 
    { Scald targets who have receive the 30% chance event },
    { Scald targets who have receive the 70% chance event },
  ]
```

### Duplicate Applicable Battle States
- Creating duplicates will use `storage.push` as seen here - [entity_copy](https://github.com/skypjack/entt/blob/master/test/example/entity_copy.cpp) - to copy values from one entity to another.
- Some components will have entities as members which point to other components within the battle state and those members will have to be reassigned to the correct battle state
- In Fixed Paths Mode, this step is skipped, therefore this step will probably be called through a delegate function assigned when the `Battle.Simulate` function is called.

### Assign Battle States to Branches
In both modes, the battle state's probability will be multiplied by the probability of the event the state is assigned to.

#### All Paths Mode
For each original battle state, assign it or its duplicates to one of the different scenarios

#### Fixed Paths Mode
- Each battle state will have an RNG seed component (assigned randomly when created) that will be used to assign the state to one of the scenarios
- Because having game-accurate RNG code is not a focus of this project, random values will, for now, be calculated using Showdown's method. This is highly subject to change (need to investigate [mt19937](https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine/seed) more).
- For example, if given an input of `[0.4, 0.5, 0.1]` probabilities, that input would translate itself to `[0.4, 0.9, 1]` and would waterfall it's options down with a loop like `for (const auto probability : probabilities) if (randomNumber <= probability) { ... break; }`.
  - No clue if this is mathematically sound for getting an even distribution, but I'll look into it...

### Return the Groups for Custom Callee Use
- Each caller of `random` might use the resulting battle groups in a different way
- The most common case will be the functions that are using `Battle.randomChance` where one group of battles will call one function and the other group another function
  - For some of those cases (like whether a move's secondary effect happens), one of those groups may not have a function called on it at all
  - Adding a way to prevent adding battles to groups that will not be used in the upcoming function would be nice
- For more complicated situations that handle a variable number of options (most of the current calls to `Battle.sample` and speed ties), a different version of `random` will be called with the following differences:
  - There are no direct inputs, but rather a set of entities that contain components for: 
    - the original battle state entity
    - the list of probabilities
    - the output: the list of battle states that are assigned to the probability list
  - When battles are duplicated, their entities are assigned to the index of the output component which matches the probability it will be used for

## Improvement
The biggest improvement this makes is allowing for various scenarios - which branch off due to randomness - to run in parallel. This eliminates the need to keep track of previous paths a branch took and makes it far easier to guarantee that creating battle branches works in every case. Unfortunately, this does not solve the problem of recreating an entire battle state, but at least this clones the memory directly.

## Special Notes
Metronome (Move) and Psywave will only be handled when in Fixed Paths mode. Otherwise, Psywave will be set to the average damage output and Metronome will default to Splash (an equally effective move IMO).

## Relevant Functions
A list of all current event handlers and battle functions which use randomness in Showdown:
> Some uses are excluded as they're in pars of the code which will not be part of this project
### Event Handlers
#### onDamagingHit
- Parameters: `damage`, `target`, `source`, `move`
- Users:
  - Cursed Body
    - Random Function: `randomChance`
  - Cute Charm
    - Random Function: `randomChance`
  - Effect Spore
    - Random Function: `random`
  - Flame Body
    - Random Function: `randomChance`
  - Poison Point
    - Random Function: `randomChance`
  - Static
    - Random Function: `randomChance`
#### onStart
- Parameters: `pokemon`
- Users:
  - Forewarn
    - Random Function: `sample`
  - slp
    - Random Function: `random`
  - confusion
    - Random Function: `random`
  - lockedmove
    - Random Function: `random`
  - twoturnmove
    - Random Function: `sample`
#### onResidual
- Parameters: `pokemon`
- Users:
  - Harvest
    - Random Function: `randomChance`
  - Healer
    - Random Function: `randomChance`
  - Moody
    - Random Function: `sample`
  - Pickup
    - Random Function: `sample`
  - Shed Skin
    - Random Function: `randomChance`
#### onFractionalPriority
- Parameters: `priority`, `pokemon`, `target`, `move`
- Users:
  - Quick Draw
    - Random Function: `randomChance`
  - Quick Claw
    - Random Function: `randomChance`
#### onUpdate
- Parameters: `pokemon`
- Users:
  - Trace
    - Random Function: `sample`
#### onBeforeMove
- Parameters: `pokemon`
- Users:
  - par
    - Random Function: `randomChance`
  - frz
    - Random Function: `randomChance`
  - confusion
    - Random Function: `randomChance`
  - Attract (condition)
    - Random Function: `randomChance`
#### durationCallback
- Parameters: `target`, `source`
- Users:
  - partiallytrapped
    - Random Function: `random`
#### onStallMove
- Parameters: `pokemon`
- Users:
  - stall
    - Random Function: `randomChance`
#### onDamage
- Parameters: `damage`, `target`, `source`, `effect`
- Users:
  - Focus Band
    - Random Function: `randomChance`
#### onEat
- Parameters: `pokemon`
- Users:
  - Starf Berry
    - Random Function: `sample`
#### onHit
- Parameters: `target`
- Users:
  - Acupressure
    - Random Function: `sample`
  - Assist
    - Random Function: `sample`
  - Conversion 2
    - Random Function: `sample`
  - G-Max Snooze
    - Random Function: `random`
  - Metronome
    - Random Function: `sample`
  - Sleep Talk
    - Random Function: `sample`
#### onHitField
- Parameters: `target`, `source`
- Users:
  - Court Change
    - Random Function: `random`
#### onAfterSubDamage
- Parameters: `damage`, `target`
- Users:
  - G-Max Snooze
    - Random Function: `random`
#### onModifyMove
- Parameters: `move`, `pokemon`
- Users:
  - Magnitude
    - Random Function: `random`
  - Present
    - Random Function: `random`
  - Shell Side Arm
    - Random Function: `random`
#### damageCallback
- Parameters: `pokemon`
- Users:
  - Psywave
    - Random Function: `random`
### Battle Functions
#### speedSort
- Random Functions
  - `prng`
#### getRandomSwitchable
- Random Functions
  - `sample`
#### getRandomTarget
- Random Functions
  - `sample`
### BattleAction Functions
#### hitStepAccuracy
- Random Functions
  - `randomChance`
#### hitStepMoveHitLoop
- Random Functions
  - `random`
  - `randomChance`
  - `sample`
#### selfDrops
- Random Functions
  - `random`
#### secondaries
- Random Functions
  - `random`
#### getDamage
- Random Functions
  - `randomChance`
#### modifyDamage
- Random Functions
  - `randomizer`
#### getConfusionDamage
- Random Functions
  - `randomizer`