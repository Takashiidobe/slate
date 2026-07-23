---
title: "Slate: a C to Rust transpiler"
author:
  - "Takashi Idobe"
  - "Pumpkin Kumar"
date: 2026-07-15
abstract: |
  The authors propose Slate, a C to Rust multipass transpiler that
  converts C code into idiomatic rust code. The frontend first lowers C
  code to Clang IR, and then lifts it back up to unsafe Rust code. This
  is similar to what current C to Rust transpilers like C2Rust do. The
  backend then gathers facts about the generated Rust code, and then
  rewrites the generated code where correct to safe Rust code.
keywords:
  - Compilers
  - C
  - Rust
bibliography: references.bib
link-citations: true
geometry: margin=0in
fontsize: 11pt
linestretch: 1.15
---

![One of the authors, using slate to transpile C to Rust](./figures/pumpkin-using-slate.jpg)

# Introduction

C has been a ubiquitous programming language for many applications for
over 50 years. It is well known for its performance, but comes short
when it comes to correctness. In 2019, Microsoft published a blog post
noting that roughly 70% of security issues in C and C++ are memory
safety issues. Rust, a newer systems programming language, is memory
safe yet provides roughly the same performance as C. Because of this,
there has been a long tail of companies migrating their code from C and
C++ to Rust.

Recent industry and government efforts have pushed toward memory-safe languages
and automated C-to-Rust migration [@microsoft2019]; [@cisa2023]; [@darpa2024a];
[@darpa2024b]; [@google2024].

Some more recent attempts include [@c2saferrust] which use neurosymbolic
techniques, using the combination of both C2Rust and LLMs to translate
rust code.

The authors propose a new tool to automate that process called slate. It
aims to be a one stop shop for translating a C program to as idiomatic
rust as possible without compromising correctness. The authors believe a
wide swath of C programs can be translated to unsafe Rust and then
lifted to safe rust.

# Background

LLVM recently upstreamed Clang's Intermediate Representation (CIR) (2025)
which is a Middle level Intermediate Representation (MLIR) for the C
and C++ languages, mainly to provide better editor diagnostics for LSPs.

Clang can emit a C program in these forms, ranging from higher level to
lower level:

Clang AST -> Clang IR -> LLVM IR -> Assembly

It is exceedingly difficult to lift Assembly IR readable Rust or
C code. There are toolchains that do this, but they lose information and
the code generated is not meant to be easily readable, but one of the
many possible translations of the original code.

LLVM IR was considered by the authors but it is similarly too low-level.
Given it transforms its input into Static Single-Assignment (SSA) form,
where every variable is assigned exactly once, lifting the code back to
a higher level form is the wrong level of abstraction.

Because of this, projects like C2Rust use the Clang AST to provide
structured information about C programs before translation to Rust. This
is due to the fact that C2Rust predates CIR. The CIR provides a good mix
of syntactic information (useful for recovering definitions, typedefs,
and source code) and semantic information (which is useful for lowering
into more idiomatic and safe Rust).

# Method

Slate is as a multipass transpiler, with a frontend and a
backend. The frontend takes in a C program, and then generates the Clang
AST and CIR using the Clang compiler. Next, Slate's frontend
visits the CIR and emits naive Rust in AST form.
This is passed to the backend, which first runs analysis passes on the
Rust AST to label nodes with information (such as alias information,
mutability, and other effects). These labeled nodes then go through iterative
rewriting phases, which take an AST and facts and mutate the existing
AST to rewrite the generated code.

While we aim for independence in pass ordering, we do accept that this
is impractical. Some passes, like inlining temporaries should come
before other passes, because they obstruct easily finding optimizations.
We may run some passes until a fixpoint, or may run them multiple times
after different passes. Some passes, such as rewriting gotos (as Rust
does not have gotos as C does) are semantically more similar to
lowering, and thus are placed earlier. Some passes work best after
some other passes, and thus have an ordering dependency. Some passes
have no ordering dependency.

### Verification

A transpiler requires rigorous testing to prove it can translate
programs. The authors test slate in multiple ways; we run fuzz tests,
differential tests, verification through alive, and effect based testing.
Of these, the effect based testing approach is the most important, so we
expound upon it here.

There are many ways to prove that a provided rewrite is valid. One way
is to compile the code to some similar representation, and shunt
validity to that system. If we compile two programs, and they have the
same assembly representation, then we can say that the compiler
considers the functions equivalent. However, this approach is only valid
under the rules of the given compiler. For a particular rewrite, we may
want to change a pointer and a length pair into a rust Vec. In
that case, we have changed the arguments (ptr + word sized integer) into
a struct (ptr, size, capacity). These are not the same for our given
compiler (Clang), and so we cannot compile both programs and compare
them for equality.

One way to loosen this requirement for equality is to define a set of
Effects that we wish to compare for equality.

Take these two programs, which put the value `1` on the heap, and then
print it out.

```c
int* v = malloc(sizeof(int) * 1);
v[0] = 1;
printf("%d\n", v[0]);
```

