/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 16:55:27 by ayal-awa          #+#    #+#             */
/*   Updated: 2025/01/02 16:56:16 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	str_len;
	size_t	actual_len;

	if (!s)
		return (NULL);
	str_len = ft_strlen(s);
	if (start >= str_len)
		return (ft_strdup(""));
	actual_len = str_len - start;
	if (actual_len > len)
		actual_len = len;
	sub = (char *)malloc(sizeof(char) * (actual_len + 1));
	if (!sub)
		return (NULL);
	ft_strlcpy(sub, s + start, actual_len + 1);
	return (sub);
}
