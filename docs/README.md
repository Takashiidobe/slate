# slate

`slate` translates C to Rust by lowering **ClangIR (CIR)** — Clang's MLIR-based
IR — rather than LLVM IR. CIR is high enough to retain structured control flow,
integer signedness, and named local variables, so this is _transpilation_, not
decompilation. (See [architecture.md](architecture.md) for more details)

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
- `bd list --status=open` - tracked gaps and in-flight idiomatization work.
- `bd list --status=blocked` - blocked tasks by upstream clang IR or
  rust.

Generated Rust trees are ignored inspection artifacts and are not regenerated
as part of feature work or completion. When explicitly requested for manual
inspection, they can be written with:

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
`fread`/`fwrite` on owned `FILE` handles), consumption of allocation metadata
by the fact passes, and `enumerate()` recovery for slice loops with a live
index use.

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

- [fixups.md](fixups.md) — how to state
  query-driven rewrite cases, proofs, typed recipes, definition lifecycles,
  scheduling, and tracing.
- [architecture.md](architecture.md) — sources, IRs, pipeline, shared context.
- [passes.md](passes.md) — the pass catalog: what runs, in what order, how.
- [facts.md](facts.md) — the salsa-memoized facts analysis layer: what each
  collector proves and which rewrite pass consumes it.

## Toolchain

Requires a CIR-enabled Clang (`CLANG_ENABLE_CIR=ON`). Local build lives at
`~/llvm-project/build-cir/bin/{clang,cir-opt}`; overridable via `SLATE_CLANG`
and `SLATE_CIR_OPT`.

Target selection can be shared across the CIR and AST Clang invocations with
`SLATE_TARGET=<triple>` and extra flags in `SLATE_CLANG_ARGS`. Android targets
also require `SLATE_ANDROID_API=<level>`; the 64-bit Bionic baseline starts at
API 21. `SLATE_TARGET=aarch64-apple-darwin` selects the narrow AArch64 macOS
profile at the macOS 11.0 deployment baseline.

`translate-project <dir> <out_dir>` lowers only the active `SLATE_TARGET`, or
Slate's own build target when that variable is unset. Repeatable
`--target <triple>` options add cfg-gated project variants; each triple selects
its architecture, endianness, libc environment, operating system, vendor, and
pointer width as one ABI configuration.

Library projects can select their configured translation units explicitly:

```bash
cargo run -- translate-project --lib --source-manifest sources.txt project crate
```

The manifest contains one `.c` path per line, resolved relative to the project
directory. Blank lines and lines beginning with `#` are ignored. Explicit
source mode translates only those files and does not auto-discover the
project's `tests/` directory, so included `.c` fragments and inactive platform
backends are not treated as standalone translation units.

The external pinned Microsoft CRT and UCRT header oracle used for MSVC work is
bootstrapped under `target/`; see [msvc-sysroot.md](msvc-sysroot.md).

The macOS ABI oracle uses an externally installed SDK plus pinned Apple public
Libc and XNU sources under `target/`; see
[macos-sdk-oracle.md](macos-sdk-oracle.md).

The Android Bionic ABI oracle uses a pinned Android NDK and explicit API level;
see [android-ndk-oracle.md](android-ndk-oracle.md).

The FreeBSD libc ABI oracle uses pinned amd64 and arm64 base-system release
artifacts; see [freebsd-libc-oracle.md](freebsd-libc-oracle.md).
