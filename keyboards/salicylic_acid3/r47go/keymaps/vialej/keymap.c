/* Copyright 2021 ai03
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

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
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_MINS, KC_DEL,
        KC_CAPS,      KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,      KC_ENT,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M, KC_COMM,  KC_DOT,  KC_RSFT,  MO(1),
        KC_LCTL, KC_LGUI, KC_LALT,        KC_SPC,         KC_SPC,         KC_SPC,     KC_LEFT, KC_DOWN,    KC_UP,KC_RGHT
    ),
    [1] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,        _______,
        _______,       _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______,
        _______, _______, _______,       _______,        _______,        _______,     _______, _______,  _______,   _______
    ),
    [2] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,        _______,
        _______,       _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______,
        _______, _______, _______,       _______,        _______,        _______,     _______, _______,  _______,   _______
    ),
    [3] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
        _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,        _______,
        _______,       _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______,
        _______, _______, _______,       _______,        _______,        _______,     _______, _______,  _______,   _______
    )
};

user_config_t user_config;

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