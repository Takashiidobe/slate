# Diagnostic severity

Extension and compatibility acceptances carry a named identity so command-line
flags can suppress them or promote them to errors. `src/diagnostics.rs` owns
both halves: the `Warning` enum (identity, default severity, pedantic
membership) and `DiagnosticOptions` (the resolved severity map).

`DiagnosticOptions` lives on `CompilerOptions`, so it reaches sema through
`unit.options` without new plumbing.

## Severity resolution

Every warning has a *default severity* for a given standard and compiler
flavor: `Ignored`, `Warning`, or `Error`. `Error` is clang's `DefaultError`
and gcc's `permerror` — on by default *as an error*, but still an ordinary
warning as far as the flags are concerned.

```
severity(w) =
  None                       if -Wno-<w>
  Error                      if -Werror=<w>, or -Werror while enabled,
                             or default_severity(w) is Error,
                             or -pedantic-errors and w is pedantic
  Warning                    if -W<w>, or default_severity(w) is Warning,
                             or -pedantic and w is pedantic
  None                       otherwise
```

`-W<w>` enables at the default severity, so it leaves a default-error warning
an error; only `-Wno-error=<w>` demotes one, and only `-Wno-<w>` silences it.
Verified against clang 22.1.8 and gcc 16.2.1 on `-Wint-conversion`.

`default_severity` takes the flavor as well as the standard, which is what
lets one warning be an error under clang and a warning under MSVC without a
second diagnostic identity. `DiagnosticContext` carries the three inputs
(options, standard, flavor) so a new warning does not thread them itself.

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

## Three ways to answer a severity question

1. **Correct per compiler and flags.** The most work, and the goal wherever
   the split is measurable and the flavor axis already reaches the decision.
2. **Permissive.** One answer, no stricter than the most permissive compiler.
   Cheap, and where most of this lands: polish costs more than it returns for
   a diagnostic nobody configures.
3. **Too strict for all three.** The bug. A configuration is rejected that no
   real compiler rejects, and real code stops ingesting.

(2) is an accepted cost, not the correct answer — so moving a warning from (2)
to (1) needs no justification beyond the measurement. (3) is always a bug.

## Warnings in use

