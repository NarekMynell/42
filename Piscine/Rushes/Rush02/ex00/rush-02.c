#include "input_check.h"
#include "output.h"
#include "number_builder.h"
#include "stdio.h"
#include <stdlib.h>

int main(const int argc, const char **argv)
{
    const char *number_str = NULL;
    const char *dict_path = NULL;

    if (!is_valid_input(argc, argv, &number_str, &dict_path))
    {
        if(!number_str)
        {
            if(!number_str)
                print_num_error();
            else if(!dict_path)
                print_dict_error();
        }
        return 0;
    }
    char *builded_num = NULL;
    if(try_build_number_string(number_str, dict_path, &builded_num))
    {
        print_solution(builded_num);
    }
    else
    {
        print_dict_error();
    }
    
    if(builded_num != NULL)
        free(builded_num);
}