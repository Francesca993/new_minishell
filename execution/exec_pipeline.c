/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:22:22 by skayed            #+#    #+#             */
/*   Updated: 2025/07/22 15:22:18 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static int	**create_pipes(int n_cmds)
{
	int	**pipes;
	int	i;

	i = 0;
	pipes = malloc(sizeof(int *) * (n_cmds - 1));
	if (!pipes)
		return (NULL);
	while (i < n_cmds - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (perror("malloc failed"), NULL);
		if (pipe(pipes[i]) < 0)
			return (perror("pipe failed"), NULL);
		i++;
	}
	return (pipes);
}

static void	execute_cmd(t_pipeline *pipeline, int i, int **pipes)
{
	t_cmd	*cmd;

	cmd = pipeline->cmds[i];
	setup_pipes(pipeline, i, pipes);
	set_redirections(cmd);
	try_execute_builtin(cmd, &pipeline->my_env);
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], X_OK) == 0)
			execve(cmd->args[0], cmd->args, pipeline->my_env);
		fprintf(stderr, "minishell: %s: No such file or directory\n",
			cmd->args[0]);
		exit(127);
	}
	else
		exec_with_env_path(cmd, pipeline->my_env);
}

int	fork_and_exec(t_pipeline *pipeline, int i, int **pipes, pid_t *pids)
{
	pids[i] = fork();
	if (pids[i] < 0)
		return (perror("Pipe failed"), -1);
	if (pids[i] == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_cmd(pipeline, i, pipes);
	}
	return (0);
}

void	wait_all(pid_t *pids, int n_cmds)
{
	int	i;
	int	status;

	i = 0;
	while (i < n_cmds)
		waitpid(pids[i++], &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			exit(131);
	}
	else
		g_exit_status = 1;
}

void	execute_pipeline(t_pipeline *pipeline)
{
	int		i;
	pid_t	*pids;
	int		**pipes;

	i = 0;
	pipes = create_pipes(pipeline->n_cmds);
	if (!pipes)
		return ;
	pids = malloc(sizeof(pid_t) * pipeline->n_cmds);
	if (!pids)
		return ;
	while (i < pipeline->n_cmds)
	{
		if (fork_and_exec(pipeline, i, pipes, pids) < 0)
			return ;
		i++;
	}
	close_pipes(pipes, pipeline->n_cmds - 1);
	wait_all(pids, pipeline->n_cmds);
	free(pipes);
	free(pids);
}
