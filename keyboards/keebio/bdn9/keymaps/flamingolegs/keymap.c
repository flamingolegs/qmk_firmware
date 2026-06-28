/* Copyright 2019 Danny Nguyen <danny@keeb.io>
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

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

enum custom_keycodes {
        SCROLL_AXIS = SAFE_RANGE,
        ZOOM_RESET,
    OMNIFOCUS_TODO,
    OPEN_RAMBOX,
    OPEN_OUTLOOK,
};

static bool middle_horizontal_scroll = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        | Knob 1: Vol Dn/Up |      | Knob 2: Page Dn/Up |
        | Press: Mute       | Home | Press: Play/Pause  |
        | Hold: Layer 2     | Up   | RGB Mode           |
        | Left              | Down | Right              |
     */
    [0] = LAYOUT(
        KC_MUTE, SCROLL_AXIS, ZOOM_RESET,
        KC_MPRV, KC_MPLY, KC_MNXT,
        OMNIFOCUS_TODO, OPEN_RAMBOX, OPEN_OUTLOOK
    ),
    /*
        | QK_BOOT          | N/A  | Media Stop |
        | Held: Layer 2  | Home | RGB Mode   |
        | Media Previous | End  | Media Next |
     */
    [1] = LAYOUT(
        QK_BOOT  , BL_STEP, KC_STOP,
        _______, KC_HOME, RM_NEXT,
        KC_MPRV, KC_END , KC_MNXT
    ),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == _MIDDLE) {
        if (middle_horizontal_scroll) {
            if (clockwise) {
                tap_code(MS_WHLR);
            } else {
                tap_code(MS_WHLL);
            }
        } else {
            if (clockwise) {
                tap_code(MS_WHLD);
            } else {
                tap_code(MS_WHLU);
            }
        }
    }
    else if (index == _RIGHT) {
        if (clockwise) {
            tap_code16(G(KC_EQL));
        } else {
            tap_code16(G(KC_MINS));
        }
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }

    switch (keycode) {
        case SCROLL_AXIS:
            middle_horizontal_scroll = !middle_horizontal_scroll;
            return false;
        case ZOOM_RESET:
            tap_code16(G(KC_0));
            return false;
        case OMNIFOCUS_TODO:
            tap_code16(C(A(KC_SPC)));
            return false;
        case OPEN_RAMBOX:
            tap_code16(G(KC_SPC));
            wait_ms(100);
            SEND_STRING("Rambox");
            tap_code(KC_ENT);
            return false;
        case OPEN_OUTLOOK:
            tap_code16(G(KC_SPC));
            wait_ms(100);
            SEND_STRING("Outlook");
            tap_code(KC_ENT);
            return false;
        default:
            return true;
    }
}
