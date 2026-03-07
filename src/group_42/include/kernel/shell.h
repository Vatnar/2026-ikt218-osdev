//
// Created by osdev on 3/1/26.
//

#pragma once

// Initializes shell and call shell_main()
void shell_init();

// Main shell loop
void shell_main();

/**
 * Executes command
 * @param command string with command to execute
 */
void shell_execute(char* command);