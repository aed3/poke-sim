# Move Primary & Secondary Effects

## Problem
The main issue with how move effects are applied is how layered the process is. Although there are only 8 effect types a move can activate, there are dozens of ways those effects are calculated. That not only makes the code more complex, but also hinders this project's ability to run as much similar code as possible. Another issue is Showdown handles effects from status moves differently than effects from attacking moves or effects added from items. This separation has other benefits (i.e. makes handling Sheer Force easier), but there exists a less intrusive way to distinguish primary from secondary effects.

## Solution Description
- All move effects, whether they be primary or secondary, will be stored the same way within a move's data along with whether it's primary or secondary. ([More details below](#overview-of-move-data-representation))
- Because move effects may have a random chance of occurring, creating the various battle states for the occurrences will happen before any effect is processed. A strategy for only making branches for one move effect per battle state at a time must be used.
- What Showdown calls `selfDrops` will be handled alongside secondary effects. Right now, I do not see this causing any out-of-order issues that change the end result of the turn, but if there are any, separating the two concepts may have to happen.
- Primary and secondary effects will be processed together. As with above, if it is an issue, they will be handled one after the other as Showdown currently does.

See the [runMoveEffects function in MoveHitSteps.cpp](../../src/Simulation/MoveHitSteps.cpp) for the implementation.

## Overview of Move Data Representation
Move data, along with item and ability data, will be stored in a separate registry from battle data. This registry will be initialized before a battle simulation begins with either any amount of IMA data the user wants and will not be allowed to change by the simulation. Each IMA will be assigned to an entity that contains various properties of it. Each move effect will be stored as its own entity, and if a move has that effect, the move will contain a component that stores the entity to the effect and whether it's primary or secondary. For the 4 moves which have multiple effects (the three fang moves and Triple Arrows), a different, TBD storage method for effects will be used.
