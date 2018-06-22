/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_unset_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 11:54:30 by saxiao            #+#    #+#             */
/*   Updated: 2018/06/22 11:55:01 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

static void	add_env(char ***env, char **paras)
{
	char	**cp;
	int		ct;
	int		i;
	char	*temp;
	char	**for_free;

	i = 0;
	ct = nb_str(*env) + 2;
	cp = *env;
	for_free = *env;
	*env = malloc(sizeof(char *) * ct);
	while (i < ct - 2)
		(*env)[i++] = ft_strdup(*cp++);
	(*env)[i] = ft_strjoin(*paras, "=");
	paras++;
	temp = (*env)[i];
	(*env)[i] = ft_strjoin((*env)[i], *paras);
	free(temp);
	(*env)[++i] = NULL;
	ft_freestrstr(for_free);
}

static char	**delet_env(char **env, int index)
{
	int		after;
	char	**temp;

	temp = env;
	free(temp[index]);
	while (index < nb_str(env) - 1)
	{
		after = index + 1;
		temp[index++] = temp[after];
	}
	temp[nb_str(env) - 1] = NULL;
	return (temp);
}

void		set_env(char **paras, char ***env)
{
	char	**cp;
	char	*temp;

	cp = *env;
	paras++;
	while (*cp && !(!ft_strncmp(*paras, *cp, ft_strlen(*paras)) && \
	ft_strlen(*paras) < ft_strlen(*cp) && (*cp)[ft_strlen(*paras)] == '='))
		cp++;
	if (*cp)
	{
		free(*cp);
		*cp = ft_strjoin(*paras++, "=");
		temp = *cp;
		*cp = ft_strjoin(*cp, *paras);
		free(temp);
	}
	else
		add_env(env, paras);
}

char		**unset_env(char **paras, char **env)
{
	char	**cp;
	int		index;

	cp = env;
	index = 0;
	paras++;
	while (*cp && !(!ft_strncmp(*paras, *cp, ft_strlen(*paras)) && \
	ft_strlen(*paras) < ft_strlen(*cp) && (*cp)[ft_strlen(*paras)] == '='))
	{
		cp++;
		index++;
	}
	if (*cp)
		env = delet_env(env, index);
	else
		ft_printf("no such variable %s\n", *paras);
	return (env);
}
