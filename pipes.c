/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 11:49:36 by saxiao            #+#    #+#             */
/*   Updated: 2018/06/22 15:18:26 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"
#include "minishell.h"

static void	inside_child(int in, int nb_pipe, int *pipe_fd, int ct)
{
	if (in)
	{
		if (dup2(in, 0) < 0)
			ft_printf("dup2 failed \n");
		close(in);
	}
	if (ct != nb_pipe)
		dup2(pipe_fd[1], 1);
}

static void	first_is_buildin(int in, char **papra, char ***env, t_sh *table)
{
	if (in)
	{
		if (dup2(in, 0) < 0)
			ft_printf("dup2 failed \n");
		close(in);
	}
	do_build(papra, env, table);
	ft_freestrstr(papra);
}

static void	init_saver(int nb_pipe, t_vari *saver)
{
	saver->in = 0;
	saver->ct = -1;
	saver->num_pipe = nb_pipe;
}

static void	parent_child(t_vari *saver, char **papra, char ***env, t_sh *table)
{
	if (saver->num_pipe)
		pipe(saver->pipe_fd);
	saver->pid = fork();
	if (saver->pid < 0)
		exit(1);
	else if (saver->pid == 0)
	{
		inside_child(saver->in, saver->num_pipe, saver->pipe_fd, saver->ct);
		if (is_buildin(*papra))
			do_build(papra, env, table);
		else
			child_pro(papra, *env, table);
	}
	else
	{
		waitpid(saver->pid, NULL, 0);
		saver->in = saver->pipe_fd[0];
		if (saver->num_pipe)
			close(saver->pipe_fd[1]);
	}
	ft_freestrstr(papra);
}

void		pipes(char *cmdline, int nb_pipe, char ***env, t_sh *table)
{
	char	**cmds;
	char	**papra;
	t_vari	saver;

	cmds = ft_strsplit(cmdline, '|');
	init_saver(nb_pipe, &saver);
	while (++saver.ct <= nb_pipe)
	{
		papra = ft_split(cmds[saver.ct], " \t");
		if (saver.ct == nb_pipe && *papra && is_buildin(*papra))
			first_is_buildin(saver.in, papra, env, table);
		else
			parent_child(&saver, papra, env, table);
	}
	ft_freestrstr(cmds);
}
