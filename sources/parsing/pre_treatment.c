/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_treatment.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:51:51 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/31 11:39:20 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pre_treatment.h>

static char	*quote_treatment(char *prompt, int *i_prompt, char c)
{
	char	*tmp;
	int		i_tmp;
	int		f_index;

	tmp = init_treatment(prompt, &i_tmp, *i_prompt);
	if (!tmp)
		return (NULL);
	tmp[i_tmp++] = 32;
	tmp[i_tmp] = prompt[(*i_prompt)];
	i_tmp++;
	while (prompt[++(*i_prompt)])
	{
		if (prompt[*i_prompt] == c && (prompt[(*i_prompt) + 1] == 9 || prompt[(*i_prompt) + 1] == 32))
			break ;
		tmp[i_tmp++] = prompt[*i_prompt];
	}
	tmp[i_tmp++] = prompt[*i_prompt];
	tmp[i_tmp++] = 32;
	f_index = i_tmp - 2;
	while (prompt[*i_prompt] && prompt[++(*i_prompt)])
		tmp[i_tmp++] = prompt[*i_prompt];
	(*i_prompt) = f_index;
	clear_buff(tmp, i_tmp, (ft_strlen(prompt) + 3));
	return (free(prompt), tmp);
}

static char	*pipe_treatment(char *prompt, int *i_prompt)
{
	char	*tmp;
	int		i_tmp;
	int		f_index;

	tmp = init_treatment(prompt, &i_tmp, *i_prompt);
	if (!tmp)
		return (NULL);
	tmp[i_tmp++] = 32;
	tmp[i_tmp++] = '|';
	tmp[i_tmp++] = 32;
	f_index = i_tmp;
	(*i_prompt)++;
	skip_spaces(prompt, i_prompt);
	while (prompt[(*i_prompt)])
	{
		tmp[i_tmp] = prompt[(*i_prompt)++];
		i_tmp++;
	}
	*i_prompt = f_index;
	clear_buff(tmp, i_tmp, (ft_strlen(prompt) + 3));
	return (free(prompt), tmp);
}

static char	*redir_treatment(char *prompt, int *i_prompt, char c)
{
	char	*tmp;
	int		i_tmp;

	tmp = init_treatment(prompt, &i_tmp, *i_prompt);
	if (!tmp)
		return (NULL);
	tmp[i_tmp++] = 32;
	while (prompt[*i_prompt] && prompt[*i_prompt] == c)
	{
		tmp[i_tmp] = prompt[*i_prompt];
		(*i_prompt)++;
		i_tmp++;
	}
	*i_prompt = filter(i_prompt, i_tmp, prompt, tmp);
	return (free(prompt), tmp);
}

static char	*check_condition(char *prompt, int *i)
{
	if (prompt[*i] == '&')
		return (ft_perror("\'&\' token not supported\n"), free(prompt), NULL);
	else if (prompt[*i] == '|')
	{
		prompt = pipe_treatment(prompt, i);
		if (!prompt)
			return (NULL);
	}
	else if (prompt[*i] == '"' || prompt[*i] == '\'')
	{
		if (*i == 0 || (prompt[*i - 1] == 9 || prompt[*i - 1] == 32))
			prompt = quote_treatment(prompt, i, prompt[*i]);
		else
			(*i)++;
		if (!prompt)
			return (NULL);
	}
	else if (prompt[*i] == '<' || prompt[*i] == '>')
	{
		prompt = redir_treatment(prompt, i, prompt[*i]);
		if (!prompt)
			return (NULL);
	}
	return (prompt);
}

char	*cut_prompt(char *prompt, int i)
{
	char	*new;
	int		y;

	y = -1;
	new = ft_calloc(ft_strlen(prompt) - 1, sizeof(char));
	if (!new)
		return (free(prompt), NULL);
	while (++y < i)
		new[y] = prompt[y];
	i += 2;
	while (prompt[i])
	{
		new[y] = prompt[i];
		y++;
		i++;
	}
	free(prompt);
	return (new);
}

char	*delete_unused_char(char *prompt)
{
	int	i;

	i = 0;
	while (prompt[i])
	{
		if (prompt[i] == '\'' || prompt[i] == '\"')
		{
			if (prompt[i + 1] == prompt[i])
			{
				prompt = cut_prompt(prompt, i);
				if (!prompt)
					return (NULL);
				i = 0;
			}
		}
		i++;
	}
	return (prompt);
}

char	*pre_treatment(char *prompt, int i)
{
	prompt = delete_unused_char(prompt);
	if (!prompt)
		return (NULL);
	while (prompt[i])
	{
		prompt = check_condition(prompt, &i);
		if (prompt && prompt[i])
			i++;
		else
			break ;
	}
	return (prompt);
}
