/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 11:46:18 by saxiao            #+#    #+#             */
/*   Updated: 2018/06/22 17:10:34 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

static void	init_tempwd(char **tempwd, char *path)
{
	tempwd[0] = "no real meaning here";
	tempwd[1] = "OLDPWD";
	tempwd[2] = getcwd(path, PATH_MAX + 1);
	tempwd[3] = NULL;
}

static void	oldpwd_home(char *cp, char ***env, int ct)
{
	char	*temp;

	if (ct == 1 || ft_strstr(cp, "~"))
	{
		temp = ft_getenv(*env, "HOME");
		if (!temp)
		{
			ft_printf("enviroment HOME is not set\n");
			ft_bzero(cp, PATH_MAX + 1);
		}
		else
			ct == 1 ? ft_strcpy(cp, temp) : replace_home(cp, temp);
	}
	else
	{
		temp = ft_getenv(*env, "OLDPWD");
		if (!temp)
		{
			ft_printf("enviroment OLDPWD  is not set\n");
			ft_bzero(cp, PATH_MAX + 1);
		}
		else
			ft_strcpy(cp, temp);
	}
}

static void	for_cd(char *paras, char ***env, char **tempwd, char *path)
{
	if (!chdir(paras))
	{
		set_env(tempwd, env);
		tempwd[1] = "PWD";
		tempwd[2] = getcwd(path, PATH_MAX);
		set_env(tempwd, env);
	}
	else
	{
		if (access(paras, F_OK))
			ft_printf("no such file or directory: %s\n", paras);
		else if (access(paras, X_OK))
			ft_printf("permission denied\n");
	}
}

int			cd(char **paras, char ***env)
{
	int		ct;
	char	*tempwd[4];
	char	path[PATH_MAX + 1];
	char	cp[PATH_MAX + 1];

	ct = nb_str(paras);
	ft_bzero(cp, PATH_MAX + 1);
	if (ct == 2)
		ft_strcpy(cp, paras[1]);
	if (ct != 1 && ct != 2)
	{
		ft_printf("Too many arguments--usage : cd path\n");
		return (1);
	}
	else
	{
		init_tempwd(tempwd, path);
		if (ct == 1 || (ct == 2 && (ft_strstr(*(paras + 1), "~") || \
						!ft_strcmp(*(paras + 1), "-"))))
			oldpwd_home(cp, env, ct);
		if (ft_strlen(cp))
			for_cd(cp, env, tempwd, path);
		return (0);
	}
}
