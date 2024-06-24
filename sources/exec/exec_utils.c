/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:59:06 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/24 12:36:24 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

bool  init_fds(int **fds)
{
  *fds = malloc(4 * sizeof(int));
  if (!*fds)
    return (1);
  fds[0][0] = 0;
  fds[0][1] = 1;
  fds[0][2] = 0;
  fds[0][3] = 0;
  return (0);
}

void  close_free_fds(int *fds)
{
  if (fds[0] > 2)
    close(fds[0]);
  if (fds[1] > 2)
    close(fds[1]);
  if (fds[2] > 2)
    close(fds[2]);
  if (fds[3] > 2)
    close(fds[3]);
  free(fds);
}

int	clean_struct(t_data *data)
{
	int ret_value;

  if (!data->args.tab)
    return (0); // don't have command
	ret_value = expand_management(data, data->env.tab);
	if (ret_value == -1)
		return (-1); // crash -> fermer minishell
	if (ret_value == 1)
		return(1); // -> rendre invite de commande
	ret_value = token_cleaner(data);
	if (ret_value == 1)
		return (1); // -> rendre invite de commande
	if (ret_value == -1)
		return (-1); // crash -> fermer minishell
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
