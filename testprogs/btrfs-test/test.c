#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    struct stat buf;
    stat("/cygdrive/c", &buf);
    
    errno = 0;
    stat("/cygdrive/c/", &buf);
    
    errno = 0;
    stat("/cygdrive/d", &buf);
    
    errno = 0;
    stat("/cygdrive/d/", &buf);
    
    return 0;
}
