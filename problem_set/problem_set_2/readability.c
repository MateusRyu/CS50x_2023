#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // L is the average number of letters per 100 words in the text
    float L = (float) letters / words * 100;

    // S is the average number of sentences per 100 words in the text.
    float S = (float) sentences / words * 100;

    // Coleman-Liau index
    int grade = round(0.0588 * L - 0.296 * S - 15.8);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    int n = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (islower(text[i]) || isupper(text[i]))
        {
            n++;
        }
    }

    return n;
}

int count_words(string text)
{
    int n = 0;
    int isword = 0;

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == ' ')
        {
            isword = 0;
        }
        else
        {
            if (isword == 0)
            {
                n++;
            }
            isword = 1;
        }
    }

    return n;
}

int count_sentences(string text)
{
    int n = 0;

    // Don't consider abbreviations, like "Mr. Robot?"
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            n++;
        }
    }

    return n;
}