#include <cs50.h>
#include <stdio.h>

long get_number(void);
long checksum(long number);

int main(void)
{
    // prompt user for input
    long number = get_number();

    // validate card
    int sum = checksum(number);
}

long get_number(void)
{
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);
    return number;
}

long checksum(long number)
{
    long last;
    int length = 0;
    int odd = 0;
    int even = 0;
    int even_digit = 0;
    int even_mult = 0;
    int odd_digit = 0;
    int odd_sum = 0;
    int even_sum = 0;
    int even_mult_digit, even_mult_split, even_mult_first;
    while (number > 0)
    {
        // find length of number
        length++;
        // find last number
        last = number % 10;
        // separate alternate numbers
        if (length % 2 == 1)
        {
            // add odd digits
            odd = odd * 10 + last;
            odd_digit = odd % 10;
            odd_sum = odd_digit + odd_sum;
        }
        else
        {
            // multiply odd digits and add products' digits
            even = even * 10 + last;
            even_digit = even % 10;
            even_mult_digit = even_digit * 2;
            even_mult_split = even_mult_digit % 10;
            even_mult_first = (even_mult_digit - even_mult_split) / 10;
            even_sum = even_mult_split + even_mult_first + even_sum;
        }
        number = number / 10;
    }
    // add all digits
    int sum = odd_sum + even_sum;
    // verify last digit of sum
    int sum_digit = sum % 10;
    if (sum_digit != 0)
    {
        printf("INVALID\n");
    }
    else if (length < 13 || length > 16)
    {
        printf("INVALID\n");
    }
    // determine company
    else if ((length == 13 || length == 16) && last == 4)
    {
        printf("VISA\n");
    }
    else if (length == 15 && last == 3 && (even_digit == 4 || even_digit == 7))
    {
        printf("AMEX\n");
    }
    else if (length == 16 && last == 5 && (odd_digit == 1 || odd_digit == 2 || odd_digit == 3 || odd_digit == 4 || odd_digit == 5))
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
    return sum;
}