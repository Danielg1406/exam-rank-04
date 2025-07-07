#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int	picoshell(char **cmds[])
{
	int	count_cmds;
	int	result;
	int	old_fd;
	int	pipe_fd[2];
	int	status;
	int	i;
	pid_t	pid;

	count_cmds = 0;
	result = 0;
	old_fd = -1;
	i = 0;
	while (cmds[count_cmds] != NULL)
		count_cmds++;
	while (i < count_cmds)
	{
		if (i < count_cmds - 1)
		{
			if (pipe(pipe_fd) == -1)
			{
				if (old_fd != -1)
					close(old_fd);
				return (1);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			if (i < count_cmds -1)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			if (old_fd != -1)
				close(old_fd);
			return (1);
		}
		// Child
		if (pid == 0)
		{
			if (old_fd != -1)
			{
				dup2(old_fd, STDIN_FILENO);
				close(old_fd);
			}
			if (i < count_cmds - 1)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[0]);
				close(pipe_fd[1]);

			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		// Parent
		if (old_fd != -1)
			close(old_fd);
		if (i < count_cmds - 1)
		{
			close(pipe_fd[1]);
			old_fd = pipe_fd[0];
		}
		i++;
	}
	// Wait for children
	i = 0;
	while (i < count_cmds)
	{
		if (wait(&status) == -1)
			result = 1;
		else
		{
			if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
				result = 1;
			else if (WIFSIGNALED(status))
				result = -1;
		}
		i++;
	}
	return (result);
}

int main() 
{
	// Example usage: picoshell with commands
	// char *cmd1[] = {"/bin/echo","hello", "\n","berlin", "|" };
	// char *cmd2[] = {"/usr/bin/wc", NULL};

	char *cmd1[] = {"/bin/ls", NULL, "|" };
	char *cmd2[] = { "/usr/bin/grep", "picoshell", NULL};
	char **cmds[] = {cmd1, cmd2, NULL};

	// char *cmd1[] = {"/bin/ls", "-l", NULL };
	// char *cmd2[] = { "bin/cat", "-e", NULL};
	// char **cmds[] = {cmd1, cmd2, NULL};

	picoshell(cmds);
	return 0;
}
