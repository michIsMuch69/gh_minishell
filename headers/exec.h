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

extern char **g_exported_env;
extern int g_exported_env_size;

void	print_tab(t_table tab);
void	print_struct(t_data *data, int tab_size);

/*===========================main/utils.c===============================*/

int		ft_perror(char *err_message);
void	free_tab(t_table *tab, int start);
void	free_struct(t_data *struc, int tab_size);

/*===========================build_exec_path.c===============================*/

char	*ft_concat_path(char *directory, char *prompt);
int 	check_all_dirs(t_data *data, char **directory);

/*===========================exec_utils.c===============================*/

int		ft_strcmp(char *s1, char *s2);
void	free_array(char **array);

/*===========================builtins.c===============================*/

void	ft_exit(t_data *data, int tab_size, int last_status);
int		ft_cd(char **args, char **env);
int		ft_pwd(void);
int		ft_echo(char **args);
int		ft_env(char **env);
int ft_export_print(void);

int		ft_unset(char *var, t_table *env);
//int		ft_export(t_table args , t_table env);
int ft_export(char **args, char ***env, int *env_size);
void init_exported_env(char **env, int env_size);




/*===========================builtins_utils.c===============================*/

int		is_builtin_parent(t_data *data);
int		is_builtin_child(t_data *data);

int		exec_builtin_parent(t_data *data, int tab_size, int i, int **fd);
int		exec_builtin_child(t_data *data, int tab_size, int i, int **fd);
int		is_numeric_str(char *str);


/*===========================redirections.c===============================*/

int		handle_redirection(t_data *data);

/*===========================redirections_utils.c===============================*/

char	*skip_redir_symbol(char *token_file, bool direction);
int		arrow_count(char *str, char c);
int		create_all(t_table outfile);
int		check_all(t_table infile);

/*===========================parsing/expand.c===============================*/

int		expand_management(t_data *data, char **envp, int last_exit);

/*===========================parsing/cleaner.c===============================*/

int		token_cleaner(t_data *data);

/*===========================heredoc.c===============================*/

int		heredoc_management(t_data *data, int tab_size);

/*===========================fds_management.c===============================*/

int     ft_dup(int read_fd, int write_fd);
int		close_fds(int *in_out_fd);
int     **init_pipe(t_data *data, int size);
int     manage_redirection(t_data *data, int tab_size, int i, int **fd);

void	free_pipes(int **tab, int size);
int     close_pipes(int **fds, int size, int i_start, int last_fd);
int		ft_getenv(char *word, char **env, char **var_content);
void	set_env(char *var, char *cwd, char **env);
char	*var_exist(char *word, char **env);
int     wait_all(t_data *data, int tab_size, int pid);
int     init_exec(t_data *data, int tab_size);
#endif
