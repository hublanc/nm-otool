# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hublanc <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/24 15:04:09 by hublanc           #+#    #+#              #
#    Updated: 2018/11/26 21:00:53 by hublanc          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	Variables
FT_NM   = ft_nm
FT_OTOOL= ft_otool
CC      = gcc
FLAGS   = -Wall -Wextra -Werror -g
LIB     = libft/libft.a
HEADER  = includes/
LIBSRC  = libft/
SRCDIR  = srcs/
OBJDIR	= objs/
SRC		= sec64.c fat.c archive.c qsort.c\
		  macho.c symbol.c tools.c jump.c print_value.c\
		  core.c print_text_section.c
SRCS	= $(addprefix $(SRCDIR), $(SRC))
OBJS	= $(addprefix $(OBJDIR), $(SRC:.c=.o))

#	Colors
GRN     =   \033[0;32m
RED     =   \033[0;31m
CYN     =   \033[0;36m
NC      =   \033[0m
	
all: $(OBJDIR) $(FT_NM) $(FT_OTOOL)

$(FT_NM): $(LIB) $(OBJS)
	$(CC) $(FLAGS) -c -o $(OBJDIR)$(FT_NM).o $(SRCDIR)$(FT_NM).c -I $(HEADER)
	$(CC) $(FLAGS) $(LIBSRC)/libft.a -o $(FT_NM) $(OBJS) $(OBJDIR)$(FT_NM).o
	@echo "\n${CYN}PROCESSING DONE !${NC}"

$(FT_OTOOL): $(LIB) $(OBJS)
	$(CC) $(FLAGS) -c -o $(OBJDIR)$(FT_OTOOL).o $(SRCDIR)$(FT_OTOOL).c -I $(HEADER)
	$(CC) $(FLAGS) $(LIBSRC)/libft.a -o $(FT_OTOOL) $(OBJS) $(OBJDIR)$(FT_OTOOL).o
	@echo "\n${CYN}PROCESSING DONE !${NC}"

$(OBJDIR):
	@mkdir -p objs

$(LIB):
	@echo "${CYN}Processing ${NC}./libft/objs ${CYN}[${NC}...${CYN}]${NC}"
	@make -C $(LIBSRC)
	@echo "\n${CYN}Processing ${NC}./objs ${CYN}[${NC}...${CYN}]${NC}"

$(OBJDIR)%.o: $(SRCDIR)%.c $(HEADER)/ft_nm.h
	@echo "${GRN}Compiling ${NC} $@"
	$(CC) $(FLAGS) -c -o $@ $< -I $(HEADER)

clean:
	@echo "${RED}Cleaning ${NC}./libft/objs/ ${RED}[${NC}...${RED}]${NC}"
	@rm -rf $(OBJS)

fclean: clean
	@echo "${RED}Cleaning ${NC}./libft/${RED}libft.h${NC}\n"
	@rm -rf $(FT_NM) $(FT_OTOOL)
	@rm -rf $(LINK)
	@echo "${RED}DELETE DONE !${NC}"

re: fclean all

.PHONY: all clean fclean re
