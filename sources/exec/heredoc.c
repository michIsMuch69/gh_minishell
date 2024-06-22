/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:50:56 by florian           #+#    #+#             */
/*   Updated: 2024/06/22 15:19:00 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
  * INVALID HEADER
*/

#include <exec.h>

static int heredoc_loop(int fd2, char *token)
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

static int new_tmp_file(char **buffer)
{
  int   fd;
  int   fd2;

  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1)
    return (perror("open urandom "), -1);
  if (read(fd, *buffer, 12) == -1)
    return (close(fd), perror("read urandom "), -1);
  if (close(fd) == -1)
    return (perror("close urandom fd "), -1);
  fd2 = open(*buffer, O_CREAT | O_WRONLY | O_TRUNC); // have to test with O_TRUNC
  if (fd2 == -1)
    return (perror("open heredoc file "), -1);
  return (fd2);
}
/*
  * have to play heredocs
  *
*/

static int init(t_table *docs_lst, int docsize, int totalsize)
{
  if (!docsize)
  {
    docs_lst->tab = ft_calloc(totalsize, sizeof(char *));
    if (!docs_lst->tab)
      return (ft_perror("error -> alloc heredoc\n"), -1);
  }
  return (0);
}

int	heredoc_management(t_table infile, t_table *docs_lst)
{
  int   i;
	int		fd2;

  i = -1;
  docs_lst->size = 0;
  while (++i < infile.size)
  {
    if (arrow_count(infile.tab[i], '<') - 1 == 2)
    {
      if (init(docs_lst, docs_lst->size, infile.size) == -1)
        return (-1); // crash -> malloc error
      fd2 = new_tmp_file(&(docs_lst->tab[docs_lst->size])); // return the fd of the tmp file && fill buffer of tmp filename
      if (fd2 == -1)
        return (docs_lst->size++, free_tab(docs_lst, 0), -1);
      if (heredoc_loop(fd2, infile.tab[i]) == -1) // fill heredoc + close fd2
        return (docs_lst->size++, free_tab(docs_lst, 0), -1); // fd2 deja ferme
      docs_lst->size++;
    }
  }
	return (0);
}
