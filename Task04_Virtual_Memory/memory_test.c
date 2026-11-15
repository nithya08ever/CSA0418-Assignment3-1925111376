#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <unistd.h>

#define SIZE (64 * 1024 * 1024)

void test_memory(int random_access)
{
    long page = sysconf(_SC_PAGESIZE);
    size_t count = SIZE / page;

    char *memory = calloc(1, SIZE);
    if (!memory) {
        perror("calloc");
        exit(1);
    }

    size_t *order = malloc(count * sizeof(size_t));
    if (!order) {
        perror("malloc");
        free(memory);
        exit(1);
    }

    for (size_t i = 0; i < count; i++)
        order[i] = i;

    if (random_access) {
        srand(42);
        for (size_t i = count - 1; i > 0; i--) {
            size_t j = (size_t)rand() % (i + 1);
            size_t temp = order[i];
            order[i] = order[j];
            order[j] = temp;
        }
    }

    struct rusage before, after;
    getrusage(RUSAGE_SELF, &before);

    volatile unsigned long sum = 0;

    for (size_t i = 0; i < count; i++) {
        size_t index = random_access ? order[i] : i;
        memory[index * page] = 1;
        sum += memory[index * page];
    }

    getrusage(RUSAGE_SELF, &after);

    printf("%s access: minor faults=%ld, major faults=%ld, checksum=%lu\n",
        random_access ? "Random" : "Sequential",
        after.ru_minflt - before.ru_minflt,
        after.ru_majflt - before.ru_majflt,
        sum);

    free(order);
    free(memory);
}

int main(void)
{
    test_memory(0);
    test_memory(1);
    return 0;
}
