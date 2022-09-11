// Native Windows application

#include <ntifs.h>
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
    //
    FILE_ALL_INFORMATION fai;
    NTSTATUS status;
    IO_STATUS_BLOCK io;
    fai.BasicInformation.FileAttributes = 0xdeadbeef;
    status = NtQueryInformationFile(h, &io, &fai, sizeof(fai), FileAllInformation);
    printf(
        "status = 0x%x, attributes = 0x%x\n",
        status,
        fai.BasicInformation.FileAttributes
    );
    //
    return 0;
}
