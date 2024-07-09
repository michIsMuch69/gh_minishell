/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:04:48 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/09 11:41:54 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// main/utils.c
int	ft_perror(char *err_message);

int	include_char(char *token, char c, int start)
{
	if (!token || start < 0)
		return (-1);
	while (token[start])
	{
		if (token[start] == c)
			return (start);
		start++;
	}
	return (-1);
}

char	*init_str(char *token, char c)
{
	int		i;
	int		size;
	char	*final;

	i = 0;
	size = 0;
	if (!token)
		return (NULL);
	while (token[i])
	{
		if (token[i++] != c)
			size++;
	}
	final = ft_calloc(sizeof(char), (size + 1));
	if (!final)
		ft_perror("error -> malloc final_build\n");
	return (final);
}

char	*final_build(char *token, char c)
{
	int		i;
	int		size;
	char	*final;

	i = 0;
	size = 0;
	final = init_str(token, c);
	if (!final)
		return (NULL);
	while (token[i])
	{
		if (token[i] != c)
		{
			while (token[i] && token[i] != c)
			{
				final[size] = token[i];
				size++;
				i++;
			}
		}
		else
			i++;
	}
	return (final);
}

int	find_end(char *prompt, char c, int *i)
{
	if (c == '<' || c == '>' || c == '$')
	{
		while (prompt[*i] && prompt[*i] != 32 && prompt[*i] != 9)
			(*i)++;
		return (*i);
	}
	while (prompt[++(*i)])
	{
		if (prompt[*i] == c || prompt[*i] == '<' || \
			prompt[*i] == '>' || prompt[*i] == '|')
			return (*i);
	}
	return (*i);
}
