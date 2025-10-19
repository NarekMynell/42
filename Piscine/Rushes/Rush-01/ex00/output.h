#ifndef OUTPUT_H
#define OUTPUT_H

#include <unistd.h>
#include <stdlib.h>


static void print_error(void)
{
    write(1, "Error\n", 6);
}

static void print_char(char c)
{
    write(1, &c, 1);
}

static void print_digit(int n)
{
    if (n >= 10)
        print_digit(n / 10);
    print_char('0' + (n % 10));
}

static void print_solution(int *grid, int N)
{
    for (int r = 0; r < N; ++r)
    {
        for (int c = 0; c < N; ++c)
        {
            print_digit(grid[r * N + c]);
            if (c != N - 1)
                print_char(' ');
        }
        print_char('\n');
    }
}

#endif
