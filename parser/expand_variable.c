/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:57:34 by francesca         #+#    #+#             */
/*   Updated: 2025/07/24 14:55:01 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static int	handle_variable(const char *str, char **env, char *result, int *i)
{
	int		var_len;
	char	*var_name;
	char	*var_value;
	int		written;
	int		v;

	written = 0;
	var_name = get_var_name(str + *i, &var_len);
	if (var_len == -1 || !var_name)
		return (-1);
	var_value = get_env_value(var_name, env);
	free(var_name);
	if (var_value)
	{
		v = 0;
		while (var_value[v])
			result[written++] = var_value[v++];
		free(var_value);
	}
	*i += var_len;
	return (written);
}

static int	is_variable_start(const char *str, int i)
{
	return (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i
				+ 1] == '?' || str[i + 1] == '_' || str[i + 1] == '$' || str[i
				+ 1] == '{'));
}

static int	expand_loop(const char *str, char **env, char *result)
{
	int	i;
	int	res_i;
	int	written;

	i = 0;
	res_i = 0;
	while (str[i])
	{
		if (is_variable_start(str, i))
		{
			written = handle_variable(str, env, result + res_i, &i);
			if (written == -1)
				return (-1);
			res_i += written;
		}
		else
			result[res_i++] = str[i++];
	}
	result[res_i] = '\0';
	return (0);
}

char	*expand_variables(const char *str, char **env)
{
	int		out_len;
	char	*result;

	out_len = compute_expanded_length(str, env);
	result = ft_calloc(out_len + 1, sizeof(char));
	if (!result)
		return (NULL);
	if (expand_loop(str, env, result) == -1)
	{
		free(result);
		return (NULL);
	}
	return (result);
}
