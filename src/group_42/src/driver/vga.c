//
// Created by osdev on 2/17/26.
//

#include "libc/stdint.h"
#include "driver/vga.h"

// Global variables

uint16_t column = 0;
uint16_t row = 0;
uint16_t* const vga_ptr = (uint16_t* const) VGA_MEMORY;

/**
 * Private helper function  for creating the 8 bits for VGA color
 * @param fg foreground color
 * @param bg background color
 * @return uint16_t where the last 8 bits are set for VGA color
 */
static inline uint16_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return (uint16_t)fg | ((uint16_t)bg << 4);
}


#define default_fg VGA_COLOR_LIGHT_GREY;
#define default_bg VGA_COLOR_BLACK;
uint8_t current_fg = default_fg;
uint8_t current_bg = default_bg;

/**
 * Creates a packed structure ready for VGA display
 * @param c char
 * @return packed uint16_t for VGA entry [4 bits bg, 4 bits fg, 8 bits char]
 */
static inline uint16_t vga_entry(char c) {
    return (uint16_t)c | (vga_entry_color(current_fg, current_bg) << 8);
}

void vga_init() {
    vga_reset();
}

void vga_clear() {
    row = 0;
    column = 0;
    // Clear every "pixel"
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_ptr[y * VGA_WIDTH + x] = vga_entry(' ');
        }
    }
}

void vga_reset() {
    current_bg = default_bg;
    current_fg = default_fg;
    vga_clear();
}

void vga_putc(const char c) {
    // First handle every special line breaking character
    if (c == '\n') {
        vga_new_line();
        return;
    }
    if (c == '\r') {
        column = 0;
        return;
    }
    if (c == '\b' && column != 0) {
        // "backspace" clear current pixel and move to prevous
        column--;
        vga_putc(' ');
        column--;
        return;
    }
    if (c == '\t') {
        const uint16_t tab_len = 4 - (column % 4);

        for (uint16_t i = 0; i < tab_len; i++) {
            vga_putc(' '); // For auto line wrapping

            // if line gets wrapped
            if (column == 0) break;

        }

        return;
    }

    // Use raw VGA memory addresses to set current "pixel"
    vga_ptr[row * VGA_WIDTH + column] = vga_entry(c);
    column++;
    if (column >= VGA_WIDTH) {  // line wrap
        vga_new_line();         // new line auto scrolls if needed
    }
}

void vga_puts(const char *cp) {
    // Loop though each char and put it on screen
    size_t index = 0;
    while (cp[index] != '\0') {
        vga_putc(cp[index]);
        index++;
    }
}

void vga_new_line() {
    row++;
    column = 0;
    // scroll if needed
    if (row >= VGA_HEIGHT) {
        vga_scroll_line();
    }
}

void vga_scroll_line() {
    // move every character 1 row upwards
    for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            vga_ptr[(y-1) * VGA_WIDTH + x] = vga_ptr[(y) * VGA_WIDTH + x];
        }
    }
    row-=1;
    // clear last screen
    for (uint16_t x = 0; x < VGA_WIDTH; x++) {
        vga_ptr[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ');
    }

}

void vga_set_color(const enum vga_color fg, const enum vga_color bg) {
    current_fg = fg;
    current_bg = bg;
}