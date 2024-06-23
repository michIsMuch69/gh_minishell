/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:47:54 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/23 15:52:37 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/*
  * check if files exist
  * return inputfile fd
  * or -1 -> error crash
  * or -2 -> error back to prompt
*/
static int	define_input_fd(t_table infile, t_table heredocs)
{
  char  *inputfile;
  char  *tmp;
	int		input_fd;

  if (arrow_count(infile.tab[(infile.size - 1)], '<') == 2)
  {
    tmp = ft_strdup(heredocs.tab[heredocs.size - 1]);
    if (!tmp)
      return (-1);
    inputfile = skip_redir_symbol(tmp, 0);
    free(tmp);
  }
  else
    inputfile = skip_redir_symbol(infile.tab[(infile.size - 1)], 0);
  if (!inputfile)
    return (ft_perror("error-> alloc inputfile\n"), -1);
  input_fd = open(inputfile, O_RDONLY);
  if (input_fd == -1)
    perror(inputfile);
  free(inputfile);
	return (input_fd);
}

/*
  * play here_docs
  * check_all
    - check if nb arrow is correct
    - check file access
    - return -1 crash
    - return -2 back to prompt
    - return 0 if all ok
  * return define_input_fd
*/
static int  redir_input(t_data *data)
{
	int ret_value;

  ret_value = heredoc_management(data->input, &(data->docs_files));
  if (ret_value == -1)
    return (-1);
  ret_value = check_all(data->input);
  if (ret_value == -1)
    return (-1);
  if (ret_value == -2)
    return (-2);
  return (define_input_fd(data->input, data->docs_files));
}

/*
	* create files and check permissions
  * get fd of the last outfile
	* return -1 if an error occured
  * else return outfile_fd
*/
static int	redir_output(t_data *data)
{
  int   ret_value;
	int		output_fd;
	char	*output_file;

	// create all files and check permissions
  ret_value = create_all(data->output);
  if (ret_value == -1) // crash
    return (-1);
  if (ret_value == -2) // back to prompt
    return (-2);
	// get the fd of the last file created
	output_file = skip_redir_symbol(data->output.tab[(data->output.size - 1)], 1);
  if (!output_file)
    return (-1); // crash
	if (arrow_count(data->output.tab[(data->output.size - 1)], '>') == 1)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (arrow_count(data->output.tab[(data->output.size - 1)], '>') == 2)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
  if (output_fd == -1)
    perror("open outfile fail");
	return (free(output_file), output_fd);
}

int handle_redirection(int *fds, t_data *data)
{
  if (data->output.size)
  {
    fds[1] = redir_output(data);
    if (fds[1] == -1)
      return (close_free_fds(fds), -1); // crash
  }
  if (data->input.size)
  {
    fds[0] = redir_input(data);
    if (fds[0] == -1)
      return (close_free_fds(fds), -1); // crash
  }
  if (fds[0] == -2 || fds[1] == -2)
    return (close_free_fds(fds), 1); // Heredocs have been played -> back to prompt
  return (0);
}
