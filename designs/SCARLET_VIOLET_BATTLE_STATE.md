# Scarlet & Violet Battle State Data

This is a list of all the minimum data needed to keep track of a double battle's state when simulating a Scarlet & Violet battle. Each field is listed list the minimum number of bits needed to represent it and the minimum number of bytes if a byte is the minimum size allowed per field.

Because data will not be stored in an object-oriented way and much of this data will only be allocated if that battle state would use it, this does not reflect the size each battle state will take up in memory.

## Battle
| **Name**             | **Type**                  | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|----------------------|---------------------------|----------|------------------|------------|--------------|------------------------|
| **Turn**             | [Turn](#BasicTypes)       | 10       | 16               | 1          | 10           | 16                     |
| **Side**             | [Side](#Side)             | 2561     | 2568             | 2          | 5122         | 5136                   |
| **Field**            | [Field](#Field)           | 29       | 32               | 1          | 29           | 32                     |
| **Last Move**        | [Move Name](#BasicTypes)  | 10       | 16               | 1          | 10           | 16                     |
| **Last Switched In** | [Target](#BasicTypes)     | 2        | 8                | 1          | 2            | 8                      |
| **Action Queue**     |                           | ?        | ?                | Variable   | 0            | 0                      |
| **Total**            |                           |          |                  |            | 5173         | 5208                   |
| **Total Bytes**      |                           |          |                  |            | 647          | 651                    |


### Notes
- `Last Move`: Only used by Copycat

## Field
| **Name**              | **Type**                          | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|-----------------------|-----------------------------------|----------|------------------|------------|--------------|------------------------|
| **Weather**           | [Weather](#BasicTypes)            | 3        | 8                | 1          | 3            | 8                      |
| **Weather Duration**  | [8 Turn Duration](#BasicTypes)    | 4        | 8                | 1          | 4            | 8                      |
| **Terrain**           | [Terrain](#BasicTypes)            | 3        | 8                | 1          | 3            | 8                      |
| **Terrain Duration**  | [8 Turn Duration](#BasicTypes)    | 4        | 8                | 1          | 4            | 8                      |
| **Pseudo Weather**    | [Pseudo Weather](#PseudoWeather)  | 15       | 16               | 1          | 15           | 16                     |
| **Total**             |                                   |          |                  |            | 29           | 48                     |

## Pseudo Weather
| **Name**        | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|-----------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Fairy Lock**  | [2 Turn Duration](#BasicTypes)  | 2        | 8                | 1          | 2            | 8                      |
| **Gravity**     | [5 Turn Duration](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Ion Deluge**  | [1 Turn Duration](#BasicTypes)  | 1        | 8                | 1          | 1            | 8                      |
| **Magic Room**  | [5 Turn Duration](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Trick Room**  | [5 Turn Duration](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Wonder Room** | [5 Turn Duration](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Total**       |                                 |          |                  |            | 15           | 48                     |

### Notes
- `Ion Deluge`: Not supported in Scarlet & Violet yet, but if/when Zeraora gets, added it will be needed

## Side
| **Name**                | **Type**                            | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|-------------------------|-------------------------------------|----------|------------------|------------|--------------|------------------------|
| **Active Pokemon**      | [Active Pokemon](#ActivePokemon)    | 444      | 448              | 2          | 888          | 896                    |
| **Pokemon**             | [Pokemon](#Pokemon)                 | 273      | 280              | 6          | 1638         | 1680                   |
| **Team Order**          | [Team Order](#BasicTypes)           | 3        | 8                | 1          | 3            | 8                      |
| **Is Turn After Faint** | [Is Turn After Faint](#BasicTypes)  | 1        | 8                | 1          | 1            | 8                      |
| **Side Conditions**     | [Side Conditions](#SideConditions)  | 31       | 32               | 1          | 31           | 32                     |
| **Bag**                 | [Bag Item Capacity](#BasicTypes)    | 10       | 16               | Variable   | 0            | 0                      |
| **Total**               |                                     |          |                  |            | 2561         | 2624                   |

### Notes
- `Active Pokemon`: Amount is 1 for single battles

## Side Conditions
| **Name**         | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|------------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Aurora Veil**  | [8 Turn Duration](#BasicTypes)  | 4        | 8                | 1          | 4            | 8                      |
| **Light Screen** | [8 Turn Duration](#BasicTypes)  | 4        | 8                | 1          | 4            | 8                      |
| **Mist**         | [5 Turn Duration](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Quick Guard**  | [1 Turn Duration](#BasicTypes)  | 1        | 8                | 1          | 1            | 8                      |
| **Reflect**      | [8 Turn Duration](#BasicTypes)  | 4        | 8                | 1          | 4            | 8                      |
| **Safeguard**    | [5 Turn Duration](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Spikes**       | [Spikes](#BasicTypes)           | 2        | 8                | 1          | 2            | 8                      |
| **Stealth Rock** | [Boolean](#BasicTypes)          | 1        | 8                | 1          | 1            | 8                      |
| **Sticky Web**   | [Boolean](#BasicTypes)          | 1        | 8                | 1          | 1            | 8                      |
| **Tailwind**     | [5 Turn Duration](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Toxic Spikes** | [Toxic Spikes](#BasicTypes)     | 2        | 8                | 1          | 2            | 8                      |
| **Pledge**       | [Boolean](#BasicTypes)          | 1        | 8                | 3          | 3            | 24                     |
| **Total**        |                                 |          |                  |            | 31           | 32                     |

## Active Pokemon
| **Name**                         | **Type**                                     | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|----------------------------------|----------------------------------------------|----------|------------------|------------|--------------|------------------------|
| **Modified Stats**               | [Modified Stats Table](#ModifiedStatsTable)  | 80       | 80               | 1          | 80           | 80                     |
| **Boosts**                       | [Boost Table](#BoostTable)                   | 32       | 32               | 1          | 32           | 32                     |
|                                  |                                              |          |                  |            |              |                        |
| **Transformed/Illusion Species** | [Species Name](#BasicTypes)                  | 11       | 16               | 1          | 11           | 16                     |
| **Transformed Moves**            | [Move](#Move)                                | 22       | 24               | 4          | 88           | 96                     |
| **Modified Type**                | [Type Name](#BasicTypes)                     | 5        | 8                | 2          | 10           | 16                     |
| **Modified Ability**             | [Ability Name](#BasicTypes)                  | 9        | 16               | 1          | 9            | 16                     |
|                                  |                                              |          |                  |            |              |                        |
| **Ability Order**                | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Did Switch in This Turn**      | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Is Being Called Back**         | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Is Being Forced Out**          | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Is Trapped**                   | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Is Switch Queued**             | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Last Move**                    | [Move Name](#BasicTypes)                     | 10       | 16               | 1          | 10           | 16                     |
| **Last Move's Target**           | [Target](#BasicTypes)                        | 2        | 8                | 1          | 2            | 8                      |
| **Were Stats Raised**            | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Were Stats Lowered**           | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Did Last Turn's Move Fail**    | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Was Hurt This Turn**           | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Attacked By**                  | [Target](#BasicTypes)                        | 2        | 8                | 3          | 6            | 24                     |
| **Turns Active**                 | [Turns Active](#BasicTypes)                  | 4        | 8                | 1          | 4            | 8                      |
| **Has Started**                  | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
|                                  |                                              |          |                  |            |              |                        |
| **Volatiles**                    | [Volatiles](#Volatiles)                      | 178      | 184              | 1          | 178          | 184                    |
|                                  |                                              |          |                  |            |              |                        |
| **Lunar Dance**                  | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Healing Wish**                 | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Wish**                         | [Boolean](#BasicTypes)                       | 1        | 8                | 1          | 1            | 8                      |
| **Total**                        |                                              |          |                  |            | 444          | 608                    |

## Pokemon
| **Name**             | **Type**                              | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|----------------------|---------------------------------------|----------|------------------|------------|--------------|------------------------|
| **Species**          | [Species Name](#BasicTypes)           | 11       | 16               | 1          | 11           | 16                     |
| **Ability**          | [Ability Name](#BasicTypes)           | 9        | 16               | 1          | 9            | 16                     |
| **Item**             | [Item Name](#BasicTypes)              | 11       | 16               | 1          | 11           | 16                     |
| **Level**            | [Level](#BasicTypes)                  | 7        | 8                | 1          | 7            | 8                      |
| **Gender**           | [Gender Name](#BasicTypes)            | 2        | 8                | 1          | 2            | 8                      |
| **Nature**           | [Nature Name](#BasicTypes)            | 5        | 8                | 1          | 5            | 8                      |
| **Happiness**        | [Happiness](#BasicTypes)              | 8        | 8                | 1          | 8            | 8                      |
| **Experience**       | [Experience](#BasicTypes)             | 21       | 24               | 1          | 21           | 24                     |
| **Base Stats**       | [Base Stats Table](#BaseStatsTable)   | 60       | 64               | 1          | 60           | 64                     |
| **EV Table**         | [EV Table](#EVTable)                  | 48       | 48               | 1          | 48           | 48                     |
| **IV Table**         | [IV Table](#IVTable)                  | 30       | 32               | 1          | 30           | 32                     |
| **Moves**            | [Move](#Move)                         | 22       | 24               | 1          | 22           | 24                     |
| **HP**               | [HP Stat](#BasicTypes)                | 10       | 16               | 1          | 10           | 16                     |
| **Status Condition** | [Status Condition Name](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Last Item**        | [Item Name](#BasicTypes)              | 11       | 16               | 1          | 11           | 16                     |
| **Did Eat Berry**    | [Boolean](#BasicTypes)                | 1        | 8                | 1          | 1            | 8                      |
| **Type Name**        | [Type Name](#BasicTypes)              | 5        | 8                | 1          | 5            | 8                      |
| **Times Attacked**   | [Rage Fist Limit](#BasicTypes)        | 3        | 8                | 1          | 3            | 8                      |
| **Tera Type**        | [Type Name](#BasicTypes)              | 5        | 8                | 1          | 5            | 8                      |
| **Did Terastallize** | [Boolean](#BasicTypes)                | 1        | 8                | 1          | 1            | 8                      |
| **Total**            |                                       |          |                  |            | 273          | 352                    |

## Move
| **Name**  | **Type**                  | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|-----------|---------------------------|----------|------------------|------------|--------------|------------------------|
| **Move**  | [Move Name](#BasicTypes)  | 10       | 16               | 1          | 10           | 16                     |
| **PP**    | [PP](#BasicTypes)         | 6        | 8                | 2          | 12           | 16                     |
| **Total** |                           |          |                  |            | 22           | 24                     |

## Base Stats Table
| **Name**                 | **Type**                  | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|--------------------------|---------------------------|----------|------------------|------------|--------------|------------------------|
| **HP Stat**              | [HP Stat](#BasicTypes)    | 10       | 16               | 1          | 10           | 16                     |
| **Attack Stat**          | [Base Stat](#BasicTypes)  | 10       | 16               | 1          | 10           | 16                     |
| **Defense Stat**         | [Base Stat](#BasicTypes)  | 10       | 16               | 1          | 10           | 16                     |
| **Special Attack Stat**  | [Base Stat](#BasicTypes)  | 10       | 16               | 1          | 10           | 16                     |
| **Special Defense Stat** | [Base Stat](#BasicTypes)  | 10       | 16               | 1          | 10           | 16                     |
| **Speed Stat**           | [Base Stat](#BasicTypes)  | 10       | 16               | 1          | 10           | 16                     |
| **Total**                |                           |          |                  |            | 60           | 96                     |

## Modified Stats Table
| **Name**                 | **Type**                      | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|--------------------------|-------------------------------|----------|------------------|------------|--------------|------------------------|
| **HP Stat**              | [HP Stat](#BasicTypes)        | 10       | 16               | 1          | 10           | 16                     |
| **Attack Stat**          | [Modified Stat](#BasicTypes)  | 14       | 16               | 1          | 14           | 16                     |
| **Defense Stat**         | [Modified Stat](#BasicTypes)  | 14       | 16               | 1          | 14           | 16                     |
| **Special Attack Stat**  | [Modified Stat](#BasicTypes)  | 14       | 16               | 1          | 14           | 16                     |
| **Special Defense Stat** | [Modified Stat](#BasicTypes)  | 14       | 16               | 1          | 14           | 16                     |
| **Speed Stat**           | [Modified Stat](#BasicTypes)  | 14       | 16               | 1          | 14           | 16                     |
| **Total**                |                               |          |                  |            | 80           | 96                     |

## EV Table
| **Name**                 | **Type**           | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|--------------------------|--------------------|----------|------------------|------------|--------------|------------------------|
| **HP Stat**              | [EV](#BasicTypes)  | 8        | 8                | 1          | 8            | 8                      |
| **Attack Stat**          | [EV](#BasicTypes)  | 8        | 8                | 1          | 8            | 8                      |
| **Defense Stat**         | [EV](#BasicTypes)  | 8        | 8                | 1          | 8            | 8                      |
| **Special Attack Stat**  | [EV](#BasicTypes)  | 8        | 8                | 1          | 8            | 8                      |
| **Special Defense Stat** | [EV](#BasicTypes)  | 8        | 8                | 1          | 8            | 8                      |
| **Speed Stat**           | [EV](#BasicTypes)  | 8        | 8                | 1          | 8            | 8                      |
| **Total**                |                    |          |                  |            | 48           | 48                     |

## IV Table
| **Name**                 | **Type**           | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|--------------------------|--------------------|----------|------------------|------------|--------------|------------------------|
| **HP Stat**              | [IV](#BasicTypes)  | 5        | 8                | 1          | 5            | 8                      |
| **Attack Stat**          | [IV](#BasicTypes)  | 5        | 8                | 1          | 5            | 8                      |
| **Defense Stat**         | [IV](#BasicTypes)  | 5        | 8                | 1          | 5            | 8                      |
| **Special Attack Stat**  | [IV](#BasicTypes)  | 5        | 8                | 1          | 5            | 8                      |
| **Special Defense Stat** | [IV](#BasicTypes)  | 5        | 8                | 1          | 5            | 8                      |
| **Speed Stat**           | [IV](#BasicTypes)  | 5        | 8                | 1          | 5            | 8                      |
| **Total**                |                    |          |                  |            | 30           | 48                     |

## Boost Table
| **Name**            | **Type**              | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|---------------------|-----------------------|----------|------------------|------------|--------------|------------------------|
| **HP**              | [Boost](#BasicTypes)  | 4        | 8                | 1          | 4            | 8                      |
| **Attack**          | [Boost](#BasicTypes)  | 4        | 8                | 1          | 4            | 8                      |
| **Defense**         | [Boost](#BasicTypes)  | 4        | 8                | 1          | 4            | 8                      |
| **Special Attack**  | [Boost](#BasicTypes)  | 4        | 8                | 1          | 4            | 8                      |
| **Special Defense** | [Boost](#BasicTypes)  | 4        | 8                | 1          | 4            | 8                      |
| **Speed**           | [Boost](#BasicTypes)  | 4        | 8                | 1          | 4            | 8                      |
| **Accuracy**        | [Boost](#BasicTypes)  | 4        | 8                | 1          | 4            | 8                      |
| **Evasion**         | [Boost](#BasicTypes)  | 4        | 8                | 1          | 4            | 8                      |
| **Total**           |                       |          |                  |            | 32           | 64                     |

## Volatiles
| **Name**                            | **Type**                                                          | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|-------------------------------------|-------------------------------------------------------------------|----------|------------------|------------|--------------|------------------------|
| **Aqua Ring**                       | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Attract**                         | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Baneful Bunker**                  | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Charge**                          | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Confusion**                       | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Curse**                           | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Defense Curl**                    | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Destiny Bond**                    | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Electrify**                       | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Endure**                          | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Flash Fire**                      | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Fling**                           | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Focus Energy**                    | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Focus Punch**                     | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Follow Me**                       | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Gastro Acid**                     | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Grudge**                          | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Helping Hand**                    | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Imprison**                        | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Ingrain**                         | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **King's Shield**                   | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Leech Seed**                      | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Lock-On**                         | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Magic Coat**                      | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Micle Berry**                     | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Minimize**                        | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Nightmare**                       | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **No Retreat**                      | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Obstruct**                        | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Power Trick**                     | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Protect**                         | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Protosynthesis**                  | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Quark Drive**                     | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Rage Powder**                     | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Roost**                           | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Salt Cure**                       | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Silk Trap**                       | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Smack Down**                      | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Sparkling Aria**                  | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Spiky Shield**                    | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Tar Shot**                        | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Taunt**                           | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Throat Chop**                     | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Torment**                         | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Truant**                          | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Unburden**                        | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Zen Mode**                        | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
|                                     |                                                                   |          |                  |            |              |                        |
| **Flinching**                       | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Recharging**                      | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
|                                     |                                                                   |          |                  |            | 0            | 0                      |
| **Bide**                            | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Counter**                         | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Cud Chew**                        | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Fury Cutter**                     | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Mirror Coat**                     | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Move Locked**                     | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Preparing**                       | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Rollout**                         | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Shell Trap**                      | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Slow Start**                      | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
| **Uproar**                          | [Boolean](#BasicTypes)                                            | 1        | 8                | 1          | 1            | 8                      |
|                                     |                                                                   |          |                  |            |              |                        |
| **Choice Locked**                   | [Choice Locked](#ChoiceLocked)                                    | 10       | 16               | 1          | 10           | 16                     |
| **Disable**                         | [Disable/Encore/Metronome](#DisableEncoreMetronome)               | 13       | 16               | 1          | 13           | 16                     |
| **Encore**                          | [Disable/Encore/Metronome](#DisableEncoreMetronome)               | 13       | 16               | 1          | 13           | 16                     |
| **Fury Cutter**                     | [Fury Cutter/Perish Song](#FuryCutterPerishSong)                  | 3        | 8                | 1          | 3            | 8                      |
| **Glaive Rush**                     | [Glaive Rush/Laser Focus/Yawn](#GlaiveRushLaserFocusYawn)        | 2        | 8                | 1          | 2            | 8                      |
| **Laser Focus**                     | [Glaive Rush/Laser Focus/Yawn](#GlaiveRushLaserFocusYawn)        | 2        | 8                | 1          | 2            | 8                      |
| **Magnet Rise**                     | [Magnet Rise/Rollout/Slow Start](#MagnetRiseRolloutSlowStart)     | 3        | 8                | 1          | 3            | 8                      |
| **Metronome (Item)**                | [Disable/Encore/Metronome](#DisableEncoreMetronome)               | 13       | 16               | 1          | 13           | 16                     |
| **Octolock**                        | [Trapped](#Trapped)                                               | 4        | 8                | 1          | 4            | 8                      |
| **Partial Trapped**                 | [Trapped](#Trapped)                                               | 4        | 8                | 1          | 4            | 8                      |
| **Perish Song**                     | [Fury Cutter/Perish Song](#FuryCutterPerishSong)                  | 3        | 8                | 1          | 3            | 8                      |
| **Stockpile**                       | [Stockpile](#Stockpile)                                           | 6        | 8                | 1          | 6            | 8                      |
| **Substitute**                      | [Substitute Move](#Substitut)                                     | 10       | 16               | 1          | 10           | 16                     |
| **Trapped**                         | [Trapped](#Trapped)                                               | 4        | 8                | 1          | 4            | 8                      |
| **Yawn**                            | [Glaive Rush/Laser Focus/Yawn](#GlaiveRushLaserFocusYawn)        | 2        | 8                | 1          | 2            | 8                      |
|                                     |                                                                   |          |                  |            |              |                        |
| **Disjoint Move Volatiles Data**    | [Disjoint Move Volatiles Data](#DisjointMoveVolatilesData)        | 13       | 16               | 1          | 13           | 16                     |
| **Disjoint Ability Volatiles Data** | [Disjoint Ability Volatiles Data](#DisjointAbilityVolatilesData)  | 13       | 16               | 1          | 13           | 16                     |
| **Total**                           |                                                                   |          |                  |            | 178          | 672                    |

## Disjoint Move Volatiles Data
| **Name**        | **Type**                                                       | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|-----------------|----------------------------------------------------------------|----------|------------------|------------|--------------|------------------------|
| **Bide**        | [Bide](#Bide)                                                  | 13       | 16               | 1          | 13           | 16                     |
| **Counter**     | [Counter/Mirror Coat](#CounterMirrorCoat)                      | 11       | 16               | 1          | 11           | 16                     |
| **Fury Cutter** | [Fury Cutter/Perish Song](#FuryCutterPerishSong)               | 3        | 8                | 1          | 3            | 8                      |
| **Mirror Coat** | [Counter/Mirror Coat](#CounterMirrorCoat)                      | 11       | 16               | 1          | 11           | 16                     |
| **Move Locked** | [Move Locked](#MoveLocked)                                     | 12       | 16               | 1          | 12           | 16                     |
| **Preparing**   | [Preparing](#Preparing)                                        | 12       | 16               | 1          | 12           | 16                     |
| **Rollout**     | [Magnet Rise/Rollout/Slow Start](#MagnetRiseRolloutSlowStart)  | 3        | 8                | 1          | 3            | 8                      |
| **Shell Trap**  | [Boolean](#BasicTypes)                                         | 1        | 8                | 1          | 1            | 8                      |
| **Uproar**      | [Uproar](#Uproar)                                              | 3        | 8                | 1          | 3            | 8                      |
| **Max**         |                                                                |          |                  |            | 13           | 16                     |

## Disjoint Ability Volatiles Data
| **Name**       | **Type**                                                       | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|----------------|----------------------------------------------------------------|----------|------------------|------------|--------------|------------------------|
| **Cud Chew**   | [Cud Chew](#CudChew)                                           | 13       | 16               | 1          | 13           | 16                     |
| **Slow Start** | [Magnet Rise/Rollout/Slow Start](#MagnetRiseRolloutSlowStart)  | 3        | 8                | 1          | 3            | 8                      |
| **Max**        |                                                                |          |                  |            | 13           | 16                     |

## Bide
| **Name**     | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|--------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Damage**   | [Damage](#BasicTypes)           | 11       | 16               | 1          | 11           | 16                     |
| **Duration** | [3 Turn Duration](#BasicTypes)  | 2        | 8                | 1          | 2            | 8                      |
| **Total**    |                                 |          |                  |            | 13           | 24                     |

## Choice Locked
| **Name**  | **Type**                  | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|-----------|---------------------------|----------|------------------|------------|--------------|------------------------|
| **Move**  | [Move Name](#BasicTypes)  | 10       | 16               | 1          | 10           | 16                     |
| **Total** |                           |          |                  |            | 10           | 16                     |

## Counter/Mirror Coat
| **Name**   | **Type**               | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|------------|------------------------|----------|------------------|------------|--------------|------------------------|
| **Damage** | [Damage](#BasicTypes)  | 11       | 16               | 1          | 11           | 16                     |
| **Total**  |                        |          |                  |            | 11           | 16                     |

## Cud Chew
| **Name**      | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|---------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Item Name** | [Item Name](#BasicTypes)        | 11       | 16               | 1          | 11           | 16                     |
| **Duration**  | [2 Turn Duration](#BasicTypes)  | 2        | 8                | 1          | 2            | 8                      |
| **Total**     |                                 |          |                  |            | 13           | 24                     |

## Disable/Encore/Metronome
| **Name**      | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|---------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Move Name** | [Move Name](#BasicTypes)        | 10       | 16               | 1          | 10           | 16                     |
| **Duration**  | [5 Turn Duration](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Total**     |                                 |          |                  |            | 13           | 24                     |

## Fury Cutter/Perish Song
| **Name**     | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|--------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Duration** | [4 Turn Duration](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Total**    |                                 |          |                  |            | 3            | 8                      |

## Glaive Rush/Laser Focus/Yawn
| **Name**     | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|--------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Duration** | [2 Turn Duration](#BasicTypes)  | 2        | 8                | 1          | 2            | 8                      |
| **Total**    |                                 |          |                  |            | 2            | 8                      |

## Move Locked
| **Name**      | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|---------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Move Name** | [Move Name](#BasicTypes)        | 10       | 16               | 1          | 10           | 16                     |
| **Duration**  | [3 Turn Duration](#BasicTypes)  | 2        | 8                | 1          | 2            | 8                      |
| **Total**     |                                 |          |                  |            | 12           | 24                     |

## Magnet Rise/Rollout/Slow Start
| **Name**     | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|--------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Duration** | [5 Turn Duration](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Total**    |                                 |          |                  |            | 3            | 8                      |

## Stockpile
| **Name**       | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|----------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Layer**      | [Stockpile Count](#BasicTypes)  | 2        | 8                | 1          | 2            | 8                      |
| **Def Boosts** | [Stockpile Count](#BasicTypes)  | 2        | 8                | 1          | 2            | 8                      |
| **Spd Boosts** | [Stockpile Count](#BasicTypes)  | 2        | 8                | 1          | 2            | 8                      |
| **Total**      |                                 |          |                  |            | 6            | 24                     |

## Substitute
| **Name**    | **Type**                | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|-------------|-------------------------|----------|------------------|------------|--------------|------------------------|
| **HP Stat** | [HP Stat](#BasicTypes)  | 10       | 16               | 1          | 10           | 16                     |
| **Total**   |                         |          |                  |            | 10           | 16                     |

## Preparing
| **Name**     | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|--------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Move**     | [Move Name](#BasicTypes)        | 10       | 16               | 1          | 10           | 16                     |
| **Duration** | [3 Turn Duration](#BasicTypes)  | 2        | 8                | 1          | 2            | 8                      |
| **Total**    |                                 |          |                  |            | 12           | 24                     |

## Uproar
| **Name**     | **Type**                        | **Bits** | **Nearest Byte** | **Amount** | **Min Bits** | **Nearest Byte Total** |
|--------------|---------------------------------|----------|------------------|------------|--------------|------------------------|
| **Duration** | [4 Turn Duration](#BasicTypes)  | 3        | 8                | 1          | 3            | 8                      |
| **Total**    |                                 |          |                  |            | 3            | 8                      |


## Basic Types
| **Name**                  | **Category**   | **Min** | **Max** | **Total Options** | **Bits** | **Nearest Byte** |
|---------------------------|----------------|---------|---------|-------------------|----------|------------------|
| **Boost**                 | Active Pokemon | -6      | 6       | 13                | 4        | 8                |
| **Modified Stat**         | Active Pokemon | 1       | 10000   | 10000             | 14       | 16               |
| **Turns Active**          | Active Pokemon | 0       | 15      | 16                | 4        | 8                |
| **Wish**                  | Active Pokemon | 0       | 499.5   | 500.5             | 9        | 16               |
| **Bag Item Capacity**     | Bag            | 0       | 999     | 1000              | 10       | 16               |
| **Pokemon Index**         | Battle         | 0       | 12      | 13                | 4        | 8                |
| **Target**                | Battle         | 0       | 3       | 4                 | 2        | 8                |
| **Turn**                  | Battle         | 0       | 1000    | 1001              | 10       | 16               |
| **1 Turn Duration**       | Duration       | 0       | 1       | 2                 | 1        | 8                |
| **2 Turn Duration**       | Duration       | 0       | 2       | 3                 | 2        | 8                |
| **3 Turn Duration**       | Duration       | 0       | 3       | 4                 | 2        | 8                |
| **4 Turn Duration**       | Duration       | 0       | 4       | 5                 | 3        | 8                |
| **5 Turn Duration**       | Duration       | 0       | 5       | 6                 | 3        | 8                |
| **6 Turn Duration**       | Duration       | 0       | 6       | 7                 | 3        | 8                |
| **7 Turn Duration**       | Duration       | 0       | 7       | 8                 | 3        | 8                |
| **8 Turn Duration**       | Duration       | 0       | 8       | 9                 | 4        | 8                |
| **Terrain**               | Field          | 0       | 4       | 5                 | 3        | 8                |
| **Weather**               | Field          | 0       | 4       | 5                 | 3        | 8                |
| **PP**                    | Move           | 1       | 64      | 64                | 6        | 8                |
| **Ability Name**          | Name           | 0       | 297     | 298               | 9        | 16               |
| **Gender Name**           | Name           | 0       | 2       | 3                 | 2        | 8                |
| **Item Name**             | Name           | 0       | 1406    | 1407              | 11       | 16               |
| **Move Name**             | Name           | 0       | 929     | 930               | 10       | 16               |
| **Nature Name**           | Name           | 0       | 25      | 26                | 5        | 8                |
| **Species Name**          | Name           | 0       | 1412    | 1413              | 11       | 16               |
| **Stat Name**             | Name           | 0       | 5       | 6                 | 3        | 8                |
| **Status Condition Name** | Name           | 0       | 6       | 7                 | 3        | 8                |
| **Type Name**             | Name           | 0       | 17      | 18                | 5        | 8                |
| **Base Stat**             | Pokemon        | 1       | 669     | 669               | 10       | 16               |
| **EV**                    | Pokemon        | 0       | 255     | 256               | 8        | 8                |
| **Experience**            | Pokemon        | 1       | 1640000 | 1640000           | 21       | 24               |
| **Happiness**             | Pokemon        | 0       | 255     | 256               | 8        | 8                |
| **HP Stat**               | Pokemon        | 1       | 714     | 714               | 10       | 16               |
| **IV**                    | Pokemon        | 0       | 31      | 32                | 5        | 8                |
| **Level**                 | Pokemon        | 1       | 100     | 100               | 7        | 8                |
| **Spikes**                | Side           | 0       | 3       | 4                 | 2        | 8                |
| **Team Order**            | Side           | 0       | 5       | 6                 | 3        | 8                |
| **Toxic Spikes**          | Side           | 0       | 2       | 3                 | 2        | 8                |
| **Boolean**               | Type           | 0       | 1       | 2                 | 1        | 8                |
| **Damage**                | Type           | 1       | 1428    | 1428              | 11       | 16               |
| **Rage Fist Limit**       | Type           | 0       | 6       | 7                 | 3        | 8                |
| **Stockpile Count**       | Type           | 0       | 3       | 4                 | 2        | 8                |

### Notes
- `Wish`: Enough bits to store half the maximum HP stat. Is 0 when wish is inactive
- `Pokemon Index`: 12 used for none
- `Turn`: Turn 0 is team preview
- `1 Turn Duration`: Set to 0 when condition is inactive
- `2 Turn Duration`: Set to 0 when condition is inactive
- `3 Turn Duration`: Set to 0 when condition is inactive
- `4 Turn Duration`: Set to 0 when condition is inactive
- `5 Turn Duration`: Set to 0 when condition is inactive
- `6 Turn Duration`: Set to 0 when condition is inactive
- `7 Turn Duration`: Set to 0 when condition is inactive
- `8 Turn Duration`: Set to 0 when condition is inactive
- `Species Name`: This is for every species and form. Could be reduced to only Pokemon supported in the simulated game, TBD
- `Base Stat`: Max value needs to be confirmed
- `Damage`: Max HP time 2 because of counter and mirror coat. Could be set to Max HP if needed

