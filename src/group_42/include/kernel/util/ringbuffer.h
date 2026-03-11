//
// Created by osdev on 3/7/26.
// Circular queue implementation
//

#pragma once
#include <stdbool.h>

// Ringbuffer struct which supports characters, strings and 32bit using unions
typedef struct {
    union {
        uint8_t *uint8_ptr;
        uint16_t *uint16_ptr;
        uint32_t *uint32_ptr;
    } data;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
    enum { RB_UINT8, RB_UINT16, RB_UINT32 } type;
} ringbuffer_t;

/**
 * Initializes ringbuffer structure
 * @param rb ringbuffer reference
 * @param array array reference
 * @param cap capacity
 * @param type enum: UINT_8 (CHAR), UINT_32
 */
void rb_init(ringbuffer_t *rb, void *array, size_t cap, int type);

/**
 * Push value to the front of the ringbuffer
 * @param rb ringbuffer reference
 * @param val value to add/push
 * @return true if successful
 */
bool rb_push(ringbuffer_t *rb, void *val);

/**
 * Pops/dequeues the first element added
 * @param rb ringbuffer reference
 * @param out_val output reference
 * @return true if successful
 */
bool rb_pop(ringbuffer_t *rb, void *out_val);

/**
 * Check if ringbuffer is full
 * @param rb ringbuffer reference
 * @return true if full
 */
bool rb_is_full(const ringbuffer_t *rb);

/**
 * Check if the ringbuffer is empty
 * @param rb ringbuffer reference
 * @return true if empty
 */
bool rb_is_empty(const ringbuffer_t *rb);

/**
 * Peeks and outputs the last element added
 * @param rb ringbuffer reference
 * @param out_val reference to output where peeked value gets placed
 * @return true if successful
 */
bool rb_peek(const ringbuffer_t *rb, void *out_val);