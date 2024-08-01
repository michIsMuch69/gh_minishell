/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:47:54 by jedusser          #+#    #+#             */
/*   Updated: 2024/08/01 07:07:28 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	define_input_fd(t_table infile, t_table heredocs)
{
	char	*inputfile;
	int		input_fd;

	if (arrow_count(infile.tab[(infile.size - 1)], '<') == 2)
	{
		input_fd = open(heredocs.tab[heredocs.size - 1], O_RDONLY | __O_CLOEXEC);
		if (input_fd == -1)
			perror("open heredoc file");
		return (input_fd);
	}
	inputfile = skip_redir_symbol(infile.tab[(infile.size - 1)], 0);
	if (!inputfile)
		return (ft_perror("error-> alloc inputfile\n"), -1);
	input_fd = open(inputfile, O_RDONLY | __O_CLOEXEC);
	if (input_fd == -1)
		perror("open infile ");
	free(inputfile);
	return (input_fd);
}

static int	redir_input(t_data *data)
{
	int	ret_value;

	ret_value = check_all(data->input);
	if (ret_value == -1 || ret_value == -2)
		return (ret_value);
	return (define_input_fd(data->input, data->docs_files));
}

static int	redir_output(t_data *data)
{
	int		ret_value;
	int		output_fd;
	char	*output_file;

	ret_value = create_all(data->output);
	if (ret_value)
		return (ret_value);
	output_file = skip_redir_symbol(data->output.tab[(data->output.size - 1)], 1);
	if (!output_file)
		return (-1);
	if (arrow_count(data->output.tab[(data->output.size - 1)], '>') == 1)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC | __O_CLOEXEC, 0644);
	else if (arrow_count(data->output.tab[(data->output.size - 1)], '>') == 2)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND | __O_CLOEXEC, 0644);
	else
		return (ft_perror("unexpected \'>\' token\n"), -2);
	if (output_fd == -1)
		perror("open outfile fail");
	return (free(output_file), output_fd);
}

int	handle_redirection(t_data *data)
{
	if (data->output.size)
	{
		data->in_out_fd[1] = redir_output(data);
		if (data->in_out_fd[1] == -1)
			return (-1);
	}
	if (data->input.size)
	{
		data->in_out_fd[0] = redir_input(data);
		if (data->in_out_fd[0] == -1)
			return (-1);
	}
	if (data->in_out_fd[0] == -2 || data->in_out_fd[1] == -2)
		return (1);
	return (0);
}
