#include <stdio.h>
#include <string.h>

#include "libc/stdint.h"
#include <arch/i386/multiboot2.h>
#include <driver/keyboard.h>
#include <driver/vga.h>
#include <arch/i386/gdt.h>
#include <arch/i386/interrupt.h>
#include <driver/keyboard_map.h>
#include <driver/PCSPK.h>
#include <kernel/log.h>
#include <kernel/memory.h>
#include <kernel/pit.h>
#include <kernel/shell.h>

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end; // This is defined in arch/i386/linker.ld

void kernel_main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    vga_init();
    vga_new_line();

    init_gdt();
    init_idt();
    init_isr();
    init_irq();
    init_pit();
    PCSPK_init();
    init_keyboard();
    init_mm(&end);
    init_paging();
    KERNEL_LOG("Memory layout before malloc.\n");
    print_memory_layout();

    int* array = (int*)alloc_page();
    if (!array) {
        KERNEL_LOG_ERROR("Malloc failed!");
        __asm__ volatile("cli; hlt");
    }

    memset(array, 'a', sizeof(int) * 100);

    for (size_t i = 0; i < 100; i++) {
        printf("%c ", array[i]);
    }

    putchar('\n');
    KERNEL_LOG_INFO("Memory layout after malloc.\n");
    print_memory_layout();

    free_page(array);
    KERNEL_LOG_INFO("Memory layout after free.\n");
    print_memory_layout();

    vga_puts("Hello world from MyrstadOS\n");


    // Enable interrupts, and send interrupts 0,1,2
    __asm__ volatile("sti");

    shell_init();

    while (1) {
        // Traps the kernel in case shell crashes
        vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_WHITE);
        vga_set_line("TODO: shutdown system instead");
        vga_update_cursor();
        __asm__ __volatile__("hlt");
    }
}