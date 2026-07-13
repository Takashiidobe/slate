# Writing a Fixup

A fixup is a **Rust-AST → Rust-AST** transformation that makes already-correct
baseline Rust cleaner. It runs after lowering, inside `src/fixups::apply`, and
must be optional in spirit: disable it and the generated Rust is still correct.

This doc is the how. For *whether* your change is a fixup or a baseline feature,
read [adding-features.md](adding-features.md) first.

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

Each pass lives in its own file under `src/fixups/` and exposes one entry point
that walks a statement list. The canonical example is
[`src/fixups/compound_assign.rs`](../src/fixups/compound_assign.rs), which turns
`a = a - 5` into `a -= 5`:

```rust
pub(super) fn fixup(body: &mut Vec<IndentStmt>) {
    for indent in body.iter_mut() {
        match &mut indent.stmt {
            // 1. recurse into every nested body first
            Stmt::If { then_body, else_body, .. }
            | Stmt::LetIf { then_body, else_body, .. } => {
                fixup(then_body);
                fixup(else_body);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                fixup(body)
            }
            Stmt::Unsafe { body } => fixup(&mut body.stmts),
            // 2. do the local rewrite
            Stmt::Assign { target, value } => { /* match, then reassign indent.stmt */ }
            _ => {}
        }
    }
}
```

Recursing into nested bodies is boilerplate repeated per pass (see
`zero_init::for_nested_body`, `inline_temps::inline_nested_temps`). Match the
existing set of nested-body arms exactly so a pass reaches statements inside
`if`/loops/scopes/`unsafe`.

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
- `src/fixups/inline_temps.rs` — `walk_stmt_exprs`.

Treat the new node like its closest existing sibling in the walkers (a
`CompoundAssign` counts/substitutes/walks exactly like `Assign`).

## Register the pass

Add it to `src/fixups/mod.rs`: a `mod` line and a call inside `apply`. **Order
matters** — put your pass where its input already exists. `compound_assign` runs
after `inline_temps` (so the `a - 5` binop is already assembled) and after
`zero_init`, but before `retval`:

```rust
inline_temps::fixup(&mut f.body);
param_spills::fixup(&mut f);
zero_init::fixup(&mut f.body);
compound_assign::fixup(&mut f.body);   // new
retval::fixup(&mut f.body);
```

## Test it

1. **Unit tests, in-file.** Build input with the `test_support.rs` helpers
   (`func`, `let_mut`, `assign`, `bin`, `var`, `int`, ...), run your `fixup`, and
   assert the emitted string with `after_body`/`emit`. Cover both the fold and the
   cases you deliberately leave alone (impure rhs, non-matching shape).
2. **Output-shape coverage.** Add or update an assertion in `tests/differential.rs`
   that the generated Rust for a real fixture now contains the cleaner form.
3. **Differential must stay green** — the fixture's C and Rust outputs must still
   match. This is the correctness backstop: a fixup that changes behavior fails
   here.

```bash
cargo fmt
cargo nextest r --release
```

Both must pass before the work is done.
