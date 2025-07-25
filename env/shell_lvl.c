/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lvl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 22:19:45 by skayed            #+#    #+#             */
/*   Updated: 2025/07/25 10:40:26 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static char	**add_new_var(char **my_env, const char *entry)
{
	int		len;
	char	**new_env;
	int		i;

	len = 0;
	i = 0;
	while (my_env && my_env[len])
		len++;
	new_env = ft_calloc(len + 2, sizeof(char *));
	if (!new_env)
		return (my_env);
	while (i < len)
	{
		new_env[i] = my_env[i];
		i++;
	}
	new_env[len] = ft_strdup(entry);
	new_env[len + 1] = NULL;
	free(my_env);
	return (new_env);
}

static int	is_numeric_str(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

/*  Restituisce l’indice della variabile SHLVL, oppure -1 se non esiste   */
static int	get_shlvl_index(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
			return (i);
		i++;
	}
	return (-1);
}

/*  Aggiorna SHLVL in env[idx] portandolo a +1, o lo imposta a 1 se      */
/*  non è numerico. ‑‑ Tutto resta sotto le 25 righe.                    */
static void	update_shlvl_value(char **env, int idx)
{
	char	*lvl_str;
	int		lvl;
	char	*tmp;

	lvl_str = env[idx] + 6;
	if (*lvl_str && is_numeric_str(lvl_str))
	{
		lvl = ft_atoi(lvl_str) + 1;
		tmp = ft_itoa(lvl);
		if (!tmp)
			return ;
	}
	else
		tmp = ft_strdup("1");
	if (!tmp)
		return ;
	free(env[idx]);
	env[idx] = ft_strjoin("SHLVL=", tmp);
	free(tmp);
}

/*  Funzione principale: massimo 10 righe                                 */
void	increment_shlvl(char ***envp)
{
	int	idx;

	idx = get_shlvl_index(*envp);
	if (idx >= 0)
		update_shlvl_value(*envp, idx);
	else
		*envp = add_new_var(*envp, "SHLVL=1");
}

/*
void	increment_shlvl(char ***envp)
{
	char	**env;
	int		i;
	char	*lvl_str;
	int		lvl;
	char	*tmp;

	env = *envp;
	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
		{
			lvl_str = env[i] + 6;
			if (*lvl_str && is_numeric_str(lvl_str))
			{
				lvl = ft_atoi(lvl_str) + 1;
				tmp = ft_itoa(lvl);
				if (!tmp)
					return ;
				free(env[i]);
				env[i] = ft_strjoin("SHLVL=", tmp);
				free(tmp);
				if (!env[i])
					return ;
				return ;
			}
			free(env[i]);
			env[i] = ft_strdup("SHLVL=1");
			return ;
		}
		i++;
	}
	*envp = add_new_var(env, "SHLVL=1");
}
*/