// Copyright 2024 sekigon-gonnoc
/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "bmp.h"
#include "bmp_custom_keycodes.h"

uint8_t set_scrolling = false;

enum {
    DRAG_SCROLL = BMP_SAFE_RANGE,
};

// Disable BMP dynamic matrix size
#undef MATRIX_ROWS
#define MATRIX_ROWS MATRIX_ROWS_DEFAULT
#undef MATRIX_COLS
#define MATRIX_COLS MATRIX_COLS_DEFAULT

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_INT3, KC_BSPC,
                KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_ENT,
                KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NUHS,
                KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_INT1, KC_UP, KC_RSFT,
                MO(1), KC_GRV, KC_LGUI, KC_LALT, KC_INT5, KC_SPC, KC_SPC, KC_INT4, KC_INT2, KC_RALT, MO(1), KC_LEFT, KC_DOWN, KC_RGHT),
    [1] = LAYOUT(KC_PWR, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_INS, KC_DEL,
                KC_CAPS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2, KC_PSCR, KC_SCRL, KC_PAUS, KC_UP, KC_TRNS, KC_TRNS,
                KC_TRNS, KC_VOLD, KC_VOLU, KC_MUTE, KC_EJCT, KC_TRNS, KC_PAST, KC_PSLS, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT, KC_TRNS,
                KC_TRNS, AD_WO_L, SEL_BLE, SEL_USB, KC_TRNS, KC_TRNS, KC_PPLS, KC_PMNS, KC_END, KC_PGDN, KC_DOWN, KC_TRNS, KC_RSFT, KC_TRNS,
                KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_DEL, KC_LGUI, KC_RCTL)
};

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (set_scrolling) {
        mouse_report.h = mouse_report.x;
        mouse_report.v = -mouse_report.y;
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == DRAG_SCROLL) {
        set_scrolling = record->event.pressed;
    }

    return true;
}