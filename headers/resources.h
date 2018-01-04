#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>

#include "classes/c_material.h"

extern unsigned images[];

extern unsigned MAT_NULL;

enum MATERIALS
{
    // GUI
    MAT_GUI_BUTTON = 0,
    MAT_GUI_PANEL_1,
    MAT_GUI_PANEL_2,
    MAT_GUI_PANEL_3,
    MAT_GUI_PANEL_4,
    MAT_GUI_STARS_1,
    MAT_GUI_STARS_2,
    MAT_GUI_STARS_3,
    MAT_GUI_SPELLBARS,
    MAT_GUI_SPELLINFO,
    MAT_GUI_BIGBOOK,
    MAT_GUI_BOOK_SCHOOLS,
    MAT_GUI_SCHOOL_ARCANE,
    MAT_GUI_SCHOOL_FIRE,
    MAT_GUI_SCHOOL_STONE,
    MAT_GUI_SCHOOL_STORM,
    MAT_GUI_SCHOOL_ICE,
    MAT_GUI_SCHOOL_DARK,
    MAT_GUI_SCHOOL_LIGHT,
    MAT_GUI_SCHOOL_WATER,
    MAT_GUI_SCHOOL_NATURE,
    MAT_GUI_SCHOOL_COGS,
    MAT_GUI_FREE,
    MAT_GUI_BUY,
    MAT_GUI_LOCKED,
    MAT_GUI_UNKNOWN,
    MAT_GUI_CUSTOMISATION,
    MAT_GUI_CHATFACE,

    // HUD
    MAT_HUD_ARROW,
    MAT_HUD_ARROWS,
    MAT_HUD_STAR,
    MAT_HUD_BIRD,
    MAT_HUD_PRESTIGE,

    // General
    MAT_CANCEL,
    MAT_WAND_1,
    MAT_WAND_5,
    MAT_CLOUD,

    // Arcane
    MAT_ARCANE_SPELLS,
    MAT_ARCANE_ARROW,
    MAT_ARCANE_TOVER,
    MAT_ARCANE_MINION_IMP_BODY,
    MAT_ARCANE_MINION_IMP_HEAD,
    MAT_ARCANE_MINION_IMP_LHAND,
    MAT_ARCANE_MINION_IMP_RHAND,
    MAT_ARCANE_MINION_IMP_LFOOT,
    MAT_ARCANE_MINION_IMP_RFOOT,
    MAT_ARCANE_AURA,
    MAT_ARCANE_GLYPHS,
    MAT_ARCANE_EXPLOSION_1,

    // Fire
    MAT_FIRE_SPELLS,
    MAT_FIRE_FIREBALL,
    MAT_FIRE_BOMB,
    MAT_FIRE_NAPALM,
    MAT_FIRE_EMBER,
    MAT_FIRE_VOLCANO,
    MAT_FIRE_FAMILIAR,
    MAT_FIRE_MINION_DRAGON,
    MAT_FIRE_SHIELD,
    MAT_FIRE_WALL,
    MAT_FIRE_EFFECT_1, // Napalm burst
    MAT_FIRE_EFFECT_2, // Smoke
    MAT_FIRE_EXPLOSION_1,

    // Stone
    MAT_STONE_SPELLS,
    MAT_TOVER_STONE,
    MAT_BOMB_ROCK,
    MAT_BOLT_MUDBALL,
    MAT_BOLT_STONE,
    MAT_BOLT_PEBBLE,
    MAT_SLAB,

    // Ice
    MAT_BOMB_ICE,
    MAT_BOLT_SNOWBALL,
    MAT_TOVER_ICE,
    MAT_COMET,

    // Water
    MAT_BOLT_WATERBALL,
    MAT_CLAM,
    MAT_DROPLET,
    MAT_PARTICLES_RAINDROP,

    // Light
    MAT_TOVER_LIGHT,

    // Dark
    MAT_TOVER_DARK,

    // Nature
    MAT_TOVER_NATURE,

    // Cogs
    MAT_TOVER_COG,
    MAT_COG_SMALL,
    MAT_COG_BIG,

    //Character parts (aka Swag)
    //Hat/Head
    MAT_CHAR_HAT_1,
    //Body
    MAT_CHAR_BODY_1,
    //Face
    MAT_CHAR_FACE_1,
    //Acessory
    MAT_CHAR_ACS_1,
    //Left hand
    MAT_CHAR_LHAND_1,
    //Right hand
    MAT_CHAR_RHAND_1,

    // Terrain backgrounds
    MAT_BACKGROUND_GRASSLAND,
    MAT_BACKGROUND_MOUNTAINS,
    MAT_BACKGROUND_ELVES,
    MAT_BACKGROUND_CAVES,
    MAT_BACKGROUND_SWAMP,
    MAT_BACKGROUND_GRAVEYARD,
    MAT_BACKGROUND_SKY,
    MAT_BACKGROUND_ISLAND,
    MAT_BACKGROUND_CRYSTALS,

    // Terrain
    MAT_TERRAIN_GRASSLAND,
    MAT_TERRAIN_MOUNTAINS,
    MAT_TERRAIN_ELVES,
    MAT_TERRAIN_CAVES,
    MAT_TERRAIN_SWAMP,
    MAT_TERRAIN_GRAVEYARD,
    MAT_TERRAIN_SKY,
    MAT_TERRAIN_ISLAND,
    MAT_TERRAIN_CRYSTALS,

    // Amount of images
    MAT_AMOUNT,
};

void SetupFilePaths( std::string* teharray );

void SetupMaterials();

void SetupSingleFrame( short matid );
void SetupEvenFrames( short matid, short amount );
void SetupPanelFrames( short matid );

CMaterial GetMaterial(int ID);
unsigned GetMaterialID(int ID);

void PrecacheTextures();
//void PrecacheSounds();
void DecacheTextures();
//void DecacheSounds();

#endif
