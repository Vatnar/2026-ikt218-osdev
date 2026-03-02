//
// Created by osdev on 3/1/26.
//

#include <driver/keyboard.h>
#include <driver/vga.h>
#include <kernel/log.h>
#include <kernel/shell.h>
#include <libc/stddef.h>
#include <libc/stdio.h>
#include <libc/string.h>

char command_buffer[MAX_COMMAND_LENGTH];

void shell_init() {
    printf("Hello from shell!\n"); // welcome/init message
    command_buffer[0] = '\0';
    shell_main();
}

void shell_main() {
    // all new lines start with this
    const char* prefix = " MyrstadOS> ";

    vga_set_line(prefix);   // set prefix
    vga_update_cursor();    // set cursor

    // shell loop
    while (1) {
        if (fgets(command_buffer, MAX_COMMAND_LENGTH, stdin) != NULL) {
            int len = (int)strlen(command_buffer);
            if (len > 0 && command_buffer[len-1] == '\n') {
                command_buffer[len-1] = '\0';
            }
            shell_execute(command_buffer);

            // new line setup
            vga_set_line(prefix);
            vga_update_cursor();
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