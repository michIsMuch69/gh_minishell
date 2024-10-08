/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 10:42:26 by florian           #+#    #+#             */
/*   Updated: 2024/08/01 07:05:00 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	free_pipes(int **tab, int size)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (i < size)
	{
		if (tab[i] != NULL)
		{
			free(tab[i]);
			tab[i] = NULL;
		}
		i++;
	}
	free(tab);
}

int	**init_pipe(t_data *data, int size)
{
	int	i;
	int	**pipe_tab;

	i = 0;
	pipe_tab = malloc(size * sizeof(int *));
	if (!pipe_tab)
		return (ft_perror("error -> alloc fd\n"), NULL);
	while (i < size)
	{
		pipe_tab[i] = malloc(2 * sizeof(int));
		if (!pipe_tab[i])
			return (free_pipes(pipe_tab, i), ft_perror("error -> alloc fd\n"), NULL);
		i++;
	}
	i = 0;
	while (i < size)
	{
		if (pipe(pipe_tab[i]) == -1)
		{
			close_fds(pipe_tab, i, 0, 0);
			return (free_pipes(pipe_tab, size), perror("init pipe\n"), NULL);
		}
		i++;
	}
	return (pipe_tab);
}
