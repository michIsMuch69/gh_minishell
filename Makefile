# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: florian <florian@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/03 11:07:17 by fberthou          #+#    #+#              #
#    Updated: 2024/07/02 13:17:24 by florian          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- SET COLORS --- #
RESET_COLOR	= \033[0m
GREEN		= \033[32m
YELLOW		= \033[33m

# --- PROG NAME --- #
NAME		= minishell
LIBFT		= $(LIBFT_PATH)/libft.a

# --- SET PATHS --- #
#PROJECT#
HDR_PATH				= headers
SRC_PATH				= sources
MAIN_PATH				= $(SRC_PATH)/main
PARSING_PATH			= $(SRC_PATH)/parsing
EXEC_PATH				= $(SRC_PATH)/exec
#LIBFT#
LIBFT_PATH				= libft
LIBFT_HDR_PATH			= $(LIBFT_PATH)/hdr

#TEMPORARY FILES#
BUILD_PATH				= .build
BUILD_MAIN_PATH			= $(BUILD_PATH)/main
BUILD_PARS_PATH			= $(BUILD_PATH)/parsing
BUILD_EXEC_PATH			= $(BUILD_PATH)/exec

# --- COMPILATION FLAGS --- #
LIB_FLAGS	=	-I$(LIBFT_HDR_PATH)
COMPFLAGS	=	-I$(HDR_PATH) $(LIB_FLAGS) -g # -Wall -Wextra -Werror

SRC		=	$(MAIN_PATH)/main.c $(MAIN_PATH)/utils.c \
			\
			$(PARSING_PATH)/parsing.c $(PARSING_PATH)/parsing_utils.c \
			$(PARSING_PATH)/pre_treatment.c $(PARSING_PATH)/pre_treatment_utils.c \
			$(PARSING_PATH)/tokenizer.c \
			$(PARSING_PATH)/cleaner.c $(PARSING_PATH)/quote_management.c \
			$(PARSING_PATH)/expand.c $(PARSING_PATH)/expand_utils.c $(PARSING_PATH)/expand_utils_nd.c \
			$(PARSING_PATH)/getenv.c \
			$(PARSING_PATH)/struct_filling.c \
			\
			$(EXEC_PATH)/exec.c $(EXEC_PATH)/init_exec.c $(EXEC_PATH)/build_exec_path.c $(EXEC_PATH)/exec_utils.c \
			$(EXEC_PATH)/redirections.c $(EXEC_PATH)/redirections_utils.c $(EXEC_PATH)/fds_management.c \
			$(EXEC_PATH)/builtins.c	$(EXEC_PATH)/builtins_utils.c $(EXEC_PATH)/heredoc.c \
			$(EXEC_PATH)/pipe.c

### ---- TEMPORARY FILES ---- ###
OBJ	= $(SRC:$(SRC_PATH)/%.c=$(BUILD_PATH)/%.o)

### --- CALL --- ###
default	: all
all		: $(LIBFT) $(NAME)

### --- MINISHELL --- ###
$(NAME)	: $(OBJ) $(LIBFT)
	$(CC) $(COMPFLAGS)  $^ -lreadline -o $@
	@echo "$(GREEN)-- minishell ready --$(RESET_COLOR)"

### --- LIBFT --- ###
$(LIBFT)	: FORCE
	@$(MAKE) -C $(LIBFT_PATH)/ all

#########     OBJECTS    #########
$(BUILD_PATH)/%.o : $(SRC_PATH)/%.c
	@mkdir -p $(BUILD_PATH) $(BUILD_MAIN_PATH) $(BUILD_PARS_PATH) $(BUILD_EXEC_PATH)
	$(CC) $(COMPFLAGS) -c $< -o $@

FORCE	:

### --- CLEAN MANAGEMENT --- ###
clean	:
	@rm -rf $(BUILD_PATH)
	$(MAKE) -C $(LIBFT_PATH) clean
	@echo "$(YELLOW)-- removed temporary files --$(RESET_COLOR)"

fclean	:
	$(MAKE) -C $(LIBFT_PATH) fclean
	@rm -rf $(BUILD_PATH) $(NAME)
	@echo "$(YELLOW)-- removed minishell programm --$(RESET_COLOR)"

re		: fclean all

.PHONY	: clean fclean re FORCE default all
