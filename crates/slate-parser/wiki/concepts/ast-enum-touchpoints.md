# AST Enum Touchpoints

_created 2026-09-12_

`src/visit.rs` owns exhaustive recursion over `DeclKind`, `StmtKind`,
`ExprKind`, `TypeSpecifier`, `Declarator`, `Initializer`, attributes, tag
bodies, and their auxiliary enums. Validation and name resolution implement
`Visitor`; adding an expression-bearing field or variant fails in the shared
walker instead of silently disappearing from one pass. The remaining
touchpoints below either render nodes or assign semantics to them. See
[[architecture_single_configuration]] for the single-configuration pipeline
these enums belong to.

Update this page whenever a new exhaustive match site over one of these
enums is added or removed.

Parsed nodes are stored as `Span<T>`: translation-unit declarations are
`Span<Decl>`, function bodies contain `Span<Stmt>`, expression-bearing
fields contain `Expr = Box<Span<ExprKind>>`, and preprocessor output uses
`Span<PPNodeKind>`. Exhaustive matches over these collections must match
the wrapper's `.value`.

## Adding a `Decl` variant

- `src/ast.rs` — `Decl::name` and `Decl::provenance` are exhaustive.
- `src/sema/validate.rs` — typedef/tag collection and the main analysis pass match declarations.
- `src/reachability.rs` — root dependency marking is exhaustive.
- `tests/filecheck.rs` — clang-oracle filtering and declaration summaries are exhaustive.

## Adding a `Stmt` variant

Control-flow bodies and if/else branches are `Box<Stmt>`; only `Block` and
function/statement-expression bodies contain statement lists. `Null` represents
an empty statement without introducing a compound scope.

- `src/visit.rs` — shared statement traversal, including attributes, asm
  operands, static assertions, nested functions, and single bodies.
- `src/sema/names.rs` — visitor overrides implement scope, binding, and label
  rules that depend on `TranslationUnit.standard`.
- `src/render.rs` — comment stripping recurses into single bodies and blocks.
- `src/sema/module.rs` — module lowering handles `Null` and explicit blocks.

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
- `StmtKind::Attribute` is a standalone GNU or C23 attribute statement.
  `StmtKind::Attributed { attributes, body }` attaches attributes to a nested
  statement; all body walkers must recurse through it without adding a scope.
  Reachability visits both the attributes and body.
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
- `src/visit.rs` — shared recursion reaches expressions in declarations,
  types, attributes, designators, tag bodies, static assertions, and asm
  operands.
- `src/sema/validate.rs` — `is_integer_constant_expression` assigns constant
  expression semantics and remains exhaustive.
- `src/sema/expression.rs` — `Lowerer::expr` is exhaustive (no catch-all
  since `lh7.2.12`); a new variant needs an IR lowering or an explicit
  `ResolveError`.
- `src/reachability.rs` — `Reachability::mark_expr` is exhaustive; mark
  identifiers and embedded type names so referenced header declarations
  survive filtering.
- `tests/filecheck.rs` — `summarize_evaluated_decl` and `array_size` use
  wildcards; no touch needed.

A variant shaped `{ ty: TypeName, value: Expr }` (`Cast`, `BitCast`,
`ConvertVector`, `VaArg`) joins the existing or-patterns in `visit.rs`,
`reachability.rs`, `sema/names.rs` and `sema/assertion.rs` instead of adding
an arm, and is parsed like `parse_va_arg`: the type operand is read with
`try_parse_type_name`, not as an expression.

`({ ... })` is parsed by `const_expr::Parser::parse_statement_expression`,
which calls back into `parser::Parser::parse_statement_expression_body`.
The callback is the `statements: Option<&parser::Parser>` threaded through
`const_expr::Parser` and `DeclaratorParser`. It is `None` for `#if`,
attribute arguments and `_BitInt` widths, where a statement expression is
an error.

## Adding an `ArraySize` variant

- `tests/filecheck.rs` — `array_size`: exhaustive, used only for the
  clang-oracle comparison path; needs a string rendering.
- `src/parser/declarator.rs` — wherever `ArraySize` is _constructed_
  (`DeclaratorParser::parse_declarator`'s `[` handling) — not a match
  site, but the natural place to add parsing for a new array-size form.

## Adding a `TypeSpecifier` variant

`TypeSpecifier` never holds pointers, arrays or functions; those live in
`Declarator`. A variant that embeds a `TypeName` (`Atomic`, `TypeOf`) must
walk its declarator too.

- `src/visit.rs` — shared recursion traverses embedded type names and their
  declarators.
- `src/sema/validate.rs` — `check_type`, `collect_tag_names`, and
  `is_register_scalar_type` assign type-specific validation semantics.
- `src/reachability.rs` — `Marker::mark_type` is exhaustive and must mark
  declarations referenced through the type.
- `tests/filecheck.rs` — `type_spelling` renders it for declaration summaries.

## Process note

This impact map was reconstructed by reading most of `src/` in one session
(slate-parser-119.3: adding `ComputedGoto`, `NestedFunction`, `LabelAddr`,
`ArraySize::Star`). Consult it first next time before grepping from
scratch, and extend it in the same edit as the enum change if a new
touchpoint appears.
