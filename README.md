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
which results in Context-Switch to build the 'Kernel stack'. ([StackOverflow source](https://stackoverflow.com/questions/72672456/does-a-system-call-involve-a-context-switch-or-not)).<br></br>
This context-switch is not a process-to-process context switch, where by the state of the current process is cached, but a different type, called a 
user-kernel switch.
My understanding of this switch is by no means deep, and further research is required for this.

In light of this, I decided to remove the aforementioned restraint and approach the problem with a new mindset:<br></br>
"Get the best of both worlds, minimal syscalls and minimal malloc/frees!"<br></br>

To achieve this I use a stack-allocated buffer with the size of `4096`. This specific number is to align with most 
modern machine's filesystem block-size. ([StackOverflow source](https://stackoverflow.com/questions/8803515/optimal-buffer-size-for-write2))
This design allows me to handle data/format-specifiers sized up to 4096 without having to call `malloc`, and with only a single call to `write`.
Whilst data > 4096, invokes `malloc` and `write` once respectively.
This is to improve throughput, and my attempt of approaching the problem with mindfulness.

> You can argue that some of these approaches are technically premature-optimizations, and while I agree with that sentiment, this project is a learning project
so whatever I attempt and use here is more for the learning experience rather than real-world feasibility.

## Pitfalls

Whilst giving the project a rough attempt, I encountered a minor issue, I had misjuded how the flags behave in `printf`.<br>
My assumption was that the '-' and '0' flags required the width to be specified after them, for an example:
```
'%025+.3'
'%-25 .3'
```
However, this is not the case as printf has a very specific format which happens to be:
```
%[flags][width][precision][specifier]
```

My first thought after uncovering this, is to always make sure to RTFM.
Followed by, realizing that this could technically be solved with regex, and I changed my code accordingly.

## Things I learned

### Bitmasking
For this project, I decided to try using bitmasking. A simple concept of mapping the 1s and 0s in a byte to arbritrary boolean values.
An example used in this project is a boolean for the '-' flag, here I called it the `FLAG_DASH_MASK`.
The value of this mask is 16, which is the 5th bit from the right assuming Big Endian-ness.
```
   64  32  16  8   4   2   1
[  0   0   1   0   0   0   0  ]
           ^
     FLAG_DASH_MASK
```
The primary usecase of this, is to avoid having multiples of `bool` or `int` data types to represent a 'true'/'false' value, and instead
have them mapped into a single byte that can hold up to 8 (number of bits) boolean values. An upside of this, that I learnt, is the decrease in binary size,
but in my opinion, nowadays, the idea of needing a slim binary is pretty archaic unless you're in the context of bare-metal programming.

### Memory Representation of Function Arguments (and how Variadic Functions can access them!)
This project requires the use of the variadic function macros `va_list`, `va_arg` and etc..
This lead to an interesting discovery that function arguments are contiguous memory segments!
Take for an example:
```
void *check_memory(char *x, char *y)
{
    printf("%li\n", y - x); // x == 6;
}

int main()
{
    char *x = "Hello";
    char *y = "Goodbye";
    check_mem(x, y);
}
```
Doing `y` - `x` and getting the total length (NUL-terminator byte included) of `x` shows that the memory is contiguous and can be represented as such:
```
function ptr here (?)
  |
  V  0x1   0x2   0x3   0x4   0x5   0x6    0x7   0x9   0xa   0xb   0xc   0xd   0xe   0xf   0x10
[ _  'h'   'e'   'l'   'l'   'o'   '\0'   'G'   'o'   'o'   'd'   'd'   'b'   'y'   'e'   '\0' ]
      ^                                    ^
     `x`                                  `y`
```

This also helps understand how the variadic macros access the function parameters and can extract the arguments (`va_arg`).

> Note this is an educated guess and I did not make full research into this.

My assumption for this is that when we start accessing the variadic funcion we have to call the macro
`va_start()`, this macro accepts the name of the last parameter in the function parameters. This allows the va_list pointer
to point to the 'start' of the memory address of the last parameter. By using `sizeof` the macro should be able to find the offset
where the following variables will coincide. (They are contiguous in memory so this allows it to do so).

```
protoype: void do_something(char *x, int y, ...);
function ptr here (?)                                 variadic function (...) begin
  |                                                                V
  V  0x1   0x2   0x3   0x4   0x5   0x6    0x7   0x9   0xa   0xb | 0xc   0xd   0xe   0xf   0x10
[ _  'h'   'e'   'l'   'l'   'o'   '\0'   '1'   '3'   '8'   '0' | 'd'   'b'   'y'   'e'   '\0' ]
      ^                                    ^                    ^
     `x`                                  `y`            end of int size 
```

The use of `va_arg()` requires that we pass it a type, this would allow the `va_list` pointer, to extract exactly how many bytes, 
the type requires from the function parameter memory addresses, and parse them into the appropriate type we asked for.
