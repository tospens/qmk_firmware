/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
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

#include "features/achordion.h"
#include "features/caps_word.h"
#include "features/select_word.h"
#include "features/layer_lock.h"

enum custom_keycodes {
  SELWORD = CHARYBDIS_SAFE_RANGE,
  UPDIR,
  JOINLN,
  SRCHSEL,
  LLOCK,
  CBLCK,
};

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif  // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_FUNCTION,
    LAYER_NAVIGATION,
    LAYER_POINTER,
    LAYER_NUMERAL,
    LAYER_SYMBOLR,
    LAYER_SYMBOLL
};

// Automatically enable sniping-mode on the pointer layer.
// #define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif  // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 1
#    endif  // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif      // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define ESC_FUN LT(LAYER_FUNCTION, KC_ESC)
#define SPC_NAV LT(LAYER_NAVIGATION, KC_SPC)
#define TAB_SYM LT(LAYER_SYMBOLL, KC_TAB)
#define ENT_SYM LT(LAYER_SYMBOLR, KC_ENT)
#define BSP_NUM LT(LAYER_NUMERAL, KC_BSPC)
#define _L_PTR(KC) LT(LAYER_POINTER, KC)

#define U_NP KC_NO // Key is not present.
#define U_NA KC_NO // Present but not available for use.
#define U_NU KC_NO // Available but not used.

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

// Danish æ, ø and å keycodes for EurKey.
#define KC_AE ALGR(KC_Q)
#define KC_OE ALGR(KC_L)
#define KC_AA ALGR(KC_W)

#ifdef COMBO_ENABLE
enum combos {
    QW_EXIT,
    MCOMM_AE,
    COMMDOT_OE,
    DOTSLSH_AA,
    CAPS_COMBO,
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM exit_combo[] = { KC_Q, KC_W, COMBO_END };
const uint16_t PROGMEM ae_combo[] = { KC_M, KC_COMM, COMBO_END };
const uint16_t PROGMEM oe_combo[] = { KC_COMM, KC_DOT, COMBO_END };
const uint16_t PROGMEM aa_combo[] = { KC_DOT, KC_SLSH, COMBO_END };
const uint16_t caps_combo[] PROGMEM = {KC_M, KC_V, COMBO_END};

combo_t key_combos[] = {
    [QW_EXIT] = COMBO(exit_combo, LALT(KC_F4)),
    [MCOMM_AE] = COMBO(ae_combo, KC_AE),
    [COMMDOT_OE] = COMBO(oe_combo, KC_OE),
    [DOTSLSH_AA] = COMBO(aa_combo, KC_AA),
    [CAPS_COMBO] = COMBO_ACTION(caps_combo),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
  if (pressed) {
    switch(combo_index) {
      case CAPS_COMBO:
        caps_word_set(true);
        break;

    }
  }
}
#endif

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
    case SPC_NAV:
    case TAB_SYM:
      return 0;  // Bypass Achordion for these keys.
  }

  return 800;  // Otherwise use a timeout of 800 ms.
}

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {
    switch (tap_hold_keycode) {
        case RGUI_T(KC_SCLN): // SCLN + L.
            if (other_keycode == LALT_T(KC_L)) {
                return true;
            }
            break;
        case LCTL_T(KC_D):
            if (other_keycode == KC_W) {
                return true;
            }
            break;
    }

    // Also allow same-hand holds when the other key is in the rows below the
    // alphas.
    if (other_record->event.key.row >= 3) {
        return true;
    }

    // Otherwise, follow the opposite hands rule.
    return achordion_opposite_hands(tap_hold_record, other_record);
}

bool achordion_eager_mod(uint8_t mod) {
  switch (mod) {
    case MOD_LSFT:
    case MOD_RSFT:
    case MOD_LCTL:
    case MOD_RCTL:
      return true;  // Eagerly apply Shift and Ctrl mods.

    default:
      return false;
  }
}

// clang-format off
/** \brief QWERTY layout (3 rows, 10 columns). */
#define LAYOUT_LAYER_BASE                                                                     \
       KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, \
       KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, \
       KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, \
                      ESC_FUN, SPC_NAV, TAB_SYM, ENT_SYM, BSP_NUM

