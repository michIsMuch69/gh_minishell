/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 10:56:02 by fberthou          #+#    #+#             */
/*   Updated: 2024/06/23 16:00:03 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ###### INCLUDES ######

#include <stdlib.h>
#include "libft.h"
#include "struct.h"

// ###### INCLUDES ######

int	ft_perror(char *err_message)
{
	return (write(2, err_message, ft_strlen(err_message)));
}

void	free_tab(t_table *tab, int start)
{
	int	i;

	i = start;
	if (!tab->tab)
		return ;
	while (i < tab->size)
  {
    if (tab->tab[i])
    {
		  free(tab->tab[i]);
      tab->tab[i] = NULL;
    }
    i++;
  }
	free(tab->tab);
	tab->tab = NULL;
}

void  destroy_heredocs(t_table *heredoc)
{
  int i;

  if (!heredoc->size)
    return;
  i = 0;
  while (i < heredoc->size)
    unlink(heredoc->tab[i++]);
  free_tab(heredoc, 0);
  heredoc->tab = NULL;
}

void	free_struct(t_data *struc, int tab_size)
{
	int	i;

	i = 0;
	while (i < tab_size)
	{
		if (struc[i].cmd_path)
		{
			free(struc[i].cmd_path);
			struc[i].cmd_path = NULL;
		}
		free_tab(&(struc[i].args), 0);
		struc[i].args.tab = NULL;
		free_tab(&(struc[i].input), 0);
		struc[i].input.tab = NULL;
		free_tab(&(struc[i].output), 0);
		struc[i].output.tab = NULL;
    destroy_heredocs(&(struc[i].docs_files));
		i++;
	}
	free_tab(&(struc->env), 0);
	free(struc);
}
