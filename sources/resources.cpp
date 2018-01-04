
#include <GL/glew.h>

#include "globals.h"
#include "resources.h"
#include "functions/image_proc.h"

unsigned images[MAT_AMOUNT];

unsigned MAT_NULL = MAT_AMOUNT;

void SetupFilePaths( std::string* teharray )
{
    // GUI
    teharray[MAT_GUI_BUTTON]  = "";
    teharray[MAT_GUI_PANEL_1] = "textures/gui/panel-1.png";
    teharray[MAT_GUI_PANEL_2] = "textures/gui/panel-2.png";
    teharray[MAT_GUI_PANEL_3] = "textures/gui/panel-3.png";
    teharray[MAT_GUI_PANEL_4] = "textures/gui/panel-4.png";
    teharray[MAT_GUI_STARS_1] = "textures/gui/stars-1.png";
    teharray[MAT_GUI_STARS_2] = "textures/gui/stars-2.png";
    teharray[MAT_GUI_STARS_3] = "textures/gui/stars-3.png";
    teharray[MAT_GUI_SPELLBARS] = "textures/gui/spell_bars.png";
    teharray[MAT_GUI_SPELLINFO] = "textures/gui/spell_info.png";
    teharray[MAT_GUI_BIGBOOK]   = "textures/gui/bigbook.png";
    teharray[MAT_GUI_BOOK_SCHOOLS]  = "textures/gui/book-schools.png";
    teharray[MAT_GUI_SCHOOL_ARCANE] = "textures/gui/school-arcane.png";
    teharray[MAT_GUI_SCHOOL_FIRE]   = "textures/gui/school-fire.png";
    teharray[MAT_GUI_SCHOOL_STONE]  = "textures/gui/school-stone.png";
    teharray[MAT_GUI_SCHOOL_STORM]  = "textures/gui/school-storm.png";
    teharray[MAT_GUI_SCHOOL_ICE]    = "textures/gui/school-ice.png";
    teharray[MAT_GUI_SCHOOL_DARK]   = "textures/gui/school-dark.png";
    teharray[MAT_GUI_SCHOOL_LIGHT]  = "textures/gui/school-light.png";
    teharray[MAT_GUI_SCHOOL_WATER]  = "textures/gui/school-water.png";
    teharray[MAT_GUI_SCHOOL_NATURE] = "textures/gui/school-nature.png";
    teharray[MAT_GUI_SCHOOL_COGS]   = "textures/gui/school-cogs.png";
    teharray[MAT_GUI_FREE]    = "textures/gui/free.png";
    teharray[MAT_GUI_BUY]     = "textures/gui/buy.png";
    teharray[MAT_GUI_LOCKED]  = "textures/gui/orb-locked.png";
    teharray[MAT_GUI_UNKNOWN] = "textures/gui/orb-unknown.png";
    teharray[MAT_GUI_CHATFACE]      = "textures/gui/chatface.png";
    teharray[MAT_GUI_CUSTOMISATION] = "textures/gui/customisation.png";
    // HUD
    teharray[MAT_HUD_ARROW]  = "";
    teharray[MAT_HUD_ARROWS] = "";
    teharray[MAT_HUD_STAR]   = "";
    teharray[MAT_HUD_BIRD]   = "";
    teharray[MAT_HUD_PRESTIGE]  = "";
    // General
    teharray[MAT_CANCEL] = "";
    teharray[MAT_WAND_1] = "";
    teharray[MAT_WAND_5] = "";
    teharray[MAT_CLOUD]  = "";
    // Arcane
    teharray[MAT_ARCANE_SPELLS] = "";
    teharray[MAT_ARCANE_ARROW]  = "";
    teharray[MAT_ARCANE_TOVER]  = "";
    teharray[MAT_ARCANE_MINION_IMP_BODY]  = "";
    teharray[MAT_ARCANE_MINION_IMP_HEAD]  = "";
    teharray[MAT_ARCANE_MINION_IMP_LHAND] = "";
    teharray[MAT_ARCANE_MINION_IMP_RHAND] = "";
    teharray[MAT_ARCANE_MINION_IMP_LFOOT] = "";
    teharray[MAT_ARCANE_MINION_IMP_RFOOT] = "";
    teharray[MAT_ARCANE_AURA]   = "";
    teharray[MAT_ARCANE_GLYPHS] = "";
    teharray[MAT_ARCANE_EXPLOSION_1] = "";
    // Fire
    teharray[MAT_FIRE_SPELLS] = "textures/sprites/fire/spells.png";
    teharray[MAT_FIRE_FIREBALL] = "textures/sprites/fire/fireball.png";
    teharray[MAT_FIRE_BOMB]     = "textures/sprites/fire/bomb.png";
    teharray[MAT_FIRE_NAPALM]   = "textures/sprites/fire/napalm.png";
    teharray[MAT_FIRE_EMBER]    = "textures/sprites/fire/ember.png";
    teharray[MAT_FIRE_VOLCANO]  = "textures/sprites/fire/volcano.png";
    teharray[MAT_FIRE_FAMILIAR]      = "textures/sprites/fire/familiar.png";
    teharray[MAT_FIRE_MINION_DRAGON] = "textures/sprites/fire/dragon-fire.png";
    teharray[MAT_FIRE_SHIELD] = "textures/sprites/fire/shield.png";
    teharray[MAT_FIRE_WALL]   = "textures/sprites/fire/wall.png";
    teharray[MAT_FIRE_EFFECT_1]    = "textures/sprites/fire/effect-1.png";
    teharray[MAT_FIRE_EFFECT_2]    = "textures/sprites/fire/effect-2.png";
    teharray[MAT_FIRE_EXPLOSION_1] = "textures/sprites/fire/explosion-1.png";
    // Stone
    teharray[MAT_TOVER_STONE] = "textures/sprites/stone/tower.png";
    teharray[MAT_SLAB] = "textures/sprites/stone/slab.png";
    // Ice
    teharray[MAT_BOMB_ICE] = "";
    // Water
    teharray[MAT_BOLT_WATERBALL] = "";
    // Light
    teharray[MAT_TOVER_LIGHT] = "";
    // Dark
    teharray[MAT_TOVER_DARK] = "";
    // Nature
    teharray[MAT_TOVER_NATURE] = "";
    // Cogs
    teharray[MAT_TOVER_COG] = "textures/sprites/cogs/tower.png";
    teharray[MAT_COG_SMALL] = "textures/sprites/cogs/cog-small.png";
    teharray[MAT_COG_BIG]   = "textures/sprites/cogs/cog-big.png";
    //Character parts
    //Hat/Head
    teharray[MAT_CHAR_HAT_1] = "textures/sprites/swag/hat/47.png";
    //Body
    teharray[MAT_CHAR_BODY_1] = "textures/sprites/swag/body/28.png";
    //Face
    teharray[MAT_CHAR_FACE_1] = "textures/sprites/swag/face/36.png";
    // Terrain backgrounds
    teharray[MAT_BACKGROUND_GRASSLAND] = "textures/terrain/grassland/background.png";
    teharray[MAT_BACKGROUND_MOUNTAINS] = "textures/terrain/mountains/background.png";
    teharray[MAT_BACKGROUND_ELVES]     = "textures/terrain/elves/background.png";
    teharray[MAT_BACKGROUND_CAVES]     = "textures/terrain/caves/background.png";
    teharray[MAT_BACKGROUND_SWAMP]     = "textures/terrain/swamp/background.png";
    teharray[MAT_BACKGROUND_GRAVEYARD] = "textures/terrain/graveyard/background.png";
    teharray[MAT_BACKGROUND_SKY]       = "textures/terrain/sky/background.png";
    teharray[MAT_BACKGROUND_ISLAND]    = "textures/terrain/island/background.png";
    teharray[MAT_BACKGROUND_CRYSTALS]  = "textures/terrain/crystals/background.png";
    // Terrain
    teharray[MAT_TERRAIN_GRASSLAND] = "textures/terrain/grassland/terrain-2.png";
    teharray[MAT_TERRAIN_MOUNTAINS] = "";
    teharray[MAT_TERRAIN_ELVES]     = "";
    teharray[MAT_TERRAIN_CAVES]     = "";
    teharray[MAT_TERRAIN_SWAMP]     = "";
    teharray[MAT_TERRAIN_GRAVEYARD] = "";
    teharray[MAT_TERRAIN_SKY]       = "";
    teharray[MAT_TERRAIN_ISLAND]    = "";
    teharray[MAT_TERRAIN_CRYSTALS]  = "textures/terrain/crystals/terrain-1.png";
}

