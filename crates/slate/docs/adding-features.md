# Adding Features

Every change starts with a C fixture. Slate's contract is behavioral first:
generated Rust must compile and match the original C program's stdout and exit
code.

Use one C file per idea under `tests/fixtures/`:

```bash
$EDITOR tests/fixtures/<name>.c
cargo test --test differential generated_differential -- --nocapture
```

The checked fixtures are C-only. Generated Rust for inspection lives under the
ignored `tests/fixtures.generated/` directory:

```bash
cargo run -- emit-fixtures
sed -n '1,160p' tests/fixtures.generated/<name>.rs
```

After adding the fixture, decide what kind of work it is.

## Baseline Language Feature

A baseline feature lets Slate translate more C programs. Examples include a new
C operator, `if`, `while`, arrays, structs, pointers, globals, enum types, or
broader union support.

Use this path when the generated Rust does not compile, contains `todo!`, drops
semantics, or produces different output from the C program.

1. Add the smallest C fixture that demonstrates the missing feature.
2. Run the generated differential test and observe the failure:

   ```bash
   cargo test --test differential generated_differential -- --nocapture
   ```

3. Inspect the CIR shape:

   ```bash
   cargo run -- emit-cir tests/fixtures/<name>.c
   ```

4. Inspect Clang's source facts when CIR does not carry enough information:

   ```bash
   ${SLATE_CLANG:-$HOME/llvm-project/build-cir/bin/clang} \
     -Xclang -ast-dump=json -fsyntax-only tests/fixtures/<name>.c
   ```

5. If CIR has the necessary semantics, add or extend the handler in
   `src/lower.rs`.
6. If the source-level fact is missing from CIR, extend `src/c_ast.rs` to extract
   it from Clang JSON. Keep raw JSON available as the escape hatch while the
   compact model grows.
7. Keep baseline Rust conservative and C-shaped: explicit temps, raw pointers,
   `libc`, and `unsafe` are acceptable.
8. Add focused unit tests for new parser or AST extraction logic when possible.
9. Run:

   ```bash
   cargo fmt
   cargo test
   cargo run -- emit-fixtures
   ```

The baseline feature is done when the fixture passes differential testing and
the generated Rust preserves the relevant C semantics.

## Rust Fixup

A fixup improves Rust that is already correct. Examples include converting
`libc::printf` to `println!`, removing redundant temps, collapsing `__retval`,
or recovering idiomatic `for` loops.

Use this path only when the baseline generated Rust already compiles and passes
differential testing.

1. Start from an existing passing fixture, or add a new C fixture if the pattern
   needs a clearer example.
2. Generate the baseline Rust:

   ```bash
   cargo run -- emit-fixtures
   ```

3. Identify the narrow Rust pattern to rewrite and the proof that semantics do
   not change.
4. Implement the fixup as a separate transformation in spirit. Do not mix it
   into baseline lowering unless the baseline is currently wrong.
5. Leave unsupported or ambiguous patterns on the baseline path.
6. Add tests for the output shape when the fixup has a stable textual form, and
   keep differential testing green.
7. Run:

   ```bash
   cargo fmt
   cargo test
   cargo run -- emit-fixtures
   ```

The fixup is done when output and exit code are unchanged and the generated Rust
is clearly better for the supported pattern.

## Choosing The Path

Use baseline language feature work when:

- the C construct cannot be translated yet;
- generated Rust fails to compile;
- generated Rust behaves differently from C;
- Slate needs a new source fact from CIR or Clang AST.

Use fixup work when:

- the C construct already translates correctly;
- the change is only about readability, idiom, or reducing `unsafe`;
- disabling the fixup would still leave correct generated Rust.

Do not make a fixup carry correctness. Correctness belongs in baseline lowering;
fixups are optional improvements.
