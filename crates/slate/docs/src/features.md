# Feature coverage

C23 support is a goal; parser acceptance and Rust lowering coverage are
separate. The supported differential buckets establish runtime parity, while
unsupported buckets track remaining parser, lowering, compilation or runtime
gaps.

| Evidence | Location |
| --- | --- |
| Small supported cases | `crates/slate/tests/fixtures/` |
| Remaining small cases | `crates/slate/tests/fixtures.unsupported/` |
| Corpus cases | `crates/slate/tests/fixtures.{gcc-torture,gcc-dg,c-testsuite,chibicc}*` |
| Current work | `bd ready`, `bd list --status open` |
| First barrier in a C file | `slate lowering-barriers [compiler arguments] <file.c>` |

The Slate profile tests host execution through raw lowering. Cross-target
execution suites are awaiting restoration. Project translation currently
requires an executable with one `main` and one configuration per unit.

Headers come from slate-sysroots; compiler flavor and target facts are modeled
by slate-parser. A declaration being available does not prove that Slate can
translate every use of it. See [testing](testing.md) for the runtime contract.
