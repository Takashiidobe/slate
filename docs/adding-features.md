# Adding Features

Every change starts with a C fixture. Slate's contract is behavioral first:
generated Rust must compile and match the original C program's stdout and exit
code.

Use one C file per idea under `tests/fixtures/`:

```bash
$EDITOR tests/fixtures/<name>.c
cargo nextest r --release --test differential -E 'test(generated_differential)' --nocapture
```

The effects regression is diagnostic-only and is not a required feature or
fixup completion gate. Do not run it by default. Use it only when working
directly on effects interpretation or when a task explicitly requests it.

The alive2 regression is also diagnostic-only and incomplete. It is ignored by
the default test run and is not a required completion gate. Run it only when a
task explicitly requests alive2 validation.

The checked fixtures are C-only. `emit-fixtures` populates ignored sibling
`*.generated/` trees for every supported fixture suite. Fixed-up Rust for the
primary fixtures lives under `tests/fixtures.generated/`:

```bash
cargo run -- emit-fixtures
sed -n '1,160p' tests/fixtures.generated/<name>.rs
```

Raw lowered Rust before fixups lives under
`tests/fixtures.lowered.generated/`:

```bash
cargo run -- emit-lowered-fixtures
sed -n '1,160p' tests/fixtures.lowered.generated/<name>.rs
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
   cargo nextest r --release --test differential -E 'test(generated_differential)' --nocapture
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
   cargo nextest r --release
   cargo run -- emit-fixtures
   ```

The baseline feature is done when the fixture passes differential testing and
the generated Rust preserves the relevant C semantics.

## Rust Fixup

A fixup improves Rust that is already correct. Examples include converting
`libc::printf` to `println!`, removing redundant temps, collapsing `__retval`,
or recovering idiomatic `for` loops.

Use this path only when the baseline generated Rust already compiles and passes
differential testing. A fixup is **AST-to-AST** — see
[writing-a-fixup.md](writing-a-fixup.md) for the pass recipe (shape, shared
helpers, safety rules, registration) before writing one.

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
   cargo nextest r --release
   cargo run -- emit-fixtures
   ```

The fixup is done when output and exit code are unchanged and the generated Rust
is clearly better for the supported pattern.

## Choosing The Path

Baseline work when the C construct can't translate yet, generated Rust fails to
compile or behaves differently from C, or Slate needs a new source fact from CIR
or the Clang AST. Fixup work when it already translates correctly and the change
is only readability, idiom, or reducing `unsafe` — disabling it would still leave
correct Rust.

Do not make a fixup carry correctness. Correctness belongs in baseline lowering;
fixups are optional improvements.

## Adding A Stdlib Coverage Probe

The `slate-nk3.*` epic maps which libc functions Slate can translate, one small
program per function. These tickets are clean parallel work because they only
add isolated fixtures.

Probes are **auto-discovered**: drop a `.c` file under
`tests/stdlib/<header>/<name>.c` and
`cargo nextest r --release --test stdlib_coverage` picks it up — there is
nothing to register.

Conventions, copied from existing probes:

- **Force the call to actually run.** Feed inputs through `volatile` locals so the
  compiler can't constant-fold the libc call away:
  `int volatile a='A', b=' ';`
- **Normalize booleans** with `?1:0` so classifier results are stable across libc
  implementations: `printf("%d %d\n", isdigit(a)?1:0, isdigit(b)?1:0);`
- **Keep every program deterministic and UB-free** — same rule as the generator.
  C and Rust must agree on stdout and exit code. Stay in the C locale; avoid
  host-specific state (time, RNG, installed locales) unless normalized to a
  predicate.

```bash
$EDITOR tests/stdlib/<header>/<name>.c
cargo nextest r --release --test stdlib_coverage --nocapture   # new probe must print `ok`
```

A probe that translates but can't pass yet is not silently skipped: add it to the
`KNOWN_UNSUPPORTED` ratchet in `tests/stdlib_coverage.rs` with a tracking bead.
When it starts passing, the test fails and tells you to promote it — coverage
only ratchets forward.
