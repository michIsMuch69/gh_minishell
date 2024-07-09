# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/11 17:36:11 by fberthou          #+#    #+#              #
#    Updated: 2024/05/28 07:38:48 by jedusser         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RESET_COLOR	= \033[0m
GREEN		= \033[32m
YELLOW		= \033[33m

NAME		= libft.a

SRC_DIR		= ./src
HDR_DIR		= ./hdr
BUILD_DIR	= .build

CFLAGS		= -Wall -Wextra -Werror -I$(HDR_DIR)

SRC =		ft_atoi.c				\
			ft_bzero.c				\
			ft_calloc.c				\
			ft_isalnum.c			\
			ft_isalpha.c			\
			ft_isascii.c			\
			ft_isdigit.c			\
			ft_isprint.c			\
			ft_itoa.c				\
			ft_lstadd_back_bonus.c	\
			ft_lstadd_front_bonus.c	\
			ft_lstclear_bonus.c		\
			ft_lstdelone_bonus.c	\
			ft_lstiter_bonus.c		\
			ft_lstlast_bonus.c		\
			ft_lstmap_bonus.c		\
			ft_lstnew_bonus.c		\
			ft_lstsize_bonus.c		\
			ft_memchr.c				\
			ft_memcmp.c				\
			ft_memcpy.c				\
			ft_memmove.c			\
			ft_memset.c				\
			ft_printf_utils.c		\
			ft_printf.c				\
			ft_putchar_fd.c			\
			ft_putendl_fd.c			\
			ft_putnbr_fd.c			\
			ft_putstr_fd.c			\
			ft_realloc.c			\
			ft_split.c				\
			ft_strchr.c				\
			ft_strdup.c				\
			ft_striteri.c			\
			ft_strjoin.c			\
			ft_strcat.c				\
			ft_strlcat.c			\
			ft_strcpy.c				\
			ft_strlcpy.c			\
			ft_strlen.c				\
			ft_strmapi.c			\
			ft_strncmp.c			\
			ft_strnstr.c			\
			ft_strrchr.c			\
			ft_strtrim.c			\
			ft_substr.c				\
			ft_tolower.c			\
			ft_toupper.c			\
			get_next_line.c			\
			get_next_line_utils.c

OBJ		= $(SRC:%.c=$(BUILD_DIR)/%.o)

# --- CALL --- #
default : all
all : $(NAME)

# --- ARCHIVE --- #
$(NAME) : $(OBJ)
	@ar -rcs $@ $^
	@echo "$(GREEN)-- generated archive --$(RESET_COLOR)"

# --- OBJECTS --- #
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --- CLEAN MANAGEMENT --- #
clean :
	@rm -rf $(BUILD_DIR)

fclean : clean
	@rm -f $(NAME)
	@echo "$(YELLOW)-- removed archived --$(RESET_COLOR)"

re : fclean all

.PHONY :
	all clean fclean re

