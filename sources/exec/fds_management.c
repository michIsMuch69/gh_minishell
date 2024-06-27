/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 10:38:31 by fberthou          #+#    #+#             */
/*   Updated: 2024/06/27 18:25:35 by florian          ###   ########.fr       */
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
		if (fds[i][0] != STDIN_FILENO && fds[i][1] != STDERR_FILENO && \
			fds[i][0] != -1)
			if (close(fds[i][0]) == -1)
				status = -1;
		if (fds[i][1] != STDOUT_FILENO && fds[i][1] != STDERR_FILENO && \
			fds[i][1] != -1)
			if (close(fds[i][1]) == -1)
				status = -1;
		fds[i][0] = 0;
		fds[i][1] = 1;
		i++;
	}
	if (status == -1)
		perror("close_fds ");
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
		if (in_out[0] != STDIN_FILENO && in_out[0] != STDERR_FILENO && \
        in_out[0] != -1)
			if (close(in_out[0]) == -1)
				status = -1;
		if (in_out[1] != STDOUT_FILENO && in_out[1] != STDERR_FILENO && \
        in_out[1] != -1)
			if (close(in_out[1]) == -1)
				status = -1;
		in_out[0] = 0;
		in_out[1] = 1;
	}
	if (status == -1)
		perror("close_fds :");
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

int **init_pipe(int size)
{
  int i;
  int **pipe_tab;

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
      close_fds(pipe_tab, i, NULL);
      return (free_pipes(pipe_tab, size), perror("init pipe\n"), NULL);
    }
    i++;
  }
  return (pipe_tab);
}
