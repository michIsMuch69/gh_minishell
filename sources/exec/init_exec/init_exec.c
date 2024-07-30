/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 13:15:17 by florian           #+#    #+#             */
/*   Updated: 2024/07/30 14:48:35 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	init_structure(t_data *data)
{
	int	status;

	data->in_out_fd[0] = STDIN_FILENO;
	data->in_out_fd[1] = STDOUT_FILENO;
	if (data->input.size || data->output.size)
	{
		status = handle_redirection(data);
		if (status)
			return (status);
	}
	status = token_cleaner(data);
	if (status == -1 || status == 1)
		return (status);
	return (0);
}

static int	is_executable_path(t_data *data)
{
	if (!data->args.tab)
		return (0);
	if (!ft_strlen(data->args.tab[0]))
		return (0);
	if (data->args.tab[0][0] == '/' || data->args.tab[0][0] == '.')
	{
		data->cmd_path = ft_strdup(data->args.tab[0]);
		if (!data->cmd_path)
			return (-1);
		return (1);
	}
	return (0);
}

static int	get_cmd_path(t_data *data)
{
	char	*directory;
	int		ret_value;

	if (!data->args.tab)
		return (1);
	if (!data->args.tab[0])
		return (ft_putstr_fd("invalid cmd\n", 2), 127);
	ret_value = is_executable_path(data);
	if (ret_value == -1)
		return (-1);
	if (ret_value == 1)
		return (0);
	directory = NULL;
	ret_value = check_all_dirs(data, &directory);
	if (ret_value)
		return (ret_value);
	if (!directory)
		return (ft_putstr_fd("cmd not found\n", 2), 127);
	data->cmd_path = ft_concat_path(directory, data->args.tab[0]);
	free(directory);
	if (!data->cmd_path)
		return (-1);
	return (0);
}

int	init_exec(t_data *data, int tab_size)
{
	int	i;
	int	exit_stat;

	if (heredoc_management(data, tab_size) == -1)
		return (1);
	i = 0;
	while (i < tab_size)
	{
		data[i].tab_size = tab_size;
		if (expand_management(&(data[i]), data[0].env.tab, data[0].exit_status))
			return (1);
		if (init_structure(&(data[i])))
			return (1);
		if (!is_builtin(&data[i]))
		{
			exit_stat = get_cmd_path(&(data[i]));
			if (exit_stat)
				return (exit_stat);
		}
		i++;
	}
	return (0);
}
