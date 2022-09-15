#include QMK_KEYBOARD_H

#include "features/achordion.h"
#include "features/caps_word.h"
#include "features/select_word.h"

enum custom_keycodes {
  SELWORD = SAFE_RANGE,
};

// Left-hand home row mods.
#define HOME_A LGUI_T(KC_A)
#define HOME_S LALT_T(KC_S)
#define HOME_D LCTL_T(KC_D)
#define HOME_F LSFT_T(KC_F)
#define HOME_X ALGR_T(KC_X) 

// Right-hand home row mods.
#define HOME_J RSFT_T(KC_J)
#define HOME_K RCTL_T(KC_K)
#define HOME_L LALT_T(KC_L)
#define HOME_SCLN RGUI_T(KC_SCLN)
#define HOME_DOT ALGR_T(KC_DOT)

// Danish æ, ø and å keycodes for EurKey.
#define KC_AE ALGR(KC_Q)
#define KC_OE ALGR(KC_L)
#define KC_AA ALGR(KC_W)

/** Convenience row shorthands. */
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX
#define ______________HOME_ROW_ALGR_L______________ U_NA,    KC_ALGR, U_NA,    U_NA,    U_NA
#define ______________HOME_ROW_GACS_R______________ XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI
#define ______________HOME_ROW_ALGR_R______________ U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA
#define _____________CLIPBOARD_KEYS_L______________ U_UND,   U_CUT,   U_CPY,   U_PST,   U_RDO
#define _____________CLIPBOARD_KEYS_R______________ U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND  
#define _____________WINDOW_MANAGEMENT_____________ U_SLFT,  U_WLFT,  U_WNUP,  U_WRGT,  U_SRGT

#ifdef COMBO_ENABLE
enum combos {
    QW_EXIT,
    MCOMM_AE,
    COMMDOT_OE,
    DOTSLSH_AA,
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM exit_combo[] = { KC_Q, KC_W, COMBO_END };
const uint16_t PROGMEM ae_combo[] = { KC_M, KC_COMM, COMBO_END };
const uint16_t PROGMEM oe_combo[] = { KC_COMM, KC_DOT, COMBO_END };
const uint16_t PROGMEM aa_combo[] = { KC_DOT, KC_SLSH, COMBO_END };

combo_t key_combos[] = {
    [QW_EXIT] = COMBO(exit_combo, LALT(KC_F4)),
    [MCOMM_AE] = COMBO(ae_combo, KC_AE),
    [COMMDOT_OE] = COMBO(oe_combo, KC_OE),
    [DOTSLSH_AA] = COMBO(aa_combo, KC_AA)
};
#endif

enum layers { BASE, BUTTON, MEDIA, NAV, MOUSE, SYM, NUM, FUN };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT_miryoku(
    KC_Q,              KC_W,    KC_E,              KC_R,              KC_T,              KC_Y,              KC_U,              KC_I,            KC_O,     KC_P,
    HOME_A,            HOME_S,  HOME_D,            HOME_F,            KC_G,              KC_H,              HOME_J,            HOME_K,          HOME_L,   HOME_SCLN,
    LT(BUTTON, KC_Z),  HOME_X,  KC_C,              KC_V,              KC_B,              KC_N,              KC_M,              KC_COMM,         KC_DOT,   KC_SLSH,
    U_NP,              U_NP,    LT(MEDIA, KC_ESC), LT(NAV, KC_SPC),   LT(MOUSE, KC_TAB), LT(SYM, KC_ENT),   LT(NUM, KC_BSPC),  LT(FUN, KC_DEL), U_NP,     U_NP
  ),
  [NAV] = LAYOUT_miryoku(
    RESET,   U_NA,    U_NA,    U_NA,    U_NA,    U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND,
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, U_NA,    KC_CAPS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
    U_SLFT,  U_WLFT,  U_WNUP,  U_WRGT,  U_SRGT,  KC_INS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,
    U_NP,    U_NP,    U_NA,    U_NA,    U_NA,    KC_ENT,  KC_BSPC, KC_DEL,  U_NP,    U_NP
  ),
  [MOUSE] = LAYOUT_miryoku(
    RESET,   U_NA,    U_NA,    U_NA,    U_NA,    U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND,
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, U_NA,    U_NU,    KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R,
    U_NA,    KC_ALGR, SELWORD, U_NA,    U_NA,    U_NU,    KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R,
    U_NP,    U_NP,    U_NA,    U_NA,    U_NA,    KC_BTN1, KC_BTN3, KC_BTN2, U_NP,    U_NP
  ),
  [MEDIA] = LAYOUT_miryoku(
    RESET,   U_NA,    U_NA,    U_NA,    U_NA,    RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI,
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, U_NA,    U_NU,    KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT,
    U_NA,    KC_ALGR, U_NA,    U_NA,    U_NA,    U_NU,    DT_PRNT, DT_DOWN, DT_UP,   U_NU,
    U_NP,    U_NP,    U_NA,    U_NA,    U_NA,    KC_MSTP, KC_MPLY, KC_MUTE, U_NP,    U_NP
  ),
  [NUM] = LAYOUT_miryoku(
    KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC, U_NA,    U_NA,    U_NA,    U_NA,    RESET,
    KC_QUOT, KC_4,    KC_5,    KC_6,    KC_EQL,  U_NA,    KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI,
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS, U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA,
    U_NP,    U_NP,    KC_DOT,  KC_0,    KC_MINS, U_NA,    U_NA,    U_NA,    U_NP,    U_NP
  ),
  [SYM] = LAYOUT_miryoku(
    KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR, U_NA,    U_NA,    U_NA,    U_NA,    RESET,
    KC_DQT,  KC_DLR,  KC_PERC, KC_CIRC, KC_PLUS, U_NA,    KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI,
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_PIPE, U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA,
    U_NP,    U_NP,    KC_LPRN, KC_RPRN, KC_UNDS, U_NA,    U_NA,    U_NA,    U_NP,    U_NP
  ),
  [FUN] = LAYOUT_miryoku(
    KC_F12,  KC_F7,   KC_F8,   KC_F9,   KC_PSCR, U_NA,    U_NA,    U_NA,    U_NA,    RESET,
    KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_SLCK, U_NA,    KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI,
    KC_F10,  KC_F1,   KC_F2,   KC_F3,   KC_PAUS, U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA,
    U_NP,    U_NP,    KC_APP,  KC_SPC,  KC_TAB,  U_NA,    U_NA,    U_NA,    U_NP,    U_NP
  ),
  [BUTTON] = LAYOUT_miryoku(
    U_UND,   U_CUT,   U_CPY,   U_PST,   U_RDO,   U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND,
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_TRNS, KC_TRNS, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI,
    U_UND,   U_CUT,   U_CPY,   U_PST,   U_RDO,   U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND,
    U_NP,    U_NP,    KC_BTN2, KC_BTN3, KC_BTN1, KC_BTN1, KC_BTN3, KC_BTN2, U_NP,    U_NP
  )
};

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

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }
  if (!process_caps_word(keycode, record)) { return false; }
  if (!process_select_word(keycode, record, SELWORD)) { return false; }
  return true;
}

void matrix_scan_user(void) {
  achordion_task();
}