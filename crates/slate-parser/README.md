# slate-parser

A C frontend (preprocessing + parsing + sema) that, unlike clang, does not
collapse conditional compilation into a single configuration. Instead it
preserves *all* preprocessor branches as first-class data in the AST, so a
downstream tool can later evaluate that AST against a chosen set of flags
(e.g. a Rust target + feature set) and get back a concrete, clang-equivalent
AST for that one configuration — repeated cheaply for every configuration
the user cares about, without re-lexing/re-parsing/re-sema'ing each time.

No codegen. The output is a structured AST; splicing it into Rust (`cfg`
attributes, etc.) is a separate downstream concern, out of scope here.

Correctness is validated by fuzzing: for any single concrete configuration,
this frontend's evaluated output must match clang's behavior for that same
configuration (`-D` set), diagnostics included.

## Architecture

```
source text
    │
    ▼
Lexer            tokens, unaware of conditionals
    │
    ▼
Preprocessor     macro table: HashMap<String, Conditional<MacroDef>>
    │            expansion produces Conditional<TokenStream> at any
    │            point where the macro's definition itself diverges
    │            across branches — not just at #ifdef sites
    ▼
Parser           branch-point aware: consumes Conditional token regions
    │            inline and emits Conditional<Decl/Stmt/Expr/...> nodes
    │            wherever branches disagree in shape.
    │
    │            Carries its own small conditional state alongside the
    │            macro table: the typedef-name set used to disambiguate
    │            C's context-sensitive grammar (e.g. `(A)(B)` as cast vs.
    │            call). This is NOT full sema and can't be deferred to
    │            after eval — it affects how the AST itself gets shaped —
    │            but it must be branch-aware for the same reason the
    │            macro table is: a name can be a typedef in one branch
    │            and not another.
    ▼
Polyvariant AST  the artifact this crate produces: a full,
    │            NOT-type-checked AST where divergence is represented as
    │            Conditional<T> nodes carrying a Condition guard +
    │            per-branch value
    ▼
Eval visitor     input: a concrete flag environment (defined macros +
                 values + target features). Pure structural fold — no
                 semantic work involved. For each Conditional node:
                 select the one true branch, concatenate non-conditional
                 siblings, or diagnose (zero-true / multiple-true on what
                 should be an exhaustive #if/#elif/#else chain).
    ▼
Concrete AST     one configuration's tree, still untyped.
    ▼
Sema             ordinary monomorphic C sema (mirrors clang exactly,
                 since there's now exactly one branch, same as clang
                 ever sees) — type checking, implicit conversions,
                 constant expression evaluation, diagnostics. Run once
                 per configuration the caller actually asked for (N
                 runs for N configs), not once per theoretical
                 combination.
    ▼
Typed concrete   input to an out-of-scope backend that splices it
AST              into Rust.
```

### Why sema is deferred past eval, not run on the polyvariant AST

If sema ran on the polyvariant AST before eval, it would have to handle
code like:
```c
#ifdef _WIN32
typedef HANDLE Socket;
#else
typedef int Socket;
#endif
...
Socket x; x = foo();     // unconditional code, conditional type
```
The `Socket` lookup here returns `Conditional<Type>`, so checking `x =
foo()` would need to become `Conditional<Diagnostic>` too, joining
conditions with `And` wherever multiple conditional bindings are jointly
live at a use site — i.e. type-checking every reachable *combination* of
conditional bindings, not just every branch in isolation. That's exactly
the combinatorial blowup structural per-config parsing was meant to
avoid, just moved one phase later.

Since the actual requirement is only "sema must hold for the flag
combinations the caller supplies" (their real Rust targets + features),
not universal validity across all theoretical combinations, deferring
sema to the concrete, single-config AST sidesteps this entirely: it
becomes the same monomorphic type checking clang already does, run N
times for N requested configs instead of being reinvented as a
conditional-propagating type system.

### Condition representation

`#if`/`#ifdef`/`#elif` guards are constant expressions, evaluated after
macro expansion — same as clang. Represent them with the same expression
AST used for regular C constant-expressions, rather than a separate mini
language, plus the preprocessor-only primitives (`defined(X)`,
`__has_include`, `__has_attribute`, etc.) as leaf nodes:

