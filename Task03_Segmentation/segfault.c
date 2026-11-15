#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *ptr = NULL;

    printf("Attempting invalid memory access...\n");
    fflush(stdout);

    *ptr = 100;

    return 0;
}
