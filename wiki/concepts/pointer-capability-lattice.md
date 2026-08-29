# Pointer capability lattice

Tracked by `slate-y0qs.4` (SCC-ordered call-graph worklist for interprocedural
fixpoint families). Implements the c2rust-derived pointer representation
lattice first proposed in `rewrite-worklist-engine.md`'s "Representation
decisions are unification, not local rewrites" section, extended beyond
c2rust per `wiki/log/2026-08-28-07-48.md`. Code: `src/backend/interproc/
pointer_lattice.rs` (analysis only, read-only, not yet wired into the
pipeline — see "Status" below).

## The tables

Key: `W`=write, `U`=unique, `F`=free, `O`=offset (indexed/advanced, either
direction), `E`=escape (can't prove containment — passed to opaque/external
code, address of the binding itself taken, or returned without a return-alias
proof), `N`=nullable, `S`=string (`T` is `u8`/`char` and nul-terminated /
string-shaped usage is observed).

Core (unlike c2rust, no `Box<[T]>` — owned+indexed always resolves to
`Vec<T>`; a `Vec<T>` that's never resized is exactly as correct as
`Box<[T]>` for differential testing, so the `GROW` fact this would otherwise
need is dropped from the lattice entirely).

This is exhaustive over all 16 `(W, U, F, O)` combinations, not a sparse
table with an implicit catch-all — `U` only gates ownership (paired with
`F`) and exclusivity (paired with `W`); it never blocks the plain
`&T`/`&[T]` rows. `W` never blocks the ownership rows either — writing
through something you own doesn't revoke ownership:

```
 W   U   F   O  │  Resulting type
────────────────┼──────────────────
 .   x   x   .  │  Box<T>
 x   x   x   .  │  Box<T>          (write-through doesn't revoke ownership)
 .   x   x   x  │  Vec<T>
 x   x   x   x  │  Vec<T>          (write-through doesn't revoke ownership)
 .   .   x   .  │  *const T        (freed but not proven unique — can't own)
 .   .   x   x  │  *const T
 x   .   x   .  │  *mut T
 x   .   x   x  │  *mut T
 x   .   .   .  │  &Cell<T>
 x   .   .   x  │  *mut T          (aliased + mutable + offset: no safe shape)
 x   x   .   .  │  &mut T
 x   x   .   x  │  &mut [T]
 .   .   .   .  │  &T
 .   x   .   .  │  &T              (U alone doesn't change this)
 .   .   .   x  │  &[T]
 .   x   .   x  │  &[T]            (U alone doesn't change this)
```

An earlier version of this table only listed 7 rows and used an implicit
catch-all for the rest, which — combined with `U` defaulting to `true`
until disproven — meant the *common* case (a plain pointer with no
aliasing evidence yet) missed the `&T`/`&[T]` rows, and any owned buffer
that was ever written (i.e. almost all of them) missed `Box<T>`/`Vec<T>`.
See `wiki/log/2026-08-28-12-38.md` for the session that found this.

String specialization (modifier over the buffer rows, `T` in `{u8, char}`,
gated on `S`):

```
 Base type (S)        │ Specializes to
────────────────────────┼────────────────
 &[T]      shared        │ &str
 Vec<T>    owned          │ String
```

`&mut [T] → &mut str` is deliberately **not** in this table — `S` alone
doesn't prove every write preserves UTF-8 validity (an arbitrary byte write
breaks it even on an otherwise string-shaped buffer).

`S` alone is likewise insufficient for the `Vec<T> → String` row: the
consumer (`length_lattice::utf8_owned_fill_proof`, `slate-y0qs.4.4`) only
lifts `StringOwned` when it can prove the buffer's content is valid UTF-8 at
the malloc call site (single `memcpy` from a UTF-8 literal over `[0,len)`, one
ownership-transfer call, no other writes); otherwise it falls back to a raw
pointer. `specialize_string` still keeps its `!write` guard so a callee that
mutates the buffer stays `Vec`, since callee writes aren't yet proven
char-safe — replacing that guard with a per-write validity fact
(`toupper`/`tolower`/literal sources) is the remaining part of `4.4`.

Evaluation order: `E` first (short-circuits to `*mut T`/`*const T` by `W`,
skipping everything else) → core table → `S` (buffer rows only) → `N` wraps
the result in `Option<...>`.

## Why not SSA

`architecture.md`'s rationale for CIR over LLVM IR is precisely that SSA+phi
nodes replace source variables, and slate wants named locals preserved
end to end. Building real SSA for this one analysis would fight that,
and it doesn't solve the problem this analysis actually has: SSA gives a
clean def/use-list for *one variable's own reassignments*, which the arena
already provides for free (`Arena::def_use_neighbors`, `src/backend/engine/
arena.rs:383`). The hard part of `UNIQUE` is *cross-binding* aliasing, which
SSA renaming doesn't touch in either representation.

