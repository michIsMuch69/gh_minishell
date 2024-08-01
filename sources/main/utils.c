/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 10:56:02 by fberthou          #+#    #+#             */
/*   Updated: 2024/08/01 09:03:35 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ###### INCLUDES ######

#include "libft.h"
#include "struct.h"
#include <stdlib.h>

// ###### INCLUDES ######

void	free_tab(t_table *tab, int start)
{
	int	i;

	i = start;
	if (!tab->tab)
		return ;
	while (i < tab->size)
	{
		if (tab->tab[i])
		{
			free(tab->tab[i]);
			tab->tab[i] = NULL;
		}
		i++;
	}
	free(tab->tab);
	tab->tab = NULL;
}
char	**create_minimal_env()
{
	char	**env_min;
	char	*cwd[1024];
	char	*shlvl = "1";
	char	*underscore = "/usr/bin/env";
	env_min = malloc(4 * sizeof(char *));
	
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		env_min[0] = malloc((ft_strlen("PWD=") + ft_strlen(cwd)) * sizeof(char));
		if (env_min[0])
			return (NULL);
	}
	env_min[0] = ft_strcat("PWD=", cwd);


	env_min[1] = malloc((ft_strlen("SHLVL=") + ft_strlen(shlvl)) * sizeof(char));
	if (!env_min[1])
		return (NULL);
	env_min[1] = ft_strcat("SHLVL=", shlvl);


	env_min[2] = malloc((ft_strlen("_=") + ft_strlen(underscore)) * sizeof(char));
	if(!env_min[2])
		return (NULL);
	env_min[2] = ft_strcat("_=", underscore);

	
	env_min[3] = NULL;
	return (env_min);	
}


t_table	ft_tabdup(char **envp)
{
	t_table	tab_env;

	tab_env.size = 0;
	if (!envp)
		return (tab_env.tab = NULL, tab_env);
	while (envp[(tab_env.size)++])
		;
	tab_env.tab = ft_calloc(tab_env.size + 1, sizeof(char *));
	if (!tab_env.tab)
		return (tab_env);
	tab_env.size = 0;
	while (envp[tab_env.size])
	{
		tab_env.tab[tab_env.size] = ft_strdup(envp[tab_env.size]);
		if (!tab_env.tab[tab_env.size])
			return (free_tab(&tab_env, 0), tab_env.tab = NULL, tab_env);
		(tab_env.size)++;
	}
	return (tab_env);
}

int	ft_perror(char *err_message)
{
	return (write(2, err_message, ft_strlen(err_message)));
}

void	destroy_heredocs(t_table *heredoc)
{
	int	i;

	if (!heredoc->size)
		return ;
	i = 0;
	while (i < heredoc->size)
		unlink(heredoc->tab[i++]);
	free_tab(heredoc, 0);
	heredoc->tab = NULL;
}

void	free_struct(t_data *struc, int tab_size)
{
	int	i;

	i = 0;
	while (i < tab_size)
	{
		if (struc[i].cmd_path)
		{
			free(struc[i].cmd_path);
			if (struc[i].prompt)
				free(struc[i].prompt);
			struc[i].cmd_path = NULL;
			struc[i].prompt = NULL;
		}
		free_tab(&(struc[i].args), 0);
		struc[i].args.tab = NULL;
		free_tab(&(struc[i].input), 0);
		struc[i].input.tab = NULL;
		free_tab(&(struc[i].output), 0);
		struc[i].output.tab = NULL;
		destroy_heredocs(&(struc[i].docs_files));
		i++;
	}
	free(struc[0].prompt);
	free_tab(&(struc->env), 0);
	free_tab(&(struc->export), 0);
	free(struc);
}
