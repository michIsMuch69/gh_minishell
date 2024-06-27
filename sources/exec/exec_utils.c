/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:59:06 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/26 16:51:20 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	clean_struct(t_data *data) // prend une instances du tableau *data
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

int  init_structure(t_data *data)
{
  int status;

  data->in_out_fd[0] = STDIN_FILENO;
  data->in_out_fd[1] = STDOUT_FILENO;
  if (data->input.size || data->output.size)
  {
    status = handle_redirection(data);
    if (status == -1 || status == 1)
      return (status); // -1 -> crash exit term : 1 -> back to prompt
  }
  status = clean_struct(data);
  if (status == -1 || status == 1)
    return (status); // -1 -> crash exit term : 1 -> back to prompt
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
