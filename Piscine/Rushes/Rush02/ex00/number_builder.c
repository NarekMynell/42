#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"


char *g_single_digits[SINGLE_DIGIT_COUNT] = {NULL};
char *g_two_digits[TWO_DIGIT_COUNT] = {NULL};
char *g_multi_digits[MULTI_DIGIT_COUNT] = {NULL};

static void free_memory()
{
    for (int i = 0; i < SINGLE_DIGIT_COUNT; i++)
    {
        if (g_single_digits[i] != NULL)
            free(g_single_digits[i]);
    }
    for (int i = 0; i < TWO_DIGIT_COUNT; i++)
    {
        if (g_two_digits[i] != NULL)
            free(g_two_digits[i]);
    }
    for (int i = 0; i < MULTI_DIGIT_COUNT; i++)
    {
        if (g_multi_digits[i] != NULL)
            free(g_multi_digits[i]);
    }
}

static int is_digit(char c) { return (c >= '0' && c <= '9'); }
static int is_space(char c) { return (c == ' ' || c == '\t'); }

static int check_key_type(const char *num, int len)
{
    if (len == 1) return 1;
    if (len == 2)
    {
        if (num[0] == '1') return 2;
        if (num[1] == '0' && num[0] >= '2' && num[0] <= '9') return 2;
        return 0;
    }
    if (num[0] == '1' && (len = 3 || (len - 1) % 3 == 0))
    {
        int i;
        i = 0;
        while (++i < len)
        {
            if(num[i] != '0')
                return 0;
        }
        return 3;
    }
    return 0;
}

static void store(char *num, int len, char *num_name)
{
    if (len == 1)
        g_single_digits[num[0]-'0'] = num_name;
    else if (len == 2)
    {
        if (num[0] == '1')
            g_two_digits[num[1]-'0'] = num_name;
        else
            g_two_digits[8 + (num[0]-'0')] = num_name;
    }
    else
    {
        if(len == 3)
        {
            g_multi_digits[0] = num_name;
        }
        else
        {
            g_multi_digits[len / 3] = num_name;
        }
    }
}

static char *create_string_from_ptr_and_len(const char *source_ptr, int length)
{
    char *new_string = (char *)malloc(length + 1);
    if (new_string == NULL)
        return NULL;
    int i;
    i = 0;
    while (i < length)
    {
        new_string[i] = source_ptr[i];
        i++;
    }
    new_string[length] = '\0';
    return new_string;
}

static void set_array(const char* num_ptr, const char* num_name_ptr, int num_lenght, int num_name_lenght)
{
    int num_type;
    num_type = check_key_type(num_ptr, num_lenght);
    if(num_type == 0)
        return;
    char* num_str = create_string_from_ptr_and_len(num_ptr, num_lenght);
    char* num_name_str = create_string_from_ptr_and_len(num_name_ptr, num_name_lenght);
    if(num_str == NULL || num_name_str == NULL)
    {
        if(num_str != NULL)
        free(num_str);
        if(num_name_str != NULL)
        free(num_name_str);
        return;
    }
    store(num_str, num_lenght, num_name_str);
    free(num_str);
}

static void reset_file_read_values(char** num_ptr, char** num_name_ptr, int* num_lenght, int* num_name_lenght, int* found_colon, int* num_index, int* num_name_index)
{
    *num_ptr = NULL;
    *num_name_ptr = NULL;
    *num_lenght = 0;
    *num_name_lenght = 0;
    *found_colon = 0;
    *num_index = 0;
    *num_name_index = 0;
}

static int str_len(const char *str)
{
    int len = 0;
    while (str[len])
        len++;
    return len;
}

static void str_append(char **dest_ptr, const char *src)
{
    int i = 0;
    while (src[i])
    {
        **dest_ptr = src[i];
        (*dest_ptr)++;
        i++;
    }
}

static char* get_single_digit_string(const char* num_str_ptr)
{
    int index;
    index = num_str_ptr[0] - '0';
    return g_single_digits[index];
}

static char* get_two_digit_string(const char* num_str_ptr)
{
    int index;
    if(num_str_ptr[0] == '1')
        index = num_str_ptr[1] - '0';
    else
        index = num_str_ptr[0] - '0' + 8;
    return g_two_digits[index];
}

static char* get_multi_digit_string(int zeros_count)
{
    int index;
    if(zeros_count == 2)
        index = 0;
    else
        index = zeros_count / 3;
    return g_multi_digits[index];
}

