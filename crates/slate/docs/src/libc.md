# Libc

Slate runs clang with `-nostdlib` and thus needs to provide definitions
for C header files that it includes. To do that, there's a libc-shim
directory that has headers for a standards compliant libc, mainly based
off of musl, but with some glibc + BSD extras, with Windows, Mac, and
Android support in the works.

An alternative approach (and one that avoids this completely) is to use
clang without `-nostdlib`. That would allow somebody to transpile C to
Rust, but it would only work on their specified rust target.

Take for example this program:

```c
#include <stdio.h>

int main(void) {
    #if defined(__x86_64__)
        printf("I'm on x86\n");
        x86_specific_call();
    #elif defined(__aarch64__) || defined(__arm__)
        printf("I'm on arm\n");
        arm_specific_call();
    #endif
}
```

There are two ways to translate this: If you run the preprocessor first,
and you're on an x86 machine, then you will only see the code that runs
on your target (x86):

```rust
fn main() {
    println!("I'm on x86");
    x86_specific_call();
}
```

Or, you can translate both:

```rust
fn main() {
    #[cfg(target_arch = "x86_64")] {
        println!("I'm on x86");
        x86_specific_call();
    }

    #[cfg(target_arch = "aarch64")] {
        println!("I'm on arm");
        arm_specific_call();
    }
}
```

However, you cannot provide the implementation of the arm specific call
on x86_64 (unless you want to install the headers yourself and point
your clang to it).

```c
void arm_specific_call() {
    vld1q_s32(...); // where do I get this intrinsic?
}
```

Another tricky example is with some of the libc functions. Take
`strerror_r`.

Is the definition of `strerror_r`:

```c
int strerror_r(int errnum, char *buf, size_t buflen);
```

Or is it?

```c
char *strerror_r(int errnum, char *buf, size_t buflen);
```

For glibc systems, the latter is generally used (since you'll probably
compile glibc with `_GNU_SOURCE` enabled). Musl however is standards
compliant, so it uses the POSIX signature.

So, if you transpile to rust with `strerror_r` and use the latter
definition, your code will end up running differently than it would for
a musl target.

There's also the issue of endianness.

There are thus 4 things to look out for when making sure your code is
really cross platform:

1. ISA (x86_64, aarch64, riscv) (since this changes word size and thus
   the signature of certain functions)
2. OS (Windows, Mac, Linux, Android)
3. libc (Msvc, libsystem, glibc, musl, bionic)
4. Endianness (Big/Little)

Since Slate knows what targets you aim for, as long as it has a standard
library that can support every definition for each target, it can run
translation once per output target:

Say you wanted `x86_64-unknown-linux-gnu` and `aarch64-apple-darwin`.
`libc-shim` would have include guards for the specific part(s) that are
specific to each target, then it can translate to rust once for each
target, and then merge the definitions under `#[cfg(target_arch = ...)]`
for each part that is truly target specific. That way, you can transpile
rust for multiple targets even on a machine that doesn't have
definitions for other targets (since libc-shim will provide those).
