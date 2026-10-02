# Slate architecture

<!-- toc -->
- [Pipeline](#pipeline)
- [Ownership](#ownership)
- [Commands](#commands)
- [Projects and targets](#projects-and-targets)
- [Validation](#validation)
<!-- /toc -->

## Pipeline

```text
C + target headers -> slate-parser -> ir::Module
  -> frontend::lowerer -> rust_ast::Program -> Rust source
```

| Stage | Entry point | Responsibility |
| --- | --- | --- |
| Arguments and target | `crates/slate/src/target.rs` | Parser arguments, target default, layout options |
| Preprocess, parse, sema | `crates/slate-parser` via `src/frontend.rs` | C semantics and typed IR |
| Rust lowering | `src/frontend/lowerer.rs` and its modules | Translate typed values, places, statements and declarations |
| Directives | `src/frontend/{preprocess,directive_translate}.rs` | Record directives and merge supported whole-item configuration branches |
| C runtime bridges | `src/frontend/{c_shim,long_double}.rs`, `src/frontend/shims/` | ABI bridges for values Rust cannot pass directly |
| Backend | `src/backend/{rust_ast,codegen,engine,interproc}` | Rust AST, emission, analyses and rewrites |
| Project orchestration | `src/main.rs`, `src/compile_commands.rs` | Parse translation units, export symbols, write a Cargo crate |

Slate source paths in the table are relative to `crates/slate` unless prefixed
with `crates/`.

## Ownership

- C types, layout, conversions and evaluation order belong to slate-parser.
- Slate consumes typed IR as a library; [IR semantics](ir-spec.md) define the boundary.
- Baseline Rust may use raw pointers, `unsafe`, `libc` and explicit temporaries.
- Intrinsic generation and builtin dispatch: [intrinsic lowering](intrinsic-lowering.md).
- Unsupported constructs produce source-located `Barrier` records. Broken IR invariants produce `InvalidIr`; they are bugs, not unsupported features.
- The backend remains available. Its worklist and pointer analyses are described in [rewrite engine](rewrite-engine-v2.md) and [pointer capability lattice](pointer-capability-lattice.md).

## Commands

| Command | Output |
| --- | --- |
| `emit-slate-ir` | Printed typed parser IR |
| `lowering-barriers` | All lowering barriers, without emitting Rust |
| `translate-lowered` | Raw lowered Rust, formatted with prettyplease |
| `translate` | Rust after the backend pipeline; supported directive branches may become cfg items |
| `record-cfg` | Recorded directive activity, branches and diagnostics as JSON |
| `translate-project` | Executable Cargo crate with translation units as modules and optional C shims |

## Projects and targets

- Headers come from [slate-sysroots](../../crates/slate-sysroots/README.md).
- Target default and explicit argument precedence: [target queries](slate-target-queries.md).
- Compilation databases supply per-unit flags and targets. Relative paths resolve against each command's `directory`.
- Project translation requires exactly one unit defining `main`. Library projects, multiple configurations of one unit, and colliding module stems are rejected.
- External definitions are exported for linker resolution across modules; tentative common globals have one owner.
- Project generation runs the backend control-flow registry; `--raw` emits the baseline. Exported C signatures remain intact. Target-conditional single-file translation is separate.

## Validation

```bash
cargo nextest r --release --profile slate
```

- Run commands from the workspace root.
- The profile exercises raw lowering through host differential fixtures and corpus suites.
- Passing raw lowering tests does not establish rewrite-specific coverage.
- [Differential fixtures](differential-fixtures.md) defines the supported/unsupported ratchet and triage commands.
- Prior design records are collected in the [historical index](../historical/index.md).