/** Convenience row shorthands. */
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX
#define ______________HOME_ROW_ALGR_L______________ U_NA,    KC_ALGR, U_NA,    U_NA,    U_NA
#define ______________HOME_ROW_GACS_R______________ XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI
#define ______________HOME_ROW_ALGR_R______________ U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA
#define _____________CLIPBOARD_KEYS_L______________ U_UND,   U_CUT,   U_CPY,   U_PST,   U_RDO
#define _____________CLIPBOARD_KEYS_R______________ U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND  
#define _____________WINDOW_MANAGEMENT_____________ U_SLFT,  U_WLFT,  U_WNUP,  U_WRGT,  U_SRGT

#define LAYOUT_LAYER_FUNCTION                                                                 \
    _______________DEAD_HALF_ROW_______________, KC_PSCR,   KC_F7,   KC_F8,   KC_F9,  KC_F12, \
    ______________HOME_ROW_GACS_L______________, KC_SLCK,   KC_F4,   KC_F5,   KC_F6,  KC_F11, \
    ______________HOME_ROW_ALGR_L______________, KC_PAUS,   KC_F1,   KC_F2,   KC_F3,  KC_F10, \
                      _______, XXXXXXX, RESET,   SRCHSEL, XXXXXXX

#define LAYOUT_LAYER_MEDIA                                                                    \
    _______________DEAD_HALF_ROW_______________, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    ______________HOME_ROW_GACS_L______________, KC_MPRV, KC_VOLD, KC_MUTE, KC_VOLU, KC_MNXT, \
    _______________DEAD_HALF_ROW_______________,   RESET, EEP_RST, XXXXXXX, XXXXXXX, LLOCK,   \
                      _______, XXXXXXX, XXXXXXX, KC_MSTP, KC_MPLY

#define LAYOUT_LAYER_POINTER                                                                  \
    _______________DEAD_HALF_ROW_______________, _____________CLIPBOARD_KEYS_R______________, \
    ______________HOME_ROW_GACS_L______________, ______________HOME_ROW_GACS_R______________, \
    XXXXXXX, DRGSCRL, SNIPING, SELWORD, XXXXXXX, DPI_MOD, S_D_MOD, LLOCK,   XXXXXXX, XXXXXXX, \
                      KC_BTN2, KC_BTN1, KC_BTN3, KC_ENT, KC_DEL

#define LAYOUT_LAYER_NAVIGATION                                                               \
    _______________DEAD_HALF_ROW_______________, _____________CLIPBOARD_KEYS_R______________, \
    ______________HOME_ROW_GACS_L______________, SELWORD, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, \
    _____________WINDOW_MANAGEMENT_____________, JOINLN,  KC_HOME, KC_PGDN, KC_PGUP,  KC_END, \
                      XXXXXXX, _______, XXXXXXX, KC_ENT,  KC_BSPC

#define LAYOUT_LAYER_NUMERAL                                                                  \
    KC_EQL,     KC_7,    KC_8,    KC_9, KC_DOT,  _______________DEAD_HALF_ROW_______________, \
    KC_COLN,    KC_4,    KC_5,    KC_6, KC_PLUS, ______________HOME_ROW_GACS_R______________, \
    KC_SLSH,    KC_1,    KC_2,    KC_3, KC_ASTR, ______________HOME_ROW_ALGR_R______________, \
                         KC_BSPC, KC_0, KC_MINS, XXXXXXX, _______

#define LAYOUT_LAYER_SYMBOLS_RIGHT                                                            \
    _______________DEAD_HALF_ROW_______________, KC_GRV,  KC_AMPR, KC_LBRC, KC_RBRC, KC_PERC, \
    ______________HOME_ROW_GACS_L______________, KC_UNDS, KC_PIPE, KC_LPRN, KC_RPRN, KC_AT,   \
    ______________HOME_ROW_ALGR_L______________, KC_TILD, KC_DLR,  KC_LCBR, KC_RCBR, KC_QUES, \
                      XXXXXXX, XXXXXXX, _______, U_NU,    U_NU    

