/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 10:38:31 by fberthou          #+#    #+#             */
/*   Updated: 2024/06/26 10:53:19 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	close_pipes(int **fds, int size)
{
	int status;
	int i;

	i = 0;
	status = 0;
	while (i < size)
	{
		if (fds[i][0] != STDIN_FILENO)
			if (close(fds[i][0]) == -1)
				status = -1;
		if (fds[i][1] != STDIN_FILENO)
			if (close(fds[i][1]) == -1)
				status = -1;
		fds[i][0] = 0;
		fds[i][1] = 1;
		i++;
	}
	return (status);
}

int close_fds(int **fds, int size, int in_out[2])
{
	int status;

	status = 0;
	if (fds)
		status = close_pipes(fds, size);
	if (in_out)
	{
		if (in_out[0] != STDIN_FILENO)
			if (close(in_out[0]) == -1)
				status = -1;
		if (in_out[1] != STDIN_FILENO)
			if (close(in_out[1]) == -1)
				status = -1;
		in_out[0] = 0;
		in_out[1] = 1;
	}
	return (status);
}

void  free_pipes(int **tab, int size)
{
  int i;

  if (!tab)
    return;
  i = 0;
  while (i < size)
    free(tab[i++]);
  free(tab);
}

bool init_pipe(int ***pipe_tab, int size)
{
  int i;

  i = 0;
  *pipe_tab = malloc(size * sizeof(int *));
  if (!*pipe_tab)
    return (ft_perror("error -> alloc fd\n"), 1);
  while (i < size)
  {
    *pipe_tab[i] = malloc(2 * sizeof(int));
    if (!*pipe_tab[i])
      return (free_pipes(*pipe_tab, i), ft_perror("error -> alloc fd\n"), 1);
    i++;
  }
  i = 0;
  while (i < size)
  {
    if (pipe(*pipe_tab[i]) == -1)
    {
      close_fds(*pipe_tab, i, NULL);
      return (free_pipes(*pipe_tab, size), perror("init pipe\n"), 1);
    }
    i++;
  }
  return (0);
}
