# Debugging the gcc-torture (and sibling) unsupported corpora

> Scope: working `slate-os0h.3.1` (triage the gcc-torture unsupported corpus
> into focused bugs) and its children, or the equivalent triage epics for
> `c-testsuite`, `chibicc`, and `libc-test/functional`. Read
> [lowerer internals](lowerer-internals.md) first if the bug turns out to be in lowering, not
> just test plumbing.

## The three-test pattern

Each corpus suite (`tests/*_suite.rs`) that has a supported/unsupported split
follows the same three-test shape. Names below use `gcc_torture`; substitute
`chibicc` or `libc_test_functional` for those suites (`c_testsuite` currently
only has the first test — no unsupported-corpus tests exist for it yet).

| Test | What it does | Run it with |
| --- | --- | --- |
| `gcc_torture_supported_tests_match_c` | Translates + compiles + runs every case in the *supported* dir; fails if any regress. | Included in `--profile lowering` and `--profile rewrites` already. |
| `gcc_torture_unsupported_tests_still_fail` | Runs every case in the *unsupported* dir; **fails loudly with ready-to-run `git mv` commands** for any case that now passes end-to-end. | Included in `--profile lowering` (wired in for this epic). |
| `gcc_torture_unsupported_triage_report` | `#[ignore]`d — prints `PASS`/`FAIL <name>` (with the failure detail) for every unsupported case, doesn't assert anything. | `cargo nextest r --release --test gcc_torture_suite -E 'test(gcc_torture_unsupported_triage_report)' --run-ignored ignored-only --nocapture` |

