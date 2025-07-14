/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:14:07 by francesca         #+#    #+#             */
/*   Updated: 2025/07/13 09:21:58 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
 * Gestisce la creazione di un token di redirezione a partire da `line[i]`.
 * Riconosce:
 *   - '>'     → REDIR_OUT
 *   - '>>'    → APPEND
 *   - '<'     → REDIR_IN
 *   - '<<'    → HEREDOC
 *
 * Scrive il token corrispondente in `tokens[*count]` e aggiorna il tipo.
 *
 * Parametri:
 * - line: linea originale
 * - i: indice corrente nella linea
 * - tokens: array dei token
 * - types: array dei tipi di token
 * - count: indice del token corrente (passato per riferimento)
 *
 * Ritorna:
 * - Nuova posizione `i` dopo aver letto il token di redirezione
 */
int	handle_redirection(const char *line, int i, t_token *tokens_struct,
		int *count)
{
	char	c;

	c = line[i++];
	if (line[i] == c)
	{
		tokens_struct->tokens[*count] = ft_substr(line, i - 1, 2);
		if (c == '>')
			tokens_struct->token_types[*count] = APPEND;
		else
			tokens_struct->token_types[*count] = HEREDOC;
		i++;
	}
	else
	{
		tokens_struct->tokens[*count] = ft_substr(line, i - 1, 1);
		if (c == '>')
			tokens_struct->token_types[*count] = REDIR_OUT;
		else
			tokens_struct->token_types[*count] = REDIR_IN;
	}
	(*count)++;
	return (i);
}

/*
 * Gestisce la gestione delle quote all'interno di una parola.
 * Ritorna -1 se trova una quote non chiusa,
	altrimenti ritorna la nuova posizione.
 */
static int	handle_quotes(const char *line, int i, char *quote, int escaped)
{
	if ((line[i] == '"' || line[i] == '\'') && !*quote)
	{
		if (!(line[i] == '"' && escaped))
			*quote = line[i];
	}
	else if (line[i] == *quote && !(*quote == '"' && escaped))
		*quote = 0;
	return (i);
}

/*
 * Processa un singolo carattere all'interno di una parola.
 * Gestisce backslash e ritorna -1 in caso di errore.
 */
static int	process_word_char(const char *line, int i)
{
	if (line[i] == '\\')
	{
		if (!line[i + 1])
			return (exit_shell(2, "Error backslah not closed \n"), -1);
		if ((line[i + 1] == ' ' || line[i + 1] == '\0') && (!line[i + 2]
				|| ft_isspace(line[i + 2])))
			return (exit_shell(2, "Error backslah not closed \n"), -1);
		i++;
	}
	return (i);
}

/*
 * Gestisce un token di tipo WORD o stringa tra quote.
 *
 * Un token è considerato una parola finché:
 *   - non è un metacarattere
 *   - non è uno spazio
 *   - oppure finché si è dentro a delle quote (singole o doppie)
 *
 * Se viene trovata una quote non chiusa, ritorna -1 per segnalare errore.
 * In caso contrario, salva il token e aggiorna `types[*count]`.
 *
 * Parametri:
 * - line: linea di input
 * - i: indice iniziale
 * - tokens: array di stringhe da riempire
 * - types: array parallelo dei tipi
 * - count: puntatore al numero corrente di token
 *
 * Ritorna:
 * - Nuovo indice i dopo il token
 * - -1 in caso di errore (quote non chiusa)
 */
int	handle_word(const char *line, int i, t_token *tokens_struct, int *count)
{
	int		start;
	char	quote;
	int		escaped;

	start = i;
	quote = 0;
	escaped = 0;
	while (line[i] && (quote || (!is_metachar(line[i])
				&& !ft_isspace(line[i]))))
	{
		escaped = (i > 0 && line[i - 1] == '\\');
		i = handle_quotes(line, i, &quote, escaped);
		if (i == -1)
			return (-1);
		i = process_word_char(line, i);
		if (i == -1)
			return (-1);
		i++;
	}
	if (quote)
		return (exit_shell(2, "syntax error: unclosed quote \n"), -1);
	tokens_struct->tokens[*count] = ft_substr(line, start, i - start);
	tokens_struct->token_types[*count] = WORD;
	(*count)++;
	return (i);
}
