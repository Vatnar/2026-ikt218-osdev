//
// Created by osdev on 3/12/26.
//

#pragma once
#include <stdint.h>

// Initlialize Programmable Interrupt Timer (PIT)
void init_pit();

/**
 * Sleep using interrupts (low CPU)
 * @param milliseconds ms to sleep
 */
void sleep_interrupt(uint32_t milliseconds);

/**
 * Sleep using busy waiting (high CPU)
 * @param milliseconds ms to sleep
 */
void sleep_busy(uint32_t milliseconds);