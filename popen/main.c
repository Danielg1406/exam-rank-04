#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "get_next_line.h"

int	ft_popen(const char *file, const char *argv[], char type)
{
	int		pipefd[2];
	pid_t	pid;
	int		fd;

	if (type != 'r' && type != 'w')
		return (-1);
	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		/* Child */
		if (type == 'r')
		{
			/* Parent will read; child must send stdout → pipefd[1] */
			close(pipefd[0]); /* close unused read end */
			if (dup2(pipefd[1], STDOUT_FILENO) == -1)
				exit(EXIT_FAILURE);
			close(pipefd[1]);
		}
		else
		{
			/* type == 'w' */
			/* Parent will write; child must receive stdin ← pipefd[0] */
			close(pipefd[1]); /* close unused write end */
			if (dup2(pipefd[0], STDIN_FILENO) == -1)
				exit(EXIT_FAILURE);
			close(pipefd[0]);
		}
		execvp(file, (char *const *)argv);
		/* If execvp fails: */
		exit(EXIT_FAILURE);
	}
	/* Parent */
	if (type == 'r')
	{
		/* Parent reads; close write end */
		close(pipefd[1]);
		fd = pipefd[0];
	}
	else
	{
		/* Parent writes; close read end */
		close(pipefd[0]);
		fd = pipefd[1];
	}
	return (fd);
}

int main(void)
{
    int     fd;
    char    *line;

    fd = ft_popen("ls", (const char *[]) {"ls", NULL}, 'r');
    while (line = get_next_line(fd))
        ft_putstr(line);
}
