/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:07:23 by yilin             #+#    #+#             */
/*   Updated: 2024/05/24 17:42:41 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*#include <stdlib.h>*/

static unsigned int	ft_nblen(int nb)
{
	int	nblen;

	nblen = 0;
	if (nb == 0)
		return (1);
	if (nb < 0)
		nblen += 1;
	while (nb != 0)
	{
		nb /= 10;
		nblen++;
	}
	return (nblen);
}

char	*ft_itoa(int n)
{
	unsigned int	nb;
	unsigned int	nblen;
	char			*str;

nblen = ft_nblen(n);
str = (char *)malloc((nblen + 1) * sizeof(*str));
	if (!str)
		return (NULL);
	if (n < 0)
	{
		str[0] = '-';
		nb = -n;
	}
	else
		nb = n;
	if (nb == 0)
		str[0] = '0';
	str[nblen] = '\0';
	while (nb != 0)
	{
		str[nblen -1] = (nb % 10) + '0';
		nb /= 10;
		nblen--;
	}
	return (str);
}
/*
#include <stdio.h>
int main()
{
	char	*result;

	result = ft_itoa(12356789);
	printf("%s\n", result);
}
*/