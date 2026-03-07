#include "libc/stdint.h"
#include <arch/i386/multiboot2.h>
#include <driver/keyboard.h>
#include <driver/vga.h>
#include <arch/i386/gdt.h>
#include <arch/i386/interrupt.h>
#include <driver/keyboard_map.h>
#include <kernel/shell.h>

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

void void_function(){}

int kernel_main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    vga_init();
    vga_new_line();

    init_gdt();
    init_idt();
    init_isr();
    init_irq();
    init_keyboard();
    vga_puts("Hello world from MyrstadOS\n");

    // Register empty interrupt handlers so th
    register_irq_routine(0, void_function);
    register_interrupt_handler(0, void_function);
    register_interrupt_handler(1, void_function);
    register_interrupt_handler(2, void_function);

    // Enable interrupts, and send interrupts 0,1,2
    __asm__ volatile("sti");
    __asm__ volatile("int $0");
    __asm__ volatile("int $1");
    __asm__ volatile("int $2");
    // __asm__ volatile("int $8");

    vga_update_cursor();

    shell_init();

    vga_putc(0xED);
    vga_puts("\x92rlig");

    while (1) {
        key_event_t ev;
        if (keyboard_get_event(&ev)) {
            if (ev.state == KEY_PRESS) {
                uint16_t key = keyboard_translate_no(ev);

                if (key > 1 && key < 0x100 && !ev.extended)
                    vga_putc((char)key);
                else {

                }
                vga_update_cursor();
            }
        }
        __asm__ __volatile__("hlt");
    }
}