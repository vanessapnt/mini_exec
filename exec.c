/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 15:16:21 by varodrig          #+#    #+#             */
/*   Updated: 2024/11/25 18:56:59 by varodrig         ###   ########.fr       */
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
	t_args	*curr;
	int		i;

	i = 0;
	args[i] = temp->cmd;
	curr = temp->args;
	i++;
	while (curr)
	{
		args[i] = curr->value;
		curr = temp->next;
		i++;
	}
	args[args_nb] = NULL;
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

char	*find_path(char *cmd, t_env *envp)
{
	char	**paths;
	char	*path;
	char	*exec;
	t_env	*curr;
	char	*value;
	int		i;

	while (curr)
	{
		if (ft_strncmp("PATH=", curr->id, 5) == 0)
			break ;
		curr = curr->next;
	}
	paths = ft_split(curr->value, ':');
	i = 0;
	while (paths[i++])
	{
		path = ft_strjoin(paths[i], "/");
		exec = ft_strjoin(path, cmd);
		free(path);
		if (!access(exec, X_OK | F_OK))
		{
			ft_free_all(paths);
			return (exec);
		}
		free(exec);
	}
	ft_free_all(paths);
	return (ft_strdup(cmd));
}

int	ft_execution(t_ctx *ctx, t_exec *temp)
{
	int		args_nb;
	char	*path;
	char	*args[size_args(temp->args) + 2];

	args_nb = size_args(temp->args) + 2;
	// execve(path, comd, envp);
	// char	*args[] = {"/bin/ls", "-l", "/home", NULL};
	create_args(temp, args_nb, args);
	if (execve(temp->cmd, args, ctx->envp) == -1)
	{
		path = find_path(temp->cmd, ctx->envp);
		if (execve(temp->cmd, args, ctx->envp) == -1)
		{
			free(path);
			perror("Error with execve");
			free(path);
			exit(1);
		}
		free(path);
		return (0);
	}
}

int	child_process(t_ctx *ctx, int (*fd)[2], int i, t_exec *temp)
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
	return (0);
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
	ft_wait_all(ctx->exec_count, pids);
	return (0);
}

//simplified parsing part :

// Fonction pour créer une commande de test
t_exec	*create_test_command(void)
{
	t_exec		*command;
	t_args		*args;
	t_filenames	*redirs;

	command = malloc(sizeof(t_exec));
	args = malloc(sizeof(t_args));
	redirs = NULL;
	// Créer une commande "ls"
	command->cmd = "ls";
	args->value = "-l"; // Argument pour "ls"
	args->next = NULL;
	command->args = args;
	// Pas de redirection dans cet exemple
	command->redirs = redirs;
	command->next = NULL;
	return (command);
}

// Fonction pour initialiser le contexte avec `envp`
t_ctx	*create_test_ctx(void)
{
	t_ctx	*ctx;

	ctx = malloc(sizeof(t_ctx));
	ctx->def_in = 0;
	ctx->def_out = 1;
	ctx->exit_code = 0;
	ctx->exec_count = 1;               // Nous n'avons qu'une commande ici
	ctx->exec = create_test_command(); // Ajouter une commande à exécuter
	// Initialiser un environnement simplifié
	char *env[] = {
		"PATH=/bin:/usr/bin", // Définir le chemin pour chercher les exécutables
		"HOME=/home/user",
		"USER=testuser",
		NULL // Doit toujours finir par NULL
	};
	ctx->envp = env;
	return (ctx);
}

int	main(void)
{
	// Initialisation du contexte de test
	t_ctx *ctx = create_test_ctx();

	// Exécution des commandes
	exec(ctx);

	// Libération de la mémoire
	free(ctx->exec->args);
	free(ctx->exec);
	free(ctx);

	return (0);
}