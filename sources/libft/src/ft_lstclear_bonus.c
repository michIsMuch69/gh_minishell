/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:33:32 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:58:35 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;
	t_list	*p;

	if (!lst || !del)
		return ;
	p = *lst;
	while (p)
	{
		(*del)(p->content);
		temp = p->next;
		free(p);
		p = temp;
	}
	*lst = NULL;
}
