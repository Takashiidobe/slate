# Compiler argument rules

Compiler arguments are parsed by the declarative `Opt` parser, then checked by
the rule pipeline. Add a rule to the narrowest bucket that owns its constraint.

`Opt` is the single source of truth for an option's canonical identity,
spellings, value form, and opposite spelling. The parser accepts compiler
spellings such as `-fwrapv`, `--fwrapv`, `-fno-wrapv`, and `--fno-wrapv`,
normalizes them to one option, and applies occurrences in order. A later
opposite spelling therefore replaces the earlier value before rules run.

Value options support both `=value` and a separate following argument. `-m`
options use the same option definitions; do not add a one-off parser for a
particular `-m` spelling. Downstream code receives the normalized typed value,
not the original spelling.

## Rule buckets

- `common_rules`: applies regardless of compiler flavor or target. Use this
  for shared constraints, such as mutually exclusive options.
- `flavor_rules`: select one branch with `Rules::branch`. Put GCC, Clang, and
  MSVC behavior in their respective branches. Do not repeat `is_gcc()` or
  `is_clang()` inside those branches.
- target rules: use the borrowed `TargetInfo` for architecture, ABI, and
  target-triple constraints. Keep target selection separate from flavor
  selection.
- `Rules::when`: use when an option is optional and its value rules apply only
  when the option is present. Do not make absence fail unless the option is
  conditionally required.
- `Rules::any`: use for genuine alternatives. Use `Rules::pipeline` or
  `Rules::all` when every constraint in a path must hold.

Shared option semantics belong in one common rule. A flavor branch should only
add the differences for that compiler. A target-dependent rule should report
the target and offending value in its diagnostic.

Leaf rules should use `Rule::validate` when the failure needs a useful or
dynamic message. Rule failures are `thiserror` errors and implement miette's
diagnostic interface; combinators preserve nested failures.

The parser owns option spelling, typed values, missing-value errors,
repetition, opposite handling, and `=` versus separate arguments. Rule code
should not parse strings or compiler spellings again. Rules can use option
presence to reject genuinely incompatible options, such as GCC's preferred
stack boundary and Clang's stack alignment. Once validated, both forms produce
the same normalized stack-alignment value for `TargetInfo`.

When adding a compiler option, add an `Opt` definition first, then place its
validation in `common_rules`, the appropriate flavor branch, or a target rule.
Add a FileCheck fixture for accepted and rejected configurations when the
option changes observable target or diagnostic behavior.
