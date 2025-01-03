#include "pipex_bonus.h"

void check_ac_and_heredoc(int ac, char **av, t_data *pipex)
{
    if (ft_strncmp(av[1], "here_doc", ft_strlen("here_doc")) == 0)
    {
        if (ac < 6)
        {
            ft_printf("error: Usage: ./pipex here_doc EOF cmd1 cmd2 [cmdN] outfile\n");
            exit(EXIT_FAILURE);
        }
        pipex->heredoc.flag = 1;
        pipex->heredoc.eof = av[2];
        pipex->heredoc.eof_l = ft_strlen(av[2]);
        pipex->comand_offset = 3;
    }
    else
    {
        if (ac < 5)
        {
            ft_printf("error: Usage: ./pipex input_file cmd1 cmd2 [cmd3 ...] outfile\n");
            exit(EXIT_FAILURE);
        }
        pipex->heredoc.flag = 0;
        pipex->comand_offset = 2;

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
        ft_handle_error("split path failed", NULL, NULL, 0);
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

    pipex->comand_quantity = pipex->ac - 1 - pipex->comand_offset;
    pipex->comand_args = ft_calloc((pipex->comand_quantity + 1), sizeof(char **));
    if (!pipex->comand_args)
    {
        free(pipex->comand_args);
        ft_handle_errors("args count calloc failed", pipex, NULL, 1);
    }
    i = 0;
    while (i < pipex->comand_quantity)
    {
        pipex->args_quantity[i] = ft_count_args(av[i + pipex->comand_offset], ' ');
        pipex->comand_args[i] = ft_split_args(av[i + pipex->comand_offset], ' ');
        if (!pipex->comand_args[i])
        {
            free(pipex->comand_args);
            free(pipex->args_quantity);
            ft_handle_errors("split comands failed", pipex, NULL, 1);
        }
        i++;
    }
}

void create_pipes(t_data *pipex, t_fd *fd)
{
    int i;
    pipex->pipe_quantity = pipex->comand_quantity - 1;
    fd->pipe = malloc((pipex->pipe_quantity + 1) * sizeof(int *));
    if (!fd->pipe)
        ft_handle_errors("pipe malloc failed", pipex, fd, 2);
    fd->pipe[pipex->pipe_quantity] = NULL;
    i = 0;
    while (i < pipex->pipe_quantity)
    {
        fd->pipe[i] = malloc(2 * sizeof(int));
        if (!fd->pipe[i])
        {
            ft_close_pipes(fd->pipe, i);
            ft_handle_errors("pipe malloc failed", pipex, fd, 2);
        }
        if (pipe(fd->pipe[i]) == -1)
        {
            free(fd->pipe[i]);
            ft_close_pipes(fd->pipe, i);
            ft_handle_errors("pipe failed", pipex, fd, 2);
        }
        i++;
    }
}

int main(int ac, char **av, char **envp)
{
    t_data pipex;
    t_fd fd;
    int i;

    check_ac_and_heredoc(ac, av, &pipex);
    get_path(envp, &pipex);
    get_comands_args(av, &pipex);
    create_pipes(&fd, &pipex);
    pipex.pid = malloc(pipex.comand_quantity * sizeof(int));
    if (!pipex.pid)
        ft_handle_errors("pid malloc failed", &pipex, &fd, 3);
    i = 0;
    while (i < pipex.comand_quantity)
    {
        pipex.pid[i] = fork();
        if (pipex.pid[i] < 0)
            ft_handle_errors("fork failed", &pipex, &fd, 4);
        if (pipex.pid[i] == 0)
            exec_comand(i, fd, av, &pipex);
        i++;
    }
    wait_finish_pipe(&fd, &pipex);
    return (pipex.exit_code);
}