/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 10:42:26 by florian           #+#    #+#             */
/*   Updated: 2024/07/29 14:21:56 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void free_pipes(int **tab, int size)
{
    int i;

    if (!tab)
        return;
    for (i = 0; i < size; i++)
    {
        if (tab[i] != NULL)
        {
            free(tab[i]);
            tab[i] = NULL;
        }
    }
    free(tab);
}


int close_pipes(int **fds, int size, int i_start, int last_fd)
{
  int status = 0;

  status = 0;
  if (last_fd > STDIN_FILENO)
  {
      if (close(last_fd) == -1)
          status = -1;
  }
  while (i_start < size)
  {
      if (fds != NULL && fds[i_start] != NULL)
      {
          if (fds[i_start][1] > STDOUT_FILENO)
          {
              if (fds[i_start][1] != -1)
              {
                  if (close(fds[i_start][1]) == -1)
                      status = -1;
                  else
                      fds[i_start][1] = -1;
              }
          }
          if (fds[i_start][0] > STDIN_FILENO)
          {
              if (fds[i_start][0] != -1)
              {
                  if (close(fds[i_start][0]) == -1)
                      status = -1;
                  else
                      fds[i_start][0] = -1;
              }
          }
      }
      i_start++;
  }
  if (status == -1)
      perror("close_fds ");
  return status;
}


int **init_pipe(t_data *data, int size)
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
      close_pipes(pipe_tab, i, 0, 0);
      return (free_pipes(pipe_tab, size), perror("init pipe\n"), NULL);
    }
    i++;
  }
  return (pipe_tab);
}
