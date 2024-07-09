/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 16:18:42 by fberthou          #+#    #+#             */
/*   Updated: 2024/01/05 11:50:30 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <unistd.h>
# include <stdlib.h>

char	*get_next_line(int fd);
void	clear_buffer(char *buffer, int index);
size_t	lenstr(char *s);
char	*ft_calloct(size_t nmemb, size_t size);
char	*cpy_substr(char *buffer);
char	*concate_buffer(char *buffer, char *tmp);

#endif