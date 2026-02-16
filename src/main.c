#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "dialogs.h"
#include "imports.h"
#include "strhelper.h"

// Imports from AEP
RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_register_replacement(enum asset_e asset_id, void *asset_data));
RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_register_replacement_with_size(enum asset_e asset_id, void *asset_data, s32 size));
RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_unregister_replacement(enum asset_e asset_id));

// Our variables
static char new[2][0x20];
static char *newPA[2] = { new[0], new[1] };
static u32 oldState = 2;

// Make sure to run after AEP initialised
RECOMP_HOOK_RETURN("assetCache_init")
void onInit()
{
    // Feed dialog asset replacements to AEP
    for(int i = 0; i < ASSETS_SIZE; i++)
    {
        if(asset_size[i] == 0x00)
            bk_recomp_aep_register_replacement(asset_name[i], (void *)asset_data[i]);
        else
            bk_recomp_aep_register_replacement_with_size(asset_name[i], (void *)asset_data[i], (u32)asset_size[i]);
    }

    // Replace global pointers to menu strings
    CONTROL_STICK_INSTRUCTIONS = (u8 *)"W[HLE MIT DEM 3D-STICK EIN SPIEL AUS.";
    ERASE_INSTRUCTIONS = (u8 *)"DR]CKE A, UM ZU SPIELEN, ODER DEN Z-TRIGGER, UM DEN SPIELSTAND ZU L\\SCHEN!";
    ERASE_CONFIRMATION = (u8 *)"SICHER? DR]CKE A, UM ZU BEST[TIGEN ODER B, UM ZU WIDERRUFEN.";

    D_8036C4E0[0].str = (u8 *)"ZUR]CK ZUM SPIEL";
    D_8036C4E0[2].str = (u8 *)"STATISTIK";
    D_8036C4E0[3].str = (u8 *)"SICHERN UND ENDE";
}

// Overwrite text of game information zoombox (start game menu)
RECOMP_HOOK_RETURN("setGameInformationZoombox")
void overwriteGKZoombox(s32 gamenum)
{
    // Get text from zoombox (without having a struct, so pointer magic)
    u8 *ptr = (u8 *)chGameSelectBottomZoombox;
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
        old0 += 5;

        // First line
        // Replace "TIME XX:YY:ZZ," with "ZEIT XX:YY:ZZ,"
        strcpy(new[0], "ZEIT ");
        strcat_till(new[0], old0, ',');
        strcat(new[0], ",");

        // Second line
        // Replace "X JIGSAW[S]" with "X PUZZLEZEIL[E]"
        old1 = strcpy_till(new[1], old1, ' ');
        strcat(new[1], " PUZZLETEIL");
        if(new[1][0] != '1')
            strcat(new[1], "E");

        // Replace "X NOTE[S]" with "X NOTE[N]"
        old1 = strstr(old1, ',');
        char *n = strstr(old1, 'S');
        if(n)
            *n = 'N';

        strcat(new[1], old1);
    }

    // Finally set the new strings in-game
    func_8031877C(chGameSelectBottomZoombox);
    gczoombox_setStrings(chGameSelectBottomZoombox, 2, newPA);
}

RECOMP_HOOK_RETURN("gcPauseMenu_update")
void overwriteGCZoombox(s32 gamenum)
{
    if(D_80383010.state != 5)
    {
        oldState = 2;
        return;
    }

    if(D_80383010.unk3_6 == oldState)
        return;

    oldState = D_80383010.unk3_6;

    u8 *ptr = (u8 *)D_80383010.zoombox[D_80383010.selection];
    void *box = ptr;
    ptr += 0x13C;
    char *old = *(char **)ptr;

    if(old != NULL && old[0] == 'A')
    {
        char *ne = D_80383010.unk3_6 ? "BIST DU SICHER?" : "A - JA, B - NEIN";

        func_8031877C(box);
        gczoombox_setStrings(box, 1, &ne);
    }
}

// TODO: Ugly workaround
static u32 inPauseMenu = 0;

RECOMP_HOOK("gcPauseMenu_update")
void startEvent(s32 gamenum)
{
    if(D_80383010.state != 5)
    {
        inPauseMenu = 0;
        return;
    }

    if(inPauseMenu)
        return;

    inPauseMenu = 1;
}

RECOMP_HOOK_RETURN("gcPauseMenu_update")
void stopEvent(s32 gamenum)
{
    if(D_80383010.state != 5)
    {
        inPauseMenu = 0;
        return;
    }

    if(inPauseMenu == 2)
        D_8036C4E0[D_80383010.selection].unkF = 1;
}

RECOMP_HOOK("func_803183A4")
void onSetTextBox(void *box, char *old)
{
    if(D_80383010.state != 5)
    {
        inPauseMenu = 0;
        return;
    }

    if(inPauseMenu != 1 || box != D_80383010.zoombox[D_80383010.selection])
        return;

    inPauseMenu = 2;

    if(old[0] == 'A')
    {
        char *ne = D_80383010.unk3_6 ? "BIST DU SICHER?" : "A - JA, B - NEIN";
        gczoombox_setStrings(box, 1, &ne);
    }
}
