# Diagnostic severity

<!-- toc -->
- [Severity resolution](#severity-resolution)
- [Warnings](#warnings)
- [Conversion warnings](#conversion-warnings)
- [Parameter alignment](#parameter-alignment)
- [Type-level extension warnings](#type-level-extension-warnings)
- [Unrecognized `-W` names](#unrecognized--w-names)
- [`#warning` and `#error`](#warning-and-error)
- [Testing](#testing)
<!-- /toc -->

Accepted extensions and incompatibilities carry a named `Warning` so flags
can silence or promote them. `src/diagnostics.rs` holds the `Warning` enum
(identity, default severity, pedantic membership) and `DiagnosticOptions`
(resolved map, on `CompilerOptions`, read via `unit.dialect.options()`).
Strictness policy: [architecture](parser-architecture.md#strictness-policy).
Error limit, poisoning, and locations:
[sema-passes](sema-passes.md#error-reporting).
Attribute registration and applicability: [attributes](attributes.md).

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
- `conflicting-types` comes from the checker (`declare_object`), stashed in
  `item_diagnostics` like other checker warnings, so a promoted warning is
  a checker error.
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
