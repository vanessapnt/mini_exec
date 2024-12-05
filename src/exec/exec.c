/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:16:21 by varodrig          #+#    #+#             */
/*   Updated: 2024/12/05 15:49:29 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exe_close(int *fd)
{
	if (fd && *fd != -1)
	{
		close(*fd);
		fprintf(stderr, "close\n");
		*fd = -1;
	}
}

void	ft_close(t_ctx *ctx)
{
	if (ctx)
	{
		exe_close(&(ctx->def_in));
		exe_close(&(ctx->def_out));
	}
}
// we close useless fds OR
// we close last fds when finished
void	close_fds(int pipes_nb, int (*fd)[2], int current_cmd,
		bool is_final_close)
{
	int	j;

	j = 0;
	while (j < pipes_nb)
	{
		if (!is_final_close)
		{
			if (j != current_cmd - 1)
			{
				exe_close(&fd[j][0]); // Close read ends except the one needed
			}
			if (j != current_cmd)
			{
				exe_close(&fd[j][1]); // Close write ends except the one needed
			}
		}
		else
		{
			exe_close(&fd[j][0]); // Close all read ends
			exe_close(&fd[j][1]); // Close all write ends
		}
		j++;
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
	while (curr && ft_strncmp("PATH=", curr->raw, 5) != 0)
		curr = curr->next;
	if (!curr)
		return (ft_strdup(cmd));
	// fprintf(stderr, "sorti de la boucle curr\n");
	paths = ft_split(curr->value, ':');
	// fprintf(stderr, "split done\n");
	i = 0;
	while (paths[i])
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
		i++;
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
			ft_free_all(envp);
			perror("Error with execve");
			exit(1);
		}
		free(path);
		return (0);
	}
	ft_free_all(envp);
	return (0);
}

void	child_process(t_ctx *ctx, int (*fd)[2], int i, t_exec *temp)
{
	int	exit_code;

	exit_code = 0;
	signal(SIGQUIT, SIG_DFL);
	if (ctx->exec_count > 1)
	{
		if (i > 0)
		{
			dup2(fd[i - 1][0], STDIN_FILENO);
			fprintf(stderr, "dup2\n");
			exe_close(&fd[i - 1][0]);
		}
		if (i < ctx->exec_count - 1)
		{
			dup2(fd[i][1], STDOUT_FILENO);
			fprintf(stderr, "dup2\n");
			exe_close(&fd[i][1]);
		}
	}
	ft_close(ctx);
	close_fds(ctx->exec_count - 1, fd, i, false);
	if (err_redirs(temp))
	{
		ctx->exit_code = 1;
		exit(ctx->exit_code);
	}
	if (ft_execution(ctx, temp) == -1)
	{
		perror("Error executing command");
		exit(1);
	}
}
void	exe_err_coredump(int pid)
{
	int	fd_tmp;

	fd_tmp = dup(STDOUT_FILENO);
	fprintf(stderr, "dup.c\n");
	dup2(STDERR_FILENO, STDOUT_FILENO);
	fprintf(stderr, "dup2\n");
	printf("[%d]: Quit (core dumped)\n", pid);
	dup2(fd_tmp, STDOUT_FILENO);
	fprintf(stderr, "dup2\n");
	exe_close(&fd_tmp);
}

void	exe_wait_all(t_ctx *ctx) // TODO
{
	int status;
	int i;

	i = 0;
	while (i < ctx->pid_count)
	{
		if (waitpid(ctx->pids[i], &(status), 0))
		{
			if (WIFEXITED(status))
			{
				g_signals.signal_code = 0;
				ctx->exit_code = WEXITSTATUS(status);
			}
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
					exe_err_coredump(ctx->pids[i]);
				g_signals.signal_code = SIGNAL_OFFSET + WTERMSIG(status);
			}
		}
		i++;
	}
	unlink_all(ctx);
}

int	open_pipes(int pipes_nb, int (*fd)[2])
{
	int	i;
	int	j;

	i = 0;
	while (i < pipes_nb)
	{
		if (pipe(fd[i]) == -1)
		{
			perror("Error creating pipe");
			// Close all previously opened pipes to avoid resource leaks
			j = 0;
			while (j < i)
			{
				exe_close(&fd[j][0]); // Close the read end
				exe_close(&fd[j][1]); // Close the write end
				j++;
			}
			return (-1); // Signal failure
		}
		fprintf(stderr, "pipe\n");
		i++;
	}
	return (0); // Signal success
}

void	set_std(t_ctx *ctx, int mode)
{
	if (!mode)
	{
		ctx->def_in = dup(STDIN_FILENO);
		fprintf(stderr, "dup.a\n");
		ctx->def_out = dup(STDOUT_FILENO);
		fprintf(stderr, "dup.b\n");
	}
	else
	{
		dup2(ctx->def_in, STDIN_FILENO);
		fprintf(stderr, "dup2\n");
		dup2(ctx->def_out, STDOUT_FILENO);
		fprintf(stderr, "dup2\n");
		ft_close(ctx);
	}
}

int	exec_parent(t_ctx *ctx)
{
	t_exec	*temp;
	int		fd[ctx->exec_count - 1][2];
	pid_t	pid;

	temp = ctx->exec;
	// Open pipes
	if (open_pipes(ctx->exec_count - 1, fd) == -1)
	{
		perror("Error opening pipes");
		return (1);
	}
	// Create child processes
	while (temp)
	{
		signal(SIGINT, sig_exec);
		pid = fork();
		if (pid == -1)
		{
			perror("Error creating process");
			close_fds(ctx->exec_count - 1, fd, -1, true);
			return (1);
		}
		else if (pid == 0)
		{
			child_process(ctx, fd, ctx->pid_count, temp);
		}
		ctx->pids[ctx->pid_count] = pid;
		ctx->pid_count++;
		temp = temp->next;
	}
	// Close all pipes in the parent
	close_fds(ctx->exec_count - 1, fd, -1, true);
	set_std(ctx, 1);
	// Wait for all child processes
	exe_wait_all(ctx);
	// unlink_all(ctx);
	return (0);
}

void	exe_dup2_close(int fd1, int fd2)
{
	dup2(fd1, fd2);
	fprintf(stderr, "dup2\n");
	exe_close(&fd1);
}

void	unlink_all(t_ctx *ctx)
{
	t_exec		*exec;
	t_filenames	*temp;

	exec = ctx->exec;
	while (exec)
	{
		temp = exec->redirs;
		while (temp)
		{
			if (temp->type == N_HEREDOC)
				unlink(temp->path);
			temp = temp->next;
		}
		exec = exec->next;
	}
}

int	redirs_type(t_exec *exec, t_filenames *file)
{
	if (file->type == INFILE || file->type == N_HEREDOC)
	{
		if (exec->fd_in != STDIN_FILENO)
		{
			exe_close(&(exec->fd_in));
		}
		exec->fd_in = open(file->path, O_RDONLY);
		fprintf(stderr, "open");
		if (exec->fd_in == -1)
		{
			printf("error with open"); // TODO
			return (1);
		}
		dup2(exec->fd_in, STDIN_FILENO);
		fprintf(stderr, "dup2\n");
		exe_close(&(exec->fd_in));
	}
	else
	{
		if (exec->fd_out != STDOUT_FILENO)
		{
			exe_close(&(exec->fd_out));
		}
		if (file->type == OUTFILE)
		{
			exec->fd_out = open(file->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			fprintf(stderr, "open");
		}
		else if (file->type == APPEND)
		{
			exec->fd_out = open(file->path, O_WRONLY | O_CREAT | O_APPEND,
					0644);
			fprintf(stderr, "open");
		}
		if (exec->fd_out == -1)
		{
			printf("err1_open");
			return (1);
		}
		dup2(exec->fd_out, STDOUT_FILENO);
		fprintf(stderr, "dup2\n");
		exe_close(&(exec->fd_out));
	}
	return (0);
}

int	exec_redirs(t_exec *exec)
{
	t_filenames	*temp;

	temp = exec->redirs;
	while (temp)
	{
		if (redirs_type(exec, temp) == 1)
			return (1);
		temp = temp->next;
	}
	return (0);
}

int	err_redirs(t_exec *exec)
{
	while (exec)
	{
		if (exec_redirs(exec) == 1)
			return (1);
		exec = exec->next;
	}
	return (0);
}

int	exec(t_ctx *ctx)
{
	t_exec	*temp;

	// il sert a quoi exit_code ?
	if (ctx->exec_count == 0)
		return (0);
	// store STDIN & STDOUT
	set_std(ctx, 0);
	// 1 command and its a builtin (no loop)
	temp = ctx->exec;
	if (ctx->exec_count == 1 && bi_is_builtin(temp->cmd))
	{
		// if errors in redirections, exit
		if (err_redirs(temp))
		{
			ctx->exit_code = 1;
			return (ctx->exit_code);
		}
		if (bi_is_builtin(temp->cmd) == 2)
			ft_putstr_fd("exit\n", STDERR_FILENO);
		// id exit, printf "exit" ?? (dans la bi_exit?)
		// unlink (close) all the N_HEREDOC(unlik dans le else)
		unlink_all(ctx);
		// fonction reutilisee dans le else
		// ctx->exit_code = do_builtin(ctx, temp->cmd, temp->args); //TODO
		ctx->exit_code = bi_do_builtin(ctx, temp->cmd, temp->args);
		set_std(ctx, 1);
		return (ctx->exit_code);
	}
	exec_parent(ctx);
	return (0);
}
