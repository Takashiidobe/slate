# Writing a Fixup

A fixup is a **Rust-AST → Rust-AST** transformation that makes already-correct
baseline Rust cleaner. It runs after lowering, inside `src/fixups::apply`, and
must be optional in spirit: disable it and the generated Rust is still correct.

This doc is the how. For _whether_ your change is a fixup or a baseline feature,
read [adding-features.md](adding-features.md) first.

If the fixup needs semantic trace coverage or changes the shape that the effects
interpreter sees, read [effects.md](effects.md) as well.

## The one rule

**Operate on AST nodes, never on rendered text.** Baseline lowering builds a
structured `Program` (`src/rust_ast.rs`); fixups rewrite that tree; `src/codegen.rs`
renders it once at the end. There is no string-rewrite stage — do not add one. A
fixup that matches on or splices rendered strings will silently break precedence,
escaping, and later passes.

Rendering is precedence-aware (`Expr::render`, `BinOp::precedence`), so you never
manage parentheses yourself. Build the node; the renderer parenthesizes minimally.

## What you transform

```
Program { items: Vec<Item> }
  Item::Fn(FnDef { params, ret, body: Vec<IndentStmt>, .. })
  Item::Raw(String)                 // items not yet structured; skip these
IndentStmt { depth: usize, stmt: Stmt }
Stmt::{ Let, Assign, CompoundAssign, Expr, Return, If, LetIf, Loop, Scope,
        LabeledBlock, Match, While, Block, Unsafe, Break, Continue }
Expr::{ Value, Var, Binary, Unary, Cast, Call, MethodCall, Field, Index, ... }
```

`IndentStmt.depth` is the nesting level used only for rendering. Preserve it: when
you rewrite a statement in place, keep its `depth`; when you build a nested body,
copy the surrounding depth like the existing passes do.

## Shape of a pass

A body pass lives in its own file under `src/fixups/rewrite/`, stores every
dependency on its pass struct, and implements the shared `Fixup` trait. Its
`fixup` method takes only the statement list and reports whether that invocation
changed it:

```rust
pub(in crate::fixups) struct MyPass<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for MyPass<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        rewrite_body(body, self.function, self.facts, self.logger)
    }
}
```

Recursing into nested bodies is boilerplate repeated per pass. Prefer the
path-aware helpers in `src/fixups/support/walk.rs`; if a traversal shape is
missing, extend the shared helper instead of adding a private walker. Match the
existing nested-body coverage so a pass reaches statements inside
`if`/loops/scopes/`unsafe`.

Do not add a module-level function that only creates a `NoopLogger` and
delegates. `src/fixups/mod.rs` constructs the pass struct with the active logger
and chooses `run_once_items` or `to_fixpoint_items`. Facts-backed fixpoint passes
use `to_fixpoint_items_with_facts`, which reanalyzes before every program round.
Program-wide and `FnDef`-shape rewrites use the corresponding program or
per-function runner without pretending to be body-only passes. Keep event-only
work behind `self.logger.is_enabled()` so the normal path does not clone AST
nodes or render snippets.

## Reuse the shared helpers

Do not re-walk the tree by hand when a helper exists:

- **`src/fixups/idents.rs`** — `expr_ident(e)` (is this a bare variable?),
  `expr_ident_count(e, name)` / `stmt_ident_count(s, name)` (how many times is
  `name` read?). Use counts to prove single-use or no-use before moving code.
- **`src/fixups/support/walk.rs`** — rewrite-only traversal helpers. Use the
  path-aware nested-body helpers to keep rewrite paths aligned with facts, and
  use the mutable expression helpers for AST rewrites that need to skip default
  recursion after replacing a node.
- **`src/fixups/facts/walk.rs`** — fact-only expression and statement walkers.
  Immutable collection, `any`/`all` predicates, semantic scans, and whole-tree
  discovery belong in `src/fixups/facts/`, not in rewrite modules. A rewrite
  should consume `FixupFacts` plus local AST shape; if it needs information that
  is not already in `FixupFacts`, add a fact collector first.
- **`Stmt::substitute_var(name, repl)` / `Expr::substitute_var(name, repl)`**
  (`src/rust_ast.rs`) — replace every `Expr::Var(name)` subtree with a cloned
  expression, returning whether anything changed. This is how `inline_temps`
  splices a temp's init into its use site.

## Safety: stay conservative

A fixup must never change observable behavior. Copy the guardrails the existing
passes already use, and keep them tight:

- **Purity** — before moving, duplicating, or reordering an expression, require it
  pure (values, vars, and pure arithmetic; no calls, no volatile intrinsics). See
  `is_pure_expr` in `inline_temps.rs`/`compound_assign.rs`. This is what excludes
  `read_volatile`/`write_volatile` and function calls automatically.
- **Single-use / no-use** — use the ident counters to prove a temp is read exactly
  once (to inline it) or not at all (to drop it) before you remove its binding.
- **Don't cross side effects** — never hoist a rewrite past a call or store that
  could observe or change the value.
- **Simple slots only** — restrict place-changing rewrites to plain `Expr::Var`
  targets. Leave complex lvalues (`*p`, `a[i]`, fields) and volatile accesses on
  the baseline path until their safety rules are explicit.

