/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:46:39 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/04 18:50:10 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*
je dois ecrire un programme en c qui imite le systeme de pipe et de redirection
de fichier de bash. Je sais que lorsque qu'il y a des redirections de fichier,
ceux ci sont prioritaire sur les pipes. Voila la strategie que j'ai mis en place.
Chaque commandes ainsi que leur infos assiocies ( nom des fichiers de redirection,
tableau d'arguments, chemin vers le binaire) sont stockes dans un tableau de structure.
Le processus parent ouvre le nombre de pipes necessaire pour la communication entre
les differentes commandes ( le nombre de pipe est egale a (taille_tableau - 1) ).
dans une boucle, a chaque iteration du tableau je stock dans la structure de la
commande les files descriptors corespondant aux fichiers de redirection des inputs et des outputs
( donc si j'ouvre un fichier file1 pour une redirection d'output dans la premiere
commande et que j'ouvre egalement le fichier file1 dans la deuxieme commande pour
une redirection d'input ils n'auront pas le meme file descriptor ).
Apres avoir recuperer les redirections de fichiers de l'instance de mon tableau
de structure je fais un fork. l'enfant doit gerer les redirection (pipes et redirections
de fichier) en redirigeant les entrees et sortie standard avec les fonctions dup et dup2
et le parent doit fermer les fd qu'il n'utilise pas mais doit garder les bon pipes ouverts.
*/

#include "exec.h"

void    free_pipes(int **tab, int size);
int     ft_dup(int read_fd, int write_fd);
int     close_pipes(int **fds, int size, int i_start, int last_fd);
int     pipe_redirection(t_data data, int *fds, int last_read);

int child_routine(t_data *data, int tab_size, int i, int **fd)
{
    int ret_value;

    if (i < tab_size - 1)
    {
        if (i)
            ret_value = pipe_redirection(data[i], fd[i], fd[i - 1][0]);
        else
            ret_value = pipe_redirection(data[i], fd[i], 0);
    }
    else
        ret_value = pipe_redirection(data[i], NULL, fd[i - 1][0]);
    if (i == tab_size -1)
    {
        if (close(fd[i - 1][0]) == -1)
            return (-1);
    }
    else
    {
        if (i && close_pipes(fd, (tab_size - 1), i, fd[i -1][0]) == -1)
                return (-1);
        else if (!i && close_pipes(fd, (tab_size - 1), i, 0) == -1)
            return (-1);
    }
    return (ret_value);
}

// cat <run.sh | cat >a | cat <README.md >b | cat <<e | cat >c

static int	exec_all(t_data *data, int tab_size, int **fd)
{
    int     i;
	pid_t   pid;
    int     ret_value;
    int     last_read_fd;

    last_read_fd = 0;
    i = 0;
    while (i < tab_size)
	{
        pid = fork();
        if (pid < 0)
            return (perror("Fork failed "), pid);  // -1 -> crash
        if (pid == 0)
        {
            if (child_routine(data, tab_size, i, fd) == -1)
                return (free_pipes(fd, tab_size -1), exit(1), 1);
            if (execve(data[i].cmd_path, data[i].args.tab, data[i].env.tab) == -1)
                exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            if (i == 0)
            {
                if (close(fd[i][1]) == -1)
                    perror("parent close write pipe ");
                if (data[i].in_out_fd[0] > STDIN_FILENO)
                    if (close(data[i].in_out_fd[0]) == -1)
                        perror("parent close infile ");
                if (data[i].in_out_fd[1] > STDOUT_FILENO)
                    if (close(data[i].in_out_fd[1]) == -1)
                        perror("parent close outfile ");
            }
            else if (i < tab_size)
            {
                if (close(last_read_fd) == -1)
                    perror("parent close read pipe ");

                if (i < tab_size - 1)
                    if (close(fd[i][1]) == -1)
                        perror("parent close write pipe ");

                if (data[i].in_out_fd[0] != STDIN_FILENO)
                    if (close(data[i].in_out_fd[0]) == -1)
                        perror("parent close infile 2 ");
                if (data[i].in_out_fd[1] != STDOUT_FILENO)
                    if (close(data[i].in_out_fd[1]) == -1)
                        perror("parent close outfile 2 ");
            }
        }
        if (i < tab_size - 1)
            last_read_fd = fd[i][0];
        i++;
    }
    free_pipes(fd, tab_size - 1);
    for (int y = 0; y < tab_size; y++){
        if (wait(&(data[0].exit_status)) == -1)
        {
            perror("errno wait ");
            printf("errno == %d\n", errno);
        }
    }
    return (0);
}

static int exec_one(t_data *data)
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
        return (perror("Fork failed "), close_fds(data->in_out_fd), -1);
    if (pid > 0)
    {
        if (wait(&(data->exit_status)) == -1)
            return (ft_perror("crash -> wait()\n"), -1);
        return (close_fds(data->in_out_fd));
    }
    if (ft_dup(data->in_out_fd[0], data->in_out_fd[1]) == -1)
    {
        close_fds(data->in_out_fd);
        exit(EXIT_FAILURE);
    }
    // if (is_builtin(data))
    //   exec_builtin(data, 1, NULL);
    if (close_fds(data->in_out_fd) == -1)
        exit(EXIT_FAILURE);
    if (execve(data->cmd_path, data->args.tab, data->env.tab) == -1)
        perror("execve ");
    exit(EXIT_FAILURE);
    return (-1);
}

/*
  * before loop
    -> play heredocs
    -> open pipes

  * in loop on all instances of t_data *data in forks ?
    -> handle redirection
      . create all outputs
      . fill fds[1] with the new output fd
      . open inputs
      . fill fd[0] with the new input fd
    -> end of parsing
      - expand variables
      - clean args tokens

    -> if (is_builtins() == 1)
      - play builtin

    -> else
      - fork and execve();
*/
int	exec(int tab_size, t_data *data)
{
    int ret_value;
    int **pipe_fd;

    pipe_fd = NULL;
    ret_value = init_exec(data, tab_size);
    if (ret_value)
        return (ret_value); // -1 -> crash : 1 -> back to prompt CLOSE ALL IN_OUT
    if (tab_size == 1)
        ret_value = exec_one(&(data[0]));
    else
    {
        pipe_fd = init_pipe(data, tab_size - 1);
        if (!pipe_fd)
            return (-1); // CLOSE ALL IN_OUT
        ret_value = exec_all(data, tab_size, pipe_fd);
    }
    return (ret_value);
}
