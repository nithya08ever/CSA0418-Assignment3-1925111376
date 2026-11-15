#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <string.h>

#define SIZE (64 * 1024 * 1024)

int main(void)
{
    long page_size = sysconf(_SC_PAGESIZE);
    char *memory = calloc(1, SIZE);

    if (!memory) {
        perror("calloc");
        return 1;
    }

    struct rusage before, after;

    getrusage(RUSAGE_SELF, &before);

    printf("Allocated memory: %d MB\n", SIZE / (1024 * 1024));
    printf("System page size: %ld bytes\n", page_size);

    for (size_t i = 0; i < SIZE; i += page_size)
        memory[i] = 1;

    getrusage(RUSAGE_SELF, &after);

    printf("Minor page faults: %ld\n",
           after.ru_minflt - before.ru_minflt);
    printf("Major page faults: %ld\n",
           after.ru_majflt - before.ru_majflt);

    free(memory);
    return 0;
}
