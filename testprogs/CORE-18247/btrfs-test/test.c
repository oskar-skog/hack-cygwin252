#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    struct stat buf;
    
    //  NTFS - FAT32
    errno = 0;
    stat("/cygdrive/c", &buf);
    errno = 0;
    stat("/cygdrive/c/", &buf);
    
    // btrfs - btrfs
    errno = 0;
    stat("/cygdrive/d", &buf);
    errno = 0;
    stat("/cygdrive/d/", &buf);
    
    // FAT32 - cdrom
    errno = 0;
    stat("/cygdrive/e", &buf);
    errno = 0;
    stat("/cygdrive/e/", &buf);
    
    return 0;
}
