#ifndef INPUT_CHECK_H
#define INPUT_CHECK_H

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

static const char *skip_spaces(const char *str_pointer)
{
    while (*str_pointer && (*str_pointer == ' ' || *str_pointer == '\t'))
        str_pointer++;
    return str_pointer;
}

static const char *calculate_nummber(const char *str_pointer, int *out_number)
{
    *out_number = 0;
    while (*str_pointer && *str_pointer >= '0' && *str_pointer <= '9')
    {
        *out_number = *out_number * 10 + *str_pointer - '0';
        str_pointer++;
    }
    return str_pointer;
}

static int is_valid_symbol(const char *str_pointer)
{
    if((*str_pointer == ' ' || *str_pointer == '\t') || (*str_pointer >= '0' && *str_pointer <= '9'))
        return 1;
    return 0;
}

int is_valid_count(int count, int allowed_side_min, int allowed_side_max)
{
    int i = allowed_side_min;
    while(i <= allowed_side_max)
    {
        if (count == i * 4)
            return 1;
        i++;
    }
    return 0;
}

static int parse_input(const char *str, int **out_clues, int *out_side_lenght, int allowed_side_min, int allowed_side_max)
{
    if (!str || !out_clues || !out_side_lenght) return 0;

    int str_nums_count = 0;
    int cur_number = 0;
    const char *cur_symbol = str;

    // calcualte numbers count in string
    while(1)
    {
        cur_symbol = skip_spaces(cur_symbol);
        if(!*cur_symbol) break;
        if(*cur_symbol == '0') return 0;

        cur_symbol = calculate_nummber(cur_symbol, &cur_number);
        if(cur_number > 0)
        {
            if(cur_number > allowed_side_max) return 0;
            str_nums_count++;
        }
        cur_number = 0;
        if(!*cur_symbol) break;
        if(!is_valid_symbol(cur_symbol)) return 0;
    }

    if(!is_valid_count(str_nums_count, allowed_side_min, allowed_side_max)) return 0;

    // all is correct
    *out_side_lenght = str_nums_count / 4;
    *out_clues = (int*)malloc(sizeof(int) * str_nums_count);

    cur_symbol = str;
    int index = 0;
    while(index < str_nums_count)
    {
        cur_symbol = skip_spaces(cur_symbol);
        if(!*cur_symbol) break;
        cur_symbol = calculate_nummber(cur_symbol, &cur_number);
        if(cur_number > 0)
        {
            (*out_clues)[index++] = cur_number;
        } 
        if(!*cur_symbol) break;
    }

    return 1;
}

#endif
