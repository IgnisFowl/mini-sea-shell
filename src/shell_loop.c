/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:28:41 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/28 10:50:16 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_tokens(t_token *token_current, t_env *env)
{
	char	*expanded;
	t_token	*token_head;

	expanded = NULL;
	token_head = token_current;
	while (token_head)
	{
		if (token_head->type == T_WORD || token_head->type == T_DQUOTE)
		{
			expanded = expand_variables(token_head->value, env);
			free(token_head->value);
			token_head->value = expanded;
		}
		token_head = token_head->next;
	}
}

static int	handle_tokenize(t_token **token_head, \
	t_node **tree, char *line, t_env **env)
{
	t_token	*token_current;

	token_current = NULL;
	tokenize(token_head, line);
	if (*token_head)
	{
		token_current = *token_head;
		merge_adjacent_tokens(token_current);
		expand_tokens(token_current, *env);
		*tree = parse_expression(&token_current);
	}
	free_tokens(token_head);
	return (1);
}

static int	get_tokens(char **line, t_node **tree, t_env **env)
{
	t_token	*token_head;

	*line = readline("mini_sea_shell$ ");
	if (!*line)
	{
		write(1, "exit\n", 5);
		return (0);
	}
	if (**line)
		add_history(*line);
	token_head = NULL;
	*tree = NULL;
	if (checker(*line))
	{
		if (!handle_tokenize(&token_head, tree, *line, env))
			return (1);
	}
	return (1);
}

static int	handle_shell_iteration(t_env **env)
{
	char	*line;
	t_node	*tree;
	t_node	*root;

	handle_signals();
	if (!get_tokens(&line, &tree, env))
		return (0);
	root = tree;
	g_last_signal = execute_tree(tree, env);
	free_all(root, line);
	root = NULL;
	tree = NULL;
	return (1);
}

void	run_shell_loop(t_env **env)
{
	while (1)
	{
		if (!handle_shell_iteration(env))
		{
			rl_clear_history();
			break ;
		}
	}
}
