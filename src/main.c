#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "string.h"

#include "dialogs.h"
#include "error.h"
#include "imports.h"
#include "strhelper.h"

// Imports from AEP
RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_register_replacement(enum asset_e asset_id, void *asset_data));
RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_register_replacement_with_size(enum asset_e asset_id, void *asset_data, s32 size));
RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_unregister_replacement(enum asset_e asset_id));

// Our variables
static char new[2][0x20];
static unsigned int tracker = 0;
static unsigned int disabled = 0;

/*
 * Patched parade info arrays and new sizes
 *
 * FF = Furnance fun
 * FC = final character parade / end of game
 */
#define PARADE_FF_SIZE 0x1C
#define PARADE_FC_SIZE 0x39

static const ParadeInfo paradeFF[PARADE_FF_SIZE] = {
    {MAP_8E_GL_FURNACE_FUN,         0x14, 90, "GRUNTILDA",         0},
    {MAP_8E_GL_FURNACE_FUN,         0x13, 110, "TOOTY",            0},
    {MAP_1_SM_SPIRAL_MOUNTAIN,      0x13, 100, "BOTTLES",          0},
    {MAP_2_MM_MUMBOS_MOUNTAIN,      0x2D, 130, "JINJO",            0},
    {MAP_E_MM_MUMBOS_SKULL,         0x02, 60, "MUMBO JUMBO",       0},
    {MAP_2_MM_MUMBOS_MOUNTAIN,      0x2F, 125, "JU-JU",            0},
    {MAP_2_MM_MUMBOS_MOUNTAIN,      0x2C, 115, "CONGA",            0},
    {MAP_2_MM_MUMBOS_MOUNTAIN,      0x2B, 110, "CHIMPY",           0},
    {MAP_74_GL_GV_PUZZLE,           0x08, 95, "BRENTILDA",         0},
    {MAP_7_TTC_TREASURE_TROVE_COVE, 0x22, 50, "CAPTAIN BLUBBER",   0},
    {MAP_7_TTC_TREASURE_TROVE_COVE, 0x23, 120, "NIPPER",           0},
    {MAP_B_CC_CLANKERS_CAVERN,      0x0B, 105, "CLANKER",          0},
    {MAP_D_BGS_BUBBLEGLOOP_SWAMP,   0x23, 95, "TANKTUP",           0},
    {MAP_10_BGS_MR_VILE,            0x03, 100, "MR. VILE",         0},
    {MAP_11_BGS_TIPTUP,             0x08, 115, "TIPTUP",           0},
    {MAP_27_FP_FREEZEEZY_PEAK,      0x3D, 115, "BOGGY",            0},
    {MAP_27_FP_FREEZEEZY_PEAK,      0x3E, 110, "WOZZA",            0},
    {MAP_12_GV_GOBIS_VALLEY,        0x39, 100, "TRUNKER",          0},
    {MAP_12_GV_GOBIS_VALLEY,        0x38, 125, "GOBI",             0},
    {MAP_16_GV_RUBEES_CHAMBER,      0x04, 35, "RUBEE AND TOOTS",   0},
    {MAP_1C_MMM_CHURCH,             0x05, 90, "MOTZAND",           0},
    {MAP_26_MMM_NAPPERS_ROOM,       0x06, 110, "NAPPER",           0},
    {MAP_2C_MMM_BATHROOM,           0x03, 115, "LOGGO",            0},
    {MAP_31_RBB_RUSTY_BUCKET_BAY,   0x27, 105, "SNORKEL",          0},
    {MAP_44_CCW_SUMMER,             0x0F, 100, "GNAWTY",           0},
    {MAP_46_CCW_WINTER,             0x0C, 125, "EYRIE",            0},
    {MAP_5E_CCW_SPRING_NABNUTS_HOUSE, 0x01, 110, "NABNUT",         0},
    {MAP_1_SM_SPIRAL_MOUNTAIN,      0xFD, 20, "BANJO AND KAZOOIE", 0}
};

