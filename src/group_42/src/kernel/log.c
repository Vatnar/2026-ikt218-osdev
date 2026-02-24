//
// Created by osdev on 2/19/26.
//

#include <driver/vga.h>
#include <kernel/log.h>
#include <libc/stdio.h>

void kernel_log(klog_level_t level, const char *format, ...) {
    FILE* stream;
    if (level == KLOG_ERROR)
        stream = stderr;
    else
        stream = stdout;

    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    switch (level) {
        case KLOG_INFO: {
            vga_putc('[');
            vga_set_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
            vga_puts("INFO\t");
            vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
            vga_puts("] ");
            break;
        }
        case KLOG_ERROR: {
            vga_putc('[');
            vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
            vga_puts("ERROR\t");
            vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
            vga_puts("] ");
            break;
        }
        case KLOG_WARN: {
            vga_putc('[');
            vga_set_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
            vga_puts("WARN\t");
            vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
            vga_puts("] ");
            break;
        }
        case KLOG_SUCCESS: {
            vga_putc('[');
            vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
            vga_puts("SUCCESS");
            vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
            vga_puts("] ");
            break;
        }
        default:
            break;
    }

    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);
    if (stream == stdout)
        fflush(stream); // in case of no newline character
}