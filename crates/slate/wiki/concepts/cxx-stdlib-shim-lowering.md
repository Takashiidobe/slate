# C++ stdlib shim lowering

> Scoping doc, not implemented. Companion to
> [cxx-translation-pain-points.md](cxx-translation-pain-points.md) (the
> "standard library surface is a scale problem" section) and
> [cxx-exceptions-lowering.md](cxx-exceptions-lowering.md) (the ABI-flip
> mechanism this doc reuses at the shim boundary). Proposes the raw-lowering
> target for `std::string`/`std::vector`/`std::map`/iostream/arbitrary
> unmodeled C++ types: shim everything to real, linked C++ object code first,
> idiomatize later, same "transliterate first, idiomatize later" shape as the
> rest of this codebase.
> [cxx-hybrid-migration.md](cxx-hybrid-migration.md) extends this raw fallback
> into an incremental migration design: native Rust and residual C++
> representations may coexist behind lazy bridges until whole-program facts
> prove that the C++ shadow can be deleted.

## Why not reimplement, why not `cxx`

Reimplementing libstdc++ types in Rust means reproducing internal structure
that's genuinely hard to get right, not just tedious — confirmed empirically
on `std::string` alone (see `cxx-translation-pain-points.md`): a 3-line
program touching `std::string` expands to 86 inlined `cir.func`s, including
a self-referential SSO pointer that must be fixed up on move (a naive
byte-copy move leaves it dangling) and internal `_Guard`/`_Terminator` RAII
types implementing the string's *own* exception safety.

The `cxx` crate's approach — opaque handle, thin wrapper functions, never
reproduce internal layout — is the right *pattern*, but the crate itself
isn't the right *mechanism* here: it wants a hand-authored, static bridge
module, and its built-in bridged-type list doesn't cover arbitrary
user-defined classes or template instantiations, which is most of what a
translator actually needs to cross. This doc generalizes the pattern into
something slate generates itself, uniformly, for every unmodeled C++ type —
not just the handful `cxx` already knows about.

Raw mangled-symbol linking (calling directly into `libstdc++.so` exported
symbols, no C++ compilation step) was considered and rejected as the default
mechanism: the Itanium C++ ABI passes non-trivial-for-the-purposes-of-calls
types by hidden reference, a different classification than a C ABI would use
for the "same" `repr(C)` struct declared by value. An `extern "C"` Rust
signature that gets this wrong isn't a lint failure, it's silent ABI
corruption. It also can't safely reach virtual dispatch (no correct vtable
resolution without going through real C++ call syntax) and is fragile across
libstdc++ versions and the dual ABI macro. It may be worth a narrow,
verified optimization later for specific already-exported, trivial-signature
free functions; it is not the general mechanism.

## Design: opaque blob + generated C++ wrapper, every crossing goes through a shim

For every CIR call into a type slate hasn't modeled (initially: all of
`std::string`, `std::vector`, `std::map`, iostream, and any user-defined
C++ class), raw lowering emits:

1. **One small `extern "C"` wrapper function per distinct call site**, into a
   generated companion `.cpp` file, compiled by the same C++ toolchain
   already required (`SLATE_CLANG`). The wrapper's body is the real
   operation — real constructor call, real `append`, real virtual dispatch —
   written using the actual C++ type, because the wrapper file has the real
   headers available. Slate never has to reconstruct C++ semantics itself;
   it only has to reconstruct enough of the call's *signature and callee
   name* (recoverable from the CIR call site's mangled symbol, demangled
   back into source-level syntax) to write the wrapper's one-line body.
2. **An opaque, fixed-size byte buffer** standing in for the C++ object on
   the Rust side, sized and aligned from CIR's own `cir.record_layouts` (no
   need to understand the object's internal fields). Rust code never reads,
   writes, or copies this buffer directly.
3. **Every construct/move/destruct goes through its own shim call** — no
   exception. A generated `Drop` impl calls the destructor shim; a move
   calls a move-constructor shim rather than ever memcpy-ing the bytes.

This is what makes the approach a strict superset of "just shim the hard
types" from the earlier scoping conversation: the same generation machinery
handles `std::string`, `std::vector<T>`, `std::map<K,V>`, iostream, and
arbitrary user classes identically, because none of them are special-cased —
they're all just "a call CIR names, wrapped."

### Why this closes the two hardest structural gaps for free

- **SSO self-referential pointer under move**: never memcpy'd on the Rust
  side at all. Every move point in the CIR (a real move-constructor call)
  becomes a call to a shim wrapping the real move constructor, which does
  libstdc++'s own pointer fixup internally, in real C++, every time.
- **Virtual inheritance / vtable-correct dispatch**: the shim's body is
  ordinary `obj->method()` C++ source: the real compiler resolves the
  virtual call, this-pointer adjustment, and thunk selection. Slate never
  has to reconstruct a vtable layout in Rust.

## Exceptions crossing the shim boundary