```rust
enum Condition {
    Defined(String),
    HasAttribute(String),
    HasInclude(String),
    Not(Box<Condition>),
    And(Box<Condition>, Box<Condition>),
    Or(Box<Condition>, Box<Condition>),
    Expr(ConstExpr), // arbitrary #if arithmetic, e.g. __GNUC__ >= 4
}

struct Conditional<T> {
    branches: Vec<(Condition, T)>, // in source order; last may be `else`
}
```

### Why macro-value divergence matters (not just structural divergence)

```c
#ifdef _WIN32
typedef HANDLE Socket;
#else
typedef int Socket;
#endif
```
is divergence at a structural site — the parser sees two different
`typedef` branches directly.

```c
#ifdef _WIN32
#define FLAG 1
#else
#define FLAG 2
#endif
...
int x = FLAG;
```
is divergence *inside a macro definition*, observed far from any
`#ifdef` in the token stream. This is why the macro table is
`HashMap<String, Conditional<MacroDef>>` from day one, and expansion of
a divergent macro produces `Conditional<TokenStream>` that the parser
must treat as a branch point, exactly like a direct `#ifdef` region.

### Provenance (implemented ahead of schedule — see `src/files.rs`, `src/pp.rs`)

A requirement added once the eval/oracle pieces existed: downstream Rust
codegen needs to tell "`ULONG_MAX` from `<limits.h>`" apart from a
same-named constant in a user-provided header, so it can special-case
known stdlib constants (`ULONG_MAX` → `u64::MAX`, say) instead of emitting
a raw translated value for everything. That requires knowing which file a
declaration actually came from, which requires `#include` resolution to
exist — pulled forward ahead of full macro expansion (phase 2) rather
than deferred, since there's nothing to report provenance *of* without it.

```rust
struct FileId(u32);   // interned path

enum HeaderKind { System, User }  // <...> search vs "..." search

struct Provenance { file: FileId, kind: HeaderKind }
```

`FunctionDecl` and `Decl::Typedef` (and their `Concrete*` counterparts)
carry a `Provenance` field. `Files` (`src/files.rs`) interns `FileId ->
(PathBuf, HeaderKind)`; `SearchPaths` holds separate `user`/`system`
directory lists, and `#include <...>` only searches `system` while
`#include "..."` checks the including file's own directory, then `user`,
then falls back to `system` — mirroring real preprocessor search order
rather than inventing a simplified rule. `Preprocessor::parse_file` walks
real files on disk (recursing into `#include`); `Preprocessor::parse_str`
keeps the existing in-memory-string entry point for tests/demos that
don't need real headers, tagging the whole source as `User` provenance
under a synthetic file name.

Confirmed against clang empirically before building this: `-ast-dump=json`
does *not* expose macro definitions directly (`-detailed-preprocessing-record`
doesn't inject `MacroDefinitionRecord` nodes into that dump — tested, not
just assumed), but every location clang emits already distinguishes
`spellingLoc` (where a token's text actually lives) from `expansionLoc`
(where a macro was invoked), so `ULONG_MAX`'s origin file is recoverable
from the AST oracle without libclang bindings, once phase 2's real macro
values exist to compare against.

## Phases

Each phase should be validated against clang before moving to the next
(fuzz harness: preprocessor output via `-E -P`, AST via
`-Xclang -ast-dump=json -fsyntax-only`, pinned to one clang version).

**Status: phases 0-5 are implemented (parsing, including most of GNU/C23
extension surface, evaluation to a concrete AST, and a reachability pass);
phase 6 (sema) is in progress; phases 7-8 are not started.** Run `bd ready` /
`bd list` for the current breakdown of open work per phase.

**Phase 0 — flat lexer + flat parser, no conditionals, no sema** (done)
Target: `int main() { return 3; }`. Tokenizer for keywords/identifiers/
int literals/punctuators; recursive-descent parser for a function
definition, compound-stmt, return-stmt, integer constant expr. No
preprocessor yet, no type checking yet — sema is deferred to its own
phase after eval (see above). Establishes the non-conditional AST shape
everything else builds on.

**Phase 1 — preprocessor conditional stack, structural divergence only** (done)
Add `#ifdef`/`#ifndef`/`#if`/`#elif`/`#else`/`#endif` and `defined()`,
tracked as a condition stack. No macro object/function-like expansion
yet — just enough to make
```c
int main() {
#ifdef _WIN32
    return 2;
#else
    return 3;
#endif
}
```
parse into a `Conditional<Stmt>` for the `return` statement. Parser
becomes branch-point aware here: this is the invasive step where
statement/decl lists must accept an interleaved branch point mid-list.
Also introduces the parser's own conditional typedef-name set (needed
for grammar disambiguation, not full sema — see architecture section).

