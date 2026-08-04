# Passes

Readability is recovered later by Rust fixups, not during baseline lowering.

## Current pipeline

| Stage              | In -> Out                               | How                                                                    |
| ------------------ | --------------------------------------- | ---------------------------------------------------------------------- |
| **emit-cir**       | C -> CIR text                           | `clang -fclangir -emit-cir` piped to `cir-opt --mlir-print-op-generic` |
| **parse-cir**      | CIR text -> generic Op-tree + locs      | recursive-descent parser over MLIR generic form                        |
| **load-ast**       | C -> compact source context + raw JSON  | `clang -Xclang -ast-dump=json -fsyntax-only`                           |
| **lower**          | CIR + AST context -> Rust source        | match `op.name`; materialize temps; use `libc` / `unsafe`              |
| **fixups**         | baseline Rust AST -> cleaner Rust AST   | fixed cleanup pipeline, `src/fixups::apply`                            |
| **generated-diff** | C + generated Rust -> output comparison | build generated Rust with Cargo + `libc`, compare stdout + exit code   |

Current code path:

```text
emit-cir -> parse-cir -> load-ast -> lower(libc/unsafe) -> fixups -> generated-diff
```

For what the baseline lowerer and fixup ladder currently cover, see
[README.md](README.md) (categorized summary) and

### parse-cir

The parser is deliberately generic. It produces:

```text
Op { results, name, operands, attrs, regions, ty, loc }
```

It does not know what `cir.add` or `cir.for` means. The lowerer owns op
semantics.

### load-ast

`src/c_ast.rs` is a Clang AST oracle, not a handwritten C parser. It filters
Clang's JSON dump down to source-file function definitions, extracts a compact
model (`Enum`, `Function`, `Decl`, `Stmt`, `Expr`, `CType`, `Loc`), and
preserves each function's raw JSON node for later features that need facts not
yet modeled.

### lower

The lowerer is the only stage that knows CIR op semantics. It emits **structured
`rust_ast` nodes**, not Rust source strings: every handler builds
`Item`/`Stmt`/`Expr` values that `src/codegen.rs` renders once at the end.
`format!`-ing into Rust text is not allowed. Keep the output as strongly typed as
possible — favor a new enum variant over a `String` bridge, so the compiler
enforces exhaustiveness and fixups can pattern-match the shape. If the AST cannot
express something, add the node to `src/rust_ast.rs`. Fixups follow the same rule
([writing-a-fixup.md](writing-a-fixup.md)).

### fixups

Fixups run after baseline lowering through the fixed `src/fixups::apply` entry
point (`src/fixups/mod.rs`). They must preserve the fallback property: the
lowered Rust remains correct without a given cleanup. Keep cleanup code outside
the CIR visitor unless the baseline lowering itself is wrong.

The fixups directory is split by concern:

- **`src/fixups/facts/`** — read-only analysis. `facts::analyze(program)`
  returns `AnalyzedProgram { program, facts }`, where `facts: FixupFacts`
  aggregates per-function analyses (definition/use, effects/purity, control
  flow, casts, loop shapes, pointer/string/heap provenance, and more — one
  module per concern, `src/fixups/facts/mod.rs` orchestrates them). Fact
  collectors walk the tree with the shared, immutable walkers in
  `src/fixups/facts/walk.rs`. See [facts.md](facts.md) for what each
  collector proves and which pass below consumes it.
- **`src/fixups/rewrite/`** — the actual AST-to-AST rewrite passes. Each
  `fixup(...)` takes the AST to rewrite plus `&FixupFacts` (and, for
  per-function passes, the `Function` fact record) and mutates in place,
  usually returning whether it changed anything. Rewrites share the mutable,
  path-aware walkers in `src/fixups/support/walk.rs`.
- **`src/fixups/idents.rs`** — ident-occurrence counting, used to prove a
  binding is single-use or dead before folding or dropping it.
- **`src/fixups/trace.rs`** — structured debug logging for `fixup-debug`.
  The normal `translate`/`apply` path passes a `NoopLogger`; only
  `fixup-debug` uses the collecting logger and renders pass summaries,
  rewrite events, snippets, and facts.

