/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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
#include "luna.c"
#include <stdio.h>

uint16_t copy_paste_timer;

// SMART BACKSPACE DELETE
bool shift_held = false;
static uint16_t held_shift = 0;

typedef struct {
  bool is_press_action;
  int state;
} tap;

char wpm_str[10];

enum tapdance_types{
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5, //send two single taps
  TRIPLE_TAP = 6,
  TRIPLE_HOLD = 7
};

enum layers {
    QWERTY = 0,
    LOWER,
    RAISE,
    NAV,
    ADJUST,
    NUMPAD
};

enum custom_keycodes {
    KC_CCCV = SAFE_RANGE,
    // Macros, where | is the cursor
    M_LRBRC,  // [|]
    M_LRCBR,  // {|}
    M_LRPRN,  // (|)
    M_LRABR,  // <|>
    M_DQUOT,  // '|'
    KC_BSPC_DEL, // Smart Backspace Delete
};

// Tap Dance declarations
enum {
    TD_AE,
    TD_OE,
    TD_AA,
    TD_COPY_PASTE_APP,
    TD_Y_NUMPAD,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * | ESC    |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  | \   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | Tab    |   A  |   S  |  D   |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  | Lower| CCCV |  |Leader| Raise|   N  |   M  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | ADJ  | GUI  | LCtrl| Space| Enter|  | Enter| Space|BSpace| Del  | Mute |
 *                        | UST  |      |      |      | Alt  |  |      | Nav  |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [QWERTY] = LAYOUT(
      KC_ESC,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                                                     TD(TD_Y_NUMPAD),    KC_U,    KC_I,    KC_O,    KC_P,    TD(TD_AA),
      KC_TAB,  KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                                                     KC_H,    KC_J,    KC_K,    KC_L,    TD(TD_AE), TD(TD_OE),
      KC_LSFT, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,  KC_LGUI, KC_CCCV,      KC_LEAD, _______,     KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                      LGUI(KC_TAB), KC_LCTL, OSL(LOWER), KC_SPC, LALT_T(KC_ENT),   KC_ENT, MO(NAV), OSL(RAISE), KC_BSPC_DEL, KC_MUTE
    ),
/*
 * Lower Layer: Media and Function keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  |                              |  F6  |  F7  |  F8  |  F9  | F10  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | Prev | Play | Next |  F11 |                              | F12  |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [LOWER] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                             KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10, _______,
      _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_F11,                                            KC_F12,  _______, _______, _______,  _______, _______,
      _______, SGUI(KC_LEFT), LGUI(KC_LEFT), LGUI(KC_UP), LGUI(KC_RGHT), SGUI(KC_RIGHT), _______, _______,       _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
    ),
/*
 * Raise Layer: Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  !   |  @   |  #   |  $   |  %   |                              |   ^  |  &   |  *   |  (   |  )   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  `   |  ~   |  _   |  -   |  +   |                              |   =  |  {   |  }   |  [   |  ]   |   ""   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  []  |  {}  |  ()  |  <>  |      |      |      |  |      |      |      |      |  <   |  >   |  ?   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      | Lower|      |      |  |      | Nav  | Raise|      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [RAISE] = LAYOUT(
      _______, KC_EXLM,  KC_AT,    KC_HASH, KC_DLR,  KC_PERC,                                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
      _______, KC_GRAVE, KC_TILDE, KC_UNDS, KC_MINS, KC_PLUS,                                      KC_EQUAL,KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, M_DQUOT,
      _______, M_LRBRC,  M_LRCBR,  M_LRPRN, M_LRABR, _______, _______, _______, _______, _______, _______, _______, KC_LT,   KC_GT,   KC_QUES, KC_CAPS,
                                 TG(ADJUST), _______, _______, _______, _______, _______, TG(NAV), _______, _______, _______
    ),
/*
 * Navigation Layer: Numbers, Arrow Keys and Mouse Keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |   1  |  2   |  3   |  4   |  5   |                              |  6   |  7   |  8   |  9   |  0   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              |      | Left | Up   | Down | Right|        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      | Home | PgUp | PgDn | End  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      | Lower|      |      |  |      | Nav  | Raise|      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [NAV] = LAYOUT(
      _______, KC_1, 	KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
      _______, _______, _______, _______, _______, _______,                                     _______, KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END,  _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Adjust Layer: RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | SAI  | HUI  | VAI  |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      | SAD  | HUD  | VAD  |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [ADJUST] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                      _______, _______, _______, _______, _______, _______,
      _______, _______, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,                                      _______, _______, _______, _______, _______, _______,
      _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 TG(ADJUST), _______, _______, _______, _______, _______, _______, _______, _______, RGB_TOG
    ),
/*
 * Numpad Layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |   Y  |  5   |  8   |  9   |  +   |   *    |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              |   =  |  4   |  5   |  6   |  -   |   /    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |   0  |  1   |  2   |  3   |  ,   |   ,    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [NUMPAD] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                      TD(TD_Y_NUMPAD), KC_P7, KC_P8, KC_P9, KC_PPLS, KC_PAST,
      _______, _______, _______, _______, _______, _______,                                      KC_EQL,          KC_P4, KC_P5, KC_P6, KC_PMNS, KC_PSLS,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  KC_P0,           KC_P1, KC_P2, KC_P3, KC_COMM, KC_PDOT,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_NLCK
    ),
// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  *
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    //state = update_tri_layer_state(state, LOWER, RAISE, ADJUST);
    rgblight_set_layer_state(1, layer_state_cmp(state, LOWER));
    rgblight_set_layer_state(2, layer_state_cmp(state, RAISE));
    rgblight_set_layer_state(3, layer_state_cmp(state, NAV));
    rgblight_set_layer_state(4, layer_state_cmp(state, ADJUST));
    rgblight_set_layer_state(5, layer_state_cmp(state, NUMPAD));
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_CCCV:  // One key copy/paste
            if (record->event.pressed) {
                copy_paste_timer = timer_read();
            } else {
                if (timer_elapsed(copy_paste_timer) > TAPPING_TERM) {  // Hold, copy
                    tap_code16(LCTL(KC_C));
                } else { // Tap, paste
                    tap_code16(LCTL(KC_V));
                }
            }
            break;
        // SMART BACKSPACE DELETE
        case KC_RSFT:
        case KC_LSFT:
            shift_held = record->event.pressed;
            held_shift = keycode;
            break;
        case KC_BSPC_DEL:
            if (record->event.pressed) {
                if (shift_held) {
                    unregister_code(held_shift);
                    register_code(KC_DEL);
                } else {
                    register_code(KC_BSPC);
                }
            } else {
                unregister_code(KC_DEL);
                unregister_code(KC_BSPC);
                if (shift_held) {
                    register_code(held_shift);
                }
            }
            return false;

        case M_LRPRN:
        if (record->event.pressed) {
            SEND_STRING("()" SS_TAP(X_LEFT));
        } else {

        }
        break;
        case M_LRCBR:
        if (record->event.pressed) {
            SEND_STRING("{}" SS_TAP(X_LEFT));
        } else {

        }
        break;
        case M_LRBRC:
        if (record->event.pressed) {
            SEND_STRING("[]" SS_TAP(X_LEFT));
        } else {

        }
        break;
        case M_LRABR:
        if (record->event.pressed) {
            SEND_STRING("<>" SS_TAP(X_LEFT));
        } else {

        }
        break;
        case M_DQUOT:
        if (record->event.pressed) {
            SEND_STRING("\"\"" SS_TAP(X_LEFT));
        } else {

        }
        break;
    }
    return true;
}


bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

LEADER_EXTERNS();

void matrix_scan_user(void) {
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 1000) {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
        }
    }

     LEADER_DICTIONARY() {
        leading = false;
        leader_end();
        SEQ_ONE_KEY(KC_Q) { // Close window
            tap_code16(A(KC_F4));
        }
        SEQ_ONE_KEY(KC_P) { // Invoke Password Manager
            SEND_STRING(SS_LCTL(SS_LALT("\\")));
        }
        SEQ_ONE_KEY(KC_S) { // Windows screenshot
            SEND_STRING(SS_LGUI("\nS"));
        }
        SEQ_ONE_KEY(KC_M) { // Mute Teams
            SEND_STRING(SS_LCTL(SS_LSFT("M")));
        }
        SEQ_TWO_KEYS(KC_E, KC_W) { // Email work
            SEND_STRING("tospmo@dsb.dk");
        }
        SEQ_TWO_KEYS(KC_E, KC_P) { // Email personal
            SEND_STRING("tsmogensen@gmail.com");
        }
     }
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

void oled_white_space(void){
  oled_write_P(PSTR(" "), false);
}
/* static void render_kyria_logo(void) {
    static const char PROGMEM kyria_logo[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
        0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
        0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };
    oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
} */

static void render_qmk_logo(void) {
  static const char PROGMEM qmk_logo[] = {
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

  oled_write_P(qmk_logo, false);
}



void render_mod_state(uint8_t modifiers) {
  oled_write_P(PSTR("\nMods: "), false);
  oled_write_P(PSTR("SHF"), (modifiers & MOD_MASK_SHIFT));
  oled_white_space();
  oled_write_P(PSTR("CTL"), (modifiers & MOD_MASK_CTRL));
  oled_white_space();
  oled_write_P(PSTR("ALT"), (modifiers & MOD_MASK_ALT));
  oled_white_space();
  oled_write_P(PSTR("GUI"), (modifiers & MOD_MASK_GUI));
}

static void render_status(void) {
    // QMK Logo and version information
    render_qmk_logo();
    oled_write_P(PSTR("       Tobi's Kyria\n\n"), false);

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case QWERTY:
            oled_write_P(PSTR("Default"), false);
            break;
        case LOWER:
            oled_write_P(PSTR("Lower"), false);
            break;
        case RAISE:
            oled_write_P(PSTR("Raise"), false);
            break;
        case NAV:
            oled_write_P(PSTR("Navigation"), false);
            break;
        case ADJUST:
            oled_write_P(PSTR("Adjust"), false);
            break;
        case NUMPAD:
            oled_write_P(PSTR("Numpad"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined"), false);
    }

    render_mod_state(get_mods()|get_oneshot_mods());

    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK)    ? PSTR("NUMLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK)   ? PSTR("CAPLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)

    } else {
        sprintf(wpm_str, "       WPM: %03d", get_current_wpm());
        oled_write(wpm_str, false);
        animate_luna();
    }
}
#endif

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        switch (biton32(layer_state)) {
            case QWERTY:
                // History scrubbing. For Adobe products, hold shift while moving
                // backward to go forward instead.
                if (clockwise) {
                    tap_code16(C(KC_Y));
                } else {
                    tap_code16(C(KC_Z));
                }
                break;
            case RAISE:
                // Browser tabbing
                if (clockwise) {
                    tap_code16(C(KC_TAB));
                    clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
                } else {
                    tap_code16(S(C(KC_TAB)));
                    clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
                }
                break;
            case ADJUST: // Underglow color
                if (clockwise) {
                    rgblight_increase_hue();
                } else {
                    rgblight_decrease_hue();
                }
                break;
            default:
                // Switch between windows on Windows with alt tab
                if (clockwise) {
                    if (!is_alt_tab_active) {
                        is_alt_tab_active = true;
                        register_code(KC_LALT);
                    }
                    alt_tab_timer = timer_read();
                    tap_code16(KC_TAB);
                    clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
                } else {
                    tap_code16(S(KC_TAB));
                    clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
                }
                break;
        }
    } else if (index == 1) {
        switch (biton32(layer_state)) {
            case QWERTY:
                // Volume control
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
                break;
            case LOWER: // Scrolling horizontally by word
                if (clockwise) {
                    tap_code16(C(KC_RGHT));
                    clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
                } else {
                    tap_code16(C(KC_LEFT));
                    clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
                }
                break;
            case ADJUST: // Underglow brightness
                if (clockwise) {
                    rgblight_increase_val();
                } else {
                    rgblight_decrease_val();
                }
                break;
            default:
                // Scrolling with Mouse Wheel Up and Down
                if (clockwise) {
                    tap_code(KC_WH_D);
                } else {
                    tap_code(KC_WH_U);
                }
                break;

        }
    }
}
#endif

