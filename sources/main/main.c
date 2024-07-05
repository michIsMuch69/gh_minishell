/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florian <florian@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:58:11 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/05 09:16:39 by florian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*
Pour configurer Visual Studio Code (VS Code) afin que le raccourci Ctrl + Clic redirige vers la définition d’une fonction, voici les étapes à suivre :

    Ouvrez VS Code.
    Appuyez sur Ctrl + K suivi de Ctrl + S (ou Cmd + K suivi de Cmd + S sur Mac) pour accéder à la fenêtre des raccourcis clavier.
    Recherchez l’option “Preferences: Open Settings (JSON)” et cliquez dessus.
    Dans le fichier keybindings.json, vous pouvez personnaliser vos raccourcis clavier. Cherchez la combinaison de touches
    que vous souhaitez utiliser pour rediriger vers la définition d’une fonction.
    Ajoutez votre raccourci personnalisé en utilisant le format JSON, par exemple :

	{
    "key": "ctrl+click",
    "command": "editor.action.goToDeclaration"
	}
	Dans cet exemple, ctrl+click est la combinaison de touches pour rediriger vers la définition.
	N’oubliez pas de sauvegarder vos modifications. Désormais, lorsque vous cliquerez tout en maintenant la touche Ctrl,
  VS Code vous dirigera vers la définition de la fonction. 🚀

*/

// ###### INCLUDES ######

#include "exec.h"
#include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "get_next_line.h"

// ###### INCLUDES ######


// ###### PROTO ######

int		ft_perror(char *err_message);
int		parse_prompt(char **prompt, char **envp, t_data **data);
void	free_struct(t_data *struc, int tab_size);
void	free_tab(t_table *tab, int start);
int		exec(int tab_size, t_data *data);

void	print_tab(t_table tab);

// ###### PROTO ######

// cat <run.sh | cat >a | cat <README.md >b | cat <<e | cat >c


/*
	* leak test with valgrind :
	* valgrind --suppressions=run.sh --leak-check=full ./minishell
*/

// function only for tests
void	print_tab(t_table tab)
{
	int	i = 0;

	while(i < tab.size)
		printf("%s\n", tab.tab[i++]);
}

// function only for tests
void	print_struct(t_data *data, int tab_size)
{
	int	i = 0;
	int	y = 0;

	while (i < tab_size)
	{
		printf("\nSTRUC %d\n\n", i+1);
		printf("cmd  = %s\n", data[i].cmd_path);
		y = 0;
		if (data[i].args.tab)
		{
			printf("args list :\n");
			print_tab(data[i].args);
		}
		if (data[i].input.tab)
		{
			printf("\n");
			printf("inputs :\n");
			print_tab(data[i].input);
		}
		if (data[i].output.tab)
		{
			printf("\n");
			printf("output :\n");
			print_tab(data[i].output);
		}
		printf("\n");
		// y = 0;
		// if (data[i].env.tab)
		// 	print_tab(data[i].env);
		i++;
	}
}

static t_table	ft_tabdup(char **envp)
{
	t_table	tab_env;

	tab_env.size = 0;
	if (!envp)
		return (tab_env.tab = NULL, tab_env);
	while (envp[(tab_env.size)++])
		;
	tab_env.tab = ft_calloc(tab_env.size + 1, sizeof(char *));
	if (!tab_env.tab)
		return (tab_env);
	tab_env.size = 0;
	while (envp[tab_env.size])
	{
		tab_env.tab[tab_env.size] = ft_strdup(envp[tab_env.size]);
		if (!tab_env.tab[tab_env.size])
			return (free_tab(&tab_env, 0), tab_env.tab = NULL, tab_env);
		(tab_env.size)++;
	}
	return (tab_env);
}

static t_data	*init_data(char **envp)
{
	t_data	*data;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (NULL);
	data->env = ft_tabdup(envp);
	if (!data->env.tab)
		return (free(data), NULL);
	return (data);
}

static t_data	*reset_env(t_data *data, int tab_size)
{
	t_table	tmp;

	tmp = ft_tabdup(data->env.tab);
	if (!tmp.tab)
	{
		free_struct(data, tab_size);
		return (ft_perror("error -> reset env\n"), NULL);
	}
	free_struct(data, tab_size);
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		return (free_tab(&tmp, 0), ft_perror("error -> reset env\n"), NULL);
	data->env.tab = tmp.tab;
	data->env.size = tmp.size;
	return (data);
}

int main (int argc, char **argv, char **envp)
{
	char 	*prompt;
	int		tab_size;
	t_data	*data;

	// static int index = 0;

	(void) argc;
	(void) argv;
	if (argc != 1)
		return (ft_perror("arguments are invalid\n"), 1);
	data = init_data(envp);
	if (!data)
		return (ft_perror("error -> init structure\n"), 2);
	while (1)
	{
		prompt = readline("\033[32mmini$hell>\033[0m ");
		add_history(prompt); // !! need to clear history
		tab_size = parse_prompt(&prompt, data->env.tab, &data);
		if (tab_size == -1)
			return (free_struct(data, 1), 4);
		if (tab_size)
      if (exec(tab_size, data) == -1)
			  return (free_struct(data, 1), free(prompt), 5);
		// if (++index == 4)
		// 	return (free(prompt), free_struct(data, tab_size), 0);
		if (prompt)
      	free(prompt);
		data = reset_env(data, tab_size);
		if (!data)
			return (5);
	}
	return (0);
}
