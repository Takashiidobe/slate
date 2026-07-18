# Facts

`src/fixups/facts/` is the analysis layer between baseline lowering and the
fixup passes described in [passes.md](passes.md). It never mutates the AST: it
reads an already-lowered `Program` and produces `FixupFacts`, a flat bag of
records — purity, definition/use, provenance, loop shape, string/heap/file
ownership, and more — that rewrite passes query instead of re-deriving the
same information by re-walking the tree.

This doc is a reference for what each collector proves and which rewrite pass
(named as in passes.md's [pass sequence](passes.md#the-pass-sequence)) reads
it. Read [writing-a-fixup.md](writing-a-fixup.md) first for the rule this
layer exists to enforce: *"A rewrite should consume `FixupFacts` plus local
AST shape; if it needs information that is not already in `FixupFacts`, add a
fact collector first."*

## Why a separate layer

Fixups must stay conservative (see
[writing-a-fixup.md#safety-stay-conservative](writing-a-fixup.md#safety-stay-conservative)):
before moving, dropping, or folding code they need to know things the AST
shape alone doesn't answer — "is this expression pure," "is this the last
read of this binding," "does this pointer alias anything else," "did this
buffer ever get indexed." Answering those per-pass, by hand, is exactly the
kind of pass-local walker `writing-a-fixup.md` warns against. `FixupFacts`
computes each answer once, in one dedicated module, so every pass gets the
same answer through the same query methods on `FixupFacts` (`def_use`,
`effect`, `place`, `has_value`, `string_buffer`, ...; see the full list in
`src/fixups/facts/mod.rs`).

Facts are a **snapshot**, not a live index: `facts::analyze(program)` walks
the whole `Program` once and returns `AnalyzedProgram { program, facts }`.
Because a rewrite can invalidate facts a later pass needs (a fold can turn an
effectful expression pure, a dead binding can vanish), `src/fixups::apply`
re-runs `facts::analyze` after any pass whose output the next pass's facts
must reflect — see the call sites of `facts::analyze` in
`src/fixups/mod.rs`. There is no incremental update; the tradeoff is
re-walking the tree rather than tracking invalidation.

## Addressing scheme

Facts don't hold references into the tree — a `Program` gets rebuilt by
value between rewrites, so borrows wouldn't survive. Instead every fact keys
off small, `Copy`, structural identifiers:

- **`FunctionId` / `BindingId` / `LoopId` / `SignatureId`** — dense indices
  assigned once, in traversal order, by the initial `Collector::program` pass
  in `src/fixups/facts/mod.rs` (functions and their parameter/local bindings,
  loop headers, call signatures). Later collectors look bindings up by
  `(function, name)` or `(function, declaration path)` rather than re-minting
  IDs.
- **`AstPath` / `PathSegment`** — a route from a function body down to a
  specific statement or expression (`Stmt(2)`, `Then`, `LoopBody`,
  `MatchArm(0)`, `Expr(1)`, ...). A `Site { function, path }` is "this fact is
  about the node at this path in this function." Because rewrites preserve
  statement order except where they explicitly restructure it, a path
  computed by one collector still resolves correctly for the next, within the
  same `analyze()` snapshot.
- **`walk::paths_overlap`** — reads are sometimes recorded at whole-statement
  granularity (a call argument's read is attributed to the enclosing
  statement) while a query needs a narrower path (the specific argument
  expression); path containment in either direction is treated as a match.

`src/fixups/facts/walk.rs` is the shared *immutable* traversal helper
(`body_exprs`, `stmt_exprs`, `with_path_segment`, `nested_bodies_with_path`,
...) that every collector below is built on — the collector-side counterpart
to `src/fixups/support/walk.rs`, which rewrite passes use instead.

## Collectors

Listed in the order `facts::analyze` runs them (`src/fixups/facts/mod.rs`).
"Consumed by" names rewrite passes from [passes.md](passes.md); a pass can
also be a producer for a later collector, noted where relevant.

| # | Module | Fact struct(s) | Proves | Consumed by |
|---|--------|-----------------|--------|-------------|
| 1 | `anonymous_structs` | `AnonymousStructFact` | Which synthesized `anon_*` records are repeated anonymous-struct shapes, and the hoisted name/field list to replace them with | `anonymous_structs` |
| 2 | `borrow_alias` | `BorrowAliasFact`, `BorrowAliasUseFact` | Per binding: whether every use is read-only, uniquely-mutated, or the value/address has escaped (borrowed, raw-ptr-derived, passed to an unknown call) | `remove_mut` (via `FixupFacts::binding_requires_mut`) |
| 3 | `def_use` | `DefUseFact` | Per binding: its definition path, every read/write path, and its last use | `param_spills`, `zero_init`, `drop_call_results`, `nullable_pointer`, `retval`, `call_args`, `atomic_compare_exchange`, `remove_mut`, `printf_format` |
| 4 | `effects` | `EffectFact`, `Purity`, `EffectKind` | Whether an expression/statement is pure enough to move or duplicate, and what side effect it has if not (volatile access, atomic access, unknown call, ...) | `inline_temps` (early/late), `call_args`, `dead_locals`, `drop_call_results` |
| 5 | `control_flow` | `ControlFlowFact` | Per statement/block: reachability, whether it falls through, its exit set (`return`/`break`/`continue`), and whether it has one clean exit an expression form could replace | `retval` |
| 6 | `casts` | `CastFact`, `CastKind`, `CastRequirement` | Per cast: whether it's structurally required (ABI, sign/width change, inference) or a candidate to drop as redundant in its typed context | `unnecessary_casts` |
| 7 | `places` | `PlaceFact`, `PlaceKind`, `PlaceAccess` | Classifies an lvalue expression as a plain local, a projection (field/index/deref), or a volatile/atomic access, plus whether it's readable/assignable | `param_spills` |
| 8 | `retval` | `RetvalCollapseFact` | Where a `let __retval = ...; ...; return __retval;` chain can collapse into a direct `return <value>` | `retval` (its own producer/consumer pair — the pass finds the shape via facts, then removes the intermediate statements) |
| 9 | `temp_chains` | `TempChainFact` | Producer→consumer chains of single-use temp bindings, with their transitive dependencies | `inline_temps` (early/late) |
| 10 | `values` | `ValueFact`, `ConstValue` | Constant values an expression or binding is statically known to hold (integers, `usize`, bytes, C-string bytes, "provably zero") | `zero_init` (`has_value`), `string_libc`, `memchr_prelude` |
| 11 | `calls` | `CallSignatureFact`, `CallsiteFact`, `CallArgFact`, `LibcCallSemantic` | Every function/extern signature in the program, every call site's resolved callee and per-argument pinning (declared-param vs. variadic vs. unknown-callee), and recognized libc call semantics (`Printf`, `StrCpy`, `MemCpy`, ...) | `call_args`, `remove_mut` (`CallArgPinning`), `prune_unused_externs`; also the input several later collectors build on (`ptr_len`, `string_params`) |
| 12 | `strings` (`collect_facts`) | `StringBufferFact`, `StringPointerViewFact`, `StringLibcUseFact`, `AsciiNumericStringFact` | Per buffer: its kind (char array / borrowed / owned), provenance, NUL-termination, ASCII-ness, and which of `&str`/`&CStr`/`&[u8]`/`String` it's a safe candidate for; plus pointer-view and libc-call-argument sites over those buffers | `string_libc`, `memchr_prelude` |
| 13 | `string_params` | `StringParamLiftFact` | Which function parameters can become `&str` because every call site (direct, by name, argument proven liftable) supports it — computed to a fixpoint since lifting one parameter can make a caller's argument liftable in turn | `string_params` |
| 14 | `heap_ownership` | `HeapOwnershipFact`, `HeapUseFact`, `HeapReallocFact` | `malloc`/`calloc`/`realloc`/`free` sequences on a pointer: allocation kind, element type/extent, whether every read is safely preceded by a write, and every reallocation's resize direction | `heap_ownership` |
| 15 | `printf` | `PrintfCallFact`, `PrintfArgFact` | `printf`-family call sites with a constant format string: parsed format plus, per argument, whether it's a constant string/char, an already-lifted Rust string, or a pointer | `printf_format` |
| 16 | `strings` (`collect_rewrite_facts`) | `StringLiftPlanFact`, `StringCopyRewriteFact` | The specific recovery to apply to a buffer (which `StringRecoveryCandidate`) and, for `strcpy`/`strcat`-only buffers, the literal/owned assign-or-push rewrite | `string_lift`, `string_copy` |
| 17 | `c_strings` | `CStringLiteralFact` | Byte payload of a C-string literal at a given receiver, for later marking/simplification | `c_strings` |
| 18 | `file_ownership` | `FileOwnershipFact`, `FileUseFact` | `fopen`/`fclose`-bracketed handle sequences: open mode, and every read/write/gets/puts/close use of the handle | `stdio` |
| 19 | `ptr_len` | `PtrLenSliceFact` | Which `(pointer, length)` parameter pairs on a callee are safe to pair into one slice parameter — proven from every call site's `CallsiteFact`, to a fixpoint | `ptr_len` |
| 20 | `array_element_pointer_origin` | `ArrayElementPointerOriginFact` | A pointer binding that is provably `&array[index]` (or the mutable equivalent), so later code can index the array directly instead of dereferencing the pointer | `array_element_pointer_origin` |
| 21 | `atomic_locals` (also globals) | `AtomicLocalFact`, `AtomicGlobalFact` | An integer local/global whose every mention is the pointer operand of an atomic op of one width — safe to give native `AtomicN` storage | `atomic_locals` |
| 22 | `lazy_singleton` | `LazyInitSingletonFact` | A function whose entire body is the "static flag guards a static payload" idiom, with the guard checked program-wide to be write-once/read-once | `lazy_singleton` |
| 23 | `buffer_cursor` | `BufferPointerFieldFact` | A pointer-typed struct field whose value always derives from `&array[index]` for a specific sibling array field — the shape `buffer_cursor` turns into cursor-struct field ops | `buffer_cursor` |
| 24 | `slice_index` | `SlicePointerViewFact`, `SliceIndexRangeFact`, `SlicePointerIndexFact` | Pointer views over an already-sliced parameter, an index's proven `0..slice.len()` range, and pointer-vs-ranged-index correspondences (element vs. byte offset) | `slice_index` |
| 25 | `counted_loop` | `CountedLoopFact`, `CountedSliceLoopFact` | A loop's induction variable start/bound/step (`0..bound`, step 1) and, for slice-bound loops, how the index is used (unused / index-only / index-and-value) | `range_loop` (`CountedLoopFact`), `slice_loop` (`CountedSliceLoopFact`) |
| 26 | `loop_shapes` | `LoopShapeFact`, `LoopShapeRejectionFact` | A loop's high-level shape (counted / reduction / search / copy / fill / sentinel-write) with its induction, accumulator, and collection bindings, plus *why* a shape match was rejected | none yet — collected but not read by any rewrite pass; `slice_reduce` and the loop-shape passes currently pattern-match the AST directly instead of querying this fact |
| 27 | `va_list` | `VaListAliasFact` | A local that's just a clone/alias of the function's sole `va_list` parameter, safe to fold away | `va_list` |

`goto` (`src/fixups/facts/goto.rs`) is a related but separate case: it is a
front-end-agnostic CFG library (`CfgNode`/`CfgEdge`, dominators, natural
loops, SCCs) used directly by the `goto`-structuring pass
(`src/fixups/rewrite/goto.rs`) to rebuild structured control flow from a
label/jump dispatch loop. It is not a `FixupFacts` field and is not populated
by `facts::analyze` — the `goto` pass runs first, before facts exist at all
(see `structure_goto` in `src/fixups/mod.rs`), and builds its own CFG
straight from the AST each time it runs.

## Adding a fact

Add a new module under `src/fixups/facts/`, following the shape every
existing collector uses:

1. Define the fact struct(s) in `src/fixups/facts/mod.rs` and add a field to
   `FixupFacts` for them (plus a lookup method next to the existing ones —
   `string_buffer`, `def_use`, `place`, ...).
2. Write `pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts)`
   in the new module, walking with `src/fixups/facts/walk.rs` helpers (or
   reusing another collector's already-built maps, the way `ptr_len` consumes
   `CallsiteFact` from `calls`).
3. Register the call in `facts::analyze` (`src/fixups/facts/mod.rs`), after
   any collector it depends on.
4. Consume it from a rewrite pass through `&FixupFacts`, never by re-walking
   the tree by hand — see
   [writing-a-fixup.md](writing-a-fixup.md#reuse-the-shared-helpers).

Keep collectors read-only and side-effect-free: `collect_facts` must not
mutate `program`. If a pass needs the AST changed, that belongs in
`src/fixups/rewrite/`, driven by the facts this layer already computed.
