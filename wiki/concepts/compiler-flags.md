# Compiler flags

<!-- toc -->
- [Contract](#contract)
- [Implemented](#implemented)
- [ISA gotchas](#isa-gotchas)
- [MS modes](#ms-modes)
- [Not implemented](#not-implemented)
<!-- /toc -->

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
| `-nostdinc` | gcc, clang, msvc | drops compiler headers and the sysroot from include search; removes the glibc `stdc-predef.h` macros (`__STDC_IEC_559__`, `__STDC_ISO_10646__`, ...) baked into snapshots |
| `-ffreestanding` / `-fhosted` | gcc, clang | last wins; `-fno-` forms are gcc only. Freestanding: `__STDC_HOSTED__ 0`, no `stdc-predef.h` macros, library builtins off even after `-fbuiltin`; clang also defaults `-fno-asynchronous-unwind-tables` |
| `-fno-builtin` / `-fbuiltin` / `-fno-builtin-<name>` | gcc, clang | `CompilerOptions::library_builtins`: library-kind builtins (`memcpy`, `abs`, ...) lose their builtin meaning; `__builtin_` spellings are unaffected |
| `-fasynchronous-unwind-tables` (and `-fno-`) | gcc, clang | `__GCC_HAVE_DWARF2_CFI_ASM` |
| `-funsigned-char` / `-fsigned-char` (and `-fno-`) | gcc, clang | `TargetInfo::char_signed`, `__CHAR_UNSIGNED__`, `_CHAR_UNSIGNED` on msvc environments |
| `-fshort-wchar` (and `-fno-`) | gcc, clang | `wchar_t` is `unsigned short`, plus the `__WCHAR_*`, `__SIZEOF_WCHAR_T__` and wide-encoding macros. `-fno-short-wchar` on windows-msvc makes it `int` |
| `-fms-anonymous-structs` (and `-fno-`) | clang | [MS modes](#ms-modes): a declarator-less member of record type is an anonymous member |
| `-fstrict-flex-arrays=0..3` | gcc, clang | `CompilerOptions::strict_flex_arrays`, with no consumer yet. The bare and `-fno-` forms (3 and 0) are gcc only |
| `-fexperimental-late-parse-attributes` | clang | `CompilerOptions::late_parsed_attributes`; `counted_by` arguments are not resolved yet, so it changes nothing |
| `-std=` | gcc/clang | `LanguageStandard` → `StandardFeatures`, `__STDC_VERSION__` |
| `-O` / `-O0`..`-O3` / `-Os` / `-Oz` / `-Og` | gcc, clang | predefines only; the last one wins. Above `-O0`: undefines `__NO_INLINE__`, defines `__OPTIMIZE__`, and `-Os`/`-Oz` also define `__OPTIMIZE_SIZE__`. `-Ofast` is rejected (fast-math is not emulated); msvc rejects all |
| `-target` | clang | selects `TargetSpec` ([adding-a-target](adding-a-target.md)) |
| `-fwrapv` / `-ftrapv` / `-fstrict-overflow` (and `-fno-`) | gcc, clang | signed `overflow=wrap\|trap\|ub`. gcc: the last active one wins. clang: an active `-ftrapv` beats `-fwrapv` |
| `-frounding-math` | gcc, clang | `rounding=environment`; gcc also defines `__ROUNDING_MATH__` |
| `-ftrapping-math` | gcc, clang | `exceptions=`. Default is observable on gcc, ignored on clang |
| `-fgnu89-inline` | gcc, clang | inline definition semantics, `__GNUC_GNU_INLINE__` |
| `-fcommon` | gcc, clang | tentative definitions become common symbols |
| `-fms-extensions` / `-fms-compatibility` (and `-fno-`) | clang | [MS modes](#ms-modes); gcc and msvc reject them |
| `-fasm-blocks` / `-fno-asm-blocks` | clang | MS `__asm` blocks on x86 without the rest of MS mode; last wins, MS extensions enable them regardless (`MicrosoftFlags::asm_blocks`) |
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

x86 features and CPUs are generated into `src/target/x86_isa_tables.rs`
([generated-sources](generated-sources.md)): every `-m<feature>` in clang's
`m_x86_Features_Group` that `X86.td` defines, and every `X86.td` CPU clang
accepts. `tools/x86_isa_diff.py` compares slate's predefines with clang's
for each CPU, each flag, and each `-mno-` flag on `diamondrapids`, on
x86_64 and i686; it fails on wrong macros or on accepting what clang
rejects, and lists what slate rejects. Rejected on purpose or not yet
modeled:

- clang-only CPU aliases (`corei7`, `skx`, `core-avx2`, `atom`, `slm`,
  `athlon64`, ...), which live in `X86TargetParser.cpp`, not `X86.td`;
- `-mapxf` and `-mvzeroupper`, which have no `X86.td` feature;
- 32-bit CPUs without SSE2 (disabling SSE2 is unsupported);
- gcc flavor: only the `x86-64` levels and the original 26 features
  (`GCC_FEATURES` in `x86_isa.rs`); other names are rejected as
  clang-only;
- `-march=<cpu>` without 64-bit support on x86_64, and 64-bit-only
  features such as `-muintr` on i686, as clang rejects them.

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

## MS modes

clang only (clang 22, `-###` and `-dM -E`). msvc flavor: always both on.
gcc's `-fms-extensions` is a different feature; not emulated.

- Default: both on for `*-windows-msvc`, off elsewhere.
- `-fms-compatibility` implies extensions. `-fno-ms-extensions` drops
  default compatibility, not an explicit `-fms-compatibility`.
- Slate: `StandardFeatures::{microsoft_extensions,
  microsoft_compatibility}`. Snapshots hold the target default;
  `<microsoft modes>` applies the delta.

| Follows | Effect |
| --- | --- |
| compatibility | predeclared `size_t` (`Sema::Initialize`) |
| compatibility off | `__GNUC__` 4.2.1, `__GNUC_STDC_INLINE__`, `__GXX_ABI_VERSION 1002`, `__STDC__`, `__GCC_ATOMIC_*` (= `__CLANG_ATOMIC_*`, plus `TEST_AND_SET_TRUEVAL 1`) |
| effective extensions | MS keywords, x86 `__asm {}`, `_MSC_EXTENSIONS` (Windows), `_M_IX86_FP 2` (i686) |
| requested extensions (Windows) | `_MSC_VER 1933`, `_MSC_FULL_VER`, `_MSC_BUILD`, `_MSVC_CONSTEXPR_ATTRIBUTE`, `_CRT_USE_BUILTIN_OFFSETOF` |
| target ABI, not flags | inline-definition visibility, object requests |
| last of `-f[no-]ms-anonymous-structs`, `-f[no-]ms-extensions`, `-fms-compatibility`; else extensions | `StandardFeatures::microsoft_anonymous_records`: `struct tag;`, a typedef'd record, or a named inline definition without a declarator becomes an anonymous member (`define_new_tag`) |

## Not implemented

Add these when a sweep or corpus needs them, emulating the flag of the
same name:

- Layout: `-fshort-enums`, `-fpack-struct`.
- Codegen: `-fPIC` / `-fpic`; their PIC predefines are not modeled by these flags.
- Language: gcc's `-fms-extensions`, `-fdollars-in-identifiers`, `-fpermissive`.
- Floating point: the fast-math family (`nnan`, `ninf`, `nsz`, `arcp`,
  `reassoc`, `afn` have no IR field yet).
- Pointers: `-fno-delete-null-pointer-checks` needs a null-access
  contract. The pointer wrapping implied by `-fno-strict-overflow` is
  stored but not yet applied.
- Scoped overrides: `__attribute__((optimize))` does not change operation
  contracts yet. `#pragma float_control`, `STDC FP_CONTRACT`, and
  `FENV_ACCESS` do (`sema/pragmas.rs`).
