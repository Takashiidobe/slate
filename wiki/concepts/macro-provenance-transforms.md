# Macro provenance in IR transforms

<!-- toc -->
- [The rule](#the-rule)
- [Check](#check)
- [Audit](#audit)
<!-- /toc -->

How the [slate-analysis](slate-analysis.md#transform-stage) passes treat
[macro provenance](ir/pipeline.md#macro-invocations): the
`Statement::MacroExpansion` regions and the `Module::macro_values` wraps that
`--merge` turns into `macro_rules!` templates. Macros carry intent, so keeping
them is the default; inlining one is a fallback, never a transform's goal.

## The rule

- **Analyses are transparent.** A fact pass (`counted_loops`, `pointer`,
  `def_use`) reads the resolved value tree. A macro wrap never blocks a fact.
- **Transforms stop at macro boundaries.** A transform does not rewrite across
  an expansion root or a hole boundary, and not inside a macro body outside its
  holes. Outside every expansion it is free. `truth_canonicalize` is the
  reference: `if (!IS_EMPTY(q))` stays `!IS_EMPTY!(q)`.
- **Required transforms record why.** A pass lowering needs for correctness
  (`structure_goto`, `labeled_blocks`, `do_while_zero`) that must change a
  region records a `Module::macro_rejections` reason for the expansion, as
  `effects_statements::normalize` does with `hoist_split_region`. Today none
  has to: they never enter a region (below).
- **Node identity.** A rebuild of the same node keeps its `NodeId`
  (`Span::with_value`, `Place { kind, ..place }`), so provenance follows it.
  `Span::derive` and `Place::new` mint a new id with no provenance. Moving a
  node keeps its id; moving it into or out of a region rejects the expansion.
- **Inline versus template is lowering's choice**, not a transform's. No pass
  re-runs on an expansion after it is inlined unless a fixture needs it.

## Check

`SLATE_CHECK_MACRO_STRUCTURE=1` (on in `project_merge`) runs
`frontend::macro_provenance::ProvenanceCheck` in `lower_parsed`. Before
`Slot::BeforeMacroKeep` it records, per expansion, every node id present in a
function body that `Module::macro_values` wraps and every
`Statement::MacroExpansion` region. After each pass, and after the macro-keep
strip (step `macro-keep`), it notes the first site that disappeared. At the end
an expansion that lost a site and has no `macro_rejections` entry fails the
unit with `MacroProvenanceLost`, naming the step, in this form:

```text
error: p.c: an IR transform dropped macro provenance
  dead-temporaries: #3 SQ lost its node 41 without a macro_rejections reason
```

It checks presence, not content: a pass that rewrites a wrapped node in place
under the same id, or moves a node into a region, is not caught. Span
`macro_nesting` is not checked.

On 2026-11-27 the `slate` profile and `merge` fixtures had no unexcused loss.
Every lost site belonged to an expansion macro-keep had already rejected
(`system_header`, `not_function_like`, `stringify_or_paste`, `variadic`,
`hoist_split_region`, ...), mostly const-fold folding object-like constants.

## Audit

Zones: **root/body** is a kept expansion's root value or its statements outside
holes; **hole** is an argument occurrence; **outside** is code in no
expansion.

| Pass | Slot | Root/body | Hole | Outside | Complies |
| --- | --- | --- | --- | --- | --- |
| structure-goto | after strip | not entered (`children` skips `MacroExpansion`); a goto in a macro makes keep reject it (`control_transfer`) | not entered | restructures; moves regions whole | yes |
| truth-canonicalize | after strip | blocked (expansion depth) | blocked (hole depth) | rewrites; keeps a protected macro value and negates around it | yes |
| inline-temps | before keep, after strip | a temp read inside a region is not inlined (`nested_depth`) | same | inlines | yes |
| const-fold | after strip | blocked (depth); comparisons with a wrapped operand are left | blocked | folds | yes |
| do-while-zero | after strip | not entered; a kept macro's `do { } while (0)` stays a loop for its template | not entered | unwraps | yes |
| labeled-blocks | after strip | not entered; `negation` keeps a protected value | not entered | rewrites | yes |
| dead-temporaries | after strip | blocked (depth) | blocked | removes | yes |
| decl-init | after strip | blocked (depth) | blocked | merges | yes |
| truth-locals | after strip | a local read in a region keeps its `int` type | same, place holes too | retypes | yes |

Every pass is stricter than the rule inside holes: rewriting there is allowed
but none does it. Relax a pass only when a fixture shows the missed rewrite.

See: `slate-8ge7.2.5`, `slate-8ge7.2.6`.
