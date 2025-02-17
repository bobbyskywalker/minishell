/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_signals.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 08:07:47 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/17 10:22:59 by agarbacz         ###   ########.fr       */
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

static void	set_new_signals(void)
{
	struct sigaction	sigint_act;
	struct sigaction	sigquit_act;
	int					status;

	sigemptyset(&sigint_act.sa_mask);
	sigint_act.sa_handler = &show_new_prompt;
	sigint_act.sa_flags = 0;
	status = sigaction(SIGINT, &sigint_act, NULL);
	if (status == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	sigemptyset(&sigquit_act.sa_mask);
	sigquit_act.sa_handler = SIG_IGN;
	sigquit_act.sa_flags = 0;
	status = sigaction(SIGQUIT, &sigquit_act, NULL);
	if (status == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

static void	stop_terminal_echo(void)
{
	struct termios	termios_p;
	int				status;

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

void	set_global_signals(void)
{
	if (!isatty(STDOUT_FILENO))
		return ;
	stop_terminal_echo();
	set_new_signals();
}
