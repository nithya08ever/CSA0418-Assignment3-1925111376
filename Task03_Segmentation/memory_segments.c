#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Process ID: %d\n", getpid());
    printf("Inspect /proc/PID/maps while this process runs.\n");
    fflush(stdout);
    sleep(30);
    return 0;
}
