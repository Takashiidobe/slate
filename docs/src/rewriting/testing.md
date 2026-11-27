# Adding and Testing a Rule

Start with a small C fixture in `crates/slate/tests/fixtures/`. Give it one case
that should rewrite and one case where the guard must preserve the original
behavior. For example, a temporary substitution rule needs a case where the
source changes before the read.

## Find the rule that fired

```bash
SLATE_RULE_TRACE=1 slate translate input.c
```

The trace lists rule names on stderr. If the shape you expect already appears in
IR, check the IR passes before changing the Rust engine. See
[Inspecting the IR Pipeline](../parsing/ir/pipeline.md).

## Check the output and behavior

Mark the functions to check with `// @check-fn-begin` and `// @check-fn-end`.
Regenerate the FileCheck block and run the fixture from the workspace root.

```bash
python3 crates/slate/tools/update_filecheck.py crates/slate/tests/fixtures/example.c
SLATE_DIFF_FIXTURE=example cargo nextest r --release --profile slate   --test differential -E 'test(generated_differential)' --nocapture
```

FileCheck verifies the emitted Rust. The differential harness also compares
native C and translated Rust stdout and exit status. Review the generated diff
to confirm which rewrite fired and that the blocked case kept its shape. See
[Testing](../testing.md) for the full profiles.
