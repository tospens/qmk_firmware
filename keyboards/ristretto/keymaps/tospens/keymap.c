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

#include QMK_KEYBOARD_H
#include "tospens.c"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[BASE] = LAYOUT(
		KC_ESC  , KC_Q    , KC_W    , KC_E    , KC_R    , KC_T              , KC_Y    , KC_U    , KC_I    , KC_O    , KC_P    , TD(TD_AA) ,
		KC_TAB  , KC_A    , KC_S    , KC_D    , KC_F    , KC_G              , KC_H    , KC_J    , KC_K    , KC_L    , TD(TD_AE) , TD(TD_OE) ,
		KC_LSFT , KC_Z    , KC_X    , KC_C    , KC_V    , KC_B              , KC_N    , KC_M    , KC_COMM , KC_DOT  , KC_SLSH , KC_RSFT  ,
		KC_CCCV , KC_LALT , KC_LGUI , KC_LCTL ,MO(LOWER),KC_SPC  , KC_MUTE , KC_ENT  , MO(RAISE) ,KC_BSPC_DEL, KC_RGUI , KC_RALT , KC_RCTRL
	),
	[GAME] = LAYOUT(
		KC_ESC  , KC_Q    , KC_W    , KC_E    , KC_R    , KC_T              , KC_Y    , KC_U    , KC_I    , KC_O    , KC_P    , TD(TD_AA) ,
		KC_LSFT , KC_A    , KC_S    , KC_D    , KC_F    , KC_G              , KC_H    , KC_J    , KC_K    , KC_L    , TD(TD_AE) , TD(TD_OE) ,
		KC_TAB  , KC_Z    , KC_X    , KC_C    , KC_V    , KC_B              , KC_N    , KC_M    , KC_COMM , KC_DOT  , KC_SLSH , KC_RSFT  ,
		KC_CCCV  , KC_LALT , KC_LGUI , KC_LCTL ,MO(NUM) , KC_SPC  , KC_MUTE , KC_ENT  , MO(RAISE) ,KC_BSPC_DEL, KC_RGUI , KC_RALT , KC_RCTRL
	),
    [LOWER] = LAYOUT(
		_______ , KC_F1   , KC_F2   , KC_F3   , KC_F4   , KC_F5	            , KC_F6   , KC_F7   , KC_F8   , KC_F9  , KC_F10  , _______  ,
		ALT_TAB , _______ , KC_MPRV , KC_MPLY , KC_MNXT , KC_F11          , KC_F12  , _______, _______, _______ , _______ , _______,
		_______ , SGUI(KC_LEFT), LGUI(KC_LEFT), LGUI(KC_UP), LGUI(KC_RGHT), SGUI(KC_RIGHT) , _______, _______, _______, _______, _______, _______,
		_______ , _______ , _______ , _______, _______ , _______, KC_MUTE , _______ , _______ ,_______,_______, _______ , _______
	),
	[RAISE] = LAYOUT(
		_______	, KC_1	  , KC_2    , KC_3    , KC_4  , KC_5              , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , _______ ,
		_______	, _______ , _______	, _______ , _______ , _______           , _______, KC_LEFT, KC_UP  , KC_DOWN, KC_RGHT, _______ ,
		_______	, TO(BASE), TO(GAME), _______ , _______ , _______           , _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END , _______ ,
		_______ , _______ , _______ , _______ , _______ , _______ , KC_MUTE , _______, _______, _______, _______, _______, _______
	),
	[NUM] = LAYOUT(
		_______ , KC_1	  , KC_2    , KC_3    , KC_4    , KC_5              , _______ , _______ , _______ , _______ , _______ , _______ ,
		_______ , KC_6    , KC_7    , KC_8    , KC_9    , KC_0              , _______ , _______ , _______ , _______ , _______ , _______ ,
		_______ , KC_PSCR , _______ , _______ , _______ , _______           , _______ , _______ , _______ , _______ , _______ , _______ ,
		_______ , _______ , _______ , _______ , _______ , _______ , KC_MUTE , _______ , _______ , _______ , _______ , _______ , _______
	),
	[SYMBOL] = LAYOUT(
		_______ , KC_EXLM , KC_AT   , KC_HASH , KC_DLR  , KC_PERC           , KC_CIRC , KC_AMPR , KC_ASTR , KC_LPRN , KC_RPRN , _______ ,
        _______ , KC_GRAVE, KC_TILDE, KC_UNDS , KC_MINS , KC_PLUS           , KC_EQUAL, KC_LCBR , KC_RCBR , KC_LBRC , KC_RBRC , M_DQUOT ,
        _______ , M_LRBRC , M_LRCBR , M_LRPRN , M_LRABR , _______           , _______ , _______ , KC_LT   , KC_GT   , KC_QUES , KC_CAPS ,
		_______ , _______ , _______ , _______ , _______ , _______ , KC_MUTE , _______ , _______ , _______ , _______ , _______ , _______
	),
};

// OLED and Encoder function is located in the ristretto.c File
