#ifndef _HACK_H
#define _HACK_H

#define HACK_DEBUG_OPEN     1

// Define buffer size
#define HACK_MAXLEN 1024

// Print string to the debug log
// Uses vsnprintf(3)
// Truncated to HACK_MAXLEN (adds notice about it in debug log)
void hack_print(const char * format, ...);

// Convert UTF-16 to UTF-8.  Needs pre-allocated buffer for dst.
// Silently truncates if input is too long.  Always appends NUL.
void hack_utf16_to_utf8(char * dst, size_t dst_size, LPWSTR src);

// Global variable (not thread or re-entrant safe if you fiddle with it)
// Disables debug log printing, used by hack_init to use POSIX functions
// that may have calls to hack_print.
extern bool hack_debug_enabled;

// Internal use
void hack_init(int argc, const char * const * argv);    // Open debug log file
void hack_end(void);                                    // Close debug log file

#ifdef _HACK_CC
#define MAXLEN HACK_MAXLEN
#endif

#endif
