# Fuzzing

Slate is verified by **differential testing**: translate a C program to Rust,
compile and run both, and require identical stdout and exit code. The fuzzer's
job is to produce a stream of _varied but valid_ C programs to feed that harness.

## Two generators

| Artifact           | File                    | Role                               |
| ------------------ | ----------------------- | ---------------------------------- |
| `c.bnf`            | reference grammar       | **documents** the supported syntax |
| C-subset generator | `tests/support/cgen.rs` | **primary** behavioral fuzzer      |

### Why a grammar cannot be the fuzzer

`c.bnf` is a context-free grammar, and a CFG has no memory. It can say "an
expression is `<expr> + <expr>`," but it cannot say "…where each leaf is a
variable that is currently in scope and initialized," or "…and this call's
arguments stay within the callee's parameter range," or "…and the running value
stays small enough not to overflow `i32`." Those constraints are exactly what
make the output valid, translatable, and UB-free, and they are all
context-sensitive — they depend on state built up earlier in the program.

So the grammar cannot drive generation. It is kept purely as a one-screen
reference for _which syntax_ Slate supports. The `bnf_fuzz` tests keep it honest:
it must parse, expose the expected top-level rules, and be internally closed
(every nonterminal referenced on a right-hand side is defined). The real
behavioral coverage comes from `cgen`, which encodes the same subset in Rust
together with the scope and value state a grammar cannot carry.

## The C-subset generator

`cgen::generate(seed)` is a small, csmith-style generator. Instead of stitching
templates, it maintains **generation state** and composes programs from it:

- a **stack of scopes**, each holding the variables currently declared (with a
  value bound); expressions only reference variables that are in scope and
  initialized;
- the **signatures of functions emitted so far**, so later code (and `main`) can
  call them and thread the results into new expressions.

From that state it emits multi-function programs with:

- helper functions taking and returning `int`, plus fixed `char` and `double`
  helpers that exercise non-int signatures;
- scoped locals, reused across statements, plus loop-scoped induction variables;
- expressions composed by `+`, `+=`, and `++` over live variables, constants, and
  nested calls to earlier functions;
- scoped locals passed as call arguments;
- `for`/`while` accumulation loops, fixed-size `int` arrays, structs/unions with
  primitive scalar fields, `sizeof`, `volatile` locals, `static` primitive
  globals, and enum constants;
- `main` that prints every function's result with `printf("%d\n", ...)`.

Everything stays inside the subset Slate can translate today (see
[README.md](README.md) for that surface). This is deliberately narrower than a
general C generator — it is a generator _for Slate's supported subset_, and it
grows as the supported subset grows.

### Correctness invariant: no undefined behavior

The differential harness compiles the generated Rust in **debug** mode, where
`i32` overflow _panics_, while C silently wraps. An overflowing program would
diverge for reasons that are not Slate's fault. Uninitialized reads and
out-of-bounds indexing are UB in C and would diverge similarly.

The generator rules these out **by construction**:

- It carries a conservative upper bound on the absolute value of every variable
  and every expression, and never builds an operation whose bound could exceed
  `VALUE_CAP` (well under `i32::MAX`; there is no multiplication, so additions
  cannot overflow).
- Function arguments are kept within each callee's assumed parameter range, so a
  recorded return bound stays valid at every call site.
- Loop trip counts and per-iteration growth are bounded, so accumulators stay
  bounded.
- Array indices are always literals in range; every variable is initialized at
  its declaration before it is ever read.

`cgen`'s unit tests assert these properties directly (determinism per seed,
program skeleton, and that the value bound never exceeds the cap across a seed
sweep).

## Running it

```bash
# fresh random seeds each run
cargo test --test bnf_fuzz generator_differential -- --nocapture
SLATE_FUZZ_CASES=64 cargo test --test bnf_fuzz generator_differential -- --nocapture

# deterministic: seeds n, n+1, ...
SLATE_FUZZ_SEED=42 cargo test --test bnf_fuzz generator_differential -- --nocapture

# replay a single seed a failure reported
SLATE_FUZZ_SEED=<seed> SLATE_FUZZ_CASES=1 cargo test --test bnf_fuzz generator_differential -- --nocapture
```

`SLATE_FUZZ_CASES` (default 8) sets how many seeds to run. By default the seeds
are random, so repeated runs explore different programs; `SLATE_FUZZ_SEED=<n>`
makes a run deterministic (seeds `n, n+1, ...`). Generation is a pure function of
its seed, so a failure reports the exact seed that reproduces the program — and
the offending `.c` and `.generated.rs` are left under `target/cgen-fuzz/` for
inspection.

## Not yet done

- **Shrinking.** Failures are reproducible by seed but not automatically
  minimized. A shrinker would re-run generation with progressively smaller knobs
  (fewer functions/statements, smaller expressions) while the failure persists.
- **Wider types.** The generator is int-only because that is Slate's solid
  subset. It should grow new value types (other integer widths, chars, pointers)
  in lockstep with lowering support, extending the same scope/bound machinery.
