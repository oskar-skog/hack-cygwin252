#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    printf("&argc = %p, main = %p, malloc(1000) -> %p\n",
           &argc, main, malloc(1000));
    return 0;
}
