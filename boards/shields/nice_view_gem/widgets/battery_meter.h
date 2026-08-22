#pragma once

#include <stdio.h>
#include <lvgl.h>
#include "util.h"
#include "../assets/custom_fonts.h"

/* Shared Style-3 battery meter: inverted letter + outline + tip + proportional fill.
 * Fits the historical 60x17 slot used by the old l_/r_battery_* bitmaps. */

#define BATTERY_METER_W 60
#define BATTERY_METER_H 17

#define BATTERY_LETTER_W 15
#define BATTERY_LETTER_BOX 13
#define BATTERY_BODY_X_OFF 16
#define BATTERY_BODY_Y_OFF 2
#define BATTERY_BODY_W 40
#define BATTERY_BODY_H 12
#define BATTERY_TIP_W 3
#define BATTERY_TIP_H 6
#define BATTERY_BORDER 1

/* Black badge with letter punched out (Sharp LCD: FOREGROUND→black glass,
 * BACKGROUND letter→clear glass). */
static inline void draw_battery_letter_inverted(lv_obj_t *canvas, int x, int y,
                                                const char *side_label) {
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_FOREGROUND);
    int box_x = x;
    int box_y = y + 2;
    lv_canvas_draw_rect(canvas, box_x, box_y, BATTERY_LETTER_BOX, BATTERY_LETTER_BOX, &rect_dsc);

    lv_draw_label_dsc_t letter_dsc;
    init_label_dsc(&letter_dsc, LVGL_BACKGROUND, &quinquefive_8, LV_TEXT_ALIGN_CENTER);
    lv_canvas_draw_text(canvas, box_x, box_y + 2, BATTERY_LETTER_BOX, &letter_dsc, side_label);
}

static inline void draw_battery_meter(lv_obj_t *canvas, int x, int y, uint8_t level,
                                      const char *side_label) {
    /* Match prior style 0/1/3 behavior: hide the glyph at <=1%, percent text
     * is drawn by the caller so the reading stays legible. */
    if (level <= 1) {
        return;
    }

    draw_battery_letter_inverted(canvas, x, y, side_label);

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

/* Percent-row bolt: sits under the letter badge, left of the % text. */
static inline void draw_battery_charging_bolt(lv_obj_t *canvas, int x, int y) {
    lv_draw_line_dsc_t line_dsc;
    init_line_dsc(&line_dsc, LVGL_FOREGROUND, 2);

    int ox = x + 2;
    int oy = y + BATTERY_METER_H + 2;
    lv_point_t pts[] = {
        {ox + 3, oy},
        {ox, oy + 5},
        {ox + 3, oy + 5},
        {ox, oy + 10},
    };
    lv_canvas_draw_line(canvas, pts, 4, &line_dsc);
}

static inline void draw_battery_percent(lv_obj_t *canvas, int x, int y, uint8_t level,
                                        bool charging) {
    int pct_y = y + BATTERY_METER_H + 2;
    if (charging) {
        draw_battery_charging_bolt(canvas, x, y);
    }

    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_8, LV_TEXT_ALIGN_LEFT);

    /* Under the letter when idle; shifted right of the bolt when charging. */
    int text_x = charging ? x + 14 : x + BATTERY_LETTER_W;
    int text_w = BATTERY_METER_W - (text_x - x);

    char buf[5];
    snprintf(buf, sizeof(buf), "%d%%", level);
    lv_canvas_draw_text(canvas, text_x, pct_y, text_w, &label_dsc, buf);
}
