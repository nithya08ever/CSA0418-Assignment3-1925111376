#include <stdio.h>

int main(void)
{
    FILE *fp = fopen("/proc/self/maps", "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp))
        printf("%s", line);

    fclose(fp);
    return 0;
}
