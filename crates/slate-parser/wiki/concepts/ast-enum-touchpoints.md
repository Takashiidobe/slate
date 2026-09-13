# AST Enum Touchpoints

_created 2026-09-12_

`Stmt`, `Expr`, `ConstExpr`, `CType`, and `ArraySize` (all in `src/ast.rs` /
`src/const_expr.rs`) are each matched exhaustively, by variant name, in
several unrelated files. The compiler will refuse to build until every one
of these is updated, but nothing points at them up front — you either grep
every constructor name across the crate or read the files end to end. This
page is that grep, done once, so the next AST change doesn't require
re-deriving it. See [[architecture_single_configuration]] for the
single-configuration pipeline these enums belong to.

Update this page whenever a new exhaustive match site over one of these
enums is added or removed.

Parsed nodes are stored as `Span<T>`: translation-unit declarations are
`Span<Decl>`, function bodies contain `Span<Stmt>`, expression-bearing
fields contain `Span<Expr>`, and preprocessor output uses
`Span<PPNodeKind>`. Exhaustive matches over these collections must match
the wrapper's `.value`.

## Adding a `Decl` variant

- `src/ast.rs` — `Decl::name` and `Decl::provenance` are exhaustive.
- `src/sema.rs` — typedef/tag collection and the main analysis pass match declarations.
- `src/reachability.rs` — root dependency marking is exhaustive.
- `tests/filecheck.rs` — clang-oracle filtering and declaration summaries are exhaustive.

## Adding a `Stmt` variant

- `src/parser/stmt.rs` — every `FunctionDecl` body (top-level and nested) is
  passed through `reachability::mark_unreachable` when it is built. A new
  variant that wraps a nested body needs the same call.
- `tests/filecheck.rs` — `summarize_evaluated_decl`'s inner match over
  `Stmt` (used to build the clang-oracle comparison). Only matters
  once the new statement can appear where `Return` is being scanned for;
  usually just add it to the `=> None` catch-group.
- `src/reachability.rs` — `Reachability::mark_stmt` is exhaustive: it
  decides which translation-unit declarations survive filtering, so a
  variant holding expressions, declarations, or nested bodies must recurse
  or the header declarations they reference get pruned from the AST.
  `mark_unreachable_in` and `always_terminates` (dead-code marking, a
  separate concern) use wildcard arms and only need touching if the new
  statement always transfers control, like `Goto`.
- `Stmt::Attribute` is a standalone GNU or C23 attribute declaration; it
  needs no reachability handling beyond that conservative default.
- `src/sema.rs` — `walk_stmt` is exhaustive: a variant holding statements
  or expressions must recurse so clang-flavor asm checks see nested
  `asm`, register locals, and labels.

## Adding an `Expr` variant

- `src/ast.rs` — `impl Display for Expr`: exhaustive, needs an arm.
- `src/sema.rs` — `walk_expr` is exhaustive; recurse into sub-expressions.
- `src/reachability.rs` — `Reachability::mark_expr` is exhaustive; mark
  identifiers and recurse so referenced header declarations are kept.
- No other file matches `ast::Expr` exhaustively outside the above (checked via
  `grep -rn "Expr::" src/*.rs tests/*.rs`). `tests/filecheck.rs` matches on
  it in two places (`summarize_evaluated_decl`'s `Return` scan,
  `array_size`) but both are exhaustive over `Expr` too — see below,
  they'll fail to compile and tell you.

Note: most *general* expressions never construct `ast::Expr` directly —
they go through `const_expr::Parser` and get wrapped once as
`Expr::Const(Box<ConstExpr>)` by `Parser::parse_expression` in
`src/parser/stmt.rs`. Only bare string literals are built as an `Expr` variant
directly, bypassing `const_expr`. When in doubt, a new expression-level
construct belongs in `ConstExpr`, not `Expr`.

