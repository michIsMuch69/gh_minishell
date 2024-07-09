/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 16:18:35 by fberthou          #+#    #+#             */
/*   Updated: 2024/01/05 11:51:10 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	check_content(char *tmp)
{
	int	i;

	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\n')
			return (i + 1);
		i++;
	}
	return (-1);
}

void	move_buffer(char *buff, int index)
{
	size_t	i;

	if (!buff || index <= 0)
		return ;
	i = 0;
	while (((char *)buff)[index])
	{
		((char *)buff)[i] = ((char *)buff)[index];
		i++;
		index++;
	}
	clear_buffer(buff, i);
}

char	*cpy_finalstr(char *tmp, int nb)
{
	int		i;
	char	*dst;

	dst = ft_calloct(sizeof(char), (nb + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (tmp[i] && i < nb)
	{
		dst[i] = tmp[i];
		i++;
	}
	return (dst);
}

int	read_file(int fd, char *buffer, char **tab)
{
	int		nb;
	char	*tmp;

	nb = check_content(*tab);
	while (nb == -1)
	{
		clear_buffer(buffer, 0);
		nb = read(fd, buffer, BUFFER_SIZE);
		if (nb == -1)
			return (-1);
		if (nb == 0)
			return (lenstr(*tab));
		tmp = cpy_substr(*tab);
		free(*tab);
		if (!tmp)
			return (-1);
		*tab = concate_buffer(buffer, tmp);
		if (!*tab)
			return (-1);
		nb = check_content(*tab);
	}
	return (nb);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE] = "\0";
	char		*tmp;
	char		*final_str;
	int			nb;

	tmp = cpy_substr(buffer);
	if (!tmp)
		return (NULL);
	nb = read_file(fd, buffer, &tmp);
	if (!tmp)
		return (NULL);
	if (nb == -1 || tmp[0] == '\0')
		return (free(tmp), NULL);
	final_str = cpy_finalstr(tmp, nb);
	if (!final_str)
		return (free(tmp), NULL);
	move_buffer(buffer, (check_content(buffer)));
	return (free(tmp), final_str);
}
