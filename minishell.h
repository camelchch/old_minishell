/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saxiao <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/31 12:26:55 by saxiao            #+#    #+#             */
/*   Updated: 2018/06/25 11:40:48 by saxiao           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <limits.h>
# include <sys/types.h>
# include "libft/libft.h"

# define BUF_MAX 2048

typedef struct		s_table
{
	char			name[NAME_MAX + 1];
	char			path[PATH_MAX + 1];
	struct s_table	*next;
}					t_table;

typedef struct		s_shtable
{
	t_table			*sh_ta;
}					t_sh;

typedef struct		s_vari
{
	int				in;
	int				num_pipe;
	int				pipe_fd[2];
	int				ct;
	pid_t			pid;
}					t_vari;

char				*get_autoline(t_sh *table);
char				**path(char **env);
char				*path_in_sh(char *app, t_sh *table);
int					calcu_index(char *name);
int					is_buildin(char *app);
int					cd(char **paras, char ***env);
void				replace_home(char *cp, char *home);
char				**unset_env(char **paras, char **env);
void				do_build(char **paras, char ***env, t_sh *table);
void				put_env(char **env, char **paras, t_sh *table);
char				*ft_getenv(char **env, char *name);
void				init_shtable(t_sh *table, char **path);
t_table				*malloc_add_sh(void);
void				shell(char **env, t_sh *table);
void				child_pro(char **paras, char **env, t_sh *table);
void				pipes(char *cmdline, int nb_pipe, char ***env, t_sh *table);
int					no_pipe(char *cmdline);
int					nb_str(char **str);
void				ft_freestrstr(char **str);
void				free_sh_table(t_sh *table, int index);
void				put_strstr(char **str);
int					set_env(char **paras, char ***env);
void				update_shlvl(char ***env);
void				update_lastapp(char *lastcmd, char ***env);
#endif
