//
// Created by osdev on 2/24/26.
//

#pragma once

#include <kernel/util.h>

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#define IRQ_COUNT 16

// defined in assembly
extern void* irq_stub_table[IRQ_COUNT];

// irq_t is a function wrapper that incorporates registers_t in its signature
typedef void (*irq_handler_t)(registers_t *r);
static irq_handler_t irq_routines[16] = {0}; // One for each IRQ (0-15)

// Initializes Interrupt ReQuests (IRQs)
void init_irq();

/**
 * Handles interrupt requests
 * @param r registers pushed by IRQ handler
 */
void irq_handler(registers_t *r);

/**
 * Register n-th IRQ callback
 * @param n IRQ number
 * @param handler callback function
 */
void register_irq_routine(uint8_t n, irq_handler_t handler);

/**
 * Removes n-th IRQ routine/callback
 * @param n IRQ number
 */
void remove_irq_routine(uint8_t n);
