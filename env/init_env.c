/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:30:00 by francesca         #+#    #+#             */
/*   Updated: 2025/07/24 22:19:58 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// stampa solo se contiene '='
int	export_myenvp(char **my_env)
{
	int	i;

	g_exit_status = 0;
	i = 0;
	while (my_env[i])
	{
		if (ft_strchr(my_env[i], '='))
			printf("%s\n", my_env[i]);
		i++;
	}
	return (1);
}

void	free_myenvp(char **my_env)
{
	int	i;

	i = 0;
	while (my_env[i])
	{
		free(my_env[i]);
		i++;
	}
	free(my_env);
}

static int	new_env_len(char **envp)
{
	int	len;

	len = 0;
	while (envp[len])
		len++;
	return (len);
}

char	**copy_env(char **envp)
{
	int		i;
	char	**my_env;
	int		len;

	len = new_env_len(envp);
	i = 0;
	my_env = ft_calloc(len + 1, sizeof(char *));
	if (!my_env)
		return (NULL);
	while (i < len)
	{
		my_env[i] = ft_strdup(envp[i]);
		if (!my_env[i])
		{
			while (i-- >= 0)
				free(my_env[i]);
			free(my_env);
			return (NULL);
		}
		i++;
	}
	my_env[i] = NULL;
	return (my_env);
}
