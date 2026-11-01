# Rust rewrite engine

<!-- toc -->
- [Ownership](#ownership)
- [Scheduling and facts](#scheduling-and-facts)
- [Control-flow rewrites](#control-flow-rewrites)
  - [Explicit-tail-call candidate](#explicit-tail-call-candidate)
- [Macro boundaries](#macro-boundaries)
- [Validation](#validation)
- [Design history](#design-history)
<!-- /toc -->

## Ownership

| Component under `crates/slate/src/backend` | Responsibility                                       |
| ------------------------------------------ | ---------------------------------------------------- |
| `rust_ast`, `codegen`                      | Rust representation and emission                     |
| `interproc`                                | Call-graph, pointer, length and string analyses      |
| `engine`                                   | Arena, rule dispatch, caches and worklist scheduling |
| `engine/rules`                             | Registered local rewrites                            |

The backend consumes the Rust AST and the IR module's target facts. C typing,
conversions and layout remain slate-parser responsibilities.

## Scheduling and facts

- Run whole-program analyses before local worklist rewrites.
- Dispatch registered rules by node kind and call anchors.
- Reschedule affected neighbors and parents after an edit; bound oscillation with the edit budget.
- Maintain facts against the edited arena; cached facts must not outlive the changes that invalidate them.
- Recover safer representations only from conservative evidence; uncertain aliases or escapes retain raw pointers.
- The rule registry and implementation are authoritative for current ordering and coverage.
- [Pointer capability lattice](pointer-capability-lattice.md) defines the interprocedural representation choices.
- Comments are sibling `Comment` nodes tagged leading, trailing, or detached
  ([comment-placement](comment-placement.md)). A rule that removes a node must
  call `Arena::retire` (leaf) or `Arena::retire_subtree` (container) instead of
  `take` plus a manual list edit, or `release_comments` when it splices the node
  away itself. The node's leading and trailing comments go to the replacement
  node, else the next surviving sibling (trailing becomes leading), else the end
  of the block as detached. Comments inside a discarded subtree move to the
  replacement as leading.

- `printf_format` wraps every libc stdio writer in a Rust stdout flush and
  `fflush(NULL)`, so `print!` and libc output keep their order. Never skip this
  because a stream looks like `stderr`: `stderr = stdout`, `freopen` and
  `setvbuf` can all change where it writes or how it buffers. ucrt defines
  `printf` and `fprintf` inline, so calls to them carry
  `FunctionIdentity::HeaderInline`. They get the flushes but no libc identity,
  because `print!` would skip Windows' text-mode `\n` to `\r\n` translation.
- A libc identity needs the known function's header to be the outermost system
  header or the declaring file. bionic's `stdlib.h` includes `stdio.h`, so the
  outermost header alone is not enough.

## Control-flow rewrites

| CFG shape                               | Rust-to-Rust rewrite                                                                  |
| --------------------------------------- | ------------------------------------------------------------------------------------- |
| Straight-line chain / forwarding states | Coalesce basic blocks / thread jumps                                                  |
| Acyclic region                          | `if` / `match` and labeled join blocks                                                |
| Natural loop                            | Labeled `loop`; existing loop/while/for peepholes refine it                           |
| Multi-entry cyclic region               | Existing SCC-local dispatch; explicit-tail-call helpers are an optimization candidate |

- Retained rules: `engine/rules/structure_goto.rs` and
  `structure_goto/reducible.rs` provide normalization, dominators, natural loops,
  and irreducible-SCC handling. `structure_dispatch.rs` handles switch recovery.
- Reuse the retained algorithms and registry; the removed frontend was CIR-specific,
  but these passes consume Rust AST. See [pass porting](pass-porting-workflow.md).
- Project translation runs the interprocedural rewrites once over all units
  (extern decls link to the defining unit by symbol), then the per-function rules
  per unit in parallel. Executable projects may lift non-root exported functions to
  pointer-ABI-identical types and retype callers' extern decls; library projects
  only touch `static` functions, so exported C signatures are kept.
- Executable projects also link units in Rust (`backend/link/`). Before the
  interproc solve, partition refinement groups each unit's records and structs
  by shape and by the groups of the types they reference. Generated
  `__SlateRecordN` names match by shape alone, a type's bit-unit wrappers count
  as part of its shape, and an opaque `struct T {}` joins the only complete
  group named `T`. Types named in an `impl` header, along with `LongDouble` and
  `VaList`, stay unit-local. Each group that spans more than one unit moves to
  `src/__slate_types.rs`, and units import it as `use crate::__slate_types::S as
local;`, so their bodies are unchanged. A liftable export whose every
  cross-unit decl matches its definition type for type under those groups is
  bound by path: it loses `no_mangle` and `extern "C-unwind"`, callers'
  decls become `use crate::__slate_unit_<stem>::f;`, and string_params and
  length_lattice treat it as a candidate like a `static` function. An export
  that cannot be lifted (address-taken, `unsafe`, or with a `link_section`) is
  still bound by path under the same decl check, but keeps `no_mangle`,
  `extern "C-unwind"` and its C params, because its address may reach C code:
  sqlite passes `&sqlite3Apis` to `dlsym`'d extension entry points, so the
  fn-pointer fields that store these functions cannot change ABI.
  Rewrites are always on: there is no raw lowering mode.
- Function identity: lowering records every function whose address it emits;
  `frontend/distinct_functions.rs` marks the address-taken definitions that
  LLVM MergeFunctions could fold into another definition with the same
  signature shape, and `backend::place_in_distinct_sections` gives each one a
  unique `.text.slate_distinct.<unit>.<name>` link section (ELF only).
- Goto recognition accepts legacy and `__slate_dispatch` / `__slate_state` names,
  integer / `usize` state values, and transfers inside conditional arms.
  Nested decisions stay in their original blocks; dispatcher exits remain labeled block exits.
- Keep loop-transfer labels when crossing labeled join blocks.
- Switch recovery accepts the current two-statement case-selector / dispatch scope.
- [SQLite runtime case study](sqlite-runtime-performance.md) measures the effects of these generic passes.
- Coalesce before structuring. Choose structured output for reducible regions;
  consider tail calls at the irreducible-SCC decision point before fallback localization.

### Explicit-tail-call candidate

- Example: `entry -> A or B`, with `A -> A or B` and `B -> A or B`.
  Extract each basic block into an internal helper; successor edges become
  `become a(...)` / `become b(...)`, removing the shared state dispatcher.
- Use guaranteed tail transfers through experimental
  [`explicit_tail_calls`](https://doc.rust-lang.org/unstable-book/language-features/explicit-tail-calls.html),
  rather than assuming an ordinary call will be optimized into a jump.
- Pass required live scalar values with compatible helper signatures/ABIs;
  passing a large state aggregate can retain the original copy overhead.
- Keep address-escaping C locals in stable storage owned by the outer wrapper.
  Helpers must not tail-transfer pointers into their disappearing local frames.
- Preserve the original exported function and C ABI, evaluation order, skipped
  initializers, uninitialized storage, and all region exits.
- Helper extraction affects function items and call-graph facts; integrate it at
  that boundary and invalidate/recompute affected analyses.
- Compare against structured output and SCC-local dispatch: executed C/Rust parity,
  tail-jump assembly, bounded stack use, live-state copies, code size, and runtime.
  Keep dispatch when eligibility or profitability is not established.
- Work: `slate-cgqg.3.1` coalescing, `slate-cgqg.3.3` retained-pass connection,
  `slate-cgqg.3.4` tail-call evaluation; retained structuring history: `slate-04q.85`.

## Macro boundaries

Under `--merge`, a C macro expansion carries two wrappers through the rules, so
its arguments stay separate from its body for
[macro recovery](preprocessor.md#macro-invocations).

- `Stmt::Scope { macro_region: Some(name) }` marks the statements of one
  invocation. `singleton_scopes` does not flatten it.
- `Expr::MacroArg { index, expr }` marks an argument. Rules that match a shape
  stop at it, so `!(a == b)` in `ASSERT(a == b, "m")` does not become `a != b`.
  Rules still rewrite inside an argument, and analyses (reads, effects, the
  interprocedural lattices) look through it.
- `lower_macro_region` only creates a region for a macro's own statements. It
  skips `let` and hoisted temporaries at the top of the expansion, and any
  expansion with `return`, a `goto`, a label, asm, a `case`, or a `break` or
  `continue` that leaves it. An expansion whose body reads a caller local gets
  no wrappers, so it rewrites as before.
- A new walker over `Expr` must handle `MacroArg`. A shape-matching rule should
  not peel it.
- `macro_boundaries::strip` removes both wrappers after the last rule pass, so
  codegen and the project merge never see them.

Expression-position macros are not wrapped yet: `PRINT(x);` from
`#define PRINT(x) printf x` has a statement that ends outside the invocation.

`macro_fold::fold` turns regions into `Item::MacroRules` and `Name!(args)` calls.
It runs after `printf_format` and before the unsafe pass, because that pass
removes `unsafe` depending on the surrounding block and would give one macro
different bodies. The unsafe pass then runs over the macro bodies too.

A template is the region's own code with each `MacroArg` replaced by `$param`,
so it re-expands to what was lowered by construction. Each unit keeps the most
common template per macro name. Other regions of that macro stay inline, and a
failed fold costs readability, never correctness.

- A parameter written after a name (`fprintf x`) is a list: `$($x:expr),*`, last
  in the matcher. `rustfmt` cannot format `$(...)` in a macro body, so a
  placeholder identifier stands in until `format.rs` restores it after formatting.
- `foldable` is decided at lowering, where the definition and statement locations
  exist. A neighbouring statement that overlaps the invocation means the region
  is partial: `ERR_RETURN`'s `return` ends at the `;` after the invocation, so a
  containment test misses it and the check has to be overlap.
- The goto graph keeps a braced macro block with no `let` as one node. Without
  that, it splits the `if` from its body and the body looks like the whole macro.

These stay inline: object-like and variadic macros, `#` and `##`, macros reading
or writing a caller local or label, arguments the IR hoists or writes
(`STORE(p++, v)`), an argument reading a name the body binds (hygiene would change
which one it means), writes through an argument (`UPDATE_HASH(s, s->ins_h, c)`,
whose place has no `MacroArg`), a list parameter that is not last, and names that
are Rust keywords or would shadow a std macro generated code uses.

Under `--merge` the largest group of units with identical definitions shares
`src/__slate_macros.rs` through `#[macro_use]`. Other units keep their own
definition, which shadows it. A macro body resolves items at the call site, so
each unit's own declarations apply.

## Validation

```bash
cargo clippy -p slate-c2rust --allow-dirty --fix
cargo fmt
cargo nextest r --release --profile slate
```

Run from the workspace root. The Slate profile covers rewritten single-file
fixtures, optimized project fixtures, and multi-unit project linkage. `translate` and project
translation both run the full backend. FileCheck
blocks in `tests/fixtures/` show which rewrites fire, and the `slate` profile
enforces them ([FileCheck](differential-fixtures.md#filecheck)).

## Design history

- [Historical index](../historical/index.md): prior worklist, query and Salsa designs, pass catalogs and measurements.
- [Pass-porting workflow](pass-porting-workflow.md): current workflow for adapting retained algorithms.
- Earlier corpus timings used a different frontend and are not current performance claims.
