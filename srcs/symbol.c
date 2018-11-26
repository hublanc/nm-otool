/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:28:22 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/26 20:23:38 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static void		print_symt(struct nlist_64 symbol, uint32_t magic,
							struct section_64 *sect, char *c_sym)
{
	uint64_t value;

	value = (magic == MH_MAGIC || magic == MH_CIGAM ?
				(uint32_t)symbol.n_value : symbol.n_value);
	value = cb(magic, V_64, value);
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

static void		print_symbol_type(struct nlist_64 symbol, t_sec64_list *list,
									uint32_t magic)
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
	print_symt(symbol, magic, sect, c_sym);
}

static t_symbol	*store_symbols(struct symtab_command *sym,
						t_info info, uint32_t magic)
{
	t_symbol		*symbols;
	char			*str;
	struct nlist_64	*array;
	uint32_t		i;
	uint32_t		j;

	if (!(symbols = (t_symbol*)ft_memalloc(sizeof(t_symbol) *
					cb(magic, V_32, sym->nsyms))))
		return (NULL);
	array = (void*)(info.ptr) + cb(magic, V_32, sym->symoff);
	str = (void*)(info.ptr) + cb(magic, V_32, sym->stroff);
	i = 0;
	j = 0;
	while (i < cb(magic, V_32, sym->nsyms) && cp(info, array) && cp(info, str))
	{
		if (!(cb(magic, V_32, array->n_type) & N_STAB))
		{
			symbols[j].info = *array;
			symbols[j].name = str + cb(magic, V_32, array->n_un.n_strx);
			j++;
		}
		array = (struct nlist_64*)((char*)array + jump_nlist(magic));
		i++;
	}
	return (symbols);
}

static void		print_symbol_name(t_info info, char *str)
{
	size_t	len;

	len = 0;
	if (str)
	{
		while (cp(info, (str + len)) && str[len])
			len++;
		write(1, str, len);
	}
	ft_putstr("\n");
}

void			print_symbol_table(t_info info, t_sec64_list *list,
					struct symtab_command *sym, uint32_t magic)
{
	int32_t			i;
	int32_t			len;
	t_symbol		*symbols;

	i = 0;
	if (!sym)
		return ;
	symbols = store_symbols(sym, info, magic);
	len = len_symbols(symbols, sym, magic);
	sort_symbols(symbols, 0, len - 1, magic);
	while (i < len)
	{
		print_value(cb(magic, V_64, symbols[i].info.n_value),
					magic, symbols[i].info.n_type);
		print_symbol_type(symbols[i].info, list, magic);
		print_symbol_name(info, symbols[i].name);
		i++;
	}
	if (symbols)
		free(symbols);
}
