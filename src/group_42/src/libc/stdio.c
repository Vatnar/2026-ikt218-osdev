//
// Created by osdev on 2/19/26.
//

#include <libc/stdio.h>
#include <libc/string.h>
#include <driver/vga.h>
#include <libc/stdarg.h>
#include <libc/stddef.h>

typedef struct _IO_FILE {
    int fd;
    int flags;
    char *buf;
    char *rpos;
    char *wpos;
} _IO_FILE;

static char stdin_buf[BUF_SIZE];
static char stdout_buf[BUF_SIZE];

static _IO_FILE __stdin = {
    .fd = 0,
    .flags = _IO_READ,
    .buf = stdin_buf,

};

static _IO_FILE __stdout = {
    .fd = 1,
    .flags = _IO_WRITE,
    .buf = stdout_buf,

};

static _IO_FILE __stderr = {
    .fd = 2,
    .flags = _IO_WRITE | _IO_UNBUFFERED,
    .buf = NULL,

};


FILE* const stdin = &__stdin;
FILE* const stdout = &__stdout;
FILE* const stderr = &__stderr;


int putchar(int ic) {
    return fputc(ic, stdout);
}

int fflush(FILE* stream) {
    if (!stream->buf)
        return EOF;

    if (stream->wpos == stream->buf)
        return 0;

    char* p = stream->buf;
    while (p < stream->wpos) {
        vga_putc(*p);
        p++;
    }

    stream->wpos = stream->buf;

    return 0;
}

int fputc(int c, FILE* stream) {
    if (!stream->buf || (stream->flags & _IO_UNBUFFERED)) {
        vga_putc((char)c);
        return c;
    }

    if (!stream->wpos) {
        stream->wpos = stream->buf;
    }

    *stream->wpos++ = (char)c;

    // Flush (write) when newline or if the buffer verflows.
    if (c == '\n' || (stream->wpos - stream->buf) >= BUF_SIZE) {
        fflush(stream);
    }

    return c;
}

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vfprintf(stdout, format, args);
    va_end(args);
    fflush(stdout);
    return ret;
}

int fprintf(FILE *__restrict__ stream, const char *__restrict__ format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vfprintf(stream, format, args);
    va_end(args);
    if (!(stream->flags & _IO_UNBUFFERED)) {
        fflush(stream); // not standard c, but flushing after a printf ensures that e.g. "hello world" gets printed
    }
    return ret;
}

/**
 * Helper for printing decimal (base 10) numbers
 * @param value decimal value
 * @param stream file/stream to write to
 * @param count reference to amounf of printed characters
 */
static void print_decimal(unsigned long value, FILE* stream, int* count) {
    if ((long)value<0) {
        fputc('-', stream);
        value = (unsigned long)(-(long)value);
        (*count)++;
    }

    if (value / 10) {
        print_decimal(value / 10,  stream, count);
    }

    // Adding '0' converts the raw integer (0-9) to its ASCII character ('0'-'9')
    fputc(value % 10 + '0', stream);
    (*count)++;
}

/**
 * Helper for printing hexidecimal (hex; base 16) numbers
 * @param value hex number
 * @param stream file/stream to write to
 * @param count referente to amount of prinded characters
 */
static void print_hex(unsigned long value, FILE* stream, int* count) {
    if (value / 16) {
        print_hex(value / 16, stream, count);
    }

    unsigned long digit = value % 16;
    char c = (char)(digit < 10 ? digit + '0' : digit - 10 + 'a');
    fputc(c, stream);
    (*count)++;
}

int vfprintf ( FILE * stream, const char * format, va_list arg ) {
    int printed = 0;

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') {
            fputc(format[i], stream);
            printed++;
            continue;
        }

        i++;

        // Handle % for formatting
        // NOTE: does not support fractional numbers nor flags/modifiers/space/padding
        switch (format[i]) {
            case 'c': {
                char c = (char)va_arg(arg, int);
                fputc(c, stream);
                printed++;
                break;
            }

            case 's': {
                char *s = va_arg(arg, char *);

                // Safety check for NULL pointers
                if (s == NULL) {
                    s = "(null)";
                }

                while (*s != '\0') {
                    fputc(*s, stream);
                    s++;
                    printed++;
                }
                break;
            }

            case 'i':
            case 'd': {
                long d = va_arg(arg, long);
                print_decimal((unsigned long)d, stream, &printed);
                break;
            }

            case 'p': {
                void* ptr = va_arg(arg, void*);
                fputc('0', stream);
                fputc('x', stream);
                printed += 2;
                print_hex((unsigned long)ptr, stream, &printed);
                break;
            }
            case 'x': {
                const unsigned int x = va_arg(arg, unsigned int);
                print_hex(x, stream, &printed);
                break;
            }

            case 'f':
            case 'F': {
                // TODO: print floating point
                break;
            }

            case '%': {
                fputc('%', stream);
                printed++;
                break;
            }

            default: {
                fputc('%', stream);
                fputc(format[i], stream);
                printed += 2;
                break;
            }
        }
    }

    return printed;
}