GNU statement expressions `({ ... })` are split across both: when the
*entire* statement is `({ ... });` (or the whole initializer is
`= ({ ... })`), `src/parser/`'s statement/initializer parsing special-cases
it directly into `ast::Expr::StatementExpression(Vec<SpannedStmt>)` with
real parsed statements (see `parse_one_stmt` and the initializer path in
`parse_declaration_tokens`). But when `({ ... })` appears nested inside a
larger expression (a call argument, a binary operand — see
slate-parser-wf8.3.8), it has to go through `const_expr::Parser`, which
has no way to call back into `src/parser/stmt.rs`'s statement grammar (that needs
`&Parser` for diagnostics, typedef names, and recursion). So
`const_expr::Parser::parse_primary` instead captures the raw token span
as `ConstExpr::StatementExpression(Vec<Span<Token>>)` — unparsed — when it
sees `(` `{`. The two `StatementExpression` variants (one on `ast::Expr`
holding parsed statements, one on `ConstExpr` holding raw tokens) are
*not* the same shape; don't assume parity between them without checking
which parser produced the node.

`_Generic` used to be one of those bypassing special forms (its own
`Expr::Generic` variant, parsed by a hand-rolled paren/comma scanner in
`parser.rs::parse_expression`, now `src/parser/stmt.rs`), removed in slate-parser-wf8.2.4: that
scanner only matched when the *entire* expression span was exactly
`_Generic(...)`, so `_Generic(x, int: 1) != 1` (a `_Generic` embedded as
a primary expression inside a larger expression) hit "expected `)` after
`_Generic`". `const_expr::Parser::parse_primary` already had a correct,
depth-aware `_Generic` implementation (`ConstExpr::Generic`, used for
`#if`), so the fix was to delete the duplicate top-level special case
entirely and let `_Generic` always flow through `const_expr`, like any
other primary expression. Fixing this also exposed a real, independent
bug it happened to route around: `const_expr::Parser::try_parse_type_name`
speculatively tries the `_Generic` controlling operand as a type name and
is supposed to fail safely (`.ok()?`) when it isn't one, but the shared
`DeclaratorParser::parse_declarator`'s grouped-declarator case used
`assert!` for a mismatched `)` instead of returning `Err`, so a
speculative parse of a call expression like `ckd_add(&a, 1, 1)` as the
controlling operand could panic instead of falling through. That `assert!`
is now `Err(DeclaratorError::ExpectedToken(..))`.

## Adding a `ConstExpr` variant

- `src/const_expr.rs` — `impl Display for ConstExpr`: exhaustive.
- `src/const_expr.rs` — `Parser::evaluate_expr`: exhaustive, must decide
  whether the new construct is foldable to `i64` or returns
  `ConstExprError::NotConstant(...)`.
- `src/sema.rs` — `is_integer_constant_expression`: exhaustive, decide
  `true`/`false` for the new construct (almost always `false` unless it's
  provably a compile-time integer constant).
- `src/sema.rs` — `walk_const_expr` is exhaustive; recurse into
  sub-expressions (it finds raw statement-expression tokens for label
  lookup).
- `src/reachability.rs` — `Reachability::mark_const_expr` is exhaustive;
  mark identifiers and embedded type names (casts, `sizeof`, compound
  literals) so the header declarations they name survive filtering.
- `ConstExpr::Elvis` is the GNU omitted-middle conditional form; preserve its
  single evaluation of the condition in evaluators and lowering.
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
- `src/parser/declarator.rs` — wherever `ArraySize` is *constructed*
  (`DeclaratorParser::parse_declarator`'s `[` handling) — not a match
  site, but the natural place to add parsing for a new array-size form.

## Adding a `CType` variant

- `src/const_expr.rs` — `ctype_size` is exhaustive and must decide whether
  the type has a compile-time size.
- `src/sema.rs` — `check_type` is exhaustive and must traverse nested types.
- `src/reachability.rs` — `Marker::mark_type` is exhaustive and must mark
  declarations referenced through the type.

## Process note

This impact map was reconstructed by reading most of `src/` in one session
(slate-parser-119.3: adding `ComputedGoto`, `NestedFunction`, `LabelAddr`,
`ArraySize::Star`). Consult it first next time before grepping from
scratch, and extend it in the same edit as the enum change if a new
touchpoint appears.
