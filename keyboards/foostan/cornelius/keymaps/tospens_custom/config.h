// Copyright 2019 Manna Harbour
// https://github.com/manna-harbour/miryoku
// generated -*- buffer-read-only: t -*-

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#define BOOTMAGIC_LITE_ROW 0
#define BOOTMAGIC_LITE_COLUMN 0

#define U_NP KC_NO // key is not present
#define U_NA KC_NO // present but not available for use
#define U_NU KC_NO // available but not used

#define TAPPING_TERM 180

// Enable permissive hold for faster hold activation on mods.
#define PERMISSIVE_HOLD

// Replace TAPPING_FORCE_HOLD with Flow Tap
// #define FLOW_TAP_TERM 150

// Enable Chordal Hold - replaces Achordion for opposite-hands rule.
// Tap-hold keys are settled as tapped when both keys are on the same hand.
#define CHORDAL_HOLD

// Recommended for heavy chording.
#define QMK_KEYS_PER_SCAN 4

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
#define U_DRGT LCTL(LGUI(KC_RGHT))
#define U_DLFT LCTL(LGUI(KC_LEFT))
#define U_DSHW LGUI(KC_TAB)
