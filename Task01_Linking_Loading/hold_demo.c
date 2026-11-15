#include <stdio.h>
#include <unistd.h>
#include "encrypt.h"

int main(void)
{
    printf("Process PID: %d\n", getpid());
    encrypt_message("BANK123");
    printf("Process running for 30 seconds...\n");
    fflush(stdout);
    sleep(30);
    return 0;
}
