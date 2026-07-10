# Passes

The translator is a small fixed pipeline. Keep it that way until there is a real
need for pass scheduling or configuration.

## V0 principle: transliterate, don't optimize

Baseline lowering emits the most mechanical, faithful Rust it can, leaning on
`libc` and `unsafe` wherever C semantics do not map directly to safe Rust. Every
CIR value is materialized into its own `let` temp. This is ugly, but it freezes
each value at its CIR definition and avoids load-after-mutation hazards.

Readability is recovered later by Rust fixups, not during baseline lowering.

## Current pipeline

| Stage | In -> Out | How | Status |
| --- | --- | --- | --- |
| **emit-cir** | C -> CIR text | `clang -fclangir -emit-cir` piped to `cir-opt --mlir-print-op-generic` | implemented |
| **parse-cir** | CIR text -> generic Op-tree + locs | recursive-descent parser over MLIR generic form | implemented |
| **load-ast** | C -> compact source context + raw JSON | `clang -Xclang -ast-dump=json -fsyntax-only` | implemented |
| **lower** | CIR + AST context -> Rust source | match `op.name`; materialize temps; use `libc` / `unsafe` | implemented |
| **main-normalize** | C `main` return -> process exit | emit `std::process::exit(code)` | implemented inside lower |
| **generated-diff** | C + generated Rust -> output comparison | build generated Rust with Cargo + `libc`, compare stdout + exit code | implemented |

Current code path:

```text
emit-cir -> parse-cir -> load-ast -> lower(libc/unsafe) -> generated-diff
```

## Current baseline coverage

The lowerer currently handles the fixture subset:

- `cir.func`, `cir.alloca`, `cir.store`, `cir.load`, `cir.const`.
- `cir.add`, `cir.inc`, `cir.cmp`.
- `cir.get_global`, `cir.cast`, `cir.call`.
- `cir.scope`, `cir.for`, `cir.condition`, `cir.yield`, `cir.return`.
- global constant strings used by `printf`.

Unknown CIR ops emit a `todo!("cir.xyz")` expression and a diagnostic. That is
intentional: failing loudly is better than silently dropping semantics.

## Stage notes

### parse-cir

The parser is deliberately generic. It produces:

```text
Op { results, name, operands, attrs, regions, ty, loc }
```

It does not know what `cir.add` or `cir.for` means. The lowerer owns op
semantics.

### load-ast

`src/c_ast.rs` is a Clang AST oracle, not a handwritten C parser. It filters
Clang's JSON dump down to source-file function definitions, extracts a compact
model (`Function`, `Decl`, `Stmt`, `Expr`, `CType`, `Loc`), and preserves each
function's raw JSON node for later features that need facts not yet modeled.

### lower

The lowerer is the only stage that knows CIR op semantics. It currently emits raw
Rust item strings for the covered baseline. That is acceptable for V0, but any
nontrivial fixup should operate on structured Rust nodes instead of string
rewrites.

## Adding a feature

A feature expands baseline C coverage. Examples: structs, arrays, pointer
arithmetic, new arithmetic operators, globals, `if`, `switch`.

Workflow:

1. Add a C fixture under `tests/fixtures/` and a matching expected hand-written
   `.rs` only if needed for the legacy differential test.
2. Run `cargo test --test differential generated_differential -- --nocapture`
   and confirm it fails for the missing feature.
3. Inspect real CIR:

   ```bash
   cargo run -- emit-cir tests/fixtures/<name>.c
   ```

4. Inspect source facts:

   ```bash
   $SLATE_CLANG -Xclang -ast-dump=json -fsyntax-only tests/fixtures/<name>.c
   ```

5. If CIR has enough information, add a handler in `src/lower.rs`.
6. If CIR lost needed source facts, extend `src/c_ast.rs` to extract them from
   Clang JSON. Keep the raw JSON as an escape hatch.
7. Keep the emitted Rust conservative and C-shaped.
8. Run `cargo fmt` and `cargo test`.

For structs, start by extracting `RecordDecl` / `FieldDecl` from Clang AST, then
emit `#[repr(C)]` Rust structs with C-compatible field types. Only after layout
and field access are correct should you attempt any idiomatic Rust rewrite.

## Adding a fixup

A fixup improves already-correct Rust. Examples: `printf -> println!`,
collapsing retval temps, inlining single-use temps, or recovering `for` loops.

Workflow:

1. Start with a generated fixture that already passes differential testing.
2. Add a test that proves output and exit code remain unchanged.
3. Match a narrow Rust pattern. If the proof is weak, leave baseline Rust alone.
4. Consult Clang AST/raw JSON only when the Rust pattern alone cannot prove the
   rewrite.
5. Keep the fixup optional in spirit: turning it off should still leave correct
   baseline Rust.

For `printf -> println!`, only rewrite when the callee is known, the format
argument is a constant C string, every format specifier is supported, and Rust
formatting can express the same output. Everything else stays as
`unsafe { libc::printf(...) }`.
