# Pointers and Lengths

Pointer analysis classifies the uses of each pointer and checks the values that
can reach it. Lowering uses the result to choose a raw pointer, reference,
optional reference, or slice.

## Follow the flows

1. Summarize each function. Local pointer copies share usage constraints such as
   reads, writes through the pointer, offsets, release, and escape.
2. Connect arguments to parameters and return values to call results. A write or
   escape in the callee constrains what its caller can borrow. A null source
   propagates toward the uses it can reach.
3. Check call sites for storage proofs. For example, a fixed array can supply
   its extent, and an aligned local can supply a single-object borrow if its
   address stays private. A pointer and count can become a slice when the
   supplied extent matches the count at every eligible call.

Two arguments that may alias cannot supply independent exclusive borrows.
Unknown calls, fixed C ABI signatures, and unproven alignment or bounds can keep
the raw representation. The analysis uses call effects when a callee might reach
the same storage through another path. See [Call Effects](effects.md).

`pointer::analyze` produces pointer facts stored in `BindingFacts`. For the Rust
signatures and call sites these facts produce, see
[Interprocedural Lifts](../lowering/interprocedural.md).
