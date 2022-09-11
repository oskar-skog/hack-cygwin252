// Native Windows application

//#include <ntifs.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <Winternl.h>



// NT Functions
// https://stackoverflow.com/questions/2964941/not-able-to-include-ntifs-h-in-win32-project

typedef NTSTATUS (__stdcall *_NtCreateFile)(
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer OPTIONAL,
    IN ULONG EaLength
);
_NtCreateFile NtCreateFile;

__kernel_entry NTSTATUS NtOpenFile(
    [out] PHANDLE            FileHandle,
    [in]  ACCESS_MASK        DesiredAccess,
    [in]  POBJECT_ATTRIBUTES ObjectAttributes,
    [out] PIO_STATUS_BLOCK   IoStatusBlock,
    [in]  ULONG              ShareAccess,
    [in]  ULONG              OpenOptions
);

__kernel_entry NTSYSCALLAPI NTSTATUS NtQueryInformationFile(
    [in]  HANDLE                 FileHandle,
    [out] PIO_STATUS_BLOCK       IoStatusBlock,
    [out] PVOID                  FileInformation,
    [in]  ULONG                  Length,
    [in]  FILE_INFORMATION_CLASS FileInformationClass
);

int load_ntfuncs()
{
    HMODULE lib = GetModuleHandle("ntdll.dll");
    if (!lib)
        return 0;
    NtCreateFile = (_NtCreateFile) GetProcAddress(lib, "NtCreateFile");
    NtOpenFile = GetProcAddress(lib, "NtOpenFile");
    NtQueryInformationFile = GetProcAddress(lib, "NtQueryInformationFile");
    return (!!NtCreateFile) && (!!NtOpenFile) && (!!NtQueryInformationFile);
}


PUNICODE_STRING convert(char *input);
void test(char *path);
int load_ntfuncs(void);

int main(int argc, char **argv)
{
    if (!load_ntfuncs()) {
        printf("Failed to load functions.\n");
        return 1;
    }
    test("\\??\\C:\\");
    test("\\??\\D:\\");
    test("\\??\\E:\\");
    test("C:\\");
    test("D:\\");
    test("E:\\");
    return 0;
}

void test(char *path)
{
    printf("Testing %s\n", path);
    
    HANDLE h;
    NTSTATUS status;
    PUNICODE_STRING upath;
    OBJECT_ATTRIBUTES attr;
    IO_STATUS_BLOCK io;
    
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
    NTSTATUS status;
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
