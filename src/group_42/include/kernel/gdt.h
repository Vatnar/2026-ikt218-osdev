//
// Created by osdev on 2/19/26.
//

#pragma once

#include <libc/stdbool.h>

/**
 * Initialize Global Descriptor Table (GDT)
 * @return true when successful and false on failure
 */
bool init_gdt();
