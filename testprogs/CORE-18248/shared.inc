#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char * const path = "//Vboxsvr/VM-shared/testfile";
// Cygwin's bash will call the Windows del:
#define DEL_TESTFILE system("del '\\\\Vboxsvr\\VM-shared\\testfile'");
