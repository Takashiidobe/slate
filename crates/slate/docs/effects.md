# Rust Effects Validation

Effects validation checks that fixups preserve behavior by comparing two Rust
programs:

```text
raw lowered Rust AST  ->  fixuped Rust AST
```

It does not compare C, CIR, or source facts. Differential testing remains the
C-to-Rust correctness backstop. Effects validation is for the fixup ladder: if a
rewrite changes what the raw lowered Rust would have done, the raw-vs-fixuped
effect traces should diverge.

## Where It Lives

- `src/effects/mod.rs` defines the shared `Value`, `Location`, `Effect`, and
  `EffectTrace` vocabulary.
- `src/effects/rust_ast/interp.rs` executes Slate's Rust AST and emits an
  `EffectTrace`.
- `src/effects/interpreter.rs` normalizes and compares traces.
- `tests/effects_regression.rs` holds the Rust-vs-Rust fixture tests and the
  ignored all-fixture ratchet.
- `slate compare-effects-rust-rust <file.c>` lowers a C fixture, interprets raw
  lowered Rust, applies all fixups, interprets fixed Rust, and compares traces.

## Semantic Boundary

Ordinary locals are state, not effects. The interpreter may track local values,
heap objects, arrays, structs, files, atomics, and function bodies so later
observable behavior can be computed, but it should only push meaningful events.

Push effects for:

- heap/resource lifetime when it matters for comparison: `Alloc`, `Dealloc`,
  `FileOpen`, `FileWrite`, `FileClose`
- modeled memory operations that can feed later effects: `Read`, `Write`
- user-visible or external call summaries: `Call`, or a more specific effect
  when one exists
- atomics and fences
- function/program termination: `Return`, `Exit`

Do not push effects for:

- scalar `let` bindings and assignments
- reads of ordinary locals
- pure arithmetic, casts, comparisons, and path constants
- temporary aliases introduced only by lowering

If a value is needed to compute a later effect, update interpreter state instead
of adding trace noise.

## Adding Coverage

Start from the failing fixture and the AST shape, not from the source C.

```bash
cargo run -- emit-lowered-fixtures
cargo run -- emit-fixtures
cargo run -- compare-effects-rust-rust tests/fixtures/<name>.c
```

For all-fixture triage:

```bash
cargo test --test effects_regression -- --ignored --nocapture
SLATE_EFFECT_FIXTURE=<name> cargo test --test effects_regression -- --ignored --nocapture
```

When adding a new semantic case:

1. Identify whether the missing node is a statement, expression, call summary,
   or comparator normalization issue.
2. Add the smallest Rust AST interpreter behavior that makes the node execute
   according to Slate's emitted subset.
3. Keep unsupported shapes explicit with a precise panic message. Do not guess.
4. Add or update an in-file unit test under `src/effects/rust_ast/interp/tests.rs`
   for the new semantic shape.
5. Add or keep a fixture-level regression in `tests/effects_regression.rs` when
   the shape corresponds to a known fixup family.
6. Run the focused fixture command and the effects regression test.

## Where To Implement

Use these entry points in `src/effects/rust_ast/interp.rs`:

- `step` for statement behavior such as `Let`, `Assign`, `Return`, loops, and
  expression statements with special effects.
- `eval` for expression behavior such as `Index`, `Field`, `AddrOf`, `Deref`,
  arithmetic, casts, method calls, and function calls.
- `assign`, `assign_index`, `assign_field`, and related helpers for writes.
- `eval_call` and `eval_call_summary` for libc/std/POSIX summaries.
- atomic helpers for `AtomicLoad`, `AtomicStore`, `AtomicFetch`, `AtomicSwap`,
  `AtomicCompareExchange`, and native atomic method calls.
- `src/effects/interpreter.rs` only when two correct traces differ by benign
  representation details and need normalization.

Prefer semantic helpers over pass-specific cases. For example, model
`AddrOf(Index(array, i))` as a pointer to the collection element, then `Deref`
and `offset_from` naturally work for every fixup that produces the same shape.

## Calls And Libraries

Known external calls that fixups rewrite need explicit summaries. Unsupported
calls should either remain opaque when both raw and fixed Rust still call the same
thing, or fail extraction with a precise message.

Current summary policy:

- memory calls such as `malloc`, `free`, `memcpy`, `memmove`, `memset`, and
  `memchr` update modeled memory and resource state
- file calls such as `fopen`, `fputs`, and `fclose` emit file effects
- string/format calls should normalize to the behavior the fixup preserves, not
  to the spelling of the API
- unknown or environment-dependent calls should not be silently treated as pure

For formatted output, the target model is stdout/stderr bytes, not "both sides
called printf". Raw `printf` and fixed `println!` should compare equal only when
the supported format expansion produces the same bytes.

## Normalization

The comparator may normalize representation-only differences:

- dead writes to unobserved allocations
- allocation id shifts caused by unobserved allocations
- raw pointer storage versus safe collection storage when reads/writes denote the
  same logical byte locations

Do not normalize away externally visible differences. If stdout bytes, file
writes, atomic orderings, return/exit code, or live memory values differ, the
trace should fail.

## Failure Triage

There are two useful failure classes:

- extraction failures mean the Rust AST interpreter cannot execute a shape yet
- trace divergences mean extraction succeeded and either a fixup changed
  behavior or the comparator needs a justified normalization

For extraction failures, implement the missing AST/call semantics. For
divergences, inspect both traces before changing normalization. A normalization
belongs in `src/effects/interpreter.rs` only when both traces represent the same
observable behavior by construction.

The `slate-tpz` beads track the current coverage buckets. Before starting a new
effects task, run:

```bash
bd show slate-tpz
bd children slate-tpz
bd list --status in_progress
```

Claim the relevant bead and keep any new coverage gap in beads, not in a loose
TODO file.

## Verification

For a narrow interpreter addition:

```bash
cargo fmt
cargo nextest r --release --test effects_regression
```

For broader changes or comparator normalization:

```bash
cargo fmt
cargo nextest r --release
```

Run the ignored all-fixture ratchet when the work is intended to reduce corpus
failures:

```bash
cargo test --test effects_regression -- --ignored --nocapture
```

Record the before/after pass and failure counts on the bead.
