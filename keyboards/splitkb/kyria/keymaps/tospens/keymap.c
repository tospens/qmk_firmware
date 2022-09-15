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
//#include "luna.c"
//#include <stdio.h>

#ifdef PIMORONI_TRACKBALL_ENABLE
#include "pimoroni_trackball.h"
#endif

#ifdef POINTING_DEVICE_ENABLE
#include "pointing_device.h"
#endif

// ONE KEY COPY-PASTE
uint16_t copy_paste_timer;

// SUPER ALT-TAB
bool is_alt_tab_active = false;

// SMART BACKSPACE DELETE
bool shift_held = false;
static uint16_t held_shift = 0;

char wpm_str[10];

#ifdef COMBO_ENABLE
enum combos {
    QW_EXIT,
    SD_RUN
};

const uint16_t PROGMEM exit_combo[] = { KC_Q, KC_W, COMBO_END };
const uint16_t PROGMEM  run_combo[] = { KC_S, KC_D, COMBO_END };

combo_t key_combos[COMBO_COUNT] = {
    [QW_EXIT] = COMBO(exit_combo, LALT(KC_F4)),
    [SD_RUN]  = COMBO(run_combo,  LALT(KC_SPC))
};
#endif

enum layers {
    QWERTY = 0,
    LOWER,
    RAISE,
    NAV,
    ADJUST
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
    ALT_TAB, // Super Alt-Tab
};

// Tap Dance declarations
enum {
    TD_AE,
    TD_OE,
    TD_AA
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
      KC_ESC,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                                                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    TD(TD_AA),
      KC_TAB,  KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                                                     KC_H,    KC_J,    KC_K,    KC_L,    TD(TD_AE), TD(TD_OE),
      KC_LSFT, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,  KC_LGUI, KC_CCCV,      KC_LEAD, KC_RALT,     KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                      LGUI(KC_TAB), KC_LCTL, MO(LOWER), KC_SPC, LALT_T(KC_ENT),   KC_ENT, MO(NAV), MO(RAISE), KC_BSPC_DEL, KC_MUTE
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
      ALT_TAB, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_F11,                                            KC_F12,  _______, _______, _______,  _______, _______,
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

#ifdef RGBLIGHT_ENABLE
layer_state_t layer_state_set_user(layer_state_t state) {
    //state = update_tri_layer_state(state, LOWER, RAISE, ADJUST);
    // Super Alt-Tab
    if (is_alt_tab_active) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
    };
    rgblight_set_layer_state(1, layer_state_cmp(state, LOWER));
    rgblight_set_layer_state(2, layer_state_cmp(state, RAISE));
    rgblight_set_layer_state(3, layer_state_cmp(state, NAV));
    rgblight_set_layer_state(4, layer_state_cmp(state, ADJUST));
    return state;
}
#endif

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
        case ALT_TAB: // Super Alt-Tab
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            break;
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

LEADER_EXTERNS();

void matrix_scan_user(void) {

     LEADER_DICTIONARY() {
        leading = false;
        leader_end();
        SEQ_ONE_KEY(KC_S) { // Windows screenshot
            SEND_STRING(SS_LGUI("\nS"));
        }
        SEQ_ONE_KEY(KC_M) { // Mute Teams
            SEND_STRING(SS_LCTL(SS_LSFT("M")));
        }
        SEQ_TWO_KEYS(KC_E, KC_W) { // Email work
            SEND_STRING("tospmo@dsb.dk");
        }
        SEQ_ONE_KEY(KC_E) { // Email personal
            SEND_STRING("tsmogensen@gmail.com");
        }
     }
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

void oled_white_space(void){
  oled_write_P(PSTR(" "), false);
}
/*
static void render_tospens_logo(void) {
    static const char PROGMEM tospens_logo[] = {
        // 'tospens_logo', 128x18px
        0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
        0xf0, 0xfe, 0xff, 0x1f, 0x03, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0xff, 0xff, 0xff,
        0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x0f, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x87, 0x07, 0x07, 0x07, 0x07, 0x07, 0xff,
        0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x87, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf8, 0xe0, 0x80, 0x00, 0xff, 0xff, 0xff,
        0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x01, 0x03, 0x01, 0x01, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xfe, 0xff,
        0x3f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x80, 0x80, 0x80, 0x80, 0x80,
        0xc0, 0xff, 0xff, 0xff, 0x00, 0x00, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x87,
        0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x03, 0x03, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x87, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83,
        0x83, 0x83, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x07, 0x1f, 0x7f, 0xff, 0xff, 0xff, 0xff,
        0xff, 0x00, 0x00, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x87, 0xff, 0xff, 0xff,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x03, 0x03, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03,
        0x03, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03
    };
    oled_write_raw_P(tospens_logo, sizeof(tospens_logo));
}
*/

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
        default:
            oled_write_P(PSTR("Undefined"), false);
    }

    render_mod_state(get_mods()/*|get_oneshot_mods()*/);

    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK)    ? PSTR("NUMLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK)   ? PSTR("CAPLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);
}

