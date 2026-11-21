# slate-analysis

<!-- toc -->
- [Boundary](#boundary)
- [Where a rewrite belongs](#where-a-rewrite-belongs)
- [Transform stage](#transform-stage)
- [Keys](#keys)
- [Call resolution](#call-resolution)
- [Scaling rule](#scaling-rule)
- [Pointer capabilities](#pointer-capabilities)
- [Effect summaries](#effect-summaries)
- [Def-use](#def-use)
  - [Counted loops](#counted-loops)
  - [Inline temporaries](#inline-temporaries)
- [Shadow mode](#shadow-mode)
- [Related](#related)
<!-- /toc -->

`crates/slate-analysis` runs whole-program analyses over slate-parser's
`ir::Module`s and hands their results to `crates/slate`'s lowering as read-only
facts. It replaces analyses that today re-derive C meaning from Rust spellings
after lowering (`backend/interproc`, `backend/engine/rules`). Tracked by
`slate-8ge7`.

```text
ir::Module per unit -> Program (one target) -> analyze -> Facts
  -> lowerer::lower(module, ..., facts.unit(id)) per unit, in parallel
```

## Boundary

- Input is one target's program: every unit of one configuration, after
  `project::merge::without_definitions` has dropped definitions owned by other
  units. Multi-target `--merge` joins per-target translations after lowering;
  the analysis never sees two targets at once.
- Analyses read the IR only. They never read `rust_ast`, spellings, or another
  analysis's output from the Rust side.
- A fact is a statement about a C entity that holds for the whole program and
  that lowering needs to choose a Rust form: a binding's pointer capability, a
  value's proven length. A fact never names a Rust type or rewrite; lowering
  decides the representation from it.
- Facts are computed once by `analyze` and are immutable afterwards. Nothing
  mutates the store during lowering, and lowering never feeds facts back.
- A missing fact means "unknown" and must lower to the transliterated form.
  An analysis that cannot see a callee or a use gives up for that entity, not
  for the program.
- C semantics the checker needs stay in slate-parser's sema. Facts that only
  idiomization needs move here (`slate-8ge7.8`): `counted_loops` was the first.

## Where a rewrite belongs

The transform stage (`slate-8ge7.24`) puts IR -> IR canonicalizations ahead of
the facts: analyses the transforms need, then transforms, then the fact
analyses once, then lowering. The goal is a simpler lowerer, not optimization:
every transform leaves IR a C programmer could have written. Decide by what
the rewrite's precondition reads:

| Precondition | Place | Examples |
| --- | --- | --- |
| C meaning only, and the result is still C | IR transform | const-fold, cast and truth canonicalization, dead code, decl-init |
| C meaning, but the result is a Rust form | fact; lowering picks the form | pointer capability, counted loops, `mut`, libc callees |
| One IR node already decides the spelling | lowering emits it directly | `+=`, `while`, `.add()`, `.is_null()`, `c""` |
| The Rust form the lowerer chose | Rust rule after lowering | `unsafe` scoping, `MaybeUninit` slots, Rust scopes |

A rule never sits half in each layer: port it whole or leave it. The
2026-11-21 triage of `backend/engine/rules` (`slate-8ge7.24.3`) assigns each
rule a place and a port bead.

## Transform stage

`slate_analysis::transform` (`slate-8ge7.24.1`) holds the IR -> IR passes.
`PASSES` lists each pass with its name and the slots it runs in, and
`Passes::run(module, slot)` runs the selected passes of one slot in list
order. `frontend::lower_parsed` calls two slots per unit:

- `Slot::BeforeMacroKeep`, after `sema.lower` and before
  `macro_keep::kept_expansions`. Every `Statement::MacroExpansion` and wrapped
  value (`Module::macro_values`) is still present and a pass must not move
  anything into, out of, or across one. Passes here change what macro_keep
  surveys (temporaries are gone before its `local_used_after_macro` and
  `argument_reads_macro_local` checks).
- `Slot::AfterMacroStrip`, after `strip_macro_structure` and
  `pass_caller_locals`. Only kept expansions remain; the code of dropped ones
  is ordinary statements now.

A pass that needs callee summaries from other units would run at a
whole-target slot after `project::merge::without_definitions`; none does yet.

Invariants: a pass moves nodes and never clones or rebuilds them, so every
surviving node keeps its `NodeId` (side tables such as `macro_written_casts`
stay valid); a new node gets a fresh id; slate-parser's printed IR is
unchanged, since the passes run in `crates/slate`'s pipeline only. Fact
analyses read the transformed IR and never see a removed node.

`slate emit-slate-ir --passes=<a,b|all> <file.c>` prints the IR after the named
passes (no flag: none, which is sema's output after the macro strip). Fixtures
live in `crates/slate/tests/fixtures.transforms/<passes>/*.c`: the directory
names the passes, the C is the input, and the generated block checks every
function body with `CHECK-LABEL`. Test: `transform_filecheck` in the `slate`
profile; regenerate with `crates/slate/tools/update_filecheck.py`, which turns
binding and type numbers into FileCheck variables.

## Keys

| Entity                          | Key                                    | Why                                                                                |
| ------------------------------- | -------------------------------------- | ---------------------------------------------------------------------------------- |
| Parameter, local, global, temp  | `ProgramBinding { unit, binding }`     | `BindingId` is a dense counter per unit's resolver, so it repeats across units     |
| Value or statement node         | `NodeId`                               | allocated from one process-wide counter, unique across units and targets           |
| Place                           | the enclosing value or statement's `NodeId` plus its root binding | places carry no id; locals and params are keyed by their root binding  |
| Defined function                | `FunctionId`                           | dense index into `Program::functions`, for per-function tables in `Vec`s           |

`UnitFacts` is one unit's view of the store, so lowering looks facts up by the
`BindingId`s and `NodeId`s it already holds.

## Call resolution

`Program::callee(unit, &Callee)` returns the `FunctionId` whose body the call
runs, or `None` when the callee is opaque:

- A direct call to a function defined in the same unit resolves to it.
- A direct call to an external declaration resolves to the external
  definition with the same link name (`asm_name`, else the C name) in another
  unit.
- Weak, `alias`, `weakref`, `ifunc` and `inline_only` definitions are opaque:
  the linker or another unit may replace the body the analysis would see.
- Indirect calls are opaque. Address-taken functions are an analysis input,
  not a resolution.

`None` is always sound: the analysis treats the call as escaping its pointer
arguments.

A declaration's libc identity (`Known`, `FunctionIdentity`) is also a fact:
`analyze` classifies each function once from its declaration provenance (the
system header it came from, resolved through the unit's `Files`) and
`module.hosted`, and lowering reads `UnitFacts::identity` instead of
classifying. The catalog lives in `slate-analysis/src/identity.rs`; slate
re-exports it from `function_identity`, where `CallBinding` (a Rust-AST
concept) stays.

## Scaling rule

Analyses here must stay near-linear (`n` or `n log n` in IR size) on
`tools/benchmark/interproc_scaling.py`, including the adversarial shapes:

- Per-function summaries, computed once and in parallel.
- Propagation over the call graph or over alias classes (union-find), with
  finite-height lattices; no per-call-site re-analysis of a callee.
- One summary per function, not per calling context: a C function gets one
  Rust signature, so context sensitivity buys nothing for representation.
- Flow-sensitive facts (`&mut` exclusivity, nullness, lengths at a point) come
  from a per-function CFG that is built and dropped per function.

Precision is measured by the benchmark's `output #lifted` column and the
give-up fixtures `pointer_lift_local_copy_chain.c` and
`pointer_lift_reassigned_local.c`.

## Pointer capabilities

`pointer::analyze` (`slate-8ge7.6`) gives every pointer parameter of a
candidate function and every named pointer local a `PointerCapability`
(write, unique, free, offset, escape, nullable, string, raw, byte elements).
The capabilities are `pointer-capability-lattice`'s flags and stay internal to
the analysis. `BindingFacts::pointer` carries the representation lowering
emits: `borrow` (`&T`, `&mut T`, or a slice when `many`) and `length` (a
companion integer parameter that is dropped and rebuilt from `len()`).
Lowering always reads them; the Rust-side `pointer_lattice`, `length_lattice`
and `string_params` passes and the `SLATE_IR_POINTERS` gate were deleted in
`slate-8ge7.7`.

- Per-function summaries, built in parallel (rayon): one node per pointer
  parameter, local, temporary, call result and return value. Statement order
  is ignored: a C binding becomes one Rust binding, so its type must fit every
  use.
- Inside a function, copies unify nodes into classes (union-find). A class
  collects use flags from its members: write through `*p`, `p[i]` and
  `p->f = ..`; offset from `p[i]`, `p + n`, `p - q`; escape from stores into
  memory or globals, aggregates, pointer-to-integer conversions, `&p`, opaque
  calls, intrinsics, atomics, and anything inside OpenMP and nested
  functions; nullable from comparisons with null and null assignments.
- An `asm` statement is an opaque call over its operands, the bound gcc, clang
  and Rust's `asm!` all assume. A pointer value as an operand (`"r"(p)`,
  `"D"(dst)`) escapes, unless the template is empty (the barrier idiom
  `"" : : "r"(p) : "memory"` executes nothing, so it only reads `p`). A pointee
  lvalue operand (`"+m"(*p)`, `"=b"(*out)`, `"r"(*p)`) is a place use: input
  reads, output writes, in-out does both. An operand whose object is itself a
  tracked pointer escapes when read and becomes `raw` when written. The
  `"memory"` clobber, symbol operands and goto labels change no parameter that
  is not an operand. Lowering passes a borrowed param to a memory operand as
  `p as *mut T` (or `as_ptr()` for a slice).
- A call-site proof from `&x` or a decayed automatic array holds only if no
  other pointer to `x` can be live: `x`'s address must not leak anywhere in the
  caller (a copy into a pointer variable, a store, an escaping use, an asm
  operand, or another call whose parameter escapes or is returned). Leaks are
  flow-insensitive, so `int *q = &x;` anywhere in the caller pins every
  `f(&x)`. A static array is a length proof only when it is `const`; a mutable
  static can be reached by name during the call (`slate-8ge7.16`).
- `&x.a.b` and `&__real__ x` prove a borrow like `&x` when every field on the
  path is a non-bit-field member that the IR layout does not mark
  `underaligned` ([ir/types](ir/types.md#records)); the leak and alias checks
  use the root object `x`, so `f(&s.a, &s.b)` aliases. A `char` in a packed
  record lifts; an `int` under `packed` or `#pragma pack(2)` stays raw. A
  part proof also needs the parameter's class to have no `offset`: C lets a
  field pointer reach its enclosing object (`container_of`), which a Rust
  borrow of the field cannot. Union fields borrow inside `unsafe`.
- `raw` is per binding and flows forward along copies: a binding assigned a
  moved pointer (`p + 1`, `&p->f`), an opaque value (call to an unresolved
  function, a load from memory), or a non-candidate parameter cannot be a
  reference, but its source can still be.
- Across functions, values flow from arguments to parameters and from return
  values to call results. Use flags (write, offset, free, string, shared,
  escape) propagate against the flow, so a caller learns what its callees do
  through its pointer; nullable propagates with the flow. Each flag is one
  reachability pass, so the join is linear. Unifying across calls instead
  merged every caller of a helper such as `sqlite3Malloc` into one class.
- Candidates (functions whose signature may change): defined, not `main`, not
  variadic, address never taken, and not fixed. A function is fixed when
  `lowerer::exports_symbol` (passed to `Program::new`) exports it, except in a
  closed program (an executable project, `liftable_root`), where an export is
  fixed only if its symbol is pinned: defined twice, the target of an
  `alias`/`weakref`/`ifunc`, named in global asm, declared elsewhere with a
  different name, arity or pointer-ness, or carrying `section`, `used`,
  constructor/destructor, target features or a non-C calling convention.
  Arguments to any other callee escape.
- Cross-unit callers are ordinary call sites: a declaration resolved to a
  candidate gets the definition's parameter facts, so its extern declaration
  and its call sites lower with the same borrow and dropped length. Link must
  then bind those calls through a Rust path; an extern declaration left with a
  slice or `&CStr` parameter is a panic, because neither has a C ABI.
- Two arguments of one call that share a class or the same `&local` object
  alias; if either parameter is written, both escape.
- Libc calls use the declaration's identity: `free` frees, `mem*`/`str*cpy`
  write argument 0, `str*` reads mark string and offset, other known
  functions escape.
- `c_string` (`slate-8ge7.7`, replacing the Rust-side `string_params`): a
  candidate's byte-pointer parameter becomes `&core::ffi::CStr` when its class
  is string-marked, never written, freed, escaped, nullable, returned, raw or
  in a macro, and every call site (none pinned) passes a NUL-terminated
  literal with no interior NUL or an unwritten `c_string` parameter of the
  caller (a shrinking fixpoint). Reads lower to `s.as_ptr()`, so `strlen`,
  `p[i]` and `memcpy` stay inside the `CStr`'s provenance, terminator
  included; call sites pass `c"..."` or the caller's `&CStr`. Not `&str`: a
  Rust `"abc"` has no NUL, so `strlen(s.as_ptr())` would read past it.

Measured (`SLATE_INTERPROC_TIMING`, pass `ir_pointer`): N=800 on every
`interproc_scaling.py` shape at most 1 ms (Rust-side `pointer_lattice`
extract 2-4 ms); sqlite 10.9 ms for 17,957 nodes (Rust-side extract 144 ms,
validate 245 ms).

## Effect summaries

`effects::Effects` (`slate-8ge7.16`) answers "can this call reach object S
while it runs?" for the pointer analysis, and is the place to extend for later
questions of the same shape (`slate-8ge7.17`, purity for `slate-8ge7.9`).

- Per function, from the pointer summary walk: the mutable internal statics it
  names (`Summary.reads` and `written`), its direct callees, and `opaque`.
  `opaque` is set by an indirect call, a call to an undefined function without
  a libc identity, an undefined call passing a function pointer (a callback),
  asm with a non-empty template, and OpenMP or nested-function bodies.
- `opaque` closes over callers in one reverse pass. Who reaches S is a reverse
  search from S's direct namers, run once per static that appears as a proof,
  so cost is linear per queried static.
- Consumer: a mutable `static` array (internal linkage, not volatile or atomic)
  decayed at a call with its full extent proves a pointer+length pair when S's
  address never leaks, the callee is not opaque, and the callee does not reach
  S. S leaks when a global initializer names it, an escaping or unknown use
  takes its address, an escaping or returned callee parameter receives it, or
  a local copy of its address escapes, is returned or is passed to any call. A
  local copy that stays local (the `ptr_offset` temporary of `s[0] += 1`) does
  not leak. Another argument of the same call naming S or containing a call
  also blocks the proof, because Rust evaluates it while the slice is live.

Limits: a static whose address is stored anywhere (a `static int *tail =
&values[2]` initializer) never proves, even when the callee cannot load that
pointer; that needs points-to for the holders.

## Def-use

`def_use::DefUse::of(body)` (`slate-8ge7.5`) is one function's def-use, built
in one walk over the IR. It is the base for local facts that need "who reads or
writes this binding, and where", in place of each analysis keeping its own
walker. No SSA: one C variable keeps one Rust binding, and the `slate-8ge7.1`
benchmark showed no analysis that SSA would make asymptotically cheaper.

- Positions number statements in pre-order, so a statement's subtree is a
  contiguous range. Each use records its binding, its position, and its access:
  `Read`, `Write` (assignment target, initializer, update), `Escape` (address
  taken, place operand, asm operand, OpenMP clause, nested-function capture,
  `cleanup`), or `Capture` (a VLA extent named by a type). An update (`x += 1`,
  compare-exchange) is both a read and a write. `single_read(b)` is true when
  `b`'s only use is one `Read`.
- Each loop and switch records its body's range, so "inside this loop" is a
  range test. Labels, asm, SEH `__try` and OpenMP regions, and every `case` with
  its switch's position, are recorded to reject unstructured regions.
- Position order is execution order only within a range with no label, case,
  or jump in it. There is no function-wide order or CFG (`slate-8ge7.22`): an
  ordering query proves its range is straight-line (`structured`, or a sibling
  scan that rejects labels, cases and jumps), so gotos elsewhere in the
  function cannot enter or leave it.

### Counted loops

`counted_loops::analyze` is the first consumer. It produces
`UnitFacts::counted_loop(for_id) -> CountedLoop { index, start, end, inclusive }`
for a `for` whose index provably runs over `start..end`. The conditions are in
[ir/control-flow](ir/control-flow.md#counted-loops). It used to be a sema pass
that wrote `counted:` into the IR; the IR no longer carries it.

Lowering reads `start` and `end` from the fact, not from the `for`. They are
clones of IR values, so a macro body keeps its `macro_arg` holes; the
`SLATE_CHECK_MACRO_TEMPLATES` self-check strips them before lowering.

### Inline temporaries

The `inline-temps` transform (`transform/inline_temporaries.rs`,
`slate-8ge7.9.1`, a transform since `slate-8ge7.24.1`) deletes an IR
`Statement::Temporary` (printed `let %n: T [synthetic]`, lowered as `__v<n>`)
and moves its initializer, same `NodeId`, into its one read. It runs in both
slots: before macro_keep, so macro_keep surveys IR without those temporaries,
and after the strip, for temporaries inside expansions macro_keep dropped. A
temporary qualifies when all of these hold:

- the temporary has an initializer, is not
  `unsequenced`, has the initializer's type, has exactly one `Read`, and no
  record field type captures it;
- the read is in one of the next 32 sibling statements (or the list's tail
  value), in that sibling's own values (not a nested body), and not inside a
  macro argument, statement expression, or address-of;
- no sibling in between is control flow or opaque (labels, cases,
  break/continue/goto, asm, SEH, OpenMP, fences), and the producer's effect
  summary does not conflict with what runs between it and the read, or with
  what runs before the read inside the consuming statement. A call, intrinsic
  or compare-exchange that contains the read counts as running after it.
- an effectful producer is never moved under a `&&`/`||` rhs or a `?:` arm.

Each statement list is processed in order and each move happens before the
next temporary is decided, so a chain (`t1 = a; t2 = t1 + b; use(t2)`)
collapses in one pass and `t2`'s producer summary already includes `a`. The
lowerer sees no inlinable temporary, so the goto state machine needs no special
case. The lowering-side gates (`element_view`, `borrow_of`, `c_string_of`)
were dropped: across 3067 fixtures none ever blocked a candidate. Temporaries
the lowerer itself makes (`__t<n>`) are not covered (`slate-8ge7.24.2`); the
engine's `inline_temps::late` still folds those.

## Shadow mode

A port runs next to the Rust-side pass it replaces until every difference is
triaged as an improvement (with a fixture) or a bug (`slate-8ge7.4`). Only then
does lowering read the fact, and the Rust-side pass is deleted.

The harness (`SLATE_SHADOW`, `backend::shadow_diff`, `Program.shadow`,
`tools/shadow_report.py`) was deleted with `pointer_lattice`, its only user,
in `slate-8ge7.7`. It compared decisions per binding keyed by `(unit, Rust fn
name, binding name)` and printed one `slate-shadow` line per difference. The
next port restores it from the commit before that deletion.

## Related

- [pointer-capability-lattice](pointer-capability-lattice.md): the lattice the
  first port reimplements on the IR.
- [ir/pipeline](ir/pipeline.md#node-identity-and-metadata): how `NodeId`s are
  allocated and preserved.
- [entity-model](entity-model.md): `BindingId`s and redeclaration merging.
