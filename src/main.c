/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:05:39 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/23 17:16:41 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	int		final_exit_code;

	(void)argc;
	(void)argv;
	env = env_from_envp(envp);
	if (!env)
		return (1);
	run_shell_loop(&env);
	final_exit_code = g_last_signal;
	free_env(env);
	return (final_exit_code);
}
