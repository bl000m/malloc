# Shared objects
we compile and create `libft_malloc_x86_64_Linux.so` and a symbolic link that point to that. 
- what is a shared object ?
    - it's a shared library in Unix-like operating systems. Here’s what distinguishes a .so file from other types of library files:

    - Shared Object (.so) Files:
    Dynamic Linking: Shared object files are dynamically linked libraries that are loaded into memory at runtime when a program that depends on them is executed. This dynamic linking allows multiple programs to share the same instance of the library in memory, reducing memory usage and allowing updates to the library without recompiling every dependent program.

    - Extension: The .so extension stands for "shared object". On Linux and other Unix-like systems, shared libraries typically use this extension to denote their type. For example, libft_malloc_x86_64_Linux.so is a shared library specifically compiled for the x86_64 architecture running Linux.

    - Versioning: Shared object files can also include versioning information. This allows multiple versions of a library to coexist on a system, each identified by a unique filename with version numbers appended (e.g., libft_malloc_x86_64_Linux.so.1, libft_malloc_x86_64_Linux.so.1.0, etc.). This versioning helps ensure compatibility and allows programs to specify which version of a library they require.

    - Dynamic Loading: Programs dynamically link against .so files using the dynamic linker/loader (ld.so on Linux). This linker resolves symbols (functions and data) from the shared library at runtime, enabling the program to call functions and use data defined in the library.