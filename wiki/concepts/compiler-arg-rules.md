# Compiler argument rules

<!-- toc -->
- [Option parsing](#option-parsing)
- [Include search](#include-search)
- [Forced files and macros](#forced-files-and-macros)
- [Rule buckets](#rule-buckets)
- [Adding an option](#adding-an-option)
<!-- /toc -->

Arguments are parsed by the declarative `Opt` parser, then checked by the
rule pipeline. Flag effects are in [compiler-flags](compiler-flags.md).

## Option parsing

- `Opt` owns an option's identity, spellings, value form, and opposite
  (`-fwrapv`, `--fwrapv`, `-fno-wrapv`, `--fno-wrapv` are one option).
  Occurrences apply in order; a later opposite replaces the earlier value
  before rules run.
- Value options take `=value` or a separate argument. `-m` options use the
  same definitions; no one-off parsers. Downstream sees typed values, never
  spellings.
- `-target` / `--target` (and `=` forms): the triple must be registered in
  `src/target_registry.rs` and have predefines for `--flavor`; checked at
  parse time (`TargetInfo::for_triple_and_flavor`). See
  [adding-a-target](adding-a-target.md).
- `-std` / `--std`: C90 and ISO 9899 aliases normalize to a language mode.
  `iso9899:199409` is C94: `__STDC_VERSION__ 199409L`, otherwise C89 rules.
  `c2y`/`gnu2y` predefine `__STDC_VERSION__` per flavor (gcc 16 `202500L`,
  clang 23 `202400L`, `LanguageStandard::predefined_stdc_version`);
  feature gates compare `stdc_version`, which is `202400` for both.
  Unknown triples and standards are errors.
- `ignored_option` is the single list of accepted-and-ignored arguments,
  shared by the parser and slate's compile-command normalization: driver and
  output options (`-c`, `-M*`, `-g*`, `-pipe`, `-o`/`-MF`/`-MT`/`-MQ`/`-MJ`
  with a separate or joined value) and codegen-only `-f`/`-fno-` flags
  (`CODEGEN_ONLY_FLAGS`, spelled like `Opt` flags; `-flto=`, `-fvisibility=`,
  `-fdebug-prefix-map=`). A flag belongs there only if it changes neither
  semantics nor predefined macros: `-fPIC`/`-fPIE` (`__PIC__`/`__PIE__`),
  `-fstack-protector*` (`__SSP*__`), and `-fcf-protection` stay unknown
  options until modeled.
- `-masm=att|intel`: picks the `{att|intel}` alternative in x86 GNU asm and
  is recorded as the asm's `dialect`. gcc flavor rejects it off x86; clang
  accepts it everywhere (no effect off x86); msvc rejects it.

## Include search

Built by `CompilerArgs::search_paths`.

- Quoted: including file's directory, `-iquote`, `-I`, system. Angled:
  `-I`, system. `-I`/`-iquote` headers are user headers; the rest are
  system headers.
- System order: `-isystem`, compiler builtin headers, sysroot standard
  headers, `-idirafter`. `-nostdlibinc` drops only the sysroot standard
  headers.
- Standard header directories come from the target's `SysrootLayout` (msvc
  flavor always uses Windows kits); builtin headers from `ClangHeaders`.
- `-isysroot` beats `--sysroot`; either replaces the target default root,
  with no fallback to other targets or the host.
- Relative directories resolve against the working directory. A leading
  `=` resolves under the sysroot, as in clang.

## Forced files and macros

- `-D`/`-U` apply in order after target predefines, then every `-imacros`
  file in order, then every `-include` file in order.
- Forced files are searched from the working directory before the include
  paths.
- `-imacros` keeps macros and discards declarations. Declarations from
  `-include` files are translation-unit roots with their own provenance.
  Macros from both keep their definition spans.

## Rule buckets

Put a rule in the narrowest bucket that owns the constraint.

- `common_rules`: flavor- and target-independent (e.g. mutually exclusive
  options). Shared semantics go here once.
- `flavor_rules`: one branch per flavor via `Rules::branch`; add only that
  compiler's differences. No `is_gcc()`/`is_clang()` inside a branch.
- Target rules: use the borrowed `TargetInfo`; report the target and the
  offending value.
- `Rules::when`: value rules for an optional option, applied only if
  present.
- `Rules::any` for real alternatives; `Rules::pipeline` / `Rules::all` when
  all must hold.
- Leaf rules use `Rule::validate` for dynamic messages. Failures are
  `thiserror` errors with miette diagnostics; combinators keep nested
  failures.
- Rules never re-parse strings or spellings. They may use presence to
  reject incompatible options (gcc's preferred stack boundary vs clang's
  stack alignment); both normalize to one `TargetInfo` value.

## Adding an option

1. Add an `Opt` definition.
2. Add validation to `common_rules`, a flavor branch, or a target rule.
3. Add a FileCheck fixture for accepted and rejected configurations if it
   changes target or diagnostic behavior.
