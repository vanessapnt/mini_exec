/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 17:13:34 by yilin             #+#    #+#             */
/*   Updated: 2024/05/18 17:43:35 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*#include <stdlib.h>*/

void	*ft_memset(void *block, int value, size_t n)
{
	char	*ptr;

	ptr = (char *)block;
	while (n > 0)
	{
		ptr[n - 1] = value;
		n--;
	}
	return (block);
}
/*
#include <stdio.h>
int	main()
{
	char	str[5];

	ft_memset(str, 'A', 4);
	str[4] = '\0';
	printf("%s\n", str);
}
*/