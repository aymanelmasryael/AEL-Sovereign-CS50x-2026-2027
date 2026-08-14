#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


void calculate_grade(int num_letter, int num_word, int num_sentence);


int main(void)
{
    string txt = get_string("Text: ");

    int num_letter = 0;
    int num_word = 1;  // blank + 1.
    int num_sentence = 0;
    int len = strlen(txt);
    for (int i = 0; i < len; ++i)
    {
        char c = txt[i];
        if (isalpha(c))
        {
            num_letter++;
        }
        if (isblank(c))
        {
            num_word++;
        }
        if (c == '.' || c == '!' || c == '?')  // only these characters finishes a sentence.
        {
            num_sentence++;
        }
    }

    calculate_grade(num_letter, num_word, num_sentence);
}


void calculate_grade(int num_letter, int num_word, int num_sentence)
{
    double L = num_letter / (double) num_word * 100;  // L is the average number of letters per 100 words in the text.
    double S = num_sentence / (double) num_word * 100;  // S is the average number of sentences per 100 words in the text.
    int index = round(0.0588 * L - 0.296 * S - 15.8);  // the formula for grade.
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", index);
    }
}
