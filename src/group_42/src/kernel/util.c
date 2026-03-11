//
// Created by osdev on 2/28/26.
//
#include <kernel/util.h>

void port_byte_out(uint16_t port, uint8_t data){
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

uint8_t port_byte_in(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}