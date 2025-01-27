/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:52:11 by agarbacz          #+#    #+#             */
/*   Updated: 2024/12/03 18:19:29 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;
	size_t	total;

	if (!nmemb || !size)
		return (malloc(0));
	if (nmemb && (size * nmemb) / nmemb != size)
		return (NULL);
	total = nmemb * size;
	p = (void *)malloc(total);
	if (!p)
		return (NULL);
	ft_bzero(p, nmemb * size);
	return (p);
}
