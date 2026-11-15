#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define SIZE (32 * 1024 * 1024)

int main(void)
{
    char *memory = malloc(SIZE);
    if (!memory) {
        perror("malloc");
        return 1;
    }

    for (size_t i = 0; i < SIZE; i += 4096)
        memory[i] = 1;

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        free(memory);
        return 1;
    }

    if (pid == 0) {
        struct rusage before, after;
        getrusage(RUSAGE_SELF, &before);

        for (size_t i = 0; i < SIZE; i += 4096)
            memory[i] = 2;

        getrusage(RUSAGE_SELF, &after);

        printf("Child minor faults after writing: %ld\n",
            after.ru_minflt - before.ru_minflt);
        fflush(stdout);
        _exit(0);
    }

    wait(NULL);
    printf("Parent memory remains unchanged: %d\n", memory[0] == 1);

    free(memory);
    return 0;
}
