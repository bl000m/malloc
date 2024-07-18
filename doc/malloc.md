# malloc
- when you include stdlib.h in a C file what you are really doing ?
    - you are including the declaration of the functions in the stdlib header provided by the libc
    - here it is the declaration: `void* malloc(size_t size);`
    - The actual implementation of the function malloc is contained in the library file => libc.so on Unix-like systems
    - During the linking stage of the compilation process, the linker resolves references to malloc (and other library functions) by finding the actual binary code for these functions in the library files.

- compilation and linking process:
    - Preprocessing
        - The preprocessor processes the #include <stdlib.h> directive and includes the content of stdlib.h into the source file of the project we want to compile
    - Compilation
        - the compiler compiles source code into object file (.o) => the declaration of malloc in in source file
    - Linking
        - The linker takes the compiled object file and links it with the necessary libraries (e.g., libc.so).
        - It resolves references to malloc by finding its implementation in the standard library binary.
        - The resulting executable contains the actual addresses where the functions' implementations can be found.
    - How the Executable Works at Runtime ?
        - Loader: The operating system's loader loads the executable into memory.
        - Dynamic Linking (if using shared libraries):
            - The loader loads the shared library (libc.so) into memory.
            - It sets up the necessary mappings so that calls to malloc in your executable point to the malloc implementation in libc.so.
        - Function Call:
            - When your program calls malloc, it jumps to the location in memory where malloc is implemented in the shared library.
    - Concurrent malloc Calls
    When two programs make a call to malloc at the same time, the following sequence of events occurs:

        - Library Code Execution: Each program’s call to malloc jumps to the shared code for malloc in libc.so. Although the code is shared, the execution context (registers, stack, etc.) is separate for each process.
        - Kernel Mediation: The malloc function itself may involve system calls to the kernel to request more memory from the operating system (e.g., using sbrk or mmap). The kernel manages these requests and ensures that each process receives a separate, appropriately sized block of memory.
        - Thread Safety: Inside malloc, if the function needs to manage global data structures (such as free lists), it uses synchronization mechanisms (like mutexes) to ensure thread safety. This prevents race conditions when multiple threads within the same process call malloc concurrently.

## inside malloc
- mmap:  mmap (memory map) is a powerful system call in Unix-like operating systems that provides a mechanism for mapping files or devices into memory, allowing a program to access file contents directly via memory addresses
    - its prototype: 
    ```
    #include <sys/mman.h>
    void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    ```
    - Parameters
        - addr: The starting address for the mapping. If NULL, the kernel chooses the address.
        - length: The length of the mapping in bytes.
        - prot: Memory protection flags, specifying the desired memory protection of the mapping. Common values:
            - PROT_READ: Pages may be read.
            - PROT_WRITE: Pages may be written.
            - PROT_EXEC: Pages may be executed.
            - PROT_NONE: Pages may not be accessed.
        - flags: Flags that determine the nature of the mapping. Common values:
            - MAP_SHARED: Changes are shared with other processes mapping the same region.
            - MAP_PRIVATE: Changes are private (copy-on-write).
            - MAP_ANONYMOUS: The mapping is not backed by any file; used for allocating memory.
        - fd: File descriptor of the file to map. Ignored if MAP_ANONYMOUS is set.
        - offset: Offset in the file from which the mapping starts. Must be a multiple of the page size.
    - Return values
        - On success, mmap returns a pointer to the mapped area.
        - On failure, it returns MAP_FAILED and sets errno.