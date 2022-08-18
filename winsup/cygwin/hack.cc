#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "winlean.h"

#include "hack.h"
#define MAXLEN 1024

// POSIX functions may only be used until hack_debug_enabled is set to true.
bool hack_debug_enabled = false;

static HANDLE debug_log;

// Sanitize filenames (not paths), modifies supplied string.
static void sanitize(char *s)
{
    for (; char c = s[0]; s++) {
        if (c < 32 || c > 126)
            s[0] = '_';
        switch (c) {
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

void hack_init(int argc, const char * const * argv)
{
    // Disable debug logging to allow using POSIX functions
    hack_debug_enabled = false;
    // Get ISO8601 timestamp
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
    fprintf(stderr, "Path: %s\n", path);        // REMOVE
    // Open log file
    // CreateFileA takes a LPCSTR (8-bit char string) filename
    debug_log = CreateFileA(
                    path,
                    GENERIC_WRITE,  // dwDesiredAccess
                    0,              // dwShareMode      7 for POSIXy
                    NULL,           // lpSecurityAttribytes
                    CREATE_ALWAYS,  // dwCreationDisposition
                    // dwFlagsandAttributes:
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
                    // hTemplateFile
                    NULL
    );
    free(filename);
    free(path);
    if (debug_log == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "debug_log == INVALID_HANDLE_VALUE\n");     // REMOVE
        fprintf(stderr, "%lx\n", (long) GetLastError());            // REMOVE
        return;
    }
    // Init completed
    hack_debug_enabled = true;
}

static void no_op(...)
{
}

void hack_print(const char *format, ...)
{
    if (!hack_debug_enabled)
        return;
    char buf[MAXLEN];
    va_list args;
    va_start(args, format);
    bool truncated = vsnprintf(buf, MAXLEN, format, args) > MAXLEN;
    va_end(args);
    // -Werror for unused variables
    no_op(truncated);
    // Write to debug_log
}
