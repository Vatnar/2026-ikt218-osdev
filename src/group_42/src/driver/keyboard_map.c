//
// Created by osdev on 3/7/26.
//

#include <driver/keyboard_map.h>

// CP437 / VGA Hardware character mappings for Norwegian vowels
#define CP437_ae_LOWER 0x91  // æ
#define CP437_ae_UPPER 0x92  // Æ
#define CP437_oo_LOWER 0xED  // ø (Technically 'phi', but used for ø)
#define CP437_oo_UPPER 0x99  // Ø (Technically Ö, but used for Ø)
#define CP437_aa_LOWER 0x86  // å
#define CP437_aa_UPPER 0x8F  // Å

// Norwegian keyboard
static unsigned char kbd_no[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '\\', '\b', /* 0x00 - 0x0E */
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', '"', '\n',    /* 0x0F - 0x1C */
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'o', 'e', '|',            /* 0x1D - 0x29 */
    0, '\'', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0, '*',        /* 0x2A - 0x36 */
    0, ' ', 0,                                                                /* 0x37 - 0x39 */
    /* Key 0x56 is the < > | key next to Left Shift */
    [0x56] = '<'
};

// shifted (shift enabled) Norwegian keyboard
static unsigned char kbd_no_shifted[128] = {
    0,  27, '!', '"', '#', '4', '%', '&', '/', '(', ')', '=', '?', '`', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', '^', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'O', 'E', '|',
    0, '*', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 0, 0,
    0, ' ', 0,
    [0x56] = '>'
};

uint16_t keyboard_translate_no(key_event_t event) {
    // 1. Handle Extended Keys (E0)
    if (event.extended) {
        switch (event.scancode) {
            case 0x48: return KEY_UP;
            case 0x50: return KEY_DOWN;
            case 0x4B: return KEY_LEFT;
            case 0x4D: return KEY_RIGHT;
            case 0x1C: return '\n'; // Keypad Enter
            case 0x38: return 0;    // AltGr pressed/released (modifier only)
            default: return 0;
        }
    }

    bool is_upper = (event.shift ^ event.caps);

    // 2. Handle Norwegian æ,ø,å
    switch (event.scancode) {
        case 0x1A: // Key 'Å'
            return is_upper ? CP437_aa_UPPER : CP437_aa_LOWER;
        case 0x27: // Key 'Ø'
            return is_upper ? CP437_oo_UPPER : CP437_oo_LOWER;
        case 0x28: // Key 'Æ'
            return is_upper ? CP437_ae_UPPER : CP437_ae_LOWER;
        default:
            break;
    }

    // 3. AltGr Combinations (Right Alt + Key)
    if (event.alt) {
        switch (event.scancode) {
            case 0x03: return '@'; // AltGr + 2
            case 0x05: return '$'; // AltGr + 4
            case 0x08: return '{'; // AltGr + 7
            case 0x09: return '['; // AltGr + 8
            case 0x0A: return ']'; // AltGr + 9
            case 0x0B: return '}'; // AltGr + 0
            default: break;
        }
    }

    // Standard map (default)
    if (event.scancode > 1 && event.scancode < 128) {
        char c = event.shift ? (char)kbd_no_shifted[event.scancode] : (char)kbd_no[event.scancode];

        // Final Caps Lock correction for A-Z
        if (event.caps && c >= 'a' && c <= 'z' && !event.shift) c -= 32;
        else if (event.caps && c >= 'A' && c <= 'Z' && event.shift) c += 32;

        return (uint16_t)c;
    }

    return 0;
}