static const ParadeInfo paradeFC[PARADE_FC_SIZE] ={
    {MAP_93_GL_DINGPOT,             0x11, 105, "DINGPOT",           0},
    {MAP_84_CS_UNUSED_MACHINE_ROOM, 0x00, 107, "KLUNGO",            0},
    {MAP_1_SM_SPIRAL_MOUNTAIN,      0x29, 110, "TOPPER",            6},
    {MAP_1_SM_SPIRAL_MOUNTAIN,      0x27, 118, "BAWL",              7},
    {MAP_1_SM_SPIRAL_MOUNTAIN,      0x28, 75, "COLLIWOBBLE",        8},
    {MAP_1_SM_SPIRAL_MOUNTAIN,      0x2A, 100, "QUARRIE",           0},
    {MAP_71_GL_STATUE_ROOM,         0x0D, 85, "GRUNTLING",          0},
    {MAP_C_MM_TICKERS_TOWER,        0x09, 113, "TICKER",            0},
    {MAP_2_MM_MUMBOS_MOUNTAIN,      0x30, 105, "BIGBUTT",           0},
    {MAP_2_MM_MUMBOS_MOUNTAIN,      0x2E, 105, "GRUBLIN",           0},
    {MAP_7_TTC_TREASURE_TROVE_COVE, 0x24, 120, "LEAKY",             0},
    {MAP_7_TTC_TREASURE_TROVE_COVE, 0x25, 105, "LOCKUP",            0},
    {MAP_7_TTC_TREASURE_TROVE_COVE, 0x26, 67, "LITTLE LOCKUP",      0},
    {MAP_7_TTC_TREASURE_TROVE_COVE, 0x27, 100, "YUM-YUM",           0},
    {MAP_7_TTC_TREASURE_TROVE_COVE, 0x28, 110, "SNIPPET",           0},
    {MAP_7_TTC_TREASURE_TROVE_COVE, 0x29, 95, "SHRAPNEL",           0},
    {MAP_7_TTC_TREASURE_TROVE_COVE, 0x2A, 105, "SNACKER",          14},
    {MAP_B_CC_CLANKERS_CAVERN,      0x1A, 110, "GLOOP",             0},
    {MAP_B_CC_CLANKERS_CAVERN,      0x1B, 60, "GRILLE CHOMPA",      1},
    {MAP_B_CC_CLANKERS_CAVERN,      0x1C, 70, "MUTIE-SNIPPET",      0},
    {MAP_22_CC_INSIDE_CLANKER,      0x0B, 93, "WHIPLASH",           0},
    {MAP_D_BGS_BUBBLEGLOOP_SWAMP,   0x24, 100, "CROCTUS",           0},
    {MAP_D_BGS_BUBBLEGLOOP_SWAMP,   0x25, 115, "FLIBBIT",           0},
    {MAP_D_BGS_BUBBLEGLOOP_SWAMP,   0x26, 90, "BUZZBOMB",           0},
    {MAP_11_BGS_TIPTUP,             0x09, 45, "THE TIPTUP CHOIR",   0},
    {MAP_41_FP_BOGGYS_IGLOO,        0x05, 105, "GROGGY",            0},
    {MAP_41_FP_BOGGYS_IGLOO,        0x06, 117, "SOGGY",             0},
    {MAP_41_FP_BOGGYS_IGLOO,        0x07, 110, "MOGGY",             0},
    {MAP_27_FP_FREEZEEZY_PEAK,      0x3F, 65, "THE TWINKLIES",     -1},
    {MAP_27_FP_FREEZEEZY_PEAK,      0x40, 37, "TWINKLY MUNCHER",   -2},
    {MAP_27_FP_FREEZEEZY_PEAK,      0x41, 100, "SIR SLUSH",         0},
    {MAP_27_FP_FREEZEEZY_PEAK,      0x42, 107, "CHINKER",           0},
    {MAP_12_GV_GOBIS_VALLEY,        0x3A, 120, "JINXY",            -3},
    {MAP_12_GV_GOBIS_VALLEY,        0x3B, 40, "THE ANCIENT ONES",   0},
    {MAP_12_GV_GOBIS_VALLEY,        0x3C, 110, "GRABBA",            2},
    {MAP_12_GV_GOBIS_VALLEY,        0x3D, 110, "SCABBY",            0},
    {MAP_12_GV_GOBIS_VALLEY,        0x3E, 113, "SLAPPA",            3},
    {MAP_13_GV_MEMORY_GAME,         0x03, 90, "MUM-MUM",            0},
    {MAP_16_GV_RUBEES_CHAMBER,      0x05, 115, "HISTUP",            0},
    {MAP_24_MMM_TUMBLARS_SHED,      0x02, 95, "TUMBLAR",            0},
    {MAP_28_MMM_EGG_ROOM,           0x01, 0x20, "PORTRAIT CHOMPA",  4},
    {MAP_1D_MMM_CELLAR,             0x0A, 110, "TEE-HEE",          11},
    {MAP_2C_MMM_BATHROOM,           0x04, 120, "LIMBO",             0},
    {MAP_1B_MMM_MAD_MONSTER_MANSION, 0x28, 118, "RIPPER",          12},
    {MAP_1B_MMM_MAD_MONSTER_MANSION, 0x29, 117, "NIBBLY",           0},
    {MAP_38_RBB_CONTAINER_3,        0x02, 83, "BOOM BOX",          13},
    {MAP_3A_RBB_BOSS_BOOM_BOX,      0x04, 50, "BOSS BOOM BOX",      0},
    {MAP_31_RBB_RUSTY_BUCKET_BAY,   0x28, 105, "GRIMLET",           9},
    {MAP_31_RBB_RUSTY_BUCKET_BAY,   0x29, 105, "FLOTSAM",           0},
    {MAP_3C_RBB_KITCHEN,            0x05, 50, "SEAMAN GRUBLIN",     0},
    {MAP_35_RBB_WAREHOUSE,          0x06, 110, "CHUMP",             0},
    {MAP_44_CCW_SUMMER,             0x10, 87, "SNAREBEAR",         10},
    {MAP_45_CCW_AUTUMN,             0x14, 85, "BIG CLUCKER",        5},
    {MAP_5B_CCW_SPRING_ZUBBA_HIVE,  0x04, 78, "THE ZUBBAS",         0},
    {MAP_46_CCW_WINTER,             0x0E, 63, "GRUBLIN HOOD",       0},
    {MAP_66_CCW_SUMMER_WHIPCRACK_ROOM, 0x06, 83, "WHIPCRACK",       0},
    {MAP_72_GL_BGS_LOBBY,           0x04, 110, "CHEATO",            0}
};

