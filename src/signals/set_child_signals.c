/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_child_signals.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 23:17:45 by kubaz             #+#    #+#             */
/*   Updated: 2025/02/17 10:22:49 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	show_new_prompt4child(int signal)
{
	if (signal == SIGINT)
	{
		write(STDIN_FILENO, "\n", 1);
		rl_replace_line("", 0);
	}
}

static void	set_signal4child(void)
{
	struct sigaction	sigint_act;
	int					status;

	sigemptyset(&sigint_act.sa_mask);
	sigint_act.sa_handler = &show_new_prompt4child;
	sigint_act.sa_flags = 0;
	status = sigaction(SIGINT, &sigint_act, NULL);
	if (status == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

void	set_child_signals(void)
{
	if (!isatty(STDOUT_FILENO))
		return ;
	set_signal4child();
}
