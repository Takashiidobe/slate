# Attribute pipeline

<!-- toc -->
- [Stages](#stages)
- [Parsing](#parsing)
- [Registration](#registration)
- [Early validation](#early-validation)
- [Applicability](#applicability)
- [Checker and lowering](#checker-and-lowering)
- [Consumers](#consumers)
- [Preprocessor queries](#preprocessor-queries)
- [Adding an attribute](#adding-an-attribute)
<!-- /toc -->

How an attribute gets from source to the IR. AST shape:
[ast-spec](ast-spec.md#attributes-and-asm); warnings:
[diagnostic-severity](diagnostic-severity.md).

## Stages

| # | Stage | Where | Decides |
| --- | --- | --- | --- |
| 1 | Parse | `parser/attributes.rs::parse_attribute_groups` | spelling → `Attribute` variant with parsed arguments |
| 2 | Registration | `attribute_support.rs` (called by the parser) | does this flavor know the spelling on this target; if not, `Unknown` / `IgnoredDeclspec` |
| 3 | Early validation | `validate.rs::check_attributes` | `Invalid` arguments, `alloc_size` arity |
| 4 | Applicability | `sema/attributes.rs::declaration_use` | what the attribute does on this `Subject` |
| 5 | Check | `TypeResolver::attribute_error` (checker), `check_attributes` (lowering, `define_tag`) | reject, warn, or accept |
| 6 | Consume | `symbol_attributes`, `function_symbol`, `record_function`, `requested_alignment`, `field_request`, record layout, `c_attributes` metadata | IR effect |

## Parsing

`parse_attribute_groups` reads any run of these, in any order:

| Syntax | Handling |
| --- | --- |
| `__cdecl`, `__stdcall`, `__fastcall`, `__vectorcall`, `__thiscall` (and `_x` forms) | `CallingConvention` |
| `__declspec(a b(x) ...)` | `align` → `aligned`, `allocate` → `section`; unregistered → `IgnoredDeclspec` |
| `_Alignas(...)` / `alignas(...)` | `AlignAs(Type \| Expr)`; a type name is tried first |
| `__attribute__((...))` / `__attribute((...))` | unregistered → `Unknown` |
| `[[name]]`, `[[scope::name]]` | `c23_attribute_name` maps standard names, `gnu::` / `clang::` names, `msvc::noinline`, `msvc::forceinline` (→ `always_inline`); anything else → `Unknown` |

- `parse_attribute_value` matches the unwrapped name (`__name__` → `name`)
  and argument shape. A modeled name with the wrong arguments is
  `Invalid`; an unmodeled name is `Unknown` with its argument tokens.
- `parse_attribute` runs under a parser checkpoint that is committed only
  when the result is not `Invalid`, so a failed argument parse leaves no
  name or annotation changes.
- Bare `aligned` reads `__BIGGEST_ALIGNMENT__` in the parser
  (`biggest_alignment`).
- Every attribute keeps its span and placement (specifiers, declarator,
  init-declarator, tag, enumerator, statement). `vector_size`,
  `ext_vector_type`, and `mode` in specifier position wrap the type
  specifier ([ast-spec](ast-spec.md#vector_size-and-mode)).
- Which syntax was used is not kept.

## Registration

`src/attribute_support.rs`; the parser asks before building a variant.

- `GNU_ATTRIBUTES`: hand table of modeled `__attribute__` names, each with
  a clang, gcc, msvc `Gate` (`Always`, `Never`, `Windows`, `NotWindows`,
  `X86`, `X86OrArm32`, `NotAArch64`, `NotArm32`, `Arm32`). msvc gates are
  `Never` (cl has no `__attribute__`).
- Unmodeled names fall back to `attribute_support/registered.rs`, generated
  by `tools/generate_attribute_lists.py` from each compiler's
  `__has_attribute` / `__has_c_attribute` over identifiers in its
  binaries. These ignore the target, so e.g. clang's Windows-only `guard`
  is registered everywhere (permissive).
- `[[scope::name]]` is registered per name: clang has `gnu::packed` but not
  `clang::packed`, `clang::overloadable` but not `gnu::overloadable`, and
  only `msvc::noinline`; gcc has only `gnu::`; msvc accepts any `msvc::`
  name. Unscoped C23 names are always registered.
- `__declspec` (`declspec_registered`): clang uses `CLANG_DECLSPECS` (no
  `__name__` unwrapping; `dllimport`/`dllexport` only on Windows); gcc
  treats `__declspec(x)` as `__attribute__((x))`, as mingw does; msvc
  accepts every name.
- An unregistered modeled spelling never reaches lowering (no `dllimport`
  on ELF).
- Measured (clang 22.1.8 over 13 triples, gcc 16.2.1 x86_64/aarch64, cl
  19.51): clang lacks `noipa`, `noclone`, `optimize`,
  `scalar_storage_order`; registers `dllimport` only on Windows and
  `interrupt` everywhere but aarch64. gcc on aarch64 lacks `naked`,
  `interrupt`, x86 conventions.
- Fixtures: `sema/unknown_attributes_{clang_linux,clang_windows,gcc_aarch64}.c`.

## Early validation

`validate.rs::check_attributes` runs in `analyze` on every attribute
position without resolving names:

- `Invalid` → "invalid arguments for attribute".
- `alloc_size` takes one or two arguments.
- Layout operands (`aligned`, `vector_size`, `_Alignas`) are not checked
  here: an enumerator or typedef operand needs name resolution. They are
  folded where consumed (`requested_alignment`, `resolve_declarator`), and
  the checker reports a non-constant operand. Name resolution walks every
  attribute position, including tag, field, and `_Alignas(type)` operands.

## Applicability

`sema/attributes.rs::declaration_use(attribute, subject) -> Use`.
`Subject` is `Function`, `Object { automatic }`, `Parameter`, `Typedef`,
`Field`, or `Record { union }`.

| `Use` | Meaning | Effect in `check_attributes` |
| --- | --- | --- |
| `Symbol` | folds into `SymbolAttributes` | none |
| `Layout` | read by type resolution or the object request | none |
| `Ignored` | nothing the declaration needs (or consumed elsewhere, e.g. by `record_function`) | none |
| `Inapplicable { spelling, applies_to }` | outside clang's subject list | `-Wignored-attributes`; dropped |
| `Unknown` | `Attribute::Unknown` | `-Wunknown-attributes` unless the fallback lists register it |
| `UnsupportedDeclspec` | `Attribute::IgnoredDeclspec` | `-Wignored-attributes` |
| `Rejected(reason)` | error in every oracle (e.g. `section` on a field, `_Alignas` on a typedef) | `ResolveError::Rejected` |
| `Unimplemented(reason)` | valid but not modeled (`code_seg`, other attributes on a parameter) | `ResolveError::Unimplemented` |

- Order: `inapplicable` first, then per-subject rules (`parameter_use`,
  `member_use`), then `general_use`.
- `general_use` is exhaustive over `Attribute`.
- `Ignored` includes `mode`, `address_space`, `cleanup`,
  `scalar_storage_order`, `transparent_union` (the calling convention is
  not modeled; a call needing it fails in argument conversion). `Ignored`
  only means no symbol or layout effect; `cleanup` has its own consumer.
- Attributes before the tag keyword of a declarator-less declaration
  (`__attribute__((packed)) struct S {...};`) never reach the tag; ignored
  with a warning, as clang does.

clang 22.1.8 subject behavior; `ok` accepted, `ign`
`-Wignored-attributes`, `err` error:

| Written on | object | function | typedef | parameter |
| --- | --- | --- | --- | --- |
| `used`, `retain` | ok, `ign` if automatic | ok | `ign` | `ign` |
| `common`, `nocommon` | ok | `ign` | `ign` | ok |
| `visibility`, `weak` | ok | ok | `ign` | ok |
| `cleanup` | `ign` unless automatic | `ign` | `ign` | `ign` |
| `packed`, `ms_struct`, `gcc_struct`, `transparent_union` | `ign` | `ign` | `ign` | `ign` |
| `malloc`, `cold`, `ifunc`, `alloc_size`, … | `ign` | ok | `ign` | `ign` |

| Written on | field | struct/union definition |
| --- | --- | --- |
| `packed`, `aligned`, `vector_size`, `may_alias` | ok | ok |
| `ms_struct`, `gcc_struct` | `ign` | ok (layout) |
| `transparent_union` | `ign` | ok on union, `ign` on struct |
| `mode` | ok | `err` |
| `section`, `alias`, `tls_model` | `err` | `err` |
| `used`, `retain`, `weak`, `common`, `nocommon`, `cleanup`, `noreturn`, `nonnull` | `ign` | `ign` |
| `malloc`, `cold`, `format`, `noinline`, … | `ign` | `ign` |

Fixtures: `sema/ir_attribute_applicability.c`,
`sema/attribute_applicability_warnings.c`,
`sema/record_attribute_applicability.c`, `error/field-section-attribute.c`.

## Checker and lowering

- Checker: `TypeResolver::attribute_error` returns only `Use::Rejected`
  (the warning-free half), called from `assertion.rs` for typedefs,
  objects, functions, and parameters.
- Lowering: `TypeResolver::check_attributes` emits the warnings and
  returns `Rejected` / `Unimplemented`; called from `module.rs` for
  functions, parameters, typedefs, and objects, and from `define_tag` for
  records and fields. It lives on the resolver because it owns the warning
  sink (`Lowerer::warn` forwards) and `define_tag` runs once per tag.
- Before a consumer reads a list, call sites drop inapplicable attributes
  with `module::applies`.
- Rule functions shared by both passes: `symbol_attributes`,
  `function_symbol` ([sema-passes](sema-passes.md#checker-owned-rules)).
- `cleanup(f)`: the argument is an identifier `Expr`, name-resolved like
  any reference. `Checker::cleanup` (automatic objects) requires a
  function with one prototyped parameter (variadic allowed; gcc also takes
  an unprototyped one) and classifies `&var` → parameter as an argument
  conversion: clang rejects every mismatch except dropped qualifiers, gcc
  applies the conversion warning's default severity.
- Known duplicate: the `always_inline` / `noinline` conflict is checked in
  `Checker::inlining` and again in `record_function`.

## Consumers

| Consumer | Reads | IR result |
| --- | --- | --- |
| `module::symbol_attributes` | `visibility`, `tls_model`, `weak`, `alias`, `section`, `used`, `retain`, `dllimport`/`dllexport`, `weakref`, `ifunc`, `selectany`, `asm("sym")` | `SymbolAttributes` on `Global` / `Function` ([ir/declarations](ir/declarations.md#symbol-attributes)) |
| `module::function_symbol` | same, filtered to function-relevant ones; rejects a register asm label or `__declspec(thread)` on a function | function `SymbolAttributes` |
| `Pragmas::apply` | `#pragma visibility`, `#pragma weak`, `#pragma redefine_extname` | fills unset `SymbolAttributes` fields |
| `Lowerer::record_function` | `gnu_inline`, `always_inline`, `noinline`, `noreturn`, `naked`, `const`, `pure` | `FunctionSemantics`; every attribute is also kept |
| `render_c_attributes` | the kept attributes, integer constant arguments folded | `c_attributes` metadata |
| `types::requested_alignment` | `aligned`, `_Alignas` | object request → `[align=N]` ([object properties](ir/declarations.md#object-properties)) |
| `types::field_request` | `packed`, `aligned` on field and its declaration | record layout |
| record layout (`define_tag`) | `packed`, `ms_struct`, `gcc_struct`, `aligned` on the tag, plus `#pragma pack` / `ms_struct` | offsets and alignment |
| entity request | `common`, `nocommon` | `[common]` |
| `TypeResolver::resolve_declarator` | declarator-position `vector_size`, `ext_vector_type`, `mode` | the declarator's type |
| `Lowerer` local declaration | `cleanup` on an automatic object | `Variable.cleanup`, the function's `BindingId` (`[cleanup=%N]`) |
| function type | `CallingConvention` | x86 conventions in the type ([calling conventions](ir/calls-abi.md#calling-conventions)) |

Statement attributes: the checker validates `[[fallthrough]]` placement;
lowering drops statement attributes
([control-flow](ir/control-flow.md)).

## Preprocessor queries

- `__has_attribute` → `attribute_support::has_attribute`: the same
  registry as the parser. gcc's also counts standard attributes without a
  GNU spelling (`nodiscard`, `maybe_unused`, `unsequenced`, …).
- `__has_c_attribute` → `has_c_attribute`: a registered scoped name is 1
  (0 under msvc); standard attributes come from `STANDARD_ATTRIBUTES`.
  clang: each attribute's own date (`deprecated` 201904, `nodiscard`
  202003, …), 0 for `unsequenced`/`reproducible`. gcc: 202311 for all.
  cl: only in C23, 202311 for `fallthrough`, `maybe_unused`, `nodiscard`,
  else 0. Pre-C23 cl rejects scoped operands (C2278); slate answers 0
  (permissive). Fixtures: `has-c-attribute-{clang,gcc,msvc-c23,msvc-c17}.c`.
- Visible to `#ifdef` / `defined`: clang and gcc define
  `__has_include(_next)`, `__has_embed`, `__has_attribute`,
  `__has_c_attribute`, `__has_builtin`, `__has_feature`,
  `__has_extension` (+ `__building_module` clang, `__has_cpp_attribute`
  gcc); cl only `__has_include`, `__has_c_attribute`. Unevaluated clang
  operators (`__has_declspec_attribute`, `__has_warning`,
  `__is_identifier`) stay undefined. glibc `<string.h>` gates its C23
  `strchr`/`memchr`/`strstr` wrappers on `#ifdef __has_extension`.
  Fixtures: `has-operators-defined-{clang,gcc,msvc}.c`.

## Adding an attribute

1. Add the `Attribute` variant ([ast-enum-touchpoints](ast-enum-touchpoints.md#adding-an-attribute-variant)).
2. Parse its arguments in `parse_attribute_value`.
3. Add it to `GNU_ATTRIBUTES` with per-flavor gates (check each oracle's
   `__has_attribute` per target), or to `CLANG_DECLSPECS` / the C23 name
   map.
4. Classify it in `general_use`, and in `inapplicable` if clang restricts
   its subjects.
5. Consume it: a `SymbolAttributes` field, `record_function`, a layout
   request, or nothing (it still appears in `c_attributes`).
6. Update the IR docs and grammar if it changes printed output.
