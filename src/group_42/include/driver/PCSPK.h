//
// Created by osdev on 3/12/26.
//

#pragma once
#include <stdint.h>

// Initialize PC SPeaKer (PCSPK)
void PCSPK_init();

/**
 * Play frequency in hertz
 * @param frequency frequency in hertz (s^-1)
 */
void PCSPK_play(uint32_t frequency);

// Stop PC Speaker from playing audio
void PCSPK_stop();