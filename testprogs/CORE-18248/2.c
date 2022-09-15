#include "shared.inc"

int main()
{
    DEL_TESTFILE;
    // Create file for reading and writing
    int fd;
    fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        printf("First call to open(2) failed, errno = %d (%s)\n",
               errno, strerror(errno));
        return 1;
    }
    close(fd);
    // Open file again
    fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        printf("Second call to open(2) failed, errno = %d (%s)\n",
               errno, strerror(errno));
    }
    close(fd);
    return 0;
}
