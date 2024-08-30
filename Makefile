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
# The -shared flag instructs the linker to create a shared library 
# (also known as a dynamic library or a shared object).
# Shared Library: A shared library is a collection of precompiled functions (object code) 
# that applications can dynamically link to during runtime.
# This allows multiple programs to use the same library file, 
# reducing the overall memory usage because the library code is loaded into memory once and shared among all programs that use it.
LDFLAGS = -shared
VPATH = $(SRC_DIR)

# Source files
SRCS = malloc.c show_alloc_mem.c utils.c zones.c blocks.c free.c # realloc.c 
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

# Hosttype
ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# Targets
all: $(NAME) $(SYMLINK)

# The rume $(NAME) creates  the actual shared library
# the $(HOSTTYPE) is determined by the Makefile based on the system's architecture 
# and operating system type (uname -m and uname -s)
$(NAME): $(LIBFT) $(OBJS)
	@echo "Compiling $@" 
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBFT)

# This creates the symbolic link ($(SYMLINK)) that points to libft_malloc_$(HOSTTYPE).so
# This link allows programs to dynamically link 
# against libft_malloc.so without worrying about the specific $(HOSTTYPE)
$(SYMLINK):
	@echo "Creating symbolic link $(SYMLINK)"
	@ln -sf $(NAME) $(SYMLINK)

$(LIBFT):
	@echo "Compiling libft"
	@$(MAKE) -s -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "Clean up..."
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@rm -f $(OBJS)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Full clean up..."
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@rm -f $(NAME) $(SYMLINK)

re: fclean all

.PHONY: all clean fclean re
