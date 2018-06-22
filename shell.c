/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 11:52:57 by saxiao            #+#    #+#             */
/*   Updated: 2018/06/22 15:16:56 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

char		*path_in_sh(char *app, t_sh *table)
{
	int			index;
	t_table		*cp;

	index = calcu_index(app);
	cp = table[index].sh_ta;
	while (cp)
	{
		if (!strcmp(app, cp->name))
			return (cp->path);
		cp = cp->next;
	}
	return (NULL);
}

static void	signal_inh(int sign)
{
	if (sign == SIGINT)
		ft_printf("\n$> ");
	if (sign == SIGQUIT)
		exit(0);
}

void		ft_freestrstr(char **str)
{
	char	*temp;
	char	**cp;

	if (str)
	{
		cp = str;
		while (*str)
		{
			temp = *str;
			str++;
			free(temp);
		}
		free(cp);
	}
}

void		shell(char **env, t_sh *table)
{
	char	*line;
	char	**cmd;
	char	*temp;
	char	**cp;

	line = NULL;
	signal(SIGINT, signal_inh);
	while (1)
	{
		ft_printf("$> ");
		if (get_next_line(1, &line) > 0)
		{
			cmd = ft_strsplit(line, ';');
			cp = cmd;
			while (*cmd)
			{
				pipes(*cmd, no_pipe(*cmd), &env, table);
				temp = *cmd;
				cmd++;
				free(temp);
			}
			free(cp);
			free(line);
		}
	}
}
