/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exec_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:58:51 by jedusser          #+#    #+#             */
/*   Updated: 2024/08/01 07:08:41 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static char	**get_path(t_data *data)
{
	int		ret_value;
	char	*tmp;
	char	**path_list;

	ret_value = ft_getenv("PATH", data->env.tab, &tmp);
	if (ret_value == -1)
		return (NULL);
	if (ret_value == 1 || !tmp)
		return (ft_putstr_fd("cmd not found\n", 2), NULL);
	path_list = ft_split(tmp, ':');
	if (!path_list)
		return (ft_perror("error-> split PATH\n"), free(tmp), NULL);
	free(tmp);
	return (path_list);
}

static int	search_loop(DIR *dir, struct dirent *entity, char *exec_searched)
{
	while (entity != NULL)
	{
		if (ft_strcmp(entity->d_name, exec_searched) == 0)
		{
			if (closedir(dir) == -1)
				return (perror("closedir"), -1);
			return (1);
		}
		entity = readdir(dir);
		if (!entity && errno)
		{
			if (errno == 2)
				break ;
			return (perror("readdir"), closedir(dir), -1);
		}
	}
	if (closedir(dir) == -1)
		return (perror("closedir"), -1);
	return (0);
}

int	exec_found(const char *dirname, char *exec_searched)
{
	DIR				*dir;
	struct dirent	*entity;

	dir = opendir(dirname);
	if (!dir)
	{
		if (errno == 2)
			return (0);
		return (perror("opendir"), -1);
	}
	entity = readdir(dir);
	if (!entity && errno)
		return (perror("readdir"), closedir(dir), -1);
	return (search_loop(dir, entity, exec_searched));
}

int	check_all_dirs(t_data *data, char **directory)
{
	int		i;
	int		ret_value;
	char	**path_list;

	i = 0;
	path_list = get_path(data);
	if (!path_list)
		return (1);
	while (path_list[i])
	{
		ret_value = exec_found(path_list[i], data->args.tab[0]);
		if (ret_value == -1)
			return (free_array(path_list), 1);
		if (ret_value == 1)
		{
			*directory = ft_strdup(path_list[i]);
			if (!*directory)
				return (ft_perror("error-> strdup path\n"), free_array(path_list), 1);
			break ;
		}
		i++;
	}
	free_array(path_list);
	return (0);
}

char	*ft_concat_path(char *directory, char *prompt)
{
	size_t	total_length;
	char	*exec_path;

	if (!directory || !prompt)
		return (NULL);
	total_length = ft_strlen(directory) + ft_strlen(prompt) + 2;
	exec_path = ft_calloc(total_length, sizeof(char));
	if (!exec_path)
	{
		ft_perror("Memory allocation failed for exec_path\n");
		return (NULL);
	}
	ft_strcpy(exec_path, directory);
	ft_strcat(exec_path, "/");
	ft_strcat(exec_path, prompt);
	return (exec_path);
}
