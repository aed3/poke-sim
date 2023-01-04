# Data Oriented Design

After learning about what data-oriented design is, it became clear that programming "style" is what this project needs. This contains my notes on the topic, resources I'm using to learn more, and how I might use what I learned in this implementation.

## Notes
This approach is being taken to get the most out of running the same functions on the data for each of the branched scenarios created from random chance events. The events that fire between different branches hardly (if at all) change between different scenarios where the greatest difference between them can be as small as a different HP stat or status condition. Therefore, the code run for each branch is mostly the same, can run against multiple branches "simultaneously" (aka in tight loops across all branches), and should only be loaded into the cache once per use case. This should reduce the lost performance when a battle scenario has a ton of possible outcomes.


## Resources
- [Adventures in data-oriented design – Part 1: Mesh data (2011)](https://molecularmusings.wordpress.com/2011/11/03/adventures-in-data-oriented-design-part-1-mesh-data-3/) - Stefan Reinalter
* [Data Locality (2014)](http://gameprogrammingpatterns.com/data-locality.html) - Robert Nystrom

## Implementation

### Libraries
[EnTT](https://github.com/skypjack/entt) seems like a good library to use to hold the data for the various branches. It has the functionality to support the "only save the branch differences" approach for scenarios exploration which requires searching up a branch's parent to find certain data. The author of the library wrote two blog posts about that exact problem found here ([Part 4 - Hierarchies](https://skypjack.github.io/2019-06-25-ecs-baf-part-4/)) and here ([Part 4, insights - Hierarchies and beyond](https://skypjack.github.io/2019-08-20-ecs-baf-part-4-insights/))
