/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:10:46 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/11 10:36:21 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <tokenizer.h>

int	include_char(char *token, char c, int start);

static int	build_token(char *prompt, int start, int end, t_table *token)
{
	int	i;
	int	tok_size;

	i = 0;
	tok_size = end - start;
	token->tab[token->size] = ft_calloc((tok_size + 1), sizeof(char));
	if (!token->tab[token->size])
		return (ft_perror("error -> token memory allocation\n"), -1);
	while (i < tok_size)
	{
		token->tab[token->size][i] = prompt[start];
		i++;
		start++;
	}
	token->size++;
	return (1);
}

static int	extract_token(char *prompt, int *i, char c, t_table *token)
{
	const int	start = *i;

	if (c == '\'' || c == '"')
	{
		while (prompt[++(*i)])
		{
			if (prompt[*i] == 9 || prompt[*i] == 32)
				return (build_token(prompt, start, ++(*i), token));
			else if (prompt[*i] == c && (prompt[(*i) + 1] == 9 || \
					prompt[(*i) + 1] == 32))
				return (build_token(prompt, start, ++(*i), token));
		}
		return (build_token(prompt, start, *i, token));
	}
	else if (c == '<' || c == '>')
		return (build_token(prompt, start, find_end(prompt, c, i), token));
	else
		return (build_token(prompt, start, find_end(prompt, ' ', i), token));
	return (0);
}

static int	split_tokens(char *prompt, int *i, t_table *token)
{
	while (prompt[*i] == ' ' || prompt[*i] == '\t')
		(*i)++;
	while (prompt[*i])
	{
		if (prompt[*i] == '\'')
			return (extract_token(prompt, i, '\'', token));
		else if (prompt[*i] == '"')
			return (extract_token(prompt, i, '"', token));
		else if (prompt[*i] == '<' || prompt[*i] == '>')
			return (extract_token(prompt, i, prompt[*i], token));
		else
			return (extract_token(prompt, i, 0, token));
	}
	return (0);
}

static int	init_tokens(t_table *token, char *prompt)
{
	if (!prompt)
	{
		token->tab = NULL;
		return (-1);
	}
	token->size = 0;
	token->tab = ft_calloc(1, sizeof(char *));
	if (!token->tab)
		return (ft_perror("error -> token memory allocation\n"), -1);
	return (0);
}

t_table	tokenizer(char *prompt)
{
	int		i;
	t_table	token;
	char	**tmp;
	int		ret_value;

	i = init_tokens(&token, prompt);
	if (i == -1)
		return (token);
	while (prompt[i])
	{
		ret_value = split_tokens(prompt, &i, &token);
		if (ret_value == -1)
			return (free_tab(&token, 0), token.tab = NULL, token);
		if (ret_value == 0)
			break ;
		tmp = ft_realloc(token.tab, ((token.size + 1) * sizeof(char *)), \
									((token.size) * sizeof(char *)));
		if (!tmp)
			return (free_tab(&token, 0), token.tab = NULL, \
					ft_perror("error -> tab_arg memory allocation\n"), token);
		token.tab = tmp;
	}
	return (token);
}
