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

The current implementation can translate and compile a small C subset:

- `int` functions, parameters, locals, returns, and `main`.
- integer constants, loads/stores, addition, increment, and comparisons.
- calls, including `printf` lowered through `libc::printf`.
- string literals used by `printf`.
- `for` loops represented as conservative Rust `loop { ... break ... }`.
- source-level context loaded from Clang's JSON AST.

Output is intentionally ugly, temp-heavy, `libc`-backed Rust. Correctness is
verified by **differential testing**: compile and run both the original C and the
generated Rust, compare stdout + exit code.

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

- [architecture.md](architecture.md) — sources, IRs, pipeline, shared context.
- [passes.md](passes.md) — the pass catalog: what runs, in what order, how.
- [idiomatization.md](idiomatization.md) — the `unsafe`/`libc` → idiomatic ladder.

## Toolchain

Requires a CIR-enabled Clang (`CLANG_ENABLE_CIR=ON`). Local build lives at
`~/llvm-project/build-cir/bin/{clang,cir-opt}`; overridable via `SLATE_CLANG`
and `SLATE_CIR_OPT`.
