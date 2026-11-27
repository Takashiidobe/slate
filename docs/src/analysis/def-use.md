# Def-Use

Def-use records where each IR binding is initialized, read, written, or exposed
through its address. Slate builds a `DefUse` index in one walk of a function
body. Queries use the binding's ID, so two locals spelled `x` in different
scopes have separate entries.

The index groups accesses to a binding. To decide whether a write feeds a read,
a consumer also checks the region between them. A branch or jump may change
which definition reaches a use.

## Collect accesses

The walker numbers statements as it enters them, before visiting their children.
Every statement subtree occupies a contiguous range. Loop and switch bodies have
recorded ranges, so a consumer can check whether a binding is touched inside a
particular body.

| Access                   | What the walker records                                                                                                                        |
| ------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------- |
| `Initialize`             | The value supplied when a binding is declared.                                                                                                 |
| `Read`, `ReadProjection` | A read of the binding or of a projected part, such as an aligned field.                                                                        |
| `Write`                  | An assignment or update. An update such as `x += 1` records a read as well.                                                                    |
| `Argument`               | An address or fixed array passed directly as a call argument. Lowering resolves the kind of borrow from the selected parameter representation. |
| `Escape`                 | An opaque place or address use, such as storing the address, exposing it to assembly, or capturing it by reference.                            |
| `Capture`                | A use with no replaceable value expression, such as a VLA extent named by a type or a by-value block capture.                                  |

The walker stops at inner function bodies. At the literal or definition it
records uses of captured outer bindings. Each inner body gets its own walk.

## Read a chain

```c
int bump(int a) {
    int x = a;
    x += 1;
    return x;
}
```

```text
statement 1   Read(a), Initialize(x)
statement 2   Read(x), Write(x)
statement 3   Read(x)
```

In this straight-line region, the initializer feeds the update's read, and the
update feeds the return's read. The write after initialization means the lowerer
emits `let mut x`.

Two branches that assign `x` produce two write entries. Their position numbers
describe where the writes appear in the IR. Which write reaches a later read
depends on the branch taken. Consumers that need execution order check a
straight-line region for labels, cases, and jumps before relying on positions.
For example, a backward jump can make a later write feed an earlier read.

## Use the facts

- **Mutability.** Analysis records writes after initialization, opaque place
  uses, and direct address or array arguments in `BindingFacts::mutability`.
  Lowering uses those facts and the chosen callee signature to decide whether
  the Rust binding needs `mut`. See [remove_mut](../lowering/remove_mut.md).
- **Declaration initialization.** Before moving a declaration to its first
  assignment, the transform checks that the intervening region leaves the
  binding untouched. An earlier call to `prepare(&x)` needs the storage in its
  original scope. See [Decl Init](../transforms/decl-init.md).
- **Temporary uses.** A single-read query requires one replaceable value read. A
  use in a VLA extent or capture keeps the temporary alive even when the
  ordinary expression walk sees only one read.
