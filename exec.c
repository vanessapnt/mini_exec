#define IN 0;
#define OUT 1;

#include "exec.h"

open_pipes(int pipes_nb, int fd[pipes_nb][2])
{
    int i;

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
//we close useless fds OR
//we close last fds when finished
void close_fds(int pipes_nb, int (*fd)[2], int i, bool exec) 
{
    int j;
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
int redirs_type(char *path, int fd_tochange, e_token_type type)
{
    int fd;
    if (type == OUTFILE)
        fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    else if (type == APPEND)
        fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
    else if (type == INFILE || N_HEREDOC)
        fd = open(path, O_RDONLY, 0644);
    else {
        perror("Unknown redirection type");
        return -1;
    }
    if (fd == -1) {
        perror("open");
        return -1;
    }
    if (dup2(fd, fd_tochange) == -1) 
    {
        perror("dup2");
        close(fd);
        return -1;
    }
    close(fd);
    return (0);
}


void exec_handle_redir(t_ctx *ctx);
{
    t_exec *temp;
    t_filenames *redir;
    e_token_type *type;
    int j;

    j = 0;
    temp = ctx->exec;
    while (j < i)
    {
        temp = temp->next;
        i++;
    }
    redir = temp->redirs;
    while(redir)
    {
        type = redir->type; 
        if (type == INFILE || N_HEREDOC)
            redirs_type(redir->path, IN, type);
        if (type == OUTFILE || APPEND)
            redirs_type(redir->path, OUT, type);
        redir = redir->next;
    }
}

child_process(t_ctx *ctx, int (*fd)[2], int i);
{
    bool exec;

    close_fds((ctx->exec_count) + 1, fd, i, false);
    dup2(fd[i][0], IN);
	dup2(fd[i + 1][1], OUT);
    exec_handle_redir(ctx, IN, OUT);
    if(ft_exec() == -1)
    {
        printf("Error with exec in process %d\n", i);
        exit(1);
    }
    close_fds((ctx->exec_count) + 1, fd, i, true);
}

void	exec(t_ctx *ctx)
{
    int fd[(ctx->exec_count) + 1][2];
    int i;
    t_exec *temp;

    //we open cmd_nb + 1 pipes
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
            child_process(ctx, fd, i);
        i++;
        temp = temp->next;
    }
}
