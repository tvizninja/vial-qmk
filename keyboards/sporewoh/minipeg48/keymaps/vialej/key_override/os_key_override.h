// Copyright 2023 sekigon-gonnoc
/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#ifndef NULL
#define NULL 0
#endif

typedef enum {
    KEY_OS_OVERRIDE_DISABLE,
    US_KEY_JP_OS_OVERRIDE_DISABLE,
} key_os_override_t;

#include "process_key_override.h"

// helper macro
#define w_shift(kc, kc_override) &ko_make_basic(MOD_MASK_SHIFT, kc, kc_override)
#define wo_shift(kc, kc_override)                            \
    &ko_make_with_layers_and_negmods(0, kc, kc_override, ~0, \
                                     (uint8_t)MOD_MASK_SHIFT)

int register_os_key_override(const key_override_t *override);
void remove_all_os_key_overrides(void);

void os_key_override_init(void);
void register_us_key_on_jp_os_overrides(void);