**Phase 2 — object/function-like macros, `HashMap<String, Conditional<MacroDef>>`** (done)
Macro expansion (object-like, function-like, `##`, `#`, `__VA_ARGS__`).
Every macro definition/redefinition is inserted into the table as an
additional branch under the current condition stack. Expansion of a
macro whose table entry has >1 branch produces `Conditional<TokenStream>`,
which the parser consumes the same way it consumes a direct `#ifdef`
region from phase 1. This is what makes the `FLAG` example above work.

**Phase 3 — full C declarator/type grammar (parse-only)** (done)
Struct/union/enum, arrays, function pointers, qualifiers, storage
classes, initializers (including designated) — parsed and represented
in the polyvariant AST, but not yet type-checked. Constant expression
evaluation still needed here for `#if` and array bounds/case labels
(this is integer constant folding, not full sema — it's needed to
parse correctly, same as the typedef-name tracking from phase 1). This
is the bulk of the "regular C" grammar surface and the largest phase by
LOC. Remaining edge cases (bitfields, anonymous struct/union members,
K&R-style declarators, `_Atomic`/`_BitInt(N)`/complex numeric types,
`typeof`) are tracked as their own follow-up issues rather than blocking
later phases.

Phase 3.5 (full C statement/expression grammar, parse-only) landed
alongside phase 3 rather than as a separate step.

**Phase 4 — GNU/clang extensions (parse-only)** (mostly done)
`__attribute__`, `_Generic`, statement expressions, computed goto,
nested functions, VLAs, C23 additions clang ships — implemented. The
`__builtin_*` catalog, vector extensions, and inline asm (parse-only —
no need to understand semantics beyond preserving it opaquely) are the
remaining slice of this phase.

**Phase 5 — eval visitor** (implemented, ahead of schedule — see `src/eval.rs`)
Given a concrete flag environment (`Env`, a defined-macro set for now),
fold the polyvariant AST into a concrete, `Conditional`-free AST —
`ConcreteDecl`/`ConcreteStmt`/`ConcreteTranslationUnit`, distinct types
from the polyvariant ones so downstream code gets a type-level guarantee
that no branching remains, not just a runtime one.

Getting this working surfaced a correction to the paragraph above: a
`Conditional` node with **zero** true branches is not an error — it's
the normal outcome of `#ifdef X ... #endif` with no `#else` when `X`
isn't defined, and correctly contributes nothing (the same as a real
preprocessor just never emitting those lines). Only **more than one**
true branch is a bug worth panicking on, since it means two conditions
this frontend built as mutually exclusive both held — which shouldn't
happen for straightforward `defined()`/`not()` pairs and would indicate
a construction bug once real `#if`/`#elif` arithmetic is added. This was
built alongside phases 1's `Conditional<Stmt>` and the typedef example's
`Decl::Conditional`, rather than deferred to the end, since both already
needed *some* eval path to be testable end-to-end.

**Phase 6 — sema, on the concrete AST** (in progress — see `src/sema.rs`)
Ordinary monomorphic C sema: implicit conversions, integer promotions,
scope/tag resolution, declarator/type validation, full diagnostics —
run once per requested flag configuration, on the already-evaluated
tree. Deferred to here specifically to avoid conditional-propagating
type checking (see architecture section above); this is also the phase
that should track most closely against clang's own diagnostics, since
by this point there's exactly one branch, same as clang ever sees.
Scope/tag/declaration resolution, implicit-conversion checking, and
full constant-expression semantic validation are still open.

