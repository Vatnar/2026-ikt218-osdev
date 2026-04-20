#pragma once
#include <stdint.h>
#include <stdbool.h>

extern uint8_t* scancodeToAscii;
extern uint8_t* scancodeToAsciiShift;

bool set_keymap(const char *keymap);