# clang-ir

Slate uses clang-ir (CIR), which is a recently upstreamed MLIR variant that's
going to be used for improving diagnostics for editors (think clangd)
and improving optimizations for C/C++ code.

Clang-ir's instability means that for now, to use Slate, you have to
pull down the project and build it yourself since most binary builds of
clang you can get don't build it by default. That's a big con. Another
big con is that the instructions themselves change quite often, in
backwards incompatible ways. Every pull down of clang could break
parsing logic (and it did for a while, when slate had a handwritten
list of supported CIR operations).

Given that, you might wonder why Slate doesn't just use clang-ast. It's
stable, has been supported for ages, and is integrated into every clang
build. [C2Rust uses it](https://crates.io/crates/c2rust-ast-exporter),
and there's pretty good support for deserializing it:
[clang-ast](https://crates.io/crates/clang-ast).

Take a hello world example:

```c
#include <stdio.h>

int main() { printf("%s\n", "hi"); }
```

How long is the result of compiling to clang-ast?

```c
clang -std=c23 -Xclang -ast-dump -fsyntax-only x.c | wc -l
876
```

Over 800 lines. Here's a sample:

```
...
|-FunctionDecl 0x55a2a22926c0 <line:953:1, col:27> col:12 __uflow 'int (FILE *)' extern
| `-ParmVarDecl 0x55a2a2292628 <col:21, col:26> col:27 'FILE *'
|-FunctionDecl 0x55a2a2292938 <line:954:1, col:35> col:12 __overflow 'int (FILE *, int)' extern
| |-ParmVarDecl 0x55a2a2292790 <col:24, col:29> col:30 'FILE *'
| `-ParmVarDecl 0x55a2a2292810 <col:32> col:35 'int'
`-FunctionDecl 0x55a2a2292a18 <x.c:3:1, col:36> col:5 main 'int (void)'
  `-CompoundStmt 0x55a2a2292c38 <col:12, col:36>
    `-CallExpr 0x55a2a2292bb8 <col:14, col:33> 'int'
      |-ImplicitCastExpr 0x55a2a2292ba0 <col:14> 'int (*)(const char *, ...)' <FunctionToPointerDecay>
      | `-DeclRefExpr 0x55a2a2292ac0 <col:14> 'int (const char *, ...)' Function 0x55a2a2294e50 'printf' 'int (const char *, ...)'
      |-ImplicitCastExpr 0x55a2a2292c08 <col:21> 'const char *' <NoOp>
      | `-ImplicitCastExpr 0x55a2a2292bf0 <col:21> 'char *' <ArrayToPointerDecay>
      |   `-StringLiteral 0x55a2a2292ae0 <col:21> 'char[4]' lvalue "%s\n"
      `-ImplicitCastExpr 0x55a2a2292c20 <col:29> 'char *' <ArrayToPointerDecay>
        `-StringLiteral 0x55a2a2292b30 <col:29> 'char[3]' lvalue "hi"
```

We really only care about two things. The function prototype for
`printf` since we want to export it to rust, and the definition of the
main function.

Using CIR for the same code:

```c
~/llvm-project/build-cir/bin/clang -std=c11 -fclangir -emit-cir x.c -o
```

You can see we basically only get what we need. In fact, there's only
really 15 or so lines to read, because that shows us the entire
function: the static strings (`%s\n` and `hi`), as well as the external
`printf` and then main, which calls `printf` and returns 0.

```mlir
!s32i = !cir.int<s, 32>
!s8i = !cir.int<s, 8>
module @"/home/takashi/Projects/slate/x.c" attributes {cir.default_tls_model = #cir.tls_model<tls_dyn>, cir.lang = #cir.lang<c>, cir.module_asm = [], cir.triple = "x86_64-unknown-linux-gnu", dlti.dl_spec = #dlti.dl_spec<!llvm.ptr<270> = dense<32> : vector<4xi64>, !llvm.ptr<271> = dense<32> : vector<4xi64>, !llvm.ptr<272> = dense<64> : vector<4xi64>, i64 = dense<64> : vector<2xi64>, i128 = dense<128> : vector<2xi64>, f80 = dense<128> : vector<2xi64>, !llvm.ptr = dense<64> : vector<4xi64>, i1 = dense<8> : vector<2xi64>, i8 = dense<8> : vector<2xi64>, i16 = dense<16> : vector<2xi64>, i32 = dense<32> : vector<2xi64>, f16 = dense<16> : vector<2xi64>, f64 = dense<64> : vector<2xi64>, f128 = dense<128> : vector<2xi64>, "dlti.endianness" = "little", "dlti.mangling_mode" = "e", "dlti.legal_int_widths" = array<i32: 8, 16, 32, 64>, "dlti.stack_alignment" = 128 : i64, !cir.ptr<!cir.void> = #cir.ptr_spec<size = 64, abi = 64, preferred = 64, index = 64>>} {
/* Look here -> */cir.global "private" constant cir_private dso_local @".str" = #cir.const_array<"%s\0A" : !cir.array<!s8i x 3>, trailing_zeros> : !cir.array<!s8i x 4> {alignment = 1 : i64} loc(#loc1)
/* Look here -> */cir.global "private" constant cir_private dso_local @".str.1" = #cir.const_array<"hi" : !cir.array<!s8i x 2>, trailing_zeros> : !cir.array<!s8i x 3> {alignment = 1 : i64} loc(#loc2)
/* Look here -> */cir.func private @printf(!cir.ptr<!s8i> {llvm.noundef}, ...) -> !s32i attributes {"cir.target-cpu" = "x86-64", "cir.target-features" = "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87", "cir.tune-cpu" = "generic"} loc(#loc9)
/* Look here -> */cir.func no_inline no_proto dso_local @main() -> !s32i attributes {"cir.target-cpu" = "x86-64", "cir.target-features" = "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87", "cir.tune-cpu" = "generic", nothrow} {
/* Look here -> */  %0 = cir.alloca "__retval" align(4) : !cir.ptr<!s32i> loc(#loc6)
/* Look here -> */  %1 = cir.const #cir.int<0> : !s32i loc(#loc7)
/* Look here -> */  cir.store %1, %0 : !s32i, !cir.ptr<!s32i> loc(#loc7)
/* Look here -> */  %2 = cir.get_global @printf : !cir.ptr<!cir.func<(!cir.ptr<!s8i>, ...) -> !s32i>> loc(#loc8)
/* Look here -> */  %3 = cir.get_global @".str" : !cir.ptr<!cir.array<!s8i x 4>> loc(#loc1)
/* Look here -> */  %4 = cir.cast array_to_ptrdecay %3 : !cir.ptr<!cir.array<!s8i x 4>> -> !cir.ptr<!s8i> loc(#loc1)
/* Look here -> */  %5 = cir.get_global @".str.1" : !cir.ptr<!cir.array<!s8i x 3>> loc(#loc2)
/* Look here -> */  %6 = cir.cast array_to_ptrdecay %5 : !cir.ptr<!cir.array<!s8i x 3>> -> !cir.ptr<!s8i> loc(#loc2)
/* Look here -> */  %7 = cir.call @printf(%4, %6) : (!cir.ptr<!s8i> {llvm.noundef}, !cir.ptr<!s8i> {llvm.noundef}) -> !s32i loc(#loc8)
/* Look here -> */  %8 = cir.load %0 : !cir.ptr<!s32i>, !s32i loc(#loc6)
/* Look here -> */  cir.return %8 : !s32i loc(#loc6)
  } loc(#loc10)
} loc(#loc)
...
```

This representation is much more conducive to lowering to rust. There's
much less to take care (thanks CIR folks!). Since they do a lot of hard
work of resolving only what's necessary, as well as handling type
definitions and even linkage. That allows slate to keep its lowering
phase smaller than would be done through clang-ast, and simpler, so it's
less prone to bugs.

## The Clang-ir crate

That still leaves the issue of instability. Slate used to handwrite its
own code to parse CIR, but because the ops changed definition/name
often, pulling down llvm and rebuilding would often cause errors at
compile time or worse, runtime, since the ops had changed their meaning.
Thankfully, Slate's tests can catch most of these, but this isn't ideal.
To get around this, parsing was factored out from Slate and put into
another crate, unimaginatively called
[clang-ir](https://crates.io/crates/clang-ir).

The crate provides some helpers to parse CIR into a structured
form. The more interesting part is how to deal with breakage from
handwriting types that correspond to CIR ops. CIR ops themselves are
written in LLVM's own parsing language, `tablegen`, with definitions
found in: `clang/include/clang/CIR/Dialect/IR` in `*.td` files
(td = TableDefinition?). If we could parse those then we could grab
their information and generate CIR op types. Of course, LLVM has a
tablegen parser, and thankfully, there's a rust crate that has bindings
to it: [tblgen](https://crates.io/crates/tblgen). So we can parse the
tablegen definitions and then emit some rust types. If anything changes,
we can run our script again and regenerate the crate.

The crate that does this is called [clang-ir-types-gen](https://github.com/Takashiidobe/clang-ir/tree/main/clang-ir-types-gen)
(also an imaginative name), and it generates the internal
[clang-ir-types](https://crates.io/crates/clang-ir-types) that is
exported through the parsing crate. This means that as long as the CIR
ops that slate relies on aren't changed in a backwards incompatible way,
then its code doesn't need to change. A nice perk is that comments are
also preserved as doc comments, so for example for the Abs op:

[Abs doc](https://docs.rs/clang-ir-types/latest/clang_ir_types/ops/arithmetic/struct.Abs.html)

The comments are preserved and turn into documentation.
