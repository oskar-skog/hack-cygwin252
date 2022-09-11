// Native Windows application
// Compile with i686-w64-mingw32-gcc

#include <ntstatus.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <Winternl.h>

PUNICODE_STRING convert(char *input);
void test(char *path);

int main(int argc, char **argv)
{
    test("\\??\\C:\\");
    test("\\??\\D:\\");
    test("\\??\\E:\\");
    /* // These do not work on Windows 2003:
    test("C:\\");
    test("D:\\");
    test("E:\\");
    */
    return 0;
}

// From winsup/cygwin/nfs*
#define NFS_ACT_ON_LINK "NfsActOnLink"
struct nfs_aol_ffei_t {
    ULONG NextEntryOffset;
    UCHAR Flags;
    UCHAR EaNameLength;
    USHORT EaValueLength;
    CHAR EaName[sizeof (NFS_ACT_ON_LINK)];
};
struct nfs_aol_ffei_t nfs_aol_ffei = {
    0,
    0,
    sizeof (NFS_ACT_ON_LINK) - 1,
    0,
    NFS_ACT_ON_LINK
};

void test(char *path)
{
    // Based on symlink_info::check from winsup/cygwin/path.cc
    printf("Testing %s\n", path);
    
    HANDLE h;
    NTSTATUS status;
    PUNICODE_STRING upath;
    OBJECT_ATTRIBUTES attr;
    IO_STATUS_BLOCK io;
    
    PVOID eabuf = &nfs_aol_ffei;
    ULONG easize = sizeof nfs_aol_ffei;
    
    const ULONG ci_flag = 0x40;     // This is what Cygwin does
    
    upath = convert(path);
    
    InitializeObjectAttributes(&attr, upath, ci_flag, NULL, NULL);
    
    status = NtCreateFile(
                &h,
                READ_CONTROL | FILE_READ_ATTRIBUTES | FILE_READ_EA,
                &attr,
                &io,
                NULL,
                0,
                FILE_SHARE_VALID_FLAGS,
                FILE_OPEN,
                FILE_OPEN_REPARSE_POINT | FILE_OPEN_FOR_BACKUP_INTENT,
                eabuf,
                easize
    );
    printf("NtCreateFile status = 0x%lx\n", status);
    
    if (status == STATUS_EAS_NOT_SUPPORTED || status == STATUS_NOT_SUPPORTED)
    {
        printf("EAs not supported, using NtOpenFile instead\n");
        status = NtOpenFile(
                &h,
                READ_CONTROL | FILE_READ_ATTRIBUTES,
                &attr,
                &io,
                FILE_SHARE_VALID_FLAGS,
                FILE_OPEN_REPARSE_POINT | FILE_OPEN_FOR_BACKUP_INTENT
        );
        printf("NtOpenFile status = 0x%lx\n", status);
    }
    
    FILE_ALL_INFORMATION fai;
    fai.BasicInformation.FileAttributes = 0xdeadbeef;
    fai.StandardInformation.EndOfFile.QuadPart = 123456789;
    status = NtQueryInformationFile(h, &io, &fai, sizeof(fai), FileAllInformation);
    printf(
        "NtQueryInformationFile status = 0x%lx\n"
        "BasicInformation.FileAttributes = 0x%lx\n"
        "StandardInformation.EndOfFile.QuadPart = %jd\n",
        status,
        fai.BasicInformation.FileAttributes,
        (intmax_t) fai.StandardInformation.EndOfFile.QuadPart
    );

    printf("\n");

    return;
}


PUNICODE_STRING convert(char *input)
{
    wchar_t * buf = malloc(sizeof(wchar_t) * (strlen(input) + 1));
    PUNICODE_STRING thing = malloc(sizeof(UNICODE_STRING));
    thing->Length = thing->MaximumLength = strlen(input) * sizeof(wchar_t);
    thing->Buffer = buf;
    while ((*buf++ = *input++));
    return thing;
}
