# Diagnostic severity

Extension and compatibility acceptances carry a named identity so command-line
flags can suppress them or promote them to errors. `src/diagnostics.rs` owns
both halves: the `Warning` enum (identity, default-on rule, pedantic
membership) and `DiagnosticOptions` (the resolved severity map).

`DiagnosticOptions` lives on `CompilerOptions`, so it reaches sema through
`unit.options` without new plumbing.

## Severity resolution

```
severity(w) =
  None                       if -Wno-<w>
  Error                      if -Werror=<w>, or -Werror while enabled,
                             or -pedantic-errors and w is pedantic
  Warning                    if -W<w>, or default-on for the standard,
                             or -pedantic and w is pedantic
  None                       otherwise
```

`None` means the diagnostic is not produced at all. An explicit `-Wno-<w>`
wins over the pedantic group regardless of flag order; otherwise later flags
win, matching the `Opt` parser's ordering rule.

`-Werror=<w>` also _enables_ a default-off warning, which blanket `-Werror`
does not. Verified against clang 22: `-Werror=long-long` alone errors on a
c89 `long long`, while `-Werror` alone is silent, as is `-Wno-error=<w>`.

## Severity never changes the AST or IR

Verified against clang 22: `clang -m32 -std=c89 -pedantic-errors` still types
`4294967296` as `long long` and reports the _use_ of the extension as an
error. The extension type is chosen first; severity only decides how the use
is reported. So `Availability::Extension` never degrades to `Rejected`, and
`StandardFeatures` does not read `DiagnosticOptions`. An earlier design note
on slate-parser-47s.8 claimed the opposite; it was wrong.

## Warnings in use

| Warning                       | Default   | Pedantic | Raised when                                                                                                                 |
| ----------------------------- | --------- | -------- | --------------------------------------------------------------------------------------------------------------------------- |
| `long-long`                   | off       | yes      | a written `long long` specifier, or a selected integer literal rank of `LongLong`, while `long_long_type` is not `Standard` |
| `c99-compat`                  | on in c89 | no       | a signed-only decimal literal lands on the C89-only `(Long, unsigned)` candidate                                            |
| `implicitly-unsigned-literal` | on        | no       | a signed-only decimal literal lands on an unsigned candidate at the widest rank                                             |
| `c23-extensions`              | on        | yes      | a function definition's parameter has no name while `unnamed_definition_parameters` is not `Standard`                      |
| `pointer-sign`                | on        | yes      | an implicit pointer conversion (assign/init, argument, return) whose integer pointees differ only in signedness            |
| `incompatible-pointer-types-discards-qualifiers` | on | yes | the same conversions dropping pointee `const`/`volatile`, or differing in qualifiers below the first pointer level |
| `conflicting-types`           | on        | no       | a redeclaration conflict that clang and gcc reject but MSVC accepts: same-size integer types differing in sign, or differing prototyped parameter lists; see [`ir-spec.md`](ir-spec.md) |

The two pointer warnings are clang `ExtWarn`s and need resolved types, so
they come from IR lowering rather than `TranslationUnit::analyze`. So does
`conflicting-types`, which has no clang counterpart (clang errors) and is
named after clang's "conflicting types" error. `Lowerer`
collects them in `diagnostics` through `Lowerer::warn`, `resolve_module`
returns them next to the `Module`, and the driver passes them through
`sema::with_sources`, the same function `analyze` uses. A promoted warning
fails `ir --dump-ir` exactly like an `analyze` error. A warning is reported on
the converted value's node, which carries the operand's span, so a warning on
`take((unsigned *)p)` points at `p` rather than the whole cast.

`c99-compat` is deliberately not in the pedantic group: clang leaves it a
warning even under `-pedantic-errors`.

`c23-extensions` is clang's `ExtWarn` shape: on by default, and an error
under `-pedantic-errors`. gcc accepts unnamed definition parameters before
C23 silently, but the flavor does not change whether the warning is raised.

Both literal warnings are derived from the candidate the selection actually
picked, so they cannot drift from the typing rules in
[`ir-spec.md`](ir-spec.md). `select_integer_candidate` in `sema/validate.rs`
is the one selection point, shared with the IR lowering in `sema/numeric.rs`.

## Where type-level extension warnings come from

Two things would otherwise be re-derived for every new warning: _where_ types
are written, and _which_ feature a written specifier needs. Each is stated
once.

`extension_warning(ty, features)` in `sema/validate.rs` maps a single
specifier to the warning it earns. Adding a type-level extension warning is a
new match arm there — no new call sites.

`check_type` consults it for every specifier it already visits, so the
declarations, parameters, fields, and return types it walks are covered for
free. Block-scope locals cannot use `check_type`, because its `typedefs` set
is file-scope only and a function-local `typedef` would be misreported as an
unknown type name; `check_body_types` walks those with `walk_type` and the
same table.

## One warning per specifier, not per declarator

`long long a, b, c;` reports once. Clang reports three identical diagnostics
at the same span, because it warns in `ConvertDeclSpecToType`, which runs once
per declarator. That is an artifact of clang's implementation, not a semantic
difference, so it is not reproduced. Every other location and count matches:
verified over 54 (source, standard, flag) combinations.

## Unrecognized `-W` names

Accepted and ignored, as clang does by default, so real command lines carrying
`-Wall -Wextra -Wshadow` parse. Only names in the `Warning` enum have an
effect.

## Testing

A successful run prints warnings to stderr, which the FileCheck harness does
not read by default. `// SLATE-FILECHECK-WARNING <prefix>` FileChecks stderr
for a config that is still expected to succeed; `// SLATE-FILECHECK-ERROR`
already covers the promoted-to-error cases. Both are generated by
`tools/update_filecheck.py`.
