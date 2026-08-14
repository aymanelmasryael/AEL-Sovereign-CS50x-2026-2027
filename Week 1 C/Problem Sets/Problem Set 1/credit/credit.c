/**
 * @file credit.c
 * @brief CS50x Problem Set 1 — Credit: LUHN-based card issuer validator.
 *
 * @author Ayman Elmasry — AEL Digital Studio
 * @project AEL Sovereign — CS50x 2026-2027
 *
 * @details
 *   Validates a credit-card number entered interactively. The program
 *   applies the LUHN checksum (the industry-standard check-digit scheme
 *   used by virtually all card networks) and then classifies the number
 *   by its length and leading digits into AMEX, MASTERCARD, VISA, or the
 *   INVALID bucket.
 *
 * Algorithm:
 *   1. Read the card number as a long (up to 16 digits).
 *   2. LUHN checksum: walking the digits from the right, every other
 *      digit (the second-to-last, fourth-to-last, ...) is doubled, and the
 *      digits of each doubled value are summed; the undoubled digits are
 *      added unchanged. The number passes iff the total is divisible by 10.
 *   3. Issuer classification from length + prefix:
 *        - AMEX       : 15 digits, prefix 34 or 37
 *        - MASTERCARD : 16 digits, prefix 51..55
 *        - VISA       : 13 or 16 digits, leading digit 4
 *   4. Emit exactly one of: "VISA", "AMEX", "MASTERCARD", "INVALID".
 *
 * Complexity:
 *   Time  — O(d): d is the digit count (bounded by 16); a constant number
 *           of passes over the digits.
 *   Space — O(1): only scalar state, no dynamic allocation.
 */

#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>

/* Card-network constants used for classification. */
#define VISA_MIN_DIGITS 13
#define VISA_MAX_DIGITS 16
#define AMEX_DIGITS 15
#define MASTERCARD_DIGITS 16
#define AMEX_PREFIX_ONE 34
#define AMEX_PREFIX_TWO 37
#define MASTERCARD_PREFIX_MIN 51
#define MASTERCARD_PREFIX_MAX 55
#define VISA_LEADING_DIGIT 4
#define VISA_PREFIX_SIZE 1
#define AMEX_PREFIX_SIZE 2
#define MASTERCARD_PREFIX_SIZE 2
#define MODULUS_TEN 10

/**
 * @brief Count the decimal digits in a non-negative number.
 *
 * @param number The number to inspect.
 * @return The number of digits, or 0 for the value zero.
 */
static int count_digits(long number)
{
    int digits = 0;

    while (number > 0)
    {
        number /= 10;
        digits++;
    }

    return digits;
}

/**
 * @brief Compute the LUHN checksum of a card number.
 *
 * @param number The card number (its most significant digit first).
 * @return The weighted sum described by the LUHN algorithm.
 */
static int luhn_checksum(long number)
{
    int sum = 0;
    bool double_next = false;

    /* Process digits from right to left. The rightmost (check) digit is
       never doubled; every other digit moving leftwards is doubled and
       its constituent digits are summed rather than the product itself. */
    while (number > 0)
    {
        int digit = (int) (number % 10);

        if (double_next)
        {
            digit *= 2;
            if (digit >= 10)
            {
                /* A doubled digit is at most 18, so its digit-sum is
                   (tens digit) + (units digit). */
                sum += (digit % 10) + (digit / 10);
            }
            else
            {
                sum += digit;
            }
        }
        else
        {
            sum += digit;
        }

        double_next = !double_next;
        number /= 10;
    }

    return sum;
}

/**
 * @brief Isolate the leading `target` digits of a number.
 *
 * @param number        The number whose prefix is required.
 * @param target        How many leading digits to keep.
 * @return The leading `target` digits as a number.
 */
static long leading_prefix(long number, int target)
{
    int digits = count_digits(number);
    long divisor = 1;

    for (int i = 0; i < digits - target; i++)
    {
        divisor *= 10;
    }

    return number / divisor;
}

/**
 * @brief Classify a number into a card issuer, or NULL if unrecognized.
 *
 * @param number The (checksum-passing) card number.
 * @return "VISA", "AMEX" or "MASTERCARD", or NULL when no pattern matches.
 */
static const char *issuer_name(long number)
{
    int digits = count_digits(number);

    if ((digits == VISA_MIN_DIGITS || digits == VISA_MAX_DIGITS) &&
        leading_prefix(number, VISA_PREFIX_SIZE) == VISA_LEADING_DIGIT)
    {
        return "VISA";
    }

    if (digits == AMEX_DIGITS)
    {
        long prefix = leading_prefix(number, AMEX_PREFIX_SIZE);
        if (prefix == AMEX_PREFIX_ONE || prefix == AMEX_PREFIX_TWO)
        {
            return "AMEX";
        }
    }

    if (digits == MASTERCARD_DIGITS)
    {
        long prefix = leading_prefix(number, MASTERCARD_PREFIX_SIZE);
        if (prefix >= MASTERCARD_PREFIX_MIN && prefix <= MASTERCARD_PREFIX_MAX)
        {
            return "MASTERCARD";
        }
    }

    return NULL;
}

int main(void)
{
    long number = get_long("Number: ");

    /* A negative number is meaningless in this context; reject it early. */
    if (number < 0)
    {
        printf("INVALID\n");
        return 0;
    }

    /* Fail fast if the number fails the LUHN checksum. */
    if (luhn_checksum(number) % MODULUS_TEN != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    /* A passing checksum is only half the story: the length and prefix
       must also match a recognized card network. */
    const char *issuer = issuer_name(number);
    if (issuer == NULL)
    {
        printf("INVALID\n");
    }
    else
    {
        printf("%s\n", issuer);
    }

    return 0;
}
