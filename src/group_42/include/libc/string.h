#pragma once
#include <libc/stdint.h>

/**
 * Get string length
 * @param str string
 * @return length of string
 */
size_t strlen(const char* str);

/**
 * Compares two strings
 * @param str1 string
 * @param str2 string
 * @return 0 if equal, difference str1-str2 if not
 */
size_t strcmp(const char* str1, const char* str2);