```rust
let v = vec![1];
println!("{}", v[0]);
```

These programs both print `1` to `stdout` and allocate memory on the
heap to store the value `1`. Even though these programs do not have the
same stack layout, or use the same printing functionality, if we only
observe effects to stdout, and the heap, these programs are equivalent.
If we also care about memory on the stack, these are not equivalent,
since the C program only allocates a pointer on the stack, whereas the
rust version will allocate a pointer and two word sized integers on the
stack.

If we create an interpreter which interprets both the program
pre-transformation and the program post-transformation, and notes every
effect that changed that matters to the environment, then it is possible
to say that transforming the program is ok.

## Some cons

For example, for offsetof, alignof, and sizeof, the Clang IR lowers to a
constant integer, so we have to recover the calls to offsetof, alignof,
and sizeof from the Clang AST.

# Results

Here are some example fixtures in our test suite. One is a simple
sum function:

```c
#include <stdio.h>

static int sum_items(int *items, int len) {
    int total = 0;
    for (int i = 0; i < len; i++) {
        int item = items[i];
        total += item;
    }
    return total;
}

int main(void) {
    int values[4] = {2, 4, 6, 8};
    printf("%d\n", sum_items(values, 4));
    return 0;
}
```

When running through C2Rust, we get this code, roughly 20 lines of rust
for about 15 lines of C.

```rust
#![feature(raw_ref_op)]
extern "C" {
    fn printf(__format: *const ::core::ffi::c_char, ...) -> ::core::ffi::c_int;
}
unsafe fn main_0() -> ::core::ffi::c_int {
    let mut a: [::core::ffi::c_int; 5] = [
        1 as ::core::ffi::c_int,
        2 as ::core::ffi::c_int,
        3 as ::core::ffi::c_int,
        4 as ::core::ffi::c_int,
        5 as ::core::ffi::c_int,
    ];
    let mut sum: ::core::ffi::c_int = 0 as ::core::ffi::c_int;
    let mut i: ::core::ffi::c_int = 0 as ::core::ffi::c_int;
    while i < 5 as ::core::ffi::c_int {
        sum += a[i as usize];
        i += 1;
    }
    printf(b"%d\n\0".as_ptr() as *const ::core::ffi::c_char, sum);
    return 0 as ::core::ffi::c_int;
}
pub fn main() {
    unsafe { ::std::process::exit(main_0() as i32) }
}
```

However, note slate's version, which lowers to this: because of slate's
fact finding and fixup passes, it operates on the semantics of the
program, and figures out that we really want a sum function, and so it
replaces the loop sum with a call to `iter().sum()`

```rust
fn sum_items(items: &[i32]) -> i32 {
    let total: i32 = items.iter().sum();
    total
}

fn main() {
    let mut values: [i32; 4] = [2, 4, 6, 8];
    println!("{}", sum_items(values.as_slice()));
}
```

## Feature Handling

Slate also comes equipped to recognize C feature macros.

```c
int printf(const char *, ...);

#if defined(__x86_64__) || defined(_M_X64)
static int arch_code(void) {
    return 64;
}
#elif defined(__aarch64__) || defined(_M_ARM64)
static int arch_code(void) {
    return 128;
}
#else
static int arch_code(void) {
    return 0;
}
#endif

int main(void) {
    printf("%d\n", arch_code());
    return 0;
}
```

C2Rust will only compile for the user's target (in this case, a 64 bit
X86 computer), since its use of compile_commands.json only allows it to
see what the clang AST sees after deleting all unused C code (in this
case, the arch_code for an ARM64 computer).

```rust
extern "C" {
    fn printf(_: *const libc::c_char, _: ...) -> libc::c_int;
}
unsafe extern "C" fn arch_code() -> libc::c_int {
    return 64 as libc::c_int;
}
unsafe fn main_0() -> libc::c_int {
    printf(b"%d\n\0" as *const u8 as *const libc::c_char, arch_code());
    return 0 as libc::c_int;
}
pub fn main() {
    unsafe { ::std::process::exit(main_0() as i32) }
}
```

This is problematic for users of c2rust that want to target multiple
architectures, since their version of rust code will only work for their
particular target, and fail to compile if not run on their target.

However, Slate will faithfully render all of the original C code and
translate it to Rust, because it compiles every feature version,
allowing it to recover every version of `arch_code` in this case.

```rust
#[cfg(target_arch = "x86_64")]
fn arch_code() -> i32 {
    return 64;
}

#[cfg(target_arch = "aarch64")]
fn arch_code() -> i32 {
    return 128;
}

#[cfg(not(any(target_arch = "x86_64", target_arch = "aarch64")))]
fn arch_code() -> i32 {
    return 0;
}

fn main() {
    println!("{}", arch_code());
}
```

# Discussion

Explain what the results mean, including limitations and implications.

Some cons include a long compile-time, which we hope will be mitigated
by the more accurate translation that can be provided by slate.

# Conclusion

We believe that the Slate transpiler can already be a practical
translator

# References
