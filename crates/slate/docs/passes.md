# Passes

The translator is a small fixed pipeline. Keep it that way until there is a real
need for pass scheduling or configuration.

## V0 principle: transliterate, don't optimize

Baseline lowering emits the most mechanical, faithful Rust it can, leaning on
`libc` and `unsafe` wherever C semantics do not map directly to safe Rust. Every
CIR value is materialized into its own `let` temp. This is ugly, but it freezes
each value at its CIR definition and avoids load-after-mutation hazards.

Readability is recovered later by Rust fixups, not during baseline lowering.

## Current pipeline

| Stage              | In -> Out                               | How                                                                    |
| ------------------- | --------------------------------------- | ------------------------------------------------------------------------ |
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
[cir-op-prioritization.md](cir-op-prioritization.md) (exhaustive CIR op
checklist). Unknown CIR ops emit a `todo!("cir.xyz")` expression and a
diagnostic — failing loudly is intentional; it is better than silently
dropping semantics.

## Stage notes

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
  `src/fixups/facts/walk.rs`.
- **`src/fixups/rewrite/`** — the actual AST-to-AST rewrite passes. Each
  `fixup(...)` takes the AST to rewrite plus `&FixupFacts` (and, for
  per-function passes, the `Function` fact record) and mutates in place,
  usually returning whether it changed anything. Rewrites share the mutable,
  path-aware walkers in `src/fixups/support/walk.rs`.
- **`src/fixups/idents.rs`** — ident-occurrence counting, used to prove a
  binding is single-use or dead before folding or dropping it.

