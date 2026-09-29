# Configuration threading

How the compiler flavor, language standard, target, and command-line
options reach each stage, and where a flavor- or target-dependent rule
belongs. Read this before adding a behavior that differs between gcc,
clang, and MSVC, or between targets.

What individual flags mean lives elsewhere:
[compiler-arg-rules](compiler-arg-rules.md) (parsing and validation),
[compiler-flags](compiler-flags.md) (semantic contract, ISA flags),
[adding-a-target](adding-a-target.md) (registry entries).

## One owner: `Dialect`

Every per-run decision comes from one value, `Dialect` (`src/dialect.rs`).
It holds five things and nothing reaches past it to `CompilerArgs`:

| Field      | Type                | Set from                                                      |
| ---------- | ------------------- | ------------------------------------------------------------- |
| `flavor`   | `CompilerFlavor`    | `--flavor=gcc\|clang\|msvc` (default clang)                   |
| `standard` | `LanguageStandard`  | `-std=` (default gnu23)                                       |
| `features` | `StandardFeatures`  | derived: `StandardFeatures::for_compiler(standard, flavor, &target)` |
| `target`   | `TargetInfo`        | `-target` triple + ISA flags, then `CompilerOptions::effective_target` |
| `options`  | `CompilerOptions`   | operation flags, layout overrides, diagnostics, `-masm`, `-fcommon`, raw args |

`Dialect::new` applies `effective_target` (`-mlong-double-*`, preferred
stack alignment) *before* deriving features, so `dialect.target()` is the
effective target. Never use `CompilerArgs.target` after the `Dialect`
exists. The ISA is resolved earlier still, once, in
`CompilerArgParser::parse` (`TargetInfo.isa`), and the flavor/triple pair is
validated there too (`TargetInfo::for_triple_and_flavor`), so no later stage
sees an unsupported combination.

## The path

```text
argv
 └─ CompilerArgParser::parse            compiler_args.rs: Opt table + rules
     └─ CompilerArgs { flavor, standard, target, options,
                       defines, preprocessor_inputs, include dirs }
         ├─ search_paths()              compiler_headers.rs, sysroot.rs: per (target, flavor)
         └─ Dialect::new(flavor, standard, target, options)      main.rs
             └─ Parser::new(search, dialect)                     parser/mod.rs (owns it)
                 ├─ Preprocessor::new(&search, &dialect)         pp/mod.rs (borrows a clone)
                 │    configure(): predefine snapshot for (triple, flavor),
                 │    then ISA / long double / __ROUNDING_MATH__ / __STDC_VERSION__
                 ├─ biggest_alignment ← __BIGGEST_ALIGNMENT__ macro after preprocessing
                 └─ TranslationUnit { dialect, .. }              ast.rs
                     └─ Sema::new(&unit)
                         ├─ TypeResolver::with_names(unit, ..)   clones unit.dialect
                         │    every resolver (module lowering, resolve_type_module,
                         │    Sema::lower, the assertion checker) is built here,
                         │    so they cannot disagree
                         └─ Lowerer { types, context }
                              context = numeric::Context::for_dialect(&dialect):
                              a snapshot of target, features, overflow, FP region, asm dialect
                                  └─ ir::Module { target, .. }   flavor is NOT recorded
```

The IR records resolved effects and the target layout only. Flavor and
flags are inputs to sema, never to IR consumers (see the resolution
contract in [compiler-flags](compiler-flags.md)).

## Accessors by stage

| Stage                  | Flavor                                   | Target                          | Standard / features                     |
| ---------------------- | ---------------------------------------- | ------------------------------- | --------------------------------------- |
| Preprocessor           | `self.dialect.flavor()`                  | `self.dialect.target()`         | `self.dialect.standard()` / `.features()` |
| Parser                 | `self.flavor()`                          | `self.dialect().target()`       | `self.standard()` / `self.features()`   |
| `TypeResolver`         | `self.compiler_flavor()`                 | `self.dialect.target()`         | `self.standard()` / `self.features()`   |
| Lowerer (`module.rs`, `function.rs`, `expression.rs`) | `self.types.compiler_flavor()` | `self.context.target` | `self.types.features()`     |
| Checker (`assertion.rs`) | `self.types.compiler_flavor()` or `self.unit.dialect.flavor()` | via `self.types` | `self.types.features()` |
| Constant folding (`fold.rs`) | `Env.flavor` (passed in)           | —                               | —                                       |
| Diagnostics            | `Warning::default_severity(standard, flavor)` | —                          | —                                       |

