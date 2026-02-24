//
// Created by osdev on 2/19/26.
//

#pragma once

typedef enum {
    KLOG,
    KLOG_INFO,
    KLOG_WARN,
    KLOG_ERROR,
    KLOG_SUCCESS
} klog_level_t;

/**
 * Kernel logging that uses printf for formatting.
 *
 * Uses stdout for info warn and success and stderr for error
 * @param level klog_level_t: info, warn, error and success
 * @param fmt formatted string
 * @param ... args
 */
void kernel_log(klog_level_t level, const char *fmt, ...);

// Macros to easier call kernel log types

// eg KERNEL_LOG_INFO("Hello, %s!\n", "world)
// instead of kernel_log(KLOG_INFO, "Hello, %s!\n", "world)

#define KERNEL_LOG_INFO( fmt, ...) kernel_log(KLOG_INFO, fmt, ##__VA_ARGS__)
#define KERNEL_LOG_ERROR( fmt, ...) kernel_log(KLOG_ERROR, fmt, ##__VA_ARGS__)
#define KERNEL_LOG_WARN( fmt, ...) kernel_log(KLOG_WARN, fmt, ##__VA_ARGS__)
#define KERNEL_LOG_SUCCESS( fmt, ...) kernel_log(KLOG_SUCCESS, fmt, ##__VA_ARGS__)
#define KERNEL_LOG( fmt, ... ) kernel_log(KLOG, fmt, ##__VA_ARGS__)
