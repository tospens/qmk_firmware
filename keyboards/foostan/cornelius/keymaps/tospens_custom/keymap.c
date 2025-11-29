#include QMK_KEYBOARD_H

enum custom_keycodes {
    UPDIR = SAFE_RANGE,
    CBLCK,
    JOINLN,
    SRCHSEL
};

// Alias for community module keycodes.
#define SNTCSE SENTENCE_CASE_TOGGLE

// Left-hand thumb keys.
#define U_FUN LT(FUN, KC_ESC)
#define U_NAV LT(NAV, KC_SPC)
#define U_SYMR LT(SYM, KC_TAB)

// Right-hand thumb keys.
#define U_SYML LT(SYM, KC_ENT)
#define U_NUM LT(NUM, KC_BSPC)
#define U_MEDIA LT(MEDIA, KC_DEL)

// Danish æ, ø and å keycodes for EurKey.
#define KC_AE ALGR(KC_Q)
#define KC_OE ALGR(KC_L)
#define KC_AA ALGR(KC_W)

// Convenience row shorthands.
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX
#define ______________HOME_ROW_ALGR_L______________ U_NA,    KC_ALGR, U_NA,    U_NA,    U_NA
#define ______________HOME_ROW_GACS_R______________ XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI
#define ______________HOME_ROW_ALGR_R______________ U_NA,    U_NA,    U_NA,    KC_ALGR, U_NA
#define _____________CLIPBOARD_KEYS_L______________ U_UND,   U_CUT,   U_CPY,   U_PST,   U_RDO
#define _____________CLIPBOARD_KEYS_R______________ U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND
#define _____________WINDOW_MANAGEMENT_____________ U_SLFT,  U_WLFT,  U_WNUP,  U_WRGT,  U_SRGT

enum layers { BASE, MEDIA, NAV, SYM, NUM, FUN };

// clang-format off
/*
┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│ BASE LAYER (with Home Row Mods: GUI/ALT/CTL/SFT on ASDF/JKL;)                                                                   │
├──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┤
│   Tab    │    Q     │    W     │    E     │    R     │    T     │    Y     │    U     │    I     │    O     │    P     │    \     │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│   Esc    │    A     │    S     │    D     │    F     │    G     │    H     │    J     │    K     │    L     │    ;     │    '     │
│          │  (GUI)   │  (Alt)   │  (Ctl)   │  (Sft)   │          │          │  (Sft)   │  (Ctl)   │  (Alt)   │  (GUI)   │          │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│  Shift   │    Z     │    X     │    C     │    V     │    B     │    N     │    M     │    ,     │    .     │    /     │  Shift   │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│   Ctl    │   Down   │    Up    │   FUN    │   NAV    │   SYM    │   SYM    │   NUM    │  MEDIA   │   Left   │  Right   │ SrchSel  │
│          │          │          │  (Esc)   │  (Spc)   │  (Tab)   │  (Ent)   │  (Bsp)   │  (Del)   │          │          │          │
└──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘

┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│ NAV LAYER - Navigation & Window Management                                                                                   │
├──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┤
│    --    │   Undo   │   Cut    │   Copy   │  Paste   │   Redo   │   Redo   │  Paste   │   Copy   │   Cut    │   Undo   │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │   GUI    │   Alt    │   Ctl    │   Sft    │    --    │ SelWord  │   Left   │   Down   │    Up    │  Right   │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │ S+G+L    │  G+Left  │   G+Up   │  G+Rght  │ S+G+R    │ JoinLine │   Home   │  PgDown  │  PgUp    │   End    │   Caps   │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │    --    │    --    │    --    │    --    │    --    │  Enter   │   Bsp    │   Del    │ DeskLft  │ DeskRgt  │    --    │
└──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘

┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│ MEDIA LAYER - Media Controls                                                                                                     │
├──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┤
│    --    │    --    │    --    │    --    │    --    │    --    │    --    │    --    │    --    │    --    │    --    │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │   Prev   │  Vol+    │  Vol-    │   Next   │    --    │    --    │   Sft    │   Ctl    │   Alt    │   GUI    │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │    --    │    --    │    --    │    --    │    --    │    --    │    --    │    --    │  AltGr   │    --    │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │    --    │    --    │   Mute   │   Play   │   Stop   │    --    │    --    │    --    │    --    │    --    │    --    │
└──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘

┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│ NUM LAYER - Number Pad                                                                                                           │
├──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┤
│    --    │    =     │    7     │    8     │    9     │    .     │    --    │    --    │    --    │    --    │    --    │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │    :     │    4     │    5     │    6     │    +     │    --    │   Sft    │   Ctl    │   Alt    │   GUI    │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │    /     │    1     │    2     │    3     │    *     │    --    │    --    │    --    │  AltGr   │    --    │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │    --    │    --    │   Bsp    │    0     │    -     │    --    │    --    │    --    │    --    │    --    │    --    │
└──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘

┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│ SYM LAYER - Symbols & Special Characters                                                                                         │
├──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┤
│    --    │    '     │    <     │    >     │    "     │   ```    │    `     │    &     │    [     │    ]     │    %     │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │    !     │    -     │    +     │    =     │    #     │    _     │    |     │    (     │    )     │    @     │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │    ^     │    /     │    *     │    \     │   ../    │    ~     │    $     │    {     │    }     │    ?     │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │    --    │    --    │   Alt    │   Ctl    │    _     │    --    │   Ctl    │   Alt    │    --    │    --    │    --    │
└──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘

┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│ FUN LAYER - Function Keys & System                                                                                               │
├──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┤
│  RESET   │    --    │    --    │    --    │    --    │    --    │  PrtScr  │    F7    │    F8    │    F9    │   F12    │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │   GUI    │   Alt    │   Ctl    │   Sft    │    --    │  ScrLk   │    F4    │    F5    │    F6    │   F11    │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │    --    │ DeskLft  │ DeskShw  │ DeskRgt  │    --    │  Pause   │    F1    │    F2    │    F3    │   F10    │    --    │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│    --    │    --    │    --    │    --    │    --    │    --    │ SrchSel  │ SentCase │   App    │    --    │    --    │    --    │
└──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘
*/
#define LAYOUT_LAYER_BASE                                                                                        \
    KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,     KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,      KC_BSLS, \
    KC_ESC,   KC_A,    KC_S,    KC_D,    KC_F,   KC_G,     KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN,   KC_QUOT, \
    KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,     KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH,   KC_RSFT, \
    KC_LCTL,  KC_DOWN, KC_UP,   U_FUN,   U_NAV,  U_SYMR,   U_SYML, U_NUM,  U_MEDIA, KC_LEFT, KC_RGHT,   SRCHSEL

