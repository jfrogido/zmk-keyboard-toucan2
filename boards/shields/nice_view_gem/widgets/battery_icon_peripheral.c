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
#define BATTERY_LETTER_W 15
#define BATTERY_LETTER_BOX 13

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

/* Black badge with letter punched out; covers the baked R in the asset. */
static void draw_inverted_letter(lv_obj_t *canvas) {
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_FOREGROUND);
    int box_x = BATTERY_ICON_PERIPH_X;
    int box_y = BATTERY_ICON_PERIPH_Y + 2;
    lv_canvas_draw_rect(canvas, box_x, box_y, BATTERY_LETTER_BOX, BATTERY_LETTER_BOX, &rect_dsc);

    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_BACKGROUND, &quinquefive_8, LV_TEXT_ALIGN_CENTER);
    lv_canvas_draw_text(canvas, box_x, box_y + 2, BATTERY_LETTER_BOX, &label_dsc, "R");
}

static void draw_percent(lv_obj_t *canvas, uint8_t level) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_8, LV_TEXT_ALIGN_LEFT);

    int pct_y = BATTERY_ICON_PERIPH_Y + BATTERY_ICON_PERIPH_H + 2;
    int text_x = BATTERY_ICON_PERIPH_X + BATTERY_LETTER_W;
    int text_w = BATTERY_ICON_PERIPH_W - BATTERY_LETTER_W;

    char buf[5];
    snprintf(buf, sizeof(buf), "%d%%", level);
    lv_canvas_draw_text(canvas, text_x, pct_y, text_w, &label_dsc, buf);
}

void draw_battery_peripheral_status(lv_obj_t *canvas, const struct status_state *state) {
    uint8_t level = state->battery_p;
    if (level == 0) {
        return;
    }
    draw_icon(canvas, level);
    draw_inverted_letter(canvas);
    draw_percent(canvas, level);
}
