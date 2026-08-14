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

static void draw_percent(lv_obj_t *canvas, uint8_t level) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_8, LV_TEXT_ALIGN_CENTER);

    char buf[5];
    snprintf(buf, sizeof(buf), "%d%%", level);
    lv_canvas_draw_text(canvas, BATTERY_ICON_X, BATTERY_ICON_Y + BATTERY_ICON_H + 2, BATTERY_ICON_W,
                        &label_dsc, buf);
}

static void draw_charging_bolt(lv_obj_t *canvas) {
    lv_draw_line_dsc_t line_dsc;
    init_line_dsc(&line_dsc, LVGL_FOREGROUND, 2);

    int ox = BATTERY_ICON_X + BATTERY_ICON_W + 5;
    int oy = BATTERY_ICON_Y + 2;
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
    draw_percent(canvas, state->battery);
    if (state->charging) {
        draw_charging_bolt(canvas);
    }
}
