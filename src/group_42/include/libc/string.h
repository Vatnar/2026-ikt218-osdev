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

/**
 * Copy string from source to destination
 * @param dest string reference to save to
 * @param src string reference to save from
 * @return destination is returned
 */
char* strcpy(char* dest, const char* src);

/**
 * Sets the first num bytes of the block of memory pointed by ptr to the specified value
 * @param ptr Pointer to the block of memory to fill.
 * @param value Value to be set
 * @param num Number of bytes to be set to the value.
 * @return ptr is returned
 */
void* memset ( void* ptr, int value, size_t num );