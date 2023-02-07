# Data-Oriented Design

After learning about what data-oriented design is, it became clear that programming "style" is what this project needs. This contains my notes on the topic, resources I'm using to learn more, and how I might use what I learned in this implementation.

## Resources
- [Adventures in data-oriented design – Part 1: Mesh data (2011)](https://molecularmusings.wordpress.com/2011/11/03/adventures-in-data-oriented-design-part-1-mesh-data-3/) - Stefan Reinalter
* [Data Locality (2014)](http://gameprogrammingpatterns.com/data-locality.html) - Robert Nystrom

## Implementation
Each main function will be designed around accepting multiple sets of inputs and running through its code once to calculate all the outputs for each input. For example, the turn simulator can accept 100 different battle states and player decisions, run all the shared code paths amongst the simulations at the same time, and return the result of all 100 scenarios. A bonus for this approach with the turn simulator specifically is when a branch is created, the branch's data can be saved alongside the other scenarios when it's created, therefore preventing the need to re-run the simulation for every extra branch. This should reduce the lost performance when a scenario has a ton of possible outcomes.

Taking this approach gets the most out of running the same functions on the data for each of the branched scenarios created from random chance events. The events that fire between different branches hardly (if at all) change between different scenarios where the greatest difference between them can be as small as a different HP stat or status condition. Therefore, because the code run for each branch is mostly the same, code can run against multiple branches "simultaneously".

Other core tenants of data-oriented design (mainly storing data separately from functions in simple structures) will also be used.

### Libraries
[EnTT](https://github.com/skypjack/entt) seems like a good library to use to hold the data for the various branches/inputs. The plan is to make heavy use of tags - structs with 0 members - to indicate when a system needs to run on an entity. For example, if a Pokemon entity has a `Burned` tag, all the burn-related systems will run for that Pokemon.
