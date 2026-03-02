//
// Created by osdev on 2/24/26.
//

#pragma once
#include <libc/stdint.h>

#define IDT_ENTRIES 256
#define KERNEL_CS 0x08

typedef struct {
    uint16_t    isr_low;      // The lower 16 bits of the ISR's address
    uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t     reserved;     // Set to zero
    uint8_t     attributes;   // Type and attributes; see the IDT page
    uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;


typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;



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
