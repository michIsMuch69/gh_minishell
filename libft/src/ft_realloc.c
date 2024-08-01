/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 22:01:42 by fberthou          #+#    #+#             */
/*   Updated: 2024/08/01 07:02:19 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

void	*ft_realloc(void *pointer, size_t memory_size, size_t src_size)
{
	void	*tmp;

	tmp = ft_calloc(memory_size, sizeof(char));
	if (!tmp)
		return (NULL);
	tmp = ft_memcpy(tmp, (const void *)pointer, src_size);
	if (pointer)
		free(pointer);
	return (tmp);
}
