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

| Stage                   | Entry point                                                   | Responsibility                                                          |
| ----------------------- | ------------------------------------------------------------- | ----------------------------------------------------------------------- |
| Arguments and target    | `crates/slate/src/target.rs`                                  | Parser arguments, target default, layout options                        |
| Preprocess, parse, sema | `crates/slate-parser` via `src/frontend.rs`                   | C semantics and typed IR                                                |
| Rust lowering           | `src/frontend/lowerer.rs` and its modules                     | Translate typed values, places, statements and declarations             |
| Directives              | `src/frontend/{preprocess,directive_translate}.rs`            | Record directives and merge supported whole-item configuration branches |
| C runtime bridges       | `src/frontend/{c_shim,long_double}.rs`, `src/frontend/shims/` | ABI bridges for values Rust cannot pass directly                        |
| Backend                 | `src/backend/{rust_ast,codegen,engine,interproc}`             | Rust AST, emission, analyses and rewrites                               |
| Project orchestration   | `src/main.rs`, `src/compile_commands.rs`                      | Parse translation units, export symbols, write a Cargo crate            |

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

| Command             | Output                                                                                                                                                                                                                                                                                           |
| ------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `emit-slate-ir`     | Printed typed parser IR                                                                                                                                                                                                                                                                          |
| `lowering-barriers` | All lowering barriers, without emitting Rust                                                                                                                                                                                                                                                     |
| `translate`         | Rust after the backend pipeline; supported directive branches may become cfg items                                                                                                                                                                                                               |
| `record-cfg`        | Recorded directive activity, branches and diagnostics as JSON                                                                                                                                                                                                                                    |
| `translate-project` | Cargo crate with translation units as modules and optional C shims: a binary when one unit defines `main`, otherwise a library with `--crate-type` rlib/staticlib/cdylib. Each command parses as the compiler it names unless `--flavor` overrides ([compiler-arg-rules](compiler-arg-rules.md)) |

## Projects and targets

- Headers come from [slate-sysroots](../../crates/slate-sysroots/README.md).
- Target default and explicit argument precedence: [target queries](slate-target-queries.md).
- Compilation databases supply per-unit flags and targets. Relative paths resolve against each command's `directory`.
- Project translation requires exactly one unit defining `main`. Library projects, multiple configurations of one unit, and colliding module stems are rejected.
- External definitions are exported for linker resolution across modules; tentative common globals have one owner.
- Project generation runs the backend control-flow registry; `--raw` emits the baseline. Exported C signatures remain intact. Target-conditional single-file translation is separate.

## Macro templates

Under `translate-project --merge`, a kept `Statement::MacroExpansion` lowers to
one `macro_rules` template per distinct body, and each site becomes a call. The
old `macro_fold` pass still runs over everything else.

- **Which expansions are kept**: `frontend/macro_keep.rs` decides on the IR,
  before lowering. It rejects place holes, control flow leaving the body,
  bodies that call project functions or read caller locals (Rust hygiene hides
  them), locals in functions whose dispatch lowering hoists them, and variadic
  or `#` macros. The analyses cannot see into a template yet, so anything they
  would have to track stays on the old path until `slate-f2zu.9.5.12.6`.
- **Identity**: two bodies are the same template when their printed Rust and
  printed hole types match. Spans and local names do not count. Text is the key
  because `NamedPrim`'s `Debug` includes the target width: a `c_ulong` body merges
  across targets, and a body that lowers differently on one target gets a variant.
- **Names**: templates carry a hash, `NAME__slate_template_<fnv16>`, through
  compilation so function merge compares equal across units and targets.
  `assign_template_names` renames them at the end of the merge to `NAME`,
  `NAME__2`, and so on, most used first, skipping names the old fold took.
- **Holes**: an argument is a `$a()` call while the rules run, so that no rule
  moves, merges, or drops its evaluation. `finish_holes` turns it into `$a` before
  printing.
- **Analyses**: `inline_temps`, `string_params`, and `pointer_lattice` treat a
  template call's arguments conservatively.
- **Self-check**: with `SLATE_CHECK_MACRO_TEMPLATES=1` (set by the `project_merge`
  tests), the lowerer re-lowers each outermost site without structure and compares
  it to its expanded templates. A difference is `Invariant::MacroTemplateMismatch`.

## Validation

```bash
cargo nextest r --release --profile slate
```

- Run commands from the workspace root.
- Browser demos: build with `bash www/build.sh` and `bash www-parser/build.sh`;
  run `node tools/check_web_demo.mjs <site>/dist` for each before deployment.
- WASI runs the CLI directly with a 16 MiB linker stack; native CLI entry uses
  a dedicated thread. Browser header archives must dereference symbolic and
  hard links because the worker unpacker reads regular files only.
- The profile exercises raw lowering through host differential fixtures and corpus suites.
- Passing raw lowering tests does not establish rewrite-specific coverage.
- Opt-in LLVM translation validation:
  `python3 tools/alive2/validate.py crates/slate/tests/fixtures`. Discovers
  functions and retains per-fixture artifacts; unsupported cases never count as
  validated. See [usage and proof scope](../../tools/alive2/README.md).
- [Differential fixtures](differential-fixtures.md) defines the supported/unsupported ratchet and triage commands.
- Prior design records are collected in the [historical index](../historical/index.md).
