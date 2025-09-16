#include "minishell.h"

static void free_redirects(t_cmds *cmd)
{
	int i;

	i = 0;
	while (i < cmd->red_len)
	{
		if (cmd->outs && cmd->outs[i].file_name)
			free(cmd->outs[i].file_name);
		i++;
	}
	free(cmd->outs);
}

void	free_strings(char **av)
{
	int i;

	if (!av)
		return;
	i = 0;
	while (av[i])
		free(av[i++]);
	free(av);
}

void	free_all(t_shell *pipe, t_cmds *cmd)
{
	int		i;
	t_list	*tmp;

	if (pipe && pipe->cmds)
	{
		i = 0;
		while (i < pipe->cmd_len)
			free(pipe->cmds[i++]);
		free(pipe->cmds);
		pipe->cmds = NULL;
	}
	if (cmd)
	{
		i = 0;
		while (i < pipe->cmd_len)
		{
			free_strings(cmd[i].cmds);
			if (cmd[i].outs)
				free_redirects(&cmd[i]);
			i++;
		}
		free(cmd);
	}
	while (pipe && pipe->environment)
	{
		tmp = pipe->environment->next;
		// environment strings are borrowed from envp; do not free content
		free(pipe->environment);
		pipe->environment = tmp;
	}
}
