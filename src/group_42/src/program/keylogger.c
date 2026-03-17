//
// Created by osdev on 3/12/26.
//

#include <stdio.h>
#include <driver/keyboard.h>
#include <driver/keyboard_map.h>
#include <driver/vga.h>

void keylogger(int argc, char** argv) {
    (void)argc;
    (void)argv;
    printf("Keyboard logger started. Type and press Enter; ESC or Ctrl+C exits.\n");
    vga_update_cursor();
    key_event_t event;
    while (1) {
        if (keyboard_get_event(&event) && event.state == KEY_PRESS) {
            if (event.control && event.scancode == 0x2E) {
                printf("\ncontrol + c detected");
                break;
            }
            if (event.scancode == 0x01) {
                printf("\nescape detected");
                break;
            }

            uint16_t c = keyboard_translate_no(event);

            if (!c) continue;

            printf("%c", keyboard_translate_no(event));
            fflush(stdout);
            vga_update_cursor();
        } else {
            __asm volatile("hlt");
        }
    }
    printf("\nExiting keyboard logger...\n");
}
