/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_ptr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:02:43 by yilin             #+#    #+#             */
/*   Updated: 2024/06/05 15:35:31 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
// int	ft_putchar(char c);
// int	ft_putstr(char *str);

static int	ft_put_ptrhex(unsigned long int nb, char xX)
{
	char				*base;
	int					count;
	unsigned long int	base_len;

	count = 0;
	base_len = 16;
	if (xX == 'x')
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	if (nb < base_len)
		count += ft_put_char(base[nb % base_len]);
	else
	{
		count += ft_put_ptrhex(nb / base_len, xX);
		count += ft_put_ptrhex(nb % base_len, xX);
	}
	return (count);
}

int	ft_put_ptr(void *addr)
{
	int					count;
	unsigned long int	ptr;

	count = 0;
	ptr = (unsigned long int)addr;
	if (!ptr)
		return (ft_put_str("(nil)"));
	count += ft_put_str("0x");
	count += ft_put_ptrhex(ptr, 'x');
	return (count);
}

// #include <stdio.h>
// int main()
// {
// 	int result;
// 	unsigned long int	*test;

// 	result = ft_put_ptr(test);
// 	printf("%p\n", &result);
// }