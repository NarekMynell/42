#include "input_check.h"
#include <fcntl.h>
#include <unistd.h>

int is_valid_number(const char* str)
{
    if(!*str || (*(str + 1) && *str == '0'))
        return 0;
    if(!*(str + 1) && *str == '-')
        return 1;
    const char* cur_ptr = str;

    while (*(cur_ptr))
    {
        if(*cur_ptr < '0' || *cur_ptr > '9')
            return 0;

        cur_ptr++;
    }

    return 1;
}

int does_file_exist(const char* path)
{
    int fd;
    fd = open(path, 'r');
    if (fd == -1)
        return 0;
    return 1;
}

int is_valid_input(const int args_count, const char **args, const char **number_str, const char **dict_path)
{
    if(args_count != 2 && args_count != 3)
        return 0;

    if(args_count == 2)
    {
        *number_str = args[1];
        *dict_path = "numbers.dict";
        return is_valid_number(args[1]);
    }
    
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

    return 0;
}
