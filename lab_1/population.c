#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int start;
    int end;
    int n;
    int years = 0;

    do
    {
        start = get_int("Start size: ");
    }
    while (start < 9);

    do
    {
        end = get_int("End size: ");
    }
    while (end < start);

    n = start;
    while (n < end)
    {
        n += n / 3 - n / 4;
        years++;
    }

    printf("Years: %i\n", years);
}