/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:10:11 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/09 11:07:44 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expand.h>

static int	find_end(char *str, int start)
{
	while (str[start] && str[start] != '"' && str[start] != '\'' && \
		str[start] != '<' && str[start] != '>' && str[start] != '$' && \
		str[start] != 9 && str[start] != 32)
		start++;
	return (start);
}

static char	*extract_word(char *str, int start)
{
	char	*tmp;
	int		end;
	int		i;

	i = 0;
	end = find_end(str, start + 1);
	tmp = ft_calloc((end - start) + 1, sizeof(char));
	if (!tmp)
		return (ft_perror("error-> alloc extract word\n"), NULL);
	while (start < end)
	{
		tmp[i] = str[start];
		i++;
		start++;
	}
	return (tmp);
}

int	change_value(char **token, char **envp)
{
	int		i;
	int		ret_value;
	char	*word;
	char	*var_content;

	i = include_char(token[0], '$', 0);
	if (i == -1)
		return (0);
	word = extract_word(token[0], ++i);
	if (!word)
		return (-1);
	ret_value = ft_getenv(word, envp, &var_content);
	if (ret_value == 1)
		return (free(word), 1);
	else if (ret_value == -1)
		return (free(word), -1);
	free(word);
	return (join_str(token, i - 1, find_end(*token, i), var_content));
}

int	cut_str(char **token, int start, int end)
{
	char	*tmp;
	int		i;

	i = -1;
	start = include_char(token[0], '$', 0);
	if (start == -1)
		return (0);
	end = find_end(token[0], start + 1);
	tmp = ft_calloc((ft_strlen(token[0]) - (end - start) + 1), sizeof(char));
	if (!tmp)
		return (ft_perror("error-> alloc cut_str\n"), -1);
	while (++i < start)
		tmp[i] = token[0][i];
	while (token[0][end])
	{
		tmp[i] = token[0][end];
		i++;
		end++;
	}
	free(token[0]);
	token[0] = tmp;
	if (token[0][1])
		return (0);
	return (1);
}

int	count_sign(char *str, char sign)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == sign)
		i++;
	return (i);
}