## Monotonicity

`W`/`O`/`F`/`E`/`N`/`S` are existential — once witnessed true, permanently
true (only flip false→true). `U` is universal ("no alias exists yet") —
starts true, only flips true→false once disproven. Every bit flips at most
once, so total work is bounded by `7 × |bindings|` regardless of recursion
depth or call-graph shape — same termination argument `interproc::run_worklist`
already relies on for `string_params` (`WORKLIST_EDIT_BUDGET`).

## Two-phase design: interproc solve, then local consumption

Splitting these matters because they run at different stages of the
pipeline (`rewrite-engine-v2.md`'s whole-Program interproc phase runs
*before* the per-function arena is even built — see `engine::run_function`,
`src/backend/engine/mod.rs:99-104`):

1. **Interproc solve** (`slate-y0qs.4.1`, implemented) operates on the
   whole-`Program` AST, the same stage `string_params.rs` runs at. Output
   is a flat `BTreeMap<PointerBinding, PointerFact>` — no per-use-site
   `NodeId`s, because arena `NodeId`s don't exist yet at this stage (each
   function gets its own `Arena` built later, one per `engine::run_function`
   call). The fact only needs to answer "what type does this binding
   resolve to," not "where are its uses" — that's the next phase's job.

2. **Local consumption** (`slate-y0qs.4.2`, implemented) turned out not to
   run inside the per-function arena worklist as originally planned here.
   Instead it's a whole-`Program` AST rewrite (`pointer_lattice::apply()`),
   run at the same interproc stage as `solve()` itself, right before the
   arena/`NodeRule` pipeline starts (`engine::apply()`,
   `src/backend/engine/mod.rs:67-68`). It looks up each candidate param's
   `PointerBinding` in the solved table, retypes the declared param, and
   walks the function body converting uses (derefs stay bare for `&T`/
   `&mut T`/`Box<T>` since Rust's own `Deref`/`DerefMut` cover them; a
   `.as_ptr()`/`.as_mut_ptr()`-shaped raw-pointer fallback is synthesized
   at call sites that still need a raw pointer). There's no separate
   accessor-*cleanup* rule as originally envisioned — the decision of
   "does this call site need a raw pointer or can it take the lifted type
   directly" (`callee_accepts_directly`) is made once, at rewrite time,
   instead of inserted-then-deleted in two passes.

`lift_kind()` currently produces two of the four `LiftKind`s:

- `Scalar` (`&T`/`&mut T`, the `Ref`/`RefMut` rows) — pure accessor
  insertion, no allocation semantics.
- `Owned` (`Box<T>`, the `Owned` row) — the harder case. Bridges a raw
  pointer into `Box<T>` at call sites via `unsafe { Box::from_raw(...) }`
  (gated to bare-`Var` args only in `arg_is_provably_bridgeable`, not
  arbitrary exprs, to keep the shape predictable), and — critically —
  deletes the C `free()` call that justified the `FREE` bit in the first
  place (`is_owned_free_stmt`/`collect_owned_aliases`), relying on `Box`'s
  own `Drop` instead. Skipping that deletion would double-free: the
  lifted `Box<T>` frees on scope exit *and* the untouched `free()` call
  would free the same allocation again. `collect_owned_aliases` exists
  because CIR routinely hoists parameter derefs through an intermediate
  local (`let mut y: *mut i32 = arg0; ...; free(y);`) rather than
  dereferencing the param directly — the `free()` call's argument has to
  be traced back through that alias to the lifted binding, not just
  matched by name.

`Cell` and `Buffer` are deliberately **not** produced by `lift_kind()`:

