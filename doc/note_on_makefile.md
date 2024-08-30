# Makefile - notes

# The -shared flag instructs the linker to create a shared library 
# (also known as a dynamic library or a shared object).
# Shared Library: A shared library is a collection of precompiled functions (object code) 
# that applications can dynamically link to during runtime.
# This allows multiple programs to use the same library file, 
# reducing the overall memory usage because the library code is loaded into memory once and shared among all programs that use it.

# The rule $(NAME) creates  the actual shared library
# the $(HOSTTYPE) is determined by the Makefile based on the system's architecture 
# and operating system type (uname -m and uname -s)

# ${SYMLINK} creates the symbolic link ($(SYMLINK)) that points to libft_malloc_$(HOSTTYPE).so
# This link allows programs to dynamically link 
# against libft_malloc.so without worrying about the specific $(HOSTTYPE)