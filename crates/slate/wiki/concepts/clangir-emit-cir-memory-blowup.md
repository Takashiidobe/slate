# ClangIR `-emit-cir` memory blowup on large, type-recursive TUs

> Upstream ClangIR/MLIR limitation, not a slate bug. Recorded so the sqlite
> translation epic (`slate-wcf7`) doesn't get re-blocked by the same
> investigation, and so an upstream bug report can be filed without
> re-deriving the root cause.

## Symptom

`clang -fclangir -emit-cir` on `~/sqlite/bld/sqlite3.c` (the 140k-line
amalgamated build) exhausts memory (unbounded RSS, 16.8GB+ observed, killed
manually) and never finishes. `-ast-dump=json -fsyntax-only` on the same file
is fine (0.86s) — the problem is specific to CIRGen/`-emit-cir`, not
Sema/parsing. Bisected by function-boundary line-count prefixes under an
`ulimit -v` safety cap: 19698 lines succeeds (7.2s/330MB); 59998 lines aborts
via `llvm::report_bad_alloc_error` (~80s); the full file times out with RSS
still climbing.

Confirmed **not** a symptom of slate's own clang fork: rebuilding a clean
`llvm-project` at the merge-base with identical flags reproduces identical
abort points/timings. Confirmed **not** driven by any slate-side flag
(`-nostdlibinc`+shim, `-std=gnu23`, target triple, `__SLATE_*` defines): a
minimal `clang -fclangir -emit-cir` invocation with default host
target/headers shows no measurable difference below the cliff (couldn't test
at the size where it actually manifests — even the known-good full-flags
case takes ~80s just to abort there, too slow to bound a comparison).
Per-file translation (sqlite's individual `src/*.c` instead of the
amalgamation) is **not** a full escape hatch either: `vdbe.c` alone (9574
lines, smaller than `btree.c` which is fine) independently reproduces the
same blowup standalone.

## Root cause: non-cyclic alias re-expansion during printing, not CIRGen

A real crash backtrace (not just a timeout) shows the abort inside a single
`mlir::Operation::print` call, cycling through
`StructType::print <-> PointerType::print <-> ArrayType/UnionType::print` via
`CIRDialect::printType`, ending in a `bad_alloc` from a `std::string`
reallocation. So this happens while **serializing** the built module, not
while building it.

`clang/lib/CIR/Dialect/IR/CIRTypes.cpp`'s `printRecordBody()` only guards
literal self-reference cycles via `tryStartCyclicPrint` (instance-identity
keyed). `CIRDialect.cpp` registers a short alias for named records
(`CIROpAsmDialectInterface::getAlias`, `!cir.rec_<name>`), but
`printRecordBody`'s per-member `printer.printType(member)` never consults
that alias table for repeat, **non-cyclic** occurrences of the same type — it
re-expands the full body every time. sqlite's `Mem`/`sqlite3_value` struct
(the `vdbe.c` trigger, reachable under both names via the
`typedef struct sqlite3_value Mem;` alias) is reachable via many
independent, non-cyclic pointer chains — `sqlite3`, `Vdbe`, `Parse`,
`sqlite3_context`, `UnpackedRecord`, etc. — so every distinct path re-prints
the full struct body, and total printed output size grows combinatorially
with the number of reachability paths, not with the type count.

Minimal isolation (via a real, minutes-scale 449-line `vdbe.c` prefix, plus
control variants): not file position (a no-op function at the same point is
fine), not mutexes (`-DSQLITE_THREADSAFE=0` still hangs), not "any call" (a
call to a trivial self-contained helper is fine). The actual trigger is
calling a function whose parameter type is `sqlite3_value*` (the
already-used-internally `Mem` struct under a second public typedef name)
once ~428 lines of real header-driven declarations have accumulated in the
TU — a minimal 16-line standalone version of just the two-typedef-names
pattern does **not** reproduce; it needs real header-scale accumulated
context.

## Fix attempted, then judged insufficient for the general case

Patched `mlir/lib/IR/AsmPrinter.cpp`'s `AliasState` to mark a type's alias
"printed" at first encounter anywhere (any nested reference), not only after
fully printing that type's own designated top-level slot. This fully fixed
the case actually blocking `slate-wcf7`:

- `/tmp/vdbe_449.i` reproducer: 52s `bad_alloc` → 25ms success
- `~/sqlite/src/vdbe.c` (9574 lines, previously independently pathological):
  0.2s, 35855 CIR lines, 373 `cir.func` defs
- `~/sqlite/bld/sqlite3.c` (full 140456-line amalgamation, the original
  `slate-wcf7` blocker): 2.05s under a 10GB `ulimit -v` cap (previously:
  unbounded, never finished)

But the general case doesn't reduce to a printer tweak. MLIR's textual alias
syntax only allows defining `!name = body` as a standalone **top-level**
line, never mid-expression. sqlite's core types (`Mem`, `Expr`, `Select`,
`Table`, `SrcList`, `AggInfo_func`, ...) form one large **mutually-recursive
cluster** (SCC) — any type in that cluster reached inline before its own
top-level slot prints must be fully re-expanded regardless of the alias
table, since there's no forward-reference syntax for it. Deduping that would
need string-level output caching (buffer + splice), a materially bigger
change than a printer tweak. Three escalating fixes were tried and rejected:

1. **Mark-on-first-touch** — fast, but produces invalid forward alias
   references for self/mutually-recursive structs (`rec_Savepoint`,
   `rec_Expr`).
2. **Gate marking by cyclic-vs-acyclic** — fixes validity, but disables
   blowup protection for exactly the huge cyclic cluster that matters, most
   of the original blowup.
3. **Tarjan's SCC with root-first ordering** — reduces but doesn't eliminate
   it; the 449-line reproducer still hits `bad_alloc` at 95s+ under a 12GB
   cap.

## Conclusion

Stopped here — this is an upstream ClangIR/MLIR limitation, not something to
solve inside slate. **Per-file translation is the working fallback** for
large, type-recursive single-TU inputs like sqlite's amalgamated build
(translate `src/*.c` individually rather than the amalgamation), with the
caveat that individual files can still be independently pathological
(`vdbe.c`) if they happen to pull in enough of the same recursive-struct
cluster. Minimal-reproducer work continues via `cvise` toward an upstream bug
report; two prior `cvise` runs falsely converged because the
interestingness test's signal (a timing threshold, then an artificially
tight `ulimit -v` cap) was weaker than the real bug — both required an
actual timeout/`bad_alloc` under a generous (8GB) cap to fix. Filed under
`slate-wcf7.2`.
