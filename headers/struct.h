/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 09:16:08 by fberthou          #+#    #+#             */
/*   Updated: 2024/08/01 07:00:59 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_table
{
	char			**tab;
	int				size;
}					t_table;

enum				e_rtype
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
	t_table			args;
	t_table			env;
	t_table			input;
	t_table			output;
	t_table			export;
	t_table			docs_files;
	int				tab_size;
	char			*prompt;
}					t_data;

typedef struct s_vars
{
	char			*key;
	char			*value;
	char			*new_var;
	char			*equal_pos;
}					t_vars;
#endif
