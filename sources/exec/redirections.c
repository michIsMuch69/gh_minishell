/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:47:54 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/20 22:10:25 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int new_tmp_file(void)
{
  int   fd;
  int   fd2;
  char  *filename;

  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1)
    return (perror("open urandom"), -1);
  if (read(fd, filename, 12) == -1)
    return (close(fd), perror("read urandom"), -1);
  if (close(fd) == -1)
    return (perror("close urandom fd"), -1);
  fd2 = open(filename, O_CREAT | O_WRONLY); // have to test with O_TRUNC
  if (fd2 == -1)
    return (perror("open heredoc file"), -1);
  return (fd2);
}

int heredoc_loop(int fd2, char *token)
{
  char  *delimiter;
  char  *prompt;

  delimiter = skip_redir_symbol(token, 0);
  if (!delimiter)
    return (ft_perror("error -> alloc delimiter\n"), close(fd2), -1);
  while (1)
  {
    prompt = readline("> ");
    if (prompt)
    {
      // !! forbidden function
      if (strcmp(prompt, delimiter) == 0)
        break ;
      // !! forbidden function
      if (ft_putstr_fd(prompt, fd2) == -1 || \
          ft_putstr_fd("\n", fd2) == -1)
        return (perror("putsrt_fd heredoc_loop"), close(fd2), \
                free(prompt), -1);
      free(prompt);
    }
  }
  free(prompt);
  if (close(fd2) == -1)
    return (perror("close heredoc_loop"), -1);
  return (0);
}

static int	heredoc_management(t_table infile)
{
  int   i;
	int		fd2;
  int   i_doc;
	char	**docs_lst;

  i = -1;
  i_doc = 0;
  docs_lst = NULL;
  while (++i < infile.size)
  {
    if (arrow_count(infile.tab[i], '<') == 2)
    {
      if (!docs_lst)
      {
        docs_lst = ft_calloc(2, sizeof(char *));
        if (!docs_lst)
          return ()
      }
      fd2 = new_tmp_file(); // return the fd of the tmp file
      if (fd2 == -1)
        return (-1);
      if (heredoc_loop(fd2, infile.tab[i])) // fill heredoc + close fd2
        return (-1);
    }
  }
	return (fd2);
}

static int	define_input_fd(t_table infile)
{
	int		input_fd;
	char	*input_file;
  int   heredoc_fd;
  int   i;
  int   ret_value;


  i = 0;
  ret_value = heredoc_management(infile);
  if (ret_value == -1)
    return (-1);

	if (arrow_count(infile.tab[0], '<') - 1 == 1)
	{
		input_file = skip_redir_symbol(infile.tab[0], 0);
		if (access(input_file, F_OK | R_OK) == -1)
			return (perror(input_file), -1);
		input_fd = open(input_file, O_RDONLY);
	}
	if (arrow_count(infile.tab[0], '<') - 1 == 2)
	{
		delimiter = skip_redir_symbol(infile.tab[0], 0);
		input_fd = here_docs(delimiter);
		unlink("temp.txt");
	}
	return (input_fd);
}

/*
  * play here_docs
	* check if files exist
  * dup2 last infile fd
*/

int	redir_input(t_data *data, int prev_fd)
{
	int		input_fd;

	if (data->input.size)
	{
		input_fd = define_input_fd(data->input);
		if (input_fd == -1)
			return (perror("Failed to open input file"), -1);
		if (dup2(input_fd, STDIN_FILENO) == -1)
			return (perror("Failed to redirect "), close(input_fd), -1);
	}
	// else if (i > 0)
	// {
	// 	if (dup2(prev_fd, STDIN_FILENO) == -1)
	// 		return (perror("Failed to duplicate previous fd"), -1);
	// 	close(prev_fd);
	// }
	return (0);
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
    return (-1); // crash alloc in skip_redir_symbol
	if (arrow_count(outfile.tab[(outfile.size - 1)], '>') - 1 == 1)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (arrow_count(outfile.tab[(outfile.size - 1)], '>') - 1 == 2)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
  if (output_fd == -1)
    perror("open outfile fail");
	return (free(output_file), output_fd);
}
/*
	* create all files
	* get the fd of the last file in the list
	* return -1 if an error occured
*/

static int	redir_output(t_data *data)
{
	int		output_fd;

	if (data->output.size)
	{
		output_fd = define_output_fd(data->output);
		if (output_fd == -1)
			return (-1); // crash
    if (output_fd == -2)
      return (1); // back to prompt
		if (dup2(output_fd, STDOUT_FILENO) == -1)
			return (perror("Failed to redirect standard output"), \
              close(output_fd), -1); // crash
		close(output_fd);
	}
	// else if (i < tab_size - 1)
	// {
	// 	if (dup2(fds[1], STDOUT_FILENO) == -1)
	// 		return (perror("Failed to duplicate pipe fd for output"), -1);
	// 	close(fds[1]);
	// }
	return (0);
}

int handle_redirection(fds, prev_fd, data)
{
  int ret_value;

  ret_value = redir_output(data);
  if (ret_value == -1)
    return (-1);
  if (ret_value == 1)
    return (1);
  ret_value = redir_input(data, prev_fd);
  if (ret_value == -1)
    return (-1);
  if (ret_value == 1)
    return (1);
  return (0);
}
