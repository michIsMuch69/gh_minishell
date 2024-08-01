/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:21:46 by jedusser          #+#    #+#             */
/*   Updated: 2024/08/01 07:40:32 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	free_vars(t_vars *vars)
{
	if (vars->equal_pos)
	{
		if (!vars->key)
			free(vars->equal_pos);
		vars->equal_pos = NULL;
	}
	if (vars->key)
	{
		free(vars->key);
		vars->key = NULL;
	}
	if (vars->new_var)
	{
		free(vars->new_var);
		vars->new_var = NULL;
	}
	if (vars->value)
	{
		free(vars->value);
		vars->value = NULL;
	}
}
