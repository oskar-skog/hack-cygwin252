#include <stdarg.h>
#include <stdio.h>

#include "hack.h"
#define MAXLEN 1024

bool hack_debug_enabled = false;

void hack_init(int argc, const char * const * argv)
{
    // Proof of concept
    // fputs("Hello world!\n", stderr);
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
}
