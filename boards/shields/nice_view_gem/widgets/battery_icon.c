#include <zephyr/kernel.h>
#include "battery_icon.h"
#include "battery_meter.h"

#define BATTERY_ICON_X 8
#define BATTERY_ICON_Y 10

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    draw_battery_meter(canvas, BATTERY_ICON_X, BATTERY_ICON_Y, state->battery);
    draw_battery_percent(canvas, BATTERY_ICON_X, BATTERY_ICON_Y, state->battery);
    if (state->charging) {
        draw_battery_charging_bolt(canvas, BATTERY_ICON_X, BATTERY_ICON_Y);
    }
}
