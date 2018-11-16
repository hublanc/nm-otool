# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hublanc <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/24 15:04:09 by hublanc           #+#    #+#              #
#    Updated: 2018/11/16 14:41:40 by hublanc          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	Variables
NAME    = ft_nm
CC      = gcc
FLAGS   = -Wall -Wextra -Werror 
LIB     = libft/libft.a
HEADER  = includes/
LIBSRC  = libft/
SRCDIR  = srcs/
OBJDIR	= objs/
SRC		= ft_nm.c sec64.c
SRCS	= $(addprefix $(SRCDIR), $(SRC))
OBJS	= $(addprefix $(OBJDIR), $(SRC:.c=.o))

#	Colors
GRN     =   \033[0;32m
RED     =   \033[0;31m
CYN     =   \033[0;36m
NC      =   \033[0m
	
all: $(OBJDIR) $(NAME)

$(NAME): $(LIB) $(OBJS)
	$(CC) $(FLAGS) $(LIBSRC)/libft.a -o $(NAME) $(OBJS)
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
	@rm -rf $(NAME)
	@rm -rf $(LINK)
	@echo "${RED}DELETE DONE !${NC}"

re: fclean all

.PHONY: all clean fclean re
