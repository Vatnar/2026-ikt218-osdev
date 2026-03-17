//
// Created by osdev on 3/1/26.
//

#include <driver/keyboard_map.h>
#include <driver/keyboard.h>
#include <driver/vga.h>
#include <kernel/shell.h>
#include <program/keylogger.h>
#include <libc/stdio.h>
#include <libc/string.h>

#define MAX_COMMAND_LENGTH 256
#define HISTORY_LENGTH 16

static char* shell_prefix = " MyrstadOS> ";

static char command_buffer[MAX_COMMAND_LENGTH];
static char command_buf_index = 0;                  // used for shell cursor
static char command_length    = 0;                  // used for shell cursor

static char history[HISTORY_LENGTH][MAX_COMMAND_LENGTH];
static int  history_count = 0;
static int  history_view_index = -1;


/* Private Cursor Helpers */
static void shell_move_cursor(int delta_x) {
    uint16_t x, y;
    get_cursor_position(&x, &y);

    // Simple boundary check (don't backspace into the prefix)
    int prefix_len = (int)strlen(shell_prefix);
    if (x + delta_x < prefix_len) return;

    vga_cursor_position(x + delta_x, y);
}

static void shell_add_history(const char* cmd) {
    if (strlen(cmd) == 0) return;

    // Shift history up
    for (int i = HISTORY_LENGTH - 1; i > 0; i--) {
        strcpy(history[i], history[i-1]);
    }
    strcpy(history[0], cmd);
    if (history_count < HISTORY_LENGTH) history_count++;
}

static void shell_render_line() {
    vga_set_line(shell_prefix);
    printf("%s", command_buffer);
}

static void handle_navigation(uint16_t key) {
    if (key == KEY_UP && history_view_index < history_count - 1) {
        history_view_index++;

        memset(command_buffer, 0, MAX_COMMAND_LENGTH);
        strcpy(command_buffer, history[history_view_index]);

        shell_move_cursor(-command_length);
        command_buf_index = (char)strlen(command_buffer);
        command_length = (char)strlen(command_buffer);
        shell_move_cursor(command_buf_index);
    }
    else if (key == KEY_DOWN && history_view_index >= 0) {
        history_view_index--;
        if (history_view_index == -1) {
            memset(command_buffer, 0, MAX_COMMAND_LENGTH);
        } else {
            memset(command_buffer, 0, MAX_COMMAND_LENGTH);
            strcpy(command_buffer, history[history_view_index]);
        }
        shell_move_cursor(-command_length);
        command_buf_index = (char)strlen(command_buffer);
        command_length = (char)strlen(command_buffer);
        shell_move_cursor(-255);
        shell_move_cursor(command_buf_index);

    } else if (key == KEY_RIGHT) {
        if (command_buf_index < command_length) {
            shell_move_cursor(1);
            command_buf_index++;
        }
    } else if (key == KEY_LEFT) {
        shell_move_cursor(-1);
        command_buf_index--;
    }
    // Note: KEY_LEFT/RIGHT would require tracking a separate
    // internal 'cursor_pos' independent of 'command_index'
}

void shell_init() {
    printf("Hello from shell!\n"); // welcome/init message
    command_buffer[0] = '\0';
    shell_main();
}

void shell_main() {
    // all new lines start with this
    key_event_t ev;

    vga_set_line(shell_prefix);   // set prefix
    vga_update_cursor();          // set cursor

    // shell loop
    while (1) {
        if (!keyboard_get_event(&ev) || ev.state != KEY_PRESS) {
            __asm__ volatile("hlt");
            continue;
        };

        uint16_t key = keyboard_translate_no(ev);

        if (key == '\n') {
            vga_putc('\n');
            shell_execute(command_buffer);
            shell_add_history(command_buffer);

            // Reset state
            memset(command_buffer, 0, MAX_COMMAND_LENGTH);
            command_buf_index = 0;
            command_length = 0;
            history_view_index = -1;

            shell_render_line();
            vga_update_cursor();
        }
        else if (key == '\b' && command_buf_index > 0) {
            // shift everything right to the left, and override current cursor position
            for (int i = command_buf_index - 1; i < command_length; i++) {
                command_buffer[i] = command_buffer[i + 1];
            }
            command_length--;
            command_buf_index--;

            command_buffer[command_length] = '\0';

            // Visual backspace: move left, print space, move left again
            shell_move_cursor(-1);
            shell_render_line();
        } else if (ev.extended) {
            handle_navigation(key);
            shell_render_line();
        } else if (ev.control && key == 'c') {
            printf("\nControl + c detected: exiting shell...\n");
            break;
        } else if (key >= 32 && key <= 126 && command_buf_index < MAX_COMMAND_LENGTH - 1) {
            for (int i = (int)command_length; i > command_buf_index; i--) {
                command_buffer[i] = command_buffer[i - 1];
            }

            command_buffer[command_buf_index++] = (char)key;
            command_length++;

            shell_move_cursor(1);
            shell_render_line();
        }
    }
}

void shell_execute(char* command) {
    if (strlen(command) == 0) return;

    if (strcmp(command, "clear") == 0) {
        vga_clear();
        return;
    }
    if (strcmp(command, "cls") == 0) {
        vga_clear();
        return;
    }
    if (strcmp(command, "help") == 0) {
        printf("All valid functions and commands are:\n\tclear/cls - clear the screen\n\tkeylogger - show all keystrokes\n\thelp - show all commands\n");
        return;
    }
    if (strcmp(command, "keylogger") == 0) {
        keylogger(0, 0);
        return;
    }

    // if command is not set
    printf("sh: %s: command not found\n", command);
}