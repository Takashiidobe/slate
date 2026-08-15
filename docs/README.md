# slate

`slate` translates C to Rust using its own C frontend (`src/parse`): a
lexer, preprocessor, and parser producing a fully typed AST directly from
source, with no external compiler IR in between. The AST retains structured
control flow, integer signedness, and named local variables directly from the
source, so this is _transpilation_, not decompilation. (See
[architecture.md](architecture.md) for more details)

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
- `bd list --status=blocked` - blocked tasks by upstream Rust or unported
  frontend coverage.

Generated Rust trees are ignored inspection artifacts and are not regenerated
as part of feature work or completion. When explicitly requested for manual
inspection, they can be written with:

```bash
cargo run -- emit-fixtures
```

That command translates every currently supported fixture suite into an ignored
sibling `*.generated/` tree while preserving its relative layout. This includes
the primary, cfg, multi-TU, supported chibicc, supported c-testsuite, supported
gcc-torture, library, and stdlib fixtures;
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
C ──lex/preprocess──► tokens ──parse──► AST ──lower──► Rust source

verified:  run(C).{stdout,exit}  ==  run(Rust).{stdout,exit}
```

## Frontend

Every C input is parsed once by `src/parse` into a single, fully typed AST
(`Program`/`Obj`/`Stmt`/`Expr`/`Type`) — struct/union layout with bitfields,
VLAs, atomics, `_BitInt`, complex numbers, and inline asm are all represented
directly on that tree, so there is no separate IR and AST to reconcile by
source location.

## Error ownership

Library failures stay typed until they reach a user-facing boundary. Each
subsystem owns its error enum: frontend lexing/preprocessing/parsing,
compile-command decoding, and directive translation. Concrete I/O, JSON,
target-triple, and nested subsystem failures remain available through the
standard error source chain; tool status, stderr, source paths, directive
locations, predicates, and lowering diagnostics remain structured fields.

`api::Error` aggregates translation failures without converting them to text.
The `slate` binary converts typed errors to their `Display` output through its
single `cli_result` adapter, and the test harness may do the same when reporting
a failed case. Library modules do not use `String` as an error type.

## Docs

- [fixups.md](fixups.md) — how to state
  query-driven rewrite cases, proofs, typed recipes, definition lifecycles,
  scheduling, and tracing.
- [architecture.md](architecture.md) — frontend, pipeline, shared context.
- [passes.md](passes.md) — the pass catalog: what runs, in what order, how.
- [facts.md](facts.md) — the salsa-memoized facts analysis layer: what each
  collector proves and which rewrite pass consumes it.

## Toolchain

Slate is self-contained: its frontend (`src/parse`) is a from-scratch C lexer,
preprocessor, and parser, so no external clang installation or build is
required to translate C. An external `clang`/`cc` is still useful as a
differential-testing oracle in `tests/`, but it is not a build or runtime
dependency of the `slate` binary itself.

Target selection is shared across preprocessing and lowering with
`SLATE_TARGET=<triple>`. Android targets also require
`SLATE_ANDROID_API=<level>`; the 64-bit Bionic baseline starts at API 21.
`SLATE_TARGET=aarch64-apple-darwin` selects the narrow AArch64 macOS profile
at the macOS 11.0 deployment baseline.

Slate defaults to GNU C23. Legacy inputs that rely on pre-C23 semantics, such
as unspecified parameter lists written as `int (*)()`, can select an older
mode explicitly without changing the default:

```bash
cargo run -- translate -std=gnu17 legacy.c
```

Frontend flags precede the input path and apply consistently across
preprocessing and parsing.

The vendored c-testsuite corpus is compiled and translated uniformly as GNU
C17 because it predates C23 and includes declarations whose meaning changed in
C23.

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

Configured library builds can instead provide one or more compilation
databases:

```bash
cargo run -- translate-project --lib \
  --compile-commands build-linux/compile_commands.json \
  --compile-commands build-android/compile_commands.json \
  project crate
```

Each command is normalized into a translation unit, target, and semantic
compiler arguments. Compiler, output, dependency-file, and source operands are
removed; relative paths are resolved against the command's `directory`. Slate
prefers the JSON `arguments` form and shell-splits `command` as a fallback.
Commands for different targets remain separate through parsing and lowering
and are merged into cfg-gated Rust. Translation units present in only some databases
produce cfg-gated modules. Two different command configurations for the same
translation unit and Rust target are rejected because Slate cannot express
that distinction as a target cfg.

The external pinned Microsoft CRT and UCRT header oracle used for MSVC work is
bootstrapped under `target/`; see [msvc-sysroot.md](msvc-sysroot.md).

The macOS ABI oracle uses an externally installed SDK plus pinned Apple public
Libc and XNU sources under `target/`; see
[macos-sdk-oracle.md](macos-sdk-oracle.md).

The Android Bionic ABI oracle uses a pinned Android NDK and explicit API level;
see [android-ndk-oracle.md](android-ndk-oracle.md).

The FreeBSD libc ABI oracle uses pinned amd64 and arm64 base-system release
artifacts; see [freebsd-libc-oracle.md](freebsd-libc-oracle.md).
