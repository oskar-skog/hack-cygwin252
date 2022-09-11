#include <stdlib.h>
#include <Winternl.h>

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
