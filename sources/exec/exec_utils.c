/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:59:06 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/30 07:42:56 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	wait_all(t_data *data, int tab_size, int pid, int **fd)
{
	int	i;

	i = 0;
	if (waitpid(pid, &(data[0].exit_status), 0) == -1)
	{
		free_pipes(fd, data[0].tab_size - 1);
		return (perror("wait_all waitpid() "), -1);
	}
	tab_size--;
	if (WIFEXITED(data[0].exit_status))
		data[0].exit_status = WEXITSTATUS(data[0].exit_status);
	else if (WIFSIGNALED(data[0].exit_status))
		data[0].exit_status = WTERMSIG(data[0].exit_status);
	else
		data[0].exit_status = -1;
	while (i < tab_size)
	{
		if (wait(NULL) == -1)
			return (perror("wait_all wait() "), -1);
		i++;
	}
	free_pipes(fd, data[0].tab_size - 1);
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
