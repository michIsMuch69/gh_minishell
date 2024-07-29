/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:34:59 by florian           #+#    #+#             */
/*   Updated: 2024/07/29 14:30:28 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <struct.h>
#include <libft.h>
#include "exec.h"

int		ft_perror(char *err_message);

t_data  *give_data(t_data *data)
{
    static t_data   *new;

    if (data != NULL)
    {
        new = data;
        return (NULL);
    }
    else
        return (new);
}

void    sig_manager(int signum)
{
    t_data  *data;

    data = give_data(NULL);
    if (write(1, "\n", 1) == -1)
        exit(EXIT_FAILURE);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    if (!data)
        return ;
    else
        data[0].exit_status = 130;
}


int init_sighandler(t_data *data)
{
    struct sigaction act_one;
    struct sigaction act_two;
    struct sigaction act_pipe;  // Add this line

    give_data(data);
    ft_memset(&act_one, 0, sizeof(struct sigaction));
    ft_memset(&act_two, 0, sizeof(struct sigaction));
    ft_memset(&act_pipe, 0, sizeof(struct sigaction));  // Add this line

    act_one.sa_handler = sig_manager;
    act_two.sa_handler = SIG_IGN;
    act_pipe.sa_handler = handle_sigpipe;  // Add this line

    if (sigaction(SIGINT, &act_one, NULL) == -1 || 
        sigaction(SIGQUIT, &act_two, NULL) == -1 || 
        sigaction(SIGPIPE, &act_pipe, NULL) == -1)  // Modify this line
    {
        return (ft_perror("error -> init sig_manager\n"), -1);
    }

    return (0);
}

