/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:06:07 by skayed            #+#    #+#             */
/*   Updated: 2025/07/13 09:29:31 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	setup_redir_in(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->infile, O_RDONLY);
	if (fd < 0)
		return (perror("open <"), -1);
	cmd->fd_in = fd;
	cmd->redir_in = 1;
	return (0);
}

int	setup_redir_out(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("open >"), -1);
	cmd->fd_out = fd;
	cmd->redir_out = 1;
	return (0);
}

int	setup_redir_append(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror("open >>"), -1);
	cmd->fd_out = fd;
	cmd->append = 1;
	return (0);
}

static void	read_heredoc(t_cmd *cmd, int write_fd, char *delim, int exp)
{
	char	*line;
	char	*tmp;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (exp)
		{
			tmp = expand_variables(line, cmd->pipeline->my_env);
			if (!tmp)
			{
				write(2, "minishell: bad substitution\n", 28);
				free(line);
				close(write_fd);
				exit(1); // blocca il figlio
			}
			write(write_fd, tmp, ft_strlen(tmp));
			write(write_fd, "\n", 1);
			free(tmp);
		}
		else
		{
			write(write_fd, line, ft_strlen(line));
			write(write_fd, "\n", 1);
		}
		free(line);
	}
}

int	setup_heredoc(t_cmd *cmd, char *delimiter)
{
	int		pipe_fd[2];
	char	*new_del;
	int		exp_var;
	pid_t	pid;
	int		status;

	new_del = NULL;
	new_del = strip_outer_quotes(delimiter);
	exp_var = !ft_strcmp(new_del, delimiter);
	if (pipe(pipe_fd) < 0)
		return (perror("pipe heredoc"), -1);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		read_heredoc(cmd, pipe_fd[1], new_del, exp_var);
		close(pipe_fd[1]);
		free(new_del);
		exit(0);
	}
	free(new_del);
	wait(NULL);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		return (-1); // heredoc fallito, non continuare
	}
	close(pipe_fd[1]);
	cmd->fd_in = pipe_fd[0];
	cmd->heredoc = 1;
	return (0);
}
