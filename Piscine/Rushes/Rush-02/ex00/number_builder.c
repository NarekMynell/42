#include <stdio.h>
#include "number_builder.h"


int try_build_number_string(const char *number_str, const char *dict_path, const char **output_num)
{
    FILE *file = fopen(dict_path, "r");
    if (file == NULL)
        return 0;


    fclose(file);
    return 0;
}

int calcuate_lines_count(FILE *file)
{

}