/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 16:14:13 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:58:35 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

void	ft_lstadd_back(t_list **lst, t_list *_new)
{
	t_list	*temp;

	if (!_new)
		return ;
	temp = ft_lstlast(*lst);
	if (!temp)
		*lst = _new;
	else
		temp->next = _new;
}
