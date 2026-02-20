#pragma once

#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "structs.h"

// core2/ch/gameSelect.c
extern char *CONTROL_STICK_INSTRUCTIONS;
extern char *ERASE_INSTRUCTIONS;
extern char *ERASE_CONFIRMATION;
extern u8 *chGameSelectBottomZoombox;

// core2/gc/zoombox.c
extern u32 gczoombox_setStrings(void *, s32, char **);
extern void gczoombox_maximize(void *);
extern void gczoombox_minimize(void *);
extern void gczoombox_resolve_minimize(void *);
extern u32 func_8031877C(void *);

// core2/gc/pauseMenu.c
typedef struct {
    f32 delay;
    f32 unk4;
    char *str;
    s16 y;
    u8 portrait;
    u8 unkF;
} struct1As;

typedef struct {
    u8          state;
    u8          menu;
    u8          selection; //menu page
    u8          exit_pause: 1;
    u8          unk3_6: 1; //busy?
    u8          unk3_5: 1;
    u8          unk3_4: 1;
    u8          left_joystick_visible: 1;
    u8          right_joystick_visible: 1;
    u8          b_button_visible: 1;
    u8          unk3_0: 1;
    s8          zoombox_opening_count;
    s8          zoombox_closing_count;
    u8          unk6;
    u8          unk7;
    s8          unk8; //header position
    s8          page;
    s8          joystick_frame;
    u8          joystick_frame_count;
    f32         unkC;
    u8         *zoombox[4];
    f32         unk20;
    void       *joystick_sprite;
    f32         unk28;
    void       *b_button_sprite;
    u8          b_button_frame;
    u8          b_button_frame_count;      //B-button total frames
    s16         b_button_alpha;      //B-button alpha
    s16         left_joystick_alpha;      //left joystick alpha
    s16         right_joystick_alpha;      //right joystick alpha
    u8          page_cnt;
    u8          sns_items;
    u8          sns_visible;
    // u8  pad3B[1];
    s16         sns_alpha; //sns opacity
    s16         unk3E[7];
    s16         unk4C[7];
    // u8 pad5A[0x3];
    void       *sns_egg_model; //SnS Egg Model
    void       *ice_key_model; //Ice key model
    u8          pad64[12];
    u32         unk70_31: 1;
    u32         unk70_30: 1;
    u32         return_to_lair_disabled: 1;
    u32         pad70_28: 29;
} D_80383010_struct;

extern struct1As D_8036C4E0[4];
extern struct1Cs_1 D_8036C58C[0xD];
extern D_80383010_struct D_80383010;

extern u32 func_803183A4(void *, char *);