`numeric::Context` is copied once per translation unit. Target and
features never change inside a unit; only `context.region` (FP pragmas)
does.

## Where a new rule goes

Pick the narrowest table that already expresses the question. An ad hoc
`flavor == CompilerFlavor::Gcc` belongs at a consumer only when no table
fits.

| Question                                              | Owner                                                                 |
| ----------------------------------------------------- | --------------------------------------------------------------------- |
| Is this macro predefined, and to what?                | Predefine snapshot (`src/predefines/`, via `TargetProfile::predefines(flavor)`); computed ones in `Preprocessor::configure` |
| Does this keyword / grammar exist in this mode?       | A `StandardFeatures` field (flavor × standard × target), read by lexer/parser |
| Does this attribute name exist here? (`__has_attribute`, unknown-attribute warning) | `Gate` table in `src/attribute_support.rs` (per flavor, per target family/OS) |
| How is this attribute's argument list parsed?         | `parse_attribute_value` in `src/parser/attributes.rs` — flavor-blind; parse the union of forms |
| Where does this attribute apply / is it ignored?      | `declaration_use` / `inapplicable` in `src/sema/attributes.rs` — flavor-blind |
| What does the attribute *do* per compiler?            | Its consumer: `symbol_attributes`/`function_symbol` (`sema/module.rs`), `record_function` (`sema/function.rs`), layout requests |
| Layout / alignment / atomic storage                   | `TargetInfo` + `TypeResolver::atomic_layout`, `effective_alignment` ([c-type-layer](c-type-layer.md)) |
| Argument / return classification                      | `AbiClassifier` (`sema/abi.rs`), reads flavor through its `&TypeResolver` |
| Builtin exists / its signature                        | `sema/builtins.rs` (`clang_builtin(name, flavor)`, gcc-only registry) |
| Constant-evaluation corner (shifts, float→int)        | `fold.rs` `Env.flavor`                                                |
| Default warning vs error                              | `Warning::default_severity` ([diagnostic-severity](diagnostic-severity.md)) |
| Include directories                                   | `compiler_headers::include_paths`, `sysroot::include_paths_at` (target, flavor) |

The parse and attribute-applicability layers are deliberately
flavor-blind: they accept the union of every compiler's forms, and the
consumer in sema decides what that form means for this flavor (or rejects
it). Adding a flavor branch to the parser for an attribute argument form
is usually the wrong place.

## Gotcha: checker and lowering both apply a rule

Sema runs two passes over declarations. The checker (`sema/assertion.rs`)
reports user errors; lowering (`sema/module.rs`, `sema/function.rs`)
re-derives the same facts and wraps any failure in
`ResolveError::checked`, which turns `Rejected` into `Internal` because
the checker should already have caught it. A flavor-dependent rule must
therefore change in both places, or live in one rule function both call
(`function_symbol`, `declared_linkage`). If only lowering learns the new
behavior, the checker still rejects; if only the checker does, lowering
fails with an internal error.

Example: "conflicting `always_inline` and `noinline`" is enforced in
`record_function` (lowering) and in the checker's declaration rules
separately.

## Gotcha: the oracle decides per flavor

Before encoding a behavior, check what each oracle does: gcc and clang are
native, MSVC is `tools/cl.exe` ([msvc-oracle](msvc-oracle.md)). They
often disagree (which of two conflicting attributes wins, whether a bare
form is an error or ignored). Model each flavor's answer: only
being stricter than the oracle is a bug. The fixture
directory sets the flavor and target for a test
([fixture-layout](fixture-layout.md)), so a per-flavor behavior needs one
fixture per flavor directory.
