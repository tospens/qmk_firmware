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

#include "features/caps_word.h"

// ONE KEY COPY-PASTE
uint16_t copy_paste_timer;

#ifdef COMBO_ENABLE
enum combos {
    QW_EXIT,
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM exit_combo[] = { KC_Q, KC_W, COMBO_END };

combo_t key_combos[] = {
    [QW_EXIT] = COMBO(exit_combo, LALT(KC_F4))
};
#endif

enum layers { BASE, BUTTON, MEDIA, NAV, MOUSE, SYM, NUM, FUN };

enum custom_keycodes {
    KC_CCCV = SAFE_RANGE,
    // Macros, where | is the cursor
    M_LRBRC,  // [|]
    M_LRCBR,  // {|}
    M_LRPRN,  // (|)
    M_LRABR,  // <|>
    M_DQUOT,  // '|'
};

// Left-hand home row mods
#define HOME_A LGUI_T(KC_A)
#define HOME_S LALT_T(KC_S)
#define HOME_D LCTL_T(KC_D)
#define HOME_F LSFT_T(KC_F)
#define HOME_X ALGR_T(KC_X)

// Right-hand home row mods
#define HOME_J RSFT_T(KC_J)
#define HOME_K RCTL_T(KC_K)
#define HOME_L LALT_T(KC_L)
#define HOME_SCLN RGUI_T(KC_SCLN)
#define HOME_DOT ALGR_T(KC_DOT)

// Danish æ, ø and å keycodes for EurKey
#define KC_AE ALGR(KC_Q)
#define KC_OE ALGR(KC_L)
#define KC_AA ALGR(KC_W)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [BASE] = LAYOUT(
      U_NU,  KC_Q,              KC_W,    KC_E,              KC_R,              KC_T,                   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    U_NU,
      U_NU,  HOME_A,            HOME_S,  HOME_D,            HOME_F,            KC_G,                   KC_H,              HOME_J,            HOME_K,          HOME_L,   HOME_SCLN, U_NU,
      U_NU, LT(BUTTON, KC_Z),  HOME_X,  KC_C,              KC_V,              KC_B,  U_NU, U_NU,     U_NU, U_NU,     KC_N,    KC_M,    KC_COMM, HOME_DOT, LT(BUTTON, KC_SLSH), U_NU,
                      LGUI(KC_TAB), LT(MEDIA, KC_ESC), LT(NAV, KC_SPC),   LT(MOUSE, KC_TAB), KC_CCCV, KC_LEAD, LT(SYM, KC_ENT),   LT(NUM, KC_BSPC),  LT(FUN, KC_DEL),  KC_MUTE
    ),

    [NAV] = LAYOUT(
      _______, RESET,   U_NA,    U_NA,    U_NA,    U_NA,                            U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND,   _______,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, U_NA,                            KC_CAPS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
      _______, U_SLFT,  U_WLFT,  U_WNUP,  U_WRGT,  U_SRGT, U_NU, U_NU, U_NU, U_NU,  KC_INS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______,
               U_NP,    U_NP,    U_NA,    U_NA,    U_NA,   U_NP, KC_ENT,  KC_BSPC, KC_DEL,  U_NP
    ),

    [MOUSE] = LAYOUT(
      _______, RESET,   U_NA,    U_NA,    U_NA,    U_NA,                            U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND,   _______,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, U_NA,                            U_NU,    KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______,
      _______, U_NA,    KC_ALGR, U_NA,    U_NA,    U_NA, U_NU, U_NU, U_NU, U_NU,    U_NU,    KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, _______,
               U_NP,    U_NP,    U_NA,    U_NA,    U_NA,                            U_NP,    KC_BTN1, KC_BTN3, KC_BTN2, U_NP
    ),

    [MEDIA] = LAYOUT(
      _______, RESET,   U_NA,    U_NA,    U_NA,    U_NA,                                     RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, _______,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, U_NA,                                     U_NU,    KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, _______,
      _______, U_NA,    KC_ALGR, U_NA,    U_NA,    U_NA, _______, _______, _______, _______, U_NU,    DT_PRNT, DT_DOWN, DT_UP,   U_NU,    _______,
                              U_NU,    U_NU,    U_NA,    U_NA,    U_NU,    U_NU,    KC_MSTP, KC_MPLY, KC_MUTE, U_NU 
    ),

    [NUM] = LAYOUT(
      _______, KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC,                                     U_NA,    U_NA,    U_NA,    U_NA,    RESET,   _______,
      _______, KC_QUOT, KC_4,    KC_5,    KC_6,    KC_EQL,                                      U_NA,    KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, _______,
      _______, KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS, _______, _______, _______, _______, U_NA,    U_NA,    KC_AE,   KC_OE,   KC_AA,   _______,
                                 U_NU, KC_DOT,  KC_0,    KC_MINS, U_NU, U_NU,    U_NA,    U_NA,    U_NA,    U_NU
    ),

    [SYM] = LAYOUT(
      _______, KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR,                                     U_NA,    U_NA,    U_NA,    U_NA,    RESET,   _______,
      _______, KC_DQT,  KC_DLR,  KC_PERC, KC_CIRC, KC_PLUS,                                     U_NA,    KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, _______,
      _______, KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_PIPE, _______, _______, _______, _______, U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA,    _______,
                                 U_NU, KC_LPRN, KC_RPRN, KC_UNDS, U_NU, U_NA,    U_NA,    U_NA,    U_NU,    U_NU
    ),

    [FUN] = LAYOUT(
      _______, KC_F12,  KC_F7,   KC_F8,   KC_F9,   KC_PSCR,                                     U_NA,    U_NA,    U_NA,    U_NA,    RESET,   _______,
      _______, KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_SLCK,                                     U_NA,    KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, _______,
      _______, KC_F10,  KC_F1,   KC_F2,   KC_F3,   KC_PAUS, _______, _______, _______, _______, U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA,    _______,
                                 U_NU, KC_APP,  KC_SPC,  KC_TAB, U_NU, U_NA,    U_NA,    U_NA,    U_NU,    U_NU
    ),

    [BUTTON] = LAYOUT(
      _______, U_UND,   U_CUT,   U_CPY,   U_PST,   U_RDO,                                     U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND,   _______,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_TRNS,                                   KC_TRNS, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, _______,
      _______, U_UND,   U_CUT,   U_CPY,   U_PST,   U_RDO, _______, _______, _______, _______, U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND,    _______,
                                 U_NU, KC_BTN2, KC_BTN3, KC_BTN1, U_NU, U_NU,    KC_BTN1, KC_BTN3, KC_BTN2,    U_NU
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
        case BASE:
            oled_write_P(PSTR("Base"), false);
            break;
        case NAV:
            oled_write_P(PSTR("Navigation"), false);
            break;
        case MOUSE:
            oled_write_P(PSTR("Mouse"), false);
            break;
        case MEDIA:
            oled_write_P(PSTR("Media"), false);
            break;
        case NUM:
            oled_write_P(PSTR("Number"), false);
            break;
        case SYM:
            oled_write_P(PSTR("Symbol"), false);
            break;
        case FUN:
            oled_write_P(PSTR("Function"), false);
            break;
        case BUTTON:
            oled_write_P(PSTR("Button"), false);
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

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        switch (biton32(layer_state)) {
            case BASE:
                // History scrubbing. For Adobe products, hold shift while moving
                // backward to go forward instead.
                if (clockwise) {
                    tap_code16(C(KC_Y));
                } else {
                    tap_code16(C(KC_Z));
                }
                break;
            case NUM: // Browser tabbing
                if (clockwise) {
                    tap_code16(C(KC_TAB));
                } else {
                    tap_code16(S(C(KC_TAB)));
                }
                break;
            case FUN: // Underglow color
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
            case BASE: // Volume control
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
                break;
            case NAV: // Scrolling with Mouse Wheel Up and Down
                if (clockwise) {
                    tap_code(KC_WH_D);
                } else {
                    tap_code(KC_WH_U);
                }
                break;
            case MEDIA: // Underglow brightness
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

#ifdef RGBLIGHT_ENABLE

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
    rgblight_sethsv_noeeprom(0, 0, 100);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}

#endif
