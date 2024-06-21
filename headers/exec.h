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

void	print_tab(t_table tab);
void	print_struct(t_data *data, int tab_size);

/*===========================main/utils.c===============================*/

int		ft_perror(char *err_message);

/*===========================build_exec_path.c===============================*/

char	*ft_concat_path(char *directory, char *prompt);
char	*check_all_dirs(char *exec_searched);

/*===========================exec_utils.c===============================*/

int		ft_strcmp(char *s1, char *s2);
void	free_array(char **array);
void	wait_all(int tab_size);
int	  clean_struct(t_data *data);
int   handle_redirection(int fds[2], int prev_fd, t_data *data);

/*===========================builtins.c===============================*/

int		ft_exit(void);
int		ft_cd(char **args);

/*===========================builtins_utils.c===============================*/

int		is_builtin(t_data *data);
void	exec_builtin(t_data *data);

/*===========================redirections.c===============================*/

int		redir_input(t_data *data, int prev_fd);
int		redir_output(t_data *data, int *fds);

/*===========================redirections_utils.c===============================*/

char	*skip_redir_symbol(char *token_file, bool direction);
int		arrow_count(char *str, char c);
int   create_all(t_table outfile);

/*===========================parsing/expand.c===============================*/

int	  expand_management(t_data *data, char **envp);

/*===========================parsing/cleaner.c===============================*/

int	  token_cleaner(t_data *data);

#endif
