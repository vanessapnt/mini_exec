/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_int.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:31:15 by yilin             #+#    #+#             */
/*   Updated: 2024/06/03 19:52:28 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// int	ft_putchar(char a);
// #include <unistd.h>
// int	ft_putchar(char a)
// {
// 	return (write(1, &a, 1));
// }

int	ft_put_int(long int nb)
{
	int	count;

	count = 0;
	if (nb < 0)
	{
		count += ft_put_char('-');
		nb = nb * (-1);
	}
	if (nb >= 10)
	{
		count += ft_put_int(nb / 10);
		count += ft_put_int(nb % 10);
	}
	else
		count += ft_put_char(nb + 48);
	return (count);
}
// #include <stdio.h>
// int main()
// {
// 	int	test;

// 	test = ft_printf_int(-123456);
// 	// printf("test: %i\n", test);
// }