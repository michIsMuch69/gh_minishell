/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:10:11 by fberthou          #+#    #+#             */
/*   Updated: 2024/08/01 06:50:29 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expand.h>

char	*extract_word(char *str, int start, int end)
{
	char	*tmp;
	int		i;

	i = 0;
	// printf("token = %s, start == %d, end = %d\n", str, start, end);
	tmp = ft_calloc((end - start) + 1, sizeof(char));
	if (!tmp)
		return (ft_perror("error-> alloc extract word\n"), NULL);
	while (start < end)
	{
		tmp[i] = str[start];
		i++;
		start++;
	}
	return (tmp);
}



int	count_sign(char *str, char sign)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == sign)
		i++;
	return (i);
}
