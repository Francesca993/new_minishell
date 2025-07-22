/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:06:07 by skayed            #+#    #+#             */
/*   Updated: 2025/07/22 16:03:43 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// int	setup_redir_in(t_cmd *cmd)
// {
// 	int	fd;

// 	fd = open(cmd->infile, O_RDONLY);
// 	if (fd < 0)
// 		return (perror("open <"), -1);
// 	cmd->fd_in = fd;
// 	cmd->redir_in = 1;
// 	return (0);
// }
int	setup_redir_in(t_cmd *cmd)
{
	int		fd;
	char	*no_quotes;

	if (cmd->fd_in > 0)
		close(cmd->fd_in);
	no_quotes = strip_outer_quotes(cmd->infile);
	fd = open(no_quotes, O_RDONLY);
	free(no_quotes);
	if (fd < 0)
	{
		perror(cmd->infile);
		//free(cmd->pipeline);
		return (-1);
	}
	//dup2(fd, STDIN_FILENO);
	//close(fd);
	cmd->fd_in = fd;
	cmd->redir_in = 1;
	return (0);
}

// int	setup_redir_out(t_cmd *cmd)
// {
// 	int	fd;

// 	fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 		return (perror("open >"), -1);
// 	cmd->fd_out = fd;
// 	cmd->redir_out = 1;
// 	return (0);
// }
int	setup_redir_out(t_cmd *cmd)
{
	int		fd;
	char	*no_quotes;

	no_quotes = strip_outer_quotes(cmd->outfile);
	fd = open(no_quotes, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	free(no_quotes);
	if (fd < 0)
		return (perror(cmd->outfile), -1);
	cmd->fd_out = fd;
	cmd->redir_out = 1;
	return (0);
}

// int	setup_redir_append(t_cmd *cmd)
// {
// 	int	fd;

// 	fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	if (fd < 0)
// 		return (perror("open >>"), -1);
// 	cmd->fd_out = fd;
// 	cmd->append = 1;
// 	return (0);
// }
int	setup_redir_append(t_cmd *cmd)
{
	int		fd;
	char	*no_quotes;

	no_quotes = strip_outer_quotes(cmd->outfile);
	fd = open(no_quotes, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(cmd->outfile), -1);
	cmd->fd_out = fd;
	cmd->append = 1;
	return (0);
}

void	read_heredoc(t_cmd *cmd, int write_fd, char *delim, int exp)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (exp)
			handle_exp(line, cmd, write_fd);
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
		exec_here_child(pipe_fd, cmd, exp_var, new_del);
	free(new_del);
	wait(NULL);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (close(pipe_fd[0]), -1);
	close(pipe_fd[1]);
	cmd->fd_in = pipe_fd[0];
	cmd->heredoc = 1;
	return (0);
}
