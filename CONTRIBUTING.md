Yes, participation 🎉! Contributions from people who are also interested in exploring the programming potential of Pokemon are greatly welcomed and appreciated.

The project is currently in the planning stage. Planning involves deciding the best algorithms to handle the most used functions of the program ([listed here](DESIGN_SPEC.md#critical-places-for-high-performance)), determining how much of Pokemon Showdown's design can be reused in this new system, and figuring out the design of the parts we will not reuse.

# How to Get Started
1. Go to the issues tab and look for issues labeled [planning](https://github.com/aed3/poke-sim/labels/planning)
2. Find a feature/change you'd link to have input on
3. Read the [code of conduct](CODE_OF_CONDUCT.md)
4. Join the conversation!

After an implementation idea is settled on, a pull request will be made to save the design idea as a markdown [implementation document](#implementation-document) in the [designs](/designs/) folder. You're more than welcome to submit design ideas as pull requests before a design is chosen to kick-start a conversation, but please check your idea is different than previous ones submitted for the same topic. PRs should be looked at within 3-4 days, and mention me (@aed3) if you're waiting for longer than that.

# Implementation Documents
Each major part of the code will be planned out ahead of time to find an implementation that helps the project stick to its [main goals](DESIGN_SPEC.md#main-goals). These plans will be saved in the [designs](/designs/) folder and new ones can be submitted via a pull request. Documents should include: 
- What problem is this implementation solving and/or what functionality will it add
- A description of the implementation
  - It should be detailed enough for a programmer to turn this into actual, working code
  - It can include a walk-through of the code path, pseudo code, links to other code that it's based on, or anything to best describe your idea
- How this improves on the current implementation, whether that be in Pokemon Showdown or this repository

This is an example of an implementation document: [designs/Features/EventHandling.md](https://github.com/aed3/poke-sim/blob/945900d6ff8f9637909ea6161f8b155ba7c34ea1/designs/Features/EventHandling.md).

# Suggesting a New Feature
If there's a feature missing from the [design spec](DESIGN_SPEC.md) that you want to see added, create an issue and tag it "new feature" to talk about it. Most feature suggestions will be added once version 1 is done, but it's good to bring them up early so that adding support can be considered when writing code now.


Once programming begins, I will update this document to include more of my coding guidelines.
