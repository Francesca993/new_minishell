/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 08:01:34 by skayed            #+#    #+#             */
/*   Updated: 2025/07/16 16:45:15 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

void	setup_pipes(t_pipeline *pipeline, int i, int **pipes)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < pipeline->n_cmds - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_pipes(pipes, pipeline->n_cmds - 1);
}

void	try_execute_builtin(t_cmd *cmd, char ***env)
{
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, env, NULL);
		exit(1);
	}
}

void	exec_with_env_path(t_cmd *cmd, char **env)
{
	char	*path;

	path = check_path(find_path(env), cmd->args[0]);
	if (!path)
		exit(127);
	execve(path, cmd->args, env);
	perror("execve");
	free(path);
	exit(1);
}
