#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PROCESSES 100

int main(void)
{
    unsigned sizes[PROCESSES];
    unsigned pages[] = {1024, 4096, 2097152};

    srand(42);

    for (int i = 0; i < PROCESSES; i++)
        sizes[i] = 1024 + (unsigned)(rand() % 6000000);

    printf("INTERNAL FRAGMENTATION\n");

    for (int j = 0; j < 3; j++) {
        uint64_t waste = 0;
        uint64_t allocated = 0;

        for (int i = 0; i < PROCESSES; i++) {
            uint64_t p = pages[j];
            uint64_t allocated_size =
                ((uint64_t)sizes[i] + p - 1) / p * p;

            waste += allocated_size - sizes[i];
            allocated += allocated_size;
        }

        printf("Page size %u bytes: waste %llu bytes (%.2f%%)\n",
            pages[j],
            (unsigned long long)waste,
            100.0 * waste / allocated);
    }

    printf("\nTLB EFFECTIVE ACCESS TIME\n");

    double hit[] = {0.80, 0.95, 0.99};
    double memory_ns = 100.0;
    double tlb_ns = 10.0;

    printf("Without TLB: %.2f ns\n", 2 * memory_ns);

    for (int i = 0; i < 3; i++) {
        double eat = hit[i] * (tlb_ns + memory_ns)
                   + (1 - hit[i]) * (tlb_ns + 2 * memory_ns);

        printf("Hit ratio %.0f%%: %.2f ns\n",
               hit[i] * 100, eat);
    }

    printf("\nPAGE TABLE SIZE (8-byte entries)\n");

    printf("32-bit address space: 8 MiB\n");
    printf("48-bit address space: 512 GiB\n");

    printf("\nPAGE BOUNDARY TESTS\n");

    unsigned addresses[] = {0, 1023, 1024, 8191, 8192};

    for (int i = 0; i < 5; i++) {
        unsigned a = addresses[i];
        unsigned p = a / 1024;
        unsigned offset = a % 1024;

        printf("Address %u: page %u, offset %u, %s\n",
            a, p, offset,
            p >= 8 ? "INVALID" : "WITHIN ADDRESS SPACE");
    }

    return 0;
}
