#include "recompui.h"
#include "recomputils.h"

#define BODY_PADDING 64.0f
#define MODAL_HEIGHT 15.0f // (RECOMPUI_TOTAL_HEIGHT - (2 * BODY_PADDING))
#define MODAL_MAX_WIDTH ((RECOMPUI_TOTAL_HEIGHT - (2 * BODY_PADDING)) * (16.0f / 9.0f))
#define MODAL_BORDER_WIDTH 1.1f
#define MODAL_BORDER_RADIUS 16.0f

static const RecompuiColor bg_color = { .r = 255, .g = 255, .b = 255, .a = 0.1f * 255 };
static const RecompuiColor border_color = { .r = 255, .g = 255, .b = 255, .a = 0.2f * 255 };
static const RecompuiColor modal_color = { .r = 8, .g = 7, .b = 13, .a = 0.9f * 255 };
static RecompuiContext context;
static RecompuiResource button;
static s32 visible = 0;

void button_pressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (resource != button || data->type != UI_EVENT_CLICK)
        return;

    if(!visible)
        visible = 1;
    else
    {
        recompui_hide_context(context);
        visible = 0;
    }
}

void showError(const char *error) {
    recomp_printf(error);

    context = recompui_create_context();
    recompui_open_context(context);

    RecompuiResource res = recompui_context_root(context);
    // Set up the root element so it takes up the full screen.
    recompui_set_position(res, POSITION_ABSOLUTE);
    recompui_set_top(res, 0, UNIT_DP);
    recompui_set_right(res, 0, UNIT_DP);
    recompui_set_bottom(res, 0, UNIT_DP);
    recompui_set_left(res, 0, UNIT_DP);
    recompui_set_width_auto(res);
    recompui_set_height_auto(res);

    // Set up the root element's padding so the modal contents don't touch the screen edges.
    recompui_set_padding(res, BODY_PADDING, UNIT_DP);
    recompui_set_background_color(res, &bg_color);

    // Set up the flexbox properties of the root element.
    recompui_set_flex_direction(res, FLEX_DIRECTION_COLUMN);
    recompui_set_justify_content(res, JUSTIFY_CONTENT_CENTER);
    recompui_set_align_items(res, ALIGN_ITEMS_CENTER);

    // Create a container to act as the modal background and hold the elements in the modal.
    RecompuiResource container = recompui_create_element(context, res);
    recompui_set_justify_content(container, JUSTIFY_CONTENT_CENTER);
    recompui_set_align_items(container, ALIGN_ITEMS_CENTER);
    recompui_set_text_align(container, TEXT_ALIGN_CENTER);

    // Take up the a fixed height and the full width, up to a maximum width.
    recompui_set_height(container, MODAL_HEIGHT, UNIT_PERCENT);
    recompui_set_flex_grow(container, 1.0f);
    recompui_set_max_width(container, MODAL_MAX_WIDTH, UNIT_DP);
    recompui_set_width(container, 100.0f, UNIT_PERCENT);

    // Set up the properties of the container.
    recompui_set_display(container, DISPLAY_FLEX);
    recompui_set_justify_content(container, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_flex_direction(container, FLEX_DIRECTION_COLUMN);
    recompui_set_padding(container, 16.0f, UNIT_DP);
    recompui_set_gap(container, 16.0f, UNIT_DP);
    recompui_set_align_items(container, ALIGN_ITEMS_FLEX_START);

    // Set up the container to be the modal's background.
    recompui_set_border_width(container, MODAL_BORDER_WIDTH, UNIT_DP);
    recompui_set_border_radius(container, MODAL_BORDER_RADIUS, UNIT_DP);
    recompui_set_border_color(container, &border_color);
    recompui_set_background_color(container, &modal_color);

    // Create the two rows in the container.
    res = recompui_create_element(context, container);
    recompui_set_flex_basis(res, 100.0f, UNIT_DP);
    recompui_set_flex_grow(res, 0);
    recompui_set_display(res, DISPLAY_FLEX);
    recompui_set_flex_direction(res, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(res, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_align_items(res, ALIGN_ITEMS_FLEX_END);
    recompui_set_gap(res, 16.0f, UNIT_DP);

    recompui_create_label(context, res, error, LABELSTYLE_NORMAL);

    // Create some buttons.
    button = recompui_create_button(context, container, "OK", BUTTONSTYLE_PRIMARY);
    recompui_set_text_align(button, TEXT_ALIGN_CENTER);
    recompui_register_callback(button, button_pressed, NULL);

    recompui_close_context(context);
    recompui_show_context(context);
}
