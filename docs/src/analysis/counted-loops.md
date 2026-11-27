# Counted Loops

Analysis recognizes loops whose index runs through a Rust-style range.

```c
for (int i = start; i < end; ++i)
    consume(i);
```

The initializer supplies the start, the comparison supplies the end, and the
increment must write exactly `i + 1`. The index must be a loop-local integer
whose address stays private. The body must leave the index and the bindings used
by the bound unchanged. This lets lowering evaluate the bound once.

An inclusive bound must be a constant below the index type's maximum.
`i <= UINT_MAX` lets an unsigned C index wrap and continue, while an inclusive
Rust range stops. Calls in the test, a global bound, or a jump into the body can
also block the proof.

`counted_loops::analyze` records the index, start, end, and inclusivity.
Lowering queries `UnitFacts::counted_loop` by the loop's ID.
