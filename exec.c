#include <sys/types.h>

typedef struct s_env
{
	char				*id;
	char				*value;
	char				*raw;
	struct s_env		*next;
}						t_env;

typedef enum e_token_type
{
	INFILE,
	OUTFILE,
	HEREDOC,
	N_HEREDOC,
	APPEND,
	PIPE,
	STRING,
	SINGLEQUOTE,
	DOUBLEQUOTE,
	COMMAND,
	FILENAME,
	ARGUMENT
}						t_token_type;

typedef struct s_args
{
	char				*value;
	struct s_args		*next;
}						t_args;

typedef struct s_filenames
{
	char				*path;
	t_token_type		type;
	struct s_filenames	*next;
}						t_filenames;

typedef struct s_exec
{
	char				*cmd;
	t_args				*args;
	t_filenames			*redirs;
	struct s_exec		*next;
	int					fd_in;
	int					fd_out;
}						t_exec;

typedef struct s_ctx
{
	int					def_in;
	int					def_out;
	unsigned char		exit_code;
	int					exec_count;
	pid_t				*pids;
	int					pid_count;
	t_exec				*exec;
	t_env				*envp;
}						t_ctx;

void	exec(t_ctx *ctx)
{
	int pids[ctx->exec_count];
	int fd[(ctx->exec_count) + 1][2];
	int i;
	int pipeline_pos;
	int process_nb;

	process_nb = ctx->exec_count;
	pipeline_pos = 0;
	while(pipeline_pos < process_nb)
	{
// we open process_nb + 1 fds (in parent process)
		i = 0;
		while (i < process_nb)
		{
			if (pipe(fd[i]) == -1)
			{
				printf("Error with creating pipe\n");
				return (1);
			}
		i++;
		}
		i = 0;
		while (i < process_nb)
		{
		// we create process_nb processes
			pids[i] = fork();
			if (pids[i] == -1)
			{
				printf("Error with creating process\n");
				return (2);
			}
			if (pids[i] == 0) // child process
			{
				int j;
				j = 0;
			//we close useless fds
				while (j < process_nb + 1)
				{
					if (i != j)
						close(fd[j][0]);
					if (i + 1 != j)
						close(fd[j][1]);
					j++;
				}
			//we close last fds when finished
				close(fd[i][0]);
				close(fd[i + 1][1]);
				dup2
				return (0);
			}
			i++;
		}
	}
}