# Extra Optimizations Ideas
These are some ideas for optimizing the simulations further. It might not be needed to implement them all to get the speed up I’m looking for, but it’ll be good to keep support for adding these in mind while programming the initial iteration.

## Action Dependencies
After each action, check to see what values have changed in all its outcomes. If the upcoming action does not depend (depend = have different outcomes based on certain values), only one simulation of the upcoming action will run and the changes from that simulation will be applied to the previous action's outcomes.

To go one step further, if some of the upcoming action is dependent on some of the changed values in the previous action, a simulation will run of the upcoming action for each variation of those changed variables and then be applied to the corresponding actions.

Example:
 - The previous action generated 50 outcomes where some have a Pokemon's attack stat drops by a stage
 - The next action uses that same Pokemon's attack stat
 - 2 variations of the next action will be simulated: one with and one without the attack drop
 - The changes from those 1 of the 2 simulations will be applied to each of the 50 outcomes of the previous action where the simulation applied is determined based on which version of the attack stat the previous outcome has

If that next outcome generates 3 outcomes for each of the 2 simulations (6 total), then after all the changes have been applied to the previous action's outcomes, there will be 150 outcomes. Also, given 1), all the outcomes of an action have already been looped over to check for similarities, so checking what variables changed for this process will not add much compute time (if done right).
The difference between doing this method rather than running individual simulations for each of the 50 previous outcomes is running 150 simulations and running 6. Both still involve making 150 copies of the data, which may dwarf the computation time of the simulation, but it's worth a try.

FSAI already does something similar to dependency checking for when to recalculate damage roles. This will require checking what store and what variables each action function uses to compute its effect, but since going through all of them is required to rewrite them, it's a relatively small add.

## Action Consolidation
At the end of each action, find all the identical outcomes and only keep one version with its probability set to the sum of all the identical ones.

FSAI currently does this at the end of each turn's simulation, but doing this within the turn will greatly reduce the number of branches per turn.