# ft_printf

This 42KL project requires that I rewrite certain parts of the C standard `printf` macro as a function.

The learning goal(s) of this project is C's variadic functions.
Despite this being the only main goal, besides improving your programming skills, there is a sense of
depth and nuance about memory that can be understood and learnt from this project if we dive deeper into how variadic functions actually work.

This project does not require that I implement the entirety of `printf`, only a small part. Most notably, I will not be handling conversions for 
floating-point integers and a handful of other possible conversion specifiers.

## Approach

The approach taken for this project is to simplify the problem of printing with flags and specifiers as much as possible.
For this project, I was able to boil this down to a three step process.

My thought process can be showcased as such:
1. Iterate the `str` that contains format specifiers.
2. If a format specifier is encountered, we will first, write the contents of `str` to stdout that we have
seen so far, followed by handling the format specifier. Otherwise, write the contents of `str` to stdout.
3. Rinse repeat step 1. and step 2. until we reach the NUL-terminator '\0' character in `str`.

Following this, I originally gave myself a restraint<br></br>
disallow the use of both `malloc` and subsequently `free`.<br></br>

Whilst working on this, I found that not having to use malloc was relatively convenient, I ended up abusing the `write` syscall.<br></br>
As of writing and to my understanding, multiple small calls to system calls such as `write` are 'expensive', as it requires that a 'user-mode' process jump over to a 'Kernel stack',
which results in Context-Switch to build the 'Kernel stack'. (![StackOverflow source](https://stackoverflow.com/questions/72672456/does-a-system-call-involve-a-context-switch-or-not)).<br></br>
This context-switch is not a process-to-process context switch, where by the state of the current process is cached, but a different type, called a 
user-kernel switch.
My understanding of this switch is by no means deep, and further research is required for this.

In light of this, I decided to remove the aforementioned restraint and approach the problem with a new mindset:<br></br>
"Get the best of both worlds, minimal syscalls and minimal malloc/frees!"<br></br>

To achieve this I use a stack-allocated buffer with the size of `4096`. This specific number is to align with most 
modern machine's filesystem block-size. (![StackOverflow source](https://stackoverflow.com/questions/8803515/optimal-buffer-size-for-write2))
This is to improve throughput, and my attempt of approaching the problem with mindfulness.

> You can argue that some of these approaches are technically premature-optimizations, and while I agree with that sentiment, this project is a learning project
so whatever I attempt and use here is more for the learning experience rather than real-world feasibility.
