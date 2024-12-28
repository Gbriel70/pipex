#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include "../libft_plus/includes/not_in_standard_includes/ft_printf_bonus.h"
# include "../libft_plus/includes/ft_string_functions.h"
# include "../libft_plus/includes/not_in_standard_includes/not_in_standard.h"

typedef struct s_fd
{
    int		input_file;
    int     output_file;
    int     **pipe;
}    t_fd;

typedef struct s_data
{
    int     ac;
    char    **envp;
    char    **path;
    int     path_quantity;
    char    *path_name;
    char    **comands_args;
    int     comands_quantity;
    int     *comands_args_quantity;
    int     pipe_quantity;
    pid_t   *pid;
    int     wait_status;
    int     exit_status;

} t_data;

//MAIN FUNCTIONS
void check_args(int ac, t_data *pipex);
void get_path(char **envp, t_data *pipex);

//PROCESS UTILS
size_t ft_calculate_substrings_in_quotes(char const *str);
int ft_count_args(char const *str, char c);
size_t ft_arg_len(const char *str, char c);
char *ft_trim_quotes(char *str, char *quotes);
char **ft_split_args(char const *s, char c);


//ERROR UTILS
void ft_free_matrix(char **matrix, int size);
void ft_free_pipex(t_data *pipex, t_fd *fd, int stage);
void ft_handle_errors(char *error_message, t_data *pipex, t_fd *fd, int stage);


#endif