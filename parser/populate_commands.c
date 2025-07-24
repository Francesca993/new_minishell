/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:18:25 by francesca         #+#    #+#             */
/*   Updated: 2025/07/22 17:27:53 by francesca        ###   ########.fr       */
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

static void	handle_redir_in(t_pipeline *p, t_cmd *cmd, int *i)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup(p->tokens[++(*i)]);
	cmd->redir_in = 1;
}

void	handle_command_redirection(t_pipeline *p, t_cmd *cmd, int *i)
{
	t_token_type	type;

	type = p->types[*i];
	if ((*i + 1) >= p->n_tokens)
		return ;
	if (type == REDIR_IN)
		handle_redir_in(p, cmd, i);
	else if (type == REDIR_OUT)
	{
		cmd->outfile = ft_strdup(p->tokens[++(*i)]);
		cmd->redir_out = 1;
	}
	else if (type == APPEND)
	{
		cmd->outfile = ft_strdup(p->tokens[++(*i)]);
		cmd->append = 1;
	}
	else if (type == HEREDOC)
	{
		cmd->infile = ft_strdup(p->tokens[++(*i)]);
		cmd->heredoc = 1;
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
