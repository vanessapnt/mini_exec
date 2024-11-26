#ifndef EXEC_H
# define EXEC_H

typedef struct s_ctx	t_ctx;
typedef struct s_env	t_env;

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

void					open_pipes(int pipes_nb, int fd[pipes_nb][2]);
void					close_fds(int pipes_nb, int (*fd)[2], int i, bool exec);
int						redirs_type(char *path, int fd_tochange,
							t_token_type type);
void					exec_handle_redir(t_exec *temp);
int						size_linked_list(t_args *args);
void					create_args(t_exec *temp, int args_nb,
							char *args[args_nb]);
int						ft_char_count(char *str, char c);
void					ft_free_all(char **arr);
char					*find_path(char *cmd, t_env *envp);
int						size_env(t_env *envp);
char					**envp_format(t_env *envp);
int						ft_execution(t_ctx *ctx, t_exec *temp);
void						child_process(t_ctx *ctx, int (*fd)[2], int i,
							t_exec *temp);
void					ft_wait_all(int childs, int *pid);
int						exec(t_ctx *ctx);

#endif