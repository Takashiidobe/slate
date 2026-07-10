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
- `cir.scope`, `cir.for`, `cir.while`, `cir.condition`, `cir.yield`,
  `cir.return`.
- global constant strings used by `printf`.
- CIR integer aliases such as `!s32i = !cir.int<s, 32>`, mapped to Rust integer
  primitives.
- source enum constants from Clang AST, emitted as Rust `const` items.
- source union records from Clang AST, emitted as `#[repr(C)] union` items with
  basic `cir.get_member` field access.
- source struct records from Clang AST, emitted as `#[repr(C)] struct` items
  with basic `cir.get_member` field access.
- fixed-size CIR arrays, emitted as Rust arrays with basic `cir.get_element`
  indexed loads and stores.
- `sizeof` expressions when Clang has folded them to CIR integer constants.
- `cir.load` and `cir.store` with `is_volatile`, emitted as
  `std::ptr::read_volatile` and `std::ptr::write_volatile`.
- file-scope `cir.global` integers with constant initializers, emitted as Rust
  `static mut` items.

Unknown CIR ops emit a `todo!("cir.xyz")` expression and a diagnostic. That is
intentional: failing loudly is better than silently dropping semantics.

Current C fixture coverage:

| Fixture | Covered behavior |
| --- | --- |
| `add.c` | `int` functions, params, locals, addition, returns, calls |
| `loop_sum.c` | `for` loops, comparisons, increments, compound addition |
| `while_loop.c` | `while` loops, comparisons, increments, compound addition |
| `enums.c` | enum constants, implicit values, explicit positive and negative values |
| `unions.c` | union declaration, integer fields, field writes, field reads |
| `structs.c` | struct declaration, integer fields, field writes, field reads |
| `arrays.c` | fixed-size local arrays, indexed stores, indexed loads |
| `sizeof.c` | `sizeof` over primitive, array, struct, union, and expression forms |
| `volatile.c` | volatile local stores and loads |
| `static_globals.c` | file-scope static integer global loads and stores |

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
model (`Enum`, `Function`, `Decl`, `Stmt`, `Expr`, `CType`, `Loc`), and
preserves each function's raw JSON node for later features that need facts not
yet modeled.

### lower

The lowerer is the only stage that knows CIR op semantics. It currently emits raw
Rust item strings for the covered baseline. That is acceptable for V0, but any
nontrivial fixup should operate on structured Rust nodes instead of string
rewrites.

## Adding a feature

A feature expands baseline C coverage. Examples: structs, arrays, pointer
arithmetic, new arithmetic operators, globals, `if`, `switch`.

See [adding-features.md](adding-features.md) for the step-by-step workflow. The
short version is: add a C fixture under `tests/fixtures/`, inspect CIR and Clang
AST as needed, implement conservative baseline lowering, run `cargo test`, and
refresh ignored generated fixtures with `cargo run -- emit-fixtures`.

## Known baseline gaps

The next baseline features should be added one fixture at a time:

- More scalar operations: subtraction, multiplication, division, modulo, bitwise
  ops, logical ops, unary negation, and explicit casts.
- Full control flow: `if`, `break`, `continue`, `switch`, and `goto`.
- Aggregate types: broader arrays, broader structs and unions, more field
  access, initialization, and layout-sensitive tests.
- Pointers: address-of, dereference, pointer arithmetic, null, arrays as
  pointers, and const-correctness.
- Globals: static locals, non-integer globals, richer initialization, and
  linkage.
- Source model: typedefs, named enum types as variable types, prototypes, and
  header-origin declarations.
- Target model: signedness and width for all C integer spellings, enum
  underlying type choices, pointer width, and ABI alignment.
- Calls: non-`printf` libc functions, user prototypes without bodies, and
  varargs beyond the direct `printf` fixture shape.

For structs, start by extracting `RecordDecl` / `FieldDecl` from Clang AST, then
emit `#[repr(C)]` Rust structs with C-compatible field types. Only after layout
and field access are correct should you attempt any idiomatic Rust rewrite.

## Adding a fixup

A fixup improves already-correct Rust. Examples: `printf -> println!`,
collapsing retval temps, inlining single-use temps, or recovering `for` loops.

See [adding-features.md](adding-features.md) for the split between baseline
language work and fixups. A fixup must start from generated Rust that already
passes differential testing.

For `printf -> println!`, only rewrite when the callee is known, the format
argument is a constant C string, every format specifier is supported, and Rust
formatting can express the same output. Everything else stays as
`unsafe { libc::printf(...) }`.
