/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 11:44:56 by saxiao            #+#    #+#             */
/*   Updated: 2018/06/22 17:14:59 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"
#include "minishell.h"

int		no_pipe(char *cmdline)
{
	int		i;

	i = 0;
	while (*cmdline)
	{
		if (*cmdline++ == '|')
			i++;
	}
	return (i);
}

void		replace_home(char *cp, char *home)
{
	int		i;
	char	temp[PATH_MAX + 1];

	ft_bzero(temp, PATH_MAX + 1);
	i = 0;
	while (cp[i] && cp[i] != '~')
		i++;
	ft_strcpy(temp, cp + i + 1);
	ft_bzero(cp + i, PATH_MAX + 1 - i);
	ft_strcpy(cp + i, home);
	ft_strcat(cp, temp);
}

void	update_shlvl(char ***env)
{
	char	**cp;
	int		level;
	char	*shlvl[4];
	char	*lv;

	cp = *env;
	level = 1;
	while (*cp && !(!ft_strncmp(*cp, "SHLVL", 5)))
		cp++;
	if (*cp)
		level = ft_atoi(*cp + 6) + 1;
	lv = ft_itoa(level);
	shlvl[0] = "no real meaning here";
	shlvl[1] = "SHLVL";
	shlvl[2] = lv;
	shlvl[3] = NULL;
	set_env(shlvl, env);
	free(lv);
}

void	update_lastapp(char *lastcmd, char ***env)
{
	char *lastapp[4];

	lastapp[0] = "no real meaning here";
	lastapp[1] = "_";
	lastapp[2] = lastcmd;
	lastapp[3] = NULL;
	set_env(lastapp, env);
}

void	child_pro(char **paras, char **env, t_sh *table)
{
	char	*path;

	if (!access(*paras, F_OK))
	{
		update_lastapp(*paras, &env);
		execve(*paras, paras, env);
		ft_printf("permission denied for this program: %s\n", *paras);
		exit(0);
	}
	path = path_in_sh(*paras, table);
	if (!path)
	{
		ft_printf("%s : there is no such program\n", *paras);
		exit(0);
	}
	else
	{
		update_lastapp(path, &env);
		execve(path, paras, env);
		ft_printf("permission denied for this program %s\n", *paras);
		exit(0);
	}
}
