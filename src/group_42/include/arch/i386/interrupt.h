//
// Created by osdev on 3/7/26.
//

#pragma once
#include <kernel/util.h>
#include <libc/stdint.h>
/**
 * Interrupt Descriptor Table (IDT) headers
 */

// Initializes Interrupt Descriptor Table (IDT)
void init_idt();

// Loades IDT into memory
void idt_load();

/**
 * Creates, sets and defines IDT entry defined by vector (n-th)
 * @param vector which IRS/IRQ to set
 * @param isr uint32_t(void*)
 * @param flags flags
 */
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);


/*
 * Interrupt ReQuests (IRQs) headers
 */



typedef void (*irq_handler_t)(registers_t *r);

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


/**
 * Interrupt Service Routine (ISR) headers
 */

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