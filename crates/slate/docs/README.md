# slate

`slate` translates C to Rust by lowering **ClangIR (CIR)** — Clang's MLIR-based
IR — rather than LLVM IR. CIR is high enough to retain structured control flow,
integer signedness, and named local variables, so this is _transpilation_, not
decompilation. (See [architecture.md](architecture.md) for why the IR level is
the whole game.)

## Approach in one line

**Transliterate first, idiomatize later.** Baseline lowering emits the most
faithful Rust it can — `unsafe`, `libc`-backed, temp-heavy — and correctness is
the only bar, checked by differential testing. Idiomatic, safer Rust is then
recovered by an independently-verified ladder of fixup passes (see
[idiomatization.md](idiomatization.md)); each fixup is optional in spirit, so
disabling any one of them still leaves correct Rust.

## Current state

Correctness is verified by **differential testing**: compile and run both the
original C and the generated Rust, and require identical stdout and exit code.
This section is a categorized summary of what baseline lowering and the fixup
ladder currently cover; it is not exhaustive. For the authoritative, exhaustive
surface:

- `tests/fixtures/*.c` — one fixture per supported idea, checked by
  `cargo nextest r --release --test differential`.
- `tests/stdlib/<header>/*.c` — one probe per libc function, checked by
  `cargo nextest r --release --test stdlib_coverage`.