/*static void render_logo(void) {
    render_tospens_logo();
    //sprintf(wpm_str, "       WPM: %03d", get_current_wpm());
    //oled_write(wpm_str, false);
}
*/

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        //render_logo();
        //animate_luna();
    }
    return false;
}
#endif

#ifdef PIMORONI_TRACKBALL_ENABLE
void pointing_device_task() {
    report_mouse_t mouse_report = pointing_device_get_report();
    if (is_keyboard_master()) {
		bool fast_scroll = (get_highest_layer(layer_state) == LOWER);
		process_mouse(&mouse_report, fast_scroll);
    }

	if (host_keyboard_led_state().caps_lock) {
		trackball_set_rgbw(150,0,0,0);
	}
	else {
		switch (get_highest_layer(layer_state)) {
			case QWERTY:
				 trackball_set_rgbw(0,0,0,150);
				break;
			case RAISE:
				trackball_set_rgbw(255,165,0,0);
				break;
			case LOWER:
				trackball_set_rgbw(0,0,150,0);
				break;
			case NAV:
				 trackball_set_rgbw(130,0,170,0);
				break;
			case ADJUST:
				 trackball_set_rgbw(0,150,0,0);
				break;
			default:
				trackball_set_rgbw(0,0,0,150);
		}
    }

    if (layer_state_is(NAV)) {
        trackball_set_scrolling(true);
    } else {
        trackball_set_scrolling(false);
    }

    pointing_device_set_report(mouse_report);
    pointing_device_send();
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
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
            case RAISE: // Browser tabbing
                if (clockwise) {
                    tap_code16(C(KC_TAB));
                } else {
                    tap_code16(S(C(KC_TAB)));
                }
                break;
            case ADJUST: // Underglow color
                if (clockwise) {
                    rgblight_increase_hue();
                } else {
                    rgblight_decrease_hue();
                }
                break;
            default: // Scrolling horizontally by word
                if (clockwise) {
                    tap_code16(C(KC_RGHT));
                } else {
                    tap_code16(C(KC_LEFT));
                }
                break;
        }
    } else if (index == 1) {
        switch (biton32(layer_state)) {
            case QWERTY: // Volume control
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
                break;
            case LOWER: // Scrolling with Mouse Wheel Up and Down
                if (clockwise) {
                    tap_code(KC_WH_D);
                } else {
                    tap_code(KC_WH_U);
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

                break;

        }
    }
    return false;
}
#endif

//Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_AE] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, ALGR(KC_Q)),
    [TD_OE] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, ALGR(KC_L)),
    [TD_AA] = ACTION_TAP_DANCE_DOUBLE(KC_BSLS, ALGR(KC_W)),
};

#ifdef RGBLIGHT_ENABLE
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
    {10, 3, HSV_SPRINGGREEN},
    {17, 1, HSV_SPRINGGREEN}
);
// Light LEDs 13 & 14 in green when keyboard adjust layer is active
const rgblight_segment_t PROGMEM adjust_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {7, 1, HSV_MAGENTA}
);


// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_capslock_layer,
    lower_layer,    // Overrides caps lock layer
    raise_layer,    // Overrides other layers
    nav_layer,     // Overrides other layers
    adjust_layer
);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = rgb_layers;
    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
    rgblight_sethsv_noeeprom(0, 0, 100);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(0, led_state.caps_lock);
    return true;
}
#endif
