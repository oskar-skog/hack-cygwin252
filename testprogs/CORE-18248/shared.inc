#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FLUSH   do{ fflush(stdout); usleep(500000); }while(0);

char * const path = "//Vboxsvr/VM-shared/testfile";

#define DEL_TESTFILE do {                                       \
    printf("Deleting test file\n");                             \
    FLUSH;                                                      \
    system("rm '\\\\Vboxsvr\\VM-shared\\testfile'");            \
    printf("Starting test\n");                                  \
    FLUSH;                                                      \
} while (0);
