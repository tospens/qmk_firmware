#include QMK_KEYBOARD_H

#include "features/achordion.h"
#include "features/caps_word.h"
#include "features/select_word.h"

enum custom_keycodes {
  SELWORD = SAFE_RANGE,
  UPDIR,
  CBLCK,
  JOINLN,
  SRCHSEL,
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

// Left-hand thumb keys.
#define U_MEDIA LT(MEDIA, KC_ESC)
#define U_NAV LT(NAV, KC_SPC)
#define U_SYMR LT(SYMR, KC_TAB)

// Right-hand thumb keys.
#define U_SYML LT(SYML, KC_ENT)
#define U_NUM LT(NUM, KC_BSPC)
#define U_FUN LT(FUN, KC_DEL)

// Danish æ, ø and å keycodes for EurKey.
#define KC_AE ALGR(KC_Q)
#define KC_OE ALGR(KC_L)
#define KC_AA ALGR(KC_W)

// Convenience row shorthands.
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX
#define ______________HOME_ROW_ALGR_L______________ U_NA,    KC_ALGR, U_NA,    U_NA,    U_NA
#define ______________HOME_ROW_GACS_R______________ XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI
#define ______________HOME_ROW_ALGR_R______________ U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA
#define _____________CLIPBOARD_KEYS_L______________ U_UND,   U_CUT,   U_CPY,   U_PST,   U_RDO
#define _____________CLIPBOARD_KEYS_R______________ U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND  
#define _____________WINDOW_MANAGEMENT_____________ U_SLFT,  U_WLFT,  U_WNUP,  U_WRGT,  U_SRGT

enum layers { BASE, MEDIA, NAV, MOUSE, SYML, SYMR, NUM, FUN };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT(
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,     KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,      KC_BSLS,
    KC_ESC,  HOME_A,  HOME_S,  HOME_D,  HOME_F, KC_G,     KC_H,   HOME_J, HOME_K,  HOME_L,  HOME_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    HOME_X,  KC_C,    KC_V,   KC_B,     KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH,   KC_RSFT,
    KC_LCTL, KC_DOWN, KC_UP,   U_MEDIA, U_NAV,  U_SYMR,   U_SYML, U_NUM,  U_FUN,   KC_LEFT, KC_RGHT,   SRCHSEL
  ),
  [NAV] = LAYOUT(
    U_NA, U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND,   U_NU,
    U_NA, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, U_NA,    SELWORD, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, U_NU,
    U_NA, U_SLFT,  U_WLFT,  U_WNUP,  U_WRGT,  U_SRGT,  JOINLN,  KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_CAPS,
    U_NA, U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    KC_ENT,  KC_BSPC, KC_DEL,  U_DLFT,  U_DRGT,  U_NU
  ),
  [MEDIA] = LAYOUT(
    U_NA, U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    U_NU,    U_NU,    U_NU,    U_NU,    U_NU,    U_NU,
    U_NA, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, U_NA,    U_NU,    KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, U_NU,
    U_NA, U_NA,    KC_ALGR, U_NA,    U_NA,    U_NA,    U_NU,    DT_PRNT, DT_DOWN, DT_UP,   U_NU,    U_NU,
    U_NA, U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    KC_MSTP, KC_MPLY, KC_MUTE, U_NU,    U_NU,    U_NU
  ),
  [NUM] = LAYOUT(
    U_NA, KC_EQL,  KC_7,    KC_8,    KC_9,    KC_DOT,  U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    U_NA,
    U_NA, KC_COLN, KC_4,    KC_5,    KC_6,    KC_PLUS, U_NA,    KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, U_NA,
    U_NA, KC_SLSH, KC_1,    KC_2,    KC_3,    KC_ASTR, U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA,    U_NA,
    U_NA, U_NA,    U_NA,    KC_BSPC, KC_0,    KC_MINS, U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    U_NA
  ),
  [SYML] = LAYOUT(
    U_NU, KC_QUOT, KC_LABK, KC_RABK, KC_DQUO, CBLCK,   U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    U_NA,
    U_NU, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH, U_NA,    KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, U_NA,
    U_NU, KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, UPDIR,   U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA,    U_NA,
    U_NU, U_NU,    U_NU,    U_NU,    KC_UNDS, U_NU,    U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    U_NA
  ),
  [SYMR] = LAYOUT(
    U_NA, U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    KC_GRV,  KC_AMPR,  KC_LBRC, KC_RBRC, KC_PERC, U_NU,
    U_NA, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, U_NA,    KC_UNDS, KC_PIPE, KC_LPRN, KC_RPRN, KC_AT,    U_NU,
    U_NA, U_NA,    KC_ALGR, U_NA,    U_NA,    U_NA,    KC_TILD, KC_DLR , KC_LCBR, KC_RCBR, KC_QUES,  U_NU,
    U_NA, U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    U_NU, U_NU, SELWORD, U_NU,    U_NU,    U_NU
  ),
  [FUN] = LAYOUT(
    U_NU, KC_F12,  KC_F7,   KC_F8,   KC_F9,   KC_PSCR, U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    RESET,
    U_NU, KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_SLCK, U_NA,    KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, U_NA,
    U_NU, KC_F10,  KC_F1,   KC_F2,   KC_F3,   KC_PAUS, U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA,    U_NA,
    U_NU, U_NU,    U_NU,    KC_APP,  KC_SPC,  KC_TAB,  U_NA,    U_NA,    U_NA,    U_NA,    U_NA,    U_NA
  )
};