void SetupSingleFrame( short matid );
void SetupEvenFrames( short matid, short amount );
void SetupPanelFrames( short matid );

// Init properties, sprite sheet clips of all materials
void SetupMaterials()
{
    CMaterial * mat;
    int width, height;
    float w, h;

    // GUI
    for ( short i=MAT_GUI_PANEL_1; i<=MAT_GUI_PANEL_4; i++ ) { SetupPanelFrames(i); }
    for ( short i=MAT_GUI_STARS_1; i<=MAT_GUI_UNKNOWN; i++ ) { SetupSingleFrame(i); }
    SetupEvenFrames(MAT_GUI_CHATFACE,3);

    // HUD

    // Arcane

    // Fire
    mat = &Materials->at(MAT_FIRE_SPELLS); mat->GenFrames(13);
    glBindTexture( GL_TEXTURE_2D, mat->GetTexID() );
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width ); w = width;
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height ); h = height;
    mat->SetFramePos ( 1 );
    mat->SetFrameSize( 1, 37.f, h );
    mat->SetFrameUV  ( 1, 0.f, 0.f, 37.f/w, 1.f );
    mat->SetFramePos ( 2 );
    mat->SetFrameSize( 2, 39.f, h );
    mat->SetFrameUV  ( 2, 37.f/w, 0.f, 39.f/w, 1.f );
    mat->SetFramePos ( 3 );
    mat->SetFrameSize( 3, 35.f, h );
    mat->SetFrameUV  ( 3, 76.f/w, 0.f, 35.f/w, 1.f );
    mat->SetFramePos ( 4 );
    mat->SetFrameSize( 4, 36.f, h );
    mat->SetFrameUV  ( 4, 111.f/w, 0.f, 36.f/w, 1.f );
    mat->SetFramePos ( 5 );
    mat->SetFrameSize( 5, 34.f, h );
    mat->SetFrameUV  ( 5, 147.f/w, 0.f, 34.f/w, 1.f );
    mat->SetFramePos ( 6 );
    mat->SetFrameSize( 6, 36.f, h );
    mat->SetFrameUV  ( 6, 181.f/w, 0.f, 36.f/w, 1.f );
    mat->SetFramePos ( 7 );
    mat->SetFrameSize( 7, 40.f, h );
    mat->SetFrameUV  ( 7, 217.f/w, 0.f, 40.f/w, 1.f );
    mat->SetFramePos ( 8 );
    mat->SetFrameSize( 8, 36.f, h );
    mat->SetFrameUV  ( 8, 257.f/w, 0.f, 36.f/w, 1.f );
    mat->SetFramePos ( 9 );
    mat->SetFrameSize( 9, 39.f, h );
    mat->SetFrameUV  ( 9, 293.f/w, 0.f, 39.f/w, 1.f );
    mat->SetFramePos ( 10 );
    mat->SetFrameSize( 10, 39.f, h );
    mat->SetFrameUV  ( 10, 332.f/w, 0.f, 39.f/w, 1.f );
    mat->SetFramePos ( 11 );
    mat->SetFrameSize( 11, 37.f, h );
    mat->SetFrameUV  ( 11, 371.f/w, 0.f, 37.f/w, 1.f );
    mat->SetFramePos ( 12 );
    mat->SetFrameSize( 12, 39.f, h );
    mat->SetFrameUV  ( 12, 408.f/w, 0.f, 39.f/w, 1.f );
    mat->SetFramePos ( 13 );
    mat->SetFrameSize( 13, 39.f, h );
    mat->SetFrameUV  ( 13, 447.f/w, 0.f, 39.f/w, 1.f );

    for ( short i=MAT_FIRE_FIREBALL; i<=MAT_FIRE_VOLCANO; i++ ) { SetupSingleFrame(i); }
    mat = &Materials->at(MAT_FIRE_SHIELD); mat->GenFrames(4);
    glBindTexture( GL_TEXTURE_2D, mat->GetTexID() );
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width ); w = width;
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height ); h = height;
    mat->SetFramePos ( 1 );
    mat->SetFrameSize( 1, 45.f, h );
    mat->SetFrameUV  ( 1, 0.f, 0.f, 45.f/w, 1.f );
    mat->SetFramePos ( 2 );
    mat->SetFrameSize( 2, 41.f, h );
    mat->SetFrameUV  ( 2, 45.f/w, 0.f, 41.f/w, 1.f );
    mat->SetFramePos ( 3 );
    mat->SetFrameSize( 3, 41.f, h );
    mat->SetFrameUV  ( 3, 86.f/w, 0.f, 41.f/w, 1.f );
    mat->SetFramePos ( 4 );
    mat->SetFrameSize( 4, 38.f, h );
    mat->SetFrameUV  ( 4, 127.f/w, 0.f, 38.f/w, 1.f );

    // Stone

    // Storm

    // Cogs
    for ( short i=MAT_TOVER_COG; i<=MAT_COG_BIG; i++ )
    {
        SetupSingleFrame(i);
    }

    // Swag
    for ( short i=MAT_CHAR_HAT_1; i<=MAT_CHAR_FACE_1; i++ )
    {
        SetupSingleFrame(i);
    }

    // Terrain
    for ( short i=MAT_BACKGROUND_GRASSLAND; i<=MAT_TERRAIN_CRYSTALS; i++ )
    {
        SetupSingleFrame(i);
    }

    glBindTexture( GL_TEXTURE_2D, 0 );
}

