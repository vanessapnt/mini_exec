/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:16:21 by varodrig          #+#    #+#             */
/*   Updated: 2024/11/25 15:42:38 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

open_pipes(int pipes_nb, int fd[pipes_nb][2])
{
	int	i;

	i = 0;
	while (i < pipes_nb)
	{
		if (pipe(fd[i++]) == -1)
		{
			printf("Error with creating pipe\n");
			exit(1);
		}
	}
}

// we close useless fds OR
// we close last fds when finished
void	close_fds(int pipes_nb, int (*fd)[2], int i, bool exec)
{
	int	j;

	if (!exec)
	{
		j = 0;
		while (j < pipes_nb)
		{
			if (i != j)
				close(fd[j][0]);
			if (i + 1 != j)
				close(fd[j][1]);
			j++;
		}
	}
	else
	{
		close(fd[i][0]);
		close(fd[i + 1][1]);
	}
}

int	redirs_type(char *path, int fd_tochange, enum e_token_type type)
{
	int	fd;

	if (type == OUTFILE)
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type == APPEND)
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (type == INFILE || N_HEREDOC)
		fd = open(path, O_RDONLY, 0644);
	else
	{
		perror("Unknown redirection type");
		return (-1);
	}
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	if (dup2(fd, fd_tochange) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	// if(type == N_HEREDOC)
	// unlink?
	return (0);
}

void	exec_handle_redir(t_ctx *ctx, int i, t_exec *temp)
{
	t_exec				*temp;
	t_filenames			*redir;
	enum e_token_type	*type;

	/*	int					j;
		j = 0;
		temp = ctx->exec;
		while (j < i)
		{
			temp = temp->next;
			i++;
		}*/
	redir = temp->redirs;
	while (redir)
	{
		type = redir->type;
		if (type == INFILE || N_HEREDOC)
			redirs_type(redir->path, IN, type);
		if (type == OUTFILE || APPEND)
			redirs_type(redir->path, OUT, type);
		redir = redir->next;
	}
}

int	size_linked_list(t_args *args)
{
	t_args	*curr;
	int		count;

	count = 0;
	curr = args;
	while (curr)
	{
		curr = curr->next;
		count++;
	}
	return (count);
}

create_args(t_exec *temp, int args_nb, char *args[args_nb])
{
	t_args *curr;
	int i;

	i = 0;
	args[i] = temp->cmd;
	curr = temp->args;
	i++;
	while(curr)
	{
		args[i] = curr->value;
		curr = temp->next;
		i++;
	}
	args[i] = "NULL";
}

int	ft_execution(t_ctx *ctx, t_exec *temp)
{
	int args_nb;

	args_nb = size_args(temp->args)+ 2;
	char	*args[args_nb];

	// execve(path, comd, envp);
	// char	*args[] = {"/bin/ls", "-l", "/home", NULL};
	create_args(temp, args_nb, args);
	if (execve(temp->cmd, args, ctx->envp) == -1)
		find_path()
}

child_process(t_ctx *ctx, int (*fd)[2], int i, t_exec *temp)
{
	close_fds((ctx->exec_count) + 1, fd, i, false);
	dup2(fd[i][0], IN);
	dup2(fd[i + 1][1], OUT);
	exec_handle_redir(ctx, i, temp);
	if (ft_execution(ctx, temp) == -1)
	{
		printf("Error with exec in process %d\n", i);
		exit(1);
	}
	close_fds((ctx->exec_count) + 1, fd, i, true);
}

void	exec(t_ctx *ctx)
{
	int		fd[(ctx->exec_count) + 1][2];
	int		pids[ctx->exec_count];
	int		i;
	t_exec	*temp;

	// we open cmd_nb + 1 pipes
	open_pipes((ctx->exec_count) + 1, fd);
	temp = ctx->exec;
	i = 0;
	while (temp)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			printf("Error with creating process\n");
			return (2);
		}
		if (pids[i] == 0)
			child_process(ctx, fd, i, temp);
		i++;
		temp = temp->next;
	}
}
