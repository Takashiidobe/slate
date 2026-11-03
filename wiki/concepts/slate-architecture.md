# Slate architecture

<!-- toc -->
- [Pipeline](#pipeline)
- [Ownership](#ownership)
- [Commands](#commands)
- [Projects and targets](#projects-and-targets)
- [Macro templates](#macro-templates)
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
one `macro_rules` template per distinct body, and each site becomes a call. Any
other expansion stays inline, and `macro_keep.rs` records why in the merge report.

- **Which expansions are kept**: `frontend/macro_keep.rs` decides on the IR,
  before lowering. It rejects `goto` and `case`, a `break` or `continue` leaving the
  body (lowered labels are hygienic, so the template could not name the caller's
  loop), an argument that reads a local the body declares, locals in functions
  whose dispatch lowering hoists
  them, variadic or `#` macros, compound literals and temporary objects (a template's
  block would end their C lifetime early), value expansions of type `void`, and place
  holes that are not plain (bit-fields, volatile, atomic, reduced alignment).
- **Caller locals**: Rust hygiene hides a caller's locals from a `macro_rules` body,
  so `pass_caller_locals` turns each one a kept body reads or writes into a hidden
  place parameter after the definition's own, named after the local: `BITS(n)`
  becomes `BITS!(n, hold)`. It runs on the final IR, wrapping the binding in
  `PlaceKind::MacroArg` and recording the list in `Module::macro_hidden_arguments`.
  A nested macro's hidden argument is the enclosing template's own local or
  parameter. Defining the macro inside each function was rejected because it gives
  up sharing across functions, units and targets.
- **Value templates**: a `ValueKind::MacroExpansion` lowers to an `Expr::Macro` in
  value position. While the rules run its body ends in `Stmt::Return(value)`, and
  `Item::MacroRules::value` tells that marker from a real `return` ending a statement
  template; codegen prints the marker as the expression, or a block ending in it.
- **`return`**: Rust's `return` crosses `macro_rules`, so a body containing one lifts.
  A function calling such a template (`Item::MacroRules::returns`, which includes
  templates nesting one) keeps its C signature in every lift, because the return
  analyses cannot see that return.
- **Identity**: two bodies are the same template when their parameters and printed
  Rust match. Spans, local names and argument types do not count: a hole is an
  opaque `$a()` to every rule, and filling it gives back that site's own body. Text
  is the key because `NamedPrim`'s `Debug` includes the target width: a `c_ulong`
  body merges across targets, and a body that lowers differently on one target gets
  a variant. The key is taken before the rules run, so two bodies that only converge
  under the rules stay separate names.
- **Names**: templates carry a hash, `NAME__slate_template_<fnv16>`, through
  compilation so function merge compares equal across units and targets.
  `assign_template_names` renames them at the end of the merge to `NAME`,
  `NAME__2`, and so on, most used first.
- **Holes**: an argument is a `$a()` call while the rules run, so that no rule
  moves, merges, or drops its evaluation. `finish_holes` turns it into `$a` before
  printing.
- **Place parameters**: while the rules run, a parameter used as a place is passed
  by address. The call is `M!(&raw mut n)` when the template writes it or takes its
  mutable address, otherwise `M!(&raw const n)`, and the body uses `*$*n()`. Every
  analysis already understands those forms, so none needs a per-template table.
  `finish_holes` prints the body as `$n`, and `finish_place_arguments` strips the
  address last, after the final `remove_mut` pass.
- **Type, name and list parameters**: `finish_macro_site` replaces a type argument's
  Rust type in `let`s and non-literal casts with `$T:ty`, and a name argument's field
  accesses with `$f:ident`. Filling the parameter gives back that site's own body, so
  this is sound even when an occurrence did not come from the argument. A parameter
  with no occurrence is dropped, leaving the concrete spelling in the body:
  `sizeof(T)`, a struct tag, a bit-field, or a callee naming a function. Each element
  of a `f args` list is a separate `$args__N`, so a different argument count is a
  different template. Writes to bit-fields stay inline.
- **Analyses**: `remove_mut` treats a template's plain arguments as reads.
  `inline_temps` will not inline into a template argument. `string_params` and
  `pointer_lattice` keep any pointer a template argument names raw, so lifting
  never changes a type a template relies on. `length_lattice` rewrites calls inside
  template arguments. A function named in a template body keeps its C signature in
  every lift, because those passes do not rewrite template bodies. The name resolves
  at the call site, so a template shared across units reaches each unit's own
  `static` function of that name.
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

## System declarations

- `slate-libc-gen` validates C declarations against vendored binding providers
  and emits alias sets mapping to one canonical `slate_support` path. See its
  [usage](../../crates/slate-libc-gen/README.md).
- Apple `mach2` paths and header candidates come from target-specific rustdoc
  JSON, exposed through `slate_support::mach`. No hand-maintained symbol list;
  ambiguous names and failed ABI probes retain local declarations.
- The catalogs contain names, destinations and translation-target eligibility.
  Layouts, signatures, linker names and values stay in the generator report.
- Lookup uses the translation target, never the host compiler cfg. Unknown or
  incompatible targets keep local declarations; canonical paths merge downstream.
- User definitions and non-system declarations stay local. Sema retains named
  system constant macro references in the IR; lowering maps the complete reference.
  Expanded expression fragments and numeric literals are never matched.
- Vendored libc defaults to FreeBSD 15 and 64-bit file offsets/time on 32-bit
  glibc. Header probes and workspace libc builds select the matching settings.
