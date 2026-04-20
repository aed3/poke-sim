# Main Goals
  - **Pokemon for any Application**. Provide an extensive Pokemon battling and analyzing codebase for building research projects, fan games, and more.
  - **Library for Speed.** Optimize for speed of its simulations above other performance metrics (memory usage, binary size, start-up time, etc.).
  - **Easy to understand.** Include a well-documented and intuitive API.
  - **Easy to use.** Require 0 excluded dependencies, work across most C++ compilers and supported operating systems, and provide a single header file version for simple integration into any project.

This project will be based on the great work done by the Pokemon Showdown team by porting its simulation code from TypeScript to C++. A lot of the game mechanic logic will stay the same, with the "backbone" functions rewritten to achieve high performance and support research-focused features.

## Critical Places for High Performance
  - Creating "branches" of a battle when a random chance event creates multiple possible outcomes.
  - Running the various events that items, moves, abilities, and [effects](#effect) can cause.
  - Calculating move damage.

# Version 1 Goals
## Supported Game Single & Double Battle Mechanics
  - Champions
  - Scarlet & Violet
  - Brilliant Diamond & Shining Pearl

## Main Function Definitions
These are the minimum requirements for the API. More functions, properties, options, and other details can be added later.
### Simulate Turn
#### Inputs:
- [Battle Scenario](#battle-scenario)
- A [Decision](#decision) for both players
#### Output:
List of [scenarios](#battle-scenario) that can happen after the turn ends with the percent chance of each happening.
#### Options:
- Limit damage rolls to:
  - all damage rolls.
  - average damage.
  - maximum damage.
  - minimum damage.
  - critical hits.
- Percent chance to consider an event always occurring (default: 100%).
- Percent chance to consider an event never occurring (default: 0%).
- A lower limit on returned branches' percent chance of occurring (default: 0%).
- Whether to return one or more [scenarios](#battle-scenario), chosen at random, instead of branching on random events.
#### Pseudo Code
```
Player_1_Decision = EMPOLEON, SURF
Player_2_Decision = AMPHAROS, THUNDERBOLT
Options = Only_Average_Damage, Return_Multiple_Scenarios
Simulate_Turn(Battle, Player_1_Decision, Player_2_Decision, Options)
→ [ 
    { Scenario where Surf does 40% damage then Thunderbolt does 66% damage },
    { Scenario where Surf does 40% damage then Thunderbolt does 66% damage and paralyzes },
  ]
```

### Calculate Damage
#### Inputs:
- [Battle Scenario](#battle-scenario)
- Two opposing [Pokemon](#pokemon).
- The [moves](#move) of the Pokemon to calculate.
#### Output:
- List of [Calculate Damage Results](#calculate-damage-result) for each move.
#### Pseudo Code
```
Player_1 = GARDEVOIR, CHOICE_SCARF, [PSYSHOCK, MOONBLAST]
Player_2 = PANGORO, LIFE_ORB, [DRAIN_PUNCH, KNOCK_OFF]
Calculate_Damage(Battle, Player_1, Player_2)
→ GARDEVOIR: [
    PSYSHOCK: { Max_Damage: 0 },
    MOONBLAST: { Max_Damage: 960, Min_Uses: 1 }
  ]
→ PANGORO: [
    DRAIN_PUNCH: { Max_Damage: 110, Min_Uses: 4, Recoil: 35, Recovery: 35 },
    KNOCK_OFF: { Max_Damage: 370, Min_Uses: 1, Recoil: 35 }
  ]
```
#### Options:
- Limit damage rolls to:
  - all damage rolls.
  - average damage.
  - maximum damage.
  - minimum damage.
  - critical hits.

### Analyze Effect
#### Inputs:
- [Battle Scenario](#battle-scenario)
- A [Pokemon](#pokemon)
- An [effect](#effect)
- A list of [moves](#move) for that [Pokemon](#pokemon).
- A list of foes to that [Pokemon](#pokemon).
#### Output:
- List of [Analyze Effect Results](#analyze-effect-result) for each move containing the multiplier the effect will have on that move when used on each of the foes.
#### Options:
- Whether to consider the multiplier even if the effect is already active (i.e. Rain will return a 1x multiplier instead of 1.5x multiplier for Surf if this option is true and it's already raining).
#### Pseudo Code
```
Pokemon = DRAGAPULT, [SHADOW_BALL, SUCKER_PUNCH, PSYCHIC_FANGS, DRAGON_CLAW, U_TURN]
Effect = PSYCHIC_TERRAIN
Foes = [RIBOMBEE]
Analyze_Effect(Battle, Pokemon, Effect, Foes)
→ [
    SHADOW_BALL: 1x,
    SUCKER_PUNCH: 0x,
    PSYCHIC_FANGS: 1.3x,
    DRAGON_CLAW: 1x,
    U_TURN: 1x,
  ]
```

## Main Type Descriptions
Same as the function, this is the minimum requirement for certain types.
### Battle Scenario
The information about the entire battle state that can change within one turn:
- The current field and side effect.
- The current teams of both opponents.
- Whether it represents the beginning or middle of a turn.
#### Helper Functions
- Create a branch of itself to allow multiple outcomes of a random chance event to be explored.
- Convert to and from the JSON interpretation of a Showdown battle.

### Decision
The move, switch, or item action a player will make. Move actions can contain any pre-move action also taken (terastallize, dynamax, etc). All actions can contain extra information for double battles (i.e. slot selection).

### Pokemon
The information about a Pokemon that can change within one turn. (Note: these are minimum requirements and how the rest of the information about a Pokemon will be considered.)

### Move
#### Helper Functions
- Retrieve a list of all effects the move can generate along with the percent chance of that happening.
  - If a [Pokemon](#pokemon) and [battle scenario](#battle-scenario) are provided, it can return values specific to the situation. For example, Rain Dance has no effect if it's already raining, Growl has no effect if the Pokemon had been Taunted, or Growth resulting +2 attack and special attack if it's sunny).

### Calculate Damage Result
- Maximum damage
- Minimum uses until KO (uses instead of hits to distinguish between multi-hit moves and a move used on multiple turns)
- HP recovered from landing the hit
- HP lost (recoil) from landing the hit
- Number of hits
#### Helper Functions
- For a given move, user, and target:
  - calculate all damage rolls.
  - calculate percent chance of a KO after each amount of uses.
  - damage after X uses.
    - Used for moves like Draco Meteor, Knock Off, or Burn Up.

### Effect
Any status condition, volatile, stat boost, side condition, pseudo weather, terrain, or weather.

### Analyze Effect Result
#### Helper Functions
- For a given move, user, and target:
  - calculate maximum damage with multipliers applied.
  - calculate all damage rolls with multipliers applied.
  - calculate percent chance of a KO after each amount of uses with multipliers applied.
  - damage after X uses with multipliers applied.
    - This will also take into effect how long the effect lasts.

# Version 2 Goals
## Supported Game Single & Double Battle Mechanics
- Sword & Shield
- Let's Go Eevee & Let's Go Pikachu
- Ultra Sun & Ultra Moon
- Sun & Moon
- Omega Ruby & Alpha Sapphire
- X & Y
- Black 2 & White 2
- Black & White
- Diamond, Pearl, & Platinum

## Additional Features
- Support for simulating single and double battles in remaining games.
- Keep track of battle state by receiving information in Showdown's message protocol format.
- Option for using multiple threads to explore branching scenarios.

# Eventual Goals
- Support earlier games.
- Make wrappers so other development environments can use this easily (JavaScript, Java, C#, etc).

# Non-Goals
  - **Be "Showdown".** This is a battle simulator only with zero functionality specifically for people to compete against each other or connect to the internet at all.
  - **Check Pokemon "correctness".** This is designed for uses in any context Pokemon battling may fit, so teams, team sizes, and Pokemon sets will not be checked for if they are possible/allowed in a game or competitive format. However, this project is still optimized for traditional use cases (6 Pokemon per team, 4 moves per Pokemon, and other quantity standards).
  - **Support mechanic "back-porting".** This will not explicitly support using newer mechanics in older generations. For pseudo code, if the simulator is set for in "Red, Blue, & Yellow" mode, a move like Scald might work by coincidence, but U-Turn might not. The same idea applies to terastallizing, dynamaxing, using Z-moves, mega evolving, etc: they might work, but don't count on it.
