/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_text_section.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 20:22:56 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/26 21:36:36 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static void		put_charhex(uint64_t nb)
{
	const char		base_str[17] = "0123456789abcdef\0";
	char			buffer[3];

	ft_bzero(buffer, 3);
	buffer[1] = base_str[nb % 16];
	nb /= 16;
	buffer[0] = base_str[nb % 16];
	ft_putstr(buffer);
}

static void		print_content(t_info info, t_sec64_list *list, uint64_t addr,
								uint64_t size)
{
	char			*content;
	uint64_t		i;
	size_t			pad;

	pad = (info.magic == MH_MAGIC || info.magic == MH_CIGAM ?
			PAD_32 : PAD_64);
	content = (info.ptr) + cb(info.magic, V_32, list->section->offset);
	i = 0;
	while (i < size)
	{
		if (i % 16 == 0)
		{
			put_hex(addr + i, pad);
			ft_putstr("\t");
		}
		put_charhex(content[i]);
		ft_putstr(" ");
		i++;
		if ((i % 16 == 0) || (i >= size))
			ft_putstr("\n");
	}
}

void			print_text_section(t_info info, t_sec64_list *list,
									uint32_t magic)
{
	uint64_t		size;
	uint64_t		addr;

	while (list && list->section
		&& (ft_strcmp(list->section->sectname, SECT_TEXT)))
		list = list->next;
	if (list)
	{
		size = cb(magic, V_64, list->section->size);
		size = (magic == MH_MAGIC || magic == MH_CIGAM ?
				(uint32_t)size : size);
		addr = cb(magic, V_64, list->section->addr);
		addr = (magic == MH_MAGIC || magic == MH_CIGAM ?
				(uint32_t)addr : addr);
		if (cp(info, (info.ptr) + cb(magic, V_32,
						list->section->offset) + size))
		{
			ft_putstr("Contents of (__TEXT,__text) section\n");
			print_content(info, list, addr, size);
		}
	}
}
