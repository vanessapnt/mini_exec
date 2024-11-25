/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 20:57:29 by yilin             #+#    #+#             */
/*   Updated: 2024/06/05 15:34:45 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// int	ft_putchar(char c);
// #include <unistd.h>
// int	ft_putchar(char a)
// {
// 	return (write(1, &a, 1));
// }
int	ft_put_hex(unsigned int nb, char xX)
{
	int				count;
	char			*nb_base;
	unsigned int	base_len;

	count = 0;
	base_len = 16;
	if (xX == 'x')
		nb_base = "0123456789abcdef";
	else
		nb_base = "0123456789ABCDEF";
	if (nb < base_len)
		count += ft_put_char(nb_base[nb % base_len]);
	else
	{
		count += ft_put_hex((nb / base_len), xX);
		count += ft_put_hex((nb % base_len), xX);
	}
	return (count);
}
/*
#include <stdio.h>
int main()
{
	unsigned int n;

	n = 1234;

	printf("%x\n", ft_put_hex(n, 'x'));
	printf("%X\n", ft_put_hex(n, 'X'));
}*/