/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:25:56 by skayed            #+#    #+#             */
/*   Updated: 2025/07/22 15:36:57 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

void	exec_here_child(int *pipe_fd, t_cmd *cmd, int exp_var, char *new_del)
{
	close(pipe_fd[0]);
	read_heredoc(cmd, pipe_fd[1], new_del, exp_var);
	close(pipe_fd[1]);
	free(new_del);
	exit(0);
}

void	handle_exp(char *line, t_cmd *cmd, int write_fd)
{
	char	*tmp;

	tmp = expand_variables(line, cmd->pipeline->my_env);
	if (!tmp)
	{
		write(2, "minishell: bad substitution\n", 28);
		free(line);
		close(write_fd);
		exit(1);
	}
	write(write_fd, tmp, ft_strlen(tmp));
	write(write_fd, "\n", 1);
	free(tmp);
}
