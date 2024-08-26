# a static library compiled into a shared library
In our case we add, at compile time, the objects (.o in machin code) of the static library libft, into the shared library libft_malloc.so.

- when creating a shared library (libft_malloc.so), the process involves:

    - Compilation of Static Library:
    Object Files: The static library (libft.a) consists of object files (*.o) that are compiled from source files (*.c).
    Archive: These object files are bundled into a static library archive (libft.a).
    Linking Process:

    - Shared Library Compilation: During the compilation of the shared library (libft_malloc.so), the linker incorporates the necessary object code from the static library (libft.a).
    - Static Library Linking: At the linking stage of building the shared library, the linker extracts relevant object code from libft.a and includes it in the shared library. This linking happens at compile time.
    - Shared Library Creation: The final shared library (libft_malloc.so) is created from:
        - The compiled object files of the shared library’s own source code.
        - The linked object files from the static library (libft.a).
- Key Points
    - Static Library (libft.a): Contains precompiled object files that are archived together.
    - Shared Library (libft_malloc.so): Contains compiled object files from its own source code as well as linked object files from libft.a.
    - Linking Stage: Involves combining code from the static library into the shared library at compile time.

- Linking:
- Static Library Linking: During the linking stage of building the shared library, the linker pulls object code from libft.a and includes it in the shared library. This happens at compile time.
- Shared Library Creation: The shared library is created from the compiled object files of its own source code and the linked object files from the static library.