A reachability pass (`src/reachability.rs`) also runs on the concrete
AST: it marks statements after an unconditional jump/return/etc. as
`ConcreteStmt::Unreachable`, purely structurally (no type information
needed) — this and a human-readable AST renderer (`src/render.rs`, used
by the FileCheck fixtures) were added ahead of schedule alongside eval
and phase 3.5, since both needed something to make the evaluated output
inspectable/testable end to end.

**Phase 7 — fuzzing harness hardening**
Corpus generation, clang-version pinning, differential fuzzing loop
(random flag subsets × generated/mutated C source → compare
per-configuration typed concrete AST against clang's oracle for that
configuration).

A first version of the oracle comparison is already implemented in the
filecheck integration test, ahead of full fuzzing infrastructure, since
eval() and the typedef example needed *some* correctness check against
real clang to be worth trusting. The comparison is deliberately made
only after eval: the polyvariant AST retains preprocessing branches and
is checked by its own FileCheck expectations, while Clang's AST is
already post-preprocessing and post-sema. Design decision worth keeping:
this does **not** convert clang's AST JSON into our own `Decl`/`Stmt`
types wholesale. A full importer would mean maintaining a
clang-JSON-import rule for every node kind we ever add to our own AST,
on top of building the feature itself — and it would be brittle in the
wrong direction, since clang's raw tree carries wrapper nodes
(`ImplicitCastExpr`, exact `SourceLocation`s) we deliberately don't
model, so literal tree-shape equality would flag non-semantic
differences as failures.

Instead, both sides get projected down into a small canonical
`DeclSummary` (defined only in the test) — "function X returns these
integer literals in order", "typedef X aliases this type name" — and
the two summaries are compared. This only needs to grow in step with
what *our evaluated, reachable* AST currently claims to model, not with
clang's entire schema. Parsing clang's JSON side uses the `clang-ast`
crate (dtolnay):
you declare only the node-kind variants and fields you care about
(`FunctionDecl`, `TypedefDecl`, `ReturnStmt`, `IntegerLiteral` so far)
and an `Other` catch-all absorbs everything else, so adding coverage
for a new C construct later is additive, not a rewrite.

## Open questions to revisit

- Basic `#include` resolution + `Provenance` landed early (see the
  Provenance section above) to unblock header-origin tracking, but it
  doesn't yet interact with the macro table's conditionality — once
  phase 2's `HashMap<String, Conditional<MacroDef>>` exists, a macro
  `#define`d differently by two included headers (rather than by two
  branches of the same `#ifdef`) needs the same conditional treatment,
  and `MacroDef` will need its own `Provenance` too (this is the
  `ULONG_MAX` case the whole feature exists for).
- Include guards / `#pragma once` aren't implemented — repeated
  `#include` of the same header currently just re-parses it and
  re-declares everything, which will produce duplicate-decl noise
  once sema (phase 6) exists to complain about it.
- Whether integer constant folding for `#if`/array-bounds (needed
  during parsing, phases 1 and 3) should share a constant-evaluator
  implementation with phase 6's sema, or stay a separate, narrower
  evaluator — they overlap but sema's version also needs real types.

## In-progress: span/location tracking overhaul

A cross-cutting refactor (tracked as the `1uq` epic) is replacing ad-hoc
string-offset error reporting with real source spans, since phase 6
diagnostics need to point at exact source locations rather than
re-searching source text for a substring. So far: `Loc{file, offset,
length}` and a generic `Span<T>{value, spelling, expansion}` (`ast.rs`);
the lexer now returns `Vec<Span<Token>>` with real byte offsets
(`lexer.rs`, rewritten around a `Lexer` struct with `peek`/`try_consume`
helpers instead of hand-rolled lookahead); a `Cursor` trait shared by the
parser's per-construct fragments; and declaration/tag/field parsing in
`parser.rs` consuming preprocessor-retained token spans instead of
re-lexing source substrings. Remaining: composing spelling vs. expansion
spans for macro-expanded tokens, and propagating `Span<T>` through the
rest of the polyvariant/concrete AST (currently only declarations, tags,
and some statements carry it) so it reaches sema diagnostics.
