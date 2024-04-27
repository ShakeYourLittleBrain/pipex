/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhastag <skhastag@student.42heilbornn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 04:00:55 by skhastag          #+#    #+#             */
/*   Updated: 2024/04/27 19:40:53 by skhastag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	close_all(t_pipex_data *data)
{
	close(data->infile);
	close(data->outfile);
	close(data->fd_pid[0]);
	close(data->fd_pid[1]);
}

void	first_thread(t_pipex_data *data, char *env[])
{
	if (dup2(data->fd_pid[1], STDOUT_FILENO) == -1)
		perror_free_exit(data, "dup2 stdout failed!\n", 1);
	if (data->infile < 0)
		perror_free_exit(data, "infile failed!\n", 127);
	if (dup2(data->infile, STDIN_FILENO) == -1)
		perror_free_exit(data, "dup2 stdin failed!\n", 1);
	close(data->fd_pid[0]);
	if (data->first_cmd == NULL)
		perror_free_exit(data, "Command not found\n", 127);
	if (execve(data->first_cmd, data->first_cmd_args, env) == -1)
		perror_free_exit(data, "Execve cmd 1 failed!\n", 1);
}

void	second_thread(t_pipex_data *data, char *env[])
{
	if (dup2(data->fd_pid[0], STDIN_FILENO) == -1)
		perror_free_exit(data, "dup2 stdin failed!\n", 1);
	if (data->outfile < 0)
		perror_free_exit(data, "outfile failed!\n", 127);
	if (dup2(data->outfile, STDOUT_FILENO) == -1)
		perror_free_exit(data, "dup2 stdout failed!\n", 1);
	close(data->fd_pid[1]);
	if (data->second_cmd == NULL)
		perror_free_exit(data, "Command not found\n", 127);
	if (execve(data->second_cmd, data->second_cmd_args, env) == -1)
		perror_free_exit(data, "Execve  cmd 2 failed!\n", 1);
}

int	main(int argc, char *argv[], char *env[])
{
	t_pipex_data	data;
	int				status;

   	if (argc != 5)
		return (perror("Invalid number of arguments.\n"), 1);
	init_pipex_data(argc, argv, env, &data);
	if (pipe(data.fd_pid) < 0)
		perror_free_exit(&data, "Pipe creation error\n", 1);
	data.pids[0] = fork();
	if (data.pids[0] == 0)
		first_thread(&data, env);
	else if (data.pids[0] == -1)
		return (EXIT_FAILURE);
	waitpid(data.fd_pid[0], &status, 0);
	data.pids[1] = fork();
	if (data.pids[1] == 0)
		second_thread(&data, env);
	else if (data.pids[1] == -1)
		return (EXIT_FAILURE);
	close_all(&data);
	waitpid(data.fd_pid[1], &status, 0);
	perror_free_exit(&data, NULL, 0);
	return ((status & 0xff00) >> 8);
}
