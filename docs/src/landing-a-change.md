# Landing a change

Start with one small C fixture. Run it against the current code and confirm that
it fails for the reason you intend to fix. Get that case passing before adding
the next one.

## Lower C IR to Rust

Start in Slate's lowerer when the IR has the information you need and
Slate emits incorrect Rust or reports a lowering barrier. Use
`slate emit-slate-ir` on the fixture to see what the parser hands the
translator, then `slate translate` to see the result.

Use an end-to-end C fixture with a `main` that exercises the affected code. For
example, Slate's `add.c` fixture calls `add(2, 3)` and prints `5`. The markers
tell the fixture tool which function to check in the generated Rust.

```c
#include <stdio.h>

// @check-fn-begin
int add(int a, int b) {
  int c = a + b;
  return c;
}
// @check-fn-end

// @check-fn-begin
int main(void) {
  printf("%d\n", add(2, 3));
}
// @check-fn-end
```

1. For a new unsupported construct, put the reproducer in Slate's
   `tests/fixtures.unsupported/` and confirm `slate translate` reports the
   expected barrier. For a regression in supported code, add or extend a fixture
   in `tests/fixtures/` and reproduce the failure in the differential test.
2. Fix the lowerer. Once a previously unsupported fixture translates, move it to
   `tests/fixtures/`. Generate its FileCheck block and run the fixture. The test
   compares C and Rust stdout and exit status, then checks the generated Rust.

Run the commands below from the workspace root. Substitute your fixture's path
and stem for `add.c` and `add`.

```sh
python3 crates/slate/tools/update_filecheck.py crates/slate/tests/fixtures/add.c
SLATE_DIFF_FIXTURE=add cargo nextest run --release --profile slate \
  --test differential -E 'test(=generated_differential)'
```

## Change parsing or IR

When the lowerer needs a fact about C that the IR omits or gets wrong, fix
`slate-parser`. It takes source through preprocessing, lexing, parsing, and
semantic analysis before emitting IR. Fix the earliest stage where the
information is wrong.

| What needs to change                                         | Where to start in `slate-parser`                                   |
| ------------------------------------------------------------ | ------------------------------------------------------------------ |
| Includes, macro expansion, or conditional compilation        | `src/pp/`                                                          |
| Tokens and literal spelling                                  | `src/lexer.rs`                                                     |
| C syntax or AST construction                                 | `src/parser/`                                                      |
| Types, conversions, name resolution, or semantic diagnostics | `src/sema/`                                                        |
| A checked C fact that must reach lowering                    | IR construction in `src/sema/`, IR types and printing in `src/ir/` |

1. Add a C fixture under `tests/fixtures/<flavor>/<os>/<arch>/` in
   `slate-parser`. Use the `error/` tree for a diagnostic. Run the fixture's
   parser or IR command first and verify that its diagnostic or output differs
   from the expected result. For a IR fixture, check the fact the lowerer
   will read.
2. Fix the owning subsystem and regenerate the fixture's checks with the
   `slate-parser` FileCheck updater. Run the `parser` profile. If the lowerer
   also needs a change, reproduce the end-to-end failure with a Slate
   differential fixture before editing the lowerer. Run the `slate` profile once
   it passes.

Replace `path/to/my_fixture.c` with your fixture's path and `my_fixture.c` with
its filename.

```sh
python3 crates/slate-parser/tools/update_filecheck.py --in-place path/to/my_fixture.c
cargo nextest run --release --profile parser --test filecheck \
  -E 'test(my_fixture.c)'
```

## Add a rewrite

Start in `slate-analysis` for improvements that can be derived from C IR, such
as pointer, length, and string facts. Lowering can use those facts to emit
better Rust. Put a rule that depends on the shape of the generated Rust in
Slate's backend rewrite engine.

1. Add or extend an end-to-end C fixture in Slate's `tests/fixtures/`. As in
   `add.c`, call the affected function from `main` and make its result
   observable. Mark the function whose Rust should change with `@check-fn-begin`
   and `@check-fn-end`. Run the fixture first and compare the generated Rust
   with the expected rewrite to confirm the output check would fail.
2. Implement the change, regenerate the FileCheck block, and review the diff to
   verify the rewrite fired. Rerun the differential fixture to check the Rust
   output and confirm it still behaves like C.

Run formatting, Clippy, and the release profiles for the crates you changed
before landing. See [Setup](setup.md) for profile requirements and
[Testing](testing.md) for fixture selection and triage.
