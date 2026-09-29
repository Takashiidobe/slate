# AST Enum Touchpoints

<!-- toc -->
- [Adding a `Decl` variant](#adding-a-decl-variant)
- [Adding a `Stmt` variant](#adding-a-stmt-variant)
- [Adding an `ExprKind` variant](#adding-an-exprkind-variant)
- [Adding an `ArraySize` variant](#adding-an-arraysize-variant)
- [Adding a `TypeSpecifier` variant](#adding-a-typespecifier-variant)
- [Adding an `Attribute` variant](#adding-an-attribute-variant)
- [The sema and IR half](#the-sema-and-ir-half)
<!-- /toc -->

Exhaustive match sites to update when adding an AST variant. Update this
page when a match site is added or removed.

- `src/visit.rs` owns exhaustive recursion over `DeclKind`, `StmtKind`,
  `ExprKind`, `TypeSpecifier`, `Declarator`, `Initializer`, attributes, and
  tag bodies. Validation and name resolution implement `Visitor`, so a new
  expression-bearing field fails in one walker instead of vanishing from a
  pass.
- Nodes are `Span<T>` (`Span<Decl>`, `Span<Stmt>`,
  `Expr = Box<Span<ExprKind>>`, `Span<PPNodeKind>`); matches go through
  `.value`.

## Adding a `Decl` variant

- `src/ast.rs`: `Decl::name`, `Decl::provenance`.
- `src/sema/validate.rs`: typedef/tag collection and the main analysis
  pass.
- `src/reachability.rs`: root dependency marking.
- `src/sema/module.rs`: `lower_item`; lower to IR or drop explicitly.
- `src/sema/pragmas.rs`: `collect`; walk variants that can hold a pragma or
  tag definition.
- `tests/filecheck.rs`: clang-oracle filtering and declaration summaries.

## Adding a `Stmt` variant

- `src/visit.rs`: shared traversal (attributes, asm operands, static
  assertions, nested functions, single bodies).
- `src/sema/names.rs`: scope, binding, and label rules (feature-dependent).
- `src/render.rs`: comment stripping recurses into bodies and blocks.
- `src/sema/module.rs`: `Lowerer::statements`, no catch-all.
- `src/sema/pragmas.rs`: ordered pragma walk; recurse into held
  statements.
- `src/reachability.rs`: `Reachability::mark_stmt`; recurse into
  expressions, declarations, and bodies, or referenced header declarations
  are pruned.
- `tests/filecheck.rs`: `summarize_evaluated_decl`; usually add to the
  `=> None` group.

Shape notes:

- Control-flow bodies are `Box<Stmt>`; only `Block` and function /
  statement-expression bodies hold lists. `Null` is an empty statement with
  no scope.
- `Labeled { label, body }` and `SwitchLabel { label, body }` nest their
  target, so `case 1: case 2: x;` is one nested `SwitchLabel`. Walkers must
  recurse into `body`.
- `Attribute` is a standalone attribute statement; `Attributed
  { attributes, body }` wraps a statement without adding a scope.
- `MsAsm` holds no `Expr`; C names are `MsAsmExpr::Name` strings. Name
  resolution (`sema/names.rs`, also collects asm labels), reachability, and
  `sema/ms_asm.rs` walk `MsAsmExpr`; a new `MsAsmExpr` variant needs all
  three.

## Adding an `ExprKind` variant

One expression type, built by `const_expr::Parser` in every context
(statements, initializers, bounds, bit widths, `typeof`, attributes,
`#if`).

- `src/ast.rs`: `impl Display for ExprKind`.
- `src/const_expr.rs`: `Parser::evaluate_expr`; fold to `i64` or return
  `ConstExprError::NotConstant`. `evaluate_wide` / `contains_wide` only for
  new arithmetic.
- `src/visit.rs`: shared recursion.
- `src/sema/validate.rs`: `is_integer_constant_expression`.
- `src/sema/expression.rs`: `Lowerer::expr`, no catch-all; lower or return
  a `ResolveError`.
- `src/sema/typer.rs`: `TypeResolver::type_expression`; a typing rule
  shared with lowering (mismatch is `Internal`) or `Err(UNTYPED)`, which
  makes `sizeof`/`typeof`/`_Generic` of it an error.
- `src/reachability.rs`: `Reachability::mark_expr`; mark identifiers and
  embedded type names.
- `tests/filecheck.rs`: wildcards; no change.

Shape notes:

- `{ ty: TypeName, value: Expr }` variants (`Cast`, `BitCast`,
  `ConvertVector`, `VaArg`) join the existing or-patterns in `visit.rs`,
  `reachability.rs`, `sema/names.rs`, `sema/assertion.rs`, and parse the
  type with `try_parse_type_name` like `parse_va_arg`.
- `({ ... })`: `const_expr::Parser::parse_statement_expression` calls back
  into `parser::Parser::parse_statement_expression_body` through the
  `statements: Option<&parser::Parser>` field of `const_expr::Parser` and
  `DeclaratorParser`. `None` (`#if`, attribute arguments, `_BitInt` widths)
  makes it an error.

## Adding an `ArraySize` variant

- `tests/filecheck.rs`: `array_size` (clang-oracle path) needs a rendering.
- `src/parser/declarator.rs`: construct it in
  `DeclaratorParser::parse_declarator`'s `[` handling.

## Adding a `TypeSpecifier` variant

`TypeSpecifier` never holds pointers, arrays, or functions (those are in
`Declarator`). A variant embedding a `TypeName` (`Atomic`, `TypeOf`) walks
its declarator too.

- `src/visit.rs`: shared recursion into embedded type names.
- `src/sema/validate.rs`: `check_type`, `is_register_scalar_type`.
- `src/reachability.rs`: `mark_type`; mark referenced declarations.
- `tests/filecheck.rs`: `type_spelling`.

## Adding an `Attribute` variant

- `src/visit.rs`: expression-bearing attributes.
- `src/parser/attributes.rs`: spelling table and argument parsing.
- `src/sema/attributes.rs`: `declaration_use`; classify as symbol, layout,
  ignored, or unsupported-with-reason.
- `src/sema/module.rs`: `symbol_attributes` folds the `Symbol` group;
  `function_symbol` has its own narrower filter.
- `src/sema/function.rs`: `record_function` interprets function attributes,
  keeps the rest as `c_attributes`.
- `src/sema/types.rs`: `requested_alignment`, `field_request` read the
  `Layout` group.

## The sema and IR half

A new type family continues through `CTypeKind`, `ir::Type`, storage,
conversions, arithmetic, and ABI: [type-family-touchpoints](type-family-touchpoints.md).
