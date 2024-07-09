/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 09:16:08 by fberthou          #+#    #+#             */
/*   Updated: 2024/07/08 16:51:01 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_table
{
	char	**tab;
	int		size;
} t_table;

enum e_rtype
{
	NO_EXPAND = -1,
	COMMAND,
	PIPE,
	INPUT,
	OUTPUT
};

typedef struct s_data
{
	enum e_rtype	cmd_type;
	char			*cmd_path;
	int				exit_status;
	int				in_out_fd[2];
	t_table	  		args;
	t_table		  	env;
	t_table		  	input;
	t_table		  	output;
  	t_table			docs_files;
    int             tab_size;
    char            *prompt;
} t_data;

#endif
