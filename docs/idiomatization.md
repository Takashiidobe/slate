# Idiomatization

V0 produces faithful but ugly Rust: `unsafe`, `libc` calls, raw pointers,
mechanical temps. Idiomatic safe Rust is recovered afterward by a **ladder** of
fixups. A fixup is different from a lowering feature:

- a **feature** expands which C programs baseline Slate can translate;
- a **fixup** rewrites already-correct generated Rust into better Rust.

Each rung:

- is an independent pass, verified by the differential harness (semantics must
  not change);
- is optional — you can stop on any rung and still have valid, correct Rust;
- may consult the **Clang AST / source oracle** when CIR alone lacks the needed
  information (this is what justifies the three-source design — see
  [architecture.md](architecture.md)).

Cleanup passes live under `src/fixups/` and are wired through a fixed post-lower
entry point. Baseline lowering owns correctness; fixups own readability.

Standard-library semantics require a `Known` function identity. Slate derives
that identity from the Clang declaration reached by the call, its canonical
function type, and plugin evidence that the declaration came through the
corresponding trusted system header. A matching spelling alone is never enough.
Project declarations, shadowing definitions, indirect calls, and declarations
from similarly named project headers remain ordinary calls throughout lowering,
fixups, and effects interpretation.

Clang's reserved `__builtin_*` memory operations are also `Known`: their
semantics come from the compiler rather than a C header. CIR operations created
directly for language intrinsics, such as `__builtin_bit_cast`, carry the same
compiler-defined identity without pretending to be libc declarations.

Runtime symbol interposition such as `LD_PRELOAD` or link-time replacement of a
proven standard-library symbol is outside Slate's supported semantics. Those
mechanisms intentionally change the program after translation and cannot be
reconciled with source-level idiomatization.

The ladder is ordered by value-for-effort, not dependency; most rungs are
independent.

## Rung 0 — baseline (V0)

Faithful transliteration. Every C construct maps to the closest mechanical Rust,
using `libc` + `unsafe` as the escape hatch.

```rust
// C: printf("%d\n", add(2, 3));
let _v0 = add(2, 3);
unsafe { libc::printf(b"%d\n\0".as_ptr() as *const libc::c_char, _v0); }
```

Correct, compiles, matches C output. Nobody wants to read it. That's fine.

### C Symbol Aliases

Function aliases from `__attribute__((alias("target")))` lower as exported C ABI
forwarding wrappers. This preserves call behavior, but it is not a true LLVM-style
symbol alias.

Global/static aliases are diagnosed as unsupported. Rust has no faithful static
aliasing construct here; emitting a second `static` would duplicate storage and
break address identity.

### C Symbol Visibility

`__attribute__((visibility("hidden")))` lowers by keeping the generated item
Rust-private and omitting `#[unsafe(no_mangle)]`. This preserves in-translation
unit behavior while avoiding an exported symbol.

`__attribute__((visibility("protected")))` has no faithful Rust representation
today. Slate emits a warning and falls back to default exported visibility.

## Rung 1 — `printf`-family → format macros

Recognize `libc::printf(fmt, args)` where `fmt` is a recovered constant C string,
parse the format specifiers, and emit a native macro. Removes `unsafe` + `libc`
for the common proven-safe case.

```rust
// after:
println!("{}", add(2, 3));
```

Mechanics: match the call, recover the constant format string, map specifiers
(`%d %i %u %ld` → `{}`; `%x` → `{:x}`; width/precision/flags → Rust format spec;
`%%` → `%`; literal `{`/`}` → `{{`/`}}`). Non-constant format strings have no
literal to feed a macro, so they **stay on rung 0** (`libc::printf`) — the
fallback is always available. Applies to `fprintf`, `sprintf`, `puts`,
`putchar`, etc., each a small recognizer.

The rule is strict: unsupported format features, dynamic format strings,
locale-sensitive behavior, or ambiguous argument typing must stay on the baseline
`libc` path.

## Rung 2 — mechanical cleanup

Pure readability, no semantic risk:

- **collapse-retval**: `__retval = e; return __retval` → `return e`.
- **inline-temps**: fold single-use materialized temps back into their use site.
- **dead-let**: drop `let mut x = 0;` that is overwritten before any read.
- **param/alloca fusion**: `fn f(arg0){ let mut a = arg0; …a… }` → use the param
  directly.

```rust
// before:               // after:
fn add(arg0: i32, arg1: i32) -> i32 {   fn add(a: i32, b: i32) -> i32 {
    let mut a = arg0;
    let mut b = arg1;
    let mut c = 0;
    c = a + b;                               a + b
    return c;
}                                        }
```

## Rung 3 — pointers → references

Raw `*mut T` / `*const T` params and locals become `&mut T` / `&T` / slices where
aliasing analysis (and the AST's original `T[]` vs `T*` distinction) permits.
This is the first rung that materially needs the **AST oracle** and real
analysis; it is where "safe Rust" actually begins.

## Rung 4 — ownership

`malloc`/`calloc`/`free` → `Box`/`Vec`; `realloc` → `Vec` growth; C strings →
`String` / `&str` with `CStr` at the boundary. Removes the remaining `unsafe`
around heap management.

## Rung 5 — control-flow idioms

Counting `while` loops recovered to `for i in a..b`; the AST's original `for`
structure disambiguates the induction variable and bounds.

```rust
// before:                    // after:
let mut i = 1;                for i in 1..=n {
while i <= n {                    total += i;
    total += i;               }
    i += 1;
}
```

## Rung 6 — naming & comments

Final polish: recover meaningful identifier names and re-attach comments from the
C **source text** (the third source), which neither CIR nor the AST preserves by
default.

---

Each rung narrows the gap between "compiles and behaves like the C" and "reads
like something a Rust programmer wrote." V0 ships rung 0; rung 1 is the first
follow-up.

## Fixup checklist

Use this checklist for every idiom pass:

1. Generate Rust from a C fixture that already passes differential testing.
2. Add a test for the rewritten output shape and keep the differential test
   green.
3. Match only the narrow pattern you can prove.
4. Preserve a fallback to baseline Rust.
5. Keep the pass independent; earlier rungs should not be required unless the
   dependency is explicit and tested.