//// BEGIN: Advanced Tap Dances
int cur_dance (qk_tap_dance_state_t *state) {
   if (state->count == 1) {
    if (state->interrupted || !state->pressed)  return SINGLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     * keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  //Assumes no one is trying to type the same letter three times (at least not quickly).
  //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
  //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8; //magic number. At some point this method will expand to work for more presses
}

// BEGIN: Copy, Paste, Apps
// https://beta.docs.qmk.fm/features/feature_tap_dance#example-6-using-tap-dance-for-momentary-layer-switch-and-layer-toggle-keys
/* static tap copy_paste_app_tap_state = {
  .is_press_action = true,
  .state = 0
};

void copy_paste_app_finished (qk_tap_dance_state_t *state, void *user_data) {
  copy_paste_app_tap_state.state = cur_dance(state);
  switch (copy_paste_app_tap_state.state) {
    case SINGLE_TAP:
      tap_code16(LGUI(KC_V)); // Tap Cmd + V
      break;
    case SINGLE_HOLD:
      tap_code16(LGUI(KC_C)); // Hold Cmd + C
      break;
    case DOUBLE_TAP:
      SEND_STRING(SS_DOWN(X_LGUI) SS_TAP(X_SPACE) SS_UP(X_LGUI));
      wait_ms(250);
      SEND_STRING("line\n");
      break;
    case TRIPLE_TAP:
      SEND_STRING(SS_DOWN(X_LGUI) SS_TAP(X_SPACE) SS_UP(X_LGUI));
      wait_ms(250);
      SEND_STRING("itunes\n");
      break;
  }
}

void copy_paste_app_reset (qk_tap_dance_state_t *state, void *user_data) {
  copy_paste_app_tap_state.state = 0;
} */
// END: Copy, Paste, Apps

// BEGIN: Y, NUMPAD
static tap y_numpad_tap_state = {
  .is_press_action = true,
  .state = 0
};

void y_numpad_finished (qk_tap_dance_state_t *state, void *user_data) {
  y_numpad_tap_state.state = cur_dance(state);
  switch (y_numpad_tap_state.state) {
    case SINGLE_TAP:
      tap_code(KC_Y);
      break;
    case SINGLE_HOLD:
      register_code16(KC_Y);
      break;
    case DOUBLE_TAP:
      if (layer_state_is(NUMPAD)) {
        layer_off(NUMPAD);
      } else {
        layer_on(NUMPAD);
      }
      break;
  }
}

void y_numpad_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (y_numpad_tap_state.state) {
    case SINGLE_HOLD:
      unregister_code16(KC_Y);
      break;
  }
  y_numpad_tap_state.state = 0;
}
// END: Y, NUMPAD

