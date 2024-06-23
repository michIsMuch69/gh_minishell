/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exec_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:58:51 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/23 15:08:25 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
int	ft_getenv(char *word, char **env, char **var_content);


int	exec_found(const char *dirname, char *exec_searched)
{
	DIR				    *dir;
	struct dirent	*entity;

  //printf("cmd == %s\ndirename == %s\n", exec_searched, dirname);
	dir = opendir(dirname);
	if (!dir)
  {
    if (errno == 2)
      return (0);
		return (perror("opendir"), -1);
  }
  //printf("\n\n");
	entity = readdir(dir);
  if (!entity && errno)
    return (perror("readdir"), closedir(dir), -1);
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
        break;
      return (perror("readdir"), closedir(dir), -1);
    }
	}
	if (closedir(dir) == -1)
    return (perror("closedir"), -1);
	return (0);
}

int check_all_dirs(t_data *data, char **directory)
{
	int			i;
  int     ret_value;
	char    *tmp;
	char		**path_list;

	i = 0;
	if (ft_getenv("PATH", data->env.tab, &tmp) == -1)
		return (-1);
  if (!tmp)
    return (ft_perror("PATH not in env\n"), 1);
	path_list = ft_split(tmp, ':'); // path list est un tableau de tableau avec $PATH split ':'
  free(tmp);
	if (!path_list)
		return (ft_perror("error-> split PATH\n"), -1);
	while (path_list[i])
	{
    ret_value = exec_found(path_list[i], data->args.tab[0]);
		if (ret_value == -1)
      return (free_array(path_list), -1);
    if (ret_value == 1)
		{
			*directory = ft_strdup(path_list[i]);// dup le path jusqu'au binnaire de la commande
      if (!*directory)
        return (ft_perror("error-> strdup path\n"), free_array(path_list), -1);
			break ;
		}
		i++;
	}
  printf("CMD == %s\n", data->cmd_path);
	free_array(path_list);
	return (0);
}

char	*ft_concat_path(char *directory, char *prompt)
{
	size_t	total_length;
	char	*exec_path;

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
