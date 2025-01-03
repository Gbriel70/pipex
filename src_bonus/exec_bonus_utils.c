#include "../include/pipex_bonus.h"

void open_input_file(t_fd *fd, char **av, t_data *pipex, t_hdoc *heredoc)
{
    if (heredoc->flag == 0)
    {
        fd->input_file = open(av[1], O_RDONLY);
        if (fd->input_file < 0)
        {
            ft_printf("%s: ", av[1]);
            ft_handle_errors("", pipex, fd, 4);
        }
    }
    else if (heredoc->flag == 1)
    {
        pipe(heredoc->pipe);
        fd->input_file = heredoc->pipe[0];
        ft_printf(">");
        heredoc->line = ft_get_next_line(STDIN_FILENO);
        while (ft_strncmp(heredoc->line, heredoc->eof, heredoc->eof_l) != 0)
        {
            ft_putstr_fd(heredoc->line, heredoc->pipe[1]);
            free(heredoc->line);
            ft_printf(">");
            heredoc->line = ft_get_next_line(STDIN_FILENO);
        }
        free(heredoc->line);
        close(heredoc->pipe[1]);
    }
}

void open_output_file(t_fd *fd, char **av, t_data *pipex)
{
    char *outfile_path;

    outfile_path = av[pipex->ac - 1];
    if (pipex->heredoc.flag == 0)
    {
        fd->output_file = open(outfile_path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
        if (fd->output_file < 0)
        {
            ft_printf("%s: ", av[pipex->ac - 1]);
            ft_handle_errors("", pipex, fd, 4);
        }
    }
    else if (pipex->heredoc.flag == 1)
    {
        fd->output_file = open(outfile_path, O_WRONLY | O_APPEND | O_CREAT, 0777);
        if (fd->output_file < 0)
        {
            ft_printf("%s: ", av[pipex->ac - 1]);
            ft_handle_errors("", pipex, fd, 4);
        }
    }
}

void  test_command_permission(char **path, char **comand_args, t_data *pipex)
{
    int i;

    i = 0;
    while (path[i])
    {
        pipex->exit_code = 0;
        if (comand_args[0][0] == '/')
            pipex->pathname = ft_str_remove_dup(comand_args[0], '/');
        else
            pipex->pathname = ft_strjoin(path[i], comand_args[0]);
        if (access(pipex->pathname, F_OK) == -1)
        {
            pipex->exit_code = 127;
            free(pipex->pathname);
            i++;
        }
        else if (access(pipex->pathname, X_OK) == -1)
        {
            pipex->exit_code = 126;
            free(pipex->pathname);
            break;
        }
        else
            return ;
    }
}

int redirect_stdin_stdout(int i, t_fd *fd, t_data *pipex)
{
    if (i == 0)
    {
        if (dup2(fd->input_file, STDIN_FILENO) == -1)
            return -1;
        close(fd->input_file);
    }
    else
    {
        if (dup2(fd->pipe[i - 1][0], STDIN_FILENO) == -1)
            return -1;
    }
    if (i == pipex->comand_quantity - 1)
    {
        if (dup2(fd->output_file, STDOUT_FILENO) == -1)
            return -1;
        close(fd->output_file);
    }
    else
    {
        if (dup2(fd->pipe[i][1], STDOUT_FILENO) == -1)
            return -1;
    }
    ft_close_pipes(fd->pipe, pipex->pipe_quantity);
    return 0;
}

void exec_comand(int i, t_fd fd, char **av, t_data *pipex)
{
    if (i == 0)
        open_input_file(&fd, av, pipex, &pipex->heredoc);
    if (i == pipex->comand_quantity - 1)
        open_output_file(&fd, av, pipex);
    if (pipex->comand_args[i][0] == 0)
        pipex->exit_code = 126;
    else
        test_command_permission(pipex->path, pipex->comand_args[i], pipex);
    if (redirect_stdin_stdout(i, &fd, pipex) == -1)
        ft_handle_errors("dup2 failed", pipex, &fd, 4);
    if (pipex->exit_code == 0)
    {
        execve(pipex->pathname, pipex->comand_args[i], pipex->envp);
        pipex->exit_code = -1;
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    ft_free_pipex(pipex, &fd, 2);
    free(pipex->pid);
    exit(pipex->exit_code);
}