//// END: Advanced Tap Dances


//Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_AE] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, ALGR(KC_Q)),
    [TD_OE] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, ALGR(KC_L)),
    [TD_AA] = ACTION_TAP_DANCE_DOUBLE(KC_BSLS, ALGR(KC_W)),

// Advanced Tap Dances
   //[TD_COPY_PASTE_APP] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, copy_paste_app_finished, copy_paste_app_reset, 300),
    [TD_Y_NUMPAD] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, y_numpad_finished, y_numpad_reset, 300),
};

// Light LEDs 6 to 9 and 12 to 15 red when caps lock is active. Hard to ignore!
const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {18, 1, HSV_RED}     // Light 4 LEDs, starting with LED 12
);
// Light LEDs 9 & 10 in blue when keyboard lower layer is active
const rgblight_segment_t PROGMEM lower_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, 150, 255, 255},
    {7, 1, 150, 255, 255}
);
// Light LEDs 11 & 12 in orange when keyboard raise layer is active
const rgblight_segment_t PROGMEM raise_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {10, 3, HSV_RAISE},
    {17, 1, HSV_RAISE}
);
// Light LEDs 13 & 14 in green when keyboard navigation layer is active
const rgblight_segment_t PROGMEM nav_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {10, 3, HSV_PURPLE},
    {17, 1, HSV_PURPLE}
);
// Light LEDs 13 & 14 in green when keyboard adjust layer is active
const rgblight_segment_t PROGMEM adjust_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {7, 1, HSV_MAGENTA}
);
const rgblight_segment_t PROGMEM numpad_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {10, 3, HSV_BLUE},
    {17, 1, HSV_BLUE}
);

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_capslock_layer,
    lower_layer,    // Overrides caps lock layer
    raise_layer,    // Overrides other layers
    nav_layer,     // Overrides other layers
    adjust_layer,
    numpad_layer
);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = rgb_layers;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(0, led_state.caps_lock);
    return true;
}
