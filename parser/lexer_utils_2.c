/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:00:39 by francesca         #+#    #+#             */
/*   Updated: 2025/07/13 09:22:51 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
 * Verifica se un carattere è uno spazio bianco valido.
 * Sono considerati spazi:
 *   - spazio ' '
 *   - tabulazione '\t'
 *   - newline '\n'
 *   - vertical tab '\v'
 *   - form feed '\f'
 *   - carriage return '\r'
 *
 * Ritorna:
 * - 1 se il carattere è uno spazio
 * - 0 altrimenti
 */
int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

/*
 * Verifica se un carattere è un metacarattere della shell.
 * Sono considerati metacaratteri:
 *   - '|': pipe
 *   - '<': redirezione input
 *   - '>': redirezione output
 *
 * Ritorna:
 * - 1 se il carattere è un metacarattere
 * - 0 altrimenti
 */
int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	free_tokens_struct(t_token *tokens_struct)
{
	if (!tokens_struct)
		return ;
	if (tokens_struct->tokens)
		free_matrix(tokens_struct->tokens);
	if (tokens_struct->token_types)
		free(tokens_struct->token_types);
	free(tokens_struct);
}

void	clear_tokens_content(t_token *tokens_struct)
{
	if (!tokens_struct)
		return ;
	if (tokens_struct->tokens)
	{
		free_matrix(tokens_struct->tokens);
		tokens_struct->tokens = NULL;
	}
	if (tokens_struct->token_types)
	{
		free(tokens_struct->token_types);
		tokens_struct->token_types = NULL;
	}
	tokens_struct->num_tokens = 0;
}

int	fill_tokens(char *line, t_token *tokens_struct)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		i = skip_spaces(line, i);
		if (!line[i])
			break ;
		if (line[i] == '|')
		{
			tokens_struct->tokens[count] = ft_strdup("|");
			tokens_struct->token_types[count++] = PIPE;
			i++;
		}
		else if (line[i] == '<' || line[i] == '>')
			i = handle_redirection(line, i, tokens_struct, &count);
		else
			i = handle_word(line, i, tokens_struct, &count);
		if (i == -1)
			return (-1);
	}
	tokens_struct->num_tokens = count;
	return (count);
}
