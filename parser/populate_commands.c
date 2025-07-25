/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:18:25 by francesca         #+#    #+#             */
/*   Updated: 2025/07/25 10:33:21 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/execution.h"
#include "../header/minishell.h"

/*
 * Popola l'array di comandi `pipeline->cmds` analizzando token e tipi.
 *
 * Per ogni comando:
 * - Conta i WORD per allocare `cmd->args`.
 * - Alloca e riempie la struttura `t_cmd`:
 *   - `args[]`: solo token di tipo WORD (duplicati con `ft_strdup`).
 *   - Redirezioni:
 *     - <    → `infile`, `redir_in`
 *     - >    → `outfile`, `redir_out`
 *     - >>   → `outfile`, `append`
 *     - <<   → `infile`, `heredoc`
 * - Se è presente una PIPE,
	imposta `cmd->pipe = 1` e passa al prossimo comando.
 *
 * Nota: termina se termina l'array di token.
 */
t_cmd	*allocate_cmd(t_pipeline *pipeline, int n_args)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(n_args + 1, sizeof(char *));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->pipeline = pipeline;
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	return (cmd);
}

void	handle_pipe(t_pipeline *pipeline, t_cmd *cmd, int *i)
{
	if (*i < pipeline->n_tokens && pipeline->types[*i] == PIPE)
	{
		cmd->pipe = 1;
		(*i)++;
	}
}

int	fill_cmds(t_pipeline *pipeline, t_cmd *cmd, int *i)
{
	int	arg_idx;

	arg_idx = 0;
	while (*i < pipeline->n_tokens && pipeline->types[*i] != PIPE)
	{
		if (pipeline->types[*i] == WORD)
			cmd->args[arg_idx++] = ft_strdup(pipeline->tokens[*i]);
		else
			handle_command_redirection(pipeline, cmd, i);
		(*i)++;
	}
	cmd->args[arg_idx] = NULL;
	return (0);
}

int	setup_redirections(t_cmd *cmd)
{
	if (cmd->redir_in && cmd->infile && setup_redir_in(cmd) == -1)
		return (1);
	if (cmd->redir_out && cmd->outfile && setup_redir_out(cmd) == -1)
		return (1);
	if (cmd->append && cmd->outfile && setup_redir_append(cmd) == -1)
		return (1);
	if (cmd->heredoc && cmd->infile && setup_heredoc(cmd, cmd->infile) == -1)
		return (1);
	return (0);
}