- `Cell` (`&Cell<T>`, the `RefCell` row): implemented in `to_raw_pointer_as`
  and the rewrite side (`.get()`/`.set()` synthesis), but never wired up.
  Session decision: `RefCell` only saves you the `unsafe` keyword at the
  deref site — every raw-pointer-needing use still falls back to
  `.as_ptr()`, which is exactly as unsafe-shaped as the raw pointer it
  replaces. The `UNIQUE`-disproving signal that gates this row
  (`ClassifyCtx::try_alias`'s use-count heuristic) is also unreliable in
  practice — hand constructing a case that disproves `unique` without
  disproving it in ways that reflect real aliasing took real effort in
  this session and still didn't reliably fire. Not worth the added
  decl-site/call-site/free-alias machinery for a row whose only payoff is
  cosmetic. Revisit only if a real corpus shows this row firing often and
  usefully.
- `Buffer` (`&[T]`/`&mut [T]`/`&str`/`Vec<T>`/`String`, the `Slice`/
  `SliceMut`/`Str`/`Vec`/`StringOwned` rows): needs call-site
  slice-*length* bridging (associating a length expression/parameter with
  the pointer at every call site and use site) that doesn't exist yet —
  a structurally different, larger piece of work than accessor insertion.
  Tracked as `slate-y0qs.4.8`.

Requeueing/accessor-cleanup machinery from the original two-`NodeRule` plan
turned out unnecessary given the whole-Program-rewrite shape that actually
shipped — there's nothing to requeue since there's no arena at this stage.

## Interprocedural propagation

Nodes = functions (candidates: private, non-variadic, non-`unsafe`, no
foreign `abi`, with at least one raw-pointer parameter — same eligibility
gate `string_params.rs`'s `candidate_for` uses, generalized past `char*`).
Edges = call sites. `CallGraph`/`scc_order`/`run_worklist`
(`src/backend/interproc/mod.rs`) are reused unchanged.

Unlike `string_params` (whose eligibility only flows caller→callee),
capability evidence flows **both ways** through a call site:

- **Backward**: if a callee writes/frees/offsets/fails-to-contain its
  parameter, that taints the *caller's argument binding* too — passing `p`
  into a function that calls `free` on it makes `p` itself `FREE`.
- **Forward**: the caller argument's own facts (e.g. already known
  `nullable`) inform what the callee can assume about its parameter.

Both directions are the same bit-wise join/meet (`PointerFact::
merge_evidence_from`), applied per call site inside `recompute(name)` for
whichever endpoint (`name`) the worklist is currently visiting — each call
site's merge is symmetric and idempotent, so it's safe for both the caller's
and the callee's `recompute` to independently re-apply it. Local
(intraprocedural) evidence is computed exactly once, up front
(`classify_function`), since — with return-value aliasing out of scope for
now (`slate-y0qs.4.3`) — it never depends on any other function's facts;
only the cross-call joins iterate.

## Status

`solve()` and `apply()` are both implemented and wired into
`engine::apply()` (`slate-y0qs.4.2`). `apply()` lifts the `Scalar` and
`Owned` rows only (see above for why `Cell`/`Buffer` are excluded from
`lift_kind()`). Verified via three differential fixtures:
`tests/fixtures/pointer_lattice.c` (all three lifted shapes — `&mut T`,
`&T`, `Box<T>` — in one program) plus the two `ptr_lattice_*` regression
fixtures from earlier bug-fix sessions. Known gaps, each tracked as its own
follow-up rather than half-implemented here: return-value aliasing
(`slate-y0qs.4.3`), `&mut str` (`slate-y0qs.4.4`), broader
`Known`-libc-function semantics (`slate-y0qs.4.5`) — `classify_call_arg`
currently only models `free`/`memcpy`/`memmove`/`memset`/the `str*` family
and defaults every other call conservatively to `ESCAPE` — and the
`Buffer`-kind rows (`slate-y0qs.4.8`).

One latent soundness gap surfaced while implementing `Owned`, not yet
filed as its own bead: the interprocedural `FREE` taint can over-approximate
across a *shared* helper. If function `h(int *q)` is called from two
different callers, and only one of them later frees the pointer it passed
in, `solve()`'s bidirectional merge taints `h`'s own parameter fact with
`FREE` regardless of which call site produced it — there's no per-call-site
attribution once the bit is merged. In this session's testing this was
caught, not exploited: `validate_plans`'s bare-`Var`-only bridging
requirement for `Owned` rejected the one call site (a non-`Var` address-of
argument) that would have produced an actual unsound `Box::from_raw` on a
stack address. That's incidental protection from an unrelated
conservatism, not a designed guarantee — a shared helper called only via
bare-`Var` args from both an owning and a non-owning context would not be
caught the same way.

## Related

- [rewrite-worklist-engine.md](rewrite-worklist-engine.md) — original lattice
  proposal and the interproc-vs-local-worklist split.
- [rewrite-engine-v2.md](rewrite-engine-v2.md) — the arena/worklist engine
  this plugs into; "Known risks" section flags interprocedural rules as a
  distinct phase.
- [facts.md](facts.md) — the fact-collector conventions this follows
  (`heap_ownership` for `FREE`, `borrow_alias` for `UNIQUE`,
  `array_element_pointer_origin`/`slice_index` for `OFFSET`).
- `wiki/log/2026-08-28-07-48.md` — the session that extended the lattice
  past c2rust's original four facts (dropped `Box<[T]>`, added `&str`/
  `String`/`Option<...>`).
