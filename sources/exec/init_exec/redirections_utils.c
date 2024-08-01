/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:14:53 by jedusser          #+#    #+#             */
/*   Updated: 2024/08/01 07:07:50 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	check_all(t_table infile)
{
	int		i;
	int		nb_arrow;
	char	*input_file;

	i = 0;
	while (i < infile.size)
	{
		nb_arrow = arrow_count(infile.tab[i], '<');
		if (nb_arrow > 2)
			return (ft_perror("unexpected \'<\' token\n"), -2);
		if (nb_arrow == 1)
		{
			input_file = skip_redir_symbol(infile.tab[i], 0);
			if (!input_file)
				return (ft_perror("error -> alloc input_file\n"), -1);
			if (access(input_file, F_OK | R_OK) == -1)
				return (perror(input_file), free(input_file), -2);
			free(input_file);
		}
		i++;
	}
	return (0);
}

int	create_all(t_table outfile)
{
	char	*output_file;
	int		output_fd;
	int		i;

	i = -1;
	while (++i < outfile.size)
	{
		output_file = skip_redir_symbol(outfile.tab[i], 1);
		if (!output_file)
			return (-1);
		if (arrow_count(outfile.tab[i], '>') > 2)
			return (free(output_file), ft_perror("unexpected \'>\' token\n"),
				-2);
		if (access(output_file, F_OK) == -1)
		{
			output_fd = open(output_file, O_CREAT, 0644);
			if (output_fd == -1)
				return (perror("output open fail"), free(output_file), -1);
			if (close(output_fd) == -1)
				return (perror("close output fd fail"), -1);
		}
		if (access(output_file, W_OK) == -1)
			return (perror(output_file), free(output_file), -2);
		free(output_file);
	}
	return (0);
}

int	arrow_count(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] == c)
		i++;
	return (i);
}

char	*clean_delimiter(char *delimiter)
{
	char	*new;
	int		count;
	int		i;

	i = -1;
	count = 0;
	if (!delimiter)
		return (NULL);
	while (delimiter[++i])
		if (delimiter[i] == '\'' || delimiter[i] == '\"')
			count++;
	if (!count)
		return (delimiter);
	new = ft_calloc(ft_strlen(delimiter) - count + 1, sizeof(char));
	if (!new)
		return (ft_perror("error -> alloc delimiter"), free(delimiter), NULL);
	i = -1;
	count = -1;
	while (delimiter[++i])
	{
		if (delimiter[i] != '\'' && delimiter[i] != '\"')
			new[++count] = delimiter[i];
	}
	return (free(delimiter), new);
}

char	*skip_redir_symbol(char *token_file, bool direction)
{
	char	*file;
	int		tok_nb;
	int		size;

	if (!token_file)
		return (NULL);
	if (direction == 1)
		tok_nb = arrow_count(token_file, '>');
	else
		tok_nb = arrow_count(token_file, '<');
	size = (ft_strlen(token_file) - tok_nb + 1);
	file = ft_calloc(size, sizeof(char));
	if (!file)
		return (ft_perror("error -> alloc skip_redir_sym\n"), NULL);
	file = ft_strcpy(file, &token_file[tok_nb]);
	if (!file[0])
		return (free(file), ft_perror("invalid delimiter\n"), NULL);
	return (file);
}
