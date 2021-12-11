/* Copyright 2020 foostan
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

uint16_t copy_paste_timer;

// SMART BACKSPACE DELETE
bool shift_held = false;
static uint16_t held_shift = 0;

typedef struct {
  bool is_press_action;
  int state;
} tap;

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
    SYMBOL
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
    TD_COPY_PASTE_APP
};



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[QWERTY] = LAYOUT(
      KC_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,     KC_I,    KC_O,   KC_P,  TD(TD_AA),
      KC_TAB,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,     KC_K,    KC_L, TD(TD_AE), TD(TD_OE),
     KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,  KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
     KC_CCCV, KC_LALT, KC_LGUI, KC_LCTL,   MO(LOWER),  KC_SPC,  KC_ENT,   MO(RAISE),  KC_BSPC_DEL, KC_RGUI, KC_RALT, KC_LEAD
  ),

[LOWER] = LAYOUT(
     _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,  KC_F7,    KC_F8,    KC_F9,    KC_F10, _______,
     _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_F11, KC_F12, _______,   _______,  _______, _______, _______,
     _______, SGUI(KC_LEFT), LGUI(KC_LEFT), LGUI(KC_UP), LGUI(KC_RGHT), SGUI(KC_RIGHT), _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______,  _______, _______, _______,   _______, _______, _______, _______, _______
  ),

[RAISE] = LAYOUT(
     _______, KC_1, 	KC_2,    KC_3,    KC_4,    KC_5,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
     _______, _______, KC_VOLD, KC_MUTE, KC_VOLU, _______, _______, KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT, _______,
     _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END, _______,
     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

[SYMBOL] = LAYOUT(
     _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
     _______, KC_GRAVE, KC_TILDE, KC_UNDS, KC_MINS, KC_PLUS, KC_EQUAL,KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, M_DQUOT,
     _______, M_LRBRC,  M_LRCBR,  M_LRPRN, M_LRABR, _______, _______, _______, KC_LT,   KC_GT,   KC_QUES, KC_CAPS,
     _______, _______, _______, _______,  _______, _______, _______,   _______, _______, _______, _______, _______
  ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, LOWER, RAISE, SYMBOL);
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

LEADER_EXTERNS();

void matrix_scan_user(void) {
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
 static tap copy_paste_app_tap_state = {
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
}
// END: Copy, Paste, Apps

//// END: Advanced Tap Dances


//Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_AE] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, ALGR(KC_Q)),
    [TD_OE] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, ALGR(KC_L)),
    [TD_AA] = ACTION_TAP_DANCE_DOUBLE(KC_BSLS, ALGR(KC_W)),

// Advanced Tap Dances
   //[TD_COPY_PASTE_APP] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, copy_paste_app_finished, copy_paste_app_reset, 300),
};
