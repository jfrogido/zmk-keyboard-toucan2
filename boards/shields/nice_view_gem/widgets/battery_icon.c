#include <zephyr/kernel.h>
#include "battery_icon.h"
#include "battery_meter.h"

#define BATTERY_ICON_X 8
#define BATTERY_ICON_Y 10

static void draw_charging_bolt(lv_obj_t *canvas) {
    lv_draw_line_dsc_t line_dsc;
    init_line_dsc(&line_dsc, LVGL_FOREGROUND, 2);

    int ox = BATTERY_ICON_X + BATTERY_METER_W + 5;
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
    draw_battery_meter(canvas, BATTERY_ICON_X, BATTERY_ICON_Y, state->battery, "L");
    draw_battery_percent(canvas, BATTERY_ICON_X, BATTERY_ICON_Y, state->battery);
    if (state->charging) {
        draw_charging_bolt(canvas);
    }
}
