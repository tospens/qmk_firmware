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
    GAME,
	RAISE,
	LOWER,
	NUM,
    SYMBOL
};

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

layer_state_t layer_state_set_user(layer_state_t state) {
  state = update_tri_layer_state(state, LOWER, RAISE, SYMBOL);

  // Super Alt-Tab
    if (is_alt_tab_active) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
    }

  combo_enable(); // Enable combos by default
  switch (get_highest_layer(layer_state))
  {
  case BASE:
      break;
  case GAME:
    combo_disable();
      break;
  case NUM:
    combo_disable();
      break;
  default:
      break;
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
		case GAME:
			oled_write_P(PSTR("GAME "), false);
			break;
		case LOWER:
			oled_write_P(PSTR("LOWER"), false);
			break;
		case RAISE:
			oled_write_P(PSTR("RAISE"), false);
			break;
		case NUM:
			oled_write_P(PSTR("NUM  "), false);
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

//Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_AE] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, ALGR(KC_Q)),
    [TD_OE] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, ALGR(KC_L)),
    [TD_AA] = ACTION_TAP_DANCE_DOUBLE(KC_BSLS, ALGR(KC_W)),

};