// Make sure to run after AEP initialised
RECOMP_HOOK_RETURN("assetCache_init")
void onInit()
{
    // Detect Latin 1 mod
    if(recomp_is_dependency_met("font_plus_latin_1") == DEPENDENCY_STATUS_FOUND)
    {
        showError("German mod: Font Plus Latin 1 mod detected! Disabling german to prevent undefined behaviour\n");
        disabled = 1;
        return;
    }

    // Move furnance fun credit dialogs up +1 after rubee and tots to make place for motzand
    int i = 0x11C8;
    for( ; i >= 0x11C3; i--)
    {
        void *asset = assetcache_get(i++);
        bk_recomp_aep_register_replacement(i--, asset);
    }

    // Feed quiz/grunty asset replacements to AEP
    for(i = 0 ; i < ASSETS_SIZES_SIZE; i++)
        bk_recomp_aep_register_replacement_with_size(asset_name[i], (void *)asset_data[i], (u32)asset_size[i]);

    // Feed dialog asset replacements to AEP
    for( ; i < ASSETS_SIZE; i++)
        bk_recomp_aep_register_replacement(asset_name[i], (void *)asset_data[i]);

    // Replace global pointers to menu strings
    CONTROL_STICK_INSTRUCTIONS = "W[HLE MIT DEM 3D-STICK EIN SPIEL AUS.";
    ERASE_INSTRUCTIONS = "DR]CKE A, UM ZU SPIELEN, ODER DEN Z-TRIGGER, UM DEN SPIELSTAND ZU L\\SCHEN!";
    ERASE_CONFIRMATION = "SICHER? DR]CKE A, UM ZU BEST[TIGEN, ODER B, UM ZU WIDERRUFEN.";

    D_8036C4E0[0].str = "ZUR]CK ZUM SPIEL";
    D_8036C4E0[2].str = "STATISTIK";
    D_8036C4E0[3].str = "SICHERN UND ENDE";

    D_8036C58C[0].string = (u8 *)"GESAMTSTATISTIK";
}

