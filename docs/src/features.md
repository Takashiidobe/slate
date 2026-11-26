# Feature coverage

The differential fixtures show which C programs Slate translates, compiles, and
runs with matching output and exit status. The unsupported fixtures track gaps
in parsing, translation, compilation, or runtime behavior.

| Evidence                  | Location                                                                |
| ------------------------- | ----------------------------------------------------------------------- |
| Small supported cases     | `crates/slate/tests/fixtures/`                                          |
| Remaining small cases     | `crates/slate/tests/fixtures.unsupported/`                              |
| Corpus cases              | `crates/slate/tests/fixtures.{gcc-torture,gcc-dg,c-testsuite,chibicc}*` |
| First barrier in a C file | `slate lowering-barriers [compiler arguments] <file.c>`                 |

See [testing](testing.md) for how the suites compare native C and translated
Rust and check generated code.
