# slate

`slate` translates C to Rust by lowering **ClangIR (CIR)** — Clang's MLIR-based
IR — rather than LLVM IR. CIR is high enough to retain structured control flow,
integer signedness, and named local variables, so this is _transpilation_, not
decompilation. (See [architecture.md](architecture.md) for why the IR level is
the whole game.)

## Approach in one line

**Transliterate first, idiomatize later.** V0 emits the most faithful Rust it
can — `unsafe`, `libc`-backed, ugly — and correctness is the only bar. Idiomatic,
safe Rust is recovered incrementally by a ladder of later passes, each
independently verified.

## Current state

The current implementation can translate, compile, and differentially test a
small C subset. This is the supported fixture-level surface today:

- functions with `int` parameters, locals, return values, and `main`.
- target-lowered CIR integer widths mapped to Rust primitives such as `i32`.
- `char`, `signed char`, and `unsigned char` locals, params, fields, and return
  values, mapped to Rust `i8`/`u8`, including `'A'`-style char literals and `%c`
  printing.
- `short`, `long`, and `long long` (with their `unsigned` variants) locals,
  params, and return values, mapped to Rust `i16`/`i64`/`u16`/`u32`/`u64` by CIR
  width, printed with the width-correct `printf` conversion (`%u`, `%ld`, ...).
- `_Bool` and `bool` locals, parameters, and return values, mapped to Rust
  `bool`, including integer-to-bool and comparison-to-bool conversions.
- `<stdint.h>` fixed-width integer typedefs and `<stddef.h>` `size_t`, resolved
  through Clang desugared type facts and CIR integer widths.
- integer overflow left to wrap two's-complement: the generated Rust builds with
  `overflow-checks = false`, matching clang's `-O0` C, so neither side panics.
- integer constants, loads/stores, comparisons, increment, and the binary
  operators `+`, `-`, `*`, `/`, `%`.
- `float`/`double` parameters, locals, return values, and constants, mapped to
  Rust `f32`/`f64`, with `+`/`-`/`*`/`/`, comparisons, and int/float casts.
- `long double` parameters, locals, return values, constants, arithmetic, and
  casts, mapped to a `#[repr(C, align(16))]` `LongDouble(f64)` newtype: ABI-exact
  size/alignment with baseline `f64`-precision arithmetic (not extended precision).
- `double`/`float _Complex` parameters, locals, return values, constants,
  `+`/`-`/`*`/`/`, and `__real__`/`__imag__` extraction, mapped to a `#[repr(C)]`
  `Complex<T>` newtype. `*`/`/` lower through clang's inline expansion plus the
  libgcc `__muldc3`/`__divdc3` runtime, which the generated Rust calls via
  `extern "C"` so results are bit-identical to C.
- calls, including `printf` lowered through `libc::printf` (`%d` and `%f`).
- string literals used by `printf`.
- `for` loops represented as conservative Rust `loop { ... break ... }`.
- `while` loops represented as conservative Rust `loop { ... break ... }`.
- `if`/`else` (including `else if` chains) represented as Rust `if { ... } else { ... }`.
- C enum constants with implicit values and explicit `= number` values, emitted
  as Rust integer `const` items.
- simple C unions with primitive scalar fields, emitted as `#[repr(C)] union` plus
  unsafe field reads/writes.
- simple C structs with primitive scalar fields, emitted as `#[repr(C)] struct`.
- fixed-size local arrays of primitive scalar element types with indexed stores
  and loads, emitted as Rust arrays.
- C pointers for locals, parameters, address-of, dereference, and basic pointer
  arithmetic, emitted as raw Rust pointers.
- C function pointer locals and parameters for direct assignment and indirect
  calls, emitted as nullable `Option<fn(...) -> ...>` values.
- `sizeof` expressions that Clang lowers to integer CIR constants.
- type-qualified primitive scalar locals, parameters, fields, returns, globals,
  and restrict-qualified pointer parameters. `volatile` operations are emitted
  through CIR volatile loads/stores with Rust volatile pointer intrinsics;
  `const`, `restrict`, and `_Atomic` are normalized to the same baseline Rust
  storage types as their unqualified forms.
- file-scope `static` integer and floating globals with constant initializers,
  emitted as `static mut`.
- `typedef` aliases for otherwise supported types, resolved through Clang's
  desugared type facts.
- source-level context loaded from Clang's JSON AST.

Output is intentionally ugly, temp-heavy, `libc`-backed Rust. Correctness is
verified by **differential testing**: compile and run both the original C and the
generated Rust, compare stdout + exit code.

The current fixtures are:

- `add.c` — integer functions, locals, calls, and addition.
- `loop_sum.c` — structured `for` loop lowering.
- `while_loop.c` — structured `while` loop lowering.
- `enums.c` — enum constants with implicit and explicit values.
- `unions.c` — basic union declaration, field writes, and field reads.
- `structs.c` — basic struct declaration, field writes, and field reads.
- `non_int_fields.c` — `char`/`unsigned char`/`float`/`double` struct and union
  fields.
