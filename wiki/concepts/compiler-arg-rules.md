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
- On/off switches (`SWITCH_OPTS`) are named once by their positive
  spelling in `Opt::switch` (`-fhosted`, `-fshort-wchar`); `parse_switch`
  derives the `-fno-`/`-mno-` form and the `--` alias. When a flavor has
  only the positive spelling, `Opt::negation_accepted_by` says so
  (`-fno-freestanding` and `-fno-hosted` are gcc only).
- Value options take `=value` or a separate argument. `-m` options use the
  same definitions; no one-off parsers. Downstream sees typed values, never
  spellings.
- `-target` / `--target` (and `=` forms): the triple must be registered in
  `src/target_registry.rs` and have predefines for `--flavor`; checked at
  parse time (`TargetInfo::for_triple_and_flavor`). See
  [adding-a-target](adding-a-target.md). A GNU short triple
  (`x86_64-linux-gnu`) resolves to its `unknown`-vendor entry
  (`target_registry::lookup`).
- `-m16` / `-m32` / `-m64` / `-mx32` (gcc, clang; last wins) rewrite the
  `--target` triple before lookup, whatever their order in argv
  (`target_registry::arch_variant`): x86_64 → i686 for `-m16`/`-m32`,
  i386..i686 → x86_64 for `-m64`, `-m64` is a no-op on x86_64 and aarch64,
  and `-mx32` selects `x86_64-…-gnux32`. Any other combination, or a
  variant with no registry entry (`i686-apple-darwin`, x32), is rejected.
- `-std` / `--std`: C90 and ISO 9899 aliases normalize to a language mode.
  `iso9899:199409` is C94: `__STDC_VERSION__ 199409L`, otherwise C89 rules.
  `c2y`/`gnu2y` predefine `__STDC_VERSION__` per flavor (gcc 16 `202500L`,
  clang 23 `202400L`, `LanguageStandard::predefined_stdc_version`);
  feature gates compare `stdc_version`, which is `202400` for both.
  Unknown triples and standards are errors.
- `ignored_option` is the list of arguments every flavor accepts and ignores:
  driver and
  output options (`-c`, `-M*`, `-g*`, `-pipe`, `-o`/`-MF`/`-MT`/`-MQ`/`-MJ`
  with a separate or joined value) and codegen-only `-f`/`-fno-` flags
  (`CODEGEN_ONLY_FLAGS`, positive spellings matched with `parse_switch`;
  `-flto=`, `-fvisibility=`, `-fdebug-prefix-map=`). A flag belongs there
  only if it changes neither semantics nor predefined macros; PIC/PIE,
  stack protector, `-fcf-protection` and `-fasynchronous-unwind-tables`
  change macros, so they are `Opt`s. A flavor-specific flag with no IR or
  macro effect (`-mcmodel=`) is an `Opt` that is parsed and dropped, so
  flavor checking still applies; backend-only validation (code model per
  target, PIC on windows-msvc) is not emulated.
- `-masm=att|intel`: picks the `{att|intel}` alternative in x86 GNU asm and
  is recorded as the asm's `dialect`. gcc flavor rejects it off x86; clang
  accepts it everywhere (no effect off x86); msvc rejects it.
- Which flavors have an option is `Opt::accepted_by`, an exhaustive match,
  so every new `Opt` must name its flavors. Parsing records each
  `(Opt, argument)` occurrence without regard to flavor, because `--flavor`
  may come later in argv; `check_flavor` then rejects the first occurrence
  the final flavor lacks as `unknown option for the <flavor> flavor`, as
  the real compiler would (`-mstack-alignment` is clang-only,
  `-mpreferred-stack-boundary` gcc-only, and msvc has none of the GNU
  codegen, ISA and optimization options).
- `CLANG_CODEGEN_ONLY_M_FLAGS` (`Opt::ClangCodegenOnly`): `-m`/`-mno-` flags
  that clang accepts and ignores and gcc does not know, such as `-moutline`
  (mimalloc).
- `CompilerArgParser::parse_in(args, directory)` resolves relative include
  directories, sysroots and forced files against a compile command's
  directory and canonicalizes them; `=`-prefixed sysroot-relative
  directories are left alone. slate's compile-command reader
  (`crates/slate/src/compile_commands.rs`) passes each database entry
  through it once and carries the resulting `CompilerArgs`. It chooses the
  flavor from the compiler name (`gcc`, `*-gcc`, `gcc-N` as gcc, `cl` as
  msvc, anything else as clang; `translate-project --flavor` overrides) and
  the target from a `<triple>-gcc`/`<triple>-clang` prefix, before argv so
  an explicit `--target` still wins. A rejected argument is reported against
  its database entry, before any translation.

## Include search

Built by `CompilerArgs::search_paths`.

- Quoted: including file's directory, `-iquote`, `-I`, system. Angled:
  `-I`, system. `-I`/`-iquote` headers are user headers; the rest are
  system headers.
- System order: `-isystem`, compiler builtin headers, sysroot standard
  headers, `-idirafter`. `-nostdlibinc` drops only the sysroot standard
  headers; `-nostdinc` also drops the compiler builtin headers.
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

- `common_rules` and `strict_flex_arrays_rule`: flavor- and
  target-independent (mutually exclusive options, value ranges). Each is
  its own entry in `validate_rules`; nesting a pipeline repeats the
  `all rules failed:` prefix, as the flavor branches already do.
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
- Rules never re-parse strings or spellings, and never check whether a
  flavor has an option; that is `Opt::accepted_by`. A flavor branch holds
  value checks (clang's stack alignment is a power of two) and emulation
  gaps (gcc's `-fms-extensions` exists but is not emulated).

## Adding an option

1. Add an `Opt` definition and its flavors in `Opt::accepted_by`. A
   clang-only codegen `-m` flag is just an entry in
   `CLANG_CODEGEN_ONLY_M_FLAGS`.
2. Add value validation to `common_rules`, a flavor branch, or a target
   rule.
3. Add a FileCheck fixture for accepted and rejected configurations if it
   changes target or diagnostic behavior.
