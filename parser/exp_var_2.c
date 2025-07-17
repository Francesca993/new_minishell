/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_var_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:23:20 by francesca         #+#    #+#             */
/*   Updated: 2025/07/17 13:55:29 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// Funzione di supporto per il caso ${VAR}
static char	*get_braced_var(const char *str, int *len)
{
	int	i;

	i = 2;
	while (str[i] && str[i] != '}')
		i++;
	if (str[i] != '}')
	{
		*len = -1;
		return (NULL);
	}
	*len = i + 1;
	return (ft_substr(str, 2, i - 2));
}

// Funzione per ottenere il nome della variabile da una stringa
char	*get_var_name(const char *str, int *len)
{
	int	i;

	if (str[0] == '$' && str[1] == '{')
		return (get_braced_var(str, len));
	if (str[1] == '?' || str[1] == '0')
	{
		*len = 2;
		return (ft_substr(str, 1, 1));
	}
	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i == 1)
	{
		*len = 1;
		return (ft_strdup("$"));
	}
	*len = i;
	return (ft_substr(str, 1, i - 1));
}

// Gestisce i casi speciali: ?, 0, $
static char	*get_special_env_value(const char *var_name)
{
	char	*tmp;
	char	*ret;

	if (ft_strcmp(var_name, "?") == 0)
	{
		tmp = ft_itoa(g_exit_status);
		ret = ft_strdup(tmp);
		free(tmp);
		return (ret);
	}
	if (ft_strcmp(var_name, "0") == 0)
		return (ft_strdup("minishell"));
	if (ft_strcmp(var_name, "$") == 0)
		return (ft_strdup("$"));
	return (NULL);
}

// Cerca la variabile nell'ambiente
static char	*find_env_value(const char *var_name, char **env)
{
	int	i;
	int	len;

	len = ft_strlen(var_name);
	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

// Funzione principale per ottenere il valore di una variabile dall'ambiente
char	*get_env_value(const char *var_name, char **env)
{
	char	*special;

	if (!var_name || !env)
		return (NULL);
	if (ft_strlen(var_name) == 0)
		return (ft_strdup(""));
	special = get_special_env_value(var_name);
	if (special)
		return (special);
	return (find_env_value(var_name, env));
}
// Funzione per ottenere il valore di una variabile dall'ambiente
// static char	*get_env_value(const char *var_name, char **env)
// {
// 	int		i;
// 	int		len;
// 	char	*value;
// 	char	*tmp;
// 	char	*ret;

// 	if (!var_name || !env)
// 		return (NULL);
// 	if (ft_strlen(var_name) == 0)
// 		return (ft_strdup(""));
// 	if (ft_strcmp(var_name, "?") == 0)
// 	{
// 		tmp = ft_itoa(g_exit_status);
// 		ret = ft_strdup(tmp);
// 		free(tmp);
// 		return (ret);
// 	}
// 	if (ft_strcmp(var_name, "0") == 0)
// 		return (ft_strdup("minishell"));
// 	if (ft_strcmp(var_name, "$") == 0)
// 		return (ft_strdup("$"));
// 	len = ft_strlen(var_name);
// 	i = 0;
// 	while (env[i])
// 	{
// 		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
// 		{
// 			value = ft_strdup(env[i] + len + 1);
// 			return (value);
// 		}
// 		i++;
// 	}
// 	return (ft_strdup(""));
// }