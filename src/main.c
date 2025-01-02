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
    pipex->path_quantity = ft_count_args(envp[i] + 5, ':');
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

void get_comands_args(char **av, t_data *pipex)
{
    int i;

    pipex->comands_quantity = pipex->ac - 3;
    pipex->comands_args = ft_calloc((pipex->comands_quantity + 1), sizeof(char **));
    if (!pipex->comands_args)
        ft_handle_errors("cmd calloc failed", pipex, NULL, 1);
    pipex->comands_args_quantity = ft_calloc((pipex->comands_quantity + 1), sizeof(char **));
    if (!pipex->comands_args_quantity)
    {
        free(pipex->comands_args);
        ft_handle_errors("args count calloc failed", pipex, NULL, 1);
    }
    i = 0;
    while (i < pipex->comands_quantity)
    {
        pipex->comands_args_quantity[i] = ft_count_args(av[i + 2], ' ');
        pipex->comands_args[i] = ft_split_args(av[i + 2], ' ');
        if (!pipex->comands_args[i])
        {
            free(pipex->comands_args);
            free(pipex->comands_args_quantity);
            ft_handle_errors("split comands failed", pipex, NULL, 1);
        }
        i++;
    }
}

void create_pipes(t_fd *fd, t_data *pipex)
{
    int i;

    pipex->pipe_quantity = pipex->comands_quantity - 1;
    fd->pipe = malloc(pipex->pipe_quantity * sizeof(int *));
    if (!fd->pipe)
        ft_handle_errors("pipe malloc failed", pipex, NULL, 2);
    fd->pipe[pipex->pipe_quantity] = NULL;
    i = 0;
    while (i < pipex->pipe_quantity)
    {
        fd->pipe[i] = malloc(2 * sizeof(int));
        if (!fd->pipe[i])
        {
            ft_close_pipes(fd->pipe, i);
            ft_handle_errors("pipe malloc failed", pipex, NULL, 2);
        }
        if (pipe(fd->pipe[i]) == -1)
        {
            free(fd->pipe[i]);
            ft_close_pipes(fd->pipe, i);
            ft_handle_errors("pipe error", pipex, fd, 2);
        }
        i++;
    }
}

int main(int ac, char **av, char **envp)
{
    t_data pipex;
    t_fd fd;
    int i;

    check_args(ac, &pipex);
    get_path(envp, &pipex);
    get_comands_args(av, &pipex);
    create_pipes(&fd, &pipex);
    pipex.pid = malloc(pipex.comands_quantity * sizeof(int));
    if (!pipex.pid)
        ft_handle_errors("pid malloc failed", &pipex, &fd, 3);
    i = 0;
    while (i < pipex.comands_quantity)
    {
        pipex.pid[i] = fork();
        if (pipex.pid[i] < 0)
            ft_handle_errors("fork failed", &pipex, &fd, 4);
        if (pipex.pid[i] == 0)
            exec_comand(i, fd, av, &pipex);
        i++;
    }
    wait_finish_pipe(&fd, &pipex);
    return 0;
}