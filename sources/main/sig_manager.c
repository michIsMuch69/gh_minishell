/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jean-micheldusserre <jean-micheldusserr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:34:59 by florian           #+#    #+#             */
/*   Updated: 2024/07/11 16:22:13 by jean-michel      ###   ########.fr       */
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

void    sig_manager(int signum)
{
    if (write(1, "\n", 1) == -1)
        exit(EXIT_FAILURE);
    rl_on_new_line();
   // rl_replace_line("", 0);
    rl_redisplay();
}

int init_sighandler(t_data *data)
{
    struct sigaction    act_one;
    struct sigaction    act_two;

    ft_memset(&act_one, 0, sizeof(act_one));
    ft_memset(&act_two, 0, sizeof(act_two));
    act_one.sa_handler = sig_manager;
	act_two.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &act_one, NULL) == -1 || \
		sigaction(SIGQUIT, &act_two, NULL) == -1)
        return (ft_perror("error -> init sig_manager\n"), -1);
	return (0);
}
