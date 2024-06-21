/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:06:47 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:58:36 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

static char	*ft_copy(char const *s1, char *dst, int left_index, int n)
{
	size_t	i;

	i = 0;
	while (s1[left_index] && left_index < n)
	{
		dst[i] = s1[left_index];
		i++;
		left_index++;
	}
	dst[i] = 0;
	return (dst);
}

static int	trim_left(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i] && set[j])
	{
		if (s1[i] == set[j])
			i++;
		else if (s1[i] != set[j])
		{
			j = 0;
			while (set[j] != s1[i] && set[j])
				j++;
		}
	}
	return (i);
}

static int	trim_right(char const *s1, char const *set)
{
	int	i;
	int	j;

	j = 0;
	i = ft_strlen(s1) - 1;
	while (i > 0 && set[j])
	{
		if (s1[i] == set[j])
			i--;
		else if (s1[i] != set[j])
		{
			j = 0;
			while (set[j] != s1[i] && set[j])
				j++;
		}
	}
	return (i + 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		size;
	int		left_index;
	int		right_index;
	char	*dst;

	left_index = trim_left(s1, set);
	right_index = trim_right(s1, set);
	size = right_index - left_index + 1;
	if (size <= 1)
	{
		dst = malloc(1 * sizeof(char));
		if (!dst)
			return (NULL);
		dst[0] = 0;
		return (dst);
	}
	else
		dst = malloc(size * sizeof(char));
	if (!dst)
		return (NULL);
	dst = ft_copy(s1, dst, left_index, right_index);
	return (dst);
}
