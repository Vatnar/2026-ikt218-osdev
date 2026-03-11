//
// Created by osdev on 2/28/26.
//

#pragma once
#include <libc/stdint.h>

/* Struct which aggregates many registers
 * Used for IRQ and ISR handlers */
typedef struct {
    uint32_t ds; /* Data segment selector */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
    uint32_t int_no, err_code; /* Interrupt number and error code (if applicable) */
    uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
} registers_t;

/**
 * Copies data to port
 * @param port port adress to be set
 * @param data data to set
 */
void port_byte_out(uint16_t port, uint8_t data);

/**
 * Retrieve byte at port adress
 * @param port port to retrive
 * @return byte at port
 */
uint8_t port_byte_in(uint16_t port);