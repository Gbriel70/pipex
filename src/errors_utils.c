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