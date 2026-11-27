# Transforms

Transforms simplify IR before [fact analysis](writing-a-query.md) and
Rust lowering. Use `emit-slate-ir` to see what a pass changes.

```bash
slate emit-slate-ir input.c
slate emit-slate-ir --passes=const-fold input.c
slate translate input.c
```

The first command prints IR with no optional transforms. Use a comma-separated
pass list or `--passes=all` to inspect more than one pass. Each pass page shows
the function emitted by `slate translate`, which runs the full pipeline. The
rewrite blocks isolate the named pass and use shorthand IR. Uppercase letters
stand for expressions or statement lists, and `continue L` and `break L` name
their exact targets.

## What the macro slots see

Sema emits the expanded C program. It also records which statement regions came
from a macro expansion and which values came from the macro body or its
arguments. Slate uses that structure to decide which expansions it can emit as
Rust macro calls.

- **Before macro keep.** All recorded macro regions are still present.
  `inline-temps` runs here so the keep decision sees eligible temporary chains
  already simplified. A temporary move must respect expansion boundaries and
  argument holes.
- **After macro strip.** Slate removes the wrappers for expansions it will emit
  inline. Their statements and values are now ordinary IR and can be rewritten.
  Wrappers and argument holes for kept expansions still need to match the Rust
  macro template. `inline-temps` runs again for code exposed by stripping.
- **After transforms.** Fact analysis reads the resulting IR, so lowering
  receives facts about the bindings and values that survived the passes.

`PASSES` in `crates/slate-analysis/src/transform/mod.rs` defines the order
within each slot. The pass list follows the order after macro strip.

## Add a transform fixture

Put a small C input in `crates/slate/tests/fixtures.transforms/<passes>/`. The
directory name selects the passes for the generated IR checks. Include an
example that rewrites and one that reaches the guard you want to preserve. Use a
Slate differential fixture when you need to check the Rust output or runtime
behavior. See [Testing](testing.md).

## Passes

- [Structure Goto](transforms/structure-goto.md)
- [Truth Canonicalize](transforms/truth-canonicalize.md)
- [Inline Temps](transforms/inline-temps.md)
- [Const Fold](transforms/const-fold.md)
- [Do While Zero](transforms/do-while-zero.md)
- [Labeled Blocks](transforms/labeled-blocks.md)
- [Dead Temporaries](transforms/dead-temporaries.md)
- [Decl Init](transforms/decl-init.md)
- [Truth Locals](transforms/truth-locals.md)
