#pragma once
#include <cstdint>

namespace pokesim {
namespace dex {
// clang-format off

/** @brief Pokemon ability name */
enum Ability : std::uint16_t {
  NO_ABILITY = 0, ADAPTABILITY, AERILATE, AFTERMATH, AIR_LOCK, ANALYTIC, ANGER_POINT, ANGER_SHELL, ANTICIPATION, ARENA_TRAP, ARMOR_TAIL, AROMA_VEIL, AS_ONE, AURA_BREAK, BAD_DREAMS, BALL_FETCH, BATTERY, BATTLE_ARMOR, BATTLE_BOND, BEADS_OF_RUIN, BEAST_BOOST, BERSERK, BIG_PECKS, BLAZE, BULLETPROOF, CHEEK_POUCH, CHILLING_NEIGH, CHLOROPHYLL, CLEAR_BODY, CLOUD_NINE, COLOR_CHANGE, COMATOSE, COMMANDER, COMPETITIVE, COMPOUND_EYES, CONTRARY, CORROSION, COSTAR, COTTON_DOWN, CUD_CHEW, CURIOUS_MEDICINE, CURSED_BODY, CUTE_CHARM, DAMP, DANCER, DARK_AURA, DAUNTLESS_SHIELD, DAZZLING, DEFEATIST, DEFIANT, DELTA_STREAM, DESOLATE_LAND, DISGUISE, DOWNLOAD, DRAGONS_MAW, DRIZZLE, DROUGHT, DRY_SKIN, EARTH_EATER, EARLY_BIRD, EFFECT_SPORE, ELECTRIC_SURGE, ELECTROMORPHOSIS, EMERGENCY_EXIT, FAIRY_AURA, FILTER, FLAME_BODY, FLARE_BOOST, FLASH_FIRE, FLOWER_GIFT, FLOWER_VEIL, FLUFFY, FORECAST, FOREWARN, FRIEND_GUARD, FRISK, FULL_METAL_BODY, FUR_COAT, GALE_WINGS, GALVANIZE, GLUTTONY, GOOD_AS_GOLD, GOOEY, GORILLA_TACTICS, GRASS_PELT, GRASSY_SURGE, GRIM_NEIGH, GUARD_DOG, GULP_MISSILE, GUTS, HADRON_ENGINE, HARVEST, HEALER, HEATPROOF, HEAVY_METAL, HONEY_GATHER, HUGE_POWER, HUNGER_SWITCH, HUSTLE, HYDRATION, HYPER_CUTTER, ICE_BODY, ICE_FACE, ICE_SCALES, ILLUMINATE, ILLUSION, IMMUNITY, IMPOSTER, INFILTRATOR, INNARDS_OUT, INNER_FOCUS, INSOMNIA, INTIMIDATE, INTREPID_SWORD, IRON_BARBS, IRON_FIST, JUSTIFIED, KEEN_EYE, KLUTZ, LEAF_GUARD, LEVITATE, LIBERO, LIGHT_METAL, LIGHTNING_ROD, LINGERING_AROMA, LIMBER, LIQUID_OOZE, LIQUID_VOICE, LONG_REACH, MAGIC_BOUNCE, MAGIC_GUARD, MAGICIAN, MAGMA_ARMOR, MAGNET_PULL, MARVEL_SCALE, MEGA_LAUNCHER, MERCILESS, MIMICRY, MINUS, MIRROR_ARMOR, MISTY_SURGE, MOLD_BREAKER, MOODY, MOTOR_DRIVE, MOXIE, MULTISCALE, MULTITYPE, MUMMY, MYCELIUM_MIGHT, NATURAL_CURE, NEUROFORCE, NEUTRALIZING_GAS, NO_GUARD, NORMALIZE, OBLIVIOUS, OPPORTUNIST, ORICHALCUM_PULSE, OVERCOAT, OVERGROW, OWN_TEMPO, PARENTAL_BOND, PASTEL_VEIL, PERISH_BODY, PICKPOCKET, PICKUP, PIXILATE, PLUS, POISON_HEAL, POISON_POINT, POISON_TOUCH, POWER_CONSTRUCT, POWER_OF_ALCHEMY, POWER_SPOT, PRANKSTER, PRESSURE, PRIMORDIAL_SEA, PRISM_ARMOR, PROPELLER_TAIL, PROTEAN, PROTOSYNTHESIS, PSYCHIC_SURGE, PUNK_ROCK, PURE_POWER, PURIFYING_SALT, QUARK_DRIVE, QUEENLY_MAJESTY, QUICK_DRAW, QUICK_FEET, RAIN_DISH, RATTLED, RECEIVER, RECKLESS, REFRIGERATE, REGENERATOR, RIPEN, RIVALRY, RKS_SYSTEM, ROCK_HEAD, ROCKY_PAYLOAD, ROUGH_SKIN, RUN_AWAY, SAND_FORCE, SAND_RUSH, SAND_SPIT, SAND_STREAM, SAND_VEIL, SAP_SIPPER, SCHOOLING, SCRAPPY, SCREEN_CLEANER, SEED_SOWER, SERENE_GRACE, SHADOW_SHIELD, SHADOW_TAG, SHARPNESS, SHED_SKIN, SHEER_FORCE, SHELL_ARMOR, SHIELD_DUST, SHIELDS_DOWN, SIMPLE, SKILL_LINK, SLOW_START, SLUSH_RUSH, SNIPER, SNOW_CLOAK, SNOW_WARNING, SOLAR_POWER, SOLID_ROCK, SOUL_HEART, SOUNDPROOF, SPEED_BOOST, STAKEOUT, STALL, STALWART, STAMINA, STANCE_CHANGE, STATIC, STEADFAST, STEAM_ENGINE, STEELWORKER, STEELY_SPIRIT, STENCH, STICKY_HOLD, STORM_DRAIN, STRONG_JAW, STURDY, SUCTION_CUPS, SUPER_LUCK, SUPREME_OVERLORD, SURGE_SURFER, SWARM, SWEET_VEIL, SWIFT_SWIM, SYMBIOSIS, SYNCHRONIZE, SWORD_OF_RUIN, TABLETS_OF_RUIN, TANGLED_FEET, TANGLING_HAIR, TECHNICIAN, TELEPATHY, TERAVOLT, THERMAL_EXCHANGE, THICK_FAT, TINTED_LENS, TORRENT, TOUGH_CLAWS, TOXIC_BOOST, TOXIC_DEBRIS, TRACE, TRANSISTOR, TRIAGE, TRUANT, TURBOBLAZE, UNAWARE, UNBURDEN, UNNERVE, UNSEEN_FIST, VESSEL_OF_RUIN, VICTORY_STAR, VITAL_SPIRIT, VOLT_ABSORB, WANDERING_SPIRIT, WATER_ABSORB, WATER_BUBBLE, WATER_COMPACTION, WATER_VEIL, WEAK_ARMOR, WELL_BAKED_BODY, WHITE_SMOKE, WIMP_OUT, WIND_POWER, WIND_RIDER, WONDER_GUARD, WONDER_SKIN, ZEN_MODE, ZERO_TO_HERO, ABILITY_TOTAL,
};

// clang-format on
}  // namespace dex

struct AbilityName {
  dex::Ability name = dex::NO_ABILITY;
};

}  // namespace pokesim