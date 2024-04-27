/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhastag <skhastag@student.42heilbornn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 04:02:18 by skhastag          #+#    #+#             */
/*   Updated: 2024/04/27 19:40:54 by skhastag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	perror_free_exit(t_pipex_data *data, char *str, int err_num)
{
	int	i;

	i = -1;
	while (data->cmd_paths && data->cmd_paths[++i])
		free(data->cmd_paths[i]);
	free(data->cmd_paths);
	i = -1;
	while (data->cmd_paths && data->first_cmd_args[++i])
		free(data->first_cmd_args[i]);
	if (data->first_cmd_args)
		free(data->first_cmd_args);
	if (data->first_cmd)
		free(data->first_cmd);
	i = 0;
	while (data->second_cmd_args && data->second_cmd_args[++i])
		free(data->second_cmd_args[i]);
	if (data->second_cmd_args)
		free(data->second_cmd_args);
	if (data->second_cmd)
		free(data->second_cmd);
	//close_all(data);
	if (str)
		perror_exit(str, err_num);
}

void	perror_exit(char *str, int err_num)
{
	write(2, str, ft_strlen(str));
	exit(err_num);
}

char	*join_cmd(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*temp1;

	i = -1;
	while (*(paths + ++i))
	{
		temp1 = ft_strjoin(*(paths + i), "/");
		temp = ft_strjoin(temp1, cmd);
		free(temp1);
		if (access(temp, F_OK | X_OK | R_OK) == 0)
			return (temp);
		free(temp);
	}
	return (NULL);
}

char	*find_path(char *env[])
{
	while (ft_strnstr(*env, "PATH", 4) == NULL)
		env++;
	return (*env + 5);
}

void	init_pipex_data(int argc, char *argv[], char *env[], t_pipex_data *data)
{
	data->infile = open(argv[1], O_RDONLY);
	if (data->infile == 0)
		perror_free_exit(data, "Infile opening failed\n", 127);
	data->outfile = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (data->outfile == 0)
		perror_free_exit(data, "Outfile opening failed\n", 127);
	data->paths = find_path(env);
	data->cmd_paths = ft_split(data->paths, ':');
	data->first_cmd_args = process_argv(argv[2]);
	data->first_cmd = join_cmd(data->cmd_paths, data->first_cmd_args[0]);
	if (!data->first_cmd)
		perror_free_exit(data, "first command null", EXIT_FAILURE);
	data->second_cmd_args = process_argv(argv[3]);
	data->second_cmd = join_cmd(data->cmd_paths, data->second_cmd_args[0]);
	if (!data->second_cmd)
		perror_free_exit(data, "second command null", EXIT_FAILURE);
}
