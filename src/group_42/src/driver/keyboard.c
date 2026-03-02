//
// Created by osdev on 2/28/26.
//

#include <driver/keyboard.h>
#include <driver/vga.h>
#include <kernel/util.h>
#include <kernel/irq.h>
#include <libc/stdbool.h>

// Norwegian keyboard
unsigned char kbd_no[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '\\', '\b', /* 0x00 - 0x0E */
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', '"', '\n',    /* 0x0F - 0x1C */
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'o', 'e', '|',            /* 0x1D - 0x29 */
    0, '\'', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0, '*',        /* 0x2A - 0x36 */
    0, ' ', 0,                                                                /* 0x37 - 0x39 */
    /* Key 0x56 is the < > | key next to Left Shift */
    [0x56] = '<'
};

// shifted (shift enabled) Norwegian keyboard
unsigned char kbd_no_shifted[128] = {
    0,  27, '!', '"', '#', '4', '%', '&', '/', '(', ')', '=', '?', '`', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', '^', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'O', 'E', '|',
    0, '*', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 0, 0,
    0, ' ', 0,
    [0x56] = '>'
};

bool shift;
bool caps;

// Circular queue structure for keyboard buffer
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int head = 0;
static int tail = 0;

void keyboard_callback(registers_t *regs) {
    /* The PIC leaves the scan code in port 0x60 */
    const uint8_t scancode = port_byte_in(0x60);

    /* If the top bit of the byte is set, it means a key was released */
    if (scancode & 0x80) {
        // Key released - we can ignore this for a basic driver
        const uint8_t released_code = scancode & 0x7F;
        if (released_code == 0x2A || released_code == 0x36) {
            shift = false; // Left or Right Shift released
        }
        return;
    }
    // Key pressed - map it to ASCII
    switch (scancode) {
        case 0x2A:
        case 0x36:
            shift = true;
            break;
        case 0x3A:
            caps = !caps;
            break;
        case 0x27: // Key 'Ø'
            if (shift || caps)  { keyboard_push('O'); keyboard_push('E'); }
            else                { keyboard_push('o'); keyboard_push('e'); }
            break;
        case 0x28: // Key 'Æ'
            if (shift || caps)  { keyboard_push('A'); keyboard_push('E'); }
            else                { keyboard_push('a'); keyboard_push('e'); }
            break;
        case 0x1A: // Key 'Å'
            if (shift || caps)  { keyboard_push('A'); keyboard_push('A'); }
            else                { keyboard_push('a'); keyboard_push('a'); }
            break;
        default: {
            char c;

            // Which keyboard scancode to use: shifted or not
            if (shift) {
                c = (char)kbd_no_shifted[scancode];
            } else {
                c = (char)kbd_no[scancode];
            }

            // Handle caps, shift up or down opposite of normal key
            if (caps && c >= 'a' && c <= 'z') {
                c -= 32;
            } else if (caps && shift && c >= 'A' && c <= 'Z') {
                c += 32;
            }

            if (c > 0)

                keyboard_push(c);
            break;
        }
    }

    vga_update_cursor(); // NOTE: should may
}

void init_keyboard() {
    // IRQ 1 is the keyboard
    register_irq_routine(1, keyboard_callback);
    shift = false;
    caps = false;
}

char keyboard_pop() {
    if (head == tail) return 0;
    char c = keyboard_buffer[tail];
    tail = (tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

void keyboard_push(char c) {
    int next = (head + 1) % KEYBOARD_BUFFER_SIZE;
    if (next != tail) {
        keyboard_buffer[head] = c;
        head = next;
    }
}