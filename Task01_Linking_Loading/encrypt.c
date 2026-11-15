#include <stdio.h>
#include "encrypt.h"

void encrypt_message(const char *message)
{
    printf("Encrypted message: ");
    while (*message) {
        putchar(*message + 1);
        message++;
    }
    printf("\n");
}