`apply` is a straight-line sequence, not a scheduler: it calls `facts::analyze`
again whenever an earlier rewrite could have invalidated the facts a later pass
needs, and re-runs several passes to a fixpoint (`inline_temps_to_fixpoint`,
`zero_init_to_fixpoint`, `dead_locals_to_fixpoint`, ...) since one fold can
expose another. Order matters — see
[writing-a-fixup.md](writing-a-fixup.md#register-the-pass) for how to place a
new pass in that sequence.

### The pass sequence

This is the order `src/fixups::apply` (`src/fixups/mod.rs`) actually runs in.
Every pass listed below lives at `src/fixups/rewrite/<name>.rs` and exposes a
`fixup` entry point, so the module name is enough to find it. "To fixpoint"
means the pass re-runs - recomputing facts each round - until a round makes no
change; "once" means it runs exactly one time per `apply` call.

1. `goto` - restructure the goto dispatch loop into structured control flow - to fixpoint, per function (`structure_goto`).
2. `early_inline_temps` - inline single-use pure temps, early variant - to fixpoint (`inline_temps_to_fixpoint`), capped at 5 rounds for very large functions (`> 2_000` statements) so pathological cases don't spin.
3. `anonymous_structs` - hoist repeated anonymous-struct shapes into named structs - once.
4. `param_spills` - fold a parameter's stack spill into its binding - once, per function.
5. `zero_init` (`cross_effects = false`) - fuse a zero-init `let` with the assignment that overwrites it - to fixpoint (`zero_init_to_fixpoint`).
6. `struct_field_init` - fold field assignments into the preceding struct literal - to fixpoint (`struct_field_init_to_fixpoint`).
7. `singleton_scopes` - unwrap a one-statement `{ }` scope - to fixpoint (`singleton_scopes_to_fixpoint`).
8. `compound_assign` - recover `a -= 5` - once, per function.
9. `for_continue` - invert synthetic continue-blocks - to fixpoint (`cleanup_for_continues`), then `singleton_scopes` again to fixpoint.
10. `constant_index_casts` - drop redundant `as usize` on constant indices - once, per function.
11. `unnecessary_casts` - drop casts a typed context already makes redundant - once, per function.
12. `call_args` - inline single-use call-argument temps - to fixpoint, per function, across the whole program.
13. `retval` - collapse a return-slot store into the final return/exit - once, per function.
14. `final_return_temps` - collapse a return-value temp into the final `return` - to fixpoint, per function, across the whole program.
15. `drop_call_results` - turn `let _v = call();` into `call();` when unused - once, per function.
16. `string_lift` - lift NUL-terminated buffers to `CStr`/`str`/byte slices - once, per function.
17. `string_params` - turn a C-string pointer parameter into `&str` - to fixpoint (its own `loop { ... }`); re-run three more times later in the sequence (after `string_copy`, after `string_libc`'s first pass, and after `printf_format`), since each of those can create a new liftable parameter.
18. `ptr_len` - pair a pointer+length parameter into a slice parameter - once.
19. `slice_index` - rewrite pointer-offset derefs into `slice[i]` once the param is a slice - once.
20. `slice_loop` - recover `for x in slice.iter()/.iter_mut()`, or `for (i, x) in slice.iter()/.iter_mut().enumerate()` when the body also reads the index directly (re-casting the `usize` enumerate index back to its original type via a shadowing `let`) - once; if it changed anything, `late_loop_cleanup` runs, itself `singleton_scopes` + `dead_locals` to fixpoint.
21. `slice_reduce` - fold a slice-iterator accumulator loop into `.sum()`/`.product()`/`.fold()` - once; runs right after `slice_loop` (its only producer) since it consumes the `for`-loop shape that pass emits; same conditional `late_loop_cleanup` as above.
22. `range_loop` - recover `for i in 0..bound` for the remaining counted loops - once; same conditional `late_loop_cleanup` as above.
23. `va_list` - remove redundant `va_list` clone/alias bookkeeping - once.
24. `remove_mut` - drop `mut` where facts prove no mutation - once, per function; re-run as a bare pass four more times later in the sequence, after each group of passes that could have made a binding provably immutable (`string_copy`, `heap_ownership`, `printf_format`, `atomic_compare_exchange`).
25. `string_copy` - `strcpy`/`strcat`-only buffers to owned `String` - once.
26. `string_libc` - `strlen`/`strcmp`-family calls on lifted strings to native Rust - once; repeated once more later (after `c_strings`) since lifting more C strings exposes more libc calls to rewrite.
27. `sort_search` - `qsort`/`bsearch` to `.sort_by()`/`.binary_search_by()` - once.
28. `heap_ownership` - `malloc`/`calloc`/`realloc`/`free` to `Box`/`Vec` - once.
29. `dead_locals` - remove locals with no live, effectful use - to fixpoint (`dead_locals_to_fixpoint`), per function, across the program.
30. `printf_format` - `printf`-family calls to `println!`/`print!` - once.
31. `c_strings` - mark/simplify recognized C-string literals - once.
32. `stdio` - `fopen`/`fputs`/`fclose` sequences to `File`/`OpenOptions` owners - once.
33. `memchr_prelude::fixup_calls` - recognize hand-written byte-scan loops as `memchr` calls - once.
34. `nullable_pointer` - recover `Option<*T>` null-check idioms - to fixpoint (its own `loop { ... }`); runs directly after its only producers - the two `string_libc::fixup` runs and `memchr_prelude::fixup_calls` (32), the sole places that emit the `<index>.map_or(null_mut(), |i| ptr.add(i) as *T)` shape it rewrites. Despite the name, it has no relationship to the pointer-provenance cluster (`slice_index`/`slice_loop`/`array_element_pointer_origin`/`buffer_cursor`): those match constant-index pointer arithmetic, this matches dynamic-index `Option`-wrapped search results, and the two never touch the same bindings.
35. `string_lift::fixup_c_strings` then `memchr_prelude` / `memchr_prelude::prune_unused_helper` - a second, narrower string-lift pass plus memchr-helper cleanup - once each.
36. `array_element_pointer_origin` - collapse pointer aliases back into direct array indexing - once.
37. `buffer_cursor` - turn pointer-cursor writes over a fixed array into cursor-struct field ops - once.
38. `atomic_locals` - give non-escaping `_Atomic` locals native `AtomicN` storage - once.
39. `late_inline_temps` - inline single-use pure temps, late variant - to fixpoint (`inline_temps_to_fixpoint`, same round cap as step 2).
40. `zero_init` (`cross_effects = true`) - same fusion as step 5, now allowed to cross intervening effects - to fixpoint.
41. `atomic_compare_exchange` - fold a CAS temp-chain into `compare_exchange` - to fixpoint, per function, across the program.
42. `var_aliases` - inline a `let b = a;` alias into its single later use - to fixpoint (`inline_var_aliases_to_fixpoint`).
43. `prune_unused_externs` - drop now-dead `extern` decls for the libc functions `string_copy`, `string_libc`, `sort_search`, and `heap_ownership` replace - once, after all four rewrites (and their re-runs) have finished, rather than once per rewrite.
44. `unused_items` - remove dead top-level items - once.
45. `unused_params` - drop a function parameter that's never read in its body and rewrite every direct call site to match, once the function's only references are direct-by-name calls whose argument at that slot is pure and whose type can't own a destructor - to fixpoint.
46. `main_zero_exit` - drop a trailing `std::process::exit(0)` in `main` - once, per function.

The repeated passes (`remove_mut`, `string_params`, `string_libc`) exist
because later groups can create new opportunities for earlier ones; re-running
the whole pipeline to a global fixpoint was judged not worth the compile time,
so those specific re-runs are placed by hand where they matter. If you add a
pass that creates a similar opportunity for an earlier pass, place an explicit
extra call rather than reaching for a global fixpoint loop.

## Adding a feature

A feature expands baseline C coverage. Examples: structs, arrays, pointer
arithmetic, new arithmetic operators, globals, `if`, `switch`.

See [adding-features.md](adding-features.md) for the step-by-step workflow. The
short version is: add a C fixture under `tests/fixtures/`, inspect CIR and Clang
AST as needed, implement conservative baseline lowering, run the test suite, and
refresh ignored generated fixtures with `cargo run -- emit-fixtures`.

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
