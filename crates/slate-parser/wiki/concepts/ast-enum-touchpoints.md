# AST Enum Touchpoints

_created 2026-09-12_

`Stmt`, `Expr`, `ConstExpr`, and `ArraySize` (all in `src/ast.rs` /
`src/const_expr.rs`) are each matched exhaustively, by variant name, in
several unrelated files. The compiler will refuse to build until every one
of these is updated, but nothing points at them up front — you either grep
every constructor name across the crate or read the files end to end. This
page is that grep, done once, so the next AST change doesn't require
re-deriving it. See [[architecture_polyvariant_ast]] for why these enums
are shaped the way they are (the `Conditional<T>` wrapping).

Update this page whenever a new exhaustive match site over one of these
enums is added or removed.

Parsed nodes are stored as `Span<T>`: translation-unit declarations are
`Span<Decl>`, function bodies contain `Span<Stmt>`, expression-bearing
fields contain `Span<Expr>`, and preprocessor output uses
`Span<PPNodeKind>`. Exhaustive matches over these collections must match
the wrapper's `.value`. Evaluation preserves the wrapper while converting
`Decl`/`Stmt` into their concrete counterparts.

## Adding a `Stmt` variant

- `src/eval.rs` — `impl Span<Stmt> { fn eval }`: must lower the new variant to a
  `ConcreteStmt`. If it wraps a nested `FunctionDecl`/body, also update
  `impl Span<Decl> { fn eval }`'s sibling logic for `mark_unreachable` — see how
  `NestedFunction` mirrors the top-level `Function` case.
- `src/ast.rs` — add the matching `ConcreteStmt` variant (`Stmt` and
  `ConcreteStmt` are separate enums with matching shapes; nothing enforces
  they stay in sync except this convention).
- `tests/filecheck.rs` — `summarize_evaluated_decl`'s inner match over
  `ConcreteStmt` (used to build the clang-oracle comparison). Only matters
  once the new statement can appear where `Return` is being scanned for;
  usually just add it to the `=> None` catch-group.
- `src/reachability.rs` — **not exhaustive**, safe to skip: both
  `mark_unreachable_in` and `always_terminates` use wildcard arms
  (`other => other`, `_ => false`), so an unhandled new variant just gets
  the conservative default (reachable, doesn't terminate) rather than a
  compile error. Only touch this file if the new statement actually needs
  special reachability behavior (e.g. it always transfers control, like
  `Goto`).

## Adding an `Expr` variant

- `src/ast.rs` — `impl Display for Expr`: exhaustive, needs an arm.
- No other file matches `ast::Expr` exhaustively (checked via
  `grep -rn "Expr::" src/*.rs tests/*.rs`). `tests/filecheck.rs` matches on
  it in two places (`summarize_evaluated_decl`'s `Return` scan,
  `array_size`) but both are exhaustive over `Expr` too — see below,
  they'll fail to compile and tell you.

Note: most *general* expressions never construct `ast::Expr` directly —
they go through `const_expr::Parser` and get wrapped once as
`Expr::Const(Box<ConstExpr>)` by `Parser::parse_expression` in
`src/parser.rs`. Only a handful of special forms (`_Generic`, bare string
literals, statement-expressions `({ ... })`) are built as `Expr` variants
directly, bypassing `const_expr`. When in doubt, a new expression-level
construct belongs in `ConstExpr`, not `Expr`.

## Adding a `ConstExpr` variant

- `src/const_expr.rs` — `impl Display for ConstExpr`: exhaustive.
- `src/const_expr.rs` — `Parser::evaluate_expr`: exhaustive, must decide
  whether the new construct is foldable to `i64` or returns
  `ConstExprError::NotConstant(...)`.
- `src/sema.rs` — `is_integer_constant_expression`: exhaustive, decide
  `true`/`false` for the new construct (almost always `false` unless it's
  provably a compile-time integer constant).
- `tests/filecheck.rs` — only reachable through the two `Expr`-level
  matches above (`Expr::Const(_)` catches it as an opaque case there), so
  usually no separate touch needed unless the test wants to unwrap and
  inspect the new `ConstExpr` shape specifically.

## Adding an `ArraySize` variant

- `src/const_expr.rs` — `declarator_size` and `ctype_size`: both match
  exhaustively (used for `sizeof`/layout constant-folding); decide whether
  the new size kind can ever be a compile-time constant, or is always
  `ConstExprError::UnsupportedTypeSize`.
- `tests/filecheck.rs` — `array_size`: exhaustive, used only for the
  clang-oracle comparison path; needs a string rendering.
- `src/parser.rs` — wherever `ArraySize` is *constructed*
  (`DeclaratorParser::parse_declarator`'s `[` handling) — not a match
  site, but the natural place to add parsing for a new array-size form.

## Process note

This impact map was reconstructed by reading most of `src/` in one session
(slate-parser-119.3: adding `ComputedGoto`, `NestedFunction`, `LabelAddr`,
`ArraySize::Star`). Consult it first next time before grepping from
scratch, and extend it in the same edit as the enum change if a new
touchpoint appears.
