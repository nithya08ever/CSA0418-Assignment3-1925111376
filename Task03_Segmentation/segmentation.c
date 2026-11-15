#include <stdio.h>

#define SEGMENTS 4

typedef struct {
    unsigned base;
    unsigned limit;
    const char *name;
} Segment;

Segment table[SEGMENTS] = {
    {1000, 400, "Code"},
    {5000, 600, "Data"},
    {8000, 300, "Heap"},
    {12000, 500, "Stack"}
};

void translate(unsigned segment, unsigned offset)
{
    if (segment >= SEGMENTS) {
        printf("Segment %u: INVALID SEGMENT\n", segment);
        return;
    }

    if (offset >= table[segment].limit) {
        printf("%s: offset %u exceeds limit %u : SEGMENTATION FAULT\n",
               table[segment].name, offset, table[segment].limit);
        return;
    }

    printf("%s: segment %u offset %u -> physical address %u\n",
           table[segment].name, segment, offset,
           table[segment].base + offset);
}

int main(void)
{
    unsigned addresses[8][2] = {
        {0, 0}, {0, 100}, {0, 399}, {0, 400},
        {1, 250}, {1, 599}, {2, 300}, {3, 499}
    };

    printf("SEGMENTATION ADDRESS TRANSLATION\n\n");

    for (int i = 0; i < 8; i++)
        translate(addresses[i][0], addresses[i][1]);

    return 0;
}
