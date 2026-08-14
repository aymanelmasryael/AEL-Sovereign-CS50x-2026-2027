#include <cs50.h>
#include <stdio.h>


void pyramid(int height);


int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");  // get height
    }
    while (height < 1 || height > 8);  // 0 < height < 9

    pyramid(height);  // call the function to print the pyramid
}


void pyramid(int height)
{
    for (int i = 1; i < height + 1; ++i)
    {
        // fill with spaces first
        for (int j = i; j < height; ++j)
        {
            printf(" ");
        }
        // then add #
        for (int j = height - i; j < height; ++j)
        {
            printf("#");
        }
        // middle space
        printf("  ");
        // second half of pyramid
        for (int j = height - i; j < height; ++j)
        {
            printf("#");
        }
        // for go to next line
        printf("\n");
    }
}
