/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 12:34:26 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:58:35 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*temp;

	if (!lst || !*f || !del)
		return (NULL);
	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = f(lst->content);
	temp = new;
	while (lst->next)
	{
		lst = lst->next;
		temp->next = malloc(sizeof(t_list));
		if (!temp->next)
			return (ft_lstclear(&new, del), NULL);
		temp = temp->next;
		temp->content = f(lst->content);
	}
	temp->next = NULL;
	return (new);
}
