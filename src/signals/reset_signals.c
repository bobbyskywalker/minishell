/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:49:37 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/14 13:35:36 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	restore_default_signals(void)
{
	struct sigaction	sigact;
	int					status;

	sigemptyset(&sigact.sa_mask);
	sigact.sa_handler = SIG_DFL;
	sigact.sa_flags = 0;
	status = sigaction(SIGINT, &sigact, NULL);
	if (status == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	status = sigaction(SIGQUIT, &sigact, NULL);
	if (status == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

static void	restore_terminal_settings(void)
{
	struct termios	termios_p;
	int				status;

	status = tcgetattr(STDOUT_FILENO, &termios_p);
	if (status == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}
	termios_p.c_lflag |= ECHOCTL;
	status = tcsetattr(STDOUT_FILENO, TCSANOW, &termios_p);
	if (status == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

void	reset_signals_handling(void)
{
	if (!isatty(STDOUT_FILENO))
		return ;
	restore_terminal_settings();
	restore_default_signals();
}
