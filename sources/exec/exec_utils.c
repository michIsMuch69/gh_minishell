/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:59:06 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/25 18:50:02 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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
int close_fds(int **fds, int size)
{
  int status;
  int i;

  i = 0;
  status = 0;
  if (!fds)
    return (0);
  while (i < size)
  {
    if (close(fds[i][0]) == -1 || close(fds[i][1]) == -1)
      status = -1;
    i++;
  }
  return (status);
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
      close_fds(*pipe_tab, i);
      return (free_pipes(*pipe_tab, size), perror("init pipe\n"), 1);
    }
    i++;
  }
  return (0);
}

int	clean_struct(t_data *data) // prend une instances du tableau *data
{
	int ret_value;

  if (!data->args.tab)
    return (0); // don't have command
	ret_value = expand_management(data, data->env.tab);
	if (ret_value == -1)
		return (-1); // -> crash
	if (ret_value == 1)
		return(1); // -> back to prompt
	ret_value = token_cleaner(data);
	if (ret_value == 1)
		return (1); // -> back to prompt
	if (ret_value == -1)
		return (-1); // -> crash
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}
