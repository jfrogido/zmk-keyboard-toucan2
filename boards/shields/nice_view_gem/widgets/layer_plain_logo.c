#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <stdio.h>
#include <string.h>

#include "layer_plain_logo.h"
#include "../assets/custom_fonts.h"
#include <zmk/physical_layouts.h>
#include <zmk/keymap.h>
#include <zmk/matrix.h>

/* Only the name is drawn here - the animated Kubernetes wheel above it is a
 * real lv_animimg object created once in zmk_widget_screen_init(), not a
 * canvas draw call, so it can animate independently of state-change redraws. */
void draw_layer_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_18, LV_TEXT_ALIGN_CENTER);

    char fallback_layer_name[16];

    const char *layer_name = zmk_keymap_layer_name(zmk_keymap_layer_index_to_id(state->layer_index));

    if (layer_name == NULL || layer_name[0] == '\0') {
        sprintf(fallback_layer_name, "L#%" PRIu8, state->layer_index);

        layer_name = fallback_layer_name;
    }

    // Sits below the native 72px-tall wheel logo, which spans y=40 to y=112.
    lv_canvas_draw_text(canvas, 0, 116, SCREEN_WIDTH, &label_dsc, layer_name);
}
