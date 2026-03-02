//
// Created by osdev on 2/28/26.
//

#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/isr.h>
#include <kernel/log.h>
#include <kernel/util.h>



void init_irq() {
    KERNEL_LOG_INFO("Initializing Interrupt Request (IRQ)\n");
    // ICW1: Start the initialization sequence (in cascade mode)
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);

    // ICW2: Vector offsets (Remap IRQs to 32-47)
    port_byte_out(0x21, 0x20); // Master PIC offset = 32
    port_byte_out(0xA1, 0x28); // Slave PIC offset = 40 (32 + 8)

    // ICW3: Tell Master PIC there is a slave PIC at IRQ2 (0000 0100)
    port_byte_out(0x21, 0x04);
    // ICW3: Tell Slave PIC its cascade identity (0000 0010)
    port_byte_out(0xA1, 0x02);

    // ICW4: Have the PICs use 8086 mode (instead of 8080 mode)
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);

    // Masking: 0x00 enables all IRQs, 0xFF masks all.
    // Usually, you start by masking everything except the ones you've handled.
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);

    // IRQ0 is ISR32, and there is 16 IRQs therefore i + IRQ, i in [0,32>
    for (int i = 0; i < 16; i++) {
        idt_set_descriptor(i + IRQ0, isr_stub_table[i + IRQ0], 0x8E);
    }

    KERNEL_LOG_SUCCESS("Initialized Interrupt Request (IRQ)\n");
}

void irq_handler(registers_t *r) {
    const uint32_t int_num = r->int_no;         // interrupt [0, 255]
    const uint32_t irq_num = r->int_no - IRQ0;  // irq [0, 15]
    const uint32_t error_code = r->err_code;    // error code

    // Handle IRQ using callback already set
    if (irq_routines[irq_num] != 0) {
        irq_routines[irq_num](r);
    } else {
        // note: do not hang the system, IRQs are not as critical as ISRs 0 though 31
        KERNEL_LOG_WARN("IRQ %d not created!\n", irq_num);
    }

    if (int_num >= 40) {
        port_byte_out(0xA0, 0x20);  // Send EOI
    }

    port_byte_out(0x20, 0x20);      // Send to master PIC
}

void register_irq_routine(const uint8_t n, const irq_handler_t handler) {
    irq_routines[n] = handler;
}
void remove_irq_routine(const uint8_t n) {
    irq_routines[n] = 0;
}
