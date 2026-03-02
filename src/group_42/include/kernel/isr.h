//
// Created by osdev on 2/24/26.
//

#pragma once

#include <libc/stdint.h>
#include <kernel/util.h>

/* ISRs reserved for CPU exceptions */
extern void* isr_stub_table[32];


// Initialize Interrupt Service Routines (ISRs)
void init_isr();

/**
 * Handle Interrupt
 * @param r registers pushed by ISRs assembly code
 */
void isr_handler(registers_t *r);

// isr_t is a function wrapper that incorporates registers_t in its signature
typedef void (*isr_t)(registers_t*);

/**
 * Register callback for the n-th interrupt
 * @param n interrupt number
 * @param handler callback function
 */
void register_interrupt_handler(uint8_t n, isr_t handler);

/**
 * Removes the n-th interrupt handler/callback
 * @param n interrupt number
 */
void remove_interrupt_handler(uint8_t n);