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

1. `goto` - restructure the goto dispatch loop into structured control flow - to fixpoint, per function (`to_fixpoint_items`).
2. `switch` - collapse a fallthrough-free switch dispatch loop into a direct `match` over the selector expression - once, per function.
3. `early_inline_temps` - inline single-use pure temps, early variant - to fixpoint with facts refreshed before every round (`to_fixpoint_items_with_facts`), capped at 5 rounds for very large functions (`> 2_000` statements) so pathological cases don't spin.
4. `anonymous_structs` - query the complete anonymous-record domain and atomically hoist its definitions, types, literals, and field accesses into named tuple structs - once.
5. `param_spills` - fold a parameter's stack spill into its binding - once, per function.
6. `zero_init` (`cross_effects = false`) - fuse a zero-init `let` with the assignment that overwrites it - to fixpoint through a query definition plan, with facts refreshed before every round (`to_fixpoint_program_with_facts`).
7. `struct_field_init` - fold field assignments into the preceding struct literal - to fixpoint (`to_fixpoint_items`).
8. `singleton_scopes` - unwrap a one-statement `{ }` scope - to fixpoint (`to_fixpoint_items`).
9. `compound_assign` - recover `a -= 5` - once, per function.
10. `for_continue` - invert synthetic continue-blocks - to fixpoint (`to_fixpoint_items`), then `singleton_scopes` again to fixpoint.
11. `constant_index_casts` - drop redundant `as usize` on constant indices - once, per function (`run_once_items`).
12. `unnecessary_casts` - drop casts a typed context already makes redundant - once, program-wide, through a query expression plan over every assignment's value.
13. `call_args` - inline single-use call-argument temps - to fixpoint with facts refreshed before every round (`to_fixpoint_items_with_facts`).
14. `retval` - collapse a return-slot store into the final return/exit - once, per function (`run_once_items`).
15. `final_return_temps` - collapse a return-value temp into the final `return` - to fixpoint with facts refreshed before every round (`to_fixpoint_items_with_facts`).
16. `lazy_singleton` - recover the "static flag guards a static payload" lazy-init idiom into `std::sync::OnceLock::get_or_init` - once, program-wide (`run_once_program`).
17. `drop_call_results` - turn `let _v = call();` into `call();` when unused - once, per function (`run_once_items`).
18. `string_lift` - lift NUL-terminated buffers to `CStr`/`str`/byte slices - once, per function (`run_once_items`).
19. `string_params` - turn a C-string pointer parameter into `&str` - program-wide to fixpoint with facts refreshed before every round (`to_fixpoint_program_with_facts`); re-run twice later in the sequence after `string_copy` and `printf_format`, since each can create a new liftable parameter.
20. `ptr_len` - pair a pointer+length parameter into a slice parameter - once, program-wide (`run_once_program`).
21. `slice_index` - rewrite pointer-offset derefs into `slice[i]` once the param is a slice - once, per function (`run_once_items`).
22. `slice_loop` - recover `for x in slice.iter()/.iter_mut()`, or `for (i, x) in slice.iter()/.iter_mut().enumerate()` when the body also reads the index directly (re-casting the `usize` enumerate index back to its original type via a shadowing `let`) - once, per function (`run_once_items`); if it changed anything, `late_loop_cleanup` runs, itself `singleton_scopes` + `dead_locals` to fixpoint.
23. `slice_reduce` - fold a slice-iterator accumulator loop into `.sum()`/`.product()`/`.fold()` - once, per function (`run_once_items`); runs right after `slice_loop` (its only producer) since it consumes the `for`-loop shape that pass emits; same conditional `late_loop_cleanup` as above.
24. `range_loop` - recover `for i in 0..bound` for the remaining counted loops - once, per function (`run_once_items`); same conditional `late_loop_cleanup` as above.
25. `va_list` - remove redundant `va_list` clone/alias bookkeeping - once, per function (`run_once_items`); it remains an explicit `FnDef`-level exception because it renames a parameter while removing its body aliases.
26. `remove_mut` - drop `mut` where facts prove no mutation - once, per function (`run_once_items`); re-run through the same explicit runner four more times later in the sequence, after each group of passes that could have made a binding provably immutable (`string_copy`, `heap_ownership`, `printf_format`, `atomic_compare_exchange`). It remains an explicit `FnDef`-level exception because it rewrites parameters as well as the function body.
27. `string_copy` - `strcpy`/`strcat`-only buffers to owned `String` - once, per function (`run_once_items`).
28. `string_libc` - `strlen`/`strcmp`-family calls on lifted strings to native Rust - once, per function (`run_once_items`), followed by program-level insertion of the numeric-parse runtime helper when the body rewrites require it; repeated once more later (after `c_strings`) since lifting more C strings exposes more libc calls to rewrite.
29. `sort_search` - `qsort`/`bsearch` to `.sort_by()`/`.binary_search_by()` - once, program-wide (`run_once_program`), since record layouts and comparator functions must be discovered across items before function bodies are rewritten.
30. `heap_ownership` - `malloc`/`calloc`/`realloc`/`free` to `Box`/`Vec` - once, per function (`run_once_items`).
31. `dead_locals` - remove locals with no live, effectful use - to fixpoint, per function, with facts refreshed before every program round (`to_fixpoint_items_with_facts`).
32. `printf_format` - `printf`-family calls to `println!`/`print!` - once, program-wide (`run_once_program`), since the pass also prunes or extends extern declarations and wraps any remaining raw calls.
33. `c_strings` - mark/simplify recognized C-string literals - once, per function (`run_once_items`).
34. `stdio` - `fopen`/`fputs`/`fclose` sequences to `File`/`OpenOptions` owners - once, per function (`run_once_items`).
35. `memchr_prelude::fixup_calls` - recognize hand-written byte-scan loops as `memchr` calls - once, per function (`run_once_items`) through the distinct `MemchrPreludeCalls` body pass.
36. `nullable_pointer` - recover `Option<*T>` null-check idioms - to fixpoint, per function, with facts refreshed before every program round (`to_fixpoint_items_with_facts`); runs directly after its only producers - the two `string_libc` runs and `memchr_prelude::fixup_calls`, the sole places that emit the `<index>.map_or(null_mut(), |i| ptr.add(i) as *T)` shape it rewrites. Despite the name, it has no relationship to the pointer-provenance cluster (`slice_index`/`slice_loop`/`array_element_pointer_origin`/`buffer_cursor`): those match constant-index pointer arithmetic, this matches dynamic-index `Option`-wrapped search results, and the two never touch the same bindings.
37. `string_lift::fixup_c_strings` then `memchr_prelude` - a second, narrower string-lift pass followed by a query-driven memchr definition lifecycle; the helper is deleted when its complete use domain is empty and otherwise receives the idiomatic fallback body.
38. `late_inline_temps` - inline single-use pure temps, late variant - to fixpoint, per function, with facts refreshed before every program round (`to_fixpoint_items_with_facts`, same round cap as step 3).
39. `ptr_copy` - collapse indexed pointer-copy loops into `std::ptr::copy` or `std::ptr::copy_nonoverlapping` - to fixpoint, per function (`to_fixpoint_items`).
40. `dead_locals` - remove locals made dead by pointer-copy recovery - to fixpoint, per function, with facts refreshed before every program round (`to_fixpoint_items_with_facts`).
41. `array_element_pointer_origin` - collapse pointer aliases back into direct array indexing - once, per function through a query definition plan with fresh facts.
42. `buffer_cursor` - turn pointer-cursor writes over a fixed array into cursor-struct field ops - once, per function through a query definition plan, reusing the same facts snapshot as `array_element_pointer_origin`.
43. `atomic_locals` - give non-escaping `_Atomic` locals native `AtomicN` storage - once, program-wide (`run_once_program`), because the same pass rewrites both static items and function bodies.
44. `late_inline_temps` - re-run late temp inlining after the pointer and atomic rewrites - to fixpoint, per function, with facts refreshed before every program round (`to_fixpoint_items_with_facts`, same round cap as step 3).
45. `zero_init` (`cross_effects = true`) - same query-driven fusion as step 6, now allowed to cross intervening effects - to fixpoint with facts refreshed before every round.
46. `atomic_compare_exchange` - fold a CAS temp-chain into `compare_exchange` - to fixpoint, per function, with facts refreshed before every program round (`to_fixpoint_items_with_facts`).
47. `remove_mut` - re-run mutability cleanup after atomic compare-exchange recovery - once, per function.
48. `var_aliases` - inline a `let b = a;` alias into its single later use - to fixpoint, per function, across the program (`to_fixpoint_items`).
49. `constant_conditions` - simplify constant `if` conditions and remove unreachable branches - to fixpoint, per function, across the program (`to_fixpoint_items`).
50. `libc_exit` - query known direct `libc::exit` calls and rewrite them as `std::process::exit` when the matching one-argument, never-returning extern declaration is proven.
51. `unused_items` - remove dead top-level struct/record/enum definitions, via mark-and-sweep reachability over a query definition plan - once, program-wide.
52. `unused_params` - drop a function parameter that's never read in its body and rewrite every direct call site to match, once the function's only references are direct-by-name calls whose argument at that slot is pure and whose type can't own a destructor - through a query program plan, to fixpoint.
53. `final_returns` - turn `return <expr>;` into plain `<expr>` at the end of a function - once, per function.
54. `main_zero_exit` - drop a trailing `std::process::exit(0)` in `main` - once, per function.
55. `prune_unused_definitions` - query the final complete definition-use domain and delete now-dead known libc `extern` declarations and generated support modules after all users have stabilized; empty extern containers are removed by the shared definition planner.

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
