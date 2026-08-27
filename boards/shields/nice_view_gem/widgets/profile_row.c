#include <zephyr/kernel.h>
#include "profile_row.h"
#include <zmk/ble.h>

#define BT_ROW_X       68
#define BT_ROW_Y       143
#define BT_DOT_SIZE    10
#define BT_DOT_SPACING 16
#define BT_N_PROFILES  5
#define BT_ACTIVE_MARGIN 2

static void draw_filled_square(lv_obj_t *canvas, int x, int y, int w, int h) {
    lv_draw_rect_dsc_t dsc;
    init_rect_dsc(&dsc, LVGL_FOREGROUND);
    lv_canvas_draw_rect(canvas, x, y, w, h, &dsc);
}

static void draw_outlined_square(lv_obj_t *canvas, int x, int y, int w, int h) {
    lv_draw_rect_dsc_t dsc;
    lv_draw_rect_dsc_init(&dsc);
    dsc.bg_color = LVGL_BACKGROUND;
    dsc.border_color = LVGL_FOREGROUND;
    dsc.border_width = 1;
    lv_canvas_draw_rect(canvas, x, y, w, h, &dsc);
}

/* Connected/bonded state is queried directly at draw time (not cached in
 * status_state) since zmk_ble_profile_is_connected/is_open take an explicit
 * slot index and already exist for every slot, not just the active one - no
 * new event plumbing needed. active_profile_index comes from status_state
 * so the currently selected output slot gets a highlight frame - otherwise
 * two simultaneously-connected profiles are visually identical. */
void draw_profile_status(lv_obj_t *canvas, const struct status_state *state) {
    for (int i = 0; i < BT_N_PROFILES; i++) {
        int x = BT_ROW_X + i * BT_DOT_SPACING;

        if (i == state->active_profile_index) {
            draw_outlined_square(canvas, x - BT_ACTIVE_MARGIN, BT_ROW_Y - BT_ACTIVE_MARGIN,
                                  BT_DOT_SIZE + BT_ACTIVE_MARGIN * 2, BT_DOT_SIZE + BT_ACTIVE_MARGIN * 2);
        }

        if (zmk_ble_profile_is_connected(i)) {
            draw_filled_square(canvas, x, BT_ROW_Y, BT_DOT_SIZE, BT_DOT_SIZE);
        } else if (!zmk_ble_profile_is_open(i)) {
            /* Bonded to this slot, but not the one currently connected. */
            draw_outlined_square(canvas, x, BT_ROW_Y, BT_DOT_SIZE, BT_DOT_SIZE);
            draw_filled_square(canvas, x + BT_DOT_SIZE / 2 - 1, BT_ROW_Y + BT_DOT_SIZE / 2 - 1, 2, 2);
        } else {
            /* Slot has never been bonded. */
            draw_outlined_square(canvas, x, BT_ROW_Y, BT_DOT_SIZE, BT_DOT_SIZE);
        }
    }
}
