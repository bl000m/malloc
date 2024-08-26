# runtime vs compile time
- Timing:
    - Compile Time: Before execution, during the translation of source code into executable code.
    - Runtime: During execution, when the program is running.
- Error Detection:
    - Compile Time: Syntax errors, type errors, and other issues that can be detected by static analysis.
    - Runtime: Logical errors, runtime exceptions, and issues arising from dynamic conditions.

- Optimization:
    - Compile Time: Code optimization based on static analysis and known constraints.
    - Runtime: Performance influenced by actual execution conditions and dynamic behavior.

- Dynamic Behavior:
    - Compile Time: Fixed code structure and data types.
    - Runtime: Variable program behavior based on user input, environment, and system state.

## More in details
- Compile Time
Compile time is the phase when source code is translated into machine code by a compiler. It occurs before a program is executed. Key aspects include:

    - Compilation:
During compile time, the source code written in a high-level language (like C or C++) is converted into machine code or an intermediate representation (like bytecode). This process involves syntax checking, type checking, and code optimization.

    - Error Detection:
Errors such as syntax errors, type mismatches, and missing dependencies are detected at compile time. If there are issues, the compiler will report them before the program runs.
    - Static Analysis:
The compiler performs static analysis to ensure that the code adheres to the language rules and constraints. This includes checking function signatures, variable types, and constant expressions.
    - Optimization:
The compiler may optimize the code for performance and efficiency, such as inlining functions, optimizing loops, and removing dead code.
    - Linking:
During compile time, the compiler links together various object files and libraries into a single executable or shared library, resolving external references.

- Runtime
Runtime is the phase when a program is actually executed on a computer. This happens after the code has been compiled. Key aspects include:

    - Execution:
    At runtime, the compiled machine code or bytecode is loaded into memory and executed by the operating system. This is when the program performs its tasks based on the input it receives and its logic.
    - Dynamic Behavior:
Runtime includes dynamic aspects of the program’s behavior, such as handling user input, interacting with files or networks, and managing memory allocation dynamically.
    - Error Detection:
Errors that occur during the execution of the program, such as runtime exceptions (e.g., division by zero, null pointer dereference, or out-of-bounds errors), are detected at runtime. These errors are not known until the program is executed.
    - Dynamic Linking:
    ```
    Programs may load shared libraries (dynamic linking) at runtime. For example, a shared object (.so) file might be linked and used by the program during its execution.
    ```

    - Performance:
Runtime performance is affected by how well the code was optimized during compile time, but also by factors like system load, input data, and real-time conditions.