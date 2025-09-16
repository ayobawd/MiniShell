/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_basic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:30:38 by ayal-awa          #+#    #+#             */
/*   Updated: 2025/09/16 11:30:38 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_echo(t_cmds *cmd)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (cmd->cmds[1] && ft_strncmp(cmd->cmds[1], "-n", 2) == 0
		&& cmd->cmds[1][2] == '\0')
	{
		newline = 0;
		i = 2;
	}
	while (cmd->cmds[i])
	{
		printf("%s", cmd->cmds[i]);
		if (cmd->cmds[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	builtin_env(t_shell *shell)
{
	t_list	*current;

	current = shell->environment;
	while (current)
	{
		printf("%s\n", (char *)current->content);
		current = current->next;
	}
	return (0);
}
