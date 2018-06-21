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

static void		unset_sub_env(char **paras, char **env, char **new_env)
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

static void	env_i(char **paras, char **new_env, char ***env, t_sh *table)
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
		*paras ? pipes(*paras, no_pipe(*paras), &cp, table) : put_strstr(cp);
	}
	else if ((*paras)[1] == 'u')
	{
		unset_sub_env(paras, *env, new_env);
		paras = paras + 2;
		*paras ? pipes(*paras, no_pipe(*paras), &new_env, table) : \
		put_strstr((char **)new_env);
	}
	else
		ft_printf("Usage: env [-u name] [-i] [name=value ...] [utlity]\n");
}

void		put_env(char **env, char **paras, t_sh *table)
{
	char	**new_env;
	int		i;
	int		j;

	paras++;
	i = -1;
	j = -1;
	new_env = malloc(sizeof(char *) * BUF_MAX);
	if (!*paras)
		put_strstr(env);
	else if (*paras && **paras == '-')
		env_i(paras, new_env, &env , table);
	else
		pipes(*paras, no_pipe(*paras), &env, table);
	free(new_env);
}
