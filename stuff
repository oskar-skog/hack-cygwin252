/*
https://stackoverflow.com/questions/10198420/open-directory-using-createfile
https://docs.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants
https://docs.microsoft.com/en-us/windows/win32/fileio/obtaining-a-handle-to-a-directory
*/


#include <stdio.h>
#include <windows.h>

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;
    HANDLE h;
    h = CreateFileA(argv[1], 0, 0, NULL, OPEN_EXISTING,
                    FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (h == INVALID_HANDLE_VALUE)
        return 2;
    FILE_BASIC_INFO info;
    if (!GetFileInformationByHandleEx(h, FileBasicInfo, &info, sizeof(info)))
        return 3;
    printf("0x%x\n", info.FileAttributes);
    return 0;
}
