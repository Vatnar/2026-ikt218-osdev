#include "libc/stdint.h"
#include <multiboot2.h>
#include <driver/vga.h>
#include <kernel/gdt.h>
#include <libc/stdio.h>

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};



int kernel_main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    vga_init();
    vga_new_line();

    init_gdt();

    printf("\nHello World from group_42-OS");

    return 0;
}