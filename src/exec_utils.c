/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:22:31 by gcosta-m          #+#    #+#             */
/*   Updated: 2025/01/07 17:42:04 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	open_input_file(t_fd *fd, char **av, t_data *pipex)
{
	fd->input_file = open(av[1], O_RDONLY);
	if (fd->input_file < 0)
	{
		ft_printf("%s: ", av[1]);
		ft_handle_errors("", pipex, fd, 4);
	}
}

void	open_output_file(t_fd *fd, char **av, t_data *pipex)
{
	char	*output_file_path;

	output_file_path = av[pipex->ac - 1];
	fd->output_file = open(output_file_path, O_WRONLY | O_TRUNC | O_CREAT,
			0777);
	if (fd->output_file < 0)
	{
		ft_printf("%s: ", av[pipex->ac - 1]);
		ft_handle_errors("", pipex, fd, 4);
	}
}

void	test_comand_permission(char **path, char **comand_args, t_data *pipex)
{
	int	i;

	i = 0;
	while (path[i])
	{
		pipex->exit_status = 0;
		if (comand_args[0][0] == '/')
			pipex->path_name = ft_str_remove_dup(comand_args[0], '/');
		else
			pipex->path_name = ft_strjoin(path[i], comand_args[0]);
		if (access(pipex->path_name, F_OK) == -1)
		{
			pipex->exit_status = 127;
			free(pipex->path_name);
			i++;
		}
		else if (access(pipex->path_name, X_OK) == -1)
		{
			pipex->exit_status = 126;
			free(pipex->path_name);
			break ;
		}
		else
			return ;
	}
}

int	redirect_stdin_stdout(int i, t_fd *fd, t_data *pipex)
{
	if (i == 0)
	{
		if (dup2(fd->input_file, STDIN_FILENO) == -1)
			return (-1);
		close(fd->input_file);
	}
	else
	{
		if (dup2(fd->pipe[i - 1][0], STDIN_FILENO) == -1)
			return (-1);
	}
	if (i == pipex->cmd_qtd - 1)
	{
		if (dup2(fd->output_file, STDOUT_FILENO) == -1)
			return (-1);
		close(fd->output_file);
	}
	else
	{
		if (dup2(fd->pipe[i][1], STDOUT_FILENO) == -1)
			return (-1);
	}
	ft_close_pipes(fd->pipe, pipex->pipe_quantity);
	return (0);
}

void	exec_comand(int i, t_fd fd, char **av, t_data *pipex)
{
	if (i == 0)
		open_input_file(&fd, av, pipex);
	if (i == pipex->cmd_qtd - 1)
		open_output_file(&fd, av, pipex);
	if (pipex->comands_args[i][0] == 0)
		pipex->exit_status = 126;
	else
		test_comand_permission(pipex->path, pipex->comands_args[i], pipex);
	if (redirect_stdin_stdout(i, &fd, pipex) == -1)
		ft_handle_errors("second dup error", pipex, &fd, 4);
	if (pipex->exit_status == 0)
	{
		execve(pipex->path_name, pipex->comands_args[i], pipex->envp);
		pipex->exit_status = -1;
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	ft_free_pipex(pipex, &fd, 2);
	free(pipex->pid);
	exit(pipex->exit_status);
}
