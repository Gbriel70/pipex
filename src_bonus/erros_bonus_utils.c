#include "pipex_bonus.h"

#include "../include/pipex.h"

void ft_free_matrix(char **matrix, int size)
{
    int i;

    i = 0;
    while (i < size)
    {
        free(matrix[i]);
        i++;
    }
    free(matrix);
}

void ft_close_pipes(int **pipe, int n)
{
    int i;

    i = 0;
    while (i < n)
    {
        close(pipe[i][0]);
        close(pipe[i][1]);
        free(pipe[i]);
        i++;
    }
    free(pipe);
}

void ft_free_pipex(t_data *pipex, t_fd *fd, int stage)
{
    int i;

    i = 0;
    if (stage >= 1)
    {
        ft_free_matrix(pipex->path, pipex->path_quantity);
        if (stage >= 2)
        {
            while (i < pipex->comands_quantity)
            {
                ft_free_matrix(pipex->comands_args[i], pipex->comands_args_quantity[i]);
                i++;
            }
            free(pipex->comands_args);
            free(pipex->comands_args_quantity);
        }
        if (stage >= 3)
            ft_close_pipes(fd->pipe, pipex->pipe_quantity);
        if (stage == 4)
            free(pipex->pid);
    }
}

void ft_handle_errors(char *error_message, t_data *pipex, t_fd *fd, int stage)
{
    ft_free_pipex(pipex, fd, stage);
    perror(error_message);
    exit(EXIT_FAILURE);
}

void wait_finish_pipe(t_fd *fd, t_data *pipex)
{
    int i;

    i = 0;
    ft_close_pipes(fd->pipe, pipex->pipe_quantity);
    while (i < pipex->comands_quantity)
    {
        if (waitpid(pipex->pid[i], &pipex->wait_status, 0) == -1)
            ft_handle_errors("waitpid failed", pipex, fd, 1);
        if (WIFEXITED(pipex->wait_status))
            pipex->exit_status = WEXITSTATUS(pipex->wait_status);
        else if (WIFSIGNALED(pipex->wait_status))
            pipex->exit_status = WTERMSIG(pipex->wait_status);
        if (pipex->exit_status == 127)
            ft_printf("%s: command not found\n", pipex->comands_args[i][0]);
        if (pipex->exit_status == 126)
            ft_printf("%s: permission denied\n", pipex->comands_args[i][0]);
        i++;
    }
    ft_free_pipex(pipex, fd, 2);
    free(pipex->pid);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}