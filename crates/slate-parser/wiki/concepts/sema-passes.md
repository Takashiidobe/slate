# Sema passes

<!-- toc -->
- [Pass order](#pass-order)
- [Files by pass](#files-by-pass)
- [`ResolveError` policy](#resolveerror-policy)
- [Shared resolver](#shared-resolver)
- [Expression typer](#expression-typer)
- [Checker-owned rules](#checker-owned-rules)
- [Error reporting](#error-reporting)
<!-- /toc -->

How `src/sema/` turns a parsed `TranslationUnit` into an IR `Module`.
Principles: [architecture](architecture.md#checker-first-lowering-trusts).

## Pass order

Driven by `src/main.rs`.

| # | Pass | Entry | Output |
| --- | --- | --- | --- |
| 1 | Reachability pruning | `reachability::filter_translation_unit`, called by the parser before sema | pruned `TranslationUnit` ([rules](ir/pipeline.md#reachability-pruning)) |
| 2 | Name resolution | `Sema::new` → `names::resolve_items` | `NameResolution` + one `ItemResolution { errors, declared, references }` per top-level item |
| 3 | Resolver setup | `Sema::new` → `TypeResolver::with_names` | shared `TypeResolver`; runs `pragmas::collect` |
| 4 | Checker | `Sema::analyze` → `assertion::validate` | errors; warnings stashed per item in `item_diagnostics` |
| 5 | Early validation | `Sema::analyze` → `validate::analyze` (after the checker) | structural errors, unknown typedef names, literal and type-extension warnings |
| 6 | Lowering | `Sema::lower` → `module::resolve_module` | `Module` + warnings |
| 7 | Module finish | `Lowerer::finish_module` | object requests, inline emission, declaration ABIs, then `effects_statements::normalize` (side-effect hoisting, unsequenced marking) |

- `analyze` returns `SemaErrors` on any error and `main` stops, so lowering
  only runs on a checker-clean unit.
- `parse` stops after step 5; `ir` / `--dump-ir` runs 6–7.
- Dump-only paths build their own resolver: `Sema::type_module`
  (`types::resolve_type_module`, for `--dump-ir-types`) and
  `Sema::expression_roots` (`--dump-ir-expressions`).

## Files by pass

| Role | Files |
| --- | --- |
| Names | `names.rs` |
| Checker | `assertion.rs` (the `Checker`), `validate.rs` (early checks, `analyze` driver, `with_sources`) |
| Shared, on `TypeResolver` | `types.rs` (declarator/tag resolution, layout, `check_attributes`), `typer.rs` (expression typer), `operand.rs` (typed arithmetic, constants), `initializer.rs` (current-object walk), `type_of.rs`, `builtins.rs` (builtin registry, custom rules), `entity.rs`, `attributes.rs` (`declaration_use`), `pragmas.rs`, `fold.rs`, `ctype/` ([c-type-layer](c-type-layer.md)) |
| Shared rules and emitters | `numeric.rs` (`ResolveError`, `Context`, `binary_rule` / `unary_rule`, IR emission) |
| Lowering, on `Lowerer` | `module.rs` (driver, declarations, statements), `function.rs`, `expression.rs`, `asm.rs`, `atomic.rs`, `abi.rs`, `ms_asm.rs`, `ms_asm_effects.rs`, `ms_asm_return.rs`; `initializer.rs` and `type_of.rs` have `Lowerer` halves |
| After lowering | `effects.rs`, `effects_statements.rs`, `sequencing.rs` |
| Generated | `clang_builtins.rs` (`tools/generate_clang_builtins.py`) |

## `ResolveError` policy

Defined in `sema/numeric.rs`.

| Variant | Meaning | Who returns it |
| --- | --- | --- |
| `Rejected(&str)` | Ill-formed C that every oracle rejects ([strictness](architecture.md#strictness-policy)) | checker, typer, shared rule functions |
| `InvalidOperands` / `InvalidOperand` | Typed rejection of arithmetic operands | `numeric::binary_rule` / `unary_rule` |
| `Unimplemented(&str)` | Valid C not modeled yet (LLVM NYI); not a rejection | anywhere |
| `UnsupportedBuiltin(String)` | Known builtin without lowering; a kind of `Unimplemented` | `expression.rs` |
| `Internal(&str)` | An invariant the checker should guarantee; always a slate bug | lowering |
| `MissingExpressionBinding(String)` | Identifier without a binding | `expression.rs` |
| `IntegerLiteral(String)` | Literal with no target type | `numeric.rs`, `operand.rs` |
| `Names`, `Literal`, `Layout` | Wrapped `names::ResolveError`, `ConstExprError`, `LayoutError` | name resolution, literal decoding, `TargetInfo` |
| `Located { loc, error }` | Location wrapper from `ResolveError::at` | `Lowerer::expr` / `place` / `statements` |

Rules:

- A new rejection goes in the checker or in a rule function both passes
  call, and returns `Rejected` (or `InvalidOperand(s)`).
- `is_rejection()` is true for `Rejected`, `InvalidOperands`,
  `InvalidOperand`. The checker reports only rejections; other `typed`
  failures (`Unimplemented`, …) are left for lowering to report.
- Lowering never returns a rejection: `checked()` maps rejections to
  `Internal`. Lowering applies it when calling shared rules and
  `resolve_module` applies it to every item error and to `finish_module`.
- Never use `Unimplemented` for ill-formed code or `Rejected` for
  unmodeled code; sweeps triage by variant (`tools/corpus_sweep.py`).
- A lowering-side lookup of something the checker should have recorded
  (a conversion, an initializer target, a typer answer) fails `Internal`.
- `at(loc)` sets a location only if none is set, so the innermost node
  wins.

## Shared resolver

- `Sema::new` builds one `TypeResolver` from the one `NameResolution`. The
  checker walks the unit over it; `lower` takes it over and resets only
  `entities`, which it redeclares in order.
- Maps from `with_names`: `references` (identifier id → `BindingId`),
  `declarations` (declaring node → `BindingId`). Identifier type:
  `entities.ty(binding)`; constant value (enumerator, folded `constexpr`):
  `constants[binding]`. Typedef names and tag references are keyed by
  their `Span<String>` `NodeId`: `aliases[binding]`,
  `tag_bindings[binding]`.
- Definitions are memoized by AST node, so the second walk finds rather
  than recreates them: `define_tag` by `TagId` (failures too, in
  `tag_failures`), `define_alias` by declarator `NodeId`. A definition's
  span is its `owner` (declarator, parameter, or function being resolved);
  ownerless tags use the body span. The module prints each definition's
  final state.
- `TypeId`s are numbered in source order (the checker also resolves type
  names inside expressions). VLA typedefs are numbered when lowering
  reaches them; the checker only gives them a provisional alias
  (`declare_provisional_alias`).
- The checker declares everything lowering would before typing its uses:
  objects, parameters, and casts under `provisional_extents`; parameter
  array qualifiers (`int a[restrict 4]`); linkage for functions and
  file-scope/`extern` objects; `extern void` objects; implicit function
  declarations (`NameResolution.implicit_functions`,
  `CTypes::implicit_function`). It walks every expression lowering
  evaluates (`case` labels, asm operands, array sizes, `typeof` operands).
- Anything scanning all tags must bound itself by position: `__asm` member
  lookup only sees tag bindings below the names.rs watermark
  (`NameResolution.ms_asm_members`), since the checker has already defined
  later tags.

## Expression typer

`TypeResolver::typed` (`typer.rs`) types an expression without lowering it.

- Used by the checker (every expression, children first), unevaluated
  operands (`sizeof`, `_Generic` control, `typeof`, `__auto_type`
  initializers, classify and derived-signature builtins), and `typeof`
  resolution (`expression_type`). Nothing is lowered speculatively.
- Memoizes `Typed { c, lvalue, bits }` per `NodeId` in `expression_types`.
  Only successes are memoized (an enum body is typed before its
  enumerators exist). Missing rules return `Unimplemented`.
- `Lowerer::expr` / `place` compare their type with the typer's and fail
  `Internal` on mismatch. Shared rules are factored, not copied:
  `binary_types`, `arithmetic_type`, `literal_type`, `derived_signature`,
  `builtin_callee`, `chosen_expr`, `real_floating_component`,
  `statement_expression_parts`, `AtomicBuiltin::result`, `swizzle`,
  `shuffle`, `predefined_name`.
- A typed `sizeof` operand is not lowered unless it is a VLA.
- Null pointer constants: `integer_constant_zero`, an ICE by 6.6p6
  operand rules that folds to zero (`(void *)(1 - 1)` yes; `(void *)(0,
  0)`, `(int)(0.0 + 0.0)`, `(size_t)(void *)0` no). Used by
  `is_null_pointer_constant` and pointer `?:`; a cast of a zero ICE lowers
  to `null<ptr<T>>`.
- `__func__` reads `function_names`. Locals declared inside a statement
  expression are typed into `locals` (`declarator_type` +
  `completed_array`), consulted after `entities`.
- Variably modified operands: the typer sets `provisional_extents`, so
  unbound sizes become `vla<T, *>`; types with unbound extents
  (`CTypes::has_unbound_extent`) are never memoized, so lowering's
  cross-check gets the bound `vla<T, %id>`.
  - `sizeof` lowers a VLA operand and keeps it if it has effects
    (capturing an extent counts).
  - `typeof` of a VM expression is evaluated where its extents are
    captured (`Lowerer::typeof_evaluations`, from `capture_extents` /
    `type_name_extents`), only if it has effects.
  - `__auto_type` reserves `BindingId`s for non-constant sizes first
    (`reserve_extents`); lowering's `extents` binds into them.

## Checker-owned rules

The checker rejects; lowering's copies are `Internal`.

- **Statements** (`StatementContext`, reset per function): `break` /
  `continue` / `case` / `default` / `[[fallthrough]]` outside their
  construct; non-integer switch; unfoldable case label; non-pointer
  computed goto; non-scalar condition; value `return` in a void function
  (a void operand is allowed and lowered as a statement then `return`);
  valueless `return` per flavor; fallthrough on a non-empty statement
  (non-gcc); nested function definitions (non-gcc). `constant_value` folds
  `?:` without requiring the unselected arm to be constant (`case (1 ? 1 :
  i)`).
- **Declarations**: shared `module::linkage`, `symbol_attributes`,
  `function_symbol`, `deduced_initializer` / `inferred_base` /
  `check_inferred` (`auto`, `__auto_type`), `attribute_error` (warning-free
  half of `check_attributes`). Restated in `Checker::object_rules`: void
  object; function initializer / thread-local / block-scope `static`;
  thread-local automatic; block-scope `extern` initializer; constexpr
  without initializer; VLA initializer; static VLA; weakref / selectany
  linkage; multiple initializers (`Checker::initialized`); conflicting
  `always_inline`/`noinline` (`Checker::inlining`). Arrays of `void` never
  resolve, so `validate.rs` checks them syntactically. Conversions to or
  from an incomplete enum are rejected by `classify_conversion`.
- **Conversions**: recorded per operand by `record_conversion`
  ([c-type-layer](c-type-layer.md#conversions)).
- **Initializers**: `check_initializer` / `check_braced`
  (`initializer.rs`) mirror lowering's `braced` / `fill` / `init_into` /
  `item_entry` / `designate` / `resume` step for step (brace elision
  ignores an already-claimed designator; a union is full once any member
  is written). Records each element's conversion and target in
  `element_targets`; `convert_element` fails `Internal` if its target is
  not compatible (compatibility, because of provisional extents).
  `inferred_array_length` runs the same walk unchecked.
- **Expressions**: each rejection is reported once, at the innermost
  expression (`rejected_at`). The typer owns lvalue / const targets, `&`
  of bit-field / register / vector element / rvalue, members, `->`,
  callees and argument counts, subscripts and pointer arithmetic
  (`pointer_offset`, `require_pointer_element`), conditions, `?:`
  operands, `sizeof`/`_Alignof` of incomplete types and bit-fields, bit
  casts, `__builtin_convertvector`, `va_arg` and `va_*`, custom and atomic
  builtin operands. Shared with emitters: `numeric::binary_rule` /
  `unary_rule` (called by `emit_binary` / `emit_unary_arith`),
  `atomic::fetch_rule`, `AtomicBuiltin::operands`, `asm_operand_rule`,
  `typeof_expression`. `sizeof` of a record with a failed definition
  (`failed_definition`) is left to the definition's own error.
- **Type resolution**: `Checker::resolution` reports the first rejection
  per declaration. `declare_object` merges through `merge_redeclaration`
  after `inherit_convention` (and `apply_convention` for definitions), so
  conflicting types and conventions are reported at the declarator.
- **FP pragmas**: the checker runs its own `FloatingPragmas` /
  `FloatingRegion`, restored at each compound; a pragma right after
  `({ ... })` is rejected, as clang does.
- Known duplicate: the `always_inline`/`noinline` conflict is also checked
  in lowering's `record_function`; merge rather than edit both.

## Error reporting

- Up to 20 errors (clang's `-ferror-limit` default), then "too many errors
  emitted, stopping now". Any error means no IR. The limit lives in
  `with_sources`, shared by `analyze` and `lower`. `-ferror-limit` is not
  accepted.
- The checker's per-item warnings go to `item_diagnostics` and are
  replayed when lowering reaches the item, so order is unchanged. An item
  whose diagnostics include an error (default-error or `-Werror`) moves
  them all into the checker's errors.
- Name errors: unresolved references are recorded and visiting continues
  (`int a = undeclared, b;` still binds `b`); other name errors end the
  item and reset to file scope. `analyze` reports unknown typedef names;
  `lower` reports the rest and skips items with name errors.
- Lowering errors reset per-function state (`reset_after_failed_item`) and
  continue with the next item. Locations come from `ResolveError::at`;
  errors outside any expression or statement fall back to the top-level
  declaration.
- Poisoning: bindings declared by a failed item are poisoned; a later
  item's lowering error is dropped if it references one. Checked before
  the failed item's own bindings are poisoned, so a recursive function
  still reports. Name errors are never dropped.
- `finish_module` runs only on an error-free unit.
- Fixtures: `sema_reports_all_errors.c`, `sema_poisoned_declaration.c`,
  `sema_error_limit.c`.
