// Native Windows application
// Compile with i686-w64-mingw32-gcc

#include <ntstatus.h>
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
    test("C:\\");
    test("D:\\");
    test("E:\\");
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
    printf("NtCreateFile status = 0x%x\n", status);
    
    if (status == STATUS_EAS_NOT_SUPPORTED || status == STATUS_NOT_SUPPORTED)
    {
        printf("Using NtOpenFile instead\n");
        status = NtOpenFile(
                &h,
                READ_CONTROL | FILE_READ_ATTRIBUTES,
                &attr,
                &io,
                FILE_SHARE_VALID_FLAGS,
                FILE_OPEN_REPARSE_POINT | FILE_OPEN_FOR_BACKUP_INTENT
        );
        printf("NtOpenFile status = 0x%x", status);
    }
    
    // Catch all
    if (h == INVALID_HANDLE_VALUE)
    {
        printf("Error: handle == INVALID_HANDLE_VALUE\n\n");
        return;
    }
    
    
    FILE_ALL_INFORMATION fai;
    IO_STATUS_BLOCK io;
    fai.BasicInformation.FileAttributes = 0xdeadbeef;
    status = NtQueryInformationFile(h, &io, &fai, sizeof(fai), FileAllInformation);
    printf(
        "NtQueryInformationFile statys = 0x%x\n"
        "BasicInformation.FileAttributes = 0x%x\n",
        status,
        fai.BasicInformation.FileAttributes
    );

    return;
}


PUNICODE_STRING convert(char *input)
{
    wchar_t * buf = malloc(sizeof(wchar_t) * (strlen(input) + 1));
    wchar_t * dst = buf;
    while (*dst++ = *input++);
    PUNICODE_STRING thing = malloc(sizeof(UNICODE_STRING));
    thing->Length = thing->MaximumLength = strlen(input);
    thing->Buffer = buf;
    return thing;
}
