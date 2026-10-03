# long double (f80) representation

<!-- toc -->
- [Slate frontend](#slate-frontend)
- [Why not just `f64`](#why-not-just-f64)
- [ABI varies by target —
  `TargetInfo::long_double`](#abi-varies-by-target--targetinfolong_double)
- [Integration points](#integration-points)
- [History](#history)
<!-- /toc -->

> The `LongDouble` prelude, the `__slate_f80_*` shim declarations, and the
> bridge type tags live in `frontend/long_double.rs`.

## Slate frontend

`frontend/lowerer.rs` lowers IR `f80` to `LongDouble` and emits
the prelude when any f80 type is lowered. Constants become
`LongDouble([10 bytes])` straight from `Number::FloatBits`. Arithmetic,
negation, and comparisons use the prelude's operator impls. Every `Convert`
with an f80 side calls `__slate_f80_from_<t>` / `__slate_f80_to_<t>`.

A call to a body-less function whose return or argument holds f80 by value
(including inside a struct) becomes a call to a C bridge named
`__slate_<callee>__r<ret>_<arg tags>`. The test harness renders it from the
name (`frontend::c_shim::render_shim_c_source_for_names`) and links it with
`shims/long_double.c`, and the direct extern declaration is dropped. Barriers
remain for variadic callees missing from `function_identity::Known` (the
bridge needs their header), and for non-pointer aggregate arguments to a
bridge. Non-variadic bridged callees depend on `long_double.c` including
`<math.h>`. A callee with no known header (a project function such as redis
`ld2string`, or libc `qecvt`) is declared as
`extern <native ret> __slate_extern_<callee>(<native params>) __asm__("<callee>")`
with `long double` for f80 tags, and the bridge calls that alias. The private
name cannot conflict with a header declaration whose integer spellings
(`long` vs `long long`) differ from the tags; known-header callees keep the
header so glibc redirects such as `__isoc23_strtold` still apply
(`long_double_unknown_extern.c`).

Variadic `long double` arguments must reach the callee as real x87 values in
memory; Rust would pass `LongDouble` as an INTEGER-class struct. A call to a
Rust-defined variadic function with f80 variadic arguments becomes a call to
`__slate_vcall__r<ret>_<fixed tags>__<variadic tags>(callee as *const (), ...)`.
The C trampoline (`c_shim::render_variadic_trampoline`) calls the pointer as
`<ret> (*)(<fixed>, ...)` with fixed f80 parameters and the return kept as
`__slate_f80`, and loads only the variadic f80 arguments into `long double`.
The callee is passed by pointer rather than exported by symbol, so static
functions with the same name in different TUs, or named like a libc or Rust
runtime symbol (`exit`), never collide, and trampolines are shared per
signature. On the callee side, `va_arg(ap, long double)` lowers to
`__slate_f80_va_arg(&mut ap)`, which relies on x86_64 `VaList` sharing C's
`__va_list_tag` layout so C advances the Rust list in place.

- Runtime and name-based bridges: `frontend/c_shim.rs` and
  `frontend/shims/{long_double,fenv}.c`.
- Generated crates compile `src/slate_long_double.c` with `cc` in `build.rs`.

## Why not just `f64`

C `long double` and `double` are not interchangeable on the targets slate
cares about most: on Linux x86_64, `long double` is genuinely 80-bit x87
extended precision (10 bytes of value, padded to 16-byte alignment), with
different rounding/precision behavior than `f64`. Silently widening it to
`f64` would pass slate's differential tests on trivial cases and diverge on
anything precision-sensitive. So slate models it as its own type,
`LongDouble` (`frontend/long_double.rs::LONG_DOUBLE_TY`), backed
by a `[u8; 10]` byte representation, with every x87 operation delegated to C
helpers in `frontend/shims/long_double.c` since Rust has no native 80-bit
float type.

## ABI varies by target — `TargetInfo::long_double`

`long double`'s size/alignment is target-dependent, not just a slate
implementation detail:

- Linux x86_64: 80-bit value, 16-byte size, 16-byte alignment (x87 extended).
- 32-bit ARM Linux: ABI-identical to `double` — 8-byte size and alignment.
- macOS and MSVC targets: `long double` is ABI-identical to `double` — 8-byte
  size and alignment.

The IR module's `TargetInfo::long_double` identifies the long-double format.
Slate consumes the resulting IR type directly: binary64 lowers as `f64`, while
x87 lowers as `LongDouble` with the C runtime bridges.

## Integration points

| Concern | Source under `crates/slate/src` |
| --- | --- |
| Prelude and bridge type tags | `frontend/long_double.rs` |
| Conversion and call lowering | `frontend/lowerer/f80.rs`, `frontend/lowerer/calls.rs` |
| Name-based bridge rendering | `frontend/c_shim.rs` |
| Runtime implementation | `frontend/shims/{long_double,fenv}.c` |

Current coverage comes from differential fixtures. A target format or source
construct being parsed does not establish runtime support.

## History

Earlier ABI and lowering records are available from the
[historical index](../historical/index.md).