/// Setup helpers
void SetupSingleFrame( short matid )
{
    int width, height;
    CMaterial * mat = &Materials->at(matid);
    mat->GenFrames(1);
    glBindTexture( GL_TEXTURE_2D, mat->GetTexID() );
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width );
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height );
    mat->SetFramePos( 1 );
    mat->SetFrameSize( 1, width, height );
    mat->SetFrameUV( 1 );
}
void SetupEvenFrames( short matid, short amount )
{
    int width, height;
    CMaterial * mat = &Materials->at(matid);
    mat->GenFrames(amount);
    glBindTexture( GL_TEXTURE_2D, mat->GetTexID() );
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width );
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height );
    float w = width/float(amount);
    float h = height;
    for ( short i=0; i<amount; i++ )
    {
        mat->SetFramePos( i+1 );
        mat->SetFrameSize( (i+1), w, h );
        mat->SetFrameUV( i+1, (i*w)/width, 0.f, w/width, 1.f );
    }
}
void SetupPanelFrames( short matid ) // Even 3x3 sections
{
    int width, height;
    CMaterial* mat = &Materials->at(matid);
    mat->GenFrames(9);
    glBindTexture( GL_TEXTURE_2D, mat->GetTexID() );
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width );
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height );
    float w = width/3.f;
    float h = height/3.f;
    for ( short i=0; i<9; i++ )
    {
        mat->SetFramePos( i+1 );
        mat->SetFrameSize( (i+1), w, h );
        mat->SetFrameUV( i+1, (i%3*w)/width, (i/3*h)/height, w/width, h/height );
    }
}

/// General Helpers
CMaterial GetMaterial(int ID)
{
    return Materials->at(ID);
}
unsigned GetMaterialID(int ID)
{
    return Materials->at(ID).GetTexID();
}

/// Precaching
void PrecacheTextures()
{
    glGenTextures( MAT_AMOUNT, images );
    std::string img_paths[MAT_AMOUNT];
    SetupFilePaths( img_paths );

    CMaterial mat;
    Materials = new std::vector<CMaterial>(MAT_AMOUNT,mat);

    for ( int i=0; i<MAT_AMOUNT; i++ )
    {
        LoadImage( img_paths[ i ], images[ i ], i );
        Materials->at(i).SetTexID( images[ i ] );
    }
}
//void PrecacheSounds();

/// Decaching
void DecacheTextures()
{
    glDeleteTextures( MAT_AMOUNT, &images[0] );
}
//void DecacheSounds();

