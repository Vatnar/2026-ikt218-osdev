//
// Created by osdev on 3/7/26.
//

#include <stdio.h>
#include <kernel/util/ringbuffer.h>

void rb_init(ringbuffer_t *rb, void *array, const size_t cap, const int type) {
    rb->capacity = cap;
    rb->type = type;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;

    // Assign the void pointer to the correct union member
    if (type == RB_UINT8) {
        rb->data.uint8_ptr = (uint8_t *)array;
    } else if (type == RB_UINT16) {
        rb->data.uint16_ptr = (uint16_t *)array;
    } else {
        rb->data.uint32_ptr = (uint32_t *)array;
    }
}

// Push: Uses a switch to handle the specific pointer arithmetic
bool rb_push(ringbuffer_t *rb, void *val) {
    if (rb->count == rb->capacity) return false;

    switch (rb->type) {
        case RB_UINT8:
            rb->data.uint8_ptr[rb->head] = *(uint8_t*)val;
            break;
        case RB_UINT16:
            rb->data.uint16_ptr[rb->head] = *(uint16_t*)val;
            break;
        case RB_UINT32:
            rb->data.uint32_ptr[rb->head] = *(uint32_t*)val;
            break;
    }

    rb->head = (rb->head + 1) % rb->capacity;
    rb->count++;
    return true;
}

// Pop: Copies the value back out to the provided pointer
bool rb_pop(ringbuffer_t *rb, void *out_val) {
    if (rb->count == 0) return false;

    switch (rb->type) {
        case RB_UINT8:
            *(uint8_t*)out_val = rb->data.uint8_ptr[rb->tail];
            break;
        case RB_UINT16:
            *(uint16_t*)out_val = rb->data.uint16_ptr[rb->tail];
            break;
        case RB_UINT32:
            *(uint32_t*)out_val = rb->data.uint32_ptr[rb->tail];
            break;
    }

    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count--;
    return true;
}

bool rb_is_full(const ringbuffer_t *rb) {
    return rb->count == rb->capacity;
}

bool rb_is_empty(const ringbuffer_t *rb) {
    return rb->count == 0;
}

bool rb_peek(const ringbuffer_t *rb, void *out_val) {
    if (rb->count == 0) return false;

    switch (rb->type) {
        case RB_UINT8:
            *(uint8_t*)out_val = rb->data.uint8_ptr[rb->tail];
            break;
        case RB_UINT16:
            *(uint16_t*)out_val = rb->data.uint16_ptr[rb->tail];
            break;
        case RB_UINT32:
            *(uint32_t*)out_val = rb->data.uint32_ptr[rb->tail];
            break;
    }
    return true;
}