#define LAYOUT_LAYER_SYMBOLS_LEFT                                                             \
    KC_QUOT, KC_LABK, KC_RABK, KC_DQUO, CBLCK,   _______________DEAD_HALF_ROW_______________, \
    KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH, ______________HOME_ROW_GACS_R______________, \
    KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, UPDIR,   ______________HOME_ROW_ALGR_R______________, \
                      U_NU,    U_NU,    KC_UNDS, _______, XXXXXXX

// Add Home Row mod to a layout.
#define _HOME_ROW_MOD_GACS(                                            \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
      LGUI_T(L10), LALT_T(L11), LCTL_T(L12), LSFT_T(L13),        L14,  \
             R15,  RSFT_T(R16), RCTL_T(R17), LALT_T(R18), RGUI_T(R19), \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)

//Add pointer layer keys to a layout.
#define _POINTER_MOD(                                                  \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    L20, L21, L22, L23, L24, R25, R26, R27, R28, R29,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
             L10,         L11,         L12,         L13,         L14,  \
             R15,         R16,         R17,         R18,         R19,  \
      _L_PTR(L20),        L21,         L22,         L23,         L24,  \
             R25,         R26,         R27,         R28,  _L_PTR(R29), \
      __VA_ARGS__
#define POINTER_MOD(...) _POINTER_MOD(__VA_ARGS__)

#define LAYOUT_wrapper(...) LAYOUT_charybdis_3x5(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(
    POINTER_MOD(HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE))
  ),
  [LAYER_FUNCTION] = LAYOUT_wrapper(LAYOUT_LAYER_FUNCTION),
  [LAYER_NAVIGATION] = LAYOUT_wrapper(LAYOUT_LAYER_NAVIGATION),
  //[LAYER_MEDIA] = LAYOUT_wrapper(LAYOUT_LAYER_MEDIA),
  [LAYER_NUMERAL] = LAYOUT_wrapper(LAYOUT_LAYER_NUMERAL),
  [LAYER_POINTER] = LAYOUT_wrapper(LAYOUT_LAYER_POINTER),
  [LAYER_SYMBOLR] = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS_RIGHT),
  [LAYER_SYMBOLL] = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS_LEFT),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif  // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_kb(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_STARTUP_MODE);
#        endif  // RGB_MATRIX_ENABLE
    }
    matrix_scan_user();
}
#    endif  // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_kb(layer_state_t state) {
    state = layer_state_set_user(state);
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif  // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif      // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif

void shutdown_user(void) {
#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_setrgb_red();
#endif  // RGBLIGHT_ENABLE
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_color_all(RGB_RED);
    rgb_matrix_update_pwm_buffers();
#endif  // RGB_MATRIX_ENABLE
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }
  if (!process_caps_word(keycode, record)) { return false; }
  if (!process_select_word(keycode, record, SELWORD)) { return false; }
  if (!process_layer_lock(keycode, record, LLOCK)) { return false; }

  if (record->event.pressed) {
    switch (keycode) {

      case UPDIR:
        SEND_STRING("../");
        return false;
     
      case CBLCK:
        SEND_STRING("```");
        return false;

      case SRCHSEL:  // Searches the current selection in a new tab.
        // Mac users, change LCTL to LGUI.
        SEND_STRING(SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER));
        return false;

      case JOINLN:  // Join lines like Vim's `J` command.
        SEND_STRING( // Go to the end of the line and tap delete.
            SS_TAP(X_END) SS_TAP(X_DEL)
            // In case this has joined two words together, insert one space.
            SS_TAP(X_SPC)
            SS_LCTL(
              // Go to the beginning of the next word.
              SS_TAP(X_RGHT) SS_TAP(X_LEFT)
              // Select back to the end of the previous word. This should select
              // all spaces and tabs between the joined lines from indentation
              // or trailing whitespace, including the space inserted earlier.
              SS_LSFT(SS_TAP(X_LEFT) SS_TAP(X_RGHT)))
            // Replace the selection with a single space.
            SS_TAP(X_SPC));
        return false;

    }
  }

  return true;
}

void matrix_scan_user(void) {
  achordion_task();
}