#include <stdio.h>

#define MB 1024

int main(void)
{
    const int total_ram = 2048;
    const int control = 128;
    const int machine_learning = 1536;
    const int logging = 128;
    const int os_reserved = 128;

    int used = control + machine_learning + logging + os_reserved;
    int free_memory = total_ram - used;

    printf("EDGE DEVICE MEMORY SIMULATION\n");
    printf("Total RAM: %d MB\n", total_ram);
    printf("Real-time control: %d MB (locked)\n", control);
    printf("Machine learning: %d MB\n", machine_learning);
    printf("Logging: %d MB\n", logging);
    printf("OS reserve: %d MB\n", os_reserved);
    printf("Total allocated: %d MB\n", used);
    printf("Remaining memory: %d MB\n", free_memory);

    printf("\nAdding fourth task of 500 MB...\n");
    int new_used = used + 500;
    printf("New allocation: %d MB\n", new_used);
    printf("Memory deficit: %d MB\n",
           new_used > total_ram ? new_used - total_ram : 0);

    return 0;
}
