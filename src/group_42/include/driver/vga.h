//
// Created by osdev on 2/17/26.
//

#pragma once

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW = 14,
    VGA_COLOR_WHITE = 15
};

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// Initlialize VGA (Video Graphics Array)
void vga_init();

// Clear VGA screen
void vga_clear();

// Reset (re-initialize) VGA: clears screen and resets color
void vga_reset();

// Makes new characters appear on a new line
void vga_new_line();

// Scroll everything up 1 line
void vga_scroll_line();

/**
 * Put character to VGA display. Supports \t \n \b \r
 * @param c character
 */
void vga_putc(char c);

/**
 * Puts a string to VGA display. Uses vga_putc under the hood
 * @param s string
 */
void vga_puts(const char *s);

/**
 * Sets VGA colors
 * @param fg foreground color
 * @param bg background color
 */
void vga_set_color(enum vga_color fg, enum vga_color bg);