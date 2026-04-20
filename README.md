# PokeSim
[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-BSD-blue.svg)](LICENSE)
[![Development Stage](https://img.shields.io/badge/development%20stage-creating%20performance%20tests-orange)](CONTRIBUTING.md)

Creating a Pokemon battling and analyzing C++ library designed for speed.

## Background
For the past few years, I've worked on [building computers that play competitive Pokemon](https://www.youtube.com/watch?v=rhvj7CmTRkg) and used [Pokemon Showdown](https://github.com/smogon/Pokemon-Showdown) to run its Pokemon-related calculations.
This worked fine, but once support for double battles was added, where waiting minutes for just one turn's worth of calculations was the norm, it was clear a new approach was needed.
After some promising speed tests where a direct port of Showdown's code from TypeScript to C++, I decided to create a speed-focused Pokemon simulator for any Pokemon coding projects where performance matters.

To learn more about the goals for this project and planned features, see [DESIGN_SPEC.md](DESIGN_SPEC.md).

## Contributing

The project is in active development and, although the user facing API is mostly complete, the implementation is not. The first [vertical slice](https://en.wikipedia.org/wiki/Vertical_slice) of the project is done ([see this issue](https://github.com/aed3/poke-sim/issues/8)) and the performance testing pipeline is being created. Once any egregious performance problems are fixed and the second vertical slice is done, then code contributions from others will be much appreciated.

See [CONTRIBUTING.md](CONTRIBUTING.md) for more details.

## License
See [LICENSE](LICENSE)
