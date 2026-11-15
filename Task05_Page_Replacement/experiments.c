#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXF 16
#define N 100000

int find_page(const int f[], int n, int p) {
    for (int i = 0; i < n; i++)
        if (f[i] == p) return i;
    return -1;
}

int fifo(const int r[], int n, int frames) {
    int f[MAXF], used = 0, next = 0, faults = 0;
    for (int i = 0; i < n; i++) {
        if (find_page(f, used, r[i]) >= 0) continue;
        faults++;
        if (used < frames) f[used++] = r[i];
        else {
            f[next] = r[i];
            next = (next + 1) % frames;
        }
    }
    return faults;
}

int lru(const int r[], int n, int frames) {
    int f[MAXF], last[MAXF], used = 0, faults = 0;
    for (int i = 0; i < n; i++) {
        int k = find_page(f, used, r[i]);
        if (k >= 0) {
            last[k] = i;
            continue;
        }
        faults++;
        if (used < frames) {
            f[used] = r[i];
            last[used++] = i;
        } else {
            int v = 0;
            for (int j = 1; j < frames; j++)
                if (last[j] < last[v]) v = j;
            f[v] = r[i];
            last[v] = i;
        }
    }
    return faults;
}

int optimal(const int r[], int n, int frames) {
    int f[MAXF], used = 0, faults = 0;
    for (int i = 0; i < n; i++) {
        if (find_page(f, used, r[i]) >= 0) continue;
        faults++;
        if (used < frames) {
            f[used++] = r[i];
            continue;
        }
        int v = 0, farthest = -1;
        for (int j = 0; j < frames; j++) {
            int next = n;
            for (int t = i + 1; t < n; t++)
                if (r[t] == f[j]) {
                    next = t;
                    break;
                }
            if (next > farthest) {
                farthest = next;
                v = j;
            }
        }
        f[v] = r[i];
    }
    return faults;
}

double elapsed_ms(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) * 1000.0 +
           (b.tv_nsec - a.tv_nsec) / 1000000.0;
}

int main(void) {
    int *local = malloc(N * sizeof(int));
    int *random = malloc(N * sizeof(int));
    if (!local || !random) {
        perror("malloc");
        free(local);
        free(random);
        return 1;
    }

    srand(42);

    /* Strong locality: 80% of references use a small working set. */
    for (int i = 0; i < N; i++) {
        if (rand() % 10 < 8)
            local[i] = rand() % 5;
        else
            local[i] = rand() % 50;
        random[i] = rand() % 50;
    }

    FILE *csv = fopen("fault_results.csv", "w");
    if (!csv) {
        perror("fopen");
        free(local);
        free(random);
        return 1;
    }

    fprintf(csv, "workload,frames,FIFO,LRU,Optimal\n");
    for (int fr = 3; fr <= 10; fr++) {
        fprintf(csv, "locality,%d,%d,%d,%d\n", fr,
                fifo(local, N, fr), lru(local, N, fr),
                optimal(local, N, fr));
        fprintf(csv, "random,%d,%d,%d,%d\n", fr,
                fifo(random, N, fr), lru(random, N, fr),
                optimal(random, N, fr));
    }
    fclose(csv);

    struct timespec a, b;
    clock_gettime(CLOCK_MONOTONIC, &a);
    int ff = fifo(random, N, 8);
    clock_gettime(CLOCK_MONOTONIC, &b);
    double fifo_ms = elapsed_ms(a, b);

    clock_gettime(CLOCK_MONOTONIC, &a);
    int lf = lru(random, N, 8);
    clock_gettime(CLOCK_MONOTONIC, &b);
    double lru_ms = elapsed_ms(a, b);

    /* Optimal is more expensive; measure it on a smaller 2,000-reference sample. */
    clock_gettime(CLOCK_MONOTONIC, &a);
    int of = optimal(random, 2000, 8);
    clock_gettime(CLOCK_MONOTONIC, &b);
    double optimal_ms = elapsed_ms(a, b);

    FILE *timing = fopen("timing_results.txt", "w");
    if (timing) {
        fprintf(timing, "FIFO: %.3f ms for 100000 references; faults=%d\n",
                fifo_ms, ff);
        fprintf(timing, "LRU: %.3f ms for 100000 references; faults=%d\n",
                lru_ms, lf);
        fprintf(timing, "Optimal: %.3f ms for 2000 references; faults=%d\n",
                optimal_ms, of);
        fclose(timing);
    }

    printf("Experiment complete. Results saved in fault_results.csv\n");
    printf("Timing saved in timing_results.txt\n");

    free(local);
    free(random);
    return 0;
}
