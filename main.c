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

void		unset_sub_env(char **paras, char **env, char **new_env)
{
	char	**cp;
	int		index;

	cp = env;
	index = 0;
	paras++;
	while (*cp && !(!ft_strncmp(*paras, *cp, ft_strlen(*paras)) && ft_strlen(*paras) < ft_strlen(*cp) && (*cp)[ft_strlen(*paras)] == '='))
		new_env[index++] = *cp++;
	new_env[index] = NULL;
	if (*cp)
	{
		cp++;
		while (cp && *cp)
			new_env[index++] = *cp++;
	new_env[index] = NULL;
	}
	else
		ft_printf("no such variable %s\n", *paras);
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
		else
			put_strstr(cp);
	}
	else if ((*paras)[1] == 'u')
	{
		unset_sub_env(paras, *env, new_env);
		paras = paras + 2;
		if (*paras)
			pipes(*paras, no_pipe(*paras), &new_env, table);
		else
			put_strstr((char **)new_env);
	}
	else
		ft_printf("Usage: env [-u name] [-i] [name=value ...] [utlity]\n");
}

void	put_env(char **env, char **paras, t_sh *table)
{
	char	**new_env;
	//	char	**cp;
	int		i;
	int		j;

	paras++;
	i = -1;
	j = -1;
	new_env = malloc(sizeof(char *) * BUF_MAX);
	//new_env[0] = 0;
	if (!*paras)
		put_strstr(env);
	else if (*paras && **paras == '-')
	{
		env_i(paras, new_env, &env , table);
		free(new_env);
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
