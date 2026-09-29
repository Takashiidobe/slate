# Configuration threading

<!-- toc -->
- [`Dialect` owns it](#dialect-owns-it)
- [Accessors](#accessors)
- [Where a rule goes](#where-a-rule-goes)
- [Checker vs lowering](#checker-vs-lowering)
<!-- /toc -->

How flavor, standard, target, and flags reach each stage, and where a
per-flavor or per-target rule belongs. The meaning of individual flags is
in [compiler-arg-rules](compiler-arg-rules.md),
[compiler-flags](compiler-flags.md), and
[adding-a-target](adding-a-target.md).

## `Dialect` owns it

`Dialect` (`src/dialect.rs`) holds `flavor`, `standard`, `features`
(`StandardFeatures::for_compiler(standard, flavor, &target)`), `target`,
and `options` (`CompilerOptions`).

- `dialect.target()` is the effective target: `Dialect::new` has already
  applied `-mlong-double-*` and the stack alignment. Never read
  `CompilerArgs.target` after that point.
- The ISA (`TargetInfo.isa`) is resolved once, in
  `CompilerArgParser::parse`. The flavor/triple pair is validated there
  too (`TargetInfo::for_triple_and_flavor`).
- The IR records the target, but not the flavor or flags.

```text
CompilerArgParser::parse → CompilerArgs
  ├─ search_paths()                 compiler_headers.rs, sysroot.rs (target, flavor)
  └─ Dialect::new                   main.rs
      └─ Parser (owns)              parser/mod.rs
          ├─ Preprocessor (clone)   pp/mod.rs configure(): snapshot for (triple, flavor) + computed macros
          ├─ biggest_alignment      read from __BIGGEST_ALIGNMENT__ after preprocessing
          └─ TranslationUnit.dialect
              └─ TypeResolver::with_names   clones it; the only constructor for every resolver
                  └─ Lowerer.context        numeric::Context::for_dialect snapshot
```

## Accessors

| Stage | Flavor | Target | Standard / features |
| --- | --- | --- | --- |
| Preprocessor | `self.dialect.flavor()` | `self.dialect.target()` | `self.dialect.standard()` / `.features()` |
| Parser | `self.flavor()` | `self.dialect().target()` | `self.standard()` / `self.features()` |
| `TypeResolver` | `self.compiler_flavor()` | `self.dialect.target()` | `self.standard()` / `self.features()` |
| Lowerer, checker | `self.types.compiler_flavor()` | `self.context.target` (Lowerer) | `self.types.features()` |
| `fold.rs` | `Env.flavor` | — | — |

Only `context.region` (FP pragmas) changes within a translation unit.

## Where a rule goes

| Question | Owner |
| --- | --- |
| Macro predefined, and its value | `src/predefines/` snapshot; computed macros in `Preprocessor::configure` |
| Keyword or grammar exists in this mode | `StandardFeatures` field |
| Attribute name exists (`__has_attribute`, unknown-attribute warning) | `Gate` table, `src/attribute_support.rs` |
| Attribute argument forms | `parse_attribute_value` (`parser/attributes.rs`); flavor-blind, accepts the union |
| Where an attribute applies | `declaration_use` / `inapplicable` (`sema/attributes.rs`); flavor-blind |
| What an attribute means for this flavor | its checker rule, then its lowering consumer (`symbol_attributes`, `record_function`, layout requests) |
| Layout, alignment, atomic storage | `TargetInfo` + `TypeResolver` ([c-type-layer](c-type-layer.md)) |
| Argument classification | `AbiClassifier` (`sema/abi.rs`) |
| Builtins | `sema/builtins.rs` |
| Constant-evaluation corners | `fold.rs` |
| Default warning vs error | `Warning::default_severity(standard, flavor)` ([diagnostic-severity](diagnostic-severity.md)) |

## Checker vs lowering

The checker validates and lowering trusts it ([architecture](architecture.md)).
Lowering wraps its failures in `ResolveError::checked`, which turns
`Rejected` into `Internal`. A per-flavor rule therefore belongs in the
checker, or in a rule function that both call (`function_symbol`,
`declared_linkage`). Some rules are still duplicated. For example, the
`always_inline`/`noinline` conflict is checked in both `record_function`
and `assertion.rs`. Change both copies, or better, merge them.

Oracles disagree often. Check all three before encoding a rule. The
fixture directory picks the flavor ([fixture-layout](fixture-layout.md)),
so a rule that differs per flavor needs one fixture per flavor.
