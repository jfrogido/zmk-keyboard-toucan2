#pragma once

#include <stdio.h>
#include <lvgl.h>
#include "util.h"
#include "../assets/custom_fonts.h"

/* Shared Style-3 battery meter: letter + outline + tip + proportional fill.
 * Fits the historical 60x17 slot used by the old l_/r_battery_* bitmaps. */

#define BATTERY_METER_W 60
#define BATTERY_METER_H 17

#define BATTERY_LETTER_W 14
#define BATTERY_BODY_X_OFF 16
#define BATTERY_BODY_Y_OFF 2
#define BATTERY_BODY_W 40
#define BATTERY_BODY_H 12
#define BATTERY_TIP_W 3
#define BATTERY_TIP_H 6
#define BATTERY_BORDER 1

static inline void draw_battery_meter(lv_obj_t *canvas, int x, int y, uint8_t level,
                                      const char *side_label) {
    /* Match prior style 0/1/3 behavior: hide the glyph at <=1%, percent text
     * is drawn by the caller so the reading stays legible. */
    if (level <= 1) {
        return;
    }

    lv_draw_label_dsc_t letter_dsc;
    init_label_dsc(&letter_dsc, LVGL_FOREGROUND, &quinquefive_8, LV_TEXT_ALIGN_LEFT);
    lv_canvas_draw_text(canvas, x, y + 4, BATTERY_LETTER_W, &letter_dsc, side_label);

    const int bx = x + BATTERY_BODY_X_OFF;
    const int by = y + BATTERY_BODY_Y_OFF;

    lv_draw_rect_dsc_t outline;
    lv_draw_rect_dsc_init(&outline);
    outline.bg_color = LVGL_BACKGROUND;
    outline.border_color = LVGL_FOREGROUND;
    outline.border_width = BATTERY_BORDER;
    lv_canvas_draw_rect(canvas, bx, by, BATTERY_BODY_W, BATTERY_BODY_H, &outline);

    lv_draw_rect_dsc_t tip;
    init_rect_dsc(&tip, LVGL_FOREGROUND);
    lv_canvas_draw_rect(canvas, bx + BATTERY_BODY_W,
                        by + (BATTERY_BODY_H - BATTERY_TIP_H) / 2, BATTERY_TIP_W, BATTERY_TIP_H,
                        &tip);

    const int inner_w = BATTERY_BODY_W - (2 * BATTERY_BORDER);
    const int inner_h = BATTERY_BODY_H - (2 * BATTERY_BORDER);
    uint8_t capped = level > 100 ? 100 : level;
    int fill_w = (capped * inner_w) / 100;
    if (fill_w > 0) {
        lv_draw_rect_dsc_t fill;
        init_rect_dsc(&fill, LVGL_FOREGROUND);
        lv_canvas_draw_rect(canvas, bx + BATTERY_BORDER, by + BATTERY_BORDER, fill_w, inner_h,
                            &fill);
    }
}

static inline void draw_battery_percent(lv_obj_t *canvas, int x, int y, uint8_t level) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_8, LV_TEXT_ALIGN_CENTER);

    char buf[5];
    snprintf(buf, sizeof(buf), "%d%%", level);
    lv_canvas_draw_text(canvas, x, y + BATTERY_METER_H + 2, BATTERY_METER_W, &label_dsc, buf);
}
