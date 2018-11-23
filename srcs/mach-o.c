/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach-o.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:24:22 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/23 18:46:05 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static uint32_t	jump_header(uint32_t magic)
{
	uint32_t	size;

	size = 0;
	if (magic == MH_MAGIC || magic == MH_CIGAM)
		size = sizeof(struct mach_header);
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		size = sizeof(struct mach_header_64);
	return (size);
}

static uint32_t	jump_segment(uint32_t magic)
{
	uint32_t	size;

	size = 0;
	if (magic == MH_MAGIC || magic == MH_CIGAM)
		size = sizeof(struct segment_command);
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		size = sizeof(struct segment_command_64);
	return (size);
}

static uint32_t	jump_section(uint32_t magic)
{
	uint32_t	size;

	size = 0;
	if (magic == MH_MAGIC || magic == MH_CIGAM)
		size = sizeof(struct section);
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		size = sizeof(struct section_64);
	return (size);
}

static uint32_t	get_nsects(struct load_command *lc, uint32_t magic)
{
	struct segment_command_64	*seg64;
	struct segment_command		*seg;
	uint32_t					nsects;

	seg64 = NULL;
	seg = NULL;
	nsects = 0;
	if (magic == MH_MAGIC || magic == MH_CIGAM)
	{
		seg = (struct segment_command*)lc;
		nsects = cb(magic, V_32, seg->nsects);
	}
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
	{
		seg64 = (struct segment_command_64*)lc;
		nsects = cb(magic, V_32, seg64->nsects);
	}
	return (nsects);
}

static void	store_sections(t_sec64_list	**list, struct load_command *lc, uint32_t magic)
{
	uint32_t					i;
	uint32_t					nsects;
	struct section_64			*sec;

	i = 0;
	nsects = get_nsects(lc, magic);
	sec = (struct section_64*)((char*)lc + jump_segment(magic));
	while (i < nsects && sec)
	{
		add_sec64(list, new_sec64(sec));
		sec = (struct section_64*)((char*)sec + jump_section(magic));
		i++;
	}
}

void		print_filename(char *filename, int arg)
{
	if (arg == M_ARG)
	{
		ft_putstr("\n");
		ft_putstr(filename);
		ft_putstr(":\n");
	}
}

void		handle_macho(t_info info, int arg)
{
	uint32_t				i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;	
	t_sec64_list			*sec64_list;

	i = 0;
	sec64_list = NULL;
	sym = NULL;
	header = (struct mach_header_64*)(info.ptr);
	lc = (void*)(info.ptr) + jump_header(header->magic);
	print_filename(info.filename, arg);
	while (i < cb(header->magic, V_32, header->ncmds)
		&& (void*)lc < (void*)(info.ptr + info.size))
	{
		if (cb(header->magic, V_32, lc->cmd) == LC_SYMTAB)
			sym = (struct symtab_command*)lc;
		if (cb(header->magic, V_32, lc->cmd) == LC_SEGMENT_64
			|| lc->cmd == LC_SEGMENT)
			store_sections(info, &sec64_list, lc, header->magic);
		i++;
		lc = (void*)lc + cb(header->magic, V_32, lc->cmdsize);
	}
	print_symbol_table(info, sec64_list, sym, header->magic);
}
