#include <stdio.h>

int main(void)
{
    double memory = 100.0;
    double fault_service = 8000000.0;

    double rates[] = {
        1.0 / 1000,
        1.0 / 100000,
        1.0 / 1000000
    };

    printf("EFFECTIVE ACCESS TIME\n");

    for (int i = 0; i < 3; i++) {
        double eat = (1 - rates[i]) * memory
                   + rates[i] * (fault_service + memory);

        printf("Fault rate 1 in %.0f: %.2f ns\n",
               1.0 / rates[i], eat);
    }

    double max_rate = 10.0 / (fault_service + 10.0);
    printf("Maximum fault rate for 10%% slowdown: %.10f\n",
           max_rate);

    return 0;
}
