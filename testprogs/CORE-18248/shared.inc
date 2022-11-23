#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Use UNC path to bypass CORE-18250
char * const path = "//Vboxsvr/VM-shared/testfile";

// Flush should be automatic if stdout is a terminal.
// The problem turns out to be that the break happens before the terminal
// gets around to display anything.
#define FLUSH   do{ fflush(stdout); usleep(500000); }while(0);

#define DEL_TESTFILE do {                                       \
    char *cmd = NULL;                                           \
    asprintf(&cmd, "rm '%s'", path);                            \
    printf("%s\n", cmd);                                        \
    FLUSH;                                                      \
    system(cmd);                                                \
    printf("Starting test\n");                                  \
    FLUSH;                                                      \
} while (0);
