#include <cs50.h>
#include <stdio.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    // Ask how many cents the customer is owed
    int cents = get_cents();

    // Calculate the number of quarters to give the customer
    int quarters = calculate_quarters(cents);
    cents = cents - quarters * 25;

    // Calculate the number of dimes to give the customer
    int dimes = calculate_dimes(cents);
    cents = cents - dimes * 10;

    // Calculate the number of nickels to give the customer
    int nickels = calculate_nickels(cents);
    cents = cents - nickels * 5;

    // Calculate the number of pennies to give the customer
    int pennies = calculate_pennies(cents);
    cents = cents - pennies * 1;

    // Sum coins
    int coins = quarters + dimes + nickels + pennies;

    // Print total number of coins to give the customer
    printf("%i\n", coins);
}

int get_cents(void)
{
    int n;
    // Prompts for a positive number of cents
    do
    {
        n = get_int("Cents: ");
    }
    while (n < 0);
    return n;
}

int calculate_quarters(int cents)
{
    // value of quarters
    int value = 25;
    // Count how many quarter are needed
    int n = 0;
    while (cents >= value)
    {
        cents -= value;
        n++;
    }
    return n;
}

int calculate_dimes(int cents)
{
    // value of dimes
    int value = 10;
    // Count how many dimes are needed
    int n = 0;
    while (cents >= value)
    {
        cents -= value;
        n++;
    }
    return n;
}

int calculate_nickels(int cents)
{
    // value of nickels
    int value = 5;
    // Count how many nickels are needed
    int n = 0;
    while (cents >= value)
    {
        cents -= value;
        n++;
    }
    return n;
}

int calculate_pennies(int cents)
{
    // value of pennies
    int value = 1;
    // Count how many pennies is need
    int n = 0;
    while (cents >= value)
    {
        cents -= value;
        n++;
    }
    return n;
}