/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:27:42 by francesca         #+#    #+#             */
/*   Updated: 2025/07/12 22:00:31 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
// ==============================
// DEBUG
// ==============================
*/
// void	print_tokens(char **tokens, t_token_type *types)
// {
// 	int	i;

// 	i = 0;
// 	if (!tokens || !tokens[0])
// 	{
// 		printf("=== TOKENS ===\n(nessun token trovato)\n==============\n");
// 		return ;
// 	}
// 	printf("=== TOKENS ===\n");
// 	while (tokens[i])
// 	{
// 		printf("[%s] -> ", tokens[i]);
// 		switch (types[i])
// 		{
// 		case WORD:
// 			printf("WORD\n");
// 			break ;
// 		case PIPE:
// 			printf("PIPE\n");
// 			break ;
// 		case REDIR_IN:
// 			printf("REDIR_IN (<)\n");
// 			break ;
// 		case REDIR_OUT:
// 			printf("REDIR_OUT (>)\n");
// 			break ;
// 		case APPEND:
// 			printf("APPEND (>>)\n");
// 			break ;
// 		case HEREDOC:
// 			printf("HEREDOC (<<)\n");
// 			break ;
// 		default:
// 			printf("UNKNOWN\n");
// 			break ;
// 		}
// 		i++;
// 	}
// 	printf("==============\n");
// }
// void	print_pipeline(t_pipeline *pipeline)
// {
// 	t_cmd	*cmd;

// 	printf("=== PIPELINE ===\n");
// 	for (int i = 0; i < pipeline->n_cmds; i++)
// 	{
// 		cmd = pipeline->cmds[i];
// 		if (!cmd)
// 		{
// 			printf("Command %d: (null)\n", i + 1);
// 			continue ;
// 		}
// 		printf("Command %d:\n", i + 1);
// 		printf("  args: ");
// 		if (cmd->args)
// 		{
// 			for (int j = 0; cmd->args[j]; j++)
// 				printf("[%s] ", cmd->args[j]);
// 			printf("\n");
// 		}
// 		else
// 		{
// 			printf("(none)\n");
// 		}
// 		if (cmd->infile)
// 			printf("  infile: %s (%s)\n", cmd->infile,
// 				cmd->heredoc ? "HEREDOC" : "REDIR_IN");
// 		if (cmd->outfile)
// 			printf("  outfile: %s (%s)\n", cmd->outfile,
// 				cmd->append ? "APPEND" : "REDIR_OUT");
// 		if (cmd->pipe)
// 			printf("  pipe: yes\n");
// 		printf("------------------------\n");
// 	}
// 	printf("=== END OF PIPELINE ===\n");
// }
/*
 * Funzione principale del parser: riceve una linea di input e la converte
 * in una struttura `t_pipeline`, pronta per l'esecuzione.
 *
 * Flusso:
 * 1. Chiama `lexer()` per suddividere la linea in token e tipi.
 * 2. Se il lexer fallisce (es. quote non chiusa), stampa un errore e termina.
 * 3. Stampa i token generati (solo per debug).
 * 4. Chiama `build_pipeline()` per costruire la struttura `t_pipeline`,
 *    che conterrà i comandi, le redirezioni, le pipe, ecc.
 * 5. Se la costruzione fallisce, stampa un errore e termina.
 * 6. Stampa la pipeline risultante (debug) e la restituisce.
 *
 * Parametri:
 * - line: linea di input da analizzare
 * - env: ambiente (ignorato per ora)
 *
 * Ritorna:
 * - Puntatore a una `t_pipeline` allocata dinamicamente
 * - NULL in caso di errore (lexer o parser)
 */
t_pipeline	*parse_line(char *line, char **env, t_pipeline *pipeline)
{
	t_token	*tokens_s;

	tokens_s = ft_calloc(1, sizeof(t_token));
	if (!tokens_s)
		return (NULL);
	tokens_s->num_tokens = lexer(line, tokens_s);
	if (tokens_s->num_tokens == 0 || !tokens_s->tokens || !tokens_s->tokens[0]
		|| tokens_s->num_tokens == -1 || !line
		|| !check_syntax(tokens_s->tokens, tokens_s->token_types,
			tokens_s->num_tokens))
		return (free_tokens_struct(tokens_s), NULL);
	pipeline = build_pipeline(tokens_s->tokens, tokens_s->token_types,
			tokens_s->num_tokens, env);
	free(tokens_s);
	if (!pipeline)
	{
		g_exit_status = 2;
		free_tokens_struct(tokens_s);
		return (NULL);
	}
	return (pipeline);
}
