/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 08:01:34 by skayed            #+#    #+#             */
/*   Updated: 2025/07/25 10:26:37 by fmontini         ###   ########.fr       */
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
	char	*env_path;

	env_path = find_path(env);
	if (!env_path)
	{
		fprintf(stderr, "minishell: %s: No such file or directory\n",
			cmd->args[0]);
		exit(127);
	}
	path = check_path(env_path, cmd->args[0]);
	if (!path)
		exit(127);
	execve(path, cmd->args, env);
	perror("execve");
	free(path);
	exit(1);
}

void	close_pipes(int **pipes, int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
