/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 09:50:36 by francesca         #+#    #+#             */
/*   Updated: 2025/07/24 14:52:44 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

/*
I segnali sono un meccanismo del sistema operativo per notificare ai processi
che si è verificato un evento, come ad esempio:
SIGINT (Ctrl-C): interruzione
-> Ctrl-C	SIGINT	Interruzione	Mostrare una nuova riga con prompt
SIGQUIT (Ctrl-): quit (interrompe e core dump)
-> Ctrl-\	SIGQUIT	Quit (dump del core)	Non deve fare nulla
SIGTERM: terminazione
-> Ctrl-D	(EOF)	Fine input (non è un segnale)	Uscire dalla shell
*/

/*
SIGINT → Ctrl-C
Ricevuto quando l’utente preme Ctrl-C
Comportamento richiesto:
Mostrare un newline (\n)
Mostrare di nuovo il prompt
Non uscire dalla shell
Cosa fa in Bash?
Interrompe il comando corrente.
Se sei sulla shell e nessun comando è in esecuzione,
	stampa semplicemente una nuova riga vuota e ripresenta il prompt.
Non chiude la shell.
⚙️ In termini tecnici:
Invia il segnale SIGINT (numero 2) al processo in foreground.
*/
// rl_replace_line("", 0);
void	handle_sigint(int signo)
{
	(void)signo;
	g_exit_status = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/*
SIGQUIT → Ctrl-\
Ricevuto con Ctrl-\
Comportamento richiesto:
Non fare nulla
Ignorarlo
Cosa fa in Bash?
Invia SIGQUIT (3) al processo in foreground.
Se sei nella shell senza processi, Bash non fa nulla.
Se un processo è attivo, di solito lo termina con core dump.
*/
void	handle_sigquit(int signo)
{
	(void)signo;
	if (rl_line_buffer && rl_line_buffer[0] != '\0')
	{
		g_exit_status = 131;
		write(1, "Quit (core dumped)\n", 18);
		exit(131);
	}
}

//SIGQUIT ignorato nella shell principale
void	init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
