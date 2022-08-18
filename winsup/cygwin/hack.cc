#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "winlean.h"

#define _HACK_CC
#include "hack.h"
// hack.h defines MAXLEN

// POSIX functions may only be used until hack_debug_enabled is set to true.
bool hack_debug_enabled;

// debug log file
static HANDLE debug_log;

// Sanitize filenames (not paths), modifies supplied string.
static void sanitize(char *s)
{
    for (; s[0]; s++) {
        if (s[0] < 32 || s[0] > 126)
            s[0] = '_';
        switch (s[0]) {
            case '<':
            case '>':
            case ':':
            case '"':
            case '/':
            case '\\':
            case '|':
            case '?':
            case '*':
            case '^':
                s[0] = '_';
        }
    }
}

// Open the log file
// This function is called immediately before main() so all POSIX functions
// can be used until hack_debug_enabled = true.
void hack_init(int argc, const char * const * argv)
{
    // Disable debug logging to allow using POSIX functions
    hack_debug_enabled = false;
    // Generate timestamp
    char timestamp[20];
    time_t t = time(NULL);
    struct tm * tmp = gmtime(&t);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H.%M.%S", tmp);
    // Produce filename
    char *filename, *path;
    asprintf(
        &filename, "%s-[%s]-%d.txt",
        timestamp, argv[0], (int) getpid()
    );
    sanitize(filename);
    asprintf(&path, "C:\\cygdbg\\%s", filename);
    // Open log file
    // CreateFileA takes a LPCSTR (8-bit char string) filename
    debug_log = CreateFileA(
                    path,
                    GENERIC_WRITE,  // dwDesiredAccess
                    0,              // dwShareMode      7 for POSIXy
                    NULL,           // lpSecurityAttribytes
                    CREATE_ALWAYS,  // dwCreationDisposition
                    // dwFlagsandAttributes:
                    FILE_ATTRIBUTE_NORMAL, // | FILE_FLAG_WRITE_THROUGH,
                    // hTemplateFile
                    NULL
    );
    // Init completed
    free(filename);
    free(path);
    // Only set to true if the file actually exists
    if (debug_log != INVALID_HANDLE_VALUE)
        hack_debug_enabled = true;

    // REMOVE test code
    char test[5];
    LPWSTR codepoint10437[] = {0xd801, 0xdc37, 0};
    hack_utf16_to_utf8(test, sizeof(test), codepoint10437);
    hack_print("Test %s\r\n", test);
}

// Write NUL terminated string to debug_log, does not flush
static void writestr(const char *str)
{
    DWORD bytes_written = 0;
    DWORD bytes_to_write = strlen(str);
    DWORD tmp;
    while (bytes_written < bytes_to_write) {
        WriteFile(
            debug_log,
            str+bytes_written,
            bytes_to_write-bytes_written,
            &tmp,
            NULL
        );
        bytes_written += tmp;
    }
}

// Basically a printf(3) that writes to debug_log without calling any unix
// syscalls.  Only uses stdarg.h macros and vsnprintf(3)
void hack_print(const char *format, ...)
{
    if (!hack_debug_enabled)
        return;
    char buf[MAXLEN];
    va_list args;
    va_start(args, format);
    bool truncated = vsnprintf(buf, MAXLEN, format, args) > MAXLEN;
    va_end(args);
    writestr(buf);
    if (truncated)
        writestr("!!!TRUNCATED!!!\r\n");
    FlushFileBuffers(debug_log);
}

// Called by do_exit in winsup/cygwin/dcrt0.cc
// stdio cleanup may have occured
void hack_end()
{
    CloseHandle(debug_log);
}

static void push_utf8(
    char *_dst,
    size_t dst_size,
    size_t &dst_index,
    int codepoint)
{
    // Need to be unsigned for math to not be crazy, but I'm too lazy
    // to write unsigned char all the time
    unsigned char *dst = (unsigned char *) _dst;
    // -2 to preserve a single byte for a trailing NUL
    size_t freespace = dst_size - dst_index - 2;
    if (codepoint == 0) {
        if (freespace >= 0)
            dst[dst_index++] = 0;
    } else if (codepoint < 1<<7) {
        if (freespace >= 1) {
            dst[dst_index++] = codepoint;
        }
    } else if (codepoint < 1<<11) {
        if (freespace >= 2) {
            dst[dst_index++] = 0xc0 | (0x1f & (codepoint >> 6));
            dst[dst_index++] = 0x80 | (0x3f & codepoint);
        }
    } else if (codepoint < 1<<16) {
        if (freespace >= 3) {
            dst[dst_index++] = 0xd0 | (0x0f & (codepoint >> 12));
            dst[dst_index++] = 0x80 | (0x3f & (codepoint >> 6));
            dst[dst_index++] = 0x80 | (0x3f & codepoint);
        }
    } else {
        if (freespace >= 4) {
            dst[dst_index++] = 0xe0 | (0x07 & (codepoint >> 18));
            dst[dst_index++] = 0x80 | (0x3f & (codepoint >> 12));
            dst[dst_index++] = 0x80 | (0x3f & (codepoint >> 6));
            dst[dst_index++] = 0x80 | (0x3f & codepoint);
        }
    }
}

void hack_utf16_to_utf8(char *dst, size_t dst_size, LPWSTR src)
{
    size_t dst_index = 0;
    int in_char, tmp, codepoint;
    while ((in_char = *src++)) {
        if (in_char >= 0xd800 && in_char <= 0xdbff) {
            tmp = in_char;
            in_char = *src++;
            if (in_char >= 0xdc00 && in_char <= 0xdfff) {
                // Valid surrogate pair
                codepoint = ( (tmp&0x3ff)<<10 | (in_char&0x3ff) ) + 0x10000;
                push_utf8(dst, dst_size, dst_index, codepoint);
            } else {
                // Invalid pair: valid high surrogate, invalid low surrogate
                push_utf8(dst, dst_size, dst_index, tmp);
                push_utf8(dst, dst_size, dst_index, in_char);
            }
        } else {
            // BMP or lone low surrogate
            push_utf8(dst, dst_size, dst_index, in_char);
        }
    }
    // Terminate string
    push_utf8(dst, dst_size, dst_index, 0);
    dst[dst_size - 1] = 0;
}

