# AST Enum Touchpoints

_created 2026-09-12_

`Stmt`, `ExprKind`, `CType`, and `ArraySize` (all in `src/ast.rs`) are each matched exhaustively, by variant name, in
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
fields contain `Expr = Box<Span<ExprKind>>`, and preprocessor output uses
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
- `Stmt::Labeled { label: String, body: Box<Stmt> }` (goto target) and
  `Stmt::SwitchLabel { label: SwitchLabel, body: Box<Stmt> }` (`case`/
  `case ... ...`/`default`) nest their target statement as `body` instead
  of appearing as a flat list item followed by the labelled statement, so
  `case 1: case 2: x;` parses as one nested `SwitchLabel`, not three flat
  `Stmt`s (`lh7.3.9`). Every exhaustive `Stmt` match above must recurse
  into `body` the same way it recurses into `Block`'s statement list, or
  whatever the label wraps (a nested `asm`, a `goto`, an expression)
  becomes invisible to that pass.

## Adding an `ExprKind` variant

There is one expression type. `const_expr::Parser` builds it for every
context (statements, initializers, array bounds, bit widths, `typeof`,
attributes, `#if`), and wraps each node in a `Span` covering its tokens.

- `src/ast.rs` — `impl Display for ExprKind`: exhaustive.
- `src/const_expr.rs` — `Parser::evaluate_expr`: exhaustive; decide whether
  the construct folds to `i64` or returns `ConstExprError::NotConstant`.
  `evaluate_wide` and `contains_wide` only need touching for new
  arithmetic forms.
- `src/sema.rs` — `is_integer_constant_expression` and `walk_expr` are
  exhaustive; `walk_expr` must recurse so asm/label checks see nested
  statement expressions.
- `src/reachability.rs` — `Reachability::mark_expr` is exhaustive; mark
  identifiers and embedded type names so referenced header declarations
  survive filtering.
- `tests/filecheck.rs` — `summarize_evaluated_decl` and `array_size` use
  wildcards; no touch needed.

`({ ... })` is parsed by `const_expr::Parser::parse_statement_expression`,
which calls back into `parser::Parser::parse_statement_expression_body`.
The callback is the `statements: Option<&parser::Parser>` threaded through
`const_expr::Parser` and `DeclaratorParser`. It is `None` for `#if`,
attribute arguments and `_BitInt` widths, where a statement expression is
an error.

## Adding an `ArraySize` variant

- `src/const_expr.rs` — `declarator_size` and `ctype_size`: both match
  exhaustively (used for `sizeof`/layout constant-folding); decide whether
  the new size kind can ever be a compile-time constant, or is always
  `ConstExprError::UnsupportedTypeSize`.
- `tests/filecheck.rs` — `array_size`: exhaustive, used only for the
  clang-oracle comparison path; needs a string rendering.
- `src/parser/declarator.rs` — wherever `ArraySize` is _constructed_
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
