#include <zephyr/kernel.h>
#include <stdio.h>
#include "battery_icon_peripheral.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(r_battery_100);
LV_IMG_DECLARE(r_battery_90);
LV_IMG_DECLARE(r_battery_75);
LV_IMG_DECLARE(r_battery_50);
LV_IMG_DECLARE(r_battery_25);
LV_IMG_DECLARE(r_battery_10);

#define BATTERY_ICON_PERIPH_X 80
#define BATTERY_ICON_PERIPH_Y 10
#define BATTERY_ICON_PERIPH_W 60
#define BATTERY_ICON_PERIPH_H 17

/* No charging indicator here: ZMK's split-central battery relay
 * (zmk_split_central_get_peripheral_battery_level) only carries a charge
 * percentage, not a charging flag - there's no real signal to draw. */

static void draw_icon(lv_obj_t *canvas, uint8_t level) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    if (level > 90) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_PERIPH_X, BATTERY_ICON_PERIPH_Y, &r_battery_100, &img_dsc);
    } else if (level > 75) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_PERIPH_X, BATTERY_ICON_PERIPH_Y, &r_battery_90, &img_dsc);
    } else if (level > 50) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_PERIPH_X, BATTERY_ICON_PERIPH_Y, &r_battery_75, &img_dsc);
    } else if (level > 25) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_PERIPH_X, BATTERY_ICON_PERIPH_Y, &r_battery_50, &img_dsc);
    } else if (level > 10) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_PERIPH_X, BATTERY_ICON_PERIPH_Y, &r_battery_25, &img_dsc);
    } else if (level > 1) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_PERIPH_X, BATTERY_ICON_PERIPH_Y, &r_battery_10, &img_dsc);
    }
}

static void draw_percent(lv_obj_t *canvas, uint8_t level) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_8, LV_TEXT_ALIGN_CENTER);

    char buf[5];
    snprintf(buf, sizeof(buf), "%d%%", level);
    lv_canvas_draw_text(canvas, BATTERY_ICON_PERIPH_X, BATTERY_ICON_PERIPH_Y + BATTERY_ICON_PERIPH_H + 2,
                        BATTERY_ICON_PERIPH_W, &label_dsc, buf);
}

void draw_battery_peripheral_status(lv_obj_t *canvas, const struct status_state *state) {
    uint8_t level = state->battery_p;
    if (level == 0) {
        return;
    }
    draw_icon(canvas, level);
    draw_percent(canvas, level);
}
