/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:50:56 by florian           #+#    #+#             */
/*   Updated: 2024/06/23 16:25:50 by florian          ###   ########.fr       */
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
      if (ft_strncmp(prompt, delimiter, ft_strlen(prompt)) == 0)
        break ;
      if (ft_putstr_fd(prompt, fd2) == -1 || \
          ft_putstr_fd("\n", fd2) == -1)
        return (perror("putsrt_fd heredoc_loop"), close(fd2), \
                free(prompt), free(delimiter), -1);
      free(prompt);
    }
  }
  free(prompt);
  free(delimiter);
  if (close(fd2) == -1)
    return (perror("close heredoc_loop"), -1);
  return (0);
}

static int new_tmp_file(t_table *heredoc, int i)
{
  int   fd;
  int   fd2;
  char  tmp[12];

  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1)
    return (perror("open urandom "), -1);
  if (read(fd, tmp, sizeof(tmp)) == -1)
    return (close(fd), perror("read urandom "), -1);
  //printf("urandom name file == %s\n", tmp);
  if (close(fd) == -1)
    return (perror("close urandom fd "), -2);
  heredoc->tab[i] = ft_strdup(tmp);
  if (!heredoc->tab[i])
    return (ft_perror("error-> strdup heredoc\n"), -1);
  fd2 = open(heredoc->tab[i], O_CREAT | O_RDWR | O_TRUNC, 0644); // have to test with O_TRUNC
  if (fd2 == -1)
    return (perror("open heredoc file "), -2);
  return (fd2);
}

static int  big_loop(t_table infile, t_table *heredoc)
{
  int i;
  int fd2;

  i = 0;
  heredoc->size = 0;
  while (i < infile.size)
  {
    if (arrow_count(infile.tab[i], '<') == 2)
    {
      fd2 = new_tmp_file(heredoc, heredoc->size); // return the fd of the tmp file && fill buffer of tmp filename
      if (fd2 == -1)
        return (-1);
      if (fd2 == -2)
      {
        heredoc->size++;
        return (-1);
      }
      if (heredoc_loop(fd2, infile.tab[i]) == -1) // fill heredoc + close fd2
        return (heredoc->size++, -1); // fd2 deja ferme
      heredoc->size++;
    }
    i++;
  }
	return (heredoc->size);
}

static int init_heredocs(t_table *docs_lst, t_table infile)
{
  int i;

  i = 0;
  docs_lst->size = 0;
  while (i < infile.size)
  {
    if (arrow_count(infile.tab[i], '<') == 2)
      docs_lst->size++;
    i++;
  }
  if (docs_lst->size)
  {
    docs_lst->tab = ft_calloc(docs_lst->size, sizeof(char *));
    if (!docs_lst->tab)
    {
      docs_lst->size = -1;
      return (-1);
    }
    return (docs_lst->size);
  }
  return (0);
}

int	heredoc_management(t_table infile, t_table *docs_lst)
{
  int ret_value;

  ret_value = init_heredocs(docs_lst, infile);
  if (ret_value == -1)
    return (-1); // crash -> malloc error
  if (!ret_value)
    return (0); // no heredocs
  return (big_loop(infile, docs_lst));
}
