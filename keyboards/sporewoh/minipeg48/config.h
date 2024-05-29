/*
Copyright 2022 sporewoh

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* key matrix size */
#define MATRIX_ROWS 4
#define MATRIX_COLS 12

/* pin-out for PCB */
#define MATRIX_ROW_PINS { F0, F1, F4, F5 }
#define MATRIX_COL_PINS { B5, B4, D6, D7, D4, D5, D3, D2, D1, D0, B6, C6 }


/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

#define DYNAMIC_KEYMAP_LAYER_COUNT 4

//#define DYNAMIC_MACRO_NO_NESTING
//#define DYNAMIC_MACRO_DELAY 8

#define MOUSEKEY_DELAY 10
#define MOUSEKEY_INTERVAL 20
#define MOUSEKEY_MOVE_DELTA 2
#define MOUSEKEY_MAX_SPEED 16
#define MOUSEKEY_TIME_TO_MAX 30
#define MOUSEKEY_WHEEL_DELAY 10
#define MOUSEKEY_WHEEL_INTERVAL 80
#define MOUSEKEY_WHEEL_DELTA 1
#define MOUSEKEY_WHEEL_MAX_SPEED 8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40
