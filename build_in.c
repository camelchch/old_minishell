#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

int		is_buildin(char *app)
{
	return (!strcmp(app, "echo") || !strcmp(app, "cd") ||\
			!strcmp(app, "setenv") || !strcmp(app, "unsetenv") ||\
			!strcmp(app, "env") || !strcmp(app, "exit"));
}

int		nb_str(char **paras)
{
	int		ct;

	ct = 0;
	while (*paras++)
		ct++;
	return (ct);
}

void	echo(char **paras)
{
	int		ct;

	ct = nb_str(paras);
	paras++;
	while (*paras)
	{
		ft_printf("%s", *paras++);
		if (ct-- > 2)
			ft_printf(" ");
		else
			ft_printf("\n");
	}
}

void	ft_exit(void)
{
	exit(0);
}


void	add_env(char ***env, char **paras)
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
		//(*env)[i++] = (*cp++);
	(*env)[i] = ft_strjoin(*paras, "=");
	paras++;
	temp = (*env)[i];
	(*env)[i] = ft_strjoin((*env)[i], *paras);
	free(temp);
	(*env)[++i] = NULL;
	ft_freestrstr(for_free);
}

char	**delet_env(char **env, int index)
{
	int		after;
	char	**temp;
	//put_env(*env);
	temp = env;
	free(temp[index]);
	while (index < nb_str(env) - 1)
	{
		after = index + 1;
		temp[index++] = temp[after];
		//		free(temp);
	}
	free(temp[nb_str(env) - 2]);
	temp[nb_str(env) - 2] = NULL;
	free(temp[nb_str(env) - 1]);
	temp[nb_str(env) - 1] = NULL;
	return (temp);
	//	free(cp);
}

void	set_env(char **paras, char ***env)
{
	char	**cp;
	char	*temp;

	cp = *env;
	paras++;
	while (*cp && !(!ft_strncmp(*paras, *cp, ft_strlen(*paras)) && ft_strlen(*paras) < ft_strlen(*cp) && (*cp)[ft_strlen(*paras)] == '='))
		cp++;
	if (*cp)
	{
		temp = *cp;
		*cp = ft_strjoin(*paras++, "=");
		free(temp);
		temp = *cp;
		*cp = ft_strjoin(*cp, *paras);
		free(temp);
	}
	else
		add_env(env, paras);
}

void	init_tempwd(char **tempwd, int ct, char ***paras, char *path)
{
	//char	*temp;
	(void)ct;
	(void)paras;

	tempwd[0] = "no real meaning here";
	tempwd[1] = "OLDPWD";
	tempwd[2] = getcwd(path, PATH_MAX + 1);
	tempwd[3] = NULL;
	/*	if (ct == 2)
		{
		temp = **paras;
		(*paras)++;
		free(temp);
		}
		*/
}

void		oldpwd_home(char **cp, char ***env, int ct)
{
	(void)ct;
	if (ct == 1 || ft_strcmp(*cp, "~"))
	{
		*cp = ft_getenv(*env, "HOME");
		if (!*cp)
			ft_printf("enviroment HOME is not set\n");
	}
	else
	{
		*cp = ft_getenv(*env, "OLDPWD");
		if (!*cp)
			ft_printf("enviroment OLDPWD  is not set\n");
	}
}

void	for_cd(char *paras, char ***env, char **tempwd, char *path)
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
			ft_printf("no such file or directory: %s\n", *paras);
		else if (access(paras, X_OK))
			ft_printf("permission denied\n");
	}
}

int		cd(char **paras, char ***env)
{
	int		ct;
	char	*tempwd[4];
	char	path[PATH_MAX + 1];
	char	*cp;

	ct = nb_str(paras);
	if (ct == 1)
		cp = paras[0];
	else
		cp = paras[1];
	if (ct != 1 && ct != 2)
	{
		ft_printf("Too many arguments--usage : cd path\n");
		return (1);
	}
	else
	{
		init_tempwd(tempwd, ct, &paras, path);
		if (ct == 1 || (ct == 2 && (!ft_strcmp(*(paras + 1), "~") || !ft_strcmp(*(paras + 1), "-"))))
			oldpwd_home(&cp, env, ct);
		for_cd(cp, env, tempwd, path);
		return (0);
	}
}

char	**unset_env(char **paras, char **env)
{
	char	**cp;
	//	char	*temp;
	int		index;

	cp = env;
	index = 0;
	paras++;
	while (*cp && !(!ft_strncmp(*paras, *cp, ft_strlen(*paras)) && ft_strlen(*paras) < ft_strlen(*cp) && (*cp)[ft_strlen(*paras)] == '='))
	{
		cp++;
		index++;
	}
	if (*cp)
		env = delet_env(env,  index);
	else
		ft_printf("no such variable %s\n", *paras);
	return (env);
}

void	do_build(char **paras, char ***env, t_sh *table)
{
	update_lastapp(*paras, env);
	if (!ft_strcmp(*paras, "cd"))
		cd(paras, env);
	else if (!ft_strcmp(*paras, "echo"))
		echo(paras);
	//else if (!ft_strcmp(*paras, "pwd"))
	//	pwd();
	else if (!ft_strcmp(*paras, "setenv") || !ft_strcmp(*paras, "unsetenv"))
	{
		if (!ft_strcmp(*paras, "unsetenv"))
			*env = unset_env(paras, *env);
		else
			set_env(paras, env);
		if (!ft_strcmp((*paras + 1), "PATH"))
			init_shtable(table, path(*env));
	}
	else if (!ft_strcmp(*paras, "env"))
		put_env(*env, paras, table);
	else if (!ft_strcmp(*paras, "exit"))
		ft_exit();
	//	ft_freestrstr(paras);
}

