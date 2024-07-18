# Variables
NAME = libft_malloc_$(HOSTTYPE).so
SYMLINK = libft_malloc.so
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC
LDFLAGS = -shared

# Source files
SRCS = malloc.c free.c realloc.c show_alloc_mem.c utils.c
OBJS = $(SRCS:.c=.o)

# Hosttype
ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# Targets
all: $(NAME) $(SYMLINK)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBFT)

$(SYMLINK):
	ln -sf $(NAME) $(SYMLINK)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -o $@ -c $<

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(SYMLINK)

re: fclean all

.PHONY: all clean fclean re
