/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_extra_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhastag <skhastag@student.42heilbornn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 04:14:55 by skhastag          #+#    #+#             */
/*   Updated: 2024/04/27 19:40:18 by skhastag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*remove_double_quotes(char *str, int len)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		perror("Malloc failed\n");
	while (str[i] != '\0')
	{
		if (str[i] == '\'' && str[i + 1] == '\"')
			result[j++] = str[i++];
		else if (str[i] != '\"')
			result[j++] = str[i++];
		else if (str[i] == '\"' && str[i + 1] == '\'')
		{
			i++;
			result[j++] = str[i++];
		}
		else
			i++;
	}
	result[j] = '\0';
	return (result);
}

int	count_strings(char *ptr)
{
	int	count;

	count = 0;
	while (*ptr != '\0')
	{
		if (*ptr == '\'')
			count++;
		ptr++;
	}
	return (count);
}

char	**extract_strings(const char *input, int size, int count, int index)
{
	char	**result;
	int		start_index;
	int		length;

	result = (char **)malloc((size + 1) * sizeof(char *));
	start_index = -1;
	while (input[index] != '\0' && count < size)
	{
		if (input[index] == '\'' && start_index == -1)
			start_index = index + 1;
		else if (input[index] == '\'' && start_index != -1)
		{
			length = index - start_index;
			result[count] = (char *)malloc((length + 1) * sizeof(char));
			ft_strlcpy(result[count], input + start_index, length + 1);
			result[count][length] = '\0';
			count++;
			start_index = -1;
		}
		index++;
	}
	result[count] = NULL;
	return (result);
}

char	*extract_substring(const char *input_string)
{
	int			length;
	const char	*ptr = input_string;
	char		*extracted_string;

	length = 0;
	while (*ptr != '\0' && *ptr != '\'' && *ptr != '\"')
	{
		length++;
		ptr++;
	}
	extracted_string = (char *)malloc((length + 1) * sizeof(char));
	if (extracted_string == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(extracted_string, input_string, length + 1);
	extracted_string[length] = '\0';
	return (extracted_string);
}

char	**add_arrays(char **array_1, char **array_2)
{
	int		length_1;
	int		length_2;
	char	**result;
	int		i;
	int		j;

	length_1 = 0;
	while (array_1 && array_1[length_1])
		length_1++;
	length_2 = 0;
	while (array_2 && array_2[length_2])
		length_2++;
	result = malloc((length_1 + length_2 + 2) * sizeof(char *));
	if (!result)
		exit(EXIT_FAILURE);
	i = 0;
	while (array_1 && array_1[i])
	{
		result[i] = ft_strdup(array_1[i]);
		if (!result[i])
			exit(EXIT_FAILURE);
		i++;
	}
	j = 0;
	while (array_2 && array_2[j])
	{
		result[length_1 + j] = ft_strdup(array_2[j]);
		if (!result[length_1 + j])
			exit(EXIT_FAILURE);
		j++;
	}
	result[length_1 + length_2 + 1] = NULL;
	return (result);
}

void	free_2d_array(char **array)
{
	int	i;

	if (array == NULL)
		return ;
	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**process_argv(char *str)
{
	char	**temp;
	char	**temp_1;
	char	**result;
	char	*str_temp;
	int		i;

	i = -1;
	temp = NULL;
	temp_1 = NULL;
	if (ft_strnstr(str, " '", ft_strlen(str)))
		temp = extract_strings(str, count_strings(str) / 2, 0, 0);
	str_temp = extract_substring(str);
	temp_1 = ft_split(str_temp, ' ');
	result = add_arrays(temp_1, temp);
	free_2d_array(temp_1);
	free_2d_array(temp);
	return (result);
}
