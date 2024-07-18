# malloc

## Focus on the context (not banal)
- libc
    libc is the standard C library, a fundamental part of the Unix and Unix-like operating systems. It provides the essential APIs for C programs to interface with the operating system. These APIs include functions for tasks such as:

    - File and I/O Operations: Opening, reading, writing, and closing files.
    - Memory Management: Allocating, reallocating, and freeing memory.
    - String Manipulation: Functions for copying, concatenating, and comparing strings.
    - Mathematical Computations: Basic arithmetic and more complex mathematical functions.
    - Process Control: Creating, managing, and terminating processes.
    - Error Handling: Reporting and managing errors.

    libc follows the standards set by ANSI C (American National Standards Institute) and ISO C (International Organization for Standardization), making it a critical component for C programs that need to be portable across different Unix-like systems.

    there are several implementations of libc, each with its own features and optimizations:
    glibc (GNU C Library): The most common implementation used in Linux distributions.

    - Usage in Programs: Almost every C program on a Unix-like system relies on libc for system calls and basic functionalities. When a C program is compiled, it is typically linked against libc.

    - Dynamic and Static Linking: libc can be linked dynamically or statically:

        - Dynamic Linking: The program uses a shared version of the library at runtime, which saves memory as multiple programs can share the same library.
        - Static Linking: The library code is included in the executable at compile time, which can increase the size of the executable but can be beneficial for portability.
    - to detect which implemntation of libc a machine have: `ldd /bin/ls`, or `strings /lib/x86_64-linux-gnu/libc.so.6 | grep "GNU"`

    - example of libc Functions:
    ``` 
    File Operations:
    c
    Copier le code
    #include <stdio.h>

    FILE *file = fopen("example.txt", "r");
    if (file) {
        char buffer[100];
        while (fgets(buffer, sizeof(buffer), file)) {
            printf("%s", buffer);
        }
        fclose(file);
    }
    Memory Management:
    c
    Copier le code
    #include <stdlib.h>

    int *arr = malloc(10 * sizeof(int));
    if (arr) {
        for (int i = 0; i < 10; i++) {
            arr[i] = i;
        }
        free(arr);
    }
    ```
    - in /lib/x86_64-linux-gnu there are compiled versions of libc. These binaries contain the machine code that programs use at runtime. 
    - glibc (GNU version) source code: https://sourceware.org/glibc/

## malloc
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
