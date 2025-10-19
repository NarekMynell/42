#ifndef SOLVER_H
#define SOLVER_H

#include <stdlib.h>

static int row_has_value(int *grid, int side_lenght, int row, int value)
{
    for (int c = 0; c < side_lenght; c++)
        if (grid[row * side_lenght + c] == value)
            return 1;
    return 0;
}

static int col_has_value(int *grid, int side_lenght, int col, int value)
{
    for (int r = 0; r < side_lenght; ++r)
        if (grid[r * side_lenght + col] == value)
            return 1;
    return 0;
}

static int visible_count(const int *arr, int len)
{
    int max = 0, vis = 0;
    for (int i = 0; i < len; ++i)
        if (arr[i] > max)
        {
            max = arr[i];
            ++vis;
        }
    return vis;
}

static int visible_count_rev(const int *arr, int len)
{
    int max = 0, vis = 0;
    for (int i = len - 1; i >= 0; --i)
        if (arr[i] > max)
        {
            max = arr[i];
            ++vis;
        }
    return vis;
}

static int check_row_clues(int *grid, int side_lenght, int *clues, int r, int *buf)
{
    for (int c = 0; c < side_lenght; ++c)
        buf[c] = grid[r * side_lenght + c];

    int left = visible_count(buf, side_lenght);
    int right = visible_count_rev(buf, side_lenght);
    return (left == clues[2 * side_lenght + r] && right == clues[3 * side_lenght + r]);
}

static int check_col_clues(int *grid, int side_lenght, int *clues, int c, int *buf)
{
    for (int r = 0; r < side_lenght; ++r)
        buf[r] = grid[r * side_lenght + c];

    int top = visible_count(buf, side_lenght);
    int bottom = visible_count_rev(buf, side_lenght);
    return (top == clues[c] && bottom == clues[side_lenght + c]);
}

static int full_check_all(int *grid, int side_lenght, int *clues, int *buf)
{
    for (int r = 0; r < side_lenght; ++r)
        if (!check_row_clues(grid, side_lenght, clues, r, buf))
            return 0;
    for (int c = 0; c < side_lenght; ++c)
        if (!check_col_clues(grid, side_lenght, clues, c, buf))
            return 0;
    return 1;
}

static int partial_row_valid(int *grid, int side_lenght, int *clues, int r)
{
    int left_hint = clues[2 * side_lenght + r];
    int right_hint = clues[3 * side_lenght + r];

    int max_left = 0, seen_left = 0;
    for (int c = 0; c < side_lenght && grid[r * side_lenght + c] != 0; c++)
        if (grid[r * side_lenght + c] > max_left)
        {
            max_left = grid[r * side_lenght + c];
            seen_left++;
        }

    if (seen_left > left_hint)
        return 0;

    int max_right = 0, seen_right = 0;
    for (int c = side_lenght - 1; c >= 0 && grid[r * side_lenght + c] != 0; c--)
        if (grid[r * side_lenght + c] > max_right)
        {
            max_right = grid[r * side_lenght + c];
            seen_right++;
        }

    if (seen_right > right_hint)
        return 0;

    return 1;
}

static int partial_col_valid(int *grid, int side_lenght, int *clues, int c)
{
    int top_hint = clues[c];
    int bottom_hint = clues[side_lenght + c];

    int max_top = 0, seen_top = 0;
    for (int r = 0; r < side_lenght && grid[r * side_lenght + c] != 0; ++r)
        if (grid[r * side_lenght + c] > max_top)
        {
            max_top = grid[r * side_lenght + c];
            ++seen_top;
        }

    if (seen_top > top_hint)
        return 0;

    int max_bottom = 0, seen_bottom = 0;
    for (int r = side_lenght - 1; r >= 0 && grid[r * side_lenght + c] != 0; --r)
        if (grid[r * side_lenght + c] > max_bottom)
        {
            max_bottom = grid[r * side_lenght + c];
            ++seen_bottom;
        }

    if (seen_bottom > bottom_hint)
        return 0;

    return 1;
}


static int solve_rec(int *grid, int side_lenght, int *clues, int idx, int *buf)
{
    if (idx == side_lenght * side_lenght)
        return full_check_all(grid, side_lenght, clues, buf);

    int row = idx / side_lenght;
    int col = idx % side_lenght;

    if (grid[row * side_lenght + col] != 0)
        return solve_rec(grid, side_lenght, clues, idx + 1, buf);

    for (int v = 1; v <= side_lenght; ++v)
    {
        if (row_has_value(grid, side_lenght, row, v)) continue;
        if (col_has_value(grid, side_lenght, col, v)) continue;

        grid[row * side_lenght + col] = v;

        if (!partial_row_valid(grid, side_lenght, clues, row) ||
            !partial_col_valid(grid, side_lenght, clues, col))
        {
            grid[row * side_lenght + col] = 0;
            continue;
        }

        int ok = 1;
        if (col == side_lenght - 1)
            ok = check_row_clues(grid, side_lenght, clues, row, buf);
        if (ok && row == side_lenght - 1)
            ok = check_col_clues(grid, side_lenght, clues, col, buf);

        if (ok && solve_rec(grid, side_lenght, clues, idx + 1, buf))
            return 1;

        grid[row * side_lenght + col] = 0;
    }

    return 0;
}

static int solve(int *grid, int side_lenght, int *clues)
{
    int *buf = (int *)malloc(sizeof(int) * side_lenght);
    if (!buf) return 0;

    int res = solve_rec(grid, side_lenght, clues, 0, buf);
    free(buf);
    return res;
}

#endif
