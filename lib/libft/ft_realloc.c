/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:56:40 by jzackiew          #+#    #+#             */
/*   Updated: 2025/01/28 11:56:51 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new;

	if (!ptr)
	{
		new = malloc(new_size);
		if (!new)
			return (NULL);
	}
	else
	{
		if (old_size < new_size)
		{
			new = malloc(new_size);
			if (!new)
				return (NULL);
			ft_memcpy(new, ptr, old_size);
		}
		else
			new = ptr;
		free(ptr);
	}
	return (new);
}