- [cir-op-prioritization.md](cir-op-prioritization.md) — the exhaustive CIR op
  checklist (what is and isn't lowered).
- `bd list --status=open` — tracked gaps and in-flight idiomatization work.

### Scalars, operators, and casts

`int`/`char`/`short`/`long`/`long long` (signed and unsigned) by CIR width,
`_Bool`/`bool`, `float`/`double`, `long double` (ABI-exact `#[repr(C, align(16))]`
newtype, `f64`-precision arithmetic), IEEE binary128 via Clang's `__float128`
spelling (native nightly Rust `f128`), and `float`/`double _Complex` (`#[repr(C)]`
newtype, libgcc-backed `*`/`/`). The standard C23 `_Float128` spelling follows
the same path when the configured CIR-enabled Clang accepts it. Full arithmetic
(`+ - * / %`), bitwise
(`& | ^ ~ << >>`), logical (`&& || !`), comparisons, compound assignment,
increment, unary negation, and explicit casts across all of the above. Integer
overflow wraps two's-complement on both sides (`overflow-checks = false`)
instead of panicking on the Rust side. `<stdint.h>`/`<stddef.h>` fixed-width
typedefs and `typedef` aliases generally, resolved through Clang's desugared
type facts.

### Control flow

`if`/`else`/`else if`, `for`, `while`, `do while`, `switch` (sparse cases,
fallthrough, missing defaults, multiple labels), `break`/`continue` (including
nested loops and `for`'s step-region `continue` semantics), and `goto`/labels.
Structured CIR lowers directly; unstructured jumps (`cir.goto`/`cir.label`/
`cir.br` across blocks) lower to a conservative dispatch-loop state machine,
which a later fixup restructures back into ordinary `if`/`loop`/sequence Rust
when the state graph is reducible enough (see [passes.md](passes.md)).

### Aggregates

Structs and unions, including nested aggregates (structs of structs/arrays,
arrays of structs), member/element access composed into a single Rust place
(`b.data[i]`, `ps[i].x`), struct assignment via derived `Copy`, bitfields
(per-field mask/sign-extend storage), and aggregate initializers (nested,
partial with zero-filled trailing fields, and designated). Enum constants
lower to Rust integer `const` items.

### Pointers and arrays

Address-of, dereference, pointer arithmetic, array-to-pointer decay, and
function pointers (`Option<fn(...) -> ...>`), all raw on the baseline path.
Fixups recover safer shapes where facts prove it's sound: a pointer+length
parameter pair becomes a `&[T]`/`&mut [T]` slice parameter, raw index
arithmetic over a lifted slice becomes `slice[i]`, and a 0-start/step-1 loop
that only indexes one slice becomes `for x in slice.iter()`/`.iter_mut()`.

### Globals and linkage

File-scope primitive and aggregate globals with internal or external linkage,
zero-initialized definitions, `extern` declarations (cross-translation-unit
imports), `static` locals, and C symbol aliases/visibility attributes (see
[idiomatization.md](idiomatization.md)).

### Strings and libc idiomatization

NUL-terminated char-array/pointer buffers get lifted to `CStr`/`str`/byte-slice
values where provenance is provable, `strlen`/`strcmp`-family calls on lifted
strings become native Rust equivalents, `strcpy`/`strcat`-only fixed buffers
become owned `String`, and a C-string pointer parameter can become `&str`.
`malloc`/`calloc`/`realloc`/`free` locals become owned `Box<T>`/`Vec<T>`.
`fopen`/`fputs`/`fclose` sequences become `File`/`OpenOptions` owners.
`qsort`/`bsearch` become `.sort_by()`/`.binary_search_by()`. `printf`-family
calls with a constant, fully-supported format string become `println!`/
`print!`; anything else stays `libc::printf`.

### Atomics and concurrency builtins

`_Atomic` locals proven non-escaping get native `std::sync::atomic::AtomicN`
storage with safe method calls (int/bool; float/pointer fall back to
non-atomic RMW); CAS temp-chains fold into a single `compare_exchange` match.

### Varargs, attributes, and builtins

`va_list`/`va_start`/`va_arg`/`va_end`. Constructor/destructor priority
attributes, `noreturn` (both spellings), inline `asm`, computed goto,
`__builtin_assume`/`__builtin_expect` and other control-flow builtins, target/
frame/stack builtins, vector extensions (`cir.vec.*`), and the byte-wise memory
builtins (`memcpy`/`memmove`/`memset`/`memchr`) — see
[cir-op-prioritization.md](cir-op-prioritization.md) for the full op-level
checklist. `nonnull`/`returns_nonnull` (including `T name[static N]`
parameters) are recovered as facts too.

### Multi-config translation

`slate translate-directives` recovers `#if`/`#ifdef`-gated portability that a single
Clang invocation can't see, mapping predicates to Rust `cfg`s and Cargo
features. See [cfg-portability.md](cfg-portability.md).

Generated Rust for inspection is written with:

```bash
cargo run -- emit-fixtures
```

That command translates every currently supported fixture suite into an ignored
sibling `*.generated/` tree while preserving its relative layout. This includes
the primary, cfg, multi-TU, supported chibicc, library, and stdlib fixtures;
explicit reject/unsupported cases are skipped. The checked fixture trees remain
C-only. To inspect raw lowered Rust before fixups, use
`cargo run -- emit-lowered-fixtures`; it writes ignored files under
`tests/fixtures.lowered.generated/`.

## Not handled yet

Tracked as beads (`bd list --status=open`), not maintained here, so this list
doesn't rot. As of this writing, open gaps are mostly about widening the
idiomatization ladder rather than baseline C coverage — e.g. fully
target-complete scalar modeling, `setjmp`/`longjmp`, remaining printf
edge cases (precision/width forms), further libc idiomatization (`fgets`/
`fread`/`fwrite` on owned `FILE` handles), `alloc_size`-driven pointer facts,
and `enumerate()` recovery for slice loops with a live index use.

## Pipeline

```
C ──emit──► CIR ──parse──► Op-tree ──lower──► Rust source
│  clang|cir-opt                    ▲
└──ast-dump=json──────► Clang AST ──┘

verified:  run(C).{stdout,exit}  ==  run(Rust).{stdout,exit}
```

## Three sources

Every C input is available to the translator in three forms, joined by source
location (`file:line:col`):

- **CIR** — the primary lowering source.
- **Clang AST** — loaded from `clang -Xclang -ast-dump=json -fsyntax-only` and
  extracted into structured source context, with raw JSON retained.
- **C source text** — for comments and naming during final readability polish.

## Docs

- [adding-features.md](adding-features.md) — how to add C coverage or a Rust
  fixup.
- [writing-a-fixup.md](writing-a-fixup.md) — the AST-to-AST recipe for a fixup
  pass: shape, shared helpers, safety rules, and registration.
- [effects.md](effects.md) — how to extend the Rust-to-Rust effects interpreter
  used to validate that fixups preserve semantic behavior.
- [architecture.md](architecture.md) — sources, IRs, pipeline, shared context.
- [passes.md](passes.md) — the pass catalog: what runs, in what order, how.
- [facts.md](facts.md) — the `FixupFacts` analysis layer: what each collector
  proves and which rewrite pass consumes it.
- [idiomatization.md](idiomatization.md) — the `unsafe`/`libc` → idiomatic
  ladder and its current implementation status.
- [fuzzing.md](fuzzing.md) — the stateful C-subset generator behind
  differential fuzzing.
- [cfg-portability.md](cfg-portability.md) — single-config vs. multi-config
  translation and the preprocessor predicate → Rust `cfg` mappings.
- [cir-op-prioritization.md](cir-op-prioritization.md) — the exhaustive CIR op
  support checklist.
- [macro-selection-interface.md](macro-selection-interface.md) — design (not
  yet implemented) for recovering value-carrying preprocessor predicates.

## Toolchain

Requires a CIR-enabled Clang (`CLANG_ENABLE_CIR=ON`). Local build lives at
`~/llvm-project/build-cir/bin/{clang,cir-opt}`; overridable via `SLATE_CLANG`
and `SLATE_CIR_OPT`.

Target selection can be shared across the CIR and AST Clang invocations with
`SLATE_TARGET=<triple>` and extra flags in `SLATE_CLANG_ARGS`.
