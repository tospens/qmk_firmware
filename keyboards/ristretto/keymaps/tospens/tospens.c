/* Copyright 2021 Brandon Lewis
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

#include "ristretto.h"

enum layers {
	BASE,
	RAISE,
	LOWER,
	GAME,
    SYMBOL
};

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

bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) { return false; }
	if(index == 0) {
		if (clockwise) {
			tap_code(KC_VOLD);
		} else {
			tap_code(KC_VOLU);
			}
		}
	return true;
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}
/*
static void render_coffee_logo(void) {
    static const char PROGMEM coffee_logo[] = {
        // 'coffee-bean-icon-8', 32x32px
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x10, 0x08, 0x04, 0x02, 0x00, 0x61, 0x89,
        0x13, 0x00, 0x00, 0x00, 0x08, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x40, 0x80, 0x50, 0x80, 0x08, 0x08, 0x08, 0x02, 0x04, 0x14, 0x20, 0x48, 0x90, 0x00, 0x9f,
        0x00, 0x80, 0x40, 0x20, 0x90, 0x43, 0x00, 0x20, 0x20, 0x20, 0xa0, 0x60, 0x00, 0x80, 0x80, 0x00,
        0x07, 0x20, 0x40, 0x00, 0x00, 0x05, 0x02, 0x08, 0x04, 0x14, 0x00, 0x08, 0x90, 0x20, 0x43, 0x18,
        0x00, 0xf8, 0x02, 0x01, 0x00, 0x00, 0x00, 0x30, 0xc4, 0x19, 0x02, 0x01, 0x00, 0x00, 0x80, 0x3e,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x04, 0x04, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x04, 0x04, 0x00, 0x1a, 0x15, 0x12, 0x10, 0x00, 0x08, 0x00, 0x04, 0x00, 0x00, 0x00
    };
    oled_write_raw_P(coffee_logo, sizeof(coffee_logo));
}
*/

bool oled_task_user(void) {
	oled_write_P(PSTR("\n\n"), false);
	oled_write_P(PSTR("LAYER"), false);
	oled_write_ln_P(PSTR(""), false);
	switch (get_highest_layer(layer_state)) {
		case BASE:
			oled_write_P(PSTR("BASE "), false);
			break;
		case LOWER:
			oled_write_P(PSTR("LOWER"), false);
			break;
		case RAISE:
			oled_write_P(PSTR("RAISE"), false);
			break;
		case GAME:
			oled_write_P(PSTR("GAME "), false);
			break;
		case SYMBOL:
			oled_write_P(PSTR("SYMBL"), false);
			break;
	};
    oled_write_P(PSTR("\n\n"), false);
    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK)    ? PSTR("\nNUML ") : PSTR("     "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK)   ? PSTR("\nCAPS ") : PSTR("     "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("\nSCRL ") : PSTR("     "), false);

    return false;
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
