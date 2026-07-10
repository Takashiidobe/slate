# Architecture

## Why CIR, not LLVM IR

Translation quality is bounded by how much structure the source IR retains.
LLVM IR has already destroyed what Rust needs:

- **Control flow** becomes a CFG of basic blocks — no `for`/`while`/`if`.
  Recovering structure (the "relooper" problem) is hard and, for irreducible
  graphs, impossible without node duplication or dispatch variables.
- **SSA + phi nodes** replace source variables.
- **Signedness and source types** are largely gone (`add` is neither signed nor
  unsigned; structs are scalarized).

ClangIR (CIR) sits between the Clang AST and LLVM IR. In its high-level,
pre-`cir-flatten-cfg` form it keeps exactly what we need:

- Structured control flow as region-carrying ops (`cir.for`, `cir.scope`,
  `cir.if`, `cir.switch`).
- Named local variables as `cir.alloca "x"` memory slots (not SSA) — these map
  directly to Rust `let mut`, sidestepping phi reconstruction.
- Integer signedness in the type (`!cir.int<s, 32>` vs `<u, 32>`).

We consume the **MLIR generic form** (`cir-opt --mlir-print-op-generic`) because
it is fully regular — `"op"(operands) <{attrs}> ({regions}) : type` — which makes
a stable, op-agnostic parser possible.

## The three sources

Every input is available in three forms, all keyed by **source location**:

| Source    | How obtained                                                     | Role                                     |
| --------- | ---------------------------------------------------------------- | ---------------------------------------- |
| CIR       | `clang -fclangir -emit-cir` \| `cir-opt --mlir-print-op-generic` | primary lowering input                   |
| Clang AST | `clang -Xclang -ast-dump=json -fsyntax-only`                     | structured source context and raw oracle |
| C text    | read the file                                                    | comments / naming (final polish)         |

**Location is the join key.** CIR ops carry `loc("f.c":4:13)`; AST nodes carry
source ranges; C text is addressable by line:col. "Consult the AST" means: take a
CIR op's `loc`, look up the AST node covering that range, read the disambiguating
fact. Therefore the parser **must retain locations** — they are structural, not
noise.

The current implementation loads Clang's JSON AST during `translate`. `c_ast.rs`
filters the dump down to source-file function definitions, extracts a compact
`Unit` (`Function`, `Decl`, `Stmt`, `Expr`, `CType`, `Loc`), and preserves each
function's raw Clang JSON node. The compact AST is the common path; the raw node
is the escape hatch when a new feature needs more source facts before the compact
model has grown.

## Two IRs

The pipeline currently flows through two main internal representations:

1. **CIR op-tree** (`ir.rs`) — a _generic_ model: `Op { results, name, operands,
attrs, regions, result_type, loc }`. It is deliberately not a per-op typed
   enum: the parser never changes as op coverage grows; only the lowering's
   `match name` does. Typed **views** (e.g. `AllocaOp::of(op)`) give ergonomic,
   checked access without coupling the parser to the op set.

2. **Clang source context** (`c_ast.rs`) — a compact AST plus raw JSON, keyed by
   source locations and function names. This is not a handwritten C parser; it is
   Clang's semantic AST reduced to the facts Slate currently needs.

`rust_ast.rs` exists as a small output-side tree and printer, but the current V0
lowerer emits raw Rust item strings for the covered subset. That is acceptable
for the baseline, but Rust fixups should move toward operating on structured Rust
nodes rather than text.

## Shared context

One `Ctx` is threaded through every pass — it _is_ the three-source model in
code:

```
Ctx {
    symbols: SymbolTable,    // globals (const strings), function signatures
    diags:   Diagnostics,    // unsupported-construct reports
}
```

`Diagnostics` is what keeps the pipeline runnable as coverage grows: an
unhandled op lowers to a marked fallback (`todo!()` / an `unsafe` `libc` call /
a comment) and records a diagnostic, instead of crashing or silently dropping
code.

## Pipeline shape

The current pipeline is fixed and explicit in the CLI: emit CIR, parse CIR, load
Clang AST JSON, lower to Rust, and verify with generated differential tests. Do
not add pass scheduling machinery until a feature needs conditional ordering.

See [passes.md](passes.md) for the current stages and extension workflow.

## Adding a lowering feature

A lowering feature teaches Slate to preserve more C semantics in baseline Rust.
Examples: structs, arrays, pointer arithmetic, new arithmetic operators, `if`,
`switch`, or global variables.

Use this order:

1. Add or extend a C fixture in `tests/fixtures/`.
2. Run `cargo test --test differential generated_differential -- --nocapture`
   and confirm the generated path fails for the missing feature.
3. Inspect actual CIR with `cargo run -- emit-cir tests/fixtures/<name>.c`.
4. Inspect source context with `clang -Xclang -ast-dump=json -fsyntax-only`.
5. If CIR has enough information, add the `cir.*` handler in `src/lower.rs`.
6. If CIR has lost source facts, extend `src/c_ast.rs` to extract those facts
   from Clang JSON and preserve the raw node while the compact model catches up.
7. Keep baseline Rust conservative: `#[repr(C)]`, raw pointers, explicit temps,
   `libc`, and `unsafe` are acceptable.
8. Run `cargo fmt` and `cargo test`.

For structs specifically, the baseline shape is:

- Read `RecordDecl` / `FieldDecl` from Clang AST.
- Emit a Rust `#[repr(C)] struct` item with C-compatible field types.
- Map simple `cir.get_member` operations to conservative Rust field access.
- Add broader and layout-sensitive tests before attempting any idiomatic rewrite.

Do not make a feature pass produce prettier Rust as part of baseline lowering.
First make it compile and match C behavior.

## Adding a Rust fixup

A fixup changes already-correct Rust into more idiomatic or safer Rust. Examples:
`libc::printf` to `print!` / `println!`, collapsing retval temps, removing
single-use temps, or recovering `for` loops.

Use this order:

1. Start from a generated fixture that already passes differential testing.
2. Add a focused expected-behavior test for the fixup. It should still run the
   differential harness; output and exit code must not change.
3. Match a narrow Rust pattern. If the proof is weak, leave the baseline code
   unchanged.
4. Use Clang AST/raw JSON only when CIR/Rust shape cannot prove the rewrite.
5. Make the fixup optional in spirit: disabling it should still leave correct
   baseline Rust.

For `printf -> println!`, the recognizer should only rewrite calls where:

- the callee is known to be `printf` or a supported printf-family function;
- the format argument is a recovered constant C string;
- every format specifier is supported;
- Rust formatting can express the same output.

Non-constant or unsupported formats stay as `unsafe { libc::printf(...) }`.
