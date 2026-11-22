# Rust rewrite engine

<!-- toc -->
- [Ownership](#ownership)
- [Scheduling and facts](#scheduling-and-facts)
- [Control-flow rewrites](#control-flow-rewrites)
  - [Explicit-tail-call candidate](#explicit-tail-call-candidate)
- [Unsafe placement](#unsafe-placement)
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
- Reschedule affected neighbors and parents after an edit; bound oscillation
  with the edit budget.
- Maintain facts against the edited arena; cached facts must not outlive the
  changes that invalidate them.
- Recover safer representations only from conservative evidence; uncertain
  aliases or escapes retain raw pointers.
- The rule registry and implementation are authoritative for current ordering
  and coverage.
- [Pointer capability lattice](pointer-capability-lattice.md) defines the
  interprocedural representation choices.
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

No goto rule is left in `backend/engine/rules/` (`slate-8ge7.24.10`). C gotos
are structured before Rust exists:

| CFG shape                                   | Where                                                                                                  |
| ------------------------------------------- | ------------------------------------------------------------------------------------------------------ |
| Forward/backward goto, label in a block/arm | IR pass `structure-goto` (labeled blocks, loops, label lifting, goto forwarding)                        |
| Diamond / single-exit tail                  | IR pass `labeled-blocks` (if/else and `match` arms, [slate-analysis](slate-analysis.md#labeled-blocks)) |
| Residual region (indirect gotos, rest)      | `lower_dispatch` + `frontend/lowerer/structure.rs`: dominators, natural loops, join labels             |
| Multi-entry cyclic region                   | `structure.rs` `split_irreducible`: a `__slate_state` selector per multi-entry SCC, recursively       |
| Switch fallthrough                          | `frontend/lowerer/switch.rs`: duplicated-tail `match`, or a case loop past the budget                  |

- `structure` returning `None` (an arm shape the extractor does not parse)
  keeps the flat `'__slate_dispatch: loop { match __slate_state }`; 5 regions
  in 4 fixtures after `slate-8ge7.24.10.11`.
- `label_elide` stays: `drop_label`, `drop_loop_label`, `tail_break_drop`,
  `loop_tail_break` and `break_to_else` clean up lowering's own labeled blocks
  (`continue` in a `for` with an increment, switch duplication, cleanup
  scopes), not gotos; moving them is `slate-8ge7.24.11`.
- Reuse the retained algorithms and registry; the removed frontend was
  CIR-specific, but these passes consume Rust AST. See
  [pass porting](pass-porting-workflow.md).
- Project translation runs the interprocedural rewrites once over all units
  (extern decls link to the defining unit by symbol), then the per-function
  rules per unit in parallel. Executable projects may lift non-root exported
  functions to pointer-ABI-identical types and retype callers' extern decls;
  library projects only touch `static` functions, so exported C signatures are
  kept.
- Executable projects also link units in Rust (`backend/link/`). Before the
  interproc solve, partition refinement groups each unit's records and structs
  by shape and by the groups of the types they reference. Generated
  `__SlateRecordN` names match by shape alone, a type's bit-unit wrappers count
  as part of its shape, and an opaque `struct T {}` joins the only complete
  group named `T`. Types named in an `impl` header, along with `LongDouble` and
  `VaList`, stay unit-local. Each group that spans more than one unit moves to
  `src/__slate_types.rs`, and units import it as
  `use crate::__slate_types::S as local;`, so their bodies are unchanged. A
  liftable export whose every cross-unit decl matches its definition type for
  type under those groups is bound by path: it loses `no_mangle` and
  `extern "C-unwind"`, callers' decls become
  `use crate::__slate_unit_<stem>::f;`, and string_params and length_lattice
  treat it as a candidate like a `static` function. An export that cannot be
  lifted (address-taken, `unsafe`, or with a `link_section`) is still bound by
  path under the same decl check, but keeps `no_mangle`, `extern "C-unwind"` and
  its C params, because its address may reach C code: sqlite passes
  `&sqlite3Apis` to `dlsym`'d extension entry points, so the fn-pointer fields
  that store these functions cannot change ABI. Rewrites are always on: there is
  no raw lowering mode.
- Function identity: lowering records every function whose address it emits;
  `frontend/distinct_functions.rs` marks the address-taken definitions that LLVM
  MergeFunctions could fold into another definition with the same signature
  shape, and `backend::place_in_distinct_sections` gives each one a unique
  `.text.slate_distinct.<unit>.<name>` link section (ELF only).
- [SQLite runtime case study](sqlite-runtime-performance.md) measures the
  effects of these generic passes.

### Explicit-tail-call candidate

- Example: `entry -> A or B`, with `A -> A or B` and `B -> A or B`. Extract each
  basic block into an internal helper; successor edges become `become a(...)` /
  `become b(...)`, removing the shared state dispatcher.
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
- Compare against structured output and SCC-local dispatch: executed C/Rust
  parity, tail-jump assembly, bounded stack use, live-state copies, code size,
  and runtime. Keep dispatch when eligibility or profitability is not
  established.
- Work: `slate-cgqg.3.1` coalescing, `slate-cgqg.3.3` retained-pass connection,
  `slate-cgqg.3.4` tail-call evaluation; retained structuring history:
  `slate-04q.85`.

## Unsafe placement

Decided in `slate-8ge7.24.11.9` (epic `slate-8ge7.24.17`). Unsafe is a marker
on the operation that needs it, not a node in the tree. The lowerer emits no
`unsafe` blocks, rules match and rewrite an unsafe-free tree, and one placement
pass before codegen adds the blocks.

Markers. Only the lowerer knows types, so it sets them; a rule that moves or
copies an expression carries them along, and two equal operations compare equal
(`*p = *p * x` folds to `*p *= x` with no peeling).

| Operation | Marker |
| --- | --- |
| Raw-pointer deref | `UnaryOp::Deref(Pointee::Raw)`; reference deref is `Deref(Pointee::Ref)` |
| `static mut`, extern static, weak extern read | `unsafe_` flag on `Expr::Var`; `Expr::var(name)` builds the safe case |
| Unsafe callee, fn-pointer call, `zeroed`, `va_arg` | `unsafe_` on `Expr::Call` |
| `.add`, `.offset`, `.read`, `.write`, volatile access | `unsafe_` on `MethodCall` / `MethodCallGeneric` |
| Union field | `union` on `Expr::Field` (a C fact; `&raw const u.f` is safe) |
| `Transmute`, `CopyNonoverlapping`, `PtrCopy`, `WriteBytes`, `InlineAsm` | none: unsafe by variant |

A side table was rejected: Rust AST expressions have no ids and rules rebuild
them freely. A missing marker fails the fixture with E0133; a spurious one is an
`unused_unsafe` warning.

Placement runs after the rule engine and after interproc/link, so a call that
link binds to a safe Rust path (its flag cleared) gets no block. It is local to
one function and never makes a function `unsafe fn`; that stays the
`FunctionName::is_unsafe` fact (`slate-eep4.6`, `slate-eep4.11`). Per statement:

- A marked operation in an assignment target wraps the statement:
  `unsafe { *p = v; }`.
- Otherwise each expression slot (let initializer, assigned value, condition,
  return value, expression statement) wraps the lowest common ancestor of its
  marked operations: `x = unsafe { *p };`, `if unsafe { *p } != 0 {`.
- Place contexts (`&raw`, field base, method receiver) follow the rules
  `nested_unsafe` used. A closure body is placed inside the closure.
- Never a whole function body.

A separate post-pass then merges runs of adjacent whole-statement unsafe blocks
that contain no `let`. Macro bodies are placed from their own contents only.

Placement replaces `nested_unsafe` and `NodeKind::Unsafe`, and covers
`slate-o3gw.21`, `slate-o3gw.18` and `slate-y0qs.13`.

## Macro boundaries

Under `--merge`, a kept C macro expansion lowers to a template and a call, as
described in [macro templates](slate-architecture.md#macro-templates). While the
rules run, an argument is a `$a()` hole, so no rule moves, merges, or drops its
evaluation, and a shape-matching rule cannot see through it.

`finish_holes` turns holes into `$a` after `printf_format` and before the unsafe
pass, because that pass removes `unsafe` depending on the surrounding block and
would give one macro different bodies. The unsafe pass then runs over the macro
bodies too.

Expansions `macro_keep.rs` rejects carry no structure and stay inline, so a
rejected lift costs readability, never correctness.

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
fixtures, optimized project fixtures, and multi-unit project linkage.
`translate` and project translation both run the full backend. FileCheck blocks
in `tests/fixtures/` show which rewrites fire, and the `slate` profile enforces
them ([FileCheck](differential-fixtures.md#filecheck)).

## Design history

- [Historical index](../historical/index.md): prior worklist, query and Salsa
  designs, pass catalogs and measurements.
- [Pass-porting workflow](pass-porting-workflow.md): current workflow for
  adapting retained algorithms.
- Earlier corpus timings used a different frontend and are not current
  performance claims.
