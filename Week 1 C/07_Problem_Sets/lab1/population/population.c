#include <stdio.h>
#include <cs50.h>
#include <stdbool.h>

int main(void)
{
    int startSize;
    int endSize;
    int year = 0;
    do
    {
        startSize = get_int("Start size: ");
    }
    while (startSize < 9);  // Start size can not be less than 9
    do
    {
        endSize = get_int("End size: ");
    }
    while (startSize > endSize);  // End size can not be less than start size

    while(endSize > startSize)
    {
        year++;  // increase year per loop
        int temp = startSize;
        startSize += startSize / 3;
        startSize -= temp / 4;
    }
    printf("Years: %d\n", year);
}
