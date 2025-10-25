#include "input_check.h"
#include "output.h";

int main(int argc, char **argv)
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
    
}