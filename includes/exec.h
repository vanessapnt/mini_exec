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

void					exe_close(int *fd);
void					ft_close(t_ctx *ctx);
int						open_pipes(int pipes_nb, int fd[pipes_nb][2]);
void					close_fds(int pipes_nb, int (*fd)[2], int current_cmd,
							bool is_final_close);
int						size_linked_list(t_args *args);
void					create_args(t_exec *temp, int args_nb,
							char *args[args_nb]);
int						ft_char_count(char *str, char c);
void					ft_free_all(char **arr);
char					*find_path(char *cmd, t_env *envp);
int						size_env(t_env *envp);
char					**envp_format(t_env *envp);
int						ft_execution(t_ctx *ctx, t_exec *temp);
void					child_process(t_ctx *ctx, int (*fd)[2], int i,
							t_exec *temp);
void					exe_wait_all(t_ctx *ctx);
int						exec(t_ctx *ctx);
int						exec_parent(t_ctx *ctx);
void					set_fds(t_ctx *ctx, int mode);
void					unlink_all(t_ctx *ctx);
int						err_redirs(t_exec *exec);
int						exec_redirs(t_exec *exec);
int						redirs_type(t_exec *exec, t_filenames *file);

#endif