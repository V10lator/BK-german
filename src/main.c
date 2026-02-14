#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "dialogs.h"
#include "imports.h"
#include "strhelper.h"

RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_register_replacement(enum asset_e asset_id, void *asset_data));
RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_unregister_replacement(enum asset_e asset_id));

static char new0[0x20];
static char new1[0x20] = "\0";
static char *new[2] = { new0, new1 };
static u32 oldState = 2;

RECOMP_HOOK_RETURN("assetCache_init")
void onInit()
{
    for(int i = 0; i < ASSETS_SIZE; i++)
        bk_recomp_aep_register_replacement(asset_name[i], (void *)asset_data[i]);

    CONTROL_STICK_INSTRUCTIONS = (u8 *)"W[HLE MIT DEM 3D-STICK EIN SPIEL AUS.";
    ERASE_INSTRUCTIONS = (u8 *)"DR]CKE A, UM ZU SPIELEN, ODER DEN Z-TRIGGER, UM DEN SPIELSTAND ZU L\\SCHEN!";
    ERASE_CONFIRMATION = (u8 *)"SICHER? DR]CKE A, UM ZU BEST[TIGEN ODER B, UM ZU WIDERRUFEN.";

    D_8036C4E0[0].str = (u8 *)"ZUR]CK ZUM SPIEL";
    D_8036C4E0[2].str = (u8 *)"STATISTIK";
    D_8036C4E0[3].str = (u8 *)"SICHERN UND ENDE";
}

RECOMP_HOOK_RETURN("setGameInformationZoombox")
void overwriteGKZoombox(s32 gamenum)
{
    u8 *ptr = (u8 *)chGameSelectBottomZoombox;
    ptr += 0x13C;
    char *old0 = *(char **)ptr;
    ptr += sizeof(char *);
    char *old1 = *(char **)ptr;

    old0 += 5;

    my_strcpy(new0, "SPIEL ");
    old0 = my_strcat_till(new0, old0, ' ');
    my_strcat(new0, " ");

    if(my_memcmp((u8 *)++old0, (u8 *)"EMPTY", sizeof("EMPTY") - 1) != 0)
    {
        my_strcat(new0, "LEER");
        new1[0] = '\0';
    }
    else
    {
        old0 += 5;

        my_strcat(new0, "ZEIT ");
        my_strcat_till(new0, old0, ',');
        my_strcat(new0, ",");

        char tmp[20] = { '\0', '\0'};
        old1 = my_strcpy_till(tmp, old1, ' ');
        my_strcpy(new1, tmp);
        my_strcat(new1, " PUZZLETEIL");
        if(tmp[1] != '\0' || tmp[0] != '1')
            my_strcat(new1, "E");

        old1 = my_strcat_till(tmp, old1, ',');
        char *n = my_strstr(old1, 'S');
        if(n)
            *n = 'N';

        strcat(new1, old1);
    }

    func_8031877C(chGameSelectBottomZoombox);
    gczoombox_setStrings(chGameSelectBottomZoombox, 2, new);
    gczoombox_maximize(chGameSelectBottomZoombox);
    gczoombox_resolve_minimize(chGameSelectBottomZoombox);
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
    if(ptr)
    {
        ptr += 0x13C;
        char *old = *(char **)ptr;

        if(old != NULL && old[0] == 'A')
        {
            char *ne;
            if(old[1] == 'R' && old[2] == 'E')
                ne = "BIST DU SICHER?";
            else if(old[1] == ' ' && old[2] == '-')
                ne = "A - JA, B - NEIN";
            else
                return;

            char *n[1] = { ne };
            func_8031877C(box);
            gczoombox_setStrings(box, 1, n);
        }
    }
}
