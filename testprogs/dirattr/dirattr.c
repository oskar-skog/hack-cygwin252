// Native Windows application

/*
https://stackoverflow.com/questions/10198420/open-directory-using-createfile
https://docs.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants
https://docs.microsoft.com/en-us/windows/win32/fileio/obtaining-a-handle-to-a-directory
*/

#include <stdio.h>
#include <windows.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: dirattr path\nEx: dirattr C:\\\n");
        return 1;
    }
    HANDLE h;
    h = CreateFileA(argv[1], 0, 0, NULL, OPEN_EXISTING,
                    FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: could not open specified path\n");
        return 2;
    }
    BY_HANDLE_FILE_INFORMATION info;
    if (!GetFileInformationByHandle(h, &info)) {
        fprintf(stderr, "Error: could not get file information\n");
        return 3;
    }
    printf("Attributes: 0x%x\n", info.dwFileAttributes);
    return 0;
}
