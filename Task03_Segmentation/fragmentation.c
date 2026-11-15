#include <stdio.h>

int main(void)
{
    int free_blocks[] = {100, 200, 150, 250, 300};
    int request = 400;
    int total = 0;
    int largest = 0;

    for (int i = 0; i < 5; i++) {
        total += free_blocks[i];
        if (free_blocks[i] > largest)
            largest = free_blocks[i];
    }

    printf("Total free memory: %d KB\n", total);
    printf("Largest free block: %d KB\n", largest);
    printf("Requested segment: %d KB\n", request);

    if (largest >= request)
        printf("Allocation possible in one block\n");
    else
        printf("Allocation fails due to external fragmentation\n");

    return 0;
}
