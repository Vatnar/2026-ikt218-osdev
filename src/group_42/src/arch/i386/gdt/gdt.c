//
// Created by osdev on 2/19/26.
//

#include <arch/i386/gdt.h>
#include <kernel/log.h>

#include <libc/stdbool.h>
#include<libc/stdint.h>
#include <libc/stdio.h>

extern void gdt_flush(uint32_t descriptor_address);
extern void gdt_reload_segments(void);

struct __attribute__((packed)) gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
};

struct __attribute__((packed)) gdt_ptr {
    uint16_t limit;
    uint32_t base;
};

static struct gdt_entry gdt[5];   // 0: null, 1: kernel code, 2: kernel data, 3: user code, 4: user data
static struct gdt_ptr   gdt_desc;

static void gdt_set_entry(const int i,
                          const uint32_t base,
                          const uint32_t limit,
                          const uint8_t access,
                          const uint8_t gran)
{
    gdt[i].base_low    =  base        & 0xFFFF;
    gdt[i].base_middle = (base >> 16) & 0xFF;
    gdt[i].base_high   = (base >> 24) & 0xFF;

    gdt[i].limit_low   =  limit       & 0xFFFF;
    gdt[i].granularity = (limit >> 16) & 0x0F;

    gdt[i].granularity |= gran & 0xF0;
    gdt[i].access       = access;
}

bool init_gdt(void)
{
    KERNEL_LOG_INFO("Initialising Global Descriptor Table...\n");

    // GDT size in bytes minus 1
    gdt_desc.limit = sizeof(gdt) - 1;
    gdt_desc.base  = (uint32_t)&gdt[0];

    // 0: null descriptor
    gdt_set_entry(0, 0, 0, 0, 0);

    // 1: kernel code (selector 0x08)
    gdt_set_entry(1,
                  0,
                  0xFFFFF,
                  0x9A,   // ring 0 code
                  0xCF);  // granularity

    // 2: kernel data (selector 0x10)
    gdt_set_entry(2,
                  0,
                  0xFFFFF,
                  0x92,   // ring 0 data
                  0xCF);

    // 3: user code (selector 0x18)
    gdt_set_entry(3,
                  0,
                  0xFFFFF,
                  0xFA,   // ring 3 code
                  0xCF);

    // 4: user data (selector 0x20)
    gdt_set_entry(4,
                  0,
                  0xFFFFF,
                  0xF2,   // ring 3 data
                  0xCF);

    // Load GDTR: limit, base, offset=0
    gdt_flush((uint32_t)&gdt_desc);

    // Reload segment registers (CS=0x08, DS/ES/FS/GS/SS=0x10)
    gdt_reload_segments();


    // quick sanity test for gdt
    uint32_t* ptr = (uint32_t*)0x100000;
    *ptr = 0xCAFEBABE;

    if (*ptr != 0xCAFEBABE) {
        KERNEL_LOG_ERROR("GDT: Something went wrong, got 0x%x but expected 0x%x\n", *ptr, 0xCAFEBABE);

        return false;
    }

    //printf("Successfully Initialised Global Descriptor Table.\n");
    KERNEL_LOG_SUCCESS("Successfully Initialised Global Descriptor Table.\n");
    return true;
}
