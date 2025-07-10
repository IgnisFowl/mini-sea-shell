/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2 <aarie-c2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:57:22 by codespace         #+#    #+#             */
/*   Updated: 2025/06/23 12:37:09 by aarie-c2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**append_arg(char **args, char *new_arg)
{
	int		i;
	char	**new_args;

	i = 0;
	while (args && args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (args);
	i = 0;
	while (args && args[i])
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[i++] = ft_strdup(new_arg);
	new_args[i] = NULL;
	free(args);
	return (new_args);
}

t_node	*init_node(t_token **tokens)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = T_WORD;
	node->cmd = NULL;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	node->redir = parse_redirections(tokens);
	if (*tokens && ((*tokens)->type == T_WORD || \
	(*tokens)->type == T_SQUOTE || (*tokens)->type == T_DQUOTE))
	{
		node->cmd = ft_strdup((*tokens)->value);
		node->type = (*tokens)->type;
		*tokens = (*tokens)->next;
	}
	return (node);
}

static void	fill_args_and_redirs(t_node *node, t_token **tokens)
{
	t_redir	*trailing;

	trailing = NULL;
	while (*tokens && (*tokens)->type != T_PIPE && \
	(*tokens)->type != T_AND && (*tokens)->type != T_OR)
	{
		if ((*tokens)->type >= T_REDIR_IN && (*tokens)->type <= T_HEREDOC)
			trailing = append_redir(trailing, parse_redirections(tokens));
		else
		{
			if (node->cmd == NULL)
			{
				node->cmd = ft_strdup((*tokens)->value);
				node->type = (*tokens)->type;
			}
			else
				node->args = append_arg(node->args, (*tokens)->value);
			*tokens = (*tokens)->next;
		}
	}
	node->redir = append_redir(node->redir, trailing);
}

t_node	*parse_cmd(t_token **tokens)
{
	t_node	*node;

	if (!tokens || !*tokens)
		return (NULL);
	if ((*tokens)->type == T_GROUP)
		return (parse_group(tokens));
	node = init_node(tokens);
	if (!node)
		return (NULL);
	fill_args_and_redirs(node, tokens);
	return (node);
}
