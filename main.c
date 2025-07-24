/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francesca <francesca@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:58:21 by francesca         #+#    #+#             */
/*   Updated: 2025/07/22 16:19:14 by francesca        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./header/minishell.h"
#include "header/envp.h"

/*
** Variabile globale usata per memorizzare lo stato di uscita della shell.
** - Aggiornata dopo l'esecuzione di ogni comando (per supportare $?)
** Modificata anche dai signal handler per riflettere l'interruzione
** da segnali (es. Ctrl-C → 130)
**
** Conforme al subject:
** - È l'unica variabile globale usata per i segnali
** - Non accede né fornisce accesso ad altre strutture dati
** - Di tipo sig_atomic_t per garantire scrittura sicura nei signal handler
*/

volatile sig_atomic_t	g_exit_status = 0;

static int	process_input_line(char *line, char ***env)
{
	t_pipeline	*pipeline;
	int			processing;

	pipeline = NULL;
	processing = 1;
	if (*line)
		add_history(line);
	pipeline = parse_line(line, *env, pipeline);
	if (!pipeline)
		return (processing);
	if (pipeline->cmds && pipeline->n_cmds > 0 && pipeline->cmds[0])
		processing = process_pipeline(pipeline, env);
	free_pipeline(pipeline);
	return (processing);
}

void	minishell_loop(char ***env)
{
	char	*line;
	int		processing;

	processing = 1;
	signal(SIGQUIT, SIG_IGN);
	while (processing == 1)
	{
		signal(SIGQUIT, handle_sigquit);
		line = readline("minishell$: ");
		signal(SIGQUIT, SIG_IGN);
		if (!line)
		{
			exit_shell(0, NULL);
			write(1, "exit\n", 5);
			break ;
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		processing = process_input_line(line, env);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**my_env;

	(void)argc;
	(void)argv;
	if (isatty(STDIN_FILENO))
		init_signals();
	my_env = copy_env(envp);
	minishell_loop(&my_env);
	free_myenvp(my_env);
	clear_history();
	return (g_exit_status);
}
