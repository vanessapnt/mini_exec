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
