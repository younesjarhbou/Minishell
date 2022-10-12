/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaggoujj <aaggoujj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 14:30:39 by aaggoujj          #+#    #+#             */
/*   Updated: 2022/10/12 18:06:30 by aaggoujj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_revsplit(char **str, char sep)
{
	char	*new;
	int		len;
	int		i;

	i = -1;
	len = 0;
	new = NULL;
	if (!str)
		return (NULL);
	while (str[++i])
		new = ft_join_sep(str[i], new, sep);
	return (new);
}

t_env	*search_env2(char *name, t_list	*lst)
{
	t_env	*e;

	if (!lst)
		return (NULL);
	while (lst)
	{
		e = lst->content;
		if (ft_strcmp(e->name, name) == 0)
			return (e);
		lst = lst->next;
	}
	return (NULL);
}

void	update_pwd(t_data *data, char *pwd)
{
	t_env	*e;
	t_env	*old;

	free(pwd);
	pwd = getcwd(NULL, 0);
	e = search_env2("PWD", data->envp);
	if (e && ft_strcmp(pwd, e->value) != 0)
	{
		old = search_env2("OLDPWD", data->envp);
		free(old->value);
		if (e->value == NULL)
			old->value = ft_strdup("");
		else
			old->value = ft_strdup(e->value);
		if (pwd == NULL)
			e->value = ft_strjoin2(e->value, "/..");
		else
		{
			free(e->value);
			e->value = ft_strdup(pwd);
		}
	}
	g_exitstatus = 0;
	free(pwd);
}

char	*last_args(char **args)
{
	int	i;

	i = 0;
	while (args[i + 1])
		i++;
	return (args[i]);
}

void	update_underscore(t_data *data, char **args)
{
	t_env	*e;

	e = NULL;
	if (!ft_strcmp(last_args(args), "$_"))
		return ;
	e = search_env2("_", data->envp);
	if (e)
	{
		if (e->value && e->value[0] != '\0')
			free(e->value);
		e->value = ft_strdup(last_args(args));
	}
	else
	{
		e = malloc(sizeof(t_env));
		e->name = ft_strdup("_");
		e->value = ft_strdup(last_args(args));
		ft_lstadd_back(&data->envp, ft_lstnew(e));
	}
}
