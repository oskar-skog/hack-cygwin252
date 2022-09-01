#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    struct stat buf;
    stat("/cygdrive/c/", &buf);
    stat("/cygdrive/d/", &buf);
}
