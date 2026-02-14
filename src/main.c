#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "dialogs.h"

RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_register_replacement(enum asset_e asset_id, void *asset_data));
RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_unregister_replacement(enum asset_e asset_id));

// core2/ch/gameSelect.c
extern u8 *CONTROL_STICK_INSTRUCTIONS;
extern u8 *ERASE_INSTRUCTIONS;
extern u8 *ERASE_CONFIRMATION;

// core2/gc/pauseMenu.c
typedef struct {
    f32 delay;
    f32 unk4;
    u8 *str;
    s16 y;
    u8 portrait;
    u8 unkF;
} struct1As;

extern struct1As D_8036C4E0[4];

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
/* TODO
RECOMP_CALLBACK("*", recomp_exit)
void on_deinit()
{
    for(int i = 0; i < ASSETS_SIZE; i++)
        bk_recomp_aep_unregister_replacement(asset_name[i]);
}
*/