- `arrays.c` — fixed-size local arrays and indexed element access.
- `array_types.c` — fixed-size local `char` and `double` arrays.
- `pointers.c` — pointer locals/params, address-of, dereference, and pointer
  arithmetic.
- `function_pointers.c` — function pointer locals, parameters, assignment, and
  indirect calls.
- `sizeof.c` — `sizeof` over primitive, array, struct, union, and expression
  forms.
- `volatile.c` — volatile local stores and loads.
- `volatile_qualified_types.c` — volatile-qualified primitive params, fields,
  returns, and globals.
- `type_qualifiers.c` — const, restrict, and `_Atomic` primitive and pointer
  type qualifiers.
- `static_globals.c` — file-scope static integer global loads and stores.
- `non_int_globals.c` — file-scope static `char`/`unsigned char`/`float`/`double`
  globals plus non-int params and returns.
- `typedefs.c` — aliases for primitive types used in params, locals, fields,
  returns, and `sizeof`.
- `stdint_types.c` — `<stdint.h>` fixed-width typedefs and `<stddef.h>` `size_t`.
- `floats.c` — `float`/`double` locals, params, arithmetic, casts, and `%f`
  printing.
- `long_double.c` — `long double` locals, params, return values, arithmetic,
  and casts.
- `complex.c` — `double _Complex` locals, `+`/`-`/`*`/`/`, and
  `__real__`/`__imag__` extraction.
- `chars.c` — `char`/`signed char`/`unsigned char` locals, params, arithmetic,
  char literals, and `%c`/`%d` printing.
- `shorts.c` — `short`/`unsigned short` locals, params, arithmetic, and return.
- `unsigned.c` — `unsigned int` arithmetic with defined wrapping overflow.
- `longs.c` — `long`/`unsigned long` locals, params, and return values.
- `longlong.c` — `long long`/`unsigned long long` locals, params, and return.
- `bool.c` — `_Bool`/`bool` locals, params, returns, integer conversions, and
  comparison conversions.
- `sub.c` — signed subtraction and defined unsigned wrapping subtraction.
- `mul.c` — signed multiplication and defined unsigned wrapping multiplication.
- `div.c` — signed/unsigned division, truncating toward zero.
- `modulo.c` — signed/unsigned remainder, taking the sign of the dividend.

Generated Rust for inspection is written with:

```bash
cargo run -- emit-fixtures
```

That command writes ignored files under `tests/fixtures.generated/`. The checked
fixtures under `tests/fixtures/` are C-only.

## Not handled yet

Important gaps remain:

- target-complete C integer modeling beyond the CIR widths already emitted
  (e.g. `__int128`).
- bitwise, logical, and assignment operators beyond `+=`; wider arithmetic and
  casts beyond the currently exercised cases.
- broader aggregate coverage beyond primitive scalar fields.
- globals beyond file-scope `static` primitive scalar globals with constant
  initializers and constant strings used by `printf`.
- `switch`, `break`, `continue`, and `goto`.
- more arithmetic, bitwise, logical, and assignment operators.
- function prototypes, declarations across translation units, and headers beyond
  what Clang resolves for the fixture.
- hex float literals and float math beyond `+`/`-`/`*`/`/`.
- string operations and varargs beyond direct `printf` calls.
- idiomatic Rust cleanup such as `println!`, temp removal, references, slices,
  and safe ownership.

## Pipeline

```
C ──emit──► CIR ──parse──► Op-tree ──lower──► Rust source
│  clang|cir-opt                    ▲
└──ast-dump=json──────► Clang AST ──┘

verified:  run(C).{stdout,exit}  ==  run(Rust).{stdout,exit}
```

## Three sources

Every C input is available to the translator in three forms, joined by source
location (`file:line:col`):

- **CIR** — the primary lowering source.
- **Clang AST** — loaded from `clang -Xclang -ast-dump=json -fsyntax-only` and
  extracted into structured source context, with raw JSON retained.
- **C source text** — for comments and naming during final readability polish.

## Docs

- [adding-features.md](adding-features.md) — how to add C coverage or a Rust
  fixup.
- [architecture.md](architecture.md) — sources, IRs, pipeline, shared context.
- [passes.md](passes.md) — the pass catalog: what runs, in what order, how.
- [idiomatization.md](idiomatization.md) — the `unsafe`/`libc` → idiomatic ladder.
- [fuzzing.md](fuzzing.md) — the stateful C-subset generator behind differential
  fuzzing.

## Toolchain

Requires a CIR-enabled Clang (`CLANG_ENABLE_CIR=ON`). Local build lives at
`~/llvm-project/build-cir/bin/{clang,cir-opt}`; overridable via `SLATE_CLANG`
and `SLATE_CIR_OPT`.

Target selection can be shared across the CIR and AST Clang invocations with
`SLATE_TARGET=<triple>` and extra flags in `SLATE_CLANG_ARGS`.
