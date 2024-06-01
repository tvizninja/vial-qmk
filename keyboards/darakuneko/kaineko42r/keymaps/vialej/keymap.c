/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H

#include "virtser.h"
#include "eeconfig.h"
#include "vial.h"

#include "os_key_override.h"

typedef union {
  uint32_t raw;
  struct {
    uint8_t     key_os_override :1;
  };
} user_config_t;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
		KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
		MT(MOD_LCTL,KC_TAB),KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_ENT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,       MT(MOD_LSFT, KC_SLSH),
		KC_LGUI, KC_LALT, MO(2),   LT(1, KC_SPC),LT(1, KC_BSPC),MO(2),   KC_RALT, KC_RGUI
	),

	[1] = LAYOUT(
		KC_GRV,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_MINS, KC_EQL,  KC_DEL,
		KC_NO,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_BSLS,
		KC_NO,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_NO,   KC_LBRC, KC_RBRC, KC_SCLN, KC_QUOT,
		KC_NO,   MO(3),   KC_NO,   KC_LSFT, KC_LSFT, KC_NO,   MO(3),   KC_NO
	),

	[2] = LAYOUT(
		KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
		KC_NO,   RGB_VAI, RGB_SAI, RGB_HUI, RGB_SPI, RGB_MOD, RGB_TOG, KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   RGB_VAD, RGB_SAD, RGB_HUD, RGB_SPD, RGB_RMOD,KC_NO,   KC_NO,   KC_NO,   KC_NO,   EE_CLR,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   QK_BOOT
	),

	[3] = LAYOUT(
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO
	),

};


led_config_t g_led_config = { {
	// Key Matrix to LED Index
	{ 0, 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, }, 
	{12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, NO_LED, }, 
	{23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, NO_LED,}, 
	{34, 35, 36, 37, 38, NO_LED, NO_LED, 39, 40, 41, NO_LED,NO_LED},
}, {
	{0,0}, {20.36,0}, {40.73,0}, {61.09,0}, {81.45,0}, {101.82,0}, {122.18,0}, {142.55,0}, {162.91,0}, {183.27,0}, {203.64,0}, {224,0}, 
	{5.09,21.33}, {25.45,21.33}, {45.82,21.33}, {66.18,21.33}, {86.54,21.33}, {106.91,21.33}, {127.27,21.33}, {147.64,21.33}, {168,21.33}, {188.36,21.33}, {224,21.33}, 
	{15.27,42.67}, {35.63,42.67}, {56,42.67}, {76.36,42.67}, {96.72,42.67}, {117.09,42.67}, {137.45,42.67}, {157.82,42.67}, {178.18,42.67}, {198.54,42.67}, {224,42.67}, 
	{0,64}, {25.45,64}, {45.82,64}, {101.82,64}, {147.63,64}, {168,64}, {193.45,64}, {224,64}
}, {
  	// LED Index to Flag
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4,
	2, 2, 2, 2, 2, 2, 2, 2
  } 
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
  int is_layer = get_highest_layer(layer_state|default_layer_state);  
  HSV hsv = {0, 255, rgblight_get_val()};
  if (is_layer == 1) {
	hsv.h = 191; //PURPLE
  } else if (is_layer == 2)  {
    hsv.h = 85; //GREEN
  } else if (is_layer == 3)  {
    hsv.h = 43; //YELLOW
  } else {
    hsv.h = 128; //CYAN
  }
  RGB rgb = hsv_to_rgb(hsv);
 
  for (uint8_t i = led_min; i <= led_max; i++) {
    if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
      rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
  }
  return false;
};

user_config_t user_config;

int8_t virtser_send_wrap(uint8_t c) {
    virtser_send(c);
    return 0;
}

void keyboard_pre_init_user(void) {
    print_set_sendchar(virtser_send_wrap);
}


void keyboard_post_init_user(void) {
    os_key_override_init();

    user_config.raw = eeconfig_read_user();
    switch (user_config.key_os_override) {
        case KEY_OS_OVERRIDE_DISABLE:
            remove_all_os_key_overrides();
            break;
        case US_KEY_JP_OS_OVERRIDE_DISABLE:
            register_us_key_on_jp_os_overrides();
            break;
    }
}

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    return true;
}

#define DEFFERED_KEY_RECORD_LEN 6
static keyrecord_t deferred_key_record[DEFFERED_KEY_RECORD_LEN];

static void push_deferred_key_record(uint16_t keycode, keyevent_t *event) {
    for (int i = 0; i < DEFFERED_KEY_RECORD_LEN; i++) {
        if (deferred_key_record[i].keycode == KC_NO) {
            keyrecord_t record     = {.event = *event, .keycode = keycode};
            deferred_key_record[i] = record;
            return;
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // To apply key overrides to keycodes combined shift modifier, separate to two actions
    if (keycode >= QK_MODS && keycode <= QK_MODS_MAX) {
        if (record->event.pressed) {
            register_mods(QK_MODS_GET_MODS(keycode));
            uint16_t   deferred_keycode   = QK_MODS_GET_BASIC_KEYCODE(keycode);
            keyevent_t deferred_key_event = (keyevent_t){.type = KEY_EVENT, .key = (keypos_t){.row = VIAL_MATRIX_MAGIC, .col = VIAL_MATRIX_MAGIC}, .pressed = 1, .time = (timer_read() | 1)};
            push_deferred_key_record(deferred_keycode, &deferred_key_event);
        } else {
            uint16_t   deferred_keycode   = QK_MODS_GET_BASIC_KEYCODE(keycode);
            keyevent_t deferred_key_event = ((keyevent_t){.type = KEY_EVENT, .key = (keypos_t){.row = VIAL_MATRIX_MAGIC, .col = VIAL_MATRIX_MAGIC}, .pressed = 0, .time = (timer_read() | 1)});
            unregister_mods(QK_MODS_GET_MODS(keycode));
            push_deferred_key_record(deferred_keycode, &deferred_key_event);
        }
        return false;
    }

    if (record->event.pressed) {
        switch (keycode) {
            case QK_KB_0:
            remove_all_os_key_overrides();
            user_config.key_os_override = KEY_OS_OVERRIDE_DISABLE;
            eeconfig_update_user(user_config.raw);
            return false;

            case QK_KB_1:
            register_us_key_on_jp_os_overrides();
            user_config.key_os_override = US_KEY_JP_OS_OVERRIDE_DISABLE;
            eeconfig_update_user(user_config.raw);
            return false;
        }
    }

    return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t* record) {
}

void housekeeping_task_user(void) {
    for (int i = 0; i < DEFFERED_KEY_RECORD_LEN; i++) {
        if (deferred_key_record[i].keycode != KC_NO) {
            g_vial_magic_keycode_override = deferred_key_record[i].keycode;
            action_exec(deferred_key_record[i].event);
            deferred_key_record[i].keycode = KC_NO;
        } else {
            break;
        }
    }
}