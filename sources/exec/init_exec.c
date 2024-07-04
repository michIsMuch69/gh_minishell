/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 13:15:17 by florian           #+#    #+#             */
/*   Updated: 2024/07/02 17:26:29 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int  init_structure(t_data *data)
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
  status = token_cleaner(data);
  if (status == -1 || status == 1)
    return (status); // -1 -> crash exit term : 1 -> back to prompt
  return (0);
}

static int get_cmd_path(t_data *data)
{
	char	*directory;
  int   ret_value;

  if (!data->args.tab)
    return (1);
  directory = NULL;
  ret_value = check_all_dirs(data, &directory); // fill directory with the path where data->arg.tab[0] is located
  if (ret_value)
    return (ret_value);
  if (!directory)
    return (1);
  data->cmd_path = ft_concat_path(directory, data->args.tab[0]); // concate directory with args.tab[0] for the complete path of the commande (ex : /usr/bin/cat)
  free(directory);
  if (!data->cmd_path)
    return (-1);
  return (0);
}
/* take a pointer on an instance of data
    * expand variables
    * init structure
        - check files redirections & get in_out_file_fd[2]
        - clean args tokens
    * get_cmd_path()
        - get the cmd path... #ofcourse
*/
int init_exec(t_data *data)
{
    int ret_value;

    if (!data)
        return (ft_perror("no cmd\n"), 1);
    ret_value = expand_management(data, data->env.tab);
    if (ret_value)
        return (ret_value);
    ret_value = init_structure(data);
    if (ret_value)
        return (ret_value);
    ret_value = get_cmd_path(data);
    if (ret_value)
        return (ret_value);
    return (0);
}
