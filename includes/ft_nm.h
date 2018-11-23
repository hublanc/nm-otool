/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 15:20:48 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/23 18:37:32 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <ar.h>
#include "../libft/includes/libft.h"

# define FT_SARFMAG 2

# define NO_ARCH	0
# define ARCH_FOUND 1
# define ALL_ARCH	2

# define PAD_32		8
# define PAD_64		16

# define V_32		0
# define V_64		1

# define O_ARG		0
# define M_ARG		1

typedef struct			s_sec64_list
{
	struct section_64	*section;
	struct s_sec64_list	*next;
}						t_sec64_list;

typedef struct			s_symbol
{
	char				*name;
	struct nlist_64		info;
}						t_symbol;

typedef struct			s_info
{
	char				*ptr;
	off_t				size;
	char				*filename;
}						t_info;

//section.c
t_sec64_list			*new_sec64(struct section_64 *section);
void					add_sec64(t_sec64_list **list, t_sec64_list *new);
void					sec64_delone(t_sec64_list **list);
void					sec64_del(t_sec64_list **list);

//fat.c
void					handle_fat32(char *ptr, off_t size, char *filename,
									uint32_t nfat_arch);
void					handle_fat(char *ptr, off_t size, char *filename);

//archive.c
void					handle_archive(char *ptr, off_t size, char *filename);

//qsort.c
void					sort_symbols(t_symbol *symbols, int32_t x, int32_t y,
									uint32_t magic);

//mach-o.c
void					handle_macho(char *ptr, off_t size, char *filename, int arg);

//symbol.c
void					print_symbol_table(t_sec64_list *list,
							struct symtab_command *sym, char *ptr, uint32_t magic);

//tools.c
int32_t					len_symbols(t_symbol *symbols,
								struct symtab_command *sym, uint32_t magic);
void					print_value(uint64_t nb, uint32_t magic,
									uint8_t n_type);
size_t					length_base(uint64_t nb, int base);
uint64_t				cb(uint32_t magic, uint32_t type, uint64_t value);

//nm.c
void					read_binary(char *ptr, off_t size, char *filename, int arg);

#endif
