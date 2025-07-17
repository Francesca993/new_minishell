/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:57:34 by francesca         #+#    #+#             */
/*   Updated: 2025/07/17 14:00:30 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// char	*expand_variables(const char *str, char **env)
// {
// 	int		i;
// 	int		var_len;
// 	char	*var_name;
// 	char	*var_value;
// 	int		out_len;
// 	char	*result;
// 	int		res_i;
// 	int		v;

// 	i = 0;
// 	out_len = compute_expanded_length(str, env);
// 	result = ft_calloc(out_len + 1, sizeof(char));
// 	if (!result)
// 		return (NULL);
// 	res_i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i
// 				+ 1] == '?' || str[i + 1] == '_' || str[i + 1] == '$' || str[i
// 				+ 1] == '{'))
// 		{
// 			var_name = get_var_name(str + i, &var_len);
// 			if (var_len == -1 || !var_name)
// 			{
// 				free(result);
// 				return (NULL);
// 			}
// 			var_value = get_env_value(var_name, env);
// 			free(var_name);
// 			if (var_value)
// 			{
// 				v = 0;
// 				while (var_value[v])
// 					result[res_i++] = var_value[v++];
// 				free(var_value);
// 			}
// 			i += var_len;
// 		}
// 		else
// 		{
// 			result[res_i++] = str[i++];
// 		}
// 	}
// 	result[res_i] = '\0';
// 	return (result);
// }
