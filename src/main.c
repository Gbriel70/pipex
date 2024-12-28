#include "../include/pipex.h"

void check_args(int ac, t_data *pipex)
{
    if (ac != 5)
    {
        ft_printf("Error: Wrong number of arguments\n");
        ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
        exit(EXIT_FAILURE);
    }
    pipex->ac = ac;
}

void get_path(char **envp, t_data *pipex)
{
    int i;
    char *tmp;

    i = 0;
    while (ft_strnstr(envp[i], "PATH=", 5) == NULL)
        i++;
    pipex->path_quantity = ft_count_args(envp[i] + 5, ":");
    pipex->path = ft_split_args(envp[i] + 5, ':');
    if (!pipex->path)
        ft_handle_errors("split path failed", pipex, NULL, 0);
    i = 0;
    while (i < pipex->path_quantity)
    {
        tmp = ft_strjoin(pipex->path[i], "/");
        free(pipex->path[i]);
        pipex->path[i] = ft_strdup(tmp);
        free(tmp);
        i++;
    }
    pipex->envp = envp;
}

int main(int ac, char **av, char **envp)
{
    (void)av;
    (void)envp; 
    t_data pipex;
    //t_fd fd;
    //int i;

    check_args(ac, &pipex);
    get_path(envp, &pipex);

    return 0;
}