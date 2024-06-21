/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:14:53 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/20 18:29:01 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int  create_all(t_table outfile)
{
  char	*output_file;
  int   output_fd;
  int   i;

  i = -1;
  while (++i < outfile.size)
	{
		output_file = skip_redir_symbol(outfile.tab[i], 1);
    if (!output_file)
      return (-1); // crash
		if (arrow_count(outfile.tab[i], '>') - 1 > 2)
			return (free(output_file), ft_perror("unexpected \'>\' token\n"), -2);
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
	while (str[i++] == c)
		;
	return (i);
}

/*
	* direction 1 == output; 0 == input;
*/

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
	size = (ft_strlen(token_file) - tok_nb);
	file = ft_calloc(size, sizeof(char));
	if (!file)
		return (ft_perror("error -> alloc skip_redir_sym\n"), NULL);
	file = ft_strcpy(file, &token_file[tok_nb - 1]);
	//ft_printf("file == %s\n", file);
	return (file);
}
