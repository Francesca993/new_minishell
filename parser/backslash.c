/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backslash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:50:39 by skayed            #+#    #+#             */
/*   Updated: 2025/07/24 14:53:54 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static size_t	count_consecutive_backslashes(const char *str, size_t *i)
{
	size_t	count;

	count = 0;
	while (str[*i] == '\\')
	{
		count++;
		(*i)++;
	}
	return (count);
}

// conserva un solo backslash
static void	handle_backslashes_outside_quotes(size_t count, char *res,
		size_t *j)
{
	if (count > 1)
		res[(*j)++] = '\\';
}

char	*handle_backslash_outside_quotes(const char *str)
{
	size_t	len;
	char	*res;
	size_t	i;
	size_t	j;
	size_t	count;

	len = ft_strlen(str);
	res = ft_calloc(len + 1, sizeof(char));
	i = 0;
	j = 0;
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\')
		{
			count = count_consecutive_backslashes(str, &i);
			handle_backslashes_outside_quotes(count, res, &j);
		}
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

int	check_backslash_error(const char *line, int i)
{
	if (!line[i + 1])
		return (exit_shell(2, "Error backslash not closed\n"), -1);
	if ((line[i + 1] == ' ' || line[i + 1] == '\0') && (!line[i + 2]
			|| ft_isspace(line[i + 2])))
		return (exit_shell(2, "Error backslash not closed\n"), -1);
	return (1);
}
