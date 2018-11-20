/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 12:44:32 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/20 20:38:58 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <stdio.h>

size_t		length_base(uint64_t nb, int base)
{
	int length;

	length = 1;
	while (nb /= base)
		length++;
	return (length);
}

void		print_value(uint64_t nb)
{
	const char		base_str[17] = "0123456789abcdef\0";
	char			buffer[100];
	size_t			index;
	size_t			length;

	ft_bzero(buffer, 100);
	length = length_base(nb, 16);
	index = 0;
	while (index < 16)
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

void	print_symbol_type(struct nlist_64 symbol, t_sec64_list *list)
{
	struct section_64	*sect;
	char				c_sym[4];
	uint8_t				i;

	i = 0;
	sect = NULL;
	bzero(c_sym, 4);
	ft_memset(c_sym, ' ', 3);
	while (i < symbol.n_sect && list)
	{
		sect = list->section;
		list = list->next;
		i++;
	}
	if ((symbol.n_type & N_TYPE) == N_UNDF)
		c_sym[1] = 'U';
	else if ((symbol.n_type & N_TYPE) == N_ABS)
		c_sym[1] = 'A';
	else if (sect && (!ft_strcmp(sect->sectname, SECT_TEXT)))
		c_sym[1] = 'T';
	else if (sect && (!ft_strcmp(sect->sectname, SECT_DATA)))
		c_sym[1] = 'D';
	else if (sect && (!ft_strcmp(sect->sectname, SECT_BSS)))
		c_sym[1] = 'B';
	else if (sect && (!ft_strcmp(sect->sectname, SECT_COMMON)))
		c_sym[1] = 'C';
	else
		c_sym[1] = 'S';
	if ((symbol.n_type & N_TYPE) == N_INDR)
		c_sym[1] = 'I';
	if (!(symbol.n_type & N_EXT))
		c_sym[1] = ft_tolower(c_sym[1]);
	ft_putstr(c_sym);
}

t_symbol	*store_symbols(struct symtab_command *sym,
						char *ptr)
{
	t_symbol		*symbols;
	char			*stringtable;
	struct nlist_64	*array;
	uint32_t		i;
	uint32_t		j;

	if (!(symbols = (t_symbol*)ft_memalloc(sizeof(t_symbol) * sym->nsyms)))
		return (NULL);
	array = (void*)ptr + sym->symoff;
	stringtable = (void*)ptr + sym->stroff;
	i = 0;
	j = 0;
	while (i < sym->nsyms)
	{
		if (!(array[i].n_type & N_STAB))
		{
			symbols[j].info = array[i];
			symbols[j].name = ft_strdup(stringtable + array[i].n_un.n_strx);
			j++;
		}
		i++;
	}
	return(symbols);
}

void	swap_symbol(t_symbol *ptr1, t_symbol *ptr2)
{
	t_symbol	tmp;

	tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

uint32_t	inside_sort(t_symbol *symbols, uint32_t x, uint32_t y)
{
	t_symbol	pivot;
	int32_t		i;
	int32_t		j;

	pivot = symbols[y];
	i = x - 1;
	j = x;
	while (j < (int32_t)y)
	{
		if (ft_strcmp(symbols[j].name, pivot.name) <= 0)
		{
			i++;
			swap_symbol(&(symbols[i]), &(symbols[j]));
		}
		j++;
	}
	swap_symbol(&(symbols[i + 1]), &(symbols[j]));
	return (i + 1);
}

void	sort_symbols(t_symbol *symbols, uint32_t x, uint32_t y)
{
	uint32_t	p;

	p = 0;
	if (x < y)
	{
		p = inside_sort(symbols, x, y);
		sort_symbols(symbols, x, p - 1);
		sort_symbols(symbols, p + 1, y);
	}
}

uint32_t	len_symbols(t_symbol *symbols)
{
	uint32_t	len;

	len = 0;
	while (symbols[len].name)
	{
		len++;
	}
	return (len);
}

void	print_symbol_table(t_sec64_list *list, struct symtab_command *sym,
							char *ptr)
{
	uint32_t		i;
	uint32_t		len;
	t_symbol		*symbols;

	i = 0;
	ft_putstr("store symbols\n");
	symbols = store_symbols(sym, ptr);
	len = len_symbols(symbols);
	ft_putstr("sort symbols\n");
	//sort_symbols(symbols, 0, len - 1);
	ft_putstr("print symbols\n");
	ft_putunbrel(sym->nsyms);
	while (i < sym->nsyms)
	{
		if (!((symbols[i].info.n_type & N_TYPE) == N_UNDF))
			print_value(symbols[i].info.n_value);
		else
			ft_putstr("                ");
		print_symbol_type(symbols[i].info, list);
		ft_putendl(symbols[i].name);
		i++;
	}
}

void	store_sections(t_sec64_list	**list, struct load_command *lc)
{
	uint32_t					i;
	struct segment_command_64	*seg;
	struct section_64			*sec;

	i = 0;
	seg = (struct segment_command_64*)lc;
	sec = (struct section_64*)(seg + 1);
	while (i < seg->nsects && sec)
	{
		add_sec64(list, new_sec64(sec));
		sec++;
		i++;
	}
}

void	handle_64(char *ptr)
{
	uint32_t				i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;	
	t_sec64_list			*sec64_list;

	i = 0;
	sec64_list = NULL;
	sym = NULL;
	header = (struct mach_header_64*)ptr;
	lc = (void*)ptr + sizeof(*header);
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
			sym = (struct symtab_command*)lc;
		if (lc->cmd == LC_SEGMENT_64)
			store_sections(&sec64_list, lc);
		i++;
		lc = (void*)lc + lc->cmdsize;
	}
	ft_putstr("printing...\n");
	print_symbol_table(sec64_list, sym, ptr);
	ft_putstr("end of printing...\n");
}

void	handle_archive(char *ptr)
{
	struct ar_hdr	*ar;
	int32_t			size_ar;
	int32_t			size_name;

	ar = (struct ar_hdr*)((void*)ptr + SARMAG);
	size_ar = ft_atoi(ar->ar_size);
	while (1)
	{
		ar = (struct ar_hdr*)((char*)ar + ((sizeof(struct ar_hdr) + size_ar)));
		size_name = ft_atoi(ar->ar_name + ft_strlen(AR_EFMT1));
		size_ar = ft_atoi(ar->ar_size);
		ft_putstr(ar->ar_name);
		read_binary((char*)ar + sizeof(struct ar_hdr) + size_name);
		ft_putstr("\n");
	}
}

void	read_binary(char *ptr)
{
	uint32_t	magic_number;
	char		*str;
	
	magic_number = *(int*)ptr;
	str = (char*)ptr;
	if (magic_number == MH_MAGIC)
	{
		ft_putendl("i m a 32 bit object file");
	}
	else if (magic_number == MH_MAGIC_64)
	{
		ft_putendl("i m a 64 bit object file");
		handle_64(ptr);
	}
	else if (magic_number == FAT_CIGAM || magic_number == FAT_MAGIC)
	{
		ft_putendl("Je susi gros (no fat shaming please)");
	}
	else if (!ft_strncmp(str, ARMAG, SARMAG))
	{
		ft_putendl("je susi une archive");
		handle_archive(ptr);
	}
	/*
	ft_putstr("FATMAGIC: ");
	ft_putunbrel(FAT_MAGIC);
	ft_putstr("FATCIGAM ");
	ft_putunbrel(FAT_CIGAM);
	ft_putstr("Actual nb: ");
	ft_putunbrel(magic_number);
	ft_putstr(str);
	*/
}

int		ft_nm(char *filename)
{
	int			fd;
	struct stat info;
	char		*ptr;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return(EXIT_FAILURE);
	if (fstat(fd, &info) == -1)
	{
		close(fd);
		return(EXIT_FAILURE);
	}
	ptr = (char*)mmap(0, info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
	{
		close(fd);
		return(EXIT_FAILURE);
	}
	read_binary(ptr);
	ft_putunbrel(info.st_size);
	munmap(ptr, info.st_size);
	close(fd);
	return (EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	int i;
	int	ret;

	i = 1;
	ret = 0;
	if (argc > 1)
	{
		while (i < argc)
		{
			ret = ft_nm(argv[i]);
			i++;
		}
	}
	return (ret);
}
