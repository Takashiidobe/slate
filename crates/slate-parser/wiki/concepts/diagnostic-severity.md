# Diagnostic severity

<!-- toc -->
- [Severity resolution](#severity-resolution)
- [Warnings](#warnings)
- [Conversion warnings](#conversion-warnings)
- [Parameter alignment](#parameter-alignment)
- [Attribute applicability](#attribute-applicability)
- [Attribute registration](#attribute-registration)
- [`__has_*` operators](#__has_-operators)
- [Type-level extension warnings](#type-level-extension-warnings)
- [Unrecognized `-W` names](#unrecognized--w-names)
- [`#warning` and `#error`](#warning-and-error)
- [Testing](#testing)
<!-- /toc -->

Accepted extensions and incompatibilities carry a named `Warning` so flags
can silence or promote them. `src/diagnostics.rs` holds the `Warning` enum
(identity, default severity, pedantic membership) and `DiagnosticOptions`
(resolved map, on `CompilerOptions`, read via `unit.dialect.options()`).
Strictness policy: [architecture](architecture.md#strictness-policy).
Error limit, poisoning, and locations:
[sema-passes](sema-passes.md#error-reporting).

## Severity resolution

`Warning::default_severity(standard, flavor)` is `Ignored`, `Warning`, or
`Error` (clang `DefaultError`, gcc `permerror`). `DiagnosticContext`
carries options, standard, and flavor.

```
severity(w) =
  None     if -Wno-<w>
  Error    if -Werror=<w>, or -Werror while enabled,
           or default_severity(w) is Error,
           or -pedantic-errors and w is pedantic
  Warning  if -W<w>, or default_severity(w) is Warning,
           or -pedantic and w is pedantic
  None     otherwise
```

- `-W<w>` keeps a default-error warning an error; only `-Wno-error=<w>`
  demotes, only `-Wno-<w>` silences.
- `-Wno-<w>` beats the pedantic group regardless of order; otherwise later
  flags win.
- `-Werror=<w>` enables a default-off warning; blanket `-Werror` does not.
- Severity never changes the AST or IR. `-m32 -std=c89 -pedantic-errors`
  still types `4294967296` as `long long` and errors on the use.
  `Availability::Extension` never becomes `Rejected`; `StandardFeatures`
  does not read `DiagnosticOptions`.
- Verified against clang 22.1.8 and gcc 16.2.1.

## Warnings

| Warning | Default | Pedantic | Raised when |
| --- | --- | --- | --- |
| `long-long` | off | yes | written `long long`, or literal rank `LongLong`, while `long_long_type` is not `Standard` |
| `c99-compat` | on in c89 | no | signed-only decimal literal lands on the C89-only `(Long, unsigned)` candidate |
| `implicitly-unsigned-literal` | on | no | signed-only decimal literal lands on an unsigned candidate at the widest rank, or (gcc) gcc's widest literal type |
| `integer-literal-too-large` | on | no | literal wider than `long long`, truncated (clang rejects, gcc warns unnamed) |
| `bit-int-extension` | off | yes | written `_BitInt` while `bit_int_type` is not `Standard` |
| `c23-extensions` | on | yes | unnamed parameter in a definition while `unnamed_definition_parameters` is not `Standard` |
| `pointer-sign` | on | yes | implicit pointer conversion whose integer pointees differ only in sign |
| `incompatible-pointer-types-discards-qualifiers` | on | yes | same conversions dropping pointee `const`/`volatile`, or differing qualifiers below the first level |
| `incompatible-pointer-types` | error; warning for msvc and gcc c89 | no | implicit pointer conversion without composite pointee, or dropping `_Atomic` |
| `pointer-type-mismatch` | on | no | clang: `?:` pointers to incompatible types, result `void *` |
| `int-conversion` | error; warning for msvc and gcc c89 | no | implicit integer↔pointer conversion |
| `pointer-integer-compare` | on | no | pointer compared with a non-null-constant integer |
| `compare-distinct-pointer-types` | on | no | pointers without composite pointee, neither `void` |
| `conflicting-types` | on | no | redeclaration conflict only MSVC accepts ([table](ir/declarations.md#redeclaration-conflicts)) |
| `parameter-alignment` | on | no | alignment attribute on a parameter |
| `ignored-attributes` | on | no | attribute outside clang's subject list |
| `unknown-attributes` | on | no | spelling the flavor doesn't register for the target (gcc `-Wattributes`, cl C5030) |

- `c99-compat` is not pedantic: clang keeps it a warning under
  `-pedantic-errors`.
- `c23-extensions` is clang's `ExtWarn` shape. gcc is silent, but the
  flavor doesn't change whether it's raised.
- Literal warnings come from the candidate `select_integer_candidate`
  (`sema/validate.rs`, shared with `sema/numeric.rs`) picked
  ([integer literals](ir/types.md#integer-literals)).

## Conversion warnings

| | clang 22.1.8 | gcc 16.2.1 | MSVC 19.51 |
| --- | --- | --- | --- |
| `int-conversion` | error, every `-std` | error; warning in c89/gnu89 | warning C4047 |
| `incompatible-pointer-types` | error, every `-std` | same c89/gnu89 demotion | warning C4133 |

- gcc's demotion uses `stdc_version().is_none()`, like `c99-compat`.
- Dropping `_Atomic` from a pointee errors under clang and gcc (MSVC has no
  `_Atomic`).
- `pointer-type-mismatch`: gcc reports it as `incompatible-pointer-types`,
  MSVC as C4133 with the left operand's type.
- Raised by the checker: `TypeResolver::record_conversion` warns on first
  record; compound assignment results go through `compound_conversion`
  (lowering's `update` re-classifies silently).
- An item whose stashed diagnostics (`item_diagnostics`) include an error
  moves them all into the checker's errors, so `parse` rejects `i = p`.
- `conflicting-types` comes from lowering: `Lowerer::warn` →
  `diagnostics` → `with_sources` (shared with `analyze`), returned next to
  the `Module`. A promoted warning fails `ir --dump-ir`.
- Warnings point at the converted value's node (`take((unsigned *)p)` →
  `p`).
- Fixtures: `error/conversion_default_errors.c`,
  `error/conversion_default_errors_gcc.c`,
  `sema/conversion_severity_{gcc,msvc}.c`,
  `sema/conversion_no_error_downgrade.c`, `sema/conversion_suppressed.c`.

## Parameter alignment

| Written on a parameter | clang | gcc | MSVC |
| --- | --- | --- | --- |
| `_Alignas(N) int p` | error | error | accepted; `__alignof(p)` = N raised to natural |
| `int p __attribute__((aligned(N)))` | accepted; `_Alignof(p)` = N | error | not parsed |

No rejecting majority, and every compiler that parses a spelling applies
it. The request goes on the parameter entity; `_Alignof` reads it via
`declared_alignment` (msvc raises below-natural requests). The IR has no
parameter slot, so it is only observable through `_Alignof`.

## Attribute applicability

`sema/attributes.rs::declaration_use(attribute, subject)`, where `Subject`
is `Function`, `Object { automatic }`, `Parameter`, `Typedef`, `Field`,
`Record { union }`:

- `Inapplicable`: outside clang's subject list; dropped, `-Wignored-attributes`.
- `Symbol` / `Layout` / `Ignored`: applied, or nothing for the IR.
- `Unsupported`: applicable but not expressible; rejects the declaration.

`TypeResolver::check_attributes` is the single place that turns this into a
warning or `ResolveError`. It lives on the resolver (which owns the warning
sink; `Lowerer::warn` forwards) because record and field attributes are
only seen in `define_tag`, which runs once per tag. Call sites filter out
inapplicable attributes before `symbol_attributes` / `requested_alignment`.

clang 22.1.8; `ok` accepted, `ign` `-Wignored-attributes`, `err` error:

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

- `transparent_union` on a union is `Ignored`: the convention isn't
  modeled, but a call needing it fails in argument conversion.
- Attributes before the tag keyword of a declarator-less declaration
  (`__attribute__((packed)) struct S {...};`) never reach the tag; ignored
  with a warning, as clang does.
- Fixtures: `sema/ir_attribute_applicability.c`,
  `sema/attribute_applicability_warnings.c`,
  `sema/record_attribute_applicability.c`,
  `error/field-section-attribute.c`.

## Attribute registration

Whether the flavor knows a spelling on this target, checked before
applicability.

- `src/attribute_support.rs`: hand table per `__attribute__` name with a
  clang, gcc, and msvc gate (`Always`, `Never`, `Windows`, `X86`, …).
- `[[scope::name]]` is registered per name, not per scope: clang has
  `gnu::packed` but not `clang::packed`, `clang::overloadable` but not
  `gnu::overloadable`, and only `msvc::noinline`; gcc has only `gnu::`;
  msvc accepts any `msvc::` name.
- Unmodeled names fall back to `src/attribute_support/registered.rs`,
  generated by `tools/generate_attribute_lists.py` from each compiler's
  `__has_attribute` / `__has_c_attribute` over identifiers in its binaries.
  These ignore the target, so e.g. clang's Windows-only `guard` is
  registered everywhere (permissive).
- The parser turns an unregistered modeled spelling into
  `Attribute::Unknown`, so it never reaches lowering (no `dllimport` on
  ELF). `check_attributes` warns `-Wunknown-attributes` only if the
  fallback list lacks it too.
- `__has_attribute` reads the same registry; gcc's also counts standard
  attributes without a GNU spelling (`nodiscard`, `maybe_unused`,
  `unsequenced`, …).
- Examples (clang 22.1.8 over 13 triples, gcc 16.2.1 x86_64/aarch64, cl
  19.51): clang lacks `noipa`, `noclone`, `optimize`,
  `scalar_storage_order`; registers `dllimport` only on Windows and
  `interrupt` everywhere but aarch64. gcc on aarch64 lacks `naked`,
  `interrupt`, x86 conventions. msvc registers no GNU spelling.
- `__declspec` is not gated (clang: `-Wignored-attributes`; cl: C2485).
- Fixtures: `sema/unknown_attributes_{clang_linux,clang_windows,gcc_aarch64}.c`.

## `__has_*` operators

- `__has_c_attribute` → `attribute_support::has_c_attribute`: registered
  scoped name = 1; standard attributes use a per-flavor date table. clang:
  each attribute's own date (`deprecated` 201904, `nodiscard` 202003, …),
  0 for `unsequenced`/`reproducible`. gcc: 202311 for all eight. cl: only
  in C23, 202311 for `fallthrough`, `maybe_unused`, `nodiscard`, else 0
  (scoped too). Pre-C23 cl rejects scoped operands (C2278); slate answers
  0 (permissive). Fixtures: `has-c-attribute-{clang,gcc,msvc-c23,msvc-c17}.c`.
- Visible to `#ifdef` / `defined`: clang and gcc define
  `__has_include(_next)`, `__has_embed`, `__has_attribute`,
  `__has_c_attribute`, `__has_builtin`, `__has_feature`,
  `__has_extension` (+ `__building_module` clang, `__has_cpp_attribute`
  gcc); cl only `__has_include`, `__has_c_attribute`. Unevaluated clang
  operators (`__has_declspec_attribute`, `__has_warning`,
  `__is_identifier`) stay undefined. glibc `<string.h>` gates its C23
  `strchr`/`memchr`/`strstr` wrappers on `#ifdef __has_extension`.
  Fixtures: `has-operators-defined-{clang,gcc,msvc}.c`.

## Type-level extension warnings

- `extension_warning(ty, features)` (`sema/validate.rs`) maps a specifier
  to its warning. A new type-level warning is one match arm.
- `check_type` calls it for declarations, parameters, fields, and returns.
  Block-scope locals use `check_body_types` / `walk_type` (`check_type`'s
  `typedefs` set is file-scope only).
- One warning per specifier: `long long a, b, c;` warns once (clang warns
  per declarator; not reproduced).

## Unrecognized `-W` names

Accepted and ignored, as clang does. Only `Warning` names have effect.

## `#warning` and `#error`

- Not `Warning`s; severity is fixed. `Preprocessor::record_directive_diagnostic`
  records a `DirectiveDiagnostic`; `main.rs::report_directives` prints
  warnings and fails on errors, before the parse result is unwrapped, so
  both a `#warning` and a later error are reported in order.
- `tools/corpus_sweep.py` `DIAGNOSTIC_TIERS` picks a failure label: an
  indented `× …` first, then `Error:   × …`, then `⚠ …` only without any
  error.

## Testing

- `// SLATE-FILECHECK-WARNING <prefix>` checks stderr of a successful run;
  `// SLATE-FILECHECK-ERROR` covers errors. Both generated by
  `tools/update_filecheck.py`.
- A fixture with either directive generates only those blocks
  (`generated_blocks`), so checking stderr and IR needs two files
  (`sema/warning_directive_nonfatal.c`, `sema/ir_warning_directive.c`).