#define LAYOUT_LAYER_NAVIGATION                                                                                  \
    U_NA, _____________CLIPBOARD_KEYS_L______________,    _____________CLIPBOARD_KEYS_R______________, U_NU,     \
    U_NA, ______________HOME_ROW_GACS_L______________,    SELWORD, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, U_NU,     \
    U_NA, _____________WINDOW_MANAGEMENT_____________,    JOINLN,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_CAPS,  \
    U_NA, _______________DEAD_HALF_ROW_______________,    KC_ENT,  KC_BSPC, KC_DEL,  U_DLFT,  U_DRGT,  U_NU

#define LAYOUT_LAYER_MEDIA                                                                                       \
    U_NA, _______________DEAD_HALF_ROW_______________,    _______________DEAD_HALF_ROW_______________, U_NU,     \
    U_NA, KC_MPRV, KC_VOLU, KC_VOLD, KC_MNXT, U_NU,       ______________HOME_ROW_GACS_R______________, U_NU,     \
    U_NA, U_NU,    U_NU,    U_NU,    U_NU,    U_NU,       ______________HOME_ROW_ALGR_R______________, U_NU,     \
    U_NA, U_NU,    U_NU,    KC_MUTE, KC_MPLY, KC_MSTP,    _______________DEAD_HALF_ROW_______________, U_NU

#define LAYOUT_LAYER_NUMERAL                                                                                     \
    U_NU, KC_EQL,  KC_7,    KC_8,    KC_9,    KC_DOT,     _______________DEAD_HALF_ROW_______________, U_NA,     \
    U_NU, KC_COLN, KC_4,    KC_5,    KC_6,    KC_PLUS,    ______________HOME_ROW_GACS_R______________, U_NA,     \
    U_NU, KC_SLSH, KC_1,    KC_2,    KC_3,    KC_ASTR,    ______________HOME_ROW_ALGR_R______________, U_NA,     \
    U_NU, U_NU,    U_NU,    KC_BSPC, KC_0,    KC_MINS,    _______________DEAD_HALF_ROW_______________, U_NA

#define LAYOUT_LAYER_SYMBOLS                                                                                     \
    U_NU, KC_QUOT, KC_LABK, KC_RABK, KC_DQUO, CBLCK,      KC_GRV,  KC_AMPR, KC_LBRC, KC_RBRC, KC_PERC,  U_NU,    \
    U_NU, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH,    KC_UNDS, KC_PIPE, KC_LPRN, KC_RPRN, KC_AT,    U_NU,    \
    U_NU, KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, UPDIR,      KC_TILD, KC_DLR,  KC_LCBR, KC_RCBR, KC_QUES,  U_NU,    \
    U_NU, U_NU,    U_NU,    KC_LALT, KC_LCTL, KC_UNDS,    U_NU,    KC_RCTL, KC_RALT, U_NU,    U_NU,     U_NU

