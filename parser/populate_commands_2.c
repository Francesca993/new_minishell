/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_commands_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:26:13 by francesca         #+#    #+#             */
/*   Updated: 2025/07/22 17:28:17 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/execution.h"
#include "../header/minishell.h"

void	handle_pipe(t_pipeline *pipeline, t_cmd *cmd, int *i)
{
	if (*i < pipeline->n_tokens && pipeline->types[*i] == PIPE)
	{
		cmd->pipe = 1;
		(*i)++;
	}
}
