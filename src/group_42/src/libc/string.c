//
// Created by osdev on 2/19/26.
//

#include <libc/string.h>

size_t strlen(const char *str) {
    // should probably include a max size, incase of no null character \0
    size_t length = 0;
    while (str[length]!='\0') length++;
    return length;
}

size_t strcmp(const char *str1, const char *str2) {
    // if *str1 is false, str1 is \0. continues until str1 and str2 are different
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    // Returns difference of str1 and str2, 0 ufif equal
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}