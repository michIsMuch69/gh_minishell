/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:58:22 by jedusser          #+#    #+#             */
/*   Updated: 2024/08/01 09:36:46 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "includes.h"

void	print_tab(t_table tab);
void	print_struct(t_data *data, int tab_size);

/*===========================main/utils.c===============================*/

int		ft_perror(char *err_message);
void	free_tab(t_table *tab, int start);
void	free_struct(t_data *struc, int tab_size);

/*===========================build_exec_path.c===============================*/

char	*ft_concat_path(char *directory, char *prompt);
int		check_all_dirs(t_data *data, char **directory);

/*===========================exec_utils.c===============================*/

int		ft_strcmp(char *s1, char *s2);
void	free_array(char **array);

/*===========================redirections.c===============================*/

int		handle_redirection(t_data *data);

/*===========================redirections_utils.c=========================*/

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

int		ft_dup(int read_fd, int write_fd);
int		**init_pipe(t_data *data, int size);
void	free_pipes(int **tab, int size);
int		close_fds(int **fds, int size, int i_start, int last_fd);
int		ft_getenv(char *word, char **env, char **var_content);
void	set_env(char *var, char *cwd, char **env);
char	*var_exist(char *word, char **env);
int		wait_all(t_data *data, int tab_size, int pid, int **fd);
int		init_exec(t_data *data, int tab_size);

/*===========================exec_one_utils.c===============================*/

int		save_std_fileno(t_data *data, int saved_fd[]);
int		redir_file(t_data *data);
int		reset_std_fileno(t_data *data, int saved_fd[]);
int		close_all_redir(t_data *data, int saved_std[]);

/*===========================exec_one_utils.c===============================*/

void	exec_builtin_child(t_data *data, int i, int **fd, int last_read);

char	**create_minimal_env();


#endif
