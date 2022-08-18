#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "winlean.h"

#include "hack.h"
#define MAXLEN 1024

// POSIX functions may only be used until hack_debug_enabled is set to true.
bool hack_debug_enabled = false;

static HANDLE debug_log;

void hack_init(int argc, const char * const * argv)
{
    // Get ISO8601 timestamp
    char timestamp[20];
    time_t t = time(NULL);
    struct tm * tmp = gmtime(&t);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", tmp);
    // Produce filename
    char *filename;
    asprintf(
        &filename, "C:\\cygdbg\\%s-[%s]-%d.txt",
        timestamp_s, argv[0], getpid()
    );
    // Open log file
    debug_log = CreateFile(
                    filename,
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
    if (debug_log == INVALID_HANDLE_VALUE) {
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
