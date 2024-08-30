# Variables
NAME = libft_malloc_$(HOSTTYPE).so
SYMLINK = libft_malloc.so
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
SRC_DIR = src
OBJ_DIR = objects
INCLUDE_DIR = include
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC -I$(INCLUDE_DIR) -I$(LIBFT_DIR)
LDFLAGS = -shared
VPATH = $(SRC_DIR)

# Hosttype
ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# Source files and object files
SRCS = malloc.c show_alloc_mem.c utils.c zones.c blocks.c free.c realloc.c
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

# Targets
all: $(NAME) $(SYMLINK)

$(NAME): $(LIBFT) $(OBJS)
	@echo "Compiling $@"
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBFT)

$(SYMLINK): $(NAME)
	@echo "Creating symbolic link $(SYMLINK)"
	@ln -sf $(NAME) $(SYMLINK)

$(LIBFT):
	@echo "Compiling libft"
	@$(MAKE) -s -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "Cleaning up..."
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@rm -f $(OBJS)

fclean: clean
	@echo "Full clean up..."
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	@rm -f $(SYMLINK)

re: fclean all

.PHONY: all clean fclean re
