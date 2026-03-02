//
// Created by osdev on 2/24/26.
//

#include <kernel/idt.h>
#include <kernel/log.h>

#include <libc/stdint.h>
#include <libc/stdbool.h>

idt_entry_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void init_idt() {
    KERNEL_LOG_INFO("Initializing Interrupt Descriptor Table\n");
    idt_reg.base = (size_t)&idt[0];
    idt_reg.limit = (uint16_t)sizeof(idt_entry_t) * IDT_ENTRIES - 1;

    __asm__ volatile ("lidt %0" : : "m"(idt_reg));

    KERNEL_LOG_SUCCESS("Initialized Interrupt Descriptor Table\n");

}

void idt_load() {
    __asm__ volatile ("lidt %0" : : "m"(idt_reg));
}


void idt_set_descriptor(uint8_t n, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[n];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = KERNEL_CS; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}