/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:47:54 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/22 19:00:30 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	define_input_fd(t_table infile)
{
  char  *inputfile;
	int		input_fd;
  int   ret_value;

  ret_value = check_all(infile);
  if (ret_value == -1)
    return (-1);
  if (ret_value == -2)
    return (-2);
  inputfile = skip_redir_symbol(infile.tab[(infile.size - 1)], 0);
  if (!inputfile)
    return (ft_perror("error-> alloc inputfile\n"), -1);
  input_fd = open(inputfile, O_RDONLY);
  if (input_fd == -1)
    perror(inputfile);
  free(inputfile);
	return (input_fd);
}

// **** create files and check permissions **** //
static int	define_output_fd(t_table outfile)
{
  int   ret_value;
	int		output_fd;
	char	*output_file;

	// create all files and check permissions
  ret_value = create_all(outfile);
  if (ret_value == -1) // crash
    return (-1);
  if (ret_value == -2) // back to prompt
    return (-2);
	// get the fd of the last file created
	output_file = skip_redir_symbol(outfile.tab[(outfile.size - 1)], 1);
  if (!output_file)
    return (-1); // crash
	if (arrow_count(outfile.tab[(outfile.size - 1)], '>') - 1 == 1)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (arrow_count(outfile.tab[(outfile.size - 1)], '>') - 1 == 2)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
  if (output_fd == -1)
    perror("open outfile fail");
	return (free(output_file), output_fd);
}

/*
  * play here_docs
	* check if files exist
  * dup2 last infile_fd
  * destroy heredoc tmp files
  * -1 if an error occured
  * else return infile_fd
*/

static int  redir_input(t_data *data)
{
	int		  input_fd;

  input_fd = heredoc_management(data->input, &(data->docs_files));
  if (input_fd == -1)
    return (-1);
  input_fd = define_input_fd(data->input);
  if (input_fd == -1)
    return (free_tab(&(data->docs_files), 0), -1);
  if (input_fd == -2)
    return (free_tab(&(data->docs_files), 0), -2);
  // if (dup2(input_fd, STDIN_FILENO) == -1)
  //   return (perror("Failed to redirec input "), \
  //           close(input_fd), -1); // crash
	return (input_fd);
}

/*
	* create all output files
	* dup2 last outfile_fd
	* return -1 if an error occured
  * else return outfile_fd
*/
static int	redir_output(t_data *data)
{
	int		output_fd;

  output_fd = define_output_fd(data->output);
  if (output_fd == -1)
    return (-1); // crash
  if (output_fd == -2)
    return (-2); // back to prompt
  // if (dup2(output_fd, STDOUT_FILENO) == -1)
  //   return (perror("Failed to redirec output "), \
  //           close(output_fd), -1); // crash
  return (output_fd);
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
