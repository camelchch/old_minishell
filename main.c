/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 11:44:10 by saxiao            #+#    #+#             */
/*   Updated: 2018/06/25 11:19:24 by saxiao           ###   ########.fr       */
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

void		free_sh_table(t_sh *table, int index)
{
	int			i;
	t_table		*cp;
	t_table		*temp;

	i = 0;
	while (i < index)
	{
		if (table[i].sh_ta != NULL)
		{
			cp = table[i].sh_ta;
			while (cp)
			{
				temp = cp;
				cp = cp->next;
				free(temp);
			}
		}
		table[i].sh_ta = NULL;
		i++;
	}
}

static void	set_sh_null(t_sh *table, int index)
{
	int		i;

	i = -1;
	while (++i < index)
		table[i].sh_ta = NULL;
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
	set_sh_null(table, 100);
	init_shtable(table, all_path);
	ft_freestrstr(all_path);
	shell(cp_env, table);
	return (0);
}
