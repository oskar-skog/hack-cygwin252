#ifndef _HACK_H
#define _HACK_H

#define HACK_FORK_ATTEMPTS          50
#define HACK_DEBUG_FORK             0

// All kinds of logging for ReactOS CORE-18247
#define HACK_DEBUG_OPEN             0
#define HACK_DEBUG_STAT             0
#define HACK_DEBUG_READ             0

// Define buffer size
#define HACK_MAXLEN                 1024
// hack_print needs a mutex for some reason, timeout in milliseconds:
#define HACK_MUTEX_TIMEOUT          10

// Print string to the debug log
// Uses vsnprintf(3)
// Truncated to HACK_MAXLEN (adds notice about it in debug log)
void hack_print(const char * format, ...);

// Convert UTF-16 to UTF-8.  Needs pre-allocated buffer for dst.
// Silently truncates if input is too long.  Always appends NUL.
void hack_utf16_to_utf8(char * dst, size_t dst_size, LPWSTR src);

// PUNICODE_STRING (why?)
void hack_PUSTR_to_utf8(char * dst, size_t dst_size, PUNICODE_STRING src);

// Global variable (not thread or re-entrant safe if you fiddle with it)
// Disables debug log printing, used by hack_init to use POSIX functions
// that may have calls to hack_print.
extern bool hack_debug_enabled;

// Internal use
void hack_init(const char * cmd_name);  // Open debug log file
void hack_end(void);                    // Close debug log file

#ifdef _HACK_CC
#define MAXLEN              HACK_MAXLEN
#endif

#endif
