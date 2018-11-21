/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 15:20:48 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/21 15:19:56 by hublanc          ###   ########.fr       */
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

//section.c
t_sec64_list			*new_sec64(struct section_64 *section);
void					add_sec64(t_sec64_list **list, t_sec64_list *new);
void					sec64_delone(t_sec64_list **list);
void					sec64_del(t_sec64_list **list);

//nm.c
void	read_binary(char *ptr, off_t size, char *filename);


#endif
