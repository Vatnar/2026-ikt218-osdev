#pragma once
#include <libc/stdbool.h>
#include <libc/stdint.h>

#include <libc/stdarg.h>

typedef struct _IO_FILE FILE;

// Standard Input
#define stdin (stdin) // file descriptor 0
// Standard Output
#define stdout (stdout) // file descriptor 1
// Standard Error Output
#define stderr (stderr) // file decriptor 2

#define _IO_READ 1
#define _IO_WRITE 2
#define _IO_UNBUFFERED 4
#define _IO_EOF_SEEN 8
#define _IO_ERR_SEEN 16

#define EOF (-1)

extern FILE* const stdin;
extern FILE* const stdout;
extern FILE* const stderr;

#define BUF_SIZE 1024

/**
 * file flush, write out any unwritten data from buffer to stream
 * @param stream stream
 * @return 0 on success EOF on error
 */
int fflush(FILE* stream);

/**
 * print character to filestream
 * @param c char to write
 * @param f filestream to write to
 * @return character writter or EOF for erros
 */
int fputc(int c, FILE *f);

/**
 * equivalent to fputc with stdout as paramater
 * @param ic char to be written
 * @return char written or EOF if error
 */
int putchar(int ic);

/**
 * formatted print
 * @param format formatted string
 * @param ... args
 * @return number of characters written
 */
int printf(const char* __restrict__ format, ...);

/**
 *
 * @param stream stream to write to
 * @param format formatted string
 * @param ... args
 * @return number of characters written
 */
int fprintf(FILE *__restrict__ stream, const char *__restrict__ format, ...);

/**
 * variable (argument list) formatted (string) print file
 * @param stream stream to write to
 * @param format formatted string
 * @param arg va_list of arguments
 * @return number of characters written
 */
int vfprintf ( FILE * stream, const char * format, const va_list arg );