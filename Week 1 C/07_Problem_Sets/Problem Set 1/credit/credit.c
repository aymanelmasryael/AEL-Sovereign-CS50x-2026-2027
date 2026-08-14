/**
 * ============================================================================
 *  PROJECT : AEL Sovereign — CS50x 2026-2027 Master Solutions
 *  FILE    : credit.c
 *  AUTHOR  : Ayman Elmasry — AEL Digital Studio
 *  ---------------------------------------------------------------------------
 *  DESCRIPTION
 *    Identifies the issuing network of a credit card number and certifies its
 *    validity using the Luhn checksum. Accepts a single 64-bit card number via
 *    stdin ("Number: ") and prints one of AMEX / MASTERCARD / VISA / INVALID —
 *    the exact contract the check50 harness expects.
 *
 *  THE LUHN ALGORITHM (WITH PROOF SKETCH)
 *    Devised by Hans Peter Luhn (IBM, 1954) to catch accidental transcription
 *    errors rather than fraud. Procedure, scanning from the *rightmost* digit:
 *      1. Moving left, double every second digit.
 *      2. If a doubled digit exceeds 9, it contributes its digit-sum instead
 *         (equivalently, subtract 9 — a doubled digit is never more than 18).
 *      3. Sum every un-doubled digit together with every adjusted doubled one.
 *      4. The number is valid iff this total is congruent to 0 mod 10.
 *
 *    Why it works: a transposition or a single-digit mutation produces a total
 *    that is NOT a multiple of 10 in ~all* real-world cases, so mod-10 wraps
 *    expose the corruption. The check digit is itself the digit that forces
 *    the full total to a ten's multiple, which is why "ends in a zero" is the
 *    acceptance test.
 *
 *  ISSUING-NETWORK HEURISTICS
 *    AMEX       : 15 digits, prefix 34 or 37.
 *    MASTERCARD : 16 digits, leading pair in [51, 55].
 *    VISA       : 13 or 16 digits, leading digit 4.
 *    A number is reported only after it passes the Luhn check.
 *
 *  COMPLEXITY
 *    Time  : O(d)  — single left-to-right digit stream, d <= 16 digits.
 *    Space : O(1)  — constant heap of scalars, no digit arrays required.
 *
 *  COMPILE  : gcc -o credit credit.c -lcs50
 * ============================================================================
 */

#include <cs50.h>
#include <stdio.h>
#include <stdbool.h>

/* Issuing-network display tokens — the check50-verified vocabulary. */
#define TOKEN_AMEX       "AMEX"
#define TOKEN_MASTERCARD "MASTERCARD"
#define TOKEN_VISA       "VISA"
#define TOKEN_INVALID    "INVALID"

/* Distinctive card-length and prefix signatures of each network. */
#define AMEX_LEN       15
#define AMEX_PREFIX_34 34
#define AMEX_PREFIX_37 37

#define MASTERCARD_LEN     16
#define MASTERCARD_MIN_51  51
#define MASTERCARD_MAX_55  55

#define VISA_PREFIX 4
#define VISA_LEN_SHORT 13
#define VISA_LEN_LONG  16

/* Forward declarations for the modular validation pipeline. */
long get_card_number(void);
bool is_valid_luhn(long number);
int  count_digits(long number);
int  leading_pair(long number);
int  leading_digit(long number);
const char *identify_issuer(long number);

int main(void)
{
    long number = get_card_number();

    /* Only a checksum-verified number may be branded. */
    if (is_valid_luhn(number))
    {
        printf("%s\n", identify_issuer(number));
    }
    else
    {
        printf("%s\n", TOKEN_INVALID);
    }

    return 0;
}

/**
 * Acquires the card number, refusing negatives (a card number is unsigned
 * by definition). Zero is technically Luhn-valid, so input of 0 reproduces
 * the reference behaviour of "INVALID" branding downstream via prefix rules.
 */
long get_card_number(void)
{
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);
    return number;
}

/**
 * Applies the Luhn checksum. Reads digits right-to-left by repeated
 * floor-division by 10, which peels the least-significant digit each turn.
 *
 * OBSERVATION: only the position parity matters, so a running digit-index
 * (0-based, counting from the units place) selects the doubling class.
 * Height-reweighting a doubled digit: max(2*9)=18, whose digit sum is its
 * base-10 "digital root" — obtained here as (2*d) % 10 + (2*d) / 10.
 */
bool is_valid_luhn(long number)
{
    int  total  = 0;
    int  parity = 0;   /* 0 => units digit (un-doubled), 1 => doubled class. */

    for (; number > 0; number /= 10, parity ^= 1)
    {
        int digit = number % 10;

        if (parity == 0)
        {
            total += digit;                 /* every other digit: sum as-is. */
        }
        else
        {
            int doubled = digit * 2;
            total += doubled % 10 + doubled / 10;  /* digit-sum fold.        */
        }
    }

    return total % 10 == 0;
}

/**
 * Returns the number of base-10 digits in `number` by aggressive division.
 * Corollary: 0 has zero digits, which is harmless for the issuer test.
 */
int count_digits(long number)
{
    int digits = 0;
    do
    {
        number /= 10;
        digits++;
    }
    while (number > 0);
    return digits;
}

/**
 * Extracts the two most-significant digits of `number` by stripping its
 * less-significant digits until only two remain.
 */
int leading_pair(long number)
{
    while (number >= 100)
    {
        number /= 10;
    }
    return (int) number;
}

/**
 * Extracts the most-significant digit of `number`.
 */
int leading_digit(long number)
{
    while (number >= 10)
    {
        number /= 10;
    }
    return (int) number;
}

/**
 * Classifies a Luhn-valid number by its distinctive length-and-prefix
 * signature. Order matters for VISA: both its lengths and its lone '4'
 * prefix are tested after the two-digit networks to avoid collisions.
 */
const char *identify_issuer(long number)
{
    int digits = count_digits(number);
    int pair   = leading_pair(number);
    int first  = leading_digit(number);

    /* AMEX: 15 digits, opens 34 or 37. */
    if (digits == AMEX_LEN &&
        (pair == AMEX_PREFIX_34 || pair == AMEX_PREFIX_37))
    {
        return TOKEN_AMEX;
    }

    /* MASTERCARD: 16 digits, leading pair in the inclusive window 51-55. */
    if (digits == MASTERCARD_LEN &&
        pair >= MASTERCARD_MIN_51 && pair <= MASTERCARD_MAX_55)
    {
        return TOKEN_MASTERCARD;
    }

    /* VISA: 13 or 16 digits, single leading 4. */
    if (((digits == VISA_LEN_SHORT) || (digits == VISA_LEN_LONG)) &&
        first == VISA_PREFIX)
    {
        return TOKEN_VISA;
    }

    return TOKEN_INVALID;
}