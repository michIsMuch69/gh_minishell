/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_nd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:39:14 by fberthou          #+#    #+#             */
/*   Updated: 2024/06/17 11:03:52 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

// main/utils.c
int	ft_perror(char *err_message);

static char	*init_join_str(int size, int *i_str, int *i_tok)
{
	char	*tmp;

	*i_str = 0;
	*i_tok = 0;
	if (size > 0)
		tmp = ft_calloc(size, sizeof(char));
	else
		tmp = ft_calloc(1, sizeof(char));
	if (!tmp)
		return (ft_perror("error-> alloc join_str\n"), NULL);
	return (tmp);
}

int	join_str(char **token, int start, int end, char *var_content)
{
	char	*str;
	int		i_str;
	int		i_tok;

	str = init_join_str((ft_strlen(*token) - (end - start) + \
		ft_strlen(var_content) + 1), &i_str, &i_tok);
	if (!str)
		return (free(var_content), -1);
	while (i_str < start)
	{
		str[i_str] = token[0][i_str];
		i_str++;
	}
	while (var_content[i_tok])
	{
		str[i_str] = var_content[i_tok++];
		i_str++;
	}
	while (token[0][end])
	{
		str[i_str] = token[0][end++];
		i_str++;
	}
	return (free(*token), free(var_content), *token = str, 2);
}
