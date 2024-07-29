/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_treatment.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:51:51 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/29 10:41:42 by jedusser         ###   ########.fr       */
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

char	*pre_treatment(char *prompt, int i)
{
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
