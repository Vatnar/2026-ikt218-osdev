    //
// Created by osdev on 3/12/26.
//

#pragma once
#include <stddef.h>
#include <stdint.h>


typedef struct {
    uint8_t status;
    uint32_t size;
} alloc_t;

typedef struct {
    uint32_t last_alloc;
    uint32_t heap_end;
    uint32_t heap_begin;
    uint32_t memory_used;

    uint32_t pheap_begin;
    uint32_t pheap_end;
    uint8_t* pheap_desc;

} memory_info_t;

/**
 * Initialize memory management (memory_info_t struct)
 * @param kernel_end where kernel ends
 */
void init_mm(const uint32_t* kernel_end);

// Initilialize paging
void init_paging();

/**
 * Map virtual page to physical page for paging
 * @param virt virtual page address
 * @param phys physical page adress
 */
void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys);

/* Function declarations for memory allocation */
extern char* pmalloc(size_t size); /* Allocates memory of given size with page alignment */
extern void* malloc(size_t size); /* Allocates memory of given size */
extern void free(void *mem); /* Frees memory previously allocated */

/**
 * Allocate a page.
 * @return pointer to page, NULL for failure
 */
char* alloc_page();

/**
 * Free memory/page
 * @param mem page to be freed
 */
void free_page(void* mem);


void* memset16(void* ptr, uint16_t value, size_t num);

void print_memory_layout();
