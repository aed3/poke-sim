#pragma once

#include <cstdint>

namespace pokesim::dex {

/**
 * @brief Pokemon and Pokemon form name.
 *
 * @details Pokemon that have multiple forms will have their base form and alternate forms listed here.
 * However, if none of a Pokemon's forms are cosmetic (i.e. change nothing expect appearance), the forms cannot be
 * changed during battle, and no true base form exists, then the Pokemon's species name without a form specifier is
 * omitted. For example:
 *  - `VENUSAUR`, `MEGA_VENUSAUR`, and `GIGANTAMAX_VENUSAUR` are all listed because Venusaur changes into the other
 * forms mid-battle
 *  - `GASTRODON`, `WEST_SEA_GASTRODON`, and `EAST_SEA_GASTRODON` are all listed because although Gastrodon's forms
 * are permanent, their only difference is how they look
 *  - `PLANT_CLOAK_WORMADAM`, `SANDY_CLOAK_WORMADAM`, and `TRASH_CLOAK_WORMADAM` listed while `WORMADAM` is not
 * because the Wormadam forms have different types, stats, and moves; their forms are permanent; and there is no base
 * Wormadam.
 */
enum class Species : std::uint16_t {
  // clang-format off
  MISSING_NO = 0, BULBASAUR, IVYSAUR, VENUSAUR, MEGA_VENUSAUR, GIGANTAMAX_VENUSAUR, CHARMANDER, CHARMELEON, CHARIZARD, MEGA_CHARIZARD_X, MEGA_CHARIZARD_Y, GIGANTAMAX_CHARIZARD, SQUIRTLE, WARTORTLE, BLASTOISE, MEGA_BLASTOISE, GIGANTAMAX_BLASTOISE, CATERPIE, METAPOD, BUTTERFREE, GIGANTAMAX_BUTTERFREE, WEEDLE, KAKUNA, BEEDRILL, MEGA_BEEDRILL, PIDGEY, PIDGEOTTO, PIDGEOT, MEGA_PIDGEOT, RATTATA, ALOLAN_RATTATA, RATICATE, ALOLAN_RATICATE, TOTEM_ALOLAN_RATICATE, SPEAROW, FEAROW, EKANS, ARBOK, PIKACHU, COSPLAY_PIKACHU, PIKACHU_ROCK_STAR, PIKACHU_BELLE, PIKACHU_POP_STAR, PIKACHU_PHD, PIKACHU_LIBRE, ORIGINAL_CAP_PIKACHU, HOENN_CAP_PIKACHU, SINNOH_CAP_PIKACHU, UNOVA_CAP_PIKACHU, KALOS_CAP_PIKACHU, ALOLA_CAP_PIKACHU, PARTNER_CAP_PIKACHU, STARTER_PIKACHU, GIGANTAMAX_PIKACHU, WORLD_CAP_PIKACHU, RAICHU, ALOLAN_RAICHU, SANDSHREW, ALOLAN_SANDSHREW, SANDSLASH, ALOLAN_SANDSLASH, NIDORAN_FEMALE, NIDORINA, NIDOQUEEN, NIDORAN_MALE, NIDORINO, NIDOKING, CLEFAIRY, CLEFABLE, VULPIX, ALOLAN_VULPIX, NINETALES, ALOLAN_NINETALES, JIGGLYPUFF, WIGGLYTUFF, ZUBAT, GOLBAT, ODDISH, GLOOM, VILEPLUME, PARAS, PARASECT, VENONAT, VENOMOTH, DIGLETT, ALOLAN_DIGLETT, DUGTRIO, ALOLAN_DUGTRIO, MEOWTH, ALOLAN_MEOWTH, GALARIAN_MEOWTH, GIGANTAMAX_MEOWTH, PERSIAN, ALOLAN_PERSIAN, PSYDUCK, GOLDUCK, MANKEY, PRIMEAPE, HISUIAN_GROWLITHE, GROWLITHE, HISUIAN_ARCANINE, ARCANINE, POLIWAG, POLIWHIRL, POLIWRATH, ABRA, KADABRA, ALAKAZAM, MEGA_ALAKAZAM, MACHOP, MACHOKE, MACHAMP, GIGANTAMAX_MACHAMP, BELLSPROUT, WEEPINBELL, VICTREEBEL, TENTACOOL, TENTACRUEL, GEODUDE, ALOLAN_GEODUDE, GRAVELER, ALOLAN_GRAVELER, GOLEM, ALOLAN_GOLEM, PONYTA, GALARIAN_PONYTA, RAPIDASH, GALARIAN_RAPIDASH, SLOWPOKE, GALARIAN_SLOWPOKE, SLOWBRO, MEGA_SLOWBRO, GALARIAN_SLOWBRO, MAGNEMITE, MAGNETON, FARFETCH_D, GALARIAN_FARFETCH_D, DODUO, DODRIO, SEEL, DEWGONG, GRIMER, ALOLAN_GRIMER, MUK, ALOLAN_MUK, SHELLDER, CLOYSTER, GASTLY, HAUNTER, GENGAR, MEGA_GENGAR, GIGANTAMAX_GENGAR, ONIX, DROWZEE, HYPNO, KRABBY, KINGLER, GIGANTAMAX_KINGLER, VOLTORB, HISUIAN_VOLTORB, ELECTRODE, HISUIAN_ELECTRODE, EXEGGCUTE, EXEGGUTOR, ALOLAN_EXEGGUTOR, CUBONE, MAROWAK, ALOLAN_MAROWAK, TOTEM_ALOLAN_MAROWAK, HITMONLEE, HITMONCHAN, LICKITUNG, KOFFING, WEEZING, GALARIAN_WEEZING, RHYHORN, RHYDON, CHANSEY, TANGELA, KANGASKHAN, MEGA_KANGASKHAN, HORSEA, SEADRA, GOLDEEN, SEAKING, STARYU, STARMIE, MR_MIME, GALARIAN_MR_MIME, SCYTHER, JYNX, ELECTABUZZ, MAGMAR, PINSIR, MEGA_PINSIR, TAUROS, PALDEAN_TAUROS_COMBAT_BREAD, PALDEAN_TAUROS_BLAZE_BREAD, PALDEAN_TAUROS_AQUA_BREAD, MAGIKARP, GYARADOS, MEGA_GYARADOS, LAPRAS, GIGANTAMAX_LAPRAS, DITTO, EEVEE, STARTER_EEVEE, GIGANTAMAX_EEVEE, VAPOREON, JOLTEON, FLAREON, PORYGON, OMANYTE, OMASTAR, KABUTO, KABUTOPS, AERODACTYL, MEGA_AERODACTYL, SNORLAX, GIGANTAMAX_SNORLAX, ARTICUNO, GALARIAN_ARTICUNO, ZAPDOS, GALARIAN_ZAPDOS, MOLTRES, GALARIAN_MOLTRES, DRATINI, DRAGONAIR, DRAGONITE, MEWTWO, MEGA_MEWTWO_X, MEGA_MEWTWO_Y, MEW, CHIKORITA, BAYLEEF, MEGANIUM, CYNDAQUIL, QUILAVA, HISUIAN_TYPHLOSION, TYPHLOSION, TOTODILE, CROCONAW, FERALIGATR, SENTRET, FURRET, HOOTHOOT, NOCTOWL, LEDYBA, LEDIAN, SPINARAK, ARIADOS, CROBAT, CHINCHOU, LANTURN, PICHU, SPIKY_EARED_PICHU, CLEFFA, IGGLYBUFF, TOGEPI, TOGETIC, NATU, XATU, MAREEP, FLAAFFY, AMPHAROS, MEGA_AMPHAROS, BELLOSSOM, MARILL, AZUMARILL, SUDOWOODO, POLITOED, HOPPIP, SKIPLOOM, JUMPLUFF, AIPOM, SUNKERN, SUNFLORA, YANMA, WOOPER, PALDEAN_WOOPER, QUAGSIRE, ESPEON, UMBREON, MURKROW, SLOWKING, GALARIAN_SLOWKING, MISDREAVUS, UNOWN, UNOWN_A, UNOWN_B, UNOWN_C, UNOWN_D, UNOWN_E, UNOWN_F, UNOWN_G, UNOWN_H, UNOWN_I, UNOWN_J, UNOWN_K, UNOWN_L, UNOWN_M, UNOWN_N, UNOWN_O, UNOWN_P, UNOWN_Q, UNOWN_R, UNOWN_S, UNOWN_T, UNOWN_U, UNOWN_V, UNOWN_W, UNOWN_X, UNOWN_Y, UNOWN_Z, UNOWN_EXCLAMATION, UNOWN_QUESTION, WOBBUFFET, GIRAFARIG, PINECO, FORRETRESS, DUNSPARCE, GLIGAR, STEELIX, MEGA_STEELIX, SNUBBULL, GRANBULL, QWILFISH, HISUIAN_QWILFISH, SCIZOR, MEGA_SCIZOR, SHUCKLE, HERACROSS, MEGA_HERACROSS, HISUIAN_SNEASEL, SNEASEL, TEDDIURSA, URSARING, SLUGMA, MAGCARGO, SWINUB, PILOSWINE, CORSOLA, GALARIAN_CORSOLA, REMORAID, OCTILLERY, DELIBIRD, MANTINE, SKARMORY, HOUNDOUR, HOUNDOOM, MEGA_HOUNDOOM, KINGDRA, PHANPY, DONPHAN, PORYGON2, STANTLER, SMEARGLE, TYROGUE, HITMONTOP, SMOOCHUM, ELEKID, MAGBY, MILTANK, BLISSEY, RAIKOU, ENTEI, SUICUNE, LARVITAR, PUPITAR, TYRANITAR, MEGA_TYRANITAR, LUGIA, HO_OH, CELEBI, TREECKO, GROVYLE, SCEPTILE, MEGA_SCEPTILE, TORCHIC, COMBUSKEN, BLAZIKEN, MEGA_BLAZIKEN, MUDKIP, MARSHTOMP, SWAMPERT, MEGA_SWAMPERT, POOCHYENA, MIGHTYENA, ZIGZAGOON, GALARIAN_ZIGZAGOON, LINOONE, GALARIAN_LINOONE, WURMPLE, SILCOON, BEAUTIFLY, CASCOON, DUSTOX, LOTAD, LOMBRE, LUDICOLO, SEEDOT, NUZLEAF, SHIFTRY, TAILLOW, SWELLOW, WINGULL, PELIPPER, RALTS, KIRLIA, GARDEVOIR, MEGA_GARDEVOIR, SURSKIT, MASQUERAIN, SHROOMISH, BRELOOM, SLAKOTH, VIGOROTH, SLAKING, NINCADA, NINJASK, SHEDINJA, WHISMUR, LOUDRED, EXPLOUD, MAKUHITA, HARIYAMA, AZURILL, NOSEPASS, SKITTY, DELCATTY, SABLEYE, MEGA_SABLEYE, MAWILE, MEGA_MAWILE, ARON, LAIRON, AGGRON, MEGA_AGGRON, MEDITITE, MEDICHAM, MEGA_MEDICHAM, ELECTRIKE, MANECTRIC, MEGA_MANECTRIC, PLUSLE, MINUN, VOLBEAT, ILLUMISE, ROSELIA, GULPIN, SWALOT, CARVANHA, SHARPEDO, MEGA_SHARPEDO, WAILMER, WAILORD, NUMEL, CAMERUPT, MEGA_CAMERUPT, TORKOAL, SPOINK, GRUMPIG, SPINDA, TRAPINCH, VIBRAVA, FLYGON, CACNEA, CACTURNE, SWABLU, ALTARIA, MEGA_ALTARIA, ZANGOOSE, SEVIPER, LUNATONE, SOLROCK, BARBOACH, WHISCASH, CORPHISH, CRAWDAUNT, BALTOY, CLAYDOL, LILEEP, CRADILY, ANORITH, ARMALDO, FEEBAS, MILOTIC, CASTFORM, SUNNY_CASTFORM, RAINY_CASTFORM, SNOWY_CASTFORM, KECLEON, SHUPPET, BANETTE, MEGA_BANETTE, DUSKULL, DUSCLOPS, TROPIUS, CHIMECHO, ABSOL, MEGA_ABSOL, WYNAUT, SNORUNT, GLALIE, MEGA_GLALIE, SPHEAL, SEALEO, WALREIN, CLAMPERL, HUNTAIL, GOREBYSS, RELICANTH, LUVDISC, BAGON, SHELGON, SALAMENCE, MEGA_SALAMENCE, BELDUM, METANG, METAGROSS, MEGA_METAGROSS, REGIROCK, REGICE, REGISTEEL, LATIAS, MEGA_LATIAS, LATIOS, MEGA_LATIOS, KYOGRE, PRIMAL_KYOGRE, GROUDON, PRIMAL_GROUDON, RAYQUAZA, MEGA_RAYQUAZA, JIRACHI, DEOXYS, ATTACK_DEOXYS, DEFENSE_DEOXYS, SPEED_DEOXYS, TURTWIG, GROTLE, TORTERRA, CHIMCHAR, MONFERNO, INFERNAPE, PIPLUP, PRINPLUP, EMPOLEON, STARLY, STARAVIA, STARAPTOR, BIDOOF, BIBAREL, KRICKETOT, KRICKETUNE, SHINX, LUXIO, LUXRAY, BUDEW, ROSERADE, CRANIDOS, RAMPARDOS, SHIELDON, BASTIODON, BURMY, PLANT_CLOAK_BURMY, SANDY_CLOAK_BURMY, TRASH_CLOAK_BURMY, PLANT_CLOAK_WORMADAM, SANDY_CLOAK_WORMADAM, TRASH_CLOAK_WORMADAM, MOTHIM, COMBEE, VESPIQUEN, PACHIRISU, BUIZEL, FLOATZEL, CHERUBI, CHERRIM, CHERRIM_OVERCAST, CHERRIM_SUNSHINE, SHELLOS, WEST_SEA_SHELLOS, EAST_SEA_SHELLOS, GASTRODON, WEST_SEA_GASTRODON, EAST_SEA_GASTRODON, AMBIPOM, DRIFLOON, DRIFBLIM, BUNEARY, LOPUNNY, MEGA_LOPUNNY, MISMAGIUS, HONCHKROW, GLAMEOW, PURUGLY, CHINGLING, STUNKY, SKUNTANK, BRONZOR, BRONZONG, BONSLY, MIME_JR, HAPPINY, CHATOT, SPIRITOMB, GIBLE, GABITE, GARCHOMP, MEGA_GARCHOMP, MUNCHLAX, RIOLU, LUCARIO, MEGA_LUCARIO, HIPPOPOTAS, HIPPOWDON, SKORUPI, DRAPION, CROAGUNK, TOXICROAK, CARNIVINE, FINNEON, LUMINEON, MANTYKE, SNOVER, ABOMASNOW, MEGA_ABOMASNOW, WEAVILE, MAGNEZONE, LICKILICKY, RHYPERIOR, TANGROWTH, ELECTIVIRE, MAGMORTAR, TOGEKISS, YANMEGA, LEAFEON, GLACEON, GLISCOR, MAMOSWINE, PORYGON_Z, GALLADE, MEGA_GALLADE, PROBOPASS, DUSKNOIR, FROSLASS, ROTOM, HEAT_ROTOM, WASH_ROTOM, FROST_ROTOM, FAN_ROTOM, MOW_ROTOM, UXIE, MESPRIT, AZELF, DIALGA, DIALGA_ORIGIN, PALKIA, PALKIA_ORIGIN, HEATRAN, REGIGIGAS, GIRATINA_ALTERED, GIRATINA_ORIGIN, CRESSELIA, PHIONE, MANAPHY, DARKRAI, SHAYMIN, SHAYMIN_LAND, SHAYMIN_SKY, ARCEUS, ARCEUS_BUG, ARCEUS_DARK, ARCEUS_DRAGON, ARCEUS_ELECTRIC, ARCEUS_FAIRY, ARCEUS_FIGHTING, ARCEUS_FIRE, ARCEUS_FLYING, ARCEUS_GHOST, ARCEUS_GRASS, ARCEUS_GROUND, ARCEUS_ICE, ARCEUS_POISON, ARCEUS_PSYCHIC, ARCEUS_ROCK, ARCEUS_STEEL, ARCEUS_WATER, VICTINI, SNIVY, SERVINE, SERPERIOR, TEPIG, PIGNITE, EMBOAR, OSHAWOTT, DEWOTT, HISUIAN_SAMUROTT, SAMUROTT, PATRAT, WATCHOG, LILLIPUP, HERDIER, STOUTLAND, PURRLOIN, LIEPARD, PANSAGE, SIMISAGE, PANSEAR, SIMISEAR, PANPOUR, SIMIPOUR, MUNNA, MUSHARNA, PIDOVE, TRANQUILL, UNFEZANT, BLITZLE, ZEBSTRIKA, ROGGENROLA, BOLDORE, GIGALITH, WOOBAT, SWOOBAT, DRILBUR, EXCADRILL, AUDINO, MEGA_AUDINO, TIMBURR, GURDURR, CONKELDURR, TYMPOLE, PALPITOAD, SEISMITOAD, THROH, SAWK, SEWADDLE, SWADLOON, LEAVANNY, VENIPEDE, WHIRLIPEDE, SCOLIPEDE, COTTONEE, WHIMSICOTT, PETILIL, LILLIGANT, HISUIAN_LILLIGANT, RED_STRIPED_BASCULIN, BLUE_STRIPED_BASCULIN, WHITE_STRIPED_BASCULIN, SANDILE, KROKOROK, KROOKODILE, DARUMAKA, GALARIAN_DARUMAKA, DARMANITAN, ZEN_MODE_DARMANITAN, GALARIAN_DARMANITAN, GALARIAN_ZEN_MODE_DARMANITAN, MARACTUS, DWEBBLE, CRUSTLE, SCRAGGY, SCRAFTY, SIGILYPH, YAMASK, GALARIAN_YAMASK, COFAGRIGUS, TIRTOUGA, CARRACOSTA, ARCHEN, ARCHEOPS, TRUBBISH, GARBODOR, GIGANTAMAX_GARBODOR, ZORUA, HISUIAN_ZORUA, HISUIAN_ZOROARK, ZOROARK, MINCCINO, CINCCINO, GOTHITA, GOTHORITA, GOTHITELLE, SOLOSIS, DUOSION, REUNICLUS, DUCKLETT, SWANNA, VANILLITE, VANILLISH, VANILLUXE, DEERLING, DEERLING_SPRING, DEERLING_SUMMER, DEERLING_AUTUMN, DEERLING_WINTER, SAWSBUCK, SAWSBUCK_SPRING, SAWSBUCK_SUMMER, SAWSBUCK_AUTUMN, SAWSBUCK_WINTER, EMOLGA, KARRABLAST, ESCAVALIER, FOONGUS, AMOONGUSS, FRILLISH, JELLICENT, ALOMOMOLA, JOLTIK, GALVANTULA, FERROSEED, FERROTHORN, KLINK, KLANG, KLINKLANG, TYNAMO, EELEKTRIK, EELEKTROSS, ELGYEM, BEHEEYEM, LITWICK, LAMPENT, CHANDELURE, AXEW, FRAXURE, HAXORUS, CUBCHOO, BEARTIC, CRYOGONAL, SHELMET, ACCELGOR, STUNFISK, GALARIAN_STUNFISK, MIENFOO, MIENSHAO, DRUDDIGON, GOLETT, GOLURK, PAWNIARD, BISHARP, BOUFFALANT, RUFFLET, HISUIAN_BRAVIARY, BRAVIARY, VULLABY, MANDIBUZZ, HEATMOR, DURANT, DEINO, ZWEILOUS, HYDREIGON, LARVESTA, VOLCARONA, COBALION, TERRAKION, VIRIZION, INCARNATE_TORNADUS, TORNADUS_THERIAN, INCARNATE_THUNDURUS, THUNDURUS_THERIAN, RESHIRAM, ZEKROM, INCARNATE_LANDORUS, LANDORUS_THERIAN, KYUREM, BLACK_KYUREM, WHITE_KYUREM, KELDEO, RESOLUTE_KELDEO, ARIA_MELOETTA, PIROUETTE_MELOETTA, GENESECT, DOUSE_DRIVE_GENESECT, SHOCK_DRIVE_GENESECT, BURN_DRIVE_GENESECT, CHILL_DRIVE_GENESECT, CHESPIN, QUILLADIN, CHESNAUGHT, FENNEKIN, BRAIXEN, DELPHOX, FROAKIE, FROGADIER, GRENINJA, ASH_GRENINJA, BUNNELBY, DIGGERSBY, FLETCHLING, FLETCHINDER, TALONFLAME, SCATTERBUG, SPEWPA, VIVILLON, MEADOW_PATTERN_VIVILLON, ARCHIPELAGO_PATTERN_VIVILLON, CONTINENTAL_PATTERN_VIVILLON, ELEGANT_PATTERN_VIVILLON, GARDEN_PATTERN_VIVILLON, HIGH_PLAINS_PATTERN_VIVILLON, ICY_SNOW_PATTERN_VIVILLON, JUNGLE_PATTERN_VIVILLON, MARINE_PATTERN_VIVILLON, MODERN_PATTERN_VIVILLON, MONSOON_PATTERN_VIVILLON, OCEAN_PATTERN_VIVILLON, POLAR_PATTERN_VIVILLON, RIVER_PATTERN_VIVILLON, SANDSTORM_PATTERN_VIVILLON, SAVANNA_PATTERN_VIVILLON, SUN_PATTERN_VIVILLON, TUNDRA_PATTERN_VIVILLON, FANCY_PATTERN_VIVILLON, POKEBALL_PATTERN_VIVILLON, LITLEO, PYROAR, FLABEBE, RED_FLOWER_FLABEBE, BLUE_FLOWER_FLABEBE, ORANGE_FLOWER_FLABEBE, WHITE_FLOWER_FLABEBE, YELLOW_FLOWER_FLABEBE, FLOETTE, RED_FLOWER_FLOETTE, BLUE_FLOWER_FLOETTE, ORANGE_FLOWER_FLOETTE, WHITE_FLOWER_FLOETTE, YELLOW_FLOWER_FLOETTE, ETERNAL_FLOWER_FLOETTE, FLORGES, RED_FLOWER_FLORGES, BLUE_FLOWER_FLORGES, ORANGE_FLOWER_FLORGES, WHITE_FLOWER_FLORGES, YELLOW_FLOWER_FLORGES, SKIDDO, GOGOAT, PANCHAM, PANGORO, FURFROU, NATURAL_FURFROU, DANDY_TRIM_FURFROU, DEBUTANTE_TRIM_FURFROU, DIAMOND_TRIM_FURFROU, HEART_TRIM_FURFROU, KABUKI_TRIM_FURFROU, LA_REINE_TRIM_FURFROU, MATRON_TRIM_FURFROU, PHARAOH_TRIM_FURFROU, STAR_TRIM_FURFROU, ESPURR, MALE_MEOWSTIC, FEMALE_MEOWSTIC, HONEDGE, DOUBLADE, AEGISLASH, SHIELD_AEGISLASH, BLADE_AEGISLASH, SPRITZEE, AROMATISSE, SWIRLIX, SLURPUFF, INKAY, MALAMAR, BINACLE, BARBARACLE, SKRELP, DRAGALGE, CLAUNCHER, CLAWITZER, HELIOPTILE, HELIOLISK, TYRUNT, TYRANTRUM, AMAURA, AURORUS, SYLVEON, HAWLUCHA, DEDENNE, CARBINK, GOOMY, HISUIAN_SLIGGOO, SLIGGOO, HISUIAN_GOODRA, GOODRA, KLEFKI, PHANTUMP, TREVENANT, AVERAGE_SIZE_PUMPKABOO, SMALL_SIZE_PUMPKABOO, LARGE_SIZE_PUMPKABOO, SUPER_SIZE_PUMPKABOO, AVERAGE_SIZE_GOURGEIST, SMALL_SIZE_GOURGEIST, LARGE_SIZE_GOURGEIST, SUPER_SIZE_GOURGEIST, BERGMITE, AVALUGG, HISUIAN_AVALUGG, NOIBAT, NOIVERN, XERNEAS, YVELTAL, ZYGARDE_50, ZYGARDE_10, ZYGARDE_COMPLETE, DIANCIE, MEGA_DIANCIE, HOOPA_CONFINED, HOOPA_UNBOUND, VOLCANION, ROWLET, DARTRIX, HISUIAN_DECIDUEYE, DECIDUEYE, LITTEN, TORRACAT, INCINEROAR, POPPLIO, BRIONNE, PRIMARINA, PIKIPEK, TRUMBEAK, TOUCANNON, YUNGOOS, GUMSHOOS, TOTEM_GUMSHOOS, GRUBBIN, CHARJABUG, VIKAVOLT, TOTEM_VIKAVOLT, CRABRAWLER, CRABOMINABLE, BAILE_STYLE_ORICORIO, POM_POM_STYLE_ORICORIO, PA_U_STYLE_ORICORIO, SENSU_STYLE_ORICORIO, CUTIEFLY, RIBOMBEE, TOTEM_RIBOMBEE, ROCKRUFF, MIDDAY_ROCKRUFF, MIDDAY_LYCANROC, MIDNIGHT_LYCANROC, DUSK_LYCANROC, WISHIWASHI, SOLO_WISHIWASHI, SCHOOL_WISHIWASHI, MAREANIE, TOXAPEX, MUDBRAY, MUDSDALE, DEWPIDER, ARAQUANID, TOTEM_ARAQUANID, FOMANTIS, LURANTIS, TOTEM_LURANTIS, MORELULL, SHIINOTIC, SALANDIT, SALAZZLE, TOTEM_SALAZZLE, STUFFUL, BEWEAR, BOUNSWEET, STEENEE, TSAREENA, COMFEY, ORANGURU, PASSIMIAN, WIMPOD, GOLISOPOD, SANDYGAST, PALOSSAND, PYUKUMUKU, TYPE_NULL, SILVALLY, SILVALLY_BUG, SILVALLY_DARK, SILVALLY_DRAGON, SILVALLY_ELECTRIC, SILVALLY_FAIRY, SILVALLY_FIGHTING, SILVALLY_FIRE, SILVALLY_FLYING, SILVALLY_GHOST, SILVALLY_GRASS, SILVALLY_GROUND, SILVALLY_ICE, SILVALLY_POISON, SILVALLY_PSYCHIC, SILVALLY_ROCK, SILVALLY_STEEL, SILVALLY_WATER, MINIOR, CORE_MINIOR, RED_CORE_MINIOR, ORANGE_CORE_MINIOR, YELLOW_CORE_MINIOR, GREEN_CORE_MINIOR, BLUE_CORE_MINIOR, INDIGO_CORE_MINIOR, VIOLET_CORE_MINIOR, METEOR_MINIOR, KOMALA, TURTONATOR, TOGEDEMARU, TOTEM_TOGEDEMARU, MIMIKYU, MIMIKYU_BUSTED, TOTEM_MIMIKYU, BUSTED_TOTEM_MIMIKYU, BRUXISH, DRAMPA, DHELMISE, JANGMO_O, HAKAMO_O, KOMMO_O, TOTEM_KOMMO_O, TAPU_KOKO, TAPU_LELE, TAPU_BULU, TAPU_FINI, COSMOG, COSMOEM, SOLGALEO, LUNALA, NIHILEGO, BUZZWOLE, PHEROMOSA, XURKITREE, CELESTEELA, KARTANA, GUZZLORD, NECROZMA, DUSK_MANE_NECROZMA, DAWN_WINGS_NECROZMA, ULTRA_NECROZMA, MAGEARNA, ORIGINAL_COLOR_MAGEARNA, MARSHADOW, POIPOLE, NAGANADEL, STAKATAKA, BLACEPHALON, ZERAORA, MELTAN, MELMETAL, GIGANTAMAX_MELMETAL, GROOKEY, THWACKEY, RILLABOOM, GIGANTAMAX_RILLABOOM, SCORBUNNY, RABOOT, CINDERACE, GIGANTAMAX_CINDERACE, SOBBLE, DRIZZILE, INTELEON, GIGANTAMAX_INTELEON, SKWOVET, GREEDENT, ROOKIDEE, CORVISQUIRE, CORVIKNIGHT, GIGANTAMAX_CORVIKNIGHT, BLIPBUG, DOTTLER, ORBEETLE, GIGANTAMAX_ORBEETLE, NICKIT, THIEVUL, GOSSIFLEUR, ELDEGOSS, WOOLOO, DUBWOOL, CHEWTLE, DREDNAW, GIGANTAMAX_DREDNAW, YAMPER, BOLTUND, ROLYCOLY, CARKOL, COALOSSAL, GIGANTAMAX_COALOSSAL, APPLIN, FLAPPLE, GIGANTAMAX_FLAPPLE, APPLETUN, GIGANTAMAX_APPLETUN, SILICOBRA, SANDACONDA, GIGANTAMAX_SANDACONDA, CRAMORANT, CRAMORANT_GULPING, CRAMORANT_GORGING, ARROKUDA, BARRASKEWDA, TOXEL, TOXTRICITY_AMPED, TOXTRICITY_LOW_KEY, GIGANTAMAX_TOXTRICITY_AMPED, GIGANTAMAX_TOXTRICITY_LOW_KEY, SIZZLIPEDE, CENTISKORCH, GIGANTAMAX_CENTISKORCH, CLOBBOPUS, GRAPPLOCT, SINISTEA, ANTIQUE_SINISTEA, POLTEAGEIST, ANTIQUE_POLTEAGEIST, HATENNA, HATTREM, HATTERENE, GIGANTAMAX_HATTERENE, IMPIDIMP, MORGREM, GRIMMSNARL, GIGANTAMAX_GRIMMSNARL, OBSTAGOON, PERRSERKER, CURSOLA, SIRFETCH_D, MR_RIME, RUNERIGUS, MILCERY, ALCREMIE, VANILLA_CREAM_ALCREMIE, RUBY_CREAM_ALCREMIE, MATCHA_CREAM_ALCREMIE, MINT_CREAM_ALCREMIE, LEMON_CREAM_ALCREMIE, SALTED_CREAM_ALCREMIE, RUBY_SWIRL_ALCREMIE, CARAMEL_SWIRL_ALCREMIE, RAINBOW_SWIRL_ALCREMIE, GIGANTAMAX_ALCREMIE, FALINKS, PINCURCHIN, SNOM, FROSMOTH, STONJOURNER, EISCUE, ICE_FACE_EISCUE, NOICE_FACE_EISCUE, MALE_INDEEDEE, FEMALE_INDEEDEE, MORPEKO, FULL_BELLY_MODE_MORPEKO, HANGRY_MODE_MORPEKO, CUFANT, COPPERAJAH, GIGANTAMAX_COPPERAJAH, DRACOZOLT, ARCTOZOLT, DRACOVISH, ARCTOVISH, DURALUDON, GIGANTAMAX_DURALUDON, DREEPY, DRAKLOAK, DRAGAPULT, HERO_OF_MANY_BATTLES_ZACIAN, CROWNED_SWORD_ZACIAN, HERO_OF_MANY_BATTLES_ZAMAZENTA, CROWNED_SHIELD_ZAMAZENTA, ETERNATUS, ETERNAMAX_ETERNATUS, KUBFU, SINGLE_STRIKE_STYLE_URSHIFU, RAPID_STRIKE_STYLE_URSHIFU, GIGANTAMAX_SINGLE_STRIKE_STYLE_URSHIFU, GIGANTAMAX_RAPID_STRIKE_STYLE_URSHIFU, ZARUDE, DADA_ZARUDE, REGIELEKI, REGIDRAGO, GLASTRIER, SPECTRIER, CALYREX, ICE_RIDER_CALYREX, SHADOW_RIDER_CALYREX, WYRDEER, KLEAVOR, URSALUNA, MALE_BASCULEGION, FEMALE_BASCULEGION, SNEASLER, OVERQWIL, INCARNATE_ENAMORUS, ENAMORUS_THERIAN, SPRIGATITO, FLORAGATO, MEOWSCARADA, FUECOCO, CROCALOR, SKELEDIRGE, QUAXLY, QUAXWELL, QUAQUAVAL, LECHONK, MALE_OINKOLOGNE, FEMALE_OINKOLOGNE, TAROUNTULA, SPIDOPS, NYMBLE, LOKIX, PAWMI, PAWMO, PAWMOT, TANDEMAUS, MAUSHOLD, FAMILY_OF_THREE_MAUSHOLD, FAMILY_OF_FOUR_MAUSHOLD, FIDOUGH, DACHSBUN, SMOLIV, DOLLIV, ARBOLIVA, GREEN_PLUMAGE_SQUAWKABILLY, BLUE_PLUMAGE_SQUAWKABILLY, YELLOW_PLUMAGE_SQUAWKABILLY, WHITE_PLUMAGE_SQUAWKABILLY, NACLI, NACLSTACK, GARGANACL, CHARCADET, ARMAROUGE, CERULEDGE, TADBULB, BELLIBOLT, WATTREL, KILOWATTREL, MASCHIFF, MABOSSTIFF, SHROODLE, GRAFAIAI, BRAMBLIN, BRAMBLEGHAST, TOEDSCOOL, TOEDSCRUEL, KLAWF, CAPSAKID, SCOVILLAIN, RELLOR, RABSCA, FLITTLE, ESPATHRA, TINKATINK, TINKATUFF, TINKATON, WIGLETT, WUGTRIO, BOMBIRDIER, FINIZEN, ZERO_PALAFIN, HERO_PALAFIN, VAROOM, REVAVROOM, CYCLIZAR, ORTHWORM, GLIMMET, GLIMMORA, GREAVARD, HOUNDSTONE, FLAMIGO, CETODDLE, CETITAN, VELUZA, DONDOZO, TATSUGIRI, CURLY_TATSUGIRI, DROOPY_TATSUGIRI, STRETCHY_TATSUGIRI, ANNIHILAPE, CLODSIRE, FARIGIRAF, DUDUNSPARCE, TWO_SEGMENT_DUDUNSPARCE, THREE_SEGMENT_DUDUNSPARCE, KINGAMBIT, GREAT_TUSK, SCREAM_TAIL, BRUTE_BONNET, FLUTTER_MANE, SLITHER_WING, SANDY_SHOCKS, IRON_TREADS, IRON_BUNDLE, IRON_HANDS, IRON_JUGULIS, IRON_MOTH, IRON_THORNS, FRIGIBAX, ARCTIBAX, BAXCALIBUR, CHEST_GIMMIGHOUL, ROAMING_GIMMIGHOUL, GHOLDENGO, WO_CHIEN, CHIEN_PAO, TING_LU, CHI_YU, ROARING_MOON, IRON_VALIANT, KORAIDON, MIRAIDON, WALKING_WAKE, IRON_LEAVES, SPECIES_TOTAL
  // clang-format on
};
}  // namespace pokesim::dex
