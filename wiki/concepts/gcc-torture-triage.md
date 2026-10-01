# GCC torture triage

<!-- toc -->
- [Buckets](#buckets)
- [Inspect one case](#inspect-one-case)
- [Promote and validate](#promote-and-validate)
<!-- /toc -->

## Buckets

| Directory under `crates/slate/tests` | Contract |
| --- | --- |
| `fixtures.gcc-torture` | Must pass C/Rust differential execution |
| `fixtures.gcc-torture.unsupported` | Must still fail; passing cases must be promoted |
| `fixtures.gcc-torture.ignored` | Deliberately excluded cases |

Corpus admission uses native `clang` on PATH. Read the corpus README for
filtering rules; [differential fixtures](differential-fixtures.md) defines the
shared harness contract.

## Inspect one case

Run from the workspace root:

```bash
SLATE_GCC_TORTURE_FIXTURE=<stem> cargo nextest r --release --profile slate \
  --test gcc_torture_suite -E 'test(gcc_torture_unsupported_triage_report)' \
  --run-ignored ignored-only --nocapture
cargo run --release -p slate -- lowering-barriers <fixture.c>
cargo run --release -p slate -- emit-slate-ir <fixture.c>
cargo run --release -p slate -- translate-lowered <fixture.c>
```

| Failure | Investigate |
| --- | --- |
| Parse/sema | slate-parser, using the fixture's compiler flags |
| Unsupported lowering | Reported IR construct in `frontend/lowerer/` |
| Invalid IR | Broken parser/lowerer invariant; never reclassify as a barrier |
| rustc | Emitted Rust types, declarations or ABI |
| Runtime mismatch | C semantics, target layout, evaluation order or runtime bridges |

## Promote and validate

- Track the gap with `bd` and fix it at its owning layer.
- Promote passing cases with the `git mv` printed by the unsupported ratchet.
- Run `cargo nextest r --release --profile slate` after Slate changes; also run the parser profile if parser code changed.
- FileCheck is suspended. A plausible Rust shape is not proof of runtime parity.
