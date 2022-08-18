#ifndef _HACK_H
#define _HACK_H

extern bool hack_debug_enabled;
void hack_init(int argc, const char * const * argv);
void hack_print(const char * format, ...);

#endif
