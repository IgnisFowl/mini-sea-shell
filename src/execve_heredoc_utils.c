/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_heredoc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarie-c2@c1r4p1.42sp.org.br <aarie-c2@c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:25:42 by aarie-c2@c1       #+#    #+#             */
/*   Updated: 2025/06/28 16:03:14 by aarie-c2@c1      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_child_sigint(int sig)
{
	(void)sig;
	g_last_signal = 130;
	write(STDOUT_FILENO, "^C\n", 3);
	close(STDIN_FILENO);
}

char	*generate_tmp_filename(void)
{
	static int	id;
	char		*base;
	char		*number;
	char		*filename;

	base = ft_strdup("/tmp/.heredoc_tmp_");
	if (!base)
		return (NULL);
	number = ft_itoa(id++);
	if (!number)
	{
		free(base);
		return (NULL);
	}
	filename = ft_strjoin(base, number);
	free(base);
	free(number);
	return (filename);
}

int	process_heredoc_line(char *line, int fd, t_redir *redir, t_env *env)
{
	char	*trimmed_line;
	char	*expanded_line;

	trimmed_line = ft_strtrim(line, "\n");
	if (!trimmed_line)
		return (1);
	if (ft_strcmp(trimmed_line, redir->delimiter) == 0)
	{
		free(trimmed_line);
		return (2);
	}
	expanded_line = expand_variables(trimmed_line, env);
	if (expanded_line)
	{
		write(fd, expanded_line, ft_strlen(expanded_line));
		free(expanded_line);
	}
	write(fd, "\n", 1);
	free(trimmed_line);
	return (0);
}

int	handle_heredoc_read(char **line)
{
	ft_putstr_fd("> ", STDOUT_FILENO);
	*line = get_next_line(STDIN_FILENO);
	if (g_last_signal == 130)
	{
		if (*line)
			free(*line);
		return (130);
	}
	if (!*line)
	{
		ft_putstr_fd("warning: here-document delimited by end-of-file\n", 2);
		return (1);
	}
	return (0);
}

int	read_heredoc_input(int fd, t_redir *redir, t_env *env)
{
	char	*line;
	int		read_status;
	int		proc_status;

	g_last_signal = 0;
	while (1)
	{
		read_status = handle_heredoc_read(&line);
		if (read_status == 130)
			return (130);
		if (read_status == 1)
			return (0);
		proc_status = process_heredoc_line(line, fd, redir, env);
		free(line);
		if (proc_status == 2)
			break ;
		if (proc_status == 1)
			return (1);
	}
	get_next_line(-1);
	return (0);
}