| Warning                       | Default   | Pedantic | Raised when                                                                                                                 |
| ----------------------------- | --------- | -------- | --------------------------------------------------------------------------------------------------------------------------- |
| `long-long`                   | off       | yes      | a written `long long` specifier, or a selected integer literal rank of `LongLong`, while `long_long_type` is not `Standard` |
| `c99-compat`                  | on in c89 | no       | a signed-only decimal literal lands on the C89-only `(Long, unsigned)` candidate                                            |
| `implicitly-unsigned-literal` | on        | no       | a signed-only decimal literal lands on an unsigned candidate at the widest rank, or (gcc flavor) on gcc's widest literal type |
| `integer-literal-too-large`   | on        | no       | an integer literal wider than `long long` is truncated to it; clang rejects, gcc warns (no gcc `-W` name, so the name is slate's) |
| `bit-int-extension`           | off       | yes      | a written `_BitInt` type while `bit_int_type` is not `Standard`                                                           |
| `c23-extensions`              | on        | yes      | a function definition's parameter has no name while `unnamed_definition_parameters` is not `Standard`                      |
| `pointer-sign`                | on        | yes      | an implicit pointer conversion (assign/init, argument, return) whose integer pointees differ only in signedness            |
| `incompatible-pointer-types-discards-qualifiers` | on | yes | the same conversions dropping pointee `const`/`volatile`, or differing in qualifiers below the first pointer level |
| `incompatible-pointer-types`  | error, warning under msvc and gcc c89 | no | an implicit pointer conversion whose pointees have no composite type (`unsigned * → long *`, `struct A * → struct B *`), or which drops `_Atomic` from the pointee |
| `int-conversion`              | error, warning under msvc and gcc c89 | no | an implicit conversion between an integer and a pointer across an assignment, argument, return or initializer                                                  |
| `pointer-integer-compare`     | on        | no       | a comparison between a pointer and an integer that is not a null pointer constant                                                                               |
| `compare-distinct-pointer-types` | on     | no       | a comparison between pointers whose pointees have no composite type and neither is `void`                                                                       |
| `conflicting-types`           | on        | no       | a redeclaration conflict that clang and gcc reject but MSVC accepts: same-size integer types differing in sign, or differing prototyped parameter lists; see [`ir-spec.md`](ir-spec.md) |
| `parameter-alignment`         | on        | no       | an alignment attribute on a function parameter, which no two of the three compilers agree to reject |
| `ignored-attributes`          | on        | no       | an attribute written on a subject outside clang's subject list for it; see [attribute applicability](#attribute-applicability) |
| `unknown-attributes`          | on        | no       | a `__attribute__` or `[[scope::name]]` spelling the flavor does not register for the target (gcc calls it `-Wattributes`, cl C5030); see [attribute applicability](#attribute-applicability) |

`incompatible-pointer-types` and `int-conversion` were at (2): one global
warning, chosen because MSVC only warns on all of them (C4047, C4057, C4133)
while clang and gcc reject. Severity is now flavor-aware, so they are at (1)
instead (measured 2026-09-21, slate-parser-dyd.43):

| | clang 22.1.8 | gcc 16.2.1 | MSVC 19.51 |
| --- | --- | --- | --- |
| `int-conversion` | error, every `-std` | error, except c89/gnu89 → warning | warning C4047 |
| `incompatible-pointer-types` | error, every `-std` | same c89/gnu89 demotion | warning C4133 |

gcc's demotion keys off the same `stdc_version().is_none()` predicate
`c99-compat` uses. Neither warning is pedantic: they report genuinely
incompatible C rather than an extension.

This also reverses slate-parser-hdt a second time. Dropping `_Atomic` from a
pointee is an error under the clang and gcc flavors, which both reject it;
MSVC casts no vote, since it does not parse `_Atomic` at all. Fixtures:
`error/conversion_default_errors.c`, `error/conversion_default_errors_gcc.c`,
`sema/conversion_severity_gcc.c`, `sema/conversion_severity_msvc.c`,
`sema/conversion_no_error_downgrade.c`, `sema/conversion_suppressed.c`.

`parameter-alignment` covers both spellings of an alignment request on a
parameter, which split the compilers differently but never reach a rejecting
majority (measured 2026-09-21, clang 22.1.8 / gcc 16.2.1 / MSVC 19.51):

| written on a parameter | clang | gcc | MSVC |
| --- | --- | --- | --- |
| `_Alignas(N) int p` | error | error | accepted, `__alignof(p)` is `N` raised to natural |
| `int p __attribute__((aligned(N)))` | accepted, `_Alignof(p)` is `N` as written | error | not parsed at all |

MSVC's rejection of the second is a syntax error for `__attribute__`, not a
judgement about parameter alignment, so it is not a vote to reject. Every
compiler that understands either spelling *applies* the alignment, so
slate-parser applies it too: the request goes on the parameter's entity and
`_Alignof` reads it back through `declared_alignment`, which is why the MSVC
flavor raises a below-natural request and the others report it as written.
The IR has no parameter home slot to align, so the request is observable only
through `_Alignof`.

## Attribute applicability

Whether an attribute is *applied* or *dropped* is a property of the pair
(attribute, subject), not of the attribute alone, and getting it wrong
produces wrong IR — a visibility, section or `used` flag on a declaration
clang leaves alone — rather than only a missing diagnostic. `sema/attributes.rs`
states the pair once: `Subject` names what the attribute is written on
(`Function`, `Object { automatic }`, `Parameter`, `Typedef`, `Field`,
`Record { union }`) and
`declaration_use(attribute, subject)` answers with

- `Inapplicable` — outside clang's subject list: dropped, never applied, and
  reported as `-Wignored-attributes`;
- `Symbol` / `Layout` / `Ignored` — applied, or carrying nothing the IR needs;
- `Unsupported` — applicable here, but lowering cannot yet express it, which
  is the only remaining reason to refuse a declaration.

`TypeResolver::check_attributes` is the single place that turns that answer
into a warning or a `ResolveError`. It lives on the type resolver, which also
owns the one warning sink (`Lowerer::warn` forwards to it), because record
and field attributes are only seen in `TypeResolver::define_tag`; `tag_ids` is
never rolled back, so a tag is defined, and diagnosed, once. Each call site filters the inapplicable
attributes out before `symbol_attributes` and `requested_alignment` read them.
That split is what fixes the too-strict half of slate-parser-dyd.15: before
it, `transparent_union`, `ms_struct`, `gcc_struct`, `ifunc` and
`scalar_storage_order` on a plain object were `Unsupported` and stopped the
lowering, where clang merely warns and keeps going.

Measured against clang 22.1.8 (2026-09-21); `ok` is silently accepted and
`ign` is `-Wignored-attributes`:

| written on | object | function | typedef | parameter |
| --- | --- | --- | --- | --- |
| `used`, `retain` | ok, `ign` if automatic | ok | `ign` | `ign` |
| `common`, `nocommon` | ok | `ign` | `ign` | ok |
| `visibility`, `weak` | ok | ok | `ign` | ok |
| `cleanup` | `ign` unless automatic | `ign` | `ign` | `ign` |
| `packed`, `ms_struct`, `gcc_struct`, `transparent_union` | `ign` | `ign` | `ign` | `ign` |
| `malloc`, `cold`, `ifunc`, `alloc_size`, … | `ign` | ok | `ign` | `ign` |

Fields and tag definitions (slate-parser-dyd.47, same clang); `err` is a hard
error in clang too:

| written on | field | struct/union definition |
| --- | --- | --- |
| `packed`, `aligned`, `vector_size`, `may_alias` | ok | ok |
| `ms_struct`, `gcc_struct` | `ign` | ok (layout) |
| `transparent_union` | `ign` | ok on a union, `ign` on a struct |
| `mode` | ok | `err` |
| `section`, `alias`, `tls_model` | `err` | `err` |
| `used`, `retain`, `weak`, `common`, `nocommon`, `cleanup`, `noreturn`, `nonnull` | `ign` | `ign` |
| `malloc`, `cold`, `format`, `noinline`, … | `ign` | `ign` |

`transparent_union` is `Ignored` rather than `Unsupported` on a union: the
calling convention is not modeled, but a call that needs it already fails in
the argument conversion, so accepting the definition cannot lower a wrong
program. Attributes written before the tag keyword of a declaration with no
declarators (`__attribute__((packed)) struct S {...};`) never reach the tag;
clang ignores them with a "place it after struct" warning whatever the
attribute, and so does slate (they used to be rejected when `Unsupported`).

Before subject applicability comes registration: whether the flavor knows the
spelling at all on this target. `src/attribute_support.rs` is the one
hand-maintained table of it — per `__attribute__` name, a clang, gcc and msvc
gate (`Always`, `Never`, `Windows`, `X86`, …). A `[[scope::name]]` spelling
is registered when the flavor lists the name under that scope and, if modeled,
its gate admits the target: scope membership is per name, not per scope —
clang has `gnu::packed` but not `clang::packed`, `clang::overloadable` but not
`gnu::overloadable`, and under `msvc::` only `noinline`; gcc has only `gnu::`;
msvc accepts any `msvc::` name (cl ignores unknown ones silently). Names slate does not model (`nonstring`, `access`, …) fall back to
`src/attribute_support/registered.rs`, generated by
`tools/generate_attribute_lists.py` from each compiler's own `__has_attribute`
(and `__has_c_attribute(scope::name)` for the scoped lists) over every
identifier in its binaries; those lists ignore the target, so an
unmodeled target-specific name such as clang's Windows-only `guard` is
permissively registered everywhere. The parser turns an unregistered modeled
spelling into `Attribute::Unknown`, so it never reaches lowering (no
`dllimport` marker on an ELF target); `check_attributes` reports an `Unknown`
as `-Wunknown-attributes` only when the flavor does not register it either,
so an unmodeled but real attribute stays silent. The preprocessor's
`__has_attribute` answers from the same registry, since in both clang and gcc
it and the unknown-attribute warning read one table — except that gcc's
`__has_attribute` also counts standard attributes with no GNU spelling
(`nodiscard`, `maybe_unused`, `unsequenced`, …). Measured 2026-09-26
(slate-parser-dyd.46) against clang 22.1.8 over 13 triples, gcc 16.2.1 on
x86_64 and aarch64, and cl 19.51 — e.g. clang does not register `noipa`,
`noclone`, `optimize` or `scalar_storage_order`, registers `dllimport` only on
Windows and `interrupt` everywhere but aarch64; gcc has none of the clang-only
spellings and, on aarch64, no `naked`, `interrupt` or x86 calling conventions;
cl has no `__attribute__` syntax, so msvc registers no GNU spelling.
`__declspec` is not gated yet: clang reports an unsupported declspec under
`-Wignored-attributes` and cl rejects it (C2485), so it needs its own column.
Fixtures: `sema/unknown_attributes_{clang_linux,clang_windows,gcc_aarch64}.c`
(warnings, the IR, and `__has_attribute` answers).

`__has_c_attribute` asks about the `[[...]]` spelling instead, and answers
`attribute_support::has_c_attribute`: a registered scoped name is 1, and a
standard attribute is a per-flavor date from the hand table there. The dates
differ by compiler (measured 2026-09-26, slate-parser-dyd.66): clang returns
each attribute's own date (`deprecated` 201904, `nodiscard` 202003, …) and 0
for `unsequenced`/`reproducible`; gcc returns 202311 for all eight; cl answers
only in C23 mode, 202311 for `fallthrough`, `maybe_unused` and `nodiscard` and
0 for everything else, scoped names included. clang and gcc answer in every C
mode. Before C23 cl rejects a scoped operand (C2278, `::` is not a token
there) where slate answers 0 — permissive. Fixtures:
`has-c-attribute-{clang,gcc,msvc-c23,msvc-c17}.c`.

The `__has_*` operators are also visible to `#ifdef`/`defined`, as each
compiler reports them: clang and gcc define `__has_include(_next)`,
`__has_embed`, `__has_attribute`, `__has_c_attribute`, `__has_builtin`,
`__has_feature` and `__has_extension` (plus `__building_module` in clang and
`__has_cpp_attribute` in gcc); cl defines only `__has_include` and
`__has_c_attribute`. clang's `__has_declspec_attribute`, `__has_warning` and
`__is_identifier` stay undefined because slate does not evaluate them. This
matters beyond attributes: glibc's `<string.h>` enables its C23
const-preserving `strchr`/`memchr`/`strstr` `_Generic` wrappers behind
`#ifdef __has_extension`. Fixtures: `has-operators-defined-{clang,gcc,msvc}.c`.

Fixtures: `sema/ir_attribute_applicability.c` (the applied/dropped half, in
the IR), `sema/attribute_applicability_warnings.c` (the diagnostics),
`sema/record_attribute_applicability.c` (fields and tags, with the layouts)
and `error/field-section-attribute.c`.

The two qualifier/sign pointer warnings are clang `ExtWarn`s and need resolved
types, so they come from IR lowering rather than `Sema::analyze`.
So do the four above. So does
`conflicting-types`, which has no clang counterpart (clang errors) and is
named after clang's "conflicting types" error. `Lowerer`
collects them in `diagnostics` through `Lowerer::warn`, and `resolve_module`
passes them through `with_sources`, the same function `analyze` uses, before
returning them next to the `Module`. A promoted warning
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

## Preprocessing directive diagnostics

`#warning` and `#error` are not in the `Warning` enum and have no flag; their
severity is fixed by the directive. `Preprocessor::record_directive_diagnostic`
tags each one on `DirectiveDiagnostic`, and `main.rs::report_directives` is the
only reader: it prints every `Severity::Warning` to stderr and turns only the
errors into the command's failure. It runs *before* the parse result is
unwrapped, so a `#warning` followed by an unrelated error reports both, in
source order.

That ordering is what made slate-parser-dyd.13 look like a severity bug.
`tools/corpus_sweep.py` labels a failing configuration with its root
diagnostic, and the pattern it matched accepted the warning marker as readily
as the error one, so `c23_language.c` was grouped under its leading `#warning`
rather than the `alignof` failure underneath. `DIAGNOSTIC_TIERS` now ranks the
candidates: an indented `× …` (the detailed diagnostic) first, then
`Error:   × …` (miette's wrapper, which is often just "semantic analysis
failed"), and a `⚠ …` only when the run produced no error marker at all.

## Reporting more than the first error

Sema reports up to 20 errors, clang's default `-ferror-limit`, then adds
"too many errors emitted, stopping now". Any error still means no IR: this is
reporting, not recovery. The limit lives in `with_sources`, so `Sema::analyze`
and `Sema::lower` share it; only errors count toward it. `-ferror-limit` is not
accepted because it changes no code's meaning.

Both IR-stage passes continue item by item over the top-level declarations:

- `names::resolve_items` runs once, in `Sema::new`, and both passes read its
  per-item results; `analyze` reports the unresolved typedef names ("unknown
  type name"), lowering the rest. It records an unresolved reference (ordinary,
  typedef, label, MS asm label) and keeps visiting, so declarations after it are still
  bound and `int a = undeclared, b;` does not cascade into errors on `b`. Any
  other names error ends that item and resets the resolver to file scope.
- `Sema::lower` does not lower an item with names errors, and does not run
  at all when `analyze` (the checker) reported an error. So every rejection
  is the checker's; a `Rejected`/`InvalidOperand(s)` that still escapes a
  lowered item or `finish_module` is turned into `Internal` by
  `ResolveError::checked` in `resolve_module` (slate-parser-cc94.5.6): it
  marks a checker gap, not an ill-formed program. A lowering error
  resets per-function `Lowerer` state and moves on. Raise sites don't carry a
  span; instead `Lowerer::expr`, `Lowerer::place` and `Lowerer::statements`
  wrap an error with `ResolveError::at(loc)`, which only sets `Located` when no
  location is attached yet. The innermost failing expression or statement
  wins, so the line matches clang's even when clang's caret sits on a token
  inside it (an operator, a pragma name). Errors outside any expression or
  statement, such as a file-scope declarator's type, still fall back to the
  top-level declaration.

Poisoning keeps one bad declaration from causing more errors: the bindings a
failed item declares are poisoned, and a later item's *lowering* error is
dropped if that item references a poisoned binding (clang marks such decls
invalid). The check runs before the failed item's own bindings are poisoned,
so a recursive function still reports its own error. Names errors are never
dropped: a name missing from scope is not caused by a lowering failure.
Post-passes (`finish_module`) run only on an error-free unit, since they would
only report the gaps the failed items left. Fixtures:
`sema_reports_all_errors.c`, `sema_poisoned_declaration.c`,
`sema_error_limit.c`.

## Testing

A successful run prints warnings to stderr, which the FileCheck harness does
not read by default. `// SLATE-FILECHECK-WARNING <prefix>` FileChecks stderr
for a config that is still expected to succeed; `// SLATE-FILECHECK-ERROR`
already covers the promoted-to-error cases. Both are generated by
`tools/update_filecheck.py`.

A fixture carrying either directive generates *only* those blocks —
`generated_blocks` returns early — so pinning both streams for one source takes
two files. `sema/warning_directive_nonfatal.c` holds the stderr half of the
`#warning` case and `sema/ir_warning_directive.c` the IR half.
