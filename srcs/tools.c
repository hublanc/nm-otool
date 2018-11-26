/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:34:32 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/26 21:24:07 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

int32_t		len_symbols(t_symbol *symbols, struct symtab_command *sym,
						uint32_t magic)
{
	uint32_t	len;

	len = 0;
	while ((symbols[len].name)
			&& (len < cb(magic, V_32, sym->nsyms)))
	{
		len++;
	}
	return (len);
}

uint64_t	cb(uint32_t magic, uint32_t type, uint64_t value)
{
	if (magic == MH_CIGAM)
		value = OSSwapInt32(value);
	else if (magic == MH_CIGAM_64)
	{
		if (type == V_32)
			value = OSSwapInt32(value);
		else if (type == V_64)
			value = OSSwapInt64(value);
	}
	return (value);
}

int			cp(t_info info, void *ptr1)
{
	int		ret;

	ret = OUT_BOUND;
	if ((void*)(info.ptr + info.size) > ptr1)
		ret = VALID_PTR;
	return (ret);
}

void		binary_tmp(t_info info, char *obj_ptr, int arg)
{
	t_info	tmp;

	tmp.ptr = obj_ptr;
	tmp.size = info.size;
	tmp.filename = info.filename;
	if (cp(info, obj_ptr))
	{
		if (arg == O_ARG || arg == M_ARG)
			read_binary(tmp, O_ARG);
		else if (arg == OTOOL_FILE || arg == OTOOL_OTH)
			read_binary(tmp, OTOOL_OTH);
	}
}