When CIR admits several equivalent source spellings (e.g. `a -= 5` vs
`a = a - 5`), state the preferred one in the module doc-comment and fold toward it
only in the safe cases.

## Need a new AST node?

If the cleaner Rust has no representation yet (e.g. `CompoundAssign` for `a -= 5`),
add the variant to `src/rust_ast.rs` and let the compiler's exhaustiveness
checking find every match to update. For a new `Stmt` variant that is a
target+value pair, the sites are:

- `src/rust_ast.rs` — the `enum` definition and `stmt_substitute_var`.
- `src/codegen.rs` — how it renders.
- `src/fixups/idents.rs` — `stmt_ident_count`.
- `src/fixups/rewrite/inline_temps.rs` and `src/fixups/support/walk.rs` — the
  expression/body traversal sites that need to know how the new node contains
  children.

Treat the new node like its closest existing sibling in the walkers (a
`CompoundAssign` counts/substitutes/walks exactly like `Assign`).

## Register the pass

Add it to `src/fixups/mod.rs`: a `mod` line and a call inside
`apply_with_logger`. **Order matters** — put your pass where its input already
exists. Instantiate the concrete pass at the call site and make its scheduling
mode explicit:

```rust
run_once_items(&mut program, |item_index, f| {
    let Some(function) = facts.function_by_item_index(item_index) else {
        return false;
    };
    let mut fixup = MyPass::new(function, &facts, logger);
    run_once(&mut f.body, &mut fixup)
});
```

Use `to_fixpoint_items` when rewrites can cascade without invalidating facts,
and `to_fixpoint_items_with_facts` when every round requires fresh facts. A
single `Fixup::fixup` invocation must perform one round; convergence belongs to
the runner.

If the pass should appear in `fixup-debug`, add a variant to
`src/fixups/trace.rs`'s `Pass` enum and wire that enum value at the pass boundary
in `src/fixups/mod.rs`. Use that same enum value for every `RewriteEvent.pass`
the pass emits. The string returned by `Pass::name()` is also the CLI spelling
for `fixup-debug --up-to-pass <pass>`, `--only-pass <pass>`, and
`--debug-only-pass <pass>`.

## Structured debug events

Instrument a pass when the event explains a real rewrite decision better than a
whole-program before/after diff. Emit events only around concrete rewrite points
or deliberate candidate skips worth debugging. Each event should include:

- `pass`: the `Pass` enum variant for the active pass.
- `kind`: a stable, pass-local action name such as `inline_temp`,
  `fold_zero_init_assignment`, or `remove_dead_local`.
- `location`: source file/function/line when available, otherwise function name
  plus AST path. Use `function_path_location(facts, function, path)` for
  facts-backed per-function passes, or `path_location(path)` when there is no
  function fact.
- `before`: cloned AST nodes from immediately before the rewrite.
- `after`: cloned AST nodes from immediately after the rewrite, or empty when a
  node is removed.
- `facts`: stable key/value strings for the facts and guard decisions that made
  the rewrite legal.

For example, `zero_init` folding:

```text
before:
  declaration:
    let mut x: i32 = 0;
  assignment:
    x = 10;
after:
  declaration:
    let mut x: i32 = 10;
facts:
  binding_name=x
  binding_is_zero=true
  assignment_reads_binding=false
  assignment_writes_binding=true
```

For `late_inline_temps`, include both the producer temp and the consumer
statement. This is especially useful for method-argument or closure-shaped
consumers, where a temp may be folded into a call argument while preserving the
surrounding receiver/call shape:

```text
before:
  producer:
    let _v3 = buf.as_slice().iter().position(|__slate_byte| *__slate_byte == needle);
  consumer:
    let _v11: i64 = _v3.unwrap() as i64;
after:
  consumer:
    let _v11: i64 = buf.as_slice().iter().position(|__slate_byte| *__slate_byte == needle).unwrap() as i64;
facts:
  temp=_v3
  phase=late
  reads=1
  producer_path=stmt[2]
  consumer_path=stmt[3]
```

Rendered snippets are for logs only. Do not match on rendered snippets, parse
them, or use them to drive a rewrite. The rewrite must still use structured
`Stmt`/`Expr` nodes plus `FixupFacts`; rendering belongs only in the trace
payload after the AST nodes have already been selected.

Good fact keys are stable and domain-specific: binding id/name, read/write
counts, producer/consumer paths, purity/effect summaries, and guard results such
as `source_changes_between=false`. Avoid dumping arbitrary debug text when a
small set of facts explains the decision.

If a changed pass emits no events, `CollectingLogger` adds a generic
`pass_changed` event with whole-program before/after snippets and summary facts.
That keeps `fixup-debug` complete across the pipeline, but it is not the desired
end state for a pass with meaningful rewrite decisions. Replace the generic
event by emitting at least one pass-local event when you instrument that pass.

## Test it

1. **Fixture coverage.** Add or update a C fixture under `tests/fixtures/`.
2. **Output-shape coverage.** Add or update an assertion in `tests/differential.rs`
   that the generated Rust for a real fixture now contains the cleaner form.
3. **Differential must stay green** — the fixture's C and Rust outputs must still
   match. This is the correctness backstop: a fixup that changes behavior fails
   here.

```bash
cargo fmt
cargo clippy --all-targets
cargo nextest r --release
```

Both must pass before the work is done.
