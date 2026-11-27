# Expressions and Conversions

Rust arithmetic uses the types of its operands. C often promotes or converts
them first. Sema records those conversions so Rust lowering can reproduce the
computation at the right width.

**C input**

```c
int sum(unsigned char a, unsigned char b) {
    return a + b;
}
```

**IR excerpt**

```ir
fn %0 @sum(%1 a: u8, %2 b: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
    return add<i32, overflow=ub>(
        reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))),
        reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%2)))
    );
}
```

Both operands carry `reason=promotion`. The addition itself is `add<i32>`.

On a target where `int` can represent every `unsigned char`, the checker
promotes both operands to `int`. The IR has two widening conversions and an
`int` addition. Adding in eight bits and widening the result could lose the
carry. For example, `200 + 100` must produce `300` in this function.

`TypeResolver::typed` records expression types and whether a node denotes a
place. The checker also records operand conversion steps and the computation
type. IR emission uses these results to build explicit operations such as reads,
conversions, and arithmetic. It keeps the distinction between a written cast and
an implicit conversion so transforms can preserve the source's intent.

The same typer handles operands used for type queries, such as ordinary `sizeof`
operands. It can determine their types without emitting code that evaluates
them. Start with `sema/typer.rs` and `sema/ctype/`.
