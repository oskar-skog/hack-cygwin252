#define _GNU_SOURCE     // for asprintf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static const char *progname = "__forktest__";
static const int runs = 10000;

int main(int argc, char **argv)
{
    // Make sure program is launched as `progname`
    if (strcmp(argv[0], progname))
        execl(argv[0], progname, NULL);

    // Test fork
    for (int i = 0; i < runs; i++) {
        if (!fork())
            exit(0);
        else
            wait(NULL);
    }

    // Move log file to current working directory
    char *cmd;
    asprintf(&cmd, "mv /cygdrive/c/cygdbg/*-\\[%s\\]-* .", progname);
    system(cmd);

    return 0;
}
