#ifndef __CONFDEF_H__
#define __CONFDEF_H__

/* Configuration defines */

#define MATERIAL_ID 4096

#define IRON (MATERIAL_ID + 1)
#define VALPURIUM (MATERIAL_ID + 2)
#define STONE (MATERIAL_ID + 3)
#define GRAVEL (MATERIAL_ID + 4)
#define MORAINE (MATERIAL_ID + 5)
#define WOOD (MATERIAL_ID + 6)
#define GLASS (MATERIAL_ID + 7)
#define PARCHMENT (MATERIAL_ID + 8)
#define CLOTH (MATERIAL_ID + 9)
#define MITHRIL (MATERIAL_ID + 10)
#define MARBLE (MATERIAL_ID + 11)
#define GOLD (MATERIAL_ID + 12)
#define GRASS (MATERIAL_ID + 13)
#define LEATHER (MATERIAL_ID + 14)
#define LEAF (MATERIAL_ID + 15)
#define FABRIC (MATERIAL_ID + 16)
#define PALM_LEAF (MATERIAL_ID + 17)
#define SULFUR (MATERIAL_ID + 18)
#define UNICORN_HORN (MATERIAL_ID + 19)
#define DIAMOND (MATERIAL_ID + 20)
#define SILVER (MATERIAL_ID + 21)
#define SAPPHIRE (MATERIAL_ID + 22)
#define RUBY (MATERIAL_ID + 23)
#define BRONZE (MATERIAL_ID + 24)
#define COPPER (MATERIAL_ID + 25)
#define TIN (MATERIAL_ID + 26)
#define STEEL (MATERIAL_ID + 27)
#define SPIDER_SILK (MATERIAL_ID + 28)
#define KEVLAR (MATERIAL_ID + 29)
#define OMMEL_HAIR (MATERIAL_ID + 30)
#define HARDENED_LEATHER (MATERIAL_ID + 31)
#define TROLL_HIDE (MATERIAL_ID + 32)
#define NYMPH_HAIR (MATERIAL_ID + 33)
#define ANGEL_HAIR (MATERIAL_ID + 34)
#define PHOENIX_FEATHER (MATERIAL_ID + 35)
#define GOLDEN_EAGLE_FEATHER (MATERIAL_ID + 36)
#define ICE (MATERIAL_ID + 37)

#define ORGANIC_SUBSTANCE_ID (4096 << 1)

#define BANANA_FLESH (ORGANIC_SUBSTANCE_ID + 1)
#define SCHOOL_FOOD (ORGANIC_SUBSTANCE_ID + 2)
#define BANANA_PEEL (ORGANIC_SUBSTANCE_ID + 3)
#define KIWI_FLESH (ORGANIC_SUBSTANCE_ID + 4)
#define PINEAPPLE_FLESH (ORGANIC_SUBSTANCE_ID + 5)
#define PLANT_FIBER (ORGANIC_SUBSTANCE_ID + 6)
#define MUTANT_PLANT_FIBER (ORGANIC_SUBSTANCE_ID + 7)
#define BONE (ORGANIC_SUBSTANCE_ID + 8)
#define BREAD (ORGANIC_SUBSTANCE_ID + 9)

#define GAS_ID (4096 * 3)

#define AIR (GAS_ID + 1)
#define MAGICAL_AIR (GAS_ID + 2)
#define SMOKE (GAS_ID + 3)

#define LIQUID_ID (4096 << 2)

#define OMMEL_URINE (LIQUID_ID + 1)
#define PEPSI (LIQUID_ID + 2)
#define WATER (LIQUID_ID + 3)
#define HEALING_LIQUID (LIQUID_ID + 4)
#define BLOOD (LIQUID_ID + 5)
#define BROWN_SLIME (LIQUID_ID + 6)
#define POISON_LIQUID (LIQUID_ID + 7)
#define VALDEMAR (LIQUID_ID + 8)
#define ANTITODE_LIQUID (LIQUID_ID + 9)
#define VODKA (LIQUID_ID + 10)
#define TROLL_BLOOD (LIQUID_ID + 11)

#define FLESH_ID (4096 * 5)

#define GOBLINOID_FLESH (FLESH_ID + 1)
#define PORK (FLESH_ID + 2)
#define BEEF (FLESH_ID + 3)
#define FROG_FLESH (FLESH_ID + 4)
#define ELPURI_FLESH (FLESH_ID + 5)
#define HUMAN_FLESH (FLESH_ID + 6)
#define DOLPHIN_FLESH (FLESH_ID + 7)
#define POLAR_BEAR_FLESH (FLESH_ID + 8)
#define WOLF_FLESH (FLESH_ID + 9)
#define DOG_FLESH (FLESH_ID + 10)
#define ENNER_BEAST_FLESH (FLESH_ID + 11)
#define SPIDER_FLESH (FLESH_ID + 12)
#define JACKAL_FLESH (FLESH_ID + 13)
#define MUTANT_ASS_FLESH (FLESH_ID + 14)
#define BAT_FLESH (FLESH_ID + 15)
#define WERE_WOLF_FLESH (FLESH_ID + 16)
#define KOBOLD_FLESH (FLESH_ID + 17)
#define GIBBERLING_FLESH (FLESH_ID + 18)
#define CAT_FLESH (FLESH_ID + 19)
#define RAT_FLESH (FLESH_ID + 20)
#define ANGEL_FLESH (FLESH_ID + 21)
#define DWARF_FLESH (FLESH_ID + 22)
#define DAEMON_FLESH (FLESH_ID + 23)
#define MAMMOTH_FLESH (FLESH_ID + 24)
#define BLACK_UNICORN_FLESH (FLESH_ID + 25)
#define GRAY_UNICORN_FLESH (FLESH_ID + 26)
#define WHITE_UNICORN_FLESH (FLESH_ID + 27)
#define LION_FLESH (FLESH_ID + 28)
#define BUFFALO_FLESH (FLESH_ID + 29)
#define SNAKE_FLESH (FLESH_ID + 30)
#define ORC_FLESH (FLESH_ID + 31)
#define OSTRICH_FLESH (FLESH_ID + 32)
#define CHAMELEON_FLESH (FLESH_ID + 33)

