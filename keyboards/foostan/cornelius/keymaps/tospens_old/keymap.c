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


// ONE KEY COPY-PASTE
uint16_t copy_paste_timer;

// SMART BACKSPACE DELETE
bool shift_held = false;
static uint16_t held_shift = 0;

// SUPER ALT-TAB
bool is_alt_tab_active = false;

#ifdef COMBO_ENABLE
enum combos {
    ZX_UNDO,
    XC_REDO,
    QW_EXIT,
    SD_RUN
};

const uint16_t PROGMEM undo_combo[] = { KC_Z, KC_X, COMBO_END };
const uint16_t PROGMEM redo_combo[] = { KC_X, KC_C, COMBO_END };
const uint16_t PROGMEM exit_combo[] = { KC_Q, KC_W, COMBO_END };
const uint16_t PROGMEM  run_combo[] = { KC_S, KC_D, COMBO_END };

combo_t key_combos[COMBO_COUNT] = {
    [ZX_UNDO] = COMBO(undo_combo, LCTL(KC_Z)),
    [XC_REDO] = COMBO(redo_combo, LCTL(KC_Y)),
    [QW_EXIT] = COMBO(exit_combo, LALT(KC_F4)),
    [SD_RUN]  = COMBO(run_combo,  LALT(KC_SPC))
};
#endif

// Tap Dance declarations
enum {
    TD_AE,
    TD_OE,
    TD_AA
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
    ALT_TAB, // Super Alt-Tab
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[QWERTY] = LAYOUT(
      KC_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,     KC_I,    KC_O,   KC_P,  TD(TD_AA),
      KC_TAB,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,     KC_K,    KC_L, TD(TD_AE), TD(TD_OE),
     KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,  KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
     KC_CCCV, KC_LALT, KC_LGUI, KC_LCTL, MO(LOWER),  KC_SPC,  KC_ENT, MO(RAISE),  KC_BSPC_DEL, KC_RGUI, KC_RALT, KC_LEAD
  ),

[LOWER] = LAYOUT(
     _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,  KC_F7,    KC_F8,    KC_F9,    KC_F10, _______,
     ALT_TAB, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_F11, KC_F12,  _______, _______, _______, _______, _______,
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
    state = update_tri_layer_state(state, LOWER, RAISE, SYMBOL);
    // Super Alt-Tab
    if (is_alt_tab_active) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
    }
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
        case ALT_TAB: // super alt tab macro
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

//Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_AE] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, ALGR(KC_Q)),
    [TD_OE] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, ALGR(KC_L)),
    [TD_AA] = ACTION_TAP_DANCE_DOUBLE(KC_BSLS, ALGR(KC_W))
};
