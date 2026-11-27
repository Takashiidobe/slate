# Compare Against a Compiler

Choose the comparison that isolates the discrepancy. Build the release parser
and use matching compiler flags and target headers on both sides.

## Preprocessed tokens

```sh
cargo build --release -p slate-parser
python3 crates/slate-parser/tools/pp_diff.py \
  --file crates/slate-parser/tests/fixtures/clang/linux/x86_64/add.c
```

`pp_diff.py` compares the tokens from `slate-parser pp` and `clang -E -P`. Clang
uses Slate's compiler headers and sysroot for the comparison. The tool ignores
spacing and line breaks and writes the mismatch report to `target/pp-diff.md` by
default. Use `--compdb <file>` for compile commands or pass extra options to a
single-file run after `--`.

## Flags, predefines, and record layout

- `crates/slate-parser/tools/flag_probe.py` compares flag acceptance and
  predefined macro changes. For example, pass `-fwrapv --flavor gcc` to compare
  the flag and its opposite under GCC. It prints `DIFF` when the results differ.
- `crates/slate-parser/tools/gcc_macro_diff.py` takes two quoted arguments, a
  compiler command and Slate parser arguments. For example, use
  `'gcc -std=gnu17'` and `'--flavor=gcc -std=gnu17'`. It lists names whose
  presence or expansion differs.
- `crates/slate-parser/tools/x86_isa_diff.py` compares the effects of x86 CPU
  and feature flags on predefined macros. Use `--gcc` for GCC and consult the
  JSON report to distinguish macro mismatches from flags rejected by Slate.
- `crates/slate-parser/tools/check_record_layout.py` compares IR record size,
  alignment, and offsets with Clang's record layout dump. Pass a source file and
  `--binary target/test-cache/release/slate-parser` to use the release binary.
  Use `--target <triple>` when investigating a specific target.

## Validate a translation with Alive2

```sh
python3 tools/alive2/install.py
cargo build --release -p slate-c2rust
python3 tools/alive2/validate.py tools/alive2/fixtures/scalars.c -f add
```

The validator compares the LLVM IR from Clang with the LLVM IR from compiling
Slate's generated Rust. Its `report.json` records each outcome, and the run
directory under `target/alive2/` keeps the exact comparison IR and tool logs.

- `validated` means Alive2 established refinement within its model and bounds.
- `incorrect` means it found a counterexample. Open the corresponding
  `alive-*.stdout` and the paired LLVM inputs.
- `unsupported`, `unknown`, `timeout`, and `tool_error` describe runs that did
  not establish refinement. The report distinguishes these outcomes.

Use `--unroll` to change the loop bound and `--glob` to select fixtures in a
directory. GCC and MSVC fixtures are outside this Clang-based comparison.
