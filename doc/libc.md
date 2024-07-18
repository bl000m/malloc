# libc
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