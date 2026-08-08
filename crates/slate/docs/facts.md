# Facts

`src/fixups/facts/` is the analysis layer between baseline lowering and the
fixup passes described in [passes.md](passes.md). It never mutates the AST: it
reads an already-lowered `Program` and produces facts — purity,
definition/use, provenance, loop shape, string/heap/file ownership, and more —
that rewrite passes query instead of re-deriving the same information by
re-walking the tree. Facts are computed and memoized by
[salsa](https://github.com/salsa-rs/salsa): `src/fixups/salsa.rs`'s
`SalsaFacts` owns a `#[salsa::db]` `Database` and a singleton `ProgramInput`.
`FunctionInput` is an interned `(ProgramInput, FunctionId)` key whose body and
base-walk bindings, binding types, and loops are derived by tracked methods.
Whole-program function and definition reductions are tracked methods on
`ProgramInput`; no externally synchronized fact inputs are retained. Salsa
reruns only the tracked functions whose inputs changed and backdates (skips
invalidating dependents) when a rerun's output is value-equal to before.

This doc is a reference for what each collector proves and which rewrite pass
(named as in passes.md's [pass sequence](passes.md#the-pass-sequence)) reads
it. _"A rewrite should consume facts plus local AST shape; if it needs
information that is not already available as a fact, add a fact collector
first."_

## Why a separate layer

Fixups must stay conservative
before moving, dropping, or folding code they need to know things the AST
shape alone doesn't answer - "is this expression pure," "is this the last
read of this binding," "does this pointer alias anything else," "did this
buffer ever get indexed." Answering those per-pass, by hand, is exactly the
kind of pass-local walker `writing-a-fixup.md` warns against. Each fact is
computed once, in one dedicated module, so every pass gets the same answer
through the same query methods on `QueryContext` (`def_use`, `effect`,
`place`, `has_value`, `string_buffer`, ...; see the full list in
`src/fixups/query/context.rs`), which is itself a thin, `#[salsa::tracked]`
memoization-backed adapter over `SalsaFacts`.

## Addressing scheme

Facts don't hold references into the tree — a `Program` gets rebuilt by
value between rewrites, so borrows wouldn't survive. Instead every fact keys
off small, `Copy`, structural identifiers:

- **`FunctionId` / `BindingId` / `LoopId` / `SignatureId`** — dense indices
  assigned once, in traversal order, by the base program walk
  (`facts::walk::BaseWalk` in `src/fixups/facts/walk.rs`; functions and their
  parameter/local bindings, loop headers, call signatures). `BaseWalk` also
  drives `SalsaFacts`'s incremental re-sync: an edited function gets its ids
  re-derived in place (stable for every other function), so `FunctionId`
  stays a valid `#[salsa::input]` key across `Program` revisions. Later
  collectors look bindings up by `(function, name)` or `(function,
  declaration path)` rather than re-minting IDs.
- **`AstPath` / `PathSegment`** — a route from a function body down to a
  specific statement or expression (`Stmt(2)`, `Then`, `LoopBody`,
  `MatchArm(0)`, `Expr(1)`, ...). A `Site { function, path }` is "this fact is
  about the node at this path in this function." Because rewrites preserve
  statement order except where they explicitly restructure it, a path
  computed by one collector still resolves correctly for the next, within the
  same `Program` revision.
- **`walk::paths_overlap`** — reads are sometimes recorded at whole-statement
  granularity (a call argument's read is attributed to the enclosing
  statement) while a query needs a narrower path (the specific argument
  expression); path containment in either direction is treated as a match.

`src/fixups/facts/walk.rs` is the shared _immutable_ traversal helper
(`body_exprs`, `stmt_exprs`, `with_path_segment`, `nested_bodies_with_path`,
...) that every collector below is built on — the collector-side counterpart
to `src/fixups/support/walk.rs`, which rewrite passes use instead.

## Collectors

Listed in dependency order (a collector that reads another's output is listed
after it); unlike the old whole-program `facts::analyze` sweep, salsa has no
fixed global run order — each `#[salsa::tracked]` fn resolves its own
dependencies lazily, on first read, and only reruns the ones an edit actually
invalidated. "Consumed by" names rewrite passes from [passes.md](passes.md); a
pass can also be a producer for a later collector, noted where relevant.

| #   | Module                              | Fact struct(s)                                                                             | Proves                                                                                                                                                                                                                                   | Consumed by                                                                                                                                                            |
| --- | ----------------------------------- | ------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 1   | `anonymous_structs`                 | `AnonymousStructFact`                                                                      | Which synthesized `anon_*` records are repeated anonymous-struct shapes, and the hoisted name/field list to replace them with                                                                                                            | `anonymous_structs`                                                                                                                                                    |
| 2   | `borrow_alias`                      | `BorrowAliasFact`, `BorrowAliasUseFact`                                                    | Per binding: whether every use is read-only, uniquely-mutated, or the value/address has escaped (borrowed, raw-ptr-derived, passed to an unknown call)                                                                                   | `remove_mut` (via `SalsaFacts::binding_requires_mut`)                                                                                                                  |
| 3   | `def_use`                           | `DefUseFact`                                                                               | Per binding: its definition path, every read/write path, and its last use                                                                                                                                                                | `param_spills`, `zero_init`, `drop_call_results`, `nullable_pointer`, `retval`, `call_args`, `atomic_compare_exchange`, `remove_mut`, `printf_format`                  |
| 4   | `effects`                           | `EffectFact`, `Purity`, `EffectKind`                                                       | Whether an expression/statement is pure enough to move or duplicate, and what side effect it has if not (volatile access, atomic access, unknown call, ...)                                                                              | `inline_temps` (early/late), `call_args`, `dead_locals`, `drop_call_results`                                                                                           |
| 5   | `control_flow`                      | `ControlFlowFact`                                                                          | Per statement/block: reachability, whether it falls through, its exit set (`return`/`break`/`continue`), and whether it has one clean exit an expression form could replace                                                              | `retval`                                                                                                                                                               |
| 6   | `casts`                             | `CastFact`, `CastKind`, `CastRequirement`                                                  | Per cast: whether it's structurally required (ABI, sign/width change, inference) or a candidate to drop as redundant in its typed context                                                                                                | `unnecessary_casts`                                                                                                                                                    |
| 7   | `places`                            | `PlaceFact`, `PlaceKind`, `PlaceAccess`                                                    | Classifies an lvalue expression as a plain local, a projection (field/index/deref), or a volatile/atomic access, plus whether it's readable/assignable                                                                                   | `param_spills`                                                                                                                                                         |
| 8   | `retval`                            | `RetvalCollapseFact`                                                                       | Where a `let __retval = ...; ...; return __retval;` chain can collapse into a direct `return <value>`                                                                                                                                    | `retval` (its own producer/consumer pair — the pass finds the shape via facts, then removes the intermediate statements)                                               |
| 9   | `values`                            | `ValueFact`, `ConstValue`                                                                  | Constant values an expression or binding is statically known to hold (integers, `usize`, bytes, C-string bytes, "provably zero")                                                                                                         | `zero_init` (`has_value`), `string_libc`, `memchr_prelude`                                                                                                             |
| 10  | `calls`                             | `CallSignatureFact`, `CallsiteFact`, `CallArgFact`, `LibcCallSemantic`                     | Every function/extern signature in the program, every call site's resolved callee and per-argument pinning (declared-param vs. variadic vs. unknown-callee), and recognized libc call semantics (`Printf`, `StrCpy`, `MemCpy`, ...)      | `call_args`, `remove_mut` (`CallArgPinning`); also the input several later collectors build on (`ptr_len`, `string_params`)                                            |
| 11  | `strings` (`collect_facts`)         | `StringBufferFact`, `StringPointerViewFact`, `StringLibcUseFact`, `AsciiNumericStringFact` | Per buffer: its kind (char array / borrowed / owned), provenance, NUL-termination, ASCII-ness, and which of `&str`/`&CStr`/`&[u8]`/`String` it's a safe candidate for; plus pointer-view and libc-call-argument sites over those buffers | `string_libc`, `memchr_prelude`                                                                                                                                        |
| 12  | `string_params`                     | `StringParamLiftFact`                                                                      | Which function parameters can become `&str` because every call site (direct, by name, argument proven liftable) supports it — computed to a fixpoint since lifting one parameter can make a caller's argument liftable in turn           | `string_params`                                                                                                                                                        |
| 13  | `heap_ownership`                    | `HeapOwnershipFact`, `HeapUseFact`, `HeapReallocFact`                                      | `malloc`/`calloc`/`realloc`/`free` sequences on a pointer: allocation kind, element type/extent, whether every read is safely preceded by a write, and every reallocation's resize direction                                             | `heap_ownership`                                                                                                                                                       |
| 14  | `printf`                            | `PrintfCallFact`, `PrintfArgFact`                                                          | `printf`-family call sites with a constant format string: parsed format plus, per argument, whether it's a constant string/char, an already-lifted Rust string, or a pointer                                                             | `printf_format`                                                                                                                                                        |
| 15  | `strings` (`collect_rewrite_facts`) | `StringLiftPlanFact`, `StringCopyRewriteFact`                                              | The specific recovery to apply to a buffer (which `StringRecoveryCandidate`) and, for `strcpy`/`strcat`-only buffers, the literal/owned assign-or-push rewrite                                                                           | `string_lift`, `string_copy`                                                                                                                                           |
| 16  | `c_strings`                         | `CStringLiteralFact`                                                                       | Byte payload of a C-string literal at a given receiver, for later marking/simplification                                                                                                                                                 | `c_strings`                                                                                                                                                            |
| 17  | `file_ownership`                    | `FileOwnershipFact`, `FileUseFact`                                                         | `fopen`/`fclose`-bracketed handle sequences: open mode, and every read/write/gets/puts/close use of the handle                                                                                                                           | `stdio`                                                                                                                                                                |
| 18  | `ptr_len`                           | `PtrLenSliceFact`                                                                          | Which `(pointer, length)` parameter pairs on a callee are safe to pair into one slice parameter — proven from every call site's `CallsiteFact`, to a fixpoint                                                                            | `ptr_len`                                                                                                                                                              |
| 19  | `array_element_pointer_origin`      | `ArrayElementPointerOriginFact`                                                            | A pointer binding that is provably `&array[index]` (or the mutable equivalent), so later code can index the array directly instead of dereferencing the pointer                                                                          | `array_element_pointer_origin`                                                                                                                                         |
| 20  | `atomic_locals` (also globals)      | `AtomicLocalFact`, `AtomicGlobalFact`                                                      | An integer local/global whose every mention is the pointer operand of an atomic op of one width — safe to give native `AtomicN` storage                                                                                                  | `atomic_locals`                                                                                                                                                        |
| 21  | `lazy_singleton`                    | `LazyInitSingletonFact`                                                                    | A function whose entire body is the "static flag guards a static payload" idiom, with the guard checked program-wide to be write-once/read-once                                                                                          | `lazy_singleton`                                                                                                                                                       |
| 22  | `buffer_cursor`                     | `BufferPointerFieldFact`                                                                   | A pointer-typed struct field whose value always derives from `&array[index]` for a specific sibling array field — the shape `buffer_cursor` turns into cursor-struct field ops                                                           | `buffer_cursor`                                                                                                                                                        |
| 23  | `slice_index`                       | `SlicePointerViewFact`, `SliceIndexRangeFact`, `SlicePointerIndexFact`                     | Pointer views over an already-sliced parameter, an index's proven `0..slice.len()` range, and pointer-vs-ranged-index correspondences (element vs. byte offset)                                                                          | `slice_index`                                                                                                                                                          |
| 24  | `counted_loop`                      | `CountedLoopFact`, `CountedSliceLoopFact`                                                  | A loop's induction variable start/bound/step (`0..bound`, step 1) and, for slice-bound loops, how the index is used (unused / index-only / index-and-value)                                                                              | `range_loop` (`CountedLoopFact`), `slice_loop` (`CountedSliceLoopFact`)                                                                                                |
| 25  | `loop_shapes`                       | `LoopShapeFact`, `LoopShapeRejectionFact`                                                  | A loop's high-level shape (counted / reduction / search / copy / fill / sentinel-write) with its induction, accumulator, and collection bindings, plus _why_ a shape match was rejected                                                  | none yet — collected but not read by any rewrite pass; `slice_reduce` and the loop-shape passes currently pattern-match the AST directly instead of querying this fact |
| 26  | `va_list`                           | `VaListAliasFact`                                                                          | A local that's just a clone/alias of the function's sole `va_list` parameter, safe to fold away                                                                                                                                          | `va_list`                                                                                                                                                              |

`goto` (`src/fixups/facts/goto.rs`) is a related but separate case: it is a
front-end-agnostic CFG library (`CfgNode`/`CfgEdge`, dominators, natural
loops, SCCs) used directly by the `goto`-structuring pass
(`src/fixups/query/rules/goto.rs`) to rebuild structured control flow from a
label/jump dispatch loop. It is not backed by any `#[salsa::tracked]` fact —
the `goto` pass runs first, before any other fact is read (see `Pass::Goto` in
`src/fixups/mod.rs`), and builds its own CFG straight from the AST each time
it runs.

## Adding a fact

Add a new module under `src/fixups/facts/`, following the shape every
existing collector uses:

1. Define the fact struct(s) in `src/fixups/facts/mod.rs`.
2. Write `pub(in crate::fixups) fn collect_for_function(function: FunctionId, f: &FnDef, ...) -> Vec<XFact>`
   in the new module, walking with `src/fixups/facts/walk.rs` helpers, for a
   fact that only needs its own function's data. If the fact genuinely needs
   other functions' data (a call graph, a whole-program reduction), write
   `compute(...)`/`collect(...)` instead, taking whatever slices/maps it
   needs as parameters — never a whole facts struct.
3. Add a `#[salsa::tracked]` method calling it: on `impl FunctionInput` in
   `src/fixups/salsa.rs` for a per-function fact (see `def_use`, `effects`,
   ... for the template), or on `impl ProgramInput` for a whole-program one
   (see `callsites`, `lazy_init_singletons`, ...). A
   whole-program tracked fn that needs one function's data narrowly should
   filter a wider tracked fn's output down (see `own_callsites`) rather than
   depending on the whole reduction directly, to avoid invalidating every
   function whenever any one function's data changes.
4. Add a lookup method on `SalsaFacts` next to the existing ones
   (`string_buffer`, `def_use`, `place`, ...).
5. Consume it from a rewrite pass through `QueryContext`/`SalsaFacts`, never
   by re-walking the tree by hand.

Keep collectors read-only and side-effect-free: `collect_for_function`/
`compute`/`collect` must not mutate the AST. If a pass needs the AST changed,
that belongs in `src/fixups/query/rules/`, driven by the facts this layer
already computed.
