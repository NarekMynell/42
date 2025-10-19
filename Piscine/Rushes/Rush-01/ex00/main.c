#include <unistd.h>
#include <stdlib.h>

#include "input_check.h"
#include "solver.h"
#include "output.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        print_error();
        return 0;
    }
    const int allowed_size_min = 4;
    const int allowed_size_max = 9;
    
    int side_lenght = 0;
    int *clues = NULL;
    int *grid = NULL;

    if (parse_input(argv[1], &clues, &side_lenght, allowed_size_min, allowed_size_max) == 0)
    {
        print_error();
        return 0;
    }

    int elements_count = side_lenght * side_lenght;
    grid = (int *)malloc(sizeof(int) * elements_count);
    if (!grid)
    {
        free(clues);
        print_error();
        return 0;
    }

    // initialize 
    for (int i = 0; i < elements_count; ++i) grid[i] = 0;
    if (solve(grid, side_lenght, clues))
    {
        print_solution(grid, side_lenght);
    }
    else
    {
        print_error();
    }
    free(grid);
    free(clues);
    return 0;
}
