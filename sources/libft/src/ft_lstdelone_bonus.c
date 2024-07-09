/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:34:58 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:58:35 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	t_list	*temp;

	if (!lst || !(*del))
		return ;
	temp = lst;
	(*del)(lst->content);
	lst = lst->next;
	free(temp);
}
