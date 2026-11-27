# IR

Slate uses IR to carry the meaning of a checked C program into analysis
and Rust lowering. Sema resolves types, conversions, layout, ABI, and
compiler-dependent behavior once. The IR writes those decisions as concrete
types, operations, and annotations that downstream stages can use directly.

The AST preserves how the author expressed the program. The IR preserves enough
of that structure to emit recognizable Rust, while making effects and operation
policies explicit. For example, a loop retains its test and increment, but the
test is now a boolean and its short circuit has an explicit branch. Source
context such as comments and macro origins travels alongside those semantics.

<figure style="margin-inline: 0; text-align: center;">

![Sema emits IR, transforms simplify it, fact analysis reads it, and Rust lowering consumes both the IR and facts.](../diagrams/parsing-ir.svg)

</figure>

## Read the IR

Run `slate emit-slate-ir input.c`. The examples use `x86_64-unknown-linux-gnu`
unless a different target is shown. IR excerpts omit unrelated output and
sometimes wrap long operations across lines.

For the full syntax and node descriptions, see the
[IR Reference](../reference/ir.md) and
[Printed IR Grammar](../reference/ir-grammar.md).

- [Reading a Module](ir/module.md). Read concrete types, binding IDs, and a
  complete operation.
- [Types and Layout](ir/types.md). Follow target-specific storage and type
  shapes.
- [Operations and Conversions](ir/operations.md). Read the computation type and
  arithmetic policy.
- [Places, Values, and Pointers](ir/places.md). Distinguish an address from the
  access it enables.
- [Control Flow and Evaluation Order](ir/control-flow.md). Follow branches,
  loops, and their effects.
- [Declarations and Initialization](ir/declarations.md). Read storage, linkage,
  and aggregate initialization.
- [Calls and ABI](ir/calls.md). See how a call carries its calling convention.
- [Atomics, Builtins, and Inline Assembly](ir/special-operations.md). Read the
  contracts for operations that need special lowering.
- [Source Context and Macro Recovery](ir/source-context.md). Follow the
  information used to recover source form.
- [Inspecting the IR Pipeline](ir/pipeline.md). Compare sema output with
  transformed IR and generated Rust.
