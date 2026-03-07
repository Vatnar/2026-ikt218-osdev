//
// Created by osdev on 3/7/26.
//

#pragma once

#include <driver/keyboard.h>

// Special Key Definitions (Mapped above ASCII range)
#define KEY_F1      0x101
#define KEY_F2      0x102
#define KEY_F3      0x103
#define KEY_F4      0x104
#define KEY_F5      0x105
#define KEY_F6      0x106
#define KEY_F7      0x107
#define KEY_F8      0x108
#define KEY_F9      0x109
#define KEY_F10     0x10A
#define KEY_F11     0x10B
#define KEY_F12     0x10C
#define KEY_UP      0x110
#define KEY_DOWN    0x111
#define KEY_LEFT    0x112
#define KEY_RIGHT   0x113
#define KEY_INSERT  0x114
#define KEY_DELETE  0x7F

/**
 * Translate keyboard event to VGA keycode using norwegian layout
 * @param event keyboard event
 * @return key character where upper range [128,256> are for special chars
 */
uint16_t keyboard_translate_no(key_event_t event);