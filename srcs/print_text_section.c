/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_text_section.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 20:22:56 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/27 14:40:34 by hublanc          ###   ########.fr       */
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

static void		print_content(t_info info, t_sec64_list *list, uint32_t offset,
								uint64_t size)
{
	char			*content;
	uint64_t		i;
	uint64_t		addr;
	size_t			pad;

	addr = cb(info.magic, V_64, list->section->addr);
	addr = (info.magic == MH_MAGIC || info.magic == MH_CIGAM ?
			(uint32_t)addr : addr);
	pad = (info.magic == MH_MAGIC || info.magic == MH_CIGAM ? PAD_32 : PAD_64);
	content = (info.ptr) + offset;
	i = 0;
	while (cp(info, info.ptr + offset + i) && i < size)
	{
		if (i % 16 == 0)
		{
			put_hex(addr + i, pad);
			ft_putstr("\t");
		}
		put_charhex(content[i++]);
		if (info.magic == MH_MAGIC || info.magic == MH_MAGIC_64 || ((info.magic
			== MH_CIGAM || info.magic == MH_CIGAM_64) && i % 4 == 0))
			ft_putstr(" ");
		if ((i % 16 == 0) || (i >= size) || !(cp(info, info.ptr + offset + i)))
			ft_putstr("\n");
	}
}

static uint64_t	get_correct_size(t_sec64_list *list, uint32_t magic)
{
	uint64_t		ret;
	struct section	*sec32;

	ret = 0;
	sec32 = NULL;
	if (list && list->section)
	{
		if (magic == MH_CIGAM || magic == MH_MAGIC)
		{
			sec32 = (struct section*)(list->section);
			ret = sec32->size;
		}
		else
			ret = list->section->size;
		ret = cb(magic, V_64, ret);
	}
	return (ret);
}

static uint32_t	get_correct_offset(t_sec64_list *list, uint32_t magic)
{
	uint32_t		ret;
	struct section	*sec32;

	ret = 0;
	sec32 = NULL;
	if (list && list->section)
	{
		if (magic == MH_CIGAM || magic == MH_MAGIC)
		{
			sec32 = (struct section*)(list->section);
			ret = sec32->offset;
		}
		else
			ret = list->section->offset;
		ret = cb(magic, V_32, ret);
	}
	return (ret);
}

void			print_text_section(t_info info, t_sec64_list *list,
									uint32_t magic)
{
	uint64_t		size;
	uint32_t		offset;

	while (list && list->section
		&& (ft_strcmp(list->section->sectname, SECT_TEXT)))
		list = list->next;
	if (list)
	{
		size = get_correct_size(list, magic);
		offset = get_correct_offset(list, magic);
		if (size != 0 && offset != 0 && cp(info,
			info.ptr + offset + size))
		{
			ft_putstr("Contents of (__TEXT,__text) section\n");
			print_content(info, list, offset, size);
		}
	}
}
