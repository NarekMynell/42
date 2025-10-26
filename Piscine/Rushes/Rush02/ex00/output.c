#include "output.h"
#include <unistd.h>

void print_num_error()
{
    const char *error_msg = "Error\n";
    write(2, error_msg, 6);
}

void print_dict_error()
{
    const char *error_msg = "Dict Error\n";
    write(2, error_msg, 11);
}

void print_solution(const char *str)
{
    while (*str)
    {
        write(1, str, 1);
        str++;
    }
    write(1, "\n", 1);
}