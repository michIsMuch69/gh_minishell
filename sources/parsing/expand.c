/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:27:19 by fberthou          #+#    #+#             */
/*   Updated: 2024/08/01 07:11:01 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expand.h>

char		*extract_word(char *str, int start, int end);

static int	find_end_var(char *token, int i)
{
	while (token[i])
	{
		if (token[i] == 9 || token[i] == 32 || token[i] == '\"' || token[i] == '$' || token[i] == '\'' || token[i] == '/')
			return (i);
		i++;
	}
	return (i);
}

int	cut_str(char **token, int start, int end, char *var)
{
	char	*tmp;
	int		i;
	int		y;

	y = 0;
	i = -1;
	tmp = ft_calloc(ft_strlen(*token) + ft_strlen(var) + 1, 1);
	if (!tmp)
		return (ft_perror("error-> alloc cut_str\n"), -1);
	while (++i < start)
		tmp[i] = token[0][i];
	while (var && var[y])
	{
		tmp[i] = var[y++];
		i++;
	}
	while (token[0][end])
	{
		tmp[i] = token[0][end++];
		i++;
	}
	if (var)
		free(var);
	free(token[0]);
	token[0] = tmp;
	return (0);
}

int	change_value(char **token, int *i, char **envp, int last_exit)
{
	char	*word;
	char	*var_content;
	int		i_end;
	int		y;

	i_end = find_end_var(*token, *i + 1);
	word = extract_word(*token, *i + 1, i_end);
	if (!word)
		return (-1);
	if (ft_strncmp(word, "?", ft_strlen(word)) == 0)
		return (free(word), cut_str(token, *i, i_end, ft_itoa(last_exit)));
	if (ft_getenv(word, envp, &var_content) == -1)
		return (free(word), -1);
	free(word);
	if (!var_content)
		return (cut_str(token, *i, i_end, NULL));
	return (cut_str(token, *i, i_end, var_content));
}

static int	arg_management(t_table *arg, char **envp, int last_exit)
{
	int	flag;
	int	i_tab;
	int	i;

	i = -1;
	i_tab = -1;
	flag = 0;
	while (++i_tab < arg->size)
	{
		while (arg->tab[i_tab] && arg->tab[i_tab][++i])
		{
			if (arg->tab[i_tab][i] == '\'' && flag == 0)
				flag = 1;
			else if (arg->tab[i_tab][i] == '\'' && flag == 1)
				flag = 0;
			else if (arg->tab[i_tab][i] == '\"' && flag == 0)
				flag = -1;
			else if (arg->tab[i_tab][i] == '\"' && flag == -1)
				flag = 0;
			else if (arg->tab[i_tab][i] == '$' && flag <= 0)
				if (change_value(&arg->tab[i_tab], &i, envp, last_exit) == -1)
					return (-1);
		}
		i = -1;
		flag = 0;
	}
	return (0);
}

/*
	* args->tab ->
		// simple quotes -> no changement
		// double quotes	-> change value if present in env
							-> delete $NAME if not present in env

	* inputs :
		// if HEREDOC -> keep the litteral value
		// if $NAME not present in env -> ambiguous redirect -> clean tab
		// if $NAME present in env -> change the value

	* output :
		// if $NAME not present in env -> ambiguous redirect -> clean tab
		// if $NAME present in env -> change the value

	* heredoc file :
		-> change value if present in env
		-> delete $NAME if not present in env
*/
int	expand_management(t_data *data, char **envp, int last_exit)
{
	int	ret_value;

	ret_value = arg_management(&(data->input), envp, last_exit);
	if (ret_value == -1)
		return (-1);
	else if (ret_value == 1)
		return (1);
	ret_value = arg_management(&(data->output), envp, last_exit);
	if (ret_value == -1)
		return (-1);
	else if (ret_value == 1)
		return (1);
	return (arg_management(&(data->args), envp, last_exit));
}