**Always run through `cargo nextest r --release --profile lowering`** (never
`cargo test`, per the top-level `CLAUDE.md`) while working this epic — see
[Why the `lowering` profile matters](#why-the-lowering-profile-matters) below.
Prefer running the whole profile over cherry-picking a single test name: it
also re-confirms the supported corpus didn't regress, and it's what
`gcc_torture_unsupported_tests_still_fail` needs to actually report newly
promotable cases.

### Reproducing one case

```bash
SLATE_GCC_TORTURE_FIXTURE=<name> cargo nextest r --release --test gcc_torture_suite \
  -E 'test(gcc_torture_unsupported_triage_report)' --run-ignored ignored-only --nocapture
```

`SLATE_GCC_TORTURE_FIXTURE` (and `SLATE_LIBC_TEST_FIXTURE` for libc-test
functional) filters to one case by file stem, regardless of which dir it's in.
chibicc and c-testsuite cases are directories, not single files, and have no
single-case filter env var — run the whole bucket, or pass the fixture's `.c`
directly to `cargo run -- translate`.

### Directory layout (this varies per suite — check before assuming)

| Suite | Supported | Unsupported |
| --- | --- | --- |
| gcc-torture | `tests/fixtures.gcc-torture/` | `tests/fixtures.gcc-torture.unsupported/` (sibling dir) |
| c-testsuite | `tests/fixtures.c-testsuite/` | `tests/fixtures.c-testsuite.unsupported/` (sibling dir) |
| chibicc | `tests/fixtures.chibicc/supported/` | `tests/fixtures.chibicc/unsupported/` (nested bucket, sibling `ignored/`) |
| libc-test functional | `tests/fixtures.libc-test/functional/supported/` | `tests/fixtures.libc-test/functional/unsupported/` (nested bucket) |

Promotion is always a `git mv` of the fixture from unsupported to supported —
`gcc_torture_unsupported_tests_still_fail`'s failure message prints the exact
command. Never hand-copy a fixture; `git mv` preserves history.

### The `.ignored` bucket

`tests/fixtures.gcc-torture.ignored/` (flat) and
`tests/fixtures.chibicc/ignored/` (nested, same shape as `supported/` and
`unsupported/`) hold cases that are not a Slate lowering gap at all -- either
genuinely undefined behavior per the C standard, e.g. `strlen-5`, which reads
past one array/struct member's declared bound into an adjacent sibling
member by relying on incidental memory layout (C11 6.5.6p8), unlike the
struct-hack/flexible-array-member idiom (also technically UB pre-C99, but
universally treated as a de facto supported contract, which Slate does
support); or a genuine toolchain limitation upstream of Slate entirely, e.g.
chibicc's `include_macro`, which relies on a GCC-specific preprocessor
whitespace-trimming behavior for macro-expanded `#include` header-names that
clang itself (with or without CIR) doesn't implement -- reproduces with plain
`clang -E`, before Slate's pipeline ever sees the file. This is not the same
as "unsupported": `.unsupported/` is tracked work we intend to eventually
fix; `.ignored/` is work we've decided not to chase because the failure
isn't Slate's to fix. No test suite scans these directories, so cases there
don't count against `.unsupported` triage progress. Every fixture moved here
needs a top-of-file comment explaining why: which standard clause makes it
UB (and why it's not the same as an idiom Slate does support), or which
toolchain limitation is at fault and how it was confirmed to reproduce
upstream of Slate.

## How to dig into one failing case

1. **`cargo run -- translate <file.c>`** — full pipeline output: lowering +
   fixups, exactly what the differential test compiles. Read this first.
2. **`cargo run -- translate-lowered <file.c>`** — raw lowering, fixups
   skipped entirely. If a bug disappears here, it's a fixup bug
   ([writing a query-driven fixup](writing-a-query-driven-fixup.md)), not a lowering bug
   ([lowerer internals](lowerer-internals.md)). This is the fastest lowering-vs-fixup triage
   step — do it before reading any lowerer code.
3. **`cargo run -- emit-cir <file.c>`** — the generic-form CIR text the
   lowerer's parser actually consumes. If you need clang's own diagnostics
   (warnings, `not yet implemented` errors) instead of the parsed tree, run
   clang directly:
   ```bash
   $SLATE_CLANG -fclangir -emit-cir -std=<std> <file.c> -o /tmp/x.cir
   $SLATE_CIR_OPT /tmp/x.cir -o /tmp/x.mlir --mlir-print-op-generic
   ```
   `grep` the `.mlir` file for the symbol/op you care about — e.g.
   `#cir.global_view<`, `cir.get_member`, a specific `sym_name`.
4. **Clang AST**, when CIR alone doesn't disambiguate (e.g. anonymous struct
   member names, signedness on a typedef): `$SLATE_CLANG -Xclang -ast-dump=json -fsyntax-only <file.c>`.
5. **`cargo run -- fixup-debug <file.c>`** — only if step 2 showed the bug is
   fixup-introduced. `--only-pass <name>` / `--up-to-pass <name>` isolate a
   single pass; see `docs/passes.md` for the pass list and order.

### Where the actually-compiled batch binary lives

The differential harness batches every case from one run into a single Cargo
crate under `<work_dir>/batch_cargo` (`tests/support::compare_batch_with_jobs`),
then redirects `CARGO_TARGET_DIR` to a per-project subdir of
`target/test-cache/` (the manifest-relative path to `batch_cargo`, with every
non-alphanumeric character replaced by `-`). For gcc-torture specifically:

```
target/gcc-torture-suite/<group>/<name>.generated.rs        # translated Rust, pre-batching
target/test-cache/target-gcc-torture-suite-<group>-batch-cargo/debug/<name>   # the actual runnable binary
```

(`<group>` is `supported` or `unsupported`.) **Run that binary directly** when
the harness's one-line diff isn't enough:

```bash
RUST_BACKTRACE=1 target/test-cache/target-gcc-torture-suite-unsupported-batch-cargo/debug/<name>
```

This gets you a real panic message and Rust backtrace instead of just "exit
code differs." It's debug-mode (`cargo build`, not `--release`), so Rust's
`#[cfg(debug_assertions)]` UB checks (misaligned/overlapping
`copy_nonoverlapping`, etc.) are active — genuinely useful here, not noise.

## Why the `lowering` profile matters

`src/backend/mod.rs`'s `apply_with` becomes a no-op — skipping every fixup
pass — whenever `NEXTEST_PROFILE=lowering` (which `cargo nextest r --profile
lowering` sets automatically) or `SLATE_RAW_LOWER` is set. So the *same*
`gcc_torture_supported_tests_match_c` test exercises raw lowering output under
`--profile lowering` and lowering-plus-fixups under `--profile rewrites` (or
plain `cargo nextest run`). Since gcc-torture triage is a lowering-focused
epic, always use `--profile lowering` — it's faster (no fixup passes to run)
and it isolates lowering bugs from fixup bugs by construction. Don't use
`cargo test` or `cargo nextest run` with no profile; see the top-level
`CLAUDE.md` for why.

## Signs to look for

- **`cannot find value X in this scope` at the batch build step** — some code
  path silently dropped a global/item instead of erroring. Grep
  `src/frontend/lowerer.rs`'s `collect_global` for a `raw.starts_with(...)`
  branch that doesn't match your case's CIR text, falling through to a final
  `else` that just returns `None` and skips insertion. `#cir.ptr<N : iM>`
  (integer-to-pointer constants) and `#cir.global_view<..>` with an index list
  both hit exactly this failure mode historically — see `parse_cir_int_ptr`
  and `parse_cir_global_view`/`parse_cir_global_view_indices` in
  `src/frontend/lowerer/constants.rs`.
- **A garbled/mangled identifier in the generated Rust** (e.g.
  `Upgrade_items___1___i32_`) — a string parser swallowed more of a raw CIR
  attribute than intended, usually because it searched for the *last* or
  *any* delimiter instead of the *first* relevant one. Compare the parser's
  terminator logic against the actual attribute grammar (`cir-opt`'s `.mlir`
  dump is the ground truth) rather than guessing from the garbled output.
- **Correct in isolation, wrong when referenced out of module order** — any
  lookup keyed by a symbol name that's only populated by a single sequential
  pass over the module (e.g. `Lowerer::globals`, populated by
  `collect_global` in CIR module order) breaks if something earlier in that
  same pass needs to look up something declared *later*. Check whether the
  lookup needs a name/type index built in a separate pre-pass instead (see
  `global_sym_types` in `lowerer.rs` for the pattern).
- **`exit code differs: C=Some(0) Rust=None`** — the Rust binary did not exit
  normally; it was killed by a signal (panic-abort, segfault) or, less often,
  the test's own timeout wrapper. `tests/support::run_with_config` reports a
  distinct `"... timed out after Ns"` message for actual timeouts, so a bare
  `Rust=None` diff usually means a real crash — go run the batch binary
  directly (see above) to get the panic and backtrace. This is a genuine
  runtime bug, not a translation-plumbing bug; don't assume the fixture is
  merely mis-triaged.
- **`unsafe precondition(s) violated: ptr::copy_nonoverlapping requires ...`**
  — almost always a real aliasing bug in the generated code, not test noise:
  the debug-mode UB checks caught the lowerer emitting a `memcpy`-shaped copy
  (`copy_nonoverlapping`) where C semantics required a `memmove`-safe copy or
  an intermediate temporary (e.g. an sret return value written straight into
  a destination that can alias its source). Don't paper over it with
  `copy_nonoverlapping` -> `copy`; understand why the aliasing is reachable
  first, since a blanket switch could mask a genuine correctness bug
  elsewhere.
- **A CIR error like `not yet implemented`** straight from clang (not from
  Slate) — this is an upstream ClangIR gap, not a Slate bug. Reproduce with
  bare `$SLATE_CLANG -fclangir -emit-cir` (no slate involved) to confirm, then
  record the upstream disposition on the bd issue and leave the fixture
  unsupported; don't try to work around it in the lowerer.

## Filing focused bugs

Per this epic's convention: each cluster of related unsupported cases becomes
one focused `bd` bug (child of `slate-os0h.3.1`), listing every case name it
covers, with acceptance criteria requiring every listed case to translate,
compile, match C's stdout/exit code, and be promoted (`git mv`) into the
supported dir — or, for a confirmed upstream CIR limitation, to record that
disposition and stay mapped under the bug until CIR fixes it. If fixing one
case in a bug's list exposes an unrelated bug in a *different* case from that
same list (as happened with `pr43784`'s sret-aliasing issue surfacing after
its `global_view` bug was fixed), split the unrelated case into its own new
bug rather than stretching the original bug's scope — keep `bd close`'s
reason and the new bug's description explicit about why the split happened.
