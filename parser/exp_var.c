/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 13:33:23 by skayed            #+#    #+#             */
/*   Updated: 2025/07/17 13:58:57 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// Funzione per ottenere la lunghezza del valore di una variabile dall'ambiente
int	get_env_value_len(const char *var_name, char **env)
{
	char	*tmp;
	int		len;
	int		i;

	if (!var_name || !env || (ft_strlen(var_name) == 0))
		return (0);
	if (ft_strcmp(var_name, "?") == 0)
	{
		tmp = ft_itoa(g_exit_status);
		len = ft_strlen(tmp);
		return (free(tmp), len);
	}
	if (ft_strcmp(var_name, "0") == 0)
		return (ft_strlen("minishell"));
	if (ft_strcmp(var_name, "$") == 0)
		return (1);
	len = ft_strlen(var_name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
			return (ft_strlen(env[i] + len + 1));
		i++;
	}
	return (0);
}

// Funzione per calcolare la lunghezza della stringa espansa
int	compute_expanded_length(const char *str, char **env)
{
	int		len;
	char	*var_name;
	int		i;
	int		var_len;

	var_len = 0;
	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i
					+ 1] == '?' || str[i + 1] == '_' || str[i + 1] == '$'))
		{
			var_name = get_var_name(str + i, &var_len);
			len += get_env_value_len(var_name, env);
			free(var_name);
			i += var_len;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

// Funzione per espandere le variabili in tutti gli argomenti di un comando
void	expand_cmd_variables(t_cmd *cmd, char **env)
{
	int		i;
	char	*expanded;

	if (!cmd || !env)
		return ;
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '$') && !cmd->quote_single)
		{
			expanded = expand_variables(cmd->args[i], env);
			if (expanded)
			{
				free(cmd->args[i]);
				cmd->args[i] = expanded;
			}
		}
		i++;
	}
}

// Funzione per espandere le variabili in tutta la pipeline
void	expand_pipeline_variables(t_pipeline *pipeline)
{
	int	i;

	if (!pipeline)
		return ;
	i = 0;
	while (i < pipeline->n_cmds)
	{
		if (pipeline->cmds[i]->dollar)
			expand_cmd_variables(pipeline->cmds[i], pipeline->my_env);
		i++;
	}
}
/*
void	check_var(t_pipeline *pipeline)
{
	int		i;
	int		j;
	char	*expanded;

	i = 0;
	while (i < pipeline->n_cmds)
	{
		if (pipeline->cmds[i]->dollar)
		{
			j = 0;
			while (pipeline->cmds[i]->args[j] != NULL)
			{
				if (ft_strchr(pipeline->cmds[i]->args[j], '$') &&
					!pipeline->cmds[i]->quote_single)
				{
					expanded = expand_variables(pipeline->cmds[i]->args[j],
												pipeline->my_env);
					free(pipeline->cmds[i]->args[j]);
					pipeline->cmds[i]->args[j] = expanded;
				}
				j++;
			}
		}
		i++;
	}
} */