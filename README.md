# PokeSim
[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-BSD-blue.svg)](LICENSE)
[![Development Stage](https://img.shields.io/badge/development%20stage-planning-orange)](CONTRIBUTING.md)

Creating a Pokemon battling and analyzing C++ library designed for speed.

## Background
For the past two years, I've worked on [AI that plays competitive Pokemon](https://www.youtube.com/watch?v=rhvj7CmTRkg) and used [Pokemon Showdown](https://github.com/smogon/Pokemon-Showdown) to run its Pokemon-related calculations.
This worked fine, but once support for double battles was added, where waiting minutes for just one turn's worth of calculations was the norm, it was clear a new approach was needed.
After some promising speed tests where a direct port of Showdown's code from TypeScript to C++ yielded a 48x speed increase, I decided to try creating not only a speed-focused Pokemon simulator of my own, but also an open-source project as a go-to library for anyone looking to do Pokemon coding projects where performance matters.

To learn more about the goals for this project and planned features, see [DESIGN_SPEC.md](DESIGN_SPEC.md).

## Contributing

The project is currently in the planning stage and is welcoming all ideas. Planning involves deciding the best algorithms to handle most used functions of the program ([listed here](DESIGN_SPEC.md#critical-places-for-high-performance)), determining how much of Pokemon Showdown design can be reused in this new system, and figuring out the design of the parts we will not reuse.

See [CONTRIBUTING.md](CONTRIBUTING.md) for more details.

## License
See [LICENSE](LICENSE)
