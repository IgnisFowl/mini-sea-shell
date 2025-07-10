/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:53:54 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/27 12:11:00 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_in(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	redir_out(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(redir->filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	redir_append(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(redir->filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}	
	close(fd);
	return (0);
}

int	redir_heredoc(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	apply_redirs(t_redir *redir_list)
{
	int	result;

	while (redir_list)
	{
		if (redir_list->type == T_REDIR_IN)
			result = redir_in(redir_list);
		else if (redir_list->type == T_REDIR_OUT)
			result = redir_out(redir_list);
		else if (redir_list->type == T_REDIR_APPEND)
			result = redir_append(redir_list);
		if (result < 0)
			return (-1);
		redir_list = redir_list->next;
	}
	return (0);
}
