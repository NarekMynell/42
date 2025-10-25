#include "input_check.h"
#include <stdio.h>

int is_valid_input(int args_count, const char **args, char **number_str, char **dict_path)
{
    if(args_count != 2 || args_count != 3)
        return 0;

    if(args_count == 2)
        return is_valid_number(args[1]);
    
    if(args_count == 3)
    {
        int valid_number;
        int dic_exist;
        valid_number = is_valid_number(args[2]);
        dic_exist = does_file_exist(args[1]);

        if(valid_number)
            *number_str = args[2];
        if(dic_exist)
            *dict_path = args[1];

        return valid_number && dic_exist;
    }
}

int is_valid_number(const char* str)
{
    if(!*str || *str == '0')
        return 0;

    if(!*(str + 1) && *str == '-')
        return 1;

    const char* cur_ptr = str - 1;

    while (*(++cur_ptr))
    {
        if(*cur_ptr < 0 || *cur_ptr > 9)
            return 0;
    }

    return 1;
}

int does_file_exist(const char* path)
{
    FILE *file = fopen(path, "r");

    if (file) {
        fclose(file);
        return 1;
    }

    return 0;
}