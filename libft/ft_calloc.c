/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:18:15 by ayal-awa          #+#    #+#             */
/*   Updated: 2025/01/02 16:54:12 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

void	*ft_calloc(size_t nitems, size_t size)
{
	void	*tmp;

	if (size != 0 && nitems > SIZE_MAX / size)
		return (NULL);
	tmp = malloc(nitems * size);
	if (!tmp)
		return (NULL);
	ft_bzero(tmp, nitems * size);
	return (tmp);
}
