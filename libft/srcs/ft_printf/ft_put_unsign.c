/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_unsign.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 20:18:03 by yilin             #+#    #+#             */
/*   Updated: 2024/06/03 19:53:03 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// int	ft_putchar(char a);
// int	ft_put_int(long int nb);
int	ft_put_unsign(unsigned int nb)
{
	int	count;

	count = 0;
	if (nb >= 10)
	{
		count += ft_put_int(nb / 10);
		count += ft_put_int(nb % 10);
	}
	else
		count += ft_put_char(nb + 48);
	return (count);
}
