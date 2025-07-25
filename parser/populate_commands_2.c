/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_commands_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:26:13 by francesca         #+#    #+#             */
/*   Updated: 2025/07/25 10:36:25 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/execution.h"
#include "../header/minishell.h"

static void	handle_redir_in(t_pipeline *p, t_cmd *cmd, int *i)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup(p->tokens[++(*i)]);
	if (!cmd->infile)
	{
		exit_shell(1, "Memory allocation failed\n");
		return ;
	}
	cmd->redir_in = 1;
}

static void	handle_redir_out(t_pipeline *p, t_cmd *cmd, int *i)
{
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup(p->tokens[++(*i)]);
	if (!cmd->outfile)
		return (exit_shell(1, "Memory allocation failed\n"));
	cmd->redir_out = 1;
}

static void	handle_append_out(t_pipeline *p, t_cmd *cmd, int *i)
{
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup(p->tokens[++(*i)]);
	if (!cmd->outfile)
		return (exit_shell(1, "Memory allocation failed\n"));
	cmd->append = 1;
}

static void	handle_heredoc(t_pipeline *p, t_cmd *cmd, int *i)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup(p->tokens[++(*i)]);
	if (!cmd->infile)
		return (exit_shell(1, "Memory allocation failed\n"));
	cmd->heredoc = 1;
}

void	handle_command_redirection(t_pipeline *p, t_cmd *cmd, int *i)
{
	t_token_type	type;

	if ((*i + 1) >= p->n_tokens)
		return ;
	type = p->types[*i];
	if (type == REDIR_IN)
		handle_redir_in(p, cmd, i);
	else if (type == REDIR_OUT)
		handle_redir_out(p, cmd, i);
	else if (type == APPEND)
		handle_append_out(p, cmd, i);
	else if (type == HEREDOC)
		handle_heredoc(p, cmd, i);
}

/*
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
		if (!cmd->outfile)
		{
			exit_shell(1, "Memory allocation failed\n");
			return ;
		}
		cmd->redir_out = 1;
	}
	else if (type == APPEND)
	{
		cmd->outfile = ft_strdup(p->tokens[++(*i)]);
		if (!cmd->outfile)
		{
			exit_shell(1, "Memory allocation failed\n");
			return ;
		}
		cmd->append = 1;
	}
	else if (type == HEREDOC)
	{
		cmd->infile = ft_strdup(p->tokens[++(*i)]);
		if (!cmd->infile)
		{
			exit_shell(1, "Memory allocation failed\n");
			return ;
		}
		cmd->heredoc = 1;
	}
}
*/