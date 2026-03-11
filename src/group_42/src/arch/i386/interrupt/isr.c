//
// Created by osdev on 2/28/26.
//

#include <arch/i386/interrupt.h>
#include <kernel/log.h>
#include <kernel/util.h>

isr_t interrupt_handlers[256];

extern void* isr_stub_table[32];


/* To print the message which defines every exception */
static char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void init_isr() {
    KERNEL_LOG_INFO("Initializing Interrupt Service Routines...\n");

    // 0x8E is interrupt gate, is 0x8F trap gate
    for (int i = 0; i < 32; i++) {
        idt_set_descriptor(i, isr_stub_table[i], 0x8E); // since IDT is set, no need to set it again.
    }

    KERNEL_LOG_SUCCESS("Initialized Interrupt Service Routines\n");

}

void isr_handler(registers_t *r) {
    const uint32_t int_num = r->int_no;         // interrupt number
    const uint32_t error_code = r->err_code;    // error code (if applicable)
    KERNEL_LOG_INFO("Handling exception int %d err %d:", int_num, error_code);

    // Is predifined by CPU, and has exeption messages
    if (int_num < 32) {
        KERNEL_LOG(": %s", exception_messages[int_num]);
    }
    KERNEL_LOG("\n");

    // If handler is registered use it
    if (interrupt_handlers[int_num] != 0) {
        interrupt_handlers[int_num](r);
        return; // critical return, else all ISRs will cause kernel panic
    } else {
        KERNEL_LOG_WARN("No interrupt handler registered for interrupt %d!\n", int_num);
    }

    // Kernel panic, since interrupt is not handled!
    KERNEL_LOG_ERROR("KERNEL PANIC! HALTING....");
    __asm__ volatile ("hlt");
}

void register_interrupt_handler(const uint8_t n, const isr_t handler) {
    interrupt_handlers[n] = handler;
}

void remove_interrupt_handler(const uint8_t n) {
    interrupt_handlers[n] = 0;
}