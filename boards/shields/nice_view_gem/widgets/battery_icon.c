#include <zephyr/kernel.h>
#include <stdio.h>
#include "battery_icon.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(l_battery_100);
LV_IMG_DECLARE(l_battery_90);
LV_IMG_DECLARE(l_battery_75);
LV_IMG_DECLARE(l_battery_50);
LV_IMG_DECLARE(l_battery_25);
LV_IMG_DECLARE(l_battery_10);

#define BATTERY_ICON_X 8
#define BATTERY_ICON_Y 10
#define BATTERY_ICON_W 60
#define BATTERY_ICON_H 17
#define BATTERY_LETTER_W 15
#define BATTERY_LETTER_BOX 13

static void draw_icon(lv_obj_t *canvas, uint8_t level) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    if (level > 90) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_X, BATTERY_ICON_Y, &l_battery_100, &img_dsc);
    } else if (level > 75) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_X, BATTERY_ICON_Y, &l_battery_90, &img_dsc);
    } else if (level > 50) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_X, BATTERY_ICON_Y, &l_battery_75, &img_dsc);
    } else if (level > 25) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_X, BATTERY_ICON_Y, &l_battery_50, &img_dsc);
    } else if (level > 10) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_X, BATTERY_ICON_Y, &l_battery_25, &img_dsc);
    } else if (level > 1) {
        lv_canvas_draw_img(canvas, BATTERY_ICON_X, BATTERY_ICON_Y, &l_battery_10, &img_dsc);
    }
    /* At <=1% no icon is drawn, matching the existing style 0/1 battery.c
     * behavior. draw_percent() below still shows the real number, so the
     * reading stays legible even once the icon itself disappears. */
}

/* Black badge with letter punched out (Sharp LCD: FOREGROUND→black glass,
 * BACKGROUND letter→clear glass). Covers the baked L in the asset. */
static void draw_inverted_letter(lv_obj_t *canvas) {
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_FOREGROUND);
    int box_x = BATTERY_ICON_X;
    int box_y = BATTERY_ICON_Y + 2;
    lv_canvas_draw_rect(canvas, box_x, box_y, BATTERY_LETTER_BOX, BATTERY_LETTER_BOX, &rect_dsc);

    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_BACKGROUND, &quinquefive_8, LV_TEXT_ALIGN_CENTER);
    lv_canvas_draw_text(canvas, box_x, box_y + 2, BATTERY_LETTER_BOX, &label_dsc, "L");
}

static void draw_percent(lv_obj_t *canvas, uint8_t level, bool charging) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_8, LV_TEXT_ALIGN_LEFT);

    int pct_y = BATTERY_ICON_Y + BATTERY_ICON_H + 2;
    /* Under the letter when idle; shifted right of the bolt when charging. */
    int text_x = charging ? BATTERY_ICON_X + 14 : BATTERY_ICON_X + BATTERY_LETTER_W;
    int text_w = BATTERY_ICON_W - (text_x - BATTERY_ICON_X);

    char buf[5];
    snprintf(buf, sizeof(buf), "%d%%", level);
    lv_canvas_draw_text(canvas, text_x, pct_y, text_w, &label_dsc, buf);
}

/* Percent-row bolt: sits under the L badge, left of the % text. */
static void draw_charging_bolt(lv_obj_t *canvas) {
    lv_draw_line_dsc_t line_dsc;
    init_line_dsc(&line_dsc, LVGL_FOREGROUND, 2);

    int ox = BATTERY_ICON_X + 2;
    int oy = BATTERY_ICON_Y + BATTERY_ICON_H + 2;
    lv_point_t pts[] = {
        {ox + 3, oy},
        {ox, oy + 5},
        {ox + 3, oy + 5},
        {ox, oy + 10},
    };
    lv_canvas_draw_line(canvas, pts, 4, &line_dsc);
}

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    draw_icon(canvas, state->battery);
    draw_inverted_letter(canvas);
    if (state->charging) {
        draw_charging_bolt(canvas);
    }
    draw_percent(canvas, state->battery, state->charging);
}