static int build_number_name(const char* num_str, char **output_num)
{
    char *result;
    result = (char *)malloc(RESULT_LENGTH);
    if (!result)
        return 0;
    int length;
    length = str_len(num_str);
    int triplets_counth;
    triplets_counth = (length - 1) / 3;
    int tokos;
    const char* digit_ptr;
    char* result_ptr;
    digit_ptr = num_str;
    result_ptr = result;
    char* number_name;
    int has_value;
    has_value = 0;

    if(length == 1 && *digit_ptr == '0')
    {
        number_name = get_single_digit_string(digit_ptr);
        str_append(&result_ptr, number_name);
        *output_num = result;
        return 1;
    }
    while (triplets_counth >= 0)
    {
        tokos = length % 3;
        if(tokos == 0)
        {
            if(digit_ptr[0] != '0')
            {
                number_name = get_single_digit_string(&digit_ptr[0]);
                str_append(&result_ptr, number_name);
                str_append(&result_ptr, " ");
                number_name = get_multi_digit_string(2);
                str_append(&result_ptr, number_name);
                str_append(&result_ptr, " ");
                has_value = 1;
            }
            if(digit_ptr[1] != '0')
            {
                if(digit_ptr[0] != '0')
                {
                    str_append(&result_ptr, "and ");
                }
                number_name = get_two_digit_string(&digit_ptr[1]);
                str_append(&result_ptr, number_name);
                has_value = 1;
            }
            if(digit_ptr[2] != '0')
            {
                if(digit_ptr[1] != '1')
                {
                    if(digit_ptr[1] != '0')
                    {
                        str_append(&result_ptr, "-");
                    }
                    number_name = get_single_digit_string(&digit_ptr[2]);
                    str_append(&result_ptr, number_name);
                    str_append(&result_ptr, " ");
                }
                has_value = 1;
            }
            else if(digit_ptr[1] != '0')
            {
                str_append(&result_ptr, " ");
            }
        }
        else if(tokos == 2)
        {
            if(digit_ptr[0] != '0')
            {
                number_name = get_two_digit_string(&digit_ptr[0]);
                str_append(&result_ptr, number_name);
                has_value = 1;
            }
            if(digit_ptr[1] != '0')
            {
                if(digit_ptr[0] != '1')
                {
                    if(digit_ptr[0] != '0')
                    {
                        str_append(&result_ptr, "-");
                    }
                    number_name = get_single_digit_string(&digit_ptr[1]);
                    str_append(&result_ptr, number_name);
                    str_append(&result_ptr, " ");
                }
                has_value = 1;
            }
            else if(digit_ptr[0] != '0')
            {
                str_append(&result_ptr, " ");
            }
        }
        else if(tokos == 1)
        {
            if(digit_ptr[0] != '0')
            {
                number_name = get_single_digit_string(&digit_ptr[0]);
                str_append(&result_ptr, number_name);
                has_value = 1;
            }
        }

        if(triplets_counth > 0 && has_value)
        {

            number_name = get_multi_digit_string(triplets_counth * 3);
            str_append(&result_ptr, number_name);
            str_append(&result_ptr, ", ");
        }
        if(tokos == 0)
        {
            digit_ptr += 3;
            length -= 3;
        }
        else
        {
            digit_ptr += tokos;
            length -= tokos;
        }
            
        triplets_counth--;
        has_value = 0;
    }
    
    str_append(&result_ptr, "\0");

    char* c = result_ptr;
    while (*c)
    {
        if(c == NULL)
            return 0;
    }
    
    *output_num = result;
    return 1;
}

int try_build_number_string(const char *number_str, const char *dict_path, char **output_num)
{
    int fd = open(dict_path, O_RDONLY);
    if (fd < 0) return 0;
    int is_file_valid;
    is_file_valid = 1;
    char line_buf[FILE_LINE_LENGTH];
    int line_index = -1;
    char c;
    char* num_ptr;
    char* num_name_ptr;
    int num_index;
    int num_name_index;
    int num_lenght;
    int num_name_lenght;
    int found_colon;
    reset_file_read_values(&num_ptr, &num_name_ptr, &num_lenght, &num_name_lenght, &found_colon, &num_index, &num_name_index);

    while (read(fd, &c, 1) > 0)
    {
        line_buf[++line_index] = c;
        if(is_space(c))
        {
            if(num_ptr != NULL && num_lenght == 0)
                num_lenght = num_index;
            if(num_name_ptr != NULL && num_name_lenght == 0)
                num_name_lenght = num_name_index;
            continue;
        }
        if(c == '\n')
        {
            line_index = -1;
            if(num_name_ptr != NULL && num_name_lenght == 0)
                num_name_lenght = num_name_index;
            if(num_lenght > 0 && found_colon && num_name_lenght > 0)
                set_array(num_ptr, num_name_ptr, num_lenght, num_name_lenght);
            reset_file_read_values(&num_ptr, &num_name_ptr, &num_lenght, &num_name_lenght, &found_colon, &num_index, &num_name_index);
            continue;
        }
        if(c == ':')
        {
            if(num_ptr != NULL)
            {
                if(num_lenght == 0)
                    num_lenght = num_index;
                found_colon = 1;
            }
            else
            {
                is_file_valid = 0;
                break;
            }
        }
        else if(is_digit(c))
        {
            if(num_ptr == NULL)
                num_ptr = &line_buf[line_index];
            num_index++;
        }
        else if(found_colon)
        {
            if(num_name_ptr == NULL)
                num_name_ptr = &line_buf[line_index];
            num_name_index++;
        }
    }
    if(num_lenght > 0 && found_colon && num_name_lenght > 0)
        set_array(num_ptr, num_name_ptr, num_lenght, num_name_lenght);

    is_file_valid = build_number_name(number_str, output_num);
        
    free_memory();
        
    close(fd);
    return is_file_valid;
}
