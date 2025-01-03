#include "pipex_bonus.h"

size_t ft_calculate_substrings_in_quotes(char const *str)
{
    size_t l;

    l = 0;
    if (str[l] == '\'')
    {
        l++;
        while (str[l] && str[l] != '\'')
            l++;
        l++;
    }
    else if (str[l] == '\"')
    {
        l++;
        while (str[l] && str[l] != '\"')
            l++;
        l++;
    }
    return l;
}

int ft_count_args(char const *str, char c)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while(str[i])
    {
        while(str[i] == c)
            i++;
        if (str[i] && str[i] != c)
            count++;
        if (str[i] == '\'' || str[i] == '\"')
            i += ft_calculate_substrings_in_quotes(&str[i]);
        while (str[i] && str[i] != c)
            i++;
    }
    return count;
}

size_t ft_arg_len(const char *str, char c)
{
    size_t l;
    
    l = 0;
    while (str[l] && str[l] != c)
    {
        if (str[l] == '\'' || str[l] == '\"')
            l += ft_calculate_substrings_in_quotes(&str[l]);
        l++;
    }
    return l;
}

char *ft_trim_quotes(char *str, char *quotes)
{
    size_t start;
    size_t end;
    size_t l;
    char *trimmed;

    if (!str || !quotes)
        return NULL;
    start = 0;
    end = ft_strlen(str);
    if (ft_strchr(quotes, str[start] && start < end))
        start++;
    if (ft_strrchr(quotes, str[end - 1] && end > start))
        end--;
    l = end - start;
    trimmed = malloc((l + 1) * sizeof(char));
    if (!trimmed)
        return NULL;
    ft_strlcpy(trimmed, &str[start], l + 1);
    free(str);
    return trimmed;
}

char **ft_split_args(char const *str, char c)
{
    char **array;
    int args_count;
    int i;
    int j;

    args_count = ft_count_args(str, c);
    array = malloc((args_count + 1) * sizeof(char *));
    if (!array)
        return NULL;
    i = 0;
    j = -1;
    while (++j < args_count)
    {
        while (str[i] == c )
            i++;
        array[j] = malloc((ft_arg_len(&str[i], c) + 1) * sizeof(char));
        if (!array[j])
        {
            ft_free_matrix(array, j);
            return NULL;
        }
        ft_strlcpy(array[j], &str[i], ft_arg_len(&str[i], c) + 1);
        array[j] = ft_trim_quotes(array[j], "\'\"");
        i = i + ft_arg_len(&str[i], c);
    }
    array[j] = NULL;
    return array;
}