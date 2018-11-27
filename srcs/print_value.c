/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_value.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 16:40:24 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/27 13:37:44 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static void		print_space_value(uint32_t magic)
{
	if (magic == MH_MAGIC || magic == MH_CIGAM)
		ft_putstr("        ");
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		ft_putstr("                ");
}

size_t			length_base(uint64_t nb, int base)
{
	int length;

	length = 1;
	while (nb /= base)
		length++;
	return (length);
}

void			put_hex(uint64_t nb, size_t pad)
{
	const char		base_str[17] = "0123456789abcdef\0";
	char			buffer[100];
	size_t			index;
	size_t			length;

	ft_bzero(buffer, 100);
	length = length_base(nb, 16);
	index = 0;
	while (index < pad)
		buffer[index++] = '0';
	index--;
	while (nb > 0)
	{
		buffer[index] = base_str[nb % 16];
		nb /= 16;
		index--;
	}
	ft_putstr(buffer);
}

void			print_value(uint64_t nb, uint32_t magic, uint8_t n_type)
{
	size_t			pad;

	if ((n_type & N_TYPE) == N_UNDF)
		return (print_space_value(magic));
	pad = (magic == MH_MAGIC || magic == MH_CIGAM ?
			PAD_32 : PAD_64);
	nb = (magic == MH_MAGIC || magic == MH_CIGAM ?
			(uint32_t)nb : nb);
	put_hex(nb, pad);
}
