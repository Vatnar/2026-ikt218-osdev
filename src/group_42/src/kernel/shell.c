//
// Created by osdev on 3/1/26.
//

#include <driver/keyboard_map.h>
#include <driver/keyboard.h>
#include <driver/vga.h>
#include <kernel/log.h>
#include <kernel/shell.h>
#include <kernel/util/ringbuffer.h>
#include <libc/stddef.h>
#include <libc/stdio.h>
#include <libc/string.h>

#define MAX_COMMAND_LENGTH 256
#define HISTORY_LENGTH 16

static char command_buffer[MAX_COMMAND_LENGTH];
static char command_index = 0;

static char* history[HISTORY_LENGTH];

void shell_init() {
    printf("Hello from shell!\n"); // welcome/init message
    command_buffer[0] = '\0';
    shell_main();
}

void shell_main() {
    // all new lines start with this
    const char* prefix = " MyrstadOS> ";
    key_event_t keyboard_event;

    vga_set_line(prefix);   // set prefix
    vga_update_cursor();    // set cursor

    // shell loop
    while (1) {
        if (keyboard_get_event(&keyboard_event)) {
            if (keyboard_event.state == KEY_PRESS) {
                uint16_t key = keyboard_translate_no(keyboard_event);

                if (key == 0) continue;

                if (key == '\n') {
                    vga_putc('\n');
                    shell_execute(command_buffer);
                    // new line setup
                    vga_set_line(prefix);
                    vga_update_cursor();

                    while (command_index != 0) {
                        command_buffer[command_index--] = '\0';
                    }
                    command_buffer[0] = '\0';
                    continue;
                }
                switch (key) {
                    case '\b': {
                        if (command_index != 0) {
                            vga_putc('\b');
                            vga_update_cursor();
                            command_buffer[command_index] = '\0';
                            command_index--;
                        }
                        break;
                    }
                    case '\r':
                    case '\t':
                        break;
                    default: {

                        if (keyboard_event.extended) {
                            if (key == KEY_UP) {
                                vga_puts("TODO: implement next command\n");
                            }
                            if (key == KEY_DOWN) {
                                vga_puts("TODO: implement next command\n");
                            }
                            if (key == KEY_LEFT) {
                                vga_puts("TODO: implement cursor left\n");
                            }
                            if (key == KEY_RIGHT) {
                                vga_puts("TODO: implement cursor right\n");
                            }
                            vga_set_line(prefix);
                            vga_puts(command_buffer);
                            vga_update_cursor();
                            continue;
                        }

                        if (key < 1 || key > 0x100) continue;
                        command_buffer[command_index++] = key;
                        vga_putc(key);
                        vga_update_cursor();
                    }
                }
            }



        }
    }
}

void shell_execute(char* command) {
    if (strlen(command) == 0) return;
    KERNEL_LOG_INFO("Executing command: %s\n", command);


    if (strcmp(command, "clear") == 0) {
        vga_clear();
        return;
    }
    if (strcmp(command, "cls") == 0) {
        vga_clear();
        return;
    }
    if (strcmp(command, "help") == 0) {
        printf("Functions are: clear/cls and help\n");
        return;
    }

    // if command is not set
    KERNEL_LOG_WARN("Command '%s' not found. Try 'help' for list of valid commands\n");
}