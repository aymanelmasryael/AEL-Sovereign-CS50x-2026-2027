#include <cs50.h>
#include <stdio.h>


bool isValid(long number);
void cardType(long number);


int main(void)
{
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);  // get number until valid

    if (isValid(number))  // if card number is valid then print card type
    {
        cardType(number);
    }
    else
    {
        printf("INVALID\n");
    }

}


bool isValid(long number)
{
    int total = 0;
    for (int i = 0; number > 0; ++i, number /= 10)  // prune off the number in every loop
    {
        int remainder = number % 10;  // take last digit
        if (i % 2 == 0)
        {
            total += remainder;  // if i is even just add it straight
        }
        else
        {
            int n = remainder * 2;  // apply formula
            total += n % 10 + n / 10;
        }
    }
    return total % 10 == 0;  // if last digit equals zero then legit
}


void cardType(long number)
{
    if ((number >= 34e13 && number < 35e13) || (number >= 37e13 && number < 38e13))  // condition for amerikanexpress
    {
        printf("AMEX\n");
    }

    else if (number >= 51e14 && number < 56e14)  // condition for mastercard
    {
        printf("MASTERCARD\n");
    }

    else if ((number >= 4e12 && number < 5e12) || (number >= 4e15 && number < 5e15))  // condition for visa
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
