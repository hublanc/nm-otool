/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 12:44:32 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/14 19:33:48 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <stdio.h>

void	print_symbol_table(uint32_t nsyms, uint32_t symoff, uint32_t stroff,
							char *ptr)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;

	i = 0;
	array = (void*)ptr + symoff;
	stringtable = (void*)ptr + stroff;
	while (i < nsyms)
	{
		/*
		if (array->n_type & N_STAB)
			ft_putendl("STAB");
		if (array->n_type & N_PEXT)
			ft_putendl("PEXT");
		if (array->n_type & N_TYPE)
			ft_putendl("TYPE");
		if (array->n_type & N_EXT)
			ft_putendl("EXT");
		*/
		if (!(array[i].n_type & N_STAB))
		{
			if (array[i].n_type & N_UNDF)
				ft_putendl("UNDEFINED");
			if (array[i].n_type & N_ABS)
				ft_putendl("ABSOLUTE");
			if (array[i].n_type & N_SECT)
				ft_putendl("SECTION");
			if (array[i].n_type & N_PBUD)
				ft_putendl("PREBOUND UNDEFINED");
			if (array[i].n_type & N_INDR)
				ft_putendl("INDIRECT");
			printf("%s\n", stringtable + array[i].n_un.n_strx);
		}
		i++;
	}
}

void	handle_64(char *ptr)
{
	uint32_t				i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;	

	i = 0;
	header = (struct mach_header_64*)ptr;
	lc = (void*)ptr + sizeof(*header);
	while (i < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			print_symbol_table(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break;
		}
		i++;
		lc = (void*)lc + lc->cmdsize;
	}
}

void	read_binary(char *ptr)
{
	uint32_t	magic_number;
	
	magic_number = *(int*)ptr;
	if (magic_number == MH_MAGIC)
	{
		ft_putendl("i m a 32 bit object file");
	}
	else if (magic_number == MH_MAGIC_64)
	{
		ft_putendl("i m a 64 bit object file");
		handle_64(ptr);
	}
}

void ft_nm(char *filename)
{
	int			fd;
	struct stat info;
	char		*ptr;

	if ((fd = open(filename, O_RDONLY)) == -1)
		exit(EXIT_FAILURE);
	if (fstat(fd, &info) == -1)
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
	ptr = mmap(0, info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
	read_binary(ptr);
	munmap(ptr, info.st_size);
	close(fd);
}

int main(int argc, char *argv[])
{
	int i;

	i = 1;
	if (argc > 1)
	{
		while (i < argc)
		{
			ft_nm(argv[i]);
			i++;
		}
	}
	return 0;
}
