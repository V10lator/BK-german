#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "dialogs.h"

RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_register_replacement(enum asset_e asset_id, void *asset_data));
RECOMP_IMPORT("bk_recomp_asset_expansion_pak", void bk_recomp_aep_unregister_replacement(enum asset_e asset_id));

RECOMP_HOOK_RETURN("assetCache_init")
void onInit()
{
    for(int i = 0; i < ASSETS_SIZE; i++)
        bk_recomp_aep_register_replacement(asset_name[i], (void *)asset_data[i]);

    recomp_printf("German mod: %i assets loaded!\n", ASSETS_SIZE);
}
/* TODO
RECOMP_CALLBACK("*", recomp_exit)
void on_deinit()
{
    for(int i = 0; i < ASSETS_SIZE; i++)
        bk_recomp_aep_unregister_replacement(asset_name[i]);
}
*/