Reuses the ABI-flip mechanism already established for `setjmp`/`longjmp`
(`wiki/concepts/setjmp-longjmp-lowering.md`) and for C++ exceptions proper
(`cxx-exceptions-lowering.md`): if a shimmed call can throw, either

- the wrapper catches internally and returns an error-code/`Result`-shaped
  output (loses nothing structurally, costs a `try`/`catch` per wrapper), or
- the extern boundary is flipped to `C-unwind` so a real `panic!`/
  `resume_unwind` propagates through, matching the panic-based raw-lowering
  target already chosen for exceptions generally.

A caught exception's payload is itself a C++ object — to preserve it as more
than a `what()` string (the choice `cxx` makes), it needs the *same*
opaque-blob-plus-RTTI-tag treatment as any other shimmed type on the way
back across, not a separate mechanism. Not free, but not new either.

## Known limits — things the shim mechanism cannot directly cross

**No-linkage types have no name a separate companion file can use.**
Anonymous-namespace classes, function-local classes, and (the common case)
lambda closure types have no stable spelling outside their original
translation unit — `extern "C" void wrapper(ClosureType c)` is simply not
writable in a standalone `.cpp`. This hits `std::sort`/`std::for_each` with
a lambda comparator and any callback-taking API (`std::function`) directly.
Escape hatch: type-erase at the boundary (function pointer + opaque `void*`
context) instead of the generic "same signature, opaque blob" treatment —
a real special case that needs its own handling, not automatic from the
general mechanism. A second, less common escape hatch — generating the
wrapper *inside* the original translation unit instead of a separate
companion file, so the no-linkage type's name is still in scope — trades
away the "one clean companion file" simplicity for coverage.

**Shim compilation must match the original TU's flags byte-for-byte.**
`_GLIBCXX_USE_CXX11_ABI`, `_GLIBCXX_DEBUG` (injects bounds-checking fields
into iterators/containers), and similar macros change struct layout. If the
generated shim is compiled with different defines than the original source
was, the opaque blob's assumed size/layout silently diverges from what
`cir.record_layouts` recorded for it. Needs exact flag propagation per
translation unit, not just "same compiler version" — an easy, silent way to
get this wrong.

**The idiomize boundary must cut along whole-type-graph lines, never through
the middle.** Once a later pass idiomizes some `MyStruct` into a different
Rust layout, any still-shimmed container templated on it (a
`std::vector<MyStruct>` still calling real C++ `push_back`) breaks, because
that shim's C++ side still expects `MyStruct`'s original bytes. The same
constraint applies to polymorphic hierarchies: a class can't be idiomized
out of a still-shimmed inheritance chain, since the vtable is only
meaningful as a single, coherently-compiled unit. Practical consequence:
idiomization has to proceed bottom-up (leaves of the type-usage graph first)
or all-at-once per connected component — never on one member type in
isolation while its containers or base classes stay shimmed.

**Performance, not correctness.** Every operation on a shimmed type is a
real ABI call. `operator+=` in a hot loop over `std::string` goes from
inlinable, optimizer-visible code to a genuine function-call round trip per
iteration. This is an accepted, known cost of "shim everything first" as a
strategy — expected to matter for loop-heavy fixtures before idiomization
recovers native Rust types, not a surprise to debug later.

## Sequencing (matches the setjmp/longjmp and exceptions precedent)

1. Raw lowering: every unmodeled C++ type becomes opaque-blob-plus-shim-calls,
   as above. Get this compiling and differentially correct first — no
   idiomization starts before this is a working baseline to diff against
   (same ordering `SetjmpRecovery` used over raw `setjmp`/`longjmp`, and the
   same ordering already declared for the exceptions
   panic-first/`Result`-second plan).
2. Idiomization: a separate pass recognizes "this blob is only ever touched
   via `std::string`'s common method set" (or `std::vector<T>`'s, etc.) and
   replaces it with a real Rust type, deleting the shim calls for that
   recovered type/subgraph — gated by the whole-type-graph-cut constraint
   above.
3. Hybrid migration may bridge across a remaining cut instead of treating it
   as an absolute barrier. This costs recursive conversion and requires alias,
   escape, identity, and synchronization preconditions; see
   [cxx-hybrid-migration.md](cxx-hybrid-migration.md).

## Open questions (not yet resolved)

- Whether to key shim wrapper generation off the mangled symbol name (dedup
  identical instantiations across call sites, same join key already used
  for ordinary template handling) or generate one per call site and dedup
  later — affects companion-file size but not correctness either way.
- Whether narrow, verified raw-symbol linking (skipping the wrapper) is
  worth doing later for the subset of already-exported, trivial-signature,
  non-virtual free functions — a possible optimization on top of this
  design, not a replacement for it.
- How exception payload marshaling (opaque-blob-plus-RTTI-tag, above) picks
  which concrete blob shape to reconstruct on the Rust side when the catch
  clause's declared type is a base class rather than the thrown type exactly
  — same base-class-catch gap already open in
  [cxx-exceptions-lowering.md](cxx-exceptions-lowering.md).
