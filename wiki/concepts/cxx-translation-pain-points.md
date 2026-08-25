# C++ translation pain points (pre-implementation scoping)

> Exploratory scoping, not implemented. CIR doesn't support C++ yet (expected
> to stabilize for it over the next year). This records what's already known
> from probing `~/llvm-project/build-cir/bin/clang -Xclang -fclangir -Xclang
> -emit-cir` and `-ast-dump` on small C++ snippets, so it isn't re-derived
> from scratch when C++ support actually starts. See
> [slate-overview.md](slate-overview.md) for the current C-only supported
> subset and [architecture.md](architecture.md) for the CIR/AST join this
> all builds on.

## Templates: CIR only ever shows the monomorphized side

Verified empirically: a template `T add(T a, T b) { return a + b; }`
instantiated at `int` and `double` produces two ordinary, separately mangled
`cir.func`s (`_Z3addIiET_S0_S0_`, `_Z3addIdET_S0_S0_`), `comdat
linkonce_odr`, with the operation already resolved per instantiation
(`cir.add` for `int`, `cir.fadd` for `double`). No generic-parameter node, no
shared body — call sites already reference the resolved mangled symbol
(`cir.call @_Z3addIiET_S0_S0_(...)`), same as any ordinary function call.

Implication: template instantiations are, in CIR, just more functions in the
module, same shape as anything already lowered. The new work is (a)
deduplicating repeated instantiations of the same `<T>` across call sites,
and (b) deciding whether to collapse instantiations back into one idiomatic
Rust generic (needs trait-bound inference CIR doesn't give you) or leave them
as separate monomorphized functions (matches CIR directly, consistent with
"transliterate first, idiomatize later").

## The AST join needs a second key for templates

The current C join (source location -> single CIR node) breaks here: N
`FunctionDecl`s share the *same* source range as each other and as the
uninstantiated template pattern. Confirmed via `-ast-dump`:

```
FunctionTemplateDecl <1:1, 4:1> add
├─ FunctionDecl 'T (T, T)'                          <- pattern, no CIR ever exists for it
│    `- BinaryOperator '<dependent type>' '+'         (never sema-checked, never lowered)
├─ FunctionDecl 'int (int, int)' implicit_instantiation instantiated_from 0x...(pattern)
│    |- TemplateArgument type 'int'
├─ FunctionDecl 'double (double, double)' implicit_instantiation instantiated_from 0x...(pattern)
│    |- TemplateArgument type 'double'
```

Each instantiation `FunctionDecl` carries `instantiated_from` (pointer back
to the pattern) plus an explicit `TemplateArgument` list — the AST-side
equivalent of the CIR mangled-name suffix. Call sites' `DeclRefExpr`s point
at the specific instantiation Decl, not the template, so decl-pointer
identity of the instantiation is the correct join key, matched against the
CIR symbol name rather than location. The generic pattern `FunctionDecl` has
no CIR counterpart at all but is the only place the uninstantiated parameter
name and dependent-type expression survive — the thing you'd need if
attempting to reconstruct a real Rust generic rather than N monomorphized
functions.

## Value-level constexpr metaprogramming is a non-issue

Recursive template metaprogramming used purely to compute a constant at
compile time (classic `Fib<N>::value` recursion) folds away entirely before
CIR — confirmed empirically: `Fib<10>::value` lowers to a single
`cir.global constant ... = #cir.int<55>`, no trace of the recursive
instantiation chain. Nothing to translate; if ever needed, the natural Rust
target is `const fn`/const generics, which do compile-time evaluation
directly rather than needing type-recursion tricks.

## SFINAE: CIR shows "what was chosen," never "what were the choices"

Confirmed empirically with a `has_size<T>`-detection-idiom `length(T)`
overload pair (`std::enable_if` gating two overloads on whether `T` has a
`.size()` method): the SFINAE probe (`test(int)`/`test(...)` overload trick)
never appears in CIR at all — only the two winning `length` instantiations
survive, distinguishable only by parsing the `enable_if` condition out of the
mangled name (`Xsr8has_sizeIT_EE5value` vs the negated `Xntsr8has_size...`).

The AST recovers the choice set the same way templates do: both winning
instantiations share `instantiated_from` back to one `FunctionTemplateDecl`,
and the `enable_if` template argument expression is inspectable on the AST
node — CIR alone can't reconstruct this, the AST is required.

Recovering the predicate splits into two buckets with very different
outcomes:

- **Single recognizable capability** (has a member, has a trait-shaped
  operation, is convertible to X): mechanically portable to a hand-authored
  Rust marker trait (`trait HasSize { fn size(&self) -> usize; }`). This
  works even on stable Rust via the autoref-count trick ("poor man's
  specialization" — method resolution prefers fewer autoderefs), no nightly
  needed.
- **Arbitrary compound expression validity**, especially chained via
  implicit-conversion priority ranking (the `priority_tag`/fallback-chain
  idiom: try overload A, silently fall back to overload B if A doesn't
  typecheck) — has no Rust target at *any* stability level. Rust trait
  bounds only test "does an impl exist," never "is this expression
  well-formed." Nightly `specialization`/`min_specialization` doesn't cover
  it either — that mechanism ranks impl specificity over type/trait
  structure, not expression well-formedness; it's a structurally different,
  narrower tool, not a gated version of the same one. This bucket is a real
  unsupported/manual-port boundary, not a fixup-pass problem.

Also checked and ruled out: `rustc_private` (linking slate itself against
rustc internals) is irrelevant to what the *generated* Rust code can do —
it's for building compiler-adjacent tools, not for granting generated code
access to unstable language features.

### The autoref trick only works as a per-call-site macro, never inside a generic function

Verified empirically. The natural-looking translation of a priority-tag
overload set is a generic Rust function using the autoref-ranking trick
(implement the fallback trait for the least-referenced wrapper type, the
more specific traits for more-referenced wrapper types, and call through
`(&&&val).process()` so method resolution's deref-candidate search finds the
shallowest — most specific — applicable impl first):

```rust
pub fn dispatch<T: ?Sized>(val: &T) {
    (&&Wrap(val)).process();
}
```

This always resolves to the single fallback impl, regardless of `T`,
confirmed by compiling and running it. The reason isn't reference-depth
arithmetic (several depth arrangements were tried and all failed the same
way) — it's structural: `dispatch<T: ?Sized>` is a generic function, and
`.process()` is type-checked exactly once against the abstract `T`, before
monomorphization. There is no per-instantiation redo of method resolution to
exploit; the trick needs a concrete type at the point method resolution
actually runs.

Rewriting the same dispatch as a `macro_rules!` that expands `(&&Wrap($val)).process()`
textually at each call site — so `$val`'s type is already concrete
(`i32`, `&str`, `&[i32]`) by the time the expansion is type-checked — does
correctly select the fallback/medium/highest-tier impl per call site, once
the reference-depth ordering is also fixed to put the *most specific* impl
at the shallowest depth and the fallback at the bare (zero-ref) type:

```rust
impl<'a> Priority1String for &&Wrap<'a, str> { .. }       // shallowest = highest priority
impl<'a, T> Priority2Slice for &Wrap<'a, [T]> { .. }       // middle
impl<'a, T: ?Sized> Priority3Generic for Wrap<'a, T> { .. } // bare = fallback, found last
```

Consequence for translation: even the "recoverable" (single-recognizable-capability)
SFINAE bucket above only survives translation as a *macro expanded per
instantiation*, not as one idiomatic generic `fn`. That's the same
already-monomorphized shape CIR hands you for ordinary templates in the
first place (see the templates section above) — the autoref trick doesn't
let you claw back a single generic function C++ users write once; it only
reproduces the per-instantiation dispatch CIR already exposes, via a
different mechanism (macro expansion instead of template instantiation).

### Disjunctive *gating* is solvable via `#[marker]`; correction to an earlier claim here

Originally logged as a third unemulatable case ("no Rust target even with
nightly `specialization`"). That was wrong, and worth correcting rather than
leaving in place — `#![feature(marker_trait_attr)]` is a distinct nightly
feature from `specialization`, not a variant of it, and it solves exactly
this case. Confirmed empirically:

```rust
#![feature(marker_trait_attr)]

trait TraitA { fn do_a(&self); }
trait TraitB { fn do_b(&self); }

#[marker]
trait PassesFilter {}

impl<T: TraitA> PassesFilter for T {}
impl<T: TraitB> PassesFilter for T {}

fn process_union_type<T: PassesFilter>(item: T) { .. }
```

Compiles and runs correctly for a type implementing only `TraitA`, only
`TraitB`, or both; a type implementing neither fails with the expected
`the trait bound Neither: PassesFilter is not satisfied`. `#[marker]`
relaxes coherence specifically for traits with **no items at all** — a
marker trait carries no methods/associated types/consts to dispatch
through, so multiple overlapping blanket impls can never be ambiguous, only
ever redundant evidence of the same fact ("at least one bound holds").
Confirmed the boundary is compiler-enforced, not just a convention: adding
so much as a defaulted method to the marker trait, or any item to either
impl, is a hard error (`E0714`/`E0715`, "marker traits cannot have
associated items" / "impls for marker traits cannot contain items").

That boundary is exactly where this technique's coverage ends. `#[marker]`
solves disjunctive **gating** — `enable_if<A<T>::value || B<T>::value>`
where the function body is uniform regardless of which disjunct matched
(`generic_handler(t)` either way) — because gating is all a marker trait
bound can express; it has nothing to dispatch through. It does **not**
solve disjunctive **dispatch** — C++'s `if constexpr (A<T>) { .. } else if
constexpr (B<T>) { .. }` pattern, where the two branches genuinely differ.
When `A`'s and `B`'s domains are provably non-overlapping (or made
non-overlapping by construction) this reduces to two ordinary impls, one
bounded `T: A` and one bounded `T: B`, each with its own body — the same
"N impls instead of one generic" outcome this section already accepts as
the baseline translation shape, no marker trait or specialization required.

### Disjunctive dispatch with real overlap and a priority tie-break: genuinely unsupported

The case that actually breaks is when `A` and `B` are independent,
open-ended, user-extensible predicates that **can both hold for the same
type**, and the two branches need different bodies with one taking priority
— e.g. a type implementing both `TraitA` and `TraitB`, where `if constexpr
(TraitA)` must win. Checked two escape routes; both confirmed closed.

A tag/autoref-based construction was tried first
(`impl<T: TraitA> BranchA for &&ChoiceTag<T> { .. }` /
`impl<T: TraitB> BranchB for &ChoiceTag<T> { .. }`, dispatching via
`(&&ChoiceTag::<T>::new()).execute()`). It fails for two independent
reasons, stacked: the un-parameterized form hits `E0207` ("type parameter
`T` is not constrained by the impl trait, self type, or predicates") because
`T` appears only in the bound, not in the implementing type; fixing that by
making the tag generic over `T` just exposes the same generic-function
timing bug as the very first autoref attempt in this doc — a call inside a
generic `fn dispatch<T>(...)` type-checks once against the abstract `T`,
before monomorphization, so it always resolves to the one branch applicable
to *every* `T` (confirmed: prints `Fallback` unconditionally, same failure
mode as the original `dispatch<T: ?Sized>` case above).

Second, and more fundamental: even sidestepping the generic-function timing
issue entirely (e.g. by expanding per call site via macro, the fix that
worked for the recoverable-SFINAE case), ordinary overlapping impls plus
`default fn` under `#![feature(specialization)]` still hard-errors:

```rust
impl<T: TraitB> Handle for T { default fn handle(&self) { println!("B branch"); } }
impl<T: TraitA> Handle for T { fn handle(&self) { println!("A branch"); } }
```
```
error[E0119]: conflicting implementations of trait `Handle`
```

`default fn` alone doesn't grant the override — specialization only permits
it when the compiler can *prove* one impl's domain is a strict subset of
the other's, and two independent trait-bounded predicates have no such
relationship. The natural C++-style workaround, manually breaking the tie
with negation (`enable_if<!is_a<T>::value && is_b<T>::value>` for the B
branch), doesn't exist in Rust either — confirmed:

```rust
fn f<T: TraitB + !TraitA>(_t: T) {}
```
```
error: negative bounds are not supported
```

Rust has no negative trait bounds for user-defined traits at any stability
level (the compiler-internal negative impls for built-in auto traits like
`impl !Send for Foo` are a closed, unrelated mechanism — not usable as a
`where`-bound on an arbitrary trait). So there is no construction — marker
traits, specialization, autoref/tag tricks, or macro expansion — that
reproduces C++'s "prefer A, fall back to B" `if constexpr` chain once `A`
and `B` can genuinely both hold for the same type. This is a real
unsupported/manual-port boundary for slate, not a fixup-pass problem: a
type actually hitting this (implementing both predicates, with the source
relying on `is_a` taking priority over `is_b`) has no faithful
single-generic-function Rust translation at all, and needs a human to
resolve the priority explicitly per instantiation.

## Move semantics are a different ownership model, not a syntax gap

C++ move leaves the moved-from object alive and still destructible with
unspecified-but-valid state; Rust move makes the source statically
inaccessible. A faithful transliteration of a C++ move constructor needs the
moved-from object to remain destructible, which likely means `Option<T>`
wrapping or an explicit moved-flag rather than an actual Rust move. Every
pass-by-value/return-by-value/assignment additionally needs a
copy-vs-move-vs-borrow decision per use site — CIR should expose the
copy-constructor call sites needed to decide this, but getting it wrong
silently changes program behavior (extra copies, or broken aliasing
assumptions the C++ source relied on) rather than failing to compile.

## RAII + exceptions have no direct Rust control-flow shape

Destructors fire during stack unwinding across arbitrarily many frames,
interleaved with partial-construction cleanup (member N failed, destruct
0..N-1 in reverse). Two possible Rust targets, both real rewrites rather than
transliterations: exceptions become panics (untyped, unwind-based, loses the
payload type) or get restructured into explicit `Result` propagation (typed,
changes control flow shape). Compare
[setjmp-longjmp-lowering.md](setjmp-longjmp-lowering.md), which already
treats a related unwind-shaped C construct (`longjmp`) as
`catch_unwind`/`panic!` rather than a raw FFI call — C++ exceptions are the
same family of problem, at a larger scale (arbitrary destructor chains
instead of a single jump target).

## Multiple/virtual inheritance has no Rust equivalent

Single inheritance maps tolerably to `dyn Trait` + vtable-shaped struct.
Multiple inheritance and virtual bases (diamond problem) rely on
this-pointer adjustment thunks with no structural Rust counterpart — Rust
has no multiple inheritance. Candidate treatment: flatten to composition
with manually synthesized dispatch, or scope as an explicit
unsupported/diagnose-and-skip boundary the way gcc-torture triage already
carves out unsupported corpus cases (see
[gcc-torture-triage.md](gcc-torture-triage.md)).

## Static initialization order (SIOF) is a genuinely new pass category

C++ constructs namespace-scope objects via constructors run in
file-link-order across translation units; Rust has no static constructors at
all — everything is `const`-evaluable or lazily initialized. This needs
explicit lazy-init synthesis (`OnceLock`-shaped), unlike anything the C
pipeline currently needs to emit.

## Standard library surface is a scale problem, not a design problem

`std::string`, `std::vector`, `std::map`, iostream, etc. need a shim in the
same shape as [libc-shim.md](libc-shim.md), just a much larger surface area
— more of the same kind of work already done for libc, not a new mechanism.
