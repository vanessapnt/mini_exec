/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 10:51:16 by yilin             #+#    #+#             */
/*   Updated: 2024/06/27 14:44:07 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (size && (nmemb / size * size) != nmemb)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}
/*
void	*ft_calloc(size_t count, size_t size)
{
	void	*bloc;

	if (count == 0 || size == 0)
	{
		bloc = malloc(0);
		if (!bloc)
			return (NULL);
		return (bloc);
	}
	if ((unsigned)count != count || (unsigned)size != size)
		return (NULL);
	bloc = malloc(count * size);
	if (bloc)
		ft_bzero(bloc, count * size);
	return (bloc);
}*/