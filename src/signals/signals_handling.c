/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 08:07:47 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/12 08:49:51 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	show_new_prompt(int signal)
{
	if (signal == SIGINT)
	{
		write(STDIN_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	set_new_signal()
{
	struct sigaction	new_act;
	int					status;
	
	sigemptyset(&new_act.sa_mask);
	new_act.sa_handler = &show_new_prompt;
	new_act.sa_flags = 0;
	status = sigaction(SIGINT, &new_act, NULL);
	if (status == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

static void	stop_terminal_echo()
{
	struct termios		termios_p;
	int					status;
	
	status = tcgetattr(STDOUT_FILENO, &termios_p);
	if (status == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}
	termios_p.c_lflag &= ~ECHOCTL;
	status = tcsetattr(STDOUT_FILENO, TCSANOW, &termios_p);
	if (status == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

void	handle_signals()
{
	if (!isatty(STDOUT_FILENO))
		return ;
	stop_terminal_echo();
	set_new_signal();
}

