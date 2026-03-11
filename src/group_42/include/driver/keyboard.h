//
// Created by osdev on 2/28/26.
//

#pragma once
#include <kernel/util.h>
#include <libc/stdbool.h>
#include <libc/stdint.h>

typedef enum {
    KEY_PRESS,
    KEY_RELEASE,
} key_state_t;

typedef struct {
    uint16_t scancode;
    key_state_t state;
    bool shift;
    bool control;
    bool alt;
    bool caps;
    bool extended;
} key_event_t;

// Initialize keyboard with IRQ0
void init_keyboard();

/**
 * Get keyboard event.
 *
 * Note: missing alt_gr, dead- and compose key.
 * @param event event stored, reference
 * @return true if event exists, false for no events
 */
bool keyboard_get_event(key_event_t* event);