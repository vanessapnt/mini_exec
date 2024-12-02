/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:16:21 by varodrig          #+#    #+#             */
/*   Updated: 2024/12/02 16:56:09 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (pipes_nb != 0)
		{
			close(fd[i][0]);
			close(fd[i + 1][1]);
		}
	}
}

int	redirs_type(char *path, int fd_tochange, t_token_type type)
{
	int	fd;

	if (type == OUTFILE)
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type == APPEND)
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (type == INFILE || type == N_HEREDOC)
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

void	exec_handle_redir(t_exec *temp)
{
	t_filenames		*redir;
	t_token_type	type;

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
		if (type == INFILE || type == N_HEREDOC)
			redirs_type(redir->path, 0, type);
		if (type == OUTFILE || type == APPEND)
			redirs_type(redir->path, 1, type);
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

void	create_args(t_exec *temp, int args_nb, char *args[args_nb])
{
	t_args	*curr;
	int		i;

	// fprintf(stderr, "entering create args\n");
	i = 0;
	args[i] = temp->cmd;
	curr = temp->args;
	i++;
	while (curr)
	{
		args[i] = curr->value;
		// fprintf(stderr, "args[%d] : %s\n", i, args[i]);
		curr = curr->next;
		i++;
	}
	args[i] = NULL;
	// fprintf(stderr, "args[%d] : %s\n", i, args[i]);
}

int	ft_char_count(char *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i++] == c)
			count++;
	}
	return (count);
}
void	ft_free_all(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	*find_path(char *cmd, t_env *envp)
{
	char	**paths;
	char	*path;
	char	*exec;
	t_env	*curr;
	int		i;

	// fprintf(stderr, "entered find_path\n");
	curr = envp;
	while (curr)
	{
		if (ft_strncmp("PATH=", curr->raw, 5) == 0)
		{
			// fprintf(stderr, "PATH found\n");
			break ;
		}
		curr = curr->next;
	}
	// fprintf(stderr, "sorti de la boucle curr\n");
	paths = ft_split(curr->value, ':');
	// fprintf(stderr, "split done\n");
	i = 0;
	while (paths[i++])
	{
		path = ft_strjoin(paths[i], "/");
		exec = ft_strjoin(path, cmd);
		free(path);
		if (!access(exec, X_OK | F_OK))
		{
			ft_free_all(paths);
			// fprintf(stderr, "command returned : %s\n", exec);
			return (exec);
		}
		free(exec);
	}
	ft_free_all(paths);
	// fprintf(stderr, "command not found");
	return (ft_strdup(cmd));
}

int	size_env(t_env *envp)
{
	int	count;

	count = 0;
	while (envp)
	{
		count++;
		envp = envp->next;
	}
	return (count);
}

char	**envp_format(t_env *envp)
{
	char	**env;
	int		size;
	int		i;

	i = 0;
	size = size_env(envp);
	env = (char **)malloc(sizeof(char *) * (size + 1));
	if (!env)
		return (NULL);
	while (i < size)
	{
		env[i] = envp->raw;
		envp = envp->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

int	ft_execution(t_ctx *ctx, t_exec *temp)
{
	int		args_nb;
	char	*path;
	char	*args[size_linked_list(temp->args) + 2];
	char	**envp;

	// fprintf(stderr, "entered ft_execution\n");
	args_nb = size_linked_list(temp->args) + 2;
	// execve(path, comd, envp);
	// char	*args[] = {"/bin/ls", "-l", "/home", NULL};
	create_args(temp, args_nb, args);
	// int n = 0;
	// while(args[n] != NULL)
	// {
	// 	//fprintf(stderr, "args[%d] : %s\n", n, args[n]);
	// 	n++;
	// }
	envp = envp_format(ctx->envp);
	// fprintf(stderr, "envp copied\n");
	if (execve(temp->cmd, args, envp) == -1)
	{
		// fprintf(stderr, "not absolute link\n");
		path = find_path(temp->cmd, ctx->envp);
		// fprintf(stderr, "path found\n");
		if (execve(path, args, envp) == -1)
		{
			free(path);
			perror("Error with execve");
			free(path);
			exit(1);
		}
		free(path);
		return (0);
	}
	return (0);
}

void	child_process(t_ctx *ctx, int (*fd)[2], int i, t_exec *temp)
{
	close_fds((ctx->exec_count) - 1, fd, i, false);
	if (ctx->exec_count > 1)
	{
		if (i > 0)
			dup2(fd[i][0], 0);
		if (i < ctx->exec_count - 1)
			dup2(fd[i + 1][1], 1);
	}
	exec_handle_redir(temp);
	if (ft_execution(ctx, temp) == -1)
		exit(1);
	close_fds((ctx->exec_count) + 1, fd, i, true);
}

void	ft_wait_all(int childs, int *pid)
{
	int	i;

	i = 0;
	while (i < childs)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
}

void	open_pipes(int pipes_nb, int fd[pipes_nb][2])
{
	int	i;

	i = 0;
	while (i < pipes_nb)
	{
		if (pipe(fd[i++]) == -1)
		{
			////fprintf(stderr, "Error with creating pipe\n");
			exit(1);
		}
		// fprintf(stderr, "%d pipes opened\n", i);
	}
}

int	exec_parent(t_ctx *ctx)
{
	t_exec	*temp;
	int		fd[ctx->exec_count][2];
	t_exec	*temp;

	// we open cmd_nb + 1 pipes
	open_pipes((ctx->exec_count - 1), fd);
	temp = ctx->exec;
	i = 0;
	while (temp)
	{
		ctx->pids[ctx->pid_count] = fork();
		if (ctx->pids[ctx->pid_count] == -1)
			err_fork; //TODO
		else if (ctx->pids[ctx->pid_count] == 0)
			child_process(ctx, fd, ctx->pid_count, temp);
		ctx->pid_count++;
		temp = temp->next;
	}
	ft_wait_all(ctx->exec_count, pids);
	return (0);
}

int	exec(t_ctx *ctx)
{
	t_exec	*temp;

	// TODO il sert a quoi exit_code ?
	if (ctx->exec_count == 0)
		return (0);
	// store STDIN & STDOUT
	// TODO
	set_std(ctx, 0);
	// 1 command and its a builtin (no loop)
	temp = ctx->exec;
	if (ctx->exec_count == 1 && is_builtin(temp->cmd)) // TODO
	{
		// if errors in redirections, exit
		if (err_redirs(temp))
		{
			ctx->exit_code = 1;
			return (ctx->exit_code);
		}
		// id exit, printf "exit" ?? (dans la bi_exit?)
		// unlink (close) all the N_HEREDOC(unlik dans le else)
		unlink_all(ctx);
		// TODO// fonction reutilisee dans le else
		ctx->exit_code = do_builtin(ctx, temp->cmd, temp->args); // TODO
		return (ctx->exit_code);
	}
	exec_parent(ctx);
	set_std(ctx, 0);
	return (0);
}