const uint32_t unicode_map[] PROGMEM = {};

#ifdef COMBO_ENABLE
enum combos {
    QW_EXIT,
    MCOMM_AE,
    COMMDOT_OE,
    DOTSLSH_AA,
    CAPS_COMBO,
    END_SENTENCE_COMBO,
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM exit_combo[] = { KC_Q, KC_W, COMBO_END };
const uint16_t PROGMEM ae_combo[] = { KC_M, KC_COMM, COMBO_END };
const uint16_t PROGMEM oe_combo[] = { KC_COMM, KC_DOT, COMBO_END };
const uint16_t PROGMEM aa_combo[] = { KC_DOT, KC_SLSH, COMBO_END };
const uint16_t caps_combo[] PROGMEM = {KC_E, KC_I, COMBO_END};
const uint16_t end_sentence_combo[] PROGMEM = {KC_U, KC_R, COMBO_END};

combo_t key_combos[] = {
    [QW_EXIT] = COMBO(exit_combo, LALT(KC_F4)),
    [MCOMM_AE] = COMBO(ae_combo, KC_AE),
    [COMMDOT_OE] = COMBO(oe_combo, KC_OE),
    [DOTSLSH_AA] = COMBO(aa_combo, KC_AA),
    [CAPS_COMBO] = COMBO_ACTION(caps_combo),
    [END_SENTENCE_COMBO] = COMBO_ACTION(end_sentence_combo),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
  if (pressed) {
    switch(combo_index) {
      case CAPS_COMBO:
        caps_word_set(true);
        break;

      case END_SENTENCE_COMBO:
        SEND_STRING(". ");
        add_oneshot_mods(MOD_BIT(KC_LSFT));  // Set one-shot mod for shift.
        break;
    }
  }
}
#endif

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
    case U_NAV:
    case U_FUN:
      return 0;  // Bypass Achordion for these keys.
  }

  return 800;  // Otherwise use a timeout of 800 ms.
}

bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {
  switch (tap_hold_keycode) {
    case HOME_SCLN:  // SCLN + L.
      if (other_keycode == HOME_L) { return true; }
      break;
    case HOME_D:
      if (other_keycode == KC_W) { return true; }
      break;
  } 

  // Also allow same-hand holds when the other key is in the rows below the
  // alphas. 
  if (other_record->event.key.row >= 3) { return true; }

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

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }
  if (!process_caps_word(keycode, record)) { return false; }
  if (!process_select_word(keycode, record, SELWORD)) { return false; }
  
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