// Game exit, clean up
RECOMP_HOOK_RETURN("mainThread_entry")
void onExit()
{
    if(disabled)
        return;

    // Unregister shifted dialogs (furnance fun)
    int i = 0x11C4;
    for( ; i < 0x11CA; i++)
        bk_recomp_aep_unregister_replacement(i);

    // Release holded assets for shift (furnance fun)
    for(i = 0x11C3; i < 0x11C9; i++)
    {
        void *asset = assetcache_get(i);
        assetcache_release(asset);
        assetcache_release(asset);
    }

    // Unregister PAL assets
    for(i = 0; i < ASSETS_SIZE; i++)
        bk_recomp_aep_unregister_replacement(asset_name[i]);

    disabled = 0;
}

// Overwrite text of game information zoombox (start game menu)
RECOMP_HOOK_RETURN("setGameInformationZoombox")
void overwriteGKZoombox()
{
    if(disabled)
        return;

    // Get text from zoombox (without having a struct, so pointer magic)
    u8 *ptr = chGameSelectBottomZoombox;
    ptr += 0x13C;

    char *old0 = *(char **)ptr;
    ptr += sizeof(char *);
    char *old1 = *(char **)ptr;

    old0 += 5;

    // Replace "GAME X:" with "SPIEL X:"
    strcpy(new[0], "SPIEL ");
    old0 = strcat_till(new[0], old0, ' ');
    strcat(new[0], " ");

    old0++;
    if(old0[0] == 'E')
    {
        // Replace "EMPTY" with "LEER", also make sure second line is empty
        strcat(new[0], "LEER");
        new[1][0] = '\0';
    }
    else
    {
        // First line
        // Replace "TIME XX:YY:ZZ," with "ZEIT XX:YY:ZZ,"
        old0 += 5;
        strcat(new[0], "ZEIT ");
        strcat_till(new[0], old0, ',');
        strcat(new[0], ",");

        // Second line
        // Replace "X JIGSAW[S]" with "X PUZZLEZEIL[E]"
        old1 = strcpy_till(new[1], old1, ' ');
        strcat(new[1], " PUZZLETEIL");
        if(new[1][0] != '1')
            strcat(new[1], "E");

        // Replace "X NOTE[S]" with "X NOTE[N]"
        old1 = strstrc(old1, ',');
        char *n = strstrc(old1, 'S');
        if(n)
            *n = 'N';

        strcat(new[1], old1);
    }

    // Set new strings
    *(char **)ptr = new[1];
    ptr -= sizeof(char *);
    *(char **)ptr = new[0];
}

// Overwrite text of exit game confirmation - this has multiple steps

// Step 1: Track when we enter the exit game confirmation
RECOMP_HOOK("gcPauseMenu_update")
void startEvent()
{
    if(!disabled && D_80383010.state == 5)
        tracker |= 1;
}

// Reset tracker when pause menu left
RECOMP_HOOK_RETURN("gcPauseMenu_update")
void stopEvent()
{
    tracker &= ~(1);
}

RECOMP_HOOK_RETURN("func_803183A4")
void overwriteString()
{
    if(!(tracker & 1))
        return;

    // get pointer to text
    u8 *ptr = D_80383010.zoombox[D_80383010.selection];
    ptr += 0x13C;
    const char *str = *(const char **)ptr;

    // Replace string pointer inside of textbox
    *(const char **)ptr = D_80383010.unk3_6 ? "SICHER?" : "A - JA, B - NEIN";

    // Reset tracker
    stopEvent();
}

// Patch parade/credits to PAL style. Has multipe steps, too

// Track parade init
RECOMP_HOOK("gcparade_setState")
void trackParade(u32 next_state)
{
    if(!disabled && next_state == 1)
        tracker |= 2;
}

/*
 * This gets called two times in gcparade_setState:
 *   if next_state == 1 then it's furnance fun parade init'
 *   else if next_state == 2 then it's end game parade init
 *
 * In both events it gets called right after gcparade_setState setted the parade info array pointer and size, so perfect time to overwrite that.
 * Use tracker value from previous hook to decide if furnance fun or end game
 */
RECOMP_HOOK("gcparade_8031AC8C")
void patchParade()
{
    if(disabled)
        return;

    if(tracker & 2)
    {
        D_803830F0.parade_element = (ParadeInfo *)paradeFF;
        D_803830F0.count = PARADE_FF_SIZE;

        // Reset tracker
        tracker &= ~(2);
    }
    else
    {
        D_803830F0.parade_element = (ParadeInfo *)paradeFC;
        D_803830F0.count = PARADE_FC_SIZE;
    }
}
