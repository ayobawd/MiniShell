/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 19:31:13 by ayal-awa          #+#    #+#             */
/*   Updated: 2025/01/06 15:26:07 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	be_free(char **str, int index)
{
	int	i;

	i = 0;
	while (i <= index)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

static int	num_of_words(char const *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && str[i] == c)
			i++;
		if (str[i] != '\0')
			count++;
		while (str[i] != '\0' && str[i] != c)
			i++;
	}
	return (count);
}

static int	strlen_sep(char const *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != c)
		i++;
	return (i);
}

static char	*alloc_word(char const *str, char c)
{
	int		i;
	int		word_len;
	char	*final;

	word_len = strlen_sep(str, c);
	final = (char *)malloc(sizeof(char) * (word_len + 1));
	if (!final)
		return (NULL);
	i = 0;
	while (i < word_len)
	{
		final[i] = str[i];
		i++;
	}
	final[i] = '\0';
	return (final);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**arr;

	arr = (char **)ft_calloc((num_of_words(s, c) + 1), sizeof(char *));
	i = 0;
	if (arr == NULL || s == NULL)
		return (NULL);
	while (*s != '\0')
	{
		while (*s != '\0' && *s == c)
			s++;
		if (*s != '\0')
		{
			arr[i] = alloc_word(s, c);
			if (arr[i] == NULL)
			{
				be_free(arr, i);
				return (NULL);
			}
			i++;
		}
		while (*s != '\0' && *s != c)
			s++;
	}
	return (arr);
}