#define LAYOUT_LAYER_FUNCTION                                                                                    \
    QK_BOOT,    _______________DEAD_HALF_ROW_______________,   KC_PSCR,  KC_F7,   KC_F8,   KC_F9,   KC_F12, U_NU,     \
    U_NA,       ______________HOME_ROW_GACS_L______________,   KC_SCRL,  KC_F4,   KC_F5,   KC_F6,   KC_F11, U_NU,     \
    U_NA,       U_NU,    U_DLFT,  U_DSHW,  U_DRGT,  U_NU,      KC_PAUS,  KC_F1,   KC_F2,   KC_F3,   KC_F10, U_NU,     \
    U_NA,       _______________DEAD_HALF_ROW_______________,   SRCHSEL, SNTCSE,  KC_APP,  U_NU,    U_NU,   U_NU

// Add Home Row mod to a layout.
#define _HOME_ROW_MOD_GACS(                                                                           \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K10, K11,                                       \
    K12, K13, K14, K15, K16, K17, K18, K19, K20, K21, K22, K23,                                       \
    ...)                                                                                              \
      K00,         K01,        K02,         K03,         K04,         K05,         K06,         K07,  \
      K08,         K09,        K10,         K11,         K12,  LGUI_T(K13), LALT_T(K14), LCTL_T(K15), \
      LSFT_T(K16), K17,        K18,  RSFT_T(K19), RCTL_T(K20), LALT_T(K21), RGUI_T(K22),        K23,  \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT_wrapper(HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE)),
  [NAV] = LAYOUT_wrapper(LAYOUT_LAYER_NAVIGATION),
  [MEDIA] = LAYOUT_wrapper(LAYOUT_LAYER_MEDIA),
  [NUM] = LAYOUT_wrapper(LAYOUT_LAYER_NUMERAL),
  [SYM] = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS),
  [FUN] = LAYOUT_wrapper(LAYOUT_LAYER_FUNCTION),
};

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
const uint16_t PROGMEM caps_combo[] = {KC_V, KC_M, COMBO_END};
const uint16_t PROGMEM end_sentence_combo[] = {KC_U, KC_R, COMBO_END};

combo_t key_combos[] = {
    [QW_EXIT] = COMBO(exit_combo, LALT(KC_F4)),
    [MCOMM_AE] = COMBO(ae_combo, KC_AE),
    [COMMDOT_OE] = COMBO(oe_combo, KC_OE),
    [DOTSLSH_AA] = COMBO(aa_combo, KC_AA),
    [CAPS_COMBO] = COMBO_ACTION(caps_combo),
    [END_SENTENCE_COMBO] = COMBO_ACTION(end_sentence_combo),
};
// clang-format on

void process_combo_event(uint16_t combo_index, bool pressed) {
    if (pressed) {
        switch (combo_index) {
            case CAPS_COMBO:
                caps_word_on();  // QMK built-in caps word
                break;

            case END_SENTENCE_COMBO:
                SEND_STRING(". ");
                add_oneshot_mods(MOD_BIT(KC_LSFT)); // Set one-shot mod for shift.
                break;
        }
    }
}
#endif

// Chordal Hold layout - defines handedness for the "opposite hands" rule.
// 'L' = left hand, 'R' = right hand, '*' = thumb keys (exempt from rule)
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
    //   0    1    2    3    4    5    6    7    8    9   10   11
    {'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R'},  // Row 0
    {'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R'},  // Row 1
    {'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R'},  // Row 2
    {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},  // Row 3 (thumb/bottom row)
};

bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                      uint16_t other_keycode, keyrecord_t* other_record) {
    // Allow same-hand chords for specific key combinations
    switch (tap_hold_keycode) {
        case RGUI_T(KC_SCLN): // GUI (;) + L - for lock screen, etc.
            if (other_keycode == LALT_T(KC_L) || other_keycode == KC_L) {
                return true;
            }
            break;
        case LCTL_T(KC_D): // D + W
            if (other_keycode == KC_W) {
                return true;
            }
            break;
    }

    // Use the default opposite hands rule based on chordal_hold_layout
    return get_chordal_hold_default(tap_hold_record, other_record);
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) {
        switch (keycode) {
            case UPDIR:
                SEND_STRING("../");
                return false;

            case CBLCK:
                SEND_STRING("```");
                return false;

            case SRCHSEL: // Searches the current selection in a new tab.
                // Mac users, change LCTL to LGUI.
                SEND_STRING(SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER));
                return false;

            case JOINLN:     // Join lines like Vim's `J` command.
                SEND_STRING( // Go to the end of the line and tap delete.
                    SS_TAP(X_END) SS_TAP(X_DEL)
                    // In case this has joined two words together, insert one space.
                    SS_TAP(X_SPC) SS_LCTL(
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

// matrix_scan_user no longer needed - chordal hold is handled by QMK core
