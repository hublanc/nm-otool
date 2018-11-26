/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 15:20:48 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/26 21:21:04 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <ar.h>
# include "../libft/includes/libft.h"

# define FT_SARFMAG 2
# define FT_SEFMT1	3

# define NO_ARCH	0
# define ARCH_FOUND 1
# define ALL_ARCH	2

# define PAD_32		8
# define PAD_64		16

# define V_32		0
# define V_64		1

# define O_ARG		0
# define M_ARG		1
# define OTOOL_FILE	2
# define OTOOL_OTH	3

# define OUT_BOUND	0
# define VALID_PTR	1

# define NO_CPU		0
# define VALID_CPU	1

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
	uint32_t			magic;
}						t_info;

/*
**	section.c
*/
t_sec64_list			*new_sec64(struct section_64 *section);
void					add_sec64(t_sec64_list **list, t_sec64_list *new);
void					sec64_delone(t_sec64_list **list);
void					sec64_del(t_sec64_list **list);

/*
**	fat.c
*/
void					handle_fat32(t_info info, uint32_t nfat_arch, int arg);
void					handle_fat(t_info info, int arg);

/*
**	archive.c
*/
void					handle_archive(t_info info, int arg);

/*
**	qsort.c
*/
void					sort_symbols(t_symbol *symbols, int32_t x, int32_t y,
									uint32_t magic);

/*
**	macho.c
*/
void					handle_macho(t_info info, int arg);

/*
**	symbols.c
*/
void					print_symbol_table(t_info info, t_sec64_list *list,
							struct symtab_command *sym, uint32_t magic);

/*
**	tools.c
*/
int32_t					len_symbols(t_symbol *symbols,
								struct symtab_command *sym, uint32_t magic);
uint64_t				cb(uint32_t magic, uint32_t type, uint64_t value);
int						cp(t_info info, void *ptr);
void					binary_tmp(t_info info, char *obj_ptr, int arg);

/*
**	print_value.c
*/
void					print_value(uint64_t nb, uint32_t magic,
									uint8_t n_type);
size_t					length_base(uint64_t nb, int base);
void					put_hex(uint64_t nb, size_t pad);

/*
**	jump.c
*/
uint32_t				jump_header(uint32_t magic);
uint32_t				jump_segment(uint32_t magic);
uint32_t				jump_section(uint32_t magic);
uint32_t				jump_nlist(uint32_t magic);

/*
**	print_text_section.c
*/
void					print_text_section(t_info info, t_sec64_list *list,
									uint32_t magic);

/*
**	core.c
*/
void					read_binary(t_info info, int arg);
int						core(char *filename, int arg);

#endif
