# Rewriting

Slate rewrites the Rust AST after lowering chooses Rust representations. Rules
simplify local expressions, bindings, and control flow while preserving the
behavior of the translated C program. Both `translate` and `translate-project`
run the backend rewrites. Project translation also links across translation
units before applying the per-function rules.

<figure style="margin-inline: 0; text-align: center;">

![The lowered Rust AST passes through local rewrites and unsafe placement before Slate emits Rust source.](diagrams/rewrite-flow.svg)

</figure>

Use [Transforms](writing-a-transform.md) for rules that simplify C IR. Use
[Analyses](writing-a-query.md) for facts that let lowering choose a
representation such as a reference or slice. Use this chapter when the rewrite
depends on the Rust shape that lowering produced.

## Rewrite stages and rules

- [The Rewrite Engine](rewriting/engine.md). Follow a rule from its match to the
  next edit.
- [Bindings and Temporary Storage](rewriting/bindings.md). Simplify Rust
  bindings without changing storage or effect order.
- [Control Flow and Iteration](rewriting/control-flow.md). Clean up the control
  flow that Rust lowering emits.
- [Expressions and Idioms](rewriting/expressions.md). Use shorter Rust
  expressions with the same type and effects.
- [Unsafe Placement](rewriting/unsafe.md). Keep unsafe operations marked until
  the Rust shape is final.
- [Macros and Comments](rewriting/source-context.md). Preserve argument
  boundaries and source annotations during edits.
- [Adding and Testing a Rule](rewriting/testing.md). Trace the rule and check
  both generated Rust and runtime behavior.
