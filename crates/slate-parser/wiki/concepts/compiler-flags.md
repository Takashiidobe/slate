# Compiler flags

Each supported flag emulates the same flag of the selected flavor's
compiler. Parsing and validation live in
[compiler-arg-rules](compiler-arg-rules.md), and how the result reaches
each stage is in [configuration-threading](configuration-threading.md).

## Contract

- Flags are inputs to sema. The IR carries their resolved effects (an
  operation's `overflow=`, a layout, `long_double`, symbol definition
  kinds), never the flags themselves. Slate never reads flags.
- Predefines follow the same effective configuration that sema uses.
- Optional permissions (`contract=`, `range=`) are recorded when Slate can
  use them. Ignoring them is always correct.
- The IR records the rounding and exception behavior an operation
  requires. Proving the runtime FP mode and lifting to plain Rust
  arithmetic is the Rust rewriter's job, not the IR's.
- The MSVC flavor rejects every `-f…` and `-m…` flag below
  (`msvc_rules`). Its own `/` spellings are not modeled.

## Implemented

| Flag | Emulates | Effect |
| --- | --- | --- |
| `-D` / `-U` / `-include` / `-imacros` | gcc/clang | [compiler-arg-rules](compiler-arg-rules.md) |
| `-I` / `-iquote` / `-isystem` / `-idirafter` / `-nostdlibinc` / `-isysroot` / `--sysroot` | gcc/clang | include search |
| `-std=` | gcc/clang | `LanguageStandard` → `StandardFeatures`, `__STDC_VERSION__` |
| `-target` | clang | selects `TargetSpec` ([adding-a-target](adding-a-target.md)) |
| `-fwrapv` / `-ftrapv` / `-fstrict-overflow` (and `-fno-`) | gcc, clang | signed `overflow=wrap\|trap\|ub`. gcc: the last active one wins. clang: an active `-ftrapv` beats `-fwrapv` |
| `-frounding-math` | gcc, clang | `rounding=environment`; gcc also defines `__ROUNDING_MATH__` |
| `-ftrapping-math` | gcc, clang | `exceptions=`. Default is observable on gcc, ignored on clang |
| `-fgnu89-inline` | gcc, clang | inline definition semantics, `__GNUC_GNU_INLINE__` |
| `-fcommon` | gcc, clang | tentative definitions become common symbols |
| `-mlong-double-64\|80\|128` | gcc, clang | `TargetInfo.long_double` and its predefines; x86 only |
| `-mpreferred-stack-boundary` | gcc | stack alignment ABI; x86, exponent from 4 (x86_64) or 2 (x86) up to 12 |
| `-mstack-alignment` | clang | stack alignment ABI; power of two; exclusive with the gcc form |
| `-masm=att\|intel` | gcc, clang | GNU asm dialect; gcc rejects it off x86 |
| `-m<feature>` / `-mno-<feature>` / `-march=` | gcc, clang | `TargetIsa`, which drives ISA predefines and ABI vector width |
| `-mfpu=` / `-mfloat-abi=` / `-mthumb` / `-marm` | gcc, clang | Arm32 ISA and float ABI (`aapcs32` vs `aapcs32_hard_float`) |
| `-msve-vector-bits=` | gcc, clang | AArch64 SVE macros |
| `-W…` / `-pedantic` | gcc, clang | severities ([diagnostic-severity](diagnostic-severity.md)) |

ISA values are checked against the `-dM` output of both oracles. The
tables live in `src/target/{x86,aarch64,arm}_isa.rs`. Where gcc and clang
resolve the same string differently, each file has per-flavor rules.

## ISA gotchas

- x86: `-march` sets the base, then `-m` flags apply in order. Enabling a
  feature enables what it implies; disabling one disables whatever implies
  it. `-mfoo … -mno-foo` drops `foo` entirely.
- x86: clang's avx512f implies AVX2, FMA, and F16C; gcc's implies only
  AVX2. In gcc, AVX → XSAVE is a real edge.
- x86: disabling SSE or SSE2 is rejected, because it would change the
  float ABI.
- i686 without `-march`: clang defaults to pentium4 and gcc to x86-64
  (from the multilib snapshot). Both have the same features.
- gcc sets `__BIGGEST_ALIGNMENT__` to the widest enabled vector register;
  clang keeps 16.
- AArch64: enable and disable are not symmetric. Each feature has its own
  `enables`/`disables` table, and gcc and clang differ on
  `+nosimd`/`+nofp`/`+crypto`.
- Arm32: the default float ABI comes from the triple (`gnueabihf` → hard).
  `thumbv7a-pc-windows-msvc` is always hard-float and Thumb-only.

## Not implemented

Add these when a sweep or corpus needs them, emulating the flag of the
same name:

- Layout: `-funsigned-char`, `-fshort-enums`, `-fshort-wchar`, `-fpack-struct`.
- Language: `-fms-extensions`, `-fdollars-in-identifiers`, `-fpermissive`,
  `-ffreestanding`, `-fno-builtin`.
- Floating point: the fast-math family (`nnan`, `ninf`, `nsz`, `arcp`,
  `reassoc`, `afn` have no IR field yet).
- Pointers: `-fno-delete-null-pointer-checks` needs a null-access
  contract. The pointer wrapping implied by `-fno-strict-overflow` is
  stored but not yet applied.
- Scoped overrides: `__attribute__((optimize))` does not change operation
  contracts yet. `#pragma float_control`, `STDC FP_CONTRACT`, and
  `FENV_ACCESS` do (`sema/pragmas.rs`).
