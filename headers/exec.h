/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handler.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:58:22 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/11 10:10:38 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "libft.h"
#include "struct.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <errno.h>

void	print_tab(t_table tab);
void	print_struct(t_data *data, int tab_size);

/*===========================main/utils.c===============================*/

int		ft_perror(char *err_message);
void	free_tab(t_table *tab, int start);

/*===========================build_exec_path.c===============================*/

char	*ft_concat_path(char *directory, char *prompt);
int 	check_all_dirs(t_data *data, char **directory);

/*===========================exec_utils.c===============================*/

int		ft_strcmp(char *s1, char *s2);
void	free_array(char **array);
int 	init_structure(t_data *data, int *in_out);

/*===========================builtins.c===============================*/

int		ft_exit(void);
int		ft_cd(char **args);

/*===========================builtins_utils.c===============================*/

int		is_builtin(t_data *data);
void	exec_builtin(t_data *data);

/*===========================redirections.c===============================*/

int   handle_redirection(int *fds, t_data *data);
int  close_fds(int **fds, int size, int in_out[2]);

/*===========================redirections_utils.c===============================*/

char	*skip_redir_symbol(char *token_file, bool direction);
int		arrow_count(char *str, char c);
int   create_all(t_table outfile);
int   check_all(t_table infile);

/*===========================parsing/expand.c===============================*/

int	  expand_management(t_data *data, char **envp);

/*===========================parsing/cleaner.c===============================*/

int	  token_cleaner(t_data *data);

/*===========================heredoc.c===============================*/

int	  heredoc_management(t_data *data, int tab_size);

#endif
