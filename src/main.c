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

    int i = 0;
    // Feed quiz/grunty asset replacements to AEP
    for( ; i < ASSETS_SIZES_SIZE; i++)
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
    if(!disabled && !tracker && D_80383010.state == 5)
        tracker = 1;
}

// Reset tracker when pause menu left
RECOMP_HOOK_RETURN("gcPauseMenu_update")
void stopEvent()
{
    tracker = 0;
}

RECOMP_HOOK_RETURN("func_803183A4")
void overwriteString()
{
    if(!tracker)
        return;

    // get pointer to text
    u8 *ptr = D_80383010.zoombox[D_80383010.selection];
    ptr += 0x13C;
    const char *str = *(const char **)ptr;

    // Check pointer and first char for validity
    if(str == NULL || str[0] != 'A')
        return;

    // Replace string pointer inside of textbox
    str++;
    if(strcmp((char *)str, "RE YOU SURE?") == 0)
        *(const char **)ptr = "SICHER?";
    else if(strcmp((char *)str, " - YES, B - NO") == 0)
        *(const char **)ptr = "A - JA, B - NEIN";

    // Reset tracker
    tracker = 0;
}
