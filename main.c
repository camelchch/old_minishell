/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 12:19:33 by saxiao            #+#    #+#             */
/*   Updated: 2018/06/04 17:03:27 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	put_strstr(char **str)
{
		while(*str)
			ft_printf("%s\n", *str++);
}

void	env_i(char **paras, char **new_env, char ***env, t_sh *table)
{
	char	**cp;
	int		i;

	i = 0;
		if ((*paras)[1] == 'i')
		{
			paras++;
			while (*paras && ft_strstr(*paras, "="))
				new_env[i++] = *paras++;
			new_env[i] = NULL;
			cp = new_env;
			if (*paras)
			pipes(*paras, no_pipe(*paras), &cp, table);
}
	 else if ((*paras)[1] == 'u')
		{
			cp  = *env;
			while (*cp)
				new_env[i++] = *cp++;
			new_env[i] = NULL;
	cp = unset_env(paras, new_env);
			paras = paras + 2;
			put_strstr(cp);
			if (*paras)
			pipes(*paras, no_pipe(*paras), &cp, table);
		}
		else
			ft_printf("Usage: env [-u name] [-i] [name=value ...] [utlity]\n");
}

void	put_env(char **env, char **paras, t_sh *table)
{
	char	*new_env[2048];
//	char	**cp;
	int		i;

	paras++;
	i = 0;
	new_env[0] = 0;
	if (!*paras)
		put_strstr(env);
	else if (*paras && **paras == '-')
	{
		/*
		if ((*paras)[1] == 'i')
		{
			paras++;
			while (*paras && ft_strstr(*paras, "="))
				new_env[i++] = *paras++;
			new_env[i] = NULL;
			cp = new_env;
			if (*paras)
			pipes(*paras, no_pipe(*paras), &cp, table);
		}
		else if ((*paras)[1] == 'u')
		{
			cp  = env;
			while (*cp)
				new_env[i++] = *cp++;
			new_env[i] = NULL;
	cp = unset_env(paras, new_env);
			paras = paras + 2;
			put_strstr(cp);
			if (*paras)
			pipes(*paras, no_pipe(*paras), &cp, table);
		}
		else
			ft_printf("Usage: env [-u name] [-i] [name=value ...] [utlity]\n");
			*/
		env_i(paras, new_env, &env , table);
	}
	else
			pipes(*paras, no_pipe(*paras), &env, table);



}

void	put_intable(t_table *in_tab)
{
	while (in_tab)
	{
		ft_printf("%s\n", in_tab->name);
		in_tab = in_tab->next;
	}
}

void	put_sh(t_sh *table)
{
	int		i;

	i = 0;
	while (i < 100)
	{
		if (table[i].sh_ta)
		{
			ft_printf("here is %d sh_table\n",i);
			put_intable(table[i].sh_ta);
			i++;
		}
	}
}

void	free_sh_table(t_sh *table, int index)
{
	int			i;
	//t_table		*temp;
	t_table		*cp;

	i = 0;
	//if (table[i])
	//{
		while (i < index)
		{
		while (table[i].sh_ta)
		{
			cp = table[i].sh_ta;
		while (cp->next)
			cp = cp->next;
			//temp = table[i].sh_ta;
		//	table[i].sh_ta = table[i].sh_ta->next;
			free(cp);
		}
		i++;
	}
//}
}

char	**copy_env(char **env)
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

int		main(int ac, char **av, char **env)
{
	t_sh	table[100];
	char	**all_path;
	char	**cp_env;

//	put_env(env);
	(void)ac;
	(void)av;
	cp_env = copy_env(env);
	update_shlvl(&cp_env);
	all_path = path(cp_env);
	init_shtable(table, all_path);
//	put_sh(table);
	shell(ac, av, cp_env, table);
//	get_autoline(table);
	free_sh_table(table, 100);
	ft_freestrstr(cp_env);
	return (0);
}