`apply` is a straight-line sequence, not a scheduler: it calls `facts::analyze`
again whenever an earlier rewrite could have invalidated the facts a later pass
needs, and re-runs several passes through `to_fixpoint_items` or
`to_fixpoint_items_with_facts` since one fold can expose another. Order matters — see
[writing-a-fixup.md](writing-a-fixup.md#register-the-pass) for how to place a
new pass in that sequence.

### The pass sequence

This is the order `src/fixups::apply` (`src/fixups/mod.rs`) actually runs in.
Every pass listed below lives at `src/fixups/rewrite/<name>.rs`, so the module
name is enough to find it. "To fixpoint" means the pass re-runs until a round
makes no change; facts-backed runners explicitly recompute facts each round.
"Once" means it runs exactly one time per `apply` call.

1. `goto` - restructure the goto dispatch loop into structured control flow.
2. `switch` - collapse a fallthrough-free switch dispatch loop into a direct `match`.
3. `early_inline_temps` - inline single-use pure temps (early variant).
4. `anonymous_structs` - hoist anonymous records into named tuple structs.
5. `param_spills` - fold a parameter's stack spill into its binding.
6. `zero_init` (`cross_effects = false`) - fuse a zero-init `let` with the assignment that overwrites it.
7. `struct_field_init` - fold field assignments into the preceding struct literal.
8. `singleton_scopes` - unwrap a one-statement `{ }` scope, or a `while`/`do-while` loop's redundant body scope around its negated-break guard.
9. `compound_assign` - recover `a -= 5`-style compound assignment from its expanded form.
10. `for_continue` - invert synthetic continue-blocks, then re-run `singleton_scopes`.
11. `constant_index_casts` - drop redundant `as usize` on constant indices.
12. `unnecessary_casts` - drop casts a typed context already makes redundant.
13. `call_args` - inline single-use call-argument temps.
14. `retval` - collapse a return-slot store into the final return/exit.
15. `final_return_temps` - collapse a return-value temp into the final `return`.
16. `lazy_singleton` - recover the "static flag guards a static payload" lazy-init idiom into `std::sync::OnceLock::get_or_init`.
17. `drop_call_results` - turn `let _v = call();` into `call();` when unused.
18. `string_lift` - lift NUL-terminated buffers to `CStr`/`str`/byte slices.
19. `string_params` - turn a C-string pointer parameter into `&str` (re-run after `string_copy` and `printf_format` since each can expose a new liftable parameter).
20. `ptr_len` - pair a pointer+length parameter into a slice parameter.
21. `slice_index` - rewrite pointer-offset derefs into `slice[i]` once the param is a slice.
22. `slice_loop` - recover `for x in slice.iter()`/`.iter_mut()`, or `for (i, x) in ....enumerate()` when the body also reads the index directly.
23. `slice_reduce` - fold a slice-iterator accumulator loop into `.sum()`/`.product()`/`.fold()`.
24. `range_loop` - recover `for i in 0..bound` for the remaining counted loops.
25. `va_list` - remove redundant `va_list` clone/alias bookkeeping.
26. `remove_mut` - drop `mut` where facts prove no mutation (re-run after later passes that can make a binding provably immutable).
27. `string_copy` - turn `strcpy`/`strcat`-only buffers into an owned `String`.
28. `string_libc` - rewrite `strlen`/`strcmp`-family calls on lifted strings to native Rust.
29. `sort_search` - rewrite `qsort`/`bsearch` to `.sort_by()`/`.binary_search_by()`.
30. `heap_ownership` - rewrite `malloc`/`calloc`/`realloc`/`free` to `Box`/`Vec`.
31. `dead_locals` - remove locals with no live, effectful use.
32. `printf_format` - rewrite `printf`-family calls to `println!`/`print!`.
33. `c_strings` - mark/simplify recognized C-string literals.
34. `stdio` - rewrite `fopen`/`fputs`/`fclose` sequences (plus `fgets`-echo loops and `fread`/`fwrite`) to `File`/`OpenOptions` owners.
35. `memchr_prelude::fixup_calls` - recognize hand-written byte-scan loops as `memchr` calls.
36. `nullable_pointer` - recover `Option<*T>` null-check idioms over dynamic-index search results.
37. `string_lift::fixup_c_strings` then `memchr_prelude` - a second, narrower string-lift pass followed by the memchr helper's lifecycle (deleted if unused, otherwise given its idiomatic fallback body).
38. `late_inline_temps` - inline single-use pure temps (late variant).
39. `ptr_copy` - recover a raw `std::ptr::copy`/`memcpy`/non-overlapping-`memmove` call between provably distinct, in-bounds local buffers into `dst = src` or `dst[..n].copy_from_slice(&src[..n])`, when the result is unused.
40. `mem_move` - recover a raw `std::ptr::copy`/`memmove` call within a single local buffer into `buf.copy_within(src_range, dst_start)`, when the offsets, length, and result are provable/unused.
41. `mem_set` - recover a raw `memset`/`bzero`/`std::ptr::write_bytes` call into `[..].fill(value)` when its destination, fill value, and length are all provable and its result is unused.
42. `mem_cmp` - recover a `memcmp(a, b, n)` compared against `0` with `==`/`!=` into `a[..n] == b[..n]` (or `a == b` for full-length compares), when the buffers, length, and comparison shape are all provable.
43. `dead_locals` - remove locals made dead by pointer-copy recovery.
44. `array_element_pointer_origin` - collapse pointer aliases back into direct array indexing.
45. `buffer_cursor` - turn pointer-cursor writes over a fixed array into cursor-struct field ops.
46. `atomic_locals` - give non-escaping `_Atomic` locals native `AtomicN` storage.
47. `late_inline_temps` - re-run late temp inlining after the pointer and atomic rewrites.
48. `zero_init` (`cross_effects = true`) - re-run the zero-init fusion, now allowed to cross intervening effects.
49. `atomic_compare_exchange` - fold a CAS temp-chain into `compare_exchange`.
50. `remove_mut` - re-run mutability cleanup after atomic compare-exchange recovery.
51. `assert_recovery` - recover `assert!(cond)` from the shim `assert()` macro's lowered `if cond { .. } else { abort(); .. }` guard, preserving the guard's result binding if it's still read elsewhere.
52. `var_aliases` - inline a `let b = a;` alias into its single later use (including the temp `assert_recovery` may leave behind).
53. `constant_conditions` - simplify constant `if` conditions and remove unreachable branches.
54. `libc_exit` - rewrite known direct `libc::exit` calls to `std::process::exit`.
55. `unused_items` - remove dead top-level struct/record/enum definitions.
56. `unused_params` - drop a function parameter that's never read and rewrite every direct call site to match.
57. `final_returns` - turn `return <expr>;` into plain `<expr>` at the end of a function.
58. `main_zero_exit` - drop a trailing `std::process::exit(0)` in `main`.
59. `prune_unused_definitions` - delete now-dead known libc `extern` declarations and generated support modules.

The repeated passes (`remove_mut`, `string_params`, `string_libc`) exist
because later groups can create new opportunities for earlier ones; re-running
the whole pipeline to a global fixpoint was judged not worth the compile time,
so those specific re-runs are placed by hand where they matter. If you add a
pass that creates a similar opportunity for an earlier pass, place an explicit
extra call rather than reaching for a global fixpoint loop.

## Debugging the pass sequence

Use `fixup-debug` to inspect what the fixed pass sequence did without changing
normal translation:

```bash
cargo run -- fixup-debug tests/fixtures/mem_memchr.c
cargo run -- fixup-debug tests/fixtures/mem_memchr.c --up-to-pass memchr_prelude
cargo run -- fixup-debug tests/fixtures/mem_memchr.c --only-pass late_inline_temps
cargo run -- fixup-debug tests/fixtures/mem_memchr.c --debug-only-pass late_inline_temps
```

Pass names are the strings from `src/fixups/trace.rs`'s `Pass` enum, for
example `zero_init`, `late_inline_temps`, `dead_locals`, and
`memchr_prelude::fixup_calls`. An unknown pass name fails with the valid names.

`--up-to-pass <pass>` runs the normal ordered pipeline and stops after the first
matching pass invocation. This is useful when a later pass hides the tree shape
you need to inspect.

`--only-pass <pass>` walks the same sequence but only applies and logs matching
pass invocations. It still starts from the lowered Rust and still recomputes
facts at the same sequence points, but skipped passes are not applied. Repeated
passes with the same enum name run at each matching sequence point.

`--debug-only-pass <pass>` runs the normal ordered pipeline but only logs
matching pass invocations. Use this when the pass needs earlier rewrites to build
the AST shape you are debugging.

Human output is grouped by pass invocation and then by function:

```text
zero_init                          changed; stmts -1, temp_lets +0, items +0
  function main:
    fold_zero_init_assignment
      at fn main
      before:
        definition:
          fn main() {
              let mut x: i32 = 0;
              x = 10;
              ...
          }
      after:
        definition:
          fn main() {
              let mut x: i32 = 10;
              ...
          }
      facts:
        query_rule=fold_zero_init_assignment
        query_case=direct
        evidence.zero_init=moved_decl=false
```

Passes that make no textual change are still listed as `skipped`. If a changed
pass has not yet been instrumented with pass-local rewrite events, debug output
emits one generic `pass_changed` event with whole-program before/after snippets
and summary facts. Treat that as a migration bridge: pass-local events should
replace it when there is a useful domain-specific explanation. Locations use
source file and line when available; otherwise they use the function name and AST
path.

## Adding a feature

A feature expands baseline C coverage. Examples: structs, arrays, pointer
arithmetic, new arithmetic operators, globals, `if`, `switch`.

See [adding-features.md](adding-features.md) for the step-by-step workflow. The
short version is: add a C fixture under `tests/fixtures/`, inspect CIR and Clang
AST as needed, implement conservative baseline lowering, and run the test suite.
Ignored generated fixture trees are refreshed manually only when requested.

## Adding a fixup

A fixup improves already-correct Rust. Examples: `printf -> println!`,
collapsing retval temps, inlining single-use temps, or recovering `for` loops.

See [adding-features.md](adding-features.md) for the split between baseline
language work and fixups, and [writing-a-fixup.md](writing-a-fixup.md) for the
AST-to-AST pass recipe. A fixup must start from generated Rust that already
passes differential testing.

For `printf -> println!`, only rewrite when the callee is known, the format
argument is a constant C string, every format specifier is supported, and Rust
formatting can express the same output. Everything else stays as
`unsafe { libc::printf(...) }`.
