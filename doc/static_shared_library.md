# Static vs Shared library
Key Differences Summary
- Linking:
    - Static Library: Linked at compile time; embedded in the executable.
    - Shared Library: Linked at runtime; not embedded in the executable.

- Memory Usage:
    - Static Library: Each executable has its own copy of the library code.
    - Shared Library: Code is shared among executables, reducing memory usage.

- Updates:
    - Static Library: Requires recompilation of all programs that use it to update.
    - Shared Library: Can be updated independently of the programs, benefiting all users of the library.

- Executable Size:
    - Static Library: Larger, as it includes the library code.
    - Shared Library: Smaller, as it relies on external shared code.

- File Extension:
    - Static Library: .a (e.g., libft.a).
    - Shared Library: .so (e.g., libft_malloc.so)