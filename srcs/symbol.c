/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:28:22 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/22 22:17:01 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static void		print_symbol_type(struct nlist_64 symbol, t_sec64_list *list,
									uint32_t magic)
{
	struct section_64	*sect;
	char				c_sym[4];
	uint8_t				i;
	uint64_t			value;

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
	value = (magic == MH_MAGIC || magic == MH_CIGAM ?
				(uint32_t)symbol.n_value : symbol.n_value);
	if ((symbol.n_type & N_TYPE) == N_UNDF && (value == 0))
		c_sym[1] = 'U';
	else if ((symbol.n_type & N_TYPE) == N_ABS)
		c_sym[1] = 'A';
	else if (sect && (!ft_strcmp(sect->sectname, SECT_TEXT)))
		c_sym[1] = 'T';
	else if (sect && (!ft_strcmp(sect->sectname, SECT_DATA)))
		c_sym[1] = 'D';
	else if (sect && (!ft_strcmp(sect->sectname, SECT_BSS)))
		c_sym[1] = 'B';
	else if ((symbol.n_type & N_TYPE) == N_UNDF
			&& symbol.n_type & N_EXT && value != 0)
		c_sym[1] = 'C';
	else
		c_sym[1] = 'S';
	if ((symbol.n_type & N_TYPE) == N_INDR)
		c_sym[1] = 'I';
	if (!(symbol.n_type & N_EXT))
		c_sym[1] = ft_tolower(c_sym[1]);
	ft_putstr(c_sym);
}

static uint32_t	jump_nlist(uint32_t magic)
{
	uint32_t	size;

	size = 0;
	if (magic == MH_MAGIC || magic == MH_CIGAM)
		size = sizeof(struct nlist);
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		size = sizeof(struct nlist_64);
	return (size);
}

static t_symbol	*store_symbols(struct symtab_command *sym,
						char *ptr, uint32_t magic)
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
		if (!(array->n_type & N_STAB))
		{
			symbols[j].info = *array;
			symbols[j].name = stringtable + array->n_un.n_strx;
			j++;
		}
		array = (struct nlist_64*)((char*)array +jump_nlist(magic));
		i++;
	}
	return(symbols);
}

void			print_symbol_table(t_sec64_list *list,
						struct symtab_command *sym, char *ptr, uint32_t magic)
{
	int32_t			i;
	int32_t			len;
	t_symbol		*symbols;

	i = 0;
	symbols = store_symbols(sym, ptr, magic);
	len = len_symbols(symbols, sym);
	sort_symbols(symbols, 0, len - 1);
	while (i < len)
	{
		print_value(symbols[i].info.n_value, magic, symbols[i].info.n_type);
		print_symbol_type(symbols[i].info, list, magic);
		ft_putendl(symbols[i].name);
		i++;
	}
}
