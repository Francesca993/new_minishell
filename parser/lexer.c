/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:03:26 by francesca         #+#    #+#             */
/*   Updated: 2025/07/12 22:33:07 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
 * Funzione principale del lexer: analizza una linea di input e produce
 * un array di token e un array parallelo con i tipi corrispondenti.
 *
 * Flusso:
 * 1. Conta quanti token saranno presenti (con `count_token()`).
 * 2. Alloca dinamicamente due array:
 *    - `tokens`: array di stringhe per ogni token
 *    - `types`: array di `t_token_type` con i tipi (es. WORD, PIPE, ecc.)
 * 3. Chiama `fill_tokens()` per popolare gli array.
 * 4. In caso di errore (es. quote non chiusa), stampa un messaggio,
 *		imposta il codice di uscita globale (`g_exit_status = 2`) e libera
 *    la memoria.
 *
 * Parametri:
 * - line: la linea di input da analizzare
 * - tokens_out: doppio puntatore dove salvare l'array dei token estratti
 * - types_out: doppio puntatore dove salvare l'array dei tipi corrispondenti
 *
 * Ritorna:
 * - Numero di token trovati e validi
 * - -1 in caso di errore (es. errore di malloc o quote non chiusa)
 */
int	lexer(char *line, t_token *tokens_struct)
{
	int	filled;

	tokens_struct->num_tokens = count_token(line);
	if (tokens_struct->num_tokens <= 0)
		return (-1);
	tokens_struct->tokens = (char **)ft_calloc(tokens_struct->num_tokens + 1,
			sizeof(char *));
	tokens_struct->token_types = ft_calloc(tokens_struct->num_tokens,
			sizeof(t_token_type));
	if (!tokens_struct->tokens || !tokens_struct->token_types)
	{
		exit_shell(1, NULL);
		return (-1);
	}
	filled = fill_tokens(line, tokens_struct);
	if (filled == -1)
	{
		clear_tokens_content(tokens_struct);
		g_exit_status = 2;
		return (-1);
	}
	return (filled);
}
