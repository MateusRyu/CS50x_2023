#include <cs50.h>
#include <stdio.h>

long get_number(void);
int check_sum(long number);
void report(int checksum, long number);
int check_AMEX(int start);
int check_MasterCard(int start);
int check_visa(int start);

int main(void)
{
    long number = get_number();
    long checksum = check_sum(number);
    report(checksum, number);
}

long get_number(void)
{
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 1);
    return number;
}

int check_sum(long number)
{
    int digit;
    int sum = 0;

    // Iterate every other digit by 2, starting with the number’s second-to-last digit
    for (long i = number / 10; i > 0; i = i / 100)
    {
        digit = (i % 10) * 2;
        if (digit < 10)
        {
            sum += digit;
        }
        else
        {
            sum += (digit / 10) + (digit % 10);
        }
    }

    // Add that sum to the digits that weren’t multiplied by 2 (starting from the end)
    for (long j = number; j > 0; j = j / 100)
    {
        sum += j % 10;
    }
    return sum;
}

void report(int checksum, long number)
{
    // Card number length and his the 2 first digits
    int length = 0;
    int start;
    // Check if it's a known card type
    int known;

    for (long i = number; i > 0; i = i / 10)
    {
        length++;
        // Get the two first digits
        if (i < 100 && i > 9)
        {
            start = i;
        }
    }

    // printf("Checksum: %i\nNumber: %li\nLength: %i\nStart: %i\n", checksum, number, length, start);

    if (checksum % 10 != 0)
    {
        printf("INVALID\n");
        known = 1;
    }
    else if (length == 15)
    {
        known = check_AMEX(start);
    }
    else if (length == 16)
    {
        known = check_MasterCard(start);
        if (known == 0)
        {
            known = check_visa(start / 10);
        }
    }
    else if (length == 13)
    {
        known = check_visa(start / 10);
    }

    if (checksum % 10 == 0 && known == 0)
    {
        printf("INVALID\n");
    }
}

int check_AMEX(int start)
{
    if (start == 34 || start == 37)
    {
        printf("AMEX\n");
        return 1;
    }
    return 0;
}

int check_MasterCard(int start)
{
    if (start > 50 && start < 56)
    {
        printf("MASTERCARD\n");
        return 1;
    }
    return 0;
}

int check_visa(int start)
{
    if (start == 4)
    {
        printf("VISA\n");
        return 1;
    }
    return 0;
}