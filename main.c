/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 11:44:10 by saxiao            #+#    #+#             */
/*   Updated: 2018/06/22 11:44:33 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void		put_strstr(char **str)
{
	while (*str)
		ft_printf("%s\n", *str++);
}

static void	free_sh_table(t_sh *table, int index)
{
	int			i;
	t_table		*cp;

	i = 0;
	while (i < index)
	{
		while (table[i].sh_ta)
		{
			cp = table[i].sh_ta;
			while (cp->next)
				cp = cp->next;
			free(cp);
		}
		i++;
	}
}

static char	**copy_env(char **env)
{
	char	**cp;
	char	**res;
	int		ct;
	int		i;

	i = 0;
	ct = nb_str(env) + 1;
	cp = env;
	res = malloc(sizeof(char *) * ct);
	while (i < ct - 1)
		(res)[i++] = ft_strdup(*cp++);
	res[i] = NULL;
	return (res);
}

int			main(int ac, char **av, char **env)
{
	t_sh	table[100];
	char	**all_path;
	char	**cp_env;

	(void)ac;
	(void)av;
	all_path = NULL;
	cp_env = copy_env(env);
	update_shlvl(&cp_env);
	all_path = path(cp_env);
	init_shtable(table, all_path);
	shell(cp_env, table);
	free_sh_table(table, 100);
	ft_freestrstr(cp_env);
	ft_freestrstr(all_path);
	return (0);
}
