//
// Created by osdev on 2/28/26.
//

#pragma once
#include <kernel/util.h>

#define KEYBOARD_BUFFER_SIZE 256

// Initialize keyboard with IRQ0
void init_keyboard();

/**
 * Handles keyboard presses, called for each keybooard interrupt.
 * @param regs registers pushed by IRS/IRQ handler
 */
void keyboard_callback(registers_t *regs);

/**
 * Push character to keyboard buffer
 * @param c char
 */
void keyboard_push(char c);

/**
 * Get last keyboard character pressed
 * @return 0 if empty or character
 */
char keyboard_pop();


