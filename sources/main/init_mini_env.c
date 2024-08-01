/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mini_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:03:02 by jedusser          #+#    #+#             */
/*   Updated: 2024/08/01 10:05:47 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	free_env_min(char **env_min)
{
	int i;

	i = 0;
	while (env_min[i] != NULL)
	{
		free(env_min[i]);
		i++;
	}
	free(env_min);
}

char	**create_minimal_env(void)
{
	char	**env_min;
	char	cwd[1024];
	char	*temp;

	env_min = malloc(4 * sizeof(char *));
	if (!env_min)
		return (NULL);
	temp = getcwd(cwd, sizeof(cwd));
	if (temp == NULL)
		return (free_env_min(env_min), NULL);
	env_min[0] = ft_strjoin("PWD=", temp);
	if (!env_min[0])
		return (free_env_min(env_min), NULL);
	env_min[1] = ft_strjoin("SHLVL=", "1");
	if (!env_min[1])
		return (free_env_min(env_min), NULL);
	env_min[2] = ft_strjoin("_=", "/usr/bin/env");
	if (!env_min[2])
		return (free_env_min(env_min), NULL);
	env_min[3] = NULL;
	return (env_min);
}