#define POWDER_ID (4096 * 6)

#define GUN_POWDER (POWDER_ID + 1)
#define SNOW (POWDER_ID + 2)

#define BROKEN 256

#define LONG_SWORD 1
#define TWO_HANDED_SWORD 2
#define TWO_HANDED_SCIMITAR 3
#define SPEAR 4
#define AXE 5
#define HALBERD 6
#define MACE 7
#define HAMMER 8
#define SICKLE 9
#define DAGGER 10
#define SHORT_SWORD 11
#define BASTARD_SWORD 12
#define BATTLE_AXE 13
#define SCYTHE 14

#define CHAIN_MAIL 1
#define PLATE_MAIL 2
#define ARMOR_OF_GREAT_HEALTH 3

#define WAND_OF_POLYMORPH 1
#define WAND_OF_STRIKING 2
#define WAND_OF_FIRE_BALLS 3
#define WAND_OF_TELEPORTATION 4
#define WAND_OF_HASTE 5
#define WAND_OF_SLOW 6
#define WAND_OF_RESURRECTION 7
#define WAND_OF_DOOR_CREATION 8
#define WAND_OF_INVISIBILITY 9
#define WAND_OF_CLONING 10
#define WAND_OF_LIGHTNING 11

#define RUNED_WHIP 1

#define CLOAK_OF_INVISIBILITY 1

#define BOOT_OF_STRENGTH 1
#define BOOT_OF_AGILITY 2
#define BOOT_OF_KICKING 3

#define GAUNTLET_OF_STRENGTH 1
#define GAUNTLET_OF_DEXTERITY 2

#define RING_OF_FIRE_RESISTANCE 1
#define RING_OF_POLYMORPH_CONTROL 2
#define RING_OF_INFRA_VISION 3
#define RING_OF_TELEPORTATION 4
#define RING_OF_TELEPORT_CONTROL 5
#define RING_OF_POLYMORPH 6
#define RING_OF_POISON_RESISTANCE 7
#define RING_OF_INVISIBILITY 8
#define RING_OF_ELECTRICITY_RESISTANCE 9
#define RING_OF_SEARCHING 10

#define AMULET_OF_LIFE_SAVING 1
#define AMULET_OF_ESP 2

#define FULL_HELMET 1
#define HELM_OF_PERCEPTION 2
#define HELM_OF_UNDERSTANDING 3
#define HELM_OF_BRILLIANCE 4
#define HELM_OF_ATTRACTIVITY 5

#define BELT_OF_CARRYING 1

#define SMALL_CHEST 1
#define CHEST 2
#define LARGE_CHEST 3
#define STRONG_BOX 4

#define BRAVERY 1
#define FEAR 2

#define ROOKIE 1
#define VETERAN 2
#define PATROL 3
#define SHOP 4
#define ELITE 5
#define MASTER 6
#define GRAND_MASTER 7

#define DARK 1
#define GREATER_DARK 2
#define LIGHT 3
#define GREATER_LIGHT 4

#define WARRIOR 1
#define WAR_LORD 2

#define BERSERKER 1
#define BUTCHER 2
#define PRINCE 3
#define KING 4

#define CONICAL 1
#define FLAT 2

#define TORTURING_CHIEF 1
#define WHIP_CHAMPION 2
#define WAR_LADY 3
#define QUEEN 4

#define CHIEFTAIN 1
#define LORD 2
#define PATRIARCH 3

#define GREATER 1

#define SLAUGHTERER 1
#define SQUAD_LEADER 2
#define OFFICER 3
#define GENERAL 4
#define MARSHAL 5

#define PARQUET 1
#define FLOOR 2
#define GROUND 3
#define GRASS_TERRAIN 4
#define LANDING_SITE 5
#define SNOW_TERRAIN 6

#define POOL 1

#define BRICK_FINE 1
#define BRICK_PROPAGANDA 2
#define BRICK_OLD 3
#define BRICK_PRIMITIVE 4
#define EARTH 5

#define PINE 1
#define FIR 2
#define LINDEN 3
#define HOLY_TREE 4
#define CARPET 5
#define COUCH 6
#define DOUBLE_BED 7
#define POOL_BORDER 8
#define POOL_CORNER 9
#define PALM 10
#define SNOW_PINE 11
#define SNOW_FIR 12
#define ANVIL 13

#define OREE_LAIR_ENTRY 3
#define OREE_LAIR_EXIT 4

#define BOOK_CASE 1

#define ROOM_NORMAL 1
#define ROOM_SHOP 2
#define ROOM_TEMPLE 3
#define ROOM_CATHEDRAL 4
#define ROOM_LIBRARY 5
#define ROOM_LANDING_SITE 6

#endif
