#include <zephyr/kernel.h>
#include "battery_icon_peripheral.h"
#include "battery_meter.h"

#define BATTERY_ICON_PERIPH_X 80
#define BATTERY_ICON_PERIPH_Y 10

/* No charging indicator here: ZMK's split-central battery relay
 * (zmk_split_central_get_peripheral_battery_level) only carries a charge
 * percentage, not a charging flag - there's no real signal to draw. */

void draw_battery_peripheral_status(lv_obj_t *canvas, const struct status_state *state) {
    uint8_t level = state->battery_p;
    if (level == 0) {
        return;
    }
    draw_battery_meter(canvas, BATTERY_ICON_PERIPH_X, BATTERY_ICON_PERIPH_Y, level);
    draw_battery_percent(canvas, BATTERY_ICON_PERIPH_X, BATTERY_ICON_PERIPH_Y, level);
}
