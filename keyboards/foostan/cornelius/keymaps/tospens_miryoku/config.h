// Copyright 2019 Manna Harbour
// https://github.com/manna-harbour/miryoku
// generated -*- buffer-read-only: t -*-

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

// Set Bootmagic key to Q
#define BOOTMAGIC_LITE_ROW 0
#define BOOTMAGIC_LITE_COLUMN 1

#define U_NP KC_NO // key is not present
#define U_NA KC_NO // present but not available for use
#define U_NU KC_NO // available but not used

// default but used in macros
#undef TAPPING_TERM
#define TAPPING_TERM 180

// Prevent normal rollover on alphas from accidentally triggering mods.
#define IGNORE_MOD_TAP_INTERRUPT

// Enable permissive hold for faster hold activation on mods.
#define PERMISSIVE_HOLD

// Enable rapid switch from tap to hold, disables double tap hold auto-repeat.
#define TAPPING_FORCE_HOLD

// Auto Shift
#define NO_AUTO_SHIFT_ALPHA
#define AUTO_SHIFT_TIMEOUT TAPPING_TERM
#define AUTO_SHIFT_NO_SETUP

// Recommended for heavy chording.
#define QMK_KEYS_PER_SCAN 4

// Mouse key speed and acceleration.
#undef MOUSEKEY_DELAY
#define MOUSEKEY_DELAY          0
#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL       16
#undef MOUSEKEY_WHEEL_DELAY
#define MOUSEKEY_WHEEL_DELAY    0
#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED      6
#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX    64

// Clipboard keycodes.
#define U_RDO C(KC_Y)
#define U_PST C(KC_V)
#define U_CPY C(KC_C)
#define U_CUT C(KC_X)
#define U_UND C(KC_Z)

// Window management keycodes.
#define U_SLFT SGUI(KC_LEFT)
#define U_WLFT LGUI(KC_LEFT)
#define U_WNUP LGUI(KC_UP)
#define U_WRGT LGUI(KC_RGHT)
#define U_SRGT SGUI(KC_RGHT)


#define XXXX KC_NO

#define LAYOUT_miryoku( \
	K000, K001, K002, K003, K004, K005, K006, K007, K008, K009, \
	K100, K101, K102, K103, K104, K105, K106, K107, K108, K109, \
	K200, K201, K202, K203, K204, K205, K206, K207, K208, K209, \
	N300, N301, K302, K303, K304, K305, K306, K307, N308, N309  \
) { \
	{ XXXX,  K000,  K001,  K002,  K003,  K004,  K005,  K006,  K007,  K008,  K009,  XXXX }, \
	{ XXXX,  K100,  K101,  K102,  K103,  K104,  K105,  K106,  K107,  K108,  K109,  XXXX }, \
	{ XXXX,  K200,  K201,  K202,  K203,  K204,  K205,  K206,  K207,  K208,  K209,  XXXX }, \
	{ XXXX,  XXXX,  XXXX,  K302,  K303,  K304,  K305,  K306,  K307,  XXXX,  XXXX,  XXXX } \
}
