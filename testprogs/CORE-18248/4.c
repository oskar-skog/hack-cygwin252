#include "shared.inc"

int main()
{
    DEL_TESTFILE;
    // Open file for reading and writing
    int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        printf("open(2) failed, errno = %d (%s)\n",
               errno, strerror(errno));
        return 1;
    }
    // Write to opened file
    char *message = "Test";
    int result = write(fd, message, strlen(message));
    if (result != strlen(message)) {
        printf("write(2) returned %d, errno = %d (%s)\n",
               result, errno, strerror(errno));
        return 1;
    }
    // Verify that message is present
    result = lseek(fd, 0, SEEK_SET);
    if (result != 0) {
        printf("lseek(2) returned %d, errno = %d (%s)\n"
               result, errno, strerror(errno));
        return 1;
    }
    char *buf = malloc(strlen(message) + 1);
    if (!buf) {
        printf("malloc failed\n");
        return 1;
    }
    buf[strlen(message)] = 0;
    result = read(fd, buf, strlen(message));
    if (result != strlen(message)) {
        printf("lseek(2) returned %d, errno = %d (%s)\n"
               result, errno, strerror(errno));
        return 1;
    }
    if (strcmp(message, buf) != 0) {
        printf("Read data does not match, got \"%s\"\n", buf);
        return 1;
    }
    close(fd);
    return 0;
}
