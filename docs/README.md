# slate

`slate` translates C to Rust by lowering **ClangIR (CIR)** — Clang's MLIR-based
IR — rather than LLVM IR. CIR is high enough to retain structured control flow,
integer signedness, and named local variables, so this is *transpilation*, not
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
- integer constants, loads/stores, addition, increment, and comparisons.
- calls, including `printf` lowered through `libc::printf`.
- string literals used by `printf`.
- `for` loops represented as conservative Rust `loop { ... break ... }`.
- C enum constants with implicit values and explicit `= number` values, emitted
  as Rust integer `const` items.
- simple C unions with integer fields, emitted as `#[repr(C)] union` plus
  unsafe field reads/writes.
- simple C structs with integer fields, emitted as `#[repr(C)] struct`.
- fixed-size local `int` arrays with indexed stores and loads, emitted as Rust
  arrays.
- `sizeof` expressions that Clang lowers to integer CIR constants.
- volatile CIR loads and stores, emitted with Rust volatile pointer intrinsics.
- source-level context loaded from Clang's JSON AST.

Output is intentionally ugly, temp-heavy, `libc`-backed Rust. Correctness is
verified by **differential testing**: compile and run both the original C and the
generated Rust, compare stdout + exit code.

The current fixtures are:

- `add.c` — integer functions, locals, calls, and addition.
- `loop_sum.c` — structured `for` loop lowering.
- `enums.c` — enum constants with implicit and explicit values.
- `unions.c` — basic union declaration, field writes, and field reads.
- `structs.c` — basic struct declaration, field writes, and field reads.
- `arrays.c` — fixed-size local arrays and indexed element access.
- `sizeof.c` — `sizeof` over primitive, array, struct, union, and expression
  forms.
- `volatile.c` — volatile local stores and loads.

Generated Rust for inspection is written with:

```bash
cargo run -- emit-fixtures
```

That command writes ignored files under `tests/fixtures.generated/`. The checked
fixtures under `tests/fixtures/` are C-only.

## Not handled yet

Important gaps remain:

- target-complete C integer modeling beyond the CIR widths already emitted.
- unsigned arithmetic behavior and casts beyond the currently exercised cases.
- pointers, broader aggregate coverage, field access beyond the current simple
  record cases, and pointer arithmetic.
- global variables other than constant strings used by `printf`.
- `if`, `while`, `switch`, `break`, `continue`, and `goto`.
- more arithmetic, bitwise, logical, and assignment operators.
- function prototypes, declarations across translation units, typedefs, and
  headers beyond what Clang resolves for the fixture.
- floating point, chars as values, string operations, varargs beyond direct
  `printf` calls.
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

## Toolchain

Requires a CIR-enabled Clang (`CLANG_ENABLE_CIR=ON`). Local build lives at
`~/llvm-project/build-cir/bin/{clang,cir-opt}`; overridable via `SLATE_CLANG`
and `SLATE_CIR_OPT`.

Target selection can be shared across the CIR and AST Clang invocations with
`SLATE_TARGET=<triple>` and extra flags in `SLATE_CLANG_ARGS`.
