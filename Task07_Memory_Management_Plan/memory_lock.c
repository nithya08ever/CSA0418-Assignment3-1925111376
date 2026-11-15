#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/resource.h>

#define SIZE (1024 * 1024)
#define ITERATIONS 100000

static volatile unsigned char *buffer;

static double now_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1000000.0;
}

static void run_loop(const char *label)
{
    double worst = 0.0;
    double start = now_ms();

    for (int i = 0; i < ITERATIONS; i++) {
        double before = now_ms();
        buffer[(i * 4096) % SIZE]++;
        double delay = now_ms() - before;
        if (delay > worst) worst = delay;
    }

    printf("%s: total %.3f ms, worst measured access %.6f ms\n",
           label, now_ms() - start, worst);
}

int main(void)
{
    struct rlimit limit;

    buffer = mmap(NULL, SIZE, PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (buffer == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    /* Touch all pages before timing to separate first-touch faults. */
    for (int i = 0; i < SIZE; i += 4096)
        buffer[i] = 0;

    run_loop("Without mlock");

    if (mlock((void *)buffer, SIZE) != 0) {
        perror("mlock");
        printf("Memory locking failed; check the process memlock limit.\n");
        munmap((void *)buffer, SIZE);
        return 1;
    }

    run_loop("With mlock");

    if (getrlimit(RLIMIT_MEMLOCK, &limit) == 0)
        printf("Memlock soft limit: %llu bytes\n",
               (unsigned long long)limit.rlim_cur);

    munlock((void *)buffer, SIZE);
    munmap((void *)buffer, SIZE);
    return 0;
}
