//
// Created by osdev on 2/28/26.
//

#include <driver/keyboard.h>
#include <kernel/util.h>
#include <kernel/util/ringbuffer.h>
#include <arch/i386/interrupt.h>
#include <kernel/log.h>
#include <libc/stdbool.h>

#define KEYBOARD_BUFFER_SIZE 256

#define PS2_KEYBOARD_PORT 0x60

// Internal Driver State
static bool lshift = false;
static bool rshift = false;
static bool lctrl  = false;
static bool lalt   = false;
static bool caps   = false;
static bool extended_next = false;

// Circular queue structure for keyboard buffer
static uint8_t keyboard_scancode[KEYBOARD_BUFFER_SIZE];
static ringbuffer_t keyboard_rb;

void keyboard_callback(registers_t *regs) {
    const uint8_t scancode = port_byte_in(PS2_KEYBOARD_PORT);
    rb_push(&keyboard_rb, (void*)&scancode);
}

void init_keyboard() {
    // IRQ 1 is the keyboard
    register_irq_routine(1, keyboard_callback);
    rb_init(&keyboard_rb, keyboard_scancode, KEYBOARD_BUFFER_SIZE, RB_UINT8);
}

char keyboard_pop() {
    char c;
    rb_pop(&keyboard_rb, &c);
    return c;
}

void keyboard_push(char c) {
    rb_push(&keyboard_rb, &c);
}

bool keyboard_get_event(key_event_t* event) {
    uint8_t scancode;

    if (!rb_pop(&keyboard_rb, &scancode)) {
        return false;
    }

    if (scancode == 0xE0) {
        extended_next = true;
        return keyboard_get_event(event);
    }

    event->state = (scancode & 0x80) ? KEY_RELEASE : KEY_PRESS;
    event->scancode = scancode & 0x7F;
    event->extended = extended_next;
    extended_next = false;
    bool is_down = (event->state == KEY_PRESS);

    switch (event->scancode) {
        case 0x2A: lshift = is_down; break;
        case 0x36: rshift = is_down; break;
        case 0x1d: lctrl = is_down; break;
        case 0x38: lalt = is_down; break;
        case 0x3A: if (is_down) caps = !caps; break;
        default: break;
    }

    event->shift = (lshift || rshift);
    event->control = lctrl;
    event->alt = lalt;
    event->caps = caps;



